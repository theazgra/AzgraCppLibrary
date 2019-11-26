#pragma once

#include "buffer_source_type.h"
#include "in_binary_stream_base.h"

namespace azgra::io::stream
{

    class InBinaryBufferStream : public InBinaryStreamBase
    {
    private:
        // Size of a buffer.
        azgra::u64 streamBufferSize = 500000;
        // Stream from which we can pull data into buffer.
        InBinaryStreamBase *underlayingStream;
        // Buffer for underlaying streams.
        ByteArray streamBuffer;
        // Type of underlaying structure, from which we pull data into main buffer.
        BufferSourceType underlayingSourceType;
        // Memory buffer.
        ByteArray *memoryBuffer;
        // Current position in buffer.
        azgra::u64 currentBufferPosition = 0;
        // Number of bytes readed from source.
        azgra::u64 readedFromSource = 0;

        azgra::u64 get_consume_size();

    public:
        InBinaryBufferStream();

        InBinaryBufferStream(ByteArray *bytes);

        InBinaryBufferStream(InBinaryStreamBase *stream, const azgra::i64 bufferSize = 500000);

        ~InBinaryBufferStream();

        // Get size of underlaying content.
        azgra::i64 get_size() const override;

        // Get current position in stream.
        azgra::i64 get_position() override;

        // Move to location in stream.
        void move_to(const azgra::i64 position) override;

        // Move stream to beginning.
        void move_to_beginning() override;

        // Move stream to end.
        void move_to_end() override;

        // Move stream by distance.
        void move_by(const azgra::i64 distance) override;

        // Read specified number of bytes.
        ByteArray consume_bytes(const azgra::u64 byteCount) override;
    };
} // namespace azgra
