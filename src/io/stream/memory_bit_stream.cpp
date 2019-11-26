#include <azgra/io/stream/memory_bit_stream.h>

namespace azgra::io::stream
{

    /******************************************************************************************************************************
    * OutMemoryBitStream implementation
    ****************************************************************************************************************************/
    OutMemoryBitStream::OutMemoryBitStream(size_t valueEncodeBitCount)
    {
        bitBuffer = 0;
        bitBufferSize = 0;
        memoryBufferIndex = 0;
        this->valueEncodeBitCount = valueEncodeBitCount;
    }

    OutMemoryBitStream::~OutMemoryBitStream()
    {
        flush_bit_buffer();
        bitBuffer = 0;
        memoryBufferIndex = 0;
        bitBufferSize = 0;
    }

    void OutMemoryBitStream::resize_for_raw_write(const size_t size)
    {
        buffer.resize(size);
    }

    void OutMemoryBitStream::operator<<(const bool &bit)
    {
        internal_write_bit(bit, true);
    }

//    void OutMemoryBitStream::write_byte(const byte &b)
//    {
//        if (bitBufferSize == 0)
//        {
//            bitBuffer = b;
//            bitBufferSize = 8;
//            internal_flush_bit_buffer(true);
//        }
//        else
//        {
//            for (unsigned int i = 7; i >= 0; --i)
//            {
//                const bool bit = (b & (static_cast<byte>(1) << i));
//                internal_write_bit(bit, true);
//            }
//        }
//    }

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


/******************************************************************************************************************************
  * InMemoryBitStream implementation
  ****************************************************************************************************************************/

    InMemoryBitStream::InMemoryBitStream(const ByteArray *buffer, size_t bufferPosition, size_t valueEncodeBitCount)
    {
        memoryBuffer = buffer;
        memoryBufferPosition = bufferPosition;
        bitBufferSize = 0;
        bitBuffer = 0;
        this->valueEncodeBitCount = valueEncodeBitCount;
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

    size_t bits_required(size_t maxValue)
    {
        size_t maxValueForBitCount = 0;
        for (size_t bitCount = 2; bitCount < MAX_BIT_COUNT; bitCount++)
        {
            maxValueForBitCount = 0;
            for (size_t i = 0; i < bitCount; i++)
            {
                maxValueForBitCount |= 1 << i;
            }
            if (maxValue <= maxValueForBitCount)
                return bitCount;
        }

        always_assert(false && "Max value is too large");
        return 0;
    }

    size_t bytes_required(size_t maxValue)
    {
        size_t bits = bits_required(maxValue);
        size_t bytes = (bits + 7) / 8;
        return bytes;
    }

} // namespace azgra