#include <azgra/io/stream/out_binary_buffer_stream.h>

namespace azgra::io::stream
{
    void OutBinaryBufferStream::ensure_capacity(const size_t &writeSize)
    {

    }

    OutBinaryBufferStream::OutBinaryBufferStream(const size_t initialBufferSize)
    {
        buffer.resize(initialBufferSize);
        bufferPtr = 0;
        isOpen = true;
    }

    OutBinaryBufferStream::~OutBinaryBufferStream()
    {
        buffer.clear();
        bufferPtr = 0;
        isOpen = false;
    }

    size_t OutBinaryBufferStream::get_position()
    {
        return bufferPtr;
    }

    void OutBinaryBufferStream::write_byte(const byte &value)
    {
        ensure_capacity(1);
        buffer[bufferPtr++] = value;
    }

    void OutBinaryBufferStream::write_bytes(const ByteArray &bytes)
    {
        size_t writeSize = bytes.size();
        ensure_capacity(writeSize);
        azgra::collection::vector_insert_at(buffer, bytes, bufferPtr, 0, writeSize);
        bufferPtr += writeSize;
    }

    ByteArray OutBinaryBufferStream::get_buffer_data() const
    {
        ByteArray data = ByteArray(buffer.begin(), (buffer.begin() + bufferPtr));
        always_assert(data.size() == bufferPtr);
        return data;
    }
}