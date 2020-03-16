#pragma once

#include <azgra/azgra.h>

namespace azgra::io::stream
{
    constexpr std::size_t MAX_BIT_COUNT = 255;
// if MSB_FIRST is true, than MSB is written first to the stream when writing value.
#define LSB_FIRST 0

    /**
     * @brief Class allowing to write invidual bits to memory buffer.
     * It is important to note that now there are two ways how one can read/write from/into those streams.
     * There is bit access and byte access (write_bytes_no_alloc, read_value(byteCount)), one must not mix those.
     * If there would be call to both of those the internal buffer could be mixed and value would be corrupted.
     */
    class OutMemoryBitStream
    {
    private:
        // Memory buffer.
        ByteArray buffer;
        // Index to the next write in memory buffer.
        std::size_t memoryBufferIndex = 0;
        // Bit buffer.
        byte bitBuffer;
        // Actual size of bit buffer.
        byte bitBufferSize = 0;

        void internal_write_bit(const bool &bit, const bool &alloc);

        void internal_flush_bit_buffer(const bool &alloc);

        /**
         * Copy bytes to aligned buffer.
         * @param bytes Bytes to copy.
         */
        void copy_bytes(const ByteArray &bytes);

        template<typename T>
        void internal_write_value(const T &value, const bool alloc)
        {
            // Lets do it Little-Endian way, start with LSB and move to MSB.
            bool bit;
            const std::size_t valueSize = sizeof(T) * 8;
            // Start with MSB.
            for (long bitPos = (valueSize - 1); bitPos >= 0; bitPos--)
            {
                assert(bitPos >= 0);
                bit = value & (static_cast<azgra::u32>(1) << static_cast<azgra::u32> (bitPos));
                if (alloc)
                    write_bit(bit);
                else
                    write_bit_no_alloc(bit);
            }
        }

        template<typename T>
        void internal_write_value(const T &value, const azgra::byte valueBitCount, const bool alloc)
        {
            // Lets do it Little-Endian way, start with LSB and move to MSB.
            bool bit;
            const long valueSize = valueBitCount;
            // Start with MSB.
            for (long bitPos = (valueSize - 1); bitPos >= 0; bitPos--)
            {
                bit = value & (static_cast<azgra::u32>(1) << static_cast<azgra::u32> (bitPos));
                if (alloc)
                    write_bit(bit);
                else
                    write_bit_no_alloc(bit);
            }
        }

    public:
        explicit OutMemoryBitStream();

        ~OutMemoryBitStream();

        // Resize the memory buffer for `write_value_no_alloc` and `write_bit_no_alloc`
        void resize_for_raw_write(const std::size_t size);

        // Write bit to memory stream.
        void write_bit(const bool &bit);

        //TODO(Moravec): Add no-alloc version

        // Write byte to aligned stream.
        void write_aligned_byte(const azgra::byte &byte);

        // Write bit without resizing buffer. Buffer must be allocated using `resize_for_raw_write` !
        void write_bit_no_alloc(const bool &bit);

        // Write bit to memory stream.
        void operator<<(const bool &bit);

        // If flush buffer is not flushed, flush it to main buffer and clear it.
        void flush_bit_buffer();

        // Get buffer of written bits, which may not be flushed.
        [[nodiscard]] ByteArray get_buffer() const;

        // Get buffer of written bits, which is flushed.
        [[nodiscard]] ByteArray get_flushed_buffer();

        // Get buffer of written bits, which is flushed. For allocated buffer.
        ByteArray get_flushed_buffer_no_alloc();

        // Write the bit count times to the stream.
        void write_replicated_bit(const bool &bit, const std::size_t count);

        /**
         * Copy buffer, which must be aligned, to the receiving stream, which must be aligned.
         * Current buffer is then reset.
         * @param stream Receiving stream.
         */
        void copy_aligned_buffer_and_reset(OutMemoryBitStream &stream);

        template<typename T>
        void write_value(const T &value)
        {
            internal_write_value(value, true);
        }

        template<typename T>
        void write_value(const T &value, const azgra::byte valueBitCount)
        {
            internal_write_value(value, valueBitCount, true);
        }

        template<typename T>
        void write_value_no_alloc(const T &value, const azgra::byte valueBitCount)
        {
            internal_write_value(value, valueBitCount, false);
        }


        // Write value without resizing buffer. Buffer must be allocated using `resize_for_raw_write` !
        template<typename T>
        void write_value_no_alloc(const T &value)
        {
            internal_write_value(value, false);
        }

        template<typename T>
        void write_bytes_no_alloc(const T &value, const azgra::i32 &byteCount)
        {
            for (azgra::i32 byteIndex = (byteCount - 1); byteIndex >= 0; byteIndex--)
            {
                *(buffer.data() + memoryBufferIndex++) = *(((byte *) &value) + byteIndex);
            }
        }
    };

    // Class allowing to read individual bits from memory buffer.
    class InMemoryBitStream
    {
    private:
        // Memory buffer.
        const ByteArray *memoryBuffer = nullptr;
        // Position in memory buffer.
        std::size_t memoryBufferPosition = 0;
        // Bit buffer.
        byte bitBuffer = 0;
        // Current bit buffer size.
        byte bitBufferSize = 0;

        // Read byte into bit buffer.
        void read_byte_to_bit_buffer();

    public:
        explicit InMemoryBitStream(const ByteArray *buffer, std::size_t bufferPosition = 0);

        ~InMemoryBitStream();

        // Read one bit from memory buffer.
        bool read_bit();

        // Read one bit from memory buffer.
        void operator>>(bool &bit);

        // Check wheter atleast one bit can be read.
        bool can_read() const;

        template<typename T>
        T read_value()
        {
            bool bit;
            T result = 0;
            const std::size_t valueSize = sizeof(T) * 8;
            for (long bitPos = (valueSize - 1); bitPos >= 0; bitPos--)
            {
                bit = read_bit();
                result |= (bit << bitPos);
            }
            return result;
        }

        template<typename T>
        T read_value(const azgra::byte bitCount)
        {
            bool bit;
            T result = 0;
            const long valueSize = bitCount;
            for (long bitPos = (valueSize - 1); bitPos >= 0; bitPos--)
            {
                bit = read_bit();
                result |= (bit << bitPos);
            }
            return result;
        }
    };

    // Find number of bits required to encode value of `maxValue`.
    std::size_t bits_required(std::size_t maxValue);

    // Find number of whole bytes required to encode value of `maxValue`.
    std::size_t bytes_required(std::size_t maxValue);
} // namespace azgra