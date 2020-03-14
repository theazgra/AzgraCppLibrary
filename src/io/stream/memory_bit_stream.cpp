#include <azgra/io/stream/memory_bit_stream.h>

namespace azgra::io::stream
{

    /******************************************************************************************************************************
    * OutMemoryBitStream implementation
    ****************************************************************************************************************************/
    OutMemoryBitStream::OutMemoryBitStream()
    {
        bitBuffer = 0;
        bitBufferSize = 0;
        memoryBufferIndex = 0;
    }

    OutMemoryBitStream::~OutMemoryBitStream()
    {
        flush_bit_buffer();
        bitBuffer = 0;
        memoryBufferIndex = 0;
        bitBufferSize = 0;
    }

    void OutMemoryBitStream::resize_for_raw_write(const std::size_t size)
    {
        buffer.resize(size);
    }

    void OutMemoryBitStream::operator<<(const bool &bit)
    {
        internal_write_bit(bit, true);
    }

    void OutMemoryBitStream::internal_write_bit(const bool &bit, const bool &alloc)
    {
        ++bitBufferSize;
        if (bit)
        {
            bitBuffer |= (1 << (8 - bitBufferSize));
        }

        if (bitBufferSize == 8)
        {
            internal_flush_bit_buffer(alloc);
        }
    }

    void OutMemoryBitStream::write_bit(const bool &bit)
    {
        internal_write_bit(bit, true);
    }

    void OutMemoryBitStream::write_bit_no_alloc(const bool &bit)
    {
        internal_write_bit(bit, false);
    }

    void OutMemoryBitStream::flush_bit_buffer()
    {
        internal_flush_bit_buffer(true);
    }

    void OutMemoryBitStream::internal_flush_bit_buffer(const bool &alloc)
    {
        if (bitBufferSize > 0)
        {
            if (alloc)
            {
                buffer.push_back(bitBuffer);
                ++memoryBufferIndex;
            }
            else
            {
                buffer[memoryBufferIndex++] = bitBuffer;
            }

            bitBuffer = 0;
            bitBufferSize = 0;
        }
    }

    ByteArray OutMemoryBitStream::get_buffer() const
    {
        return buffer;
    }

    ByteArray OutMemoryBitStream::get_flushed_buffer()
    {
        internal_flush_bit_buffer(true);
        return buffer;
    }

    ByteArray OutMemoryBitStream::get_flushed_buffer_no_alloc()
    {
        internal_flush_bit_buffer(false);
        return buffer;
    }

    void OutMemoryBitStream::write_replicated_bit(const bool &bit, const std::size_t count)
    {
        for (std::size_t i = 0; i < count; ++i)
        {
            internal_write_bit(bit, true);
        }
    }


/******************************************************************************************************************************
  * InMemoryBitStream implementation
  ****************************************************************************************************************************/

    InMemoryBitStream::InMemoryBitStream(const ByteArray *buffer, std::size_t bufferPosition)
    {
        memoryBuffer = buffer;
        memoryBufferPosition = bufferPosition;
        bitBufferSize = 0;
        bitBuffer = 0;
    }

    InMemoryBitStream::~InMemoryBitStream()
    {
        memoryBuffer = nullptr;
        memoryBufferPosition = 0;
        bitBufferSize = 0;
        bitBuffer = 0;
    }

    void InMemoryBitStream::read_byte_to_bit_buffer()
    {
        if (memoryBufferPosition < memoryBuffer->size())
        {
            bitBuffer = memoryBuffer->operator[](memoryBufferPosition++);
            bitBufferSize = 8;
        }
        else
        {
            always_assert(false && "Out of memory in buffer");
        }
    }

    bool InMemoryBitStream::read_bit()
    {
        if (bitBufferSize == 0)
        {
            read_byte_to_bit_buffer();
        }

        --bitBufferSize;
        bool result = bitBuffer & (1 << bitBufferSize);
        return result;
    }

    void InMemoryBitStream::operator>>(bool &bit)
    {
        bit = read_bit();
    }

    bool InMemoryBitStream::can_read() const
    {
        return ((bitBufferSize > 0) || (memoryBufferPosition < memoryBuffer->size()));
    }

    std::size_t bits_required(std::size_t maxValue)
    {
        std::size_t maxValueForBitCount = 0;
        for (std::size_t bitCount = 2; bitCount < MAX_BIT_COUNT; bitCount++)
        {
            maxValueForBitCount = 0;
            for (std::size_t i = 0; i < bitCount; i++)
            {
                maxValueForBitCount |= 1 << i;
            }
            if (maxValue <= maxValueForBitCount)
                return bitCount;
        }

        always_assert(false && "Max value is too large");
        return 0;
    }

    std::size_t bytes_required(std::size_t maxValue)
    {
        std::size_t bits = bits_required(maxValue);
        std::size_t bytes = (bits + 7) / 8;
        return bytes;
    }

} // namespace azgra