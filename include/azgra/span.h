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
    class Span
    {
    public:
        using Type = T;

        /**
         * Default constructor, creating empty data span.
         */
        Span() = default;

        /**
         * Create memory span.
         * @param data Memory pointer.
         * @param dataSize Number of the objects in the span.
         */
        explicit Span(const T *data, const std::size_t dataSize) : m_ptr(data), m_size(dataSize)
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
            return Span<T>(m_ptr + offset, subSpanSize);
        }

        [[nodiscard]] virtual inline T operator[](const std::size_t index) const
        {
            return m_ptr[index];
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
            const std::size_t count = std::min(m_size, other.m_size);
            const int compare = m_comparer(m_ptr, other.m_ptr, count);
            if ((compare == 0) && (m_size != other.m_size))
            {
                if (m_size < other.m_size)
                    return -1;
                else if (other.m_size < m_size)
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
            const int compare = m_comparer(m_ptr, other.m_ptr, count);
            return compare;
        }

        /**
         * Get the number of objects same in both spans. Will use other.size as count.
         * @param other Span to match to.
         * @return Number of same objects in the span.
         */
        [[nodiscard]] std::size_t match_length(const Span<T> &other) const
        {
            assert(other.m_size <= m_size);
            std::size_t len = 0;

            for (size_t i = 0; i < other.m_size; ++i)
            {
                if (m_ptr[i] != other.m_ptr[i])
                {
                    return len;
                }
                ++len;
            }
            return len;
        }

        [[nodiscard]] const T *data() const
        { return m_ptr; }

        [[nodiscard]] std::size_t size() const
        { return m_size; }

    protected:
        /**
         * Memory pointer.
         */
        const T *m_ptr{nullptr};

        /**
         * Number of objects.
         */
        std::size_t m_size{0};

        /**
         * Lexicographic comparer.
         */
        LexicographicSpanComparer<T> m_comparer{};
    };

/**
 * Typedef for memory span.
 */
    typedef Span<azgra::byte> ByteSpan;
}