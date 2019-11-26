#include <azgra/io/stream/in_binary_file_stream.h>

namespace azgra::io::stream
{

    InBinaryFileStream::InBinaryFileStream()
    {
        this->isOpen = false;
    }

    InBinaryFileStream::InBinaryFileStream(const std::string &file)
    {
        this->isOpen = false;
        open_stream(file);
    }

    void InBinaryFileStream::open_stream(const std::string &file)
    {
        always_assert(!this->isOpen);

        this->fileStream = std::ifstream(file, std::ios::binary | std::ios::in | std::ios::ate);
        always_assert(this->fileStream.is_open());

        this->isOpen = true;

        this->fileStream.unsetf(std::ios::skipws);
        this->fileSize = fileStream.tellg();
        fileStream.seekg(std::ios::beg);
    }

    void InBinaryFileStream::close_stream()
    {
        if (this->isOpen)
        {
            this->fileStream.close();
            this->isOpen = false;
        }
    }

    InBinaryFileStream::~InBinaryFileStream()
    {
        close_stream();
    }

    azgra::i64 InBinaryFileStream::get_size() const
    {
        always_assert(this->isOpen);
        return this->fileSize;
    }

    azgra::i64 InBinaryFileStream::get_position()
    {
        always_assert(this->isOpen);
        azgra::i64 result = (azgra::i64) (this->fileStream.tellg());
        if (result < 0)
        {
            printf("why...\n");
            always_assert(result >= 0);
        }
        return result;
    }

    void InBinaryFileStream::move_to(const azgra::i64 position)
    {
        always_assert(this->isOpen);
        this->fileStream.seekg(position);
    }

    void InBinaryFileStream::move_to_beginning()
    {
        always_assert(this->isOpen);
        this->fileStream.seekg(std::ios::beg);
    }

    void InBinaryFileStream::move_to_end()
    {
        always_assert(this->isOpen);
        this->fileStream.seekg(std::ios::end);
    }

    void InBinaryFileStream::move_by(const azgra::i64 distance)
    {
        always_assert(this->isOpen);
        auto currentPosition = this->fileStream.tellg();
        std::streampos requiredPosition = currentPosition.operator+(distance);
        this->fileStream.seekg(requiredPosition);
    }

    ByteArray InBinaryFileStream::consume_whole_file()
    {
        always_assert(this->isOpen);
        ByteArray result;
        this->fileStream.seekg(std::ios::beg);
        result.insert(result.begin(), std::istream_iterator<byte>(this->fileStream), std::istream_iterator<byte>());
        return result;
    }

    ByteArray InBinaryFileStream::consume_rest_of_file()
    {
        always_assert(this->isOpen);
        ByteArray result;
        result.insert(result.begin(), std::istream_iterator<byte>(this->fileStream), std::istream_iterator<byte>());
        return result;
    }

    ByteArray InBinaryFileStream::consume_bytes(const azgra::u64 byteCount)
    {
        /*
        always_assert(this->isOpen);

        //TODO: Can this be made faster?
        ByteArray result;

        if (byteCount == 0)
            return result;

        auto readIterator = std::istream_iterator<byte>(fileStream);

        result.resize(byteCount);

        for (size_t i = 0; i < byteCount - 1; i++)
        {
            result[i] = *readIterator++;
        }
        // This is done, so we don't move stream one position too far.
        result[byteCount - 1] = *readIterator;
        return result;
        */
        always_assert(this->isOpen);

        ByteArray result(byteCount);
        if (byteCount == 0)
            return result;

        fileStream.read(reinterpret_cast<char *>(result.data()), byteCount);

        return result;
    }

    void InBinaryFileStream::consume_into(ByteArray &dst, size_t pos, size_t byteCount)
    {
        always_assert(this->isOpen);
        always_assert(dst.size() - pos >= byteCount);

        if (byteCount == 0)
            return;

        byte *writePtr = (dst.data() + pos);
        fileStream.read(reinterpret_cast<char *>(writePtr), byteCount);
    }
} // namespace azgra