#pragma once

#include "azgra/utilities/binary_converter.h"

namespace azgra::io::stream
{
    // Base class for custom writable binary streams.
    class OutBinaryStreamBase
    {
    protected:
        // True if underlaying stream is open.
        bool isOpen;
    public:
        OutBinaryStreamBase()
        {};

        ~OutBinaryStreamBase()
        {};

        // Check if stream is open.
        bool is_open() const;

        // Close stream.
        virtual void close_stream()
        {};

        // Get current position in stream.
        virtual size_t get_position() = 0;

        // Write byte to the stream.
        virtual void write_byte(const byte &value) = 0;

        // Write bytes to the stream.
        virtual void write_bytes(const ByteArray &bytes) = 0;

        // Write boolean value in defined number of bytes.
        virtual void write_bool(const bool &value, const azgra::u16 byteCount = sizeof(bool));

        // Write short value to the stream.
        virtual void write_short16(const azgra::i16 &value);

        // Write ushort value to the stream.
        virtual void write_ushort16(const azgra::u16 &value);

        // Write int value to the stream.
        virtual void write_int32(const azgra::i32 &value);

        // Write uint value to the stream.
        virtual void write_uint32(const azgra::u32 &value);

        // Write long value to the stream.
        virtual void write_long64(const azgra::i64 &value);

        // Write ulong value to the stream.
        virtual void write_ulong64(const azgra::u64 &value);

        // Write float value to the stream.
        virtual void write_float(const float &value);

        // Write float value to the stream.
        virtual void write_double(const double &value);

        // Write bytes from buffer to the stream.
        virtual void write_bytes_from_buffer(const ByteArray &buffer, const size_t bufferPos, const size_t byteCount);

        // Write bytes from buffer to the stream.
        virtual void write_bytes_from_buffer(const char *buffer, const size_t byteCount);

        // Write the byte value `repCount` times into the stream.
        virtual void write_replicated_bytes(const byte &repValue, const size_t repCount);
    };

} // namespace azgra