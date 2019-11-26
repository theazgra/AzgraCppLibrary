#pragma once

#include "azgra/utilities/binary_converter.h"

namespace azgra::io::stream
{

    // Base class for readable custom binary streams.
    class InBinaryStreamBase
    {
    protected:
        // True if underlaying stream is open.
        bool isOpen;

    public:
        InBinaryStreamBase()
        {};

        ~InBinaryStreamBase()
        {};

        // Check if stream is open.
        bool is_open() const;

        // Close stream.
        virtual void close_stream()
        {};

        // Get size of underlaying content.
        virtual azgra::i64 get_size() const = 0;

        // Get current position in stream.
        virtual azgra::i64 get_position() = 0;

        // Move to location in stream.
        virtual void move_to(const azgra::i64 position) = 0;

        // Move stream to beginning.
        virtual void move_to_beginning() = 0;

        // Move stream to end.
        virtual void move_to_end() = 0;

        // Move stream by distance.
        virtual void move_by(const azgra::i64 distance) = 0;

        // Read one byte.
        virtual byte consume_byte();

        // Read specified number of bytes are convert them into bool.
        virtual bool consume_bool(const azgra::u16 byteCount);

        // Read two bytes and convert them into azgra::i16.
        virtual azgra::i16 consume_short16();

        // Read two bytes and convert them into azgra::u16.
        virtual azgra::u16 consume_ushort16();

        // Read four bytes and convert them into azgra::i32.
        virtual azgra::i32 consume_int32();

        // Read four bytes and convert them into azgra::u32.
        virtual azgra::u32 consume_uint32();

        // Read eight bytes and convert them into azgra::i64.
        virtual azgra::i64 consume_long64();

        // Read eight bytes and convert them into azgra::u64.
        virtual azgra::u64 consume_ulong64();

        // Read four bytes and convert them into float.
        virtual float consume_float();

        // Read eight bytes and convert them into double.
        virtual double consume_double();

        // Read specified number of bytes.
        virtual ByteArray consume_bytes(const azgra::u64 byteCount) = 0;

        // Move stream to requested position and read specified number of bytes.
        virtual ByteArray move_and_consume_bytes(const azgra::i64 position, const azgra::i64 byteCount);

        // Read specified number of bytes into dst buffer, insert read bytes from pos.
        virtual void consume_into(ByteArray &dst, size_t pos, size_t byteCount);
    };

} // namespace azgra