#include <azgra/io/stream/in_binary_buffer_stream.h>

namespace azgra::io::stream
{
    InBinaryBufferStream::InBinaryBufferStream()
    {
        this->isOpen = false;
        this->underlayingSourceType = BufferSourceType_NoSource;
    }

    InBinaryBufferStream::InBinaryBufferStream(ByteArray *bytes)
    {
        this->streamBufferSize = bytes->size();
        this->isOpen = true;
        this->underlayingSourceType = BufferSourceType_Memory;
        this->memoryBuffer = bytes;
        this->currentBufferPosition = 0;
    }

    InBinaryBufferStream::InBinaryBufferStream(InBinaryStreamBase *stream, const azgra::i64 streamBufferSize)
    {
        this->isOpen = stream->is_open();
        this->underlayingSourceType = BufferSourceType_Stream;
        this->streamBufferSize = streamBufferSize;
        this->underlayingStream = stream;
        this->currentBufferPosition = 0;
    }

    InBinaryBufferStream::~InBinaryBufferStream()
    {
        // Do we want to close stream / clear buffer?
        // Probably not, because someone may want to use them later.
        /*
        switch (this->underlayingSourceType)
        {
        case BufferSourceType_Memory:
        {
            this->buffer.clear();
        }
        break;
        case BufferSourceType_Stream:
        {
            this->underlayingStream->close_stream();
        }
        break;
        }
        */
    }

    azgra::i64 InBinaryBufferStream::get_size() const
    {
        switch (this->underlayingSourceType)
        {
            case BufferSourceType_Memory:
            {
                return this->memoryBuffer->size();
            }
                break;
            case BufferSourceType_Stream:
            {
                return this->underlayingStream->get_size();
            }
                break;
            default:
                always_assert(false && "Invalid BufferSourceType.");
                break;
        }

        return 0;
    }

    azgra::i64 InBinaryBufferStream::get_position()
    {
        switch (this->underlayingSourceType)
        {
            case BufferSourceType_Memory:
            {
                return this->currentBufferPosition;
            }
                break;
            case BufferSourceType_Stream:
            {
                always_assert(this->currentBufferPosition <= this->streamBufferSize);
                azgra::i64 pos = (this->readedFromSource > 0) ? this->readedFromSource -
                                                                (this->streamBufferSize - this->currentBufferPosition) : 0;
                return pos;
            }
                break;
            default:
                always_assert(false && "Invalid BufferSourceType.");
                break;
        }

        return 0;
    }

    azgra::u64 InBinaryBufferStream::get_consume_size()
    {

        switch (this->underlayingSourceType)
        {
            case BufferSourceType_Memory:
            {
                always_assert(false && "This shouldn't be called ever.");
                return 0;
            }
                break;
            case BufferSourceType_Stream:
            {
                azgra::i64 totalSize = get_size();
                azgra::u64 currPos = this->underlayingStream->get_position();
                azgra::u64 avaible = totalSize - currPos;
                azgra::u64 toConsume = (avaible > this->streamBufferSize) ? this->streamBufferSize : avaible;
                return toConsume;
            }
                break;
            default:
                always_assert(false && "Invalid BufferSourceType.");
                break;
        }
        return 0;
    }

    void InBinaryBufferStream::move_to(const azgra::i64 position)
    {
        always_assert(position < get_size());

        switch (this->underlayingSourceType)
        {
            case BufferSourceType_Memory:
            {
                this->currentBufferPosition = position;
            }
                break;
            case BufferSourceType_Stream:
            {

                // move_to will always reload the buffer.
                this->underlayingStream->move_to(position);
                azgra::u64 toConsume = get_consume_size();
                this->streamBuffer = this->underlayingStream->consume_bytes(toConsume);
                this->readedFromSource = this->underlayingStream->get_position();

                // Just for sure.
                //always_assert(this->readedFromSource == position + this->streamBufferSize);

                this->currentBufferPosition = 0;
            }
                break;
            default:
                always_assert(false && "Invalid BufferSourceType.");
                break;
        }
    }

    void InBinaryBufferStream::move_to_beginning()
    {
        switch (this->underlayingSourceType)
        {
            case BufferSourceType_Memory:
            {
                this->currentBufferPosition = 0;
            }
                break;
            case BufferSourceType_Stream:
            {
                // move_to_beginning will always reload the buffer.
                this->underlayingStream->move_to_beginning();
                this->streamBuffer = this->underlayingStream->consume_bytes(get_consume_size());
                this->readedFromSource = this->streamBufferSize;
                this->currentBufferPosition = 0;
            }
                break;
            default:
                always_assert(false && "Invalid BufferSourceType.");
                break;
        }
    }

    void InBinaryBufferStream::move_to_end()
    {
        this->currentBufferPosition = get_size();
        switch (this->underlayingSourceType)
        {
            case BufferSourceType_Stream:
            {
                // move_to_beginning will always clear the buffer.
                this->streamBuffer.clear();
                this->readedFromSource = this->currentBufferPosition;
                this->currentBufferPosition = 0;
            }
                break;
            default:
                always_assert(false && "Invalid BufferSourceType.");
                break;
        }
    }

    void InBinaryBufferStream::move_by(const azgra::i64 distance)
    {
        always_assert((get_position() + distance) < get_size());

        switch (this->underlayingSourceType)
        {
            case BufferSourceType_Memory:
            {
                always_assert((this->currentBufferPosition + distance) < this->memoryBuffer->size());
                this->currentBufferPosition += distance;
            }
                break;
            case BufferSourceType_Stream:
            {
                // If atleast one byte can be read, just advance the buffer position.
                if ((this->currentBufferPosition + distance + 1) < this->streamBufferSize)
                {
                    this->currentBufferPosition += distance;
                }
                else
                {
                    // If we are moving outside buffer, reload the buffer from desired location.
                    azgra::i64 currentPosInStreamRelativeToBufferPos = get_position();

                    always_assert((currentPosInStreamRelativeToBufferPos + distance) < this->underlayingStream->get_size());
                    this->underlayingStream->move_to((currentPosInStreamRelativeToBufferPos + distance));

                    azgra::u64 toConsume = get_consume_size();
                    this->streamBuffer = this->underlayingStream->consume_bytes(toConsume);
                    this->readedFromSource = this->underlayingStream->get_position();
                    this->currentBufferPosition = 0;
                }
            }
                break;
            default:
                always_assert(false && "Invalid BufferSourceType.");
                break;
        }
    }

    ByteArray InBinaryBufferStream::consume_bytes(const azgra::u64 byteCount)
    {
        if (this->streamBufferSize < byteCount)
        {
            printf("To parse this file, you need buffer of size atleast: %lu\n", byteCount);
        }
        // We won't allow consuming more bytes than is buffer size.
        always_assert(byteCount <= this->streamBufferSize && "Buffer is too small.");

        switch (this->underlayingSourceType)
        {
            case BufferSourceType_Memory:
            {
                // Check how many bytes are avaible in memory buffer.
                azgra::u64 bytesAvaible = this->memoryBuffer->size() - this->currentBufferPosition;
                always_assert(bytesAvaible >= byteCount);

                auto fromIt = this->memoryBuffer->begin() + this->currentBufferPosition;
                auto toIt = fromIt + byteCount;
                this->currentBufferPosition += byteCount;

                ByteArray result(fromIt, toIt);
                return result;
            }
                break;
            case BufferSourceType_Stream:
            {
                azgra::u64 bytesAvaible = this->streamBuffer.size() - this->currentBufferPosition;
                if ((bytesAvaible == 0) || (byteCount > bytesAvaible))
                {
                    azgra::i64 currentPos = get_position();
                    // Moving to current position will ensure, that streamBufferSize of data will be read into buffer.
                    move_to(currentPos);
                }

                auto fromIt = this->streamBuffer.begin() + this->currentBufferPosition;
                auto toIt = fromIt + byteCount;
                this->currentBufferPosition += byteCount;

                ByteArray result(fromIt, toIt);
                return result;
            }
                break;
            default:
                always_assert(false && "Invalid BufferSourceType.");
                break;
        }

        return ByteArray();
    }
} // namespace azgra