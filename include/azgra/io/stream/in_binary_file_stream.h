#pragma once

#include "in_binary_stream_base.h"
#include <fstream>
#include <iterator>

namespace azgra::io::stream
{

    class InBinaryFileStream : public InBinaryStreamBase
    {
    private:
        // Binary stream of opened file.
        std::ifstream fileStream;
        // Size of opened file.
        azgra::i64 fileSize;

    public:
        // ifstream wrapper around binary stream.
        InBinaryFileStream();

        // Open binary stream to file.
        explicit InBinaryFileStream(const std::string &file);

        // Open binary stream to file.
        explicit InBinaryFileStream(const char *file);

        // Closes file stream.
        ~InBinaryFileStream();

        // Open binary stream to file.
        void open_stream(const char *file);

        // Close binary stream to file.
        void close_stream() override;

        // Get size of opened file.
        azgra::i64 get_size() const override;

        // Get current position in stream.
        azgra::i64 get_position() override;

        // Advance file stream to requested position.
        void move_to(const azgra::i64 position) override;

        // Move file stream to beginning.
        void move_to_beginning() override;

        // Move file stream to end.
        void move_to_end() override;

        // Move file stream by distance.
        void move_by(const azgra::i64 distance) override;

        // Read whole file, from beginning to end, to memory.
        ByteArray consume_whole_file();

        // Read rest of the file, from current position to end, into memory.
        ByteArray consume_rest_of_file();

        // Read specified number of bytes.
        ByteArray consume_bytes(const azgra::u64 byteCount) override;

        // Read specified number of bytes into dst buffer, insert read bytes from pos.
        void consume_into(ByteArray &dst, size_t pos, size_t byteCount) override;

    };
} // namespace azgra