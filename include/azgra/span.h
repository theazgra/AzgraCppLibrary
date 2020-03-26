#pragma once

#include <azgra/azgra.h>

namespace azgra
{
/**
 * Default lexicographic comparer.
 * @tparam T Type of element.
 */
    template<typename T>
    struct LexicographicSpanComparer
    {
        [[nodiscard]] inline int operator()(const T *aData, const T *bData, const std::size_t count) const
        {
            for (size_t i = 0; i < count; ++i)
            {
                if (aData[i] < bData[i])
                { return -1; }
                if (bData[i] < aData[i])
                { return 1; }
            }
            return 0;
        }
    };

/**
 * Lexicographic comparer for bytes, using memcmp.
 */
    template<>
    struct LexicographicSpanComparer<azgra::byte>
    {
        [[nodiscard]] inline int operator()(const azgra::byte *aData, const azgra::byte *bData, const std::size_t count) const
        {
            const int result = std::memcmp(aData, bData, count);
            if (result == 0)
                return 0;
            else if (result < 0)
                return -1;
            else
                return 1;
        }
    };

/**
 * Object representing selected part of continuous memory.
 * @tparam T Object in the memory.
 */
    template<typename T>
    struct Span
    {
        /**
         * Memory pointer.
         */
        const T *ptr{nullptr};

        /**
         * Number of objects.
         */
        std::size_t size{0};

        /**
         * Lexicographic comparer.
         */
        LexicographicSpanComparer<T> m_comparer{};

        /**
         * Default constructor.
         */
        Span() = default;

        /**
         * Create memory span.
         * @param data Memory pointer.
         * @param dataSize Number of the objects in the span.
         */
        explicit Span(const T *data, const std::size_t dataSize) : ptr(data), size(dataSize)
        {
        }

        /**
         * Create subspan from this span
         * @param offset Offset the of subspan.
         * @param subSpanSize Size of the subspan.
         * @return Subspan of this span.
         */
        [[nodiscard]] Span<T> sub_span(const std::size_t offset, const std::size_t subSpanSize) const
        {
            return Span<T>(ptr + offset, subSpanSize);
        }

        [[nodiscard]] inline T operator[](const std::size_t index) const
        {
            return ptr[index];
        }

        [[nodiscard]] inline bool operator==(const Span<T> &other) const
        {
            return (lexicographic_compare(other) == 0);
        }

        [[nodiscard]] inline bool operator!=(const Span<T> &other) const
        {
            return (lexicographic_compare(other) != 0);
        }

        [[nodiscard]] inline bool operator<(const Span<T> &other) const
        {
            return (lexicographic_compare(other) < 0);
        }

        [[nodiscard]] inline bool operator>(const Span<T> &other) const
        {
            return (lexicographic_compare(other) > 0);
        }

        /**
         * Lexicographically compare two Spans.
         * @param other Another span to compare to.
         * @return -1 if this is smaller than other, 0 if equal, 1 if other is bigger.
         */
        [[nodiscard]] int lexicographic_compare(const Span<T> &other) const
        {
            const std::size_t count = std::min(size, other.size);
            const int compare = m_comparer(ptr, other.ptr, count);
            if ((compare == 0) && (size != other.size))
            {
                if (size < other.size)
                    return -1;
                else if (other.size < size)
                    return 1;
            }
            return compare;
        }

        /**
         * Lexicographically compare two Spans, compare first count of objects.
         * @param other Another span to compare to.
         * @param count Number of objects to compare.
         * @return -1 if this is smaller than other, 0 if equal, 1 if other is bigger.
         */
        [[nodiscard]] int lexicographic_compare(const Span<T> &other, const std::size_t count) const
        {
            const int compare = m_comparer(ptr, other.ptr, count);
            return compare;
        }

        /**
         * Get the number of objects same in both spans. Will use other.size as count.
         * @param other Span to match to.
         * @return Number of same objects in the span.
         */
        [[nodiscard]] std::size_t match_length(const Span<T> &other) const
        {
            assert(other.size <= size);
            std::size_t len = 0;

            for (size_t i = 0; i < other.size; ++i)
            {
                if (ptr[i] != other.ptr[i])
                {
                    return len;
                }
                ++len;
            }
            return len;
        }

    };

/**
 * Typedef for memory span.
 */
    typedef Span<azgra::byte> ByteSpan;
}