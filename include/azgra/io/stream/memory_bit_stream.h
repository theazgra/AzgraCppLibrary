#pragma once

#include <azgra/azgra.h>

namespace azgra::io::stream
{
    constexpr size_t MAX_BIT_COUNT = 255;
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
        size_t memoryBufferIndex = 0;
        // Bit buffer.
        byte bitBuffer;
        // Actual size of bit buffer.
        byte bitBufferSize = 0;
        // Number of bits to which value will be encoded when `write_value` is called.
        size_t valueEncodeBitCount = 0;

        void internal_write_bit(const bool &bit, const bool &alloc);

        void internal_flush_bit_buffer(const bool &alloc);

    public:
        OutMemoryBitStream(size_t valueEncodeBitCount = 1);

        ~OutMemoryBitStream();

        // Resize the memory buffer for `write_value_no_alloc` and `write_bit_no_alloc`
        void resize_for_raw_write(const size_t size);

        // Write bit to memory stream.
        void write_bit(const bool &bit);

        // Write bit without resizing buffer. Buffer must be allocated using `resize_for_raw_write` !
        void write_bit_no_alloc(const bool &bit);

        // Write bit to memory stream.
        void operator<<(const bool &bit);

        // If flush buffer is not flushed, flush it to main buffer and clear it.
        void flush_bit_buffer();

        // Get buffer of written bits, which may not be flushed.
        ByteArray get_buffer() const;

        // Get buffer of written bits, which is flushed.
        ByteArray get_flushed_buffer();

        // Get buffer of written bits, which is flushed. For allocated buffer.
        ByteArray get_flushed_buffer_no_alloc();

        template<typename T>
        void write_value(const T &value)
        {
            // Lets do it Little-Endian way, start with LSB and move to MSB.
            bool bit;
#if LSB_FIRST
            for (size_t bitPos = 0; bitPos < valueEncodeBitCount; bitPos++)
            {
                bit = value & (1 << bitPos);
                write_bit(bit);
            }
#else
            const size_t valueSize = sizeof(T) * 8;
            // Start with MSB.
            for (long bitPos = (valueSize - 1); bitPos >= 0; bitPos--)
            {
                bit = value & (1 << bitPos);
                write_bit(bit);
            }
#endif
        }

        // Write value without resizing buffer. Buffer must be allocated using `resize_for_raw_write` !
        template<typename T>
        void write_value_no_alloc(const T &value)
        {
            // Lets do it Little-Endian way, start with LSB and move to MSB.
            bool bit;
#if LSB_FIRST
            for (size_t bitPos = 0; bitPos < valueEncodeBitCount; bitPos++)
            {
                bit = value & (1 << bitPos);
                write_bit_no_alloc(bit);
            }
#else
            // Start with MSB.
            for (azgra::i32 bitPos = (valueEncodeBitCount - 1); bitPos >= 0; bitPos--)
            {
                bit = value & (1 << bitPos);
                write_bit_no_alloc(bit);
            }
#endif
        }

        template<typename T>
        void write_bytes_no_alloc(const T &value, const azgra::i32 &byteCount)
        {
#if LSB_FIRST
            for (size_t byteIndex = 0; byteIndex < byteCount; byteIndex++)
            {
                *(buffer.data() + memoryBufferIndex++) = *(((byte *)&value) + byteIndex);
            }
#else
            for (azgra::i32 byteIndex = (byteCount - 1); byteIndex >= 0; byteIndex--)
            {
                *(buffer.data() + memoryBufferIndex++) = *(((byte *) &value) + byteIndex);
            }
#endif
        }
    };

// Class alowing to read invidual bits from memory buffer.
    class InMemoryBitStream
    {
    private:
        // Memory buffer.
        const ByteArray *memoryBuffer = nullptr;
        // Position in memory buffer.
        size_t memoryBufferPosition = 0;
        // Bit buffer.
        byte bitBuffer = 0;
        // Current bit buffer size.
        byte bitBufferSize = 0;
        // Number of bits to consume when reading value by `read_value`.
        size_t valueEncodeBitCount;

        // Read byte into bit buffer.
        void read_byte_to_bit_buffer();

    public:
        InMemoryBitStream(const ByteArray *buffer, size_t bufferPosition = 0, size_t valueEncodeBitCount = 1);

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
#if LSB_FIRST
            for (size_t bitPos = 0; bitPos < valueEncodeBitCount; bitPos++)
            {
                bit = read_bit();
                result |= (bit << bitPos);
            }
#else
            const size_t valueSize = sizeof(T) * 8;
            for (long bitPos = (valueSize - 1); bitPos >= 0; bitPos--)
            {
                bit = read_bit();
                result |= (bit << bitPos);
            }
#endif
            return result;
        }

        template<typename T>
        T read_value(const azgra::i32 &byteCount)
        {
            always_assert(memoryBufferPosition + (byteCount - 1) < memoryBuffer->size());
            T result = 0;
            // bitBuffer = (*memoryBuffer)[memoryBufferPosition++];
#if LSB_FIRST
            for (size_t byteIndex = 0; byteIndex < byteCount; byteIndex++)
            {
                result |= (memoryBuffer->operator[](memoryBufferPosition++)) << (byteIndex * 8);
            }
#else
            for (azgra::i32 byteIndex = (byteCount - 1); byteIndex >= 0; byteIndex--)
            {
                result |= (memoryBuffer->operator[](memoryBufferPosition++)) << (byteIndex * 8);
            }
#endif
            return result;
        }
    };

// Find number of bits required to encode value of `maxValue`.
    size_t bits_required(size_t maxValue);

// Find number of whole bytes required to encode value of `maxValue`.
    size_t bytes_required(size_t maxValue);
} // namespace azgra