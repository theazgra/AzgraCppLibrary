#pragma once

namespace azgra
{
    template<typename T>
    class ConstCustomAdvancementIterator
    {
    private:
        T const *m_current;
        T const *m_end;
        size_t m_advanceBy;
    public:

        // iterator traits
        using difference_type = long;
        using value_type = T;
        using pointer = T const *;
        using reference = long const &;
        using iterator_category = std::input_iterator_tag;

        explicit ConstCustomAdvancementIterator(const T *begin,
                                                const T *end,
                                                const size_t advancement)
        {
            m_current = begin;
            m_end = end;
            m_advanceBy = advancement;
        }

        explicit ConstCustomAdvancementIterator(const T *begin,
                                                const size_t advancement) :
                ConstCustomAdvancementIterator(begin, begin, advancement)
        {
        }


        ConstCustomAdvancementIterator const &operator++()
        {
            m_current += m_advanceBy;
            if (m_current > m_end)
            {
                m_current = m_end;
            }
            return *this;
        }

        bool operator==(const ConstCustomAdvancementIterator &other) const
        {
            return (m_current == other.m_current);
        }

        bool operator!=(const ConstCustomAdvancementIterator &other) const
        {
            return (m_current != other.m_current);
        }

        T const &operator*()
        {
            return *m_current;
        }

        T const *end() const
        {
            return m_end;
        }
    };

    template<typename T>
    class CustomAdvancementIterator
    {
    private:
        T *m_current;
        T const *m_end;
        size_t m_advanceBy;
    public:

        // iterator traits
        using difference_type = long;
        using value_type = T;
        using pointer = T *;
        using reference = long &;
        using iterator_category = std::forward_iterator_tag;

        explicit CustomAdvancementIterator(T *begin,
                                           const T *end,
                                           const size_t advancement)
        {
            m_current = begin;
            m_end = end;
            m_advanceBy = advancement;
        }

        explicit CustomAdvancementIterator(T *begin,
                                           const size_t advancement) :
                CustomAdvancementIterator(begin, begin, advancement)
        {
        }


        CustomAdvancementIterator &operator++()
        {
            m_current += m_advanceBy;
            if (m_current > m_end)
            {
                m_current = const_cast<T *> (m_end);
            }
            return *this;
        }

        bool operator==(const CustomAdvancementIterator &other) const
        {
            return (m_current == other.m_current);
        }

        bool operator!=(const CustomAdvancementIterator &other) const
        {
            return (m_current != other.m_current);
        }

        bool operator==(const ConstCustomAdvancementIterator<T> &other) const
        {
            return (m_current == other.end());
        }

        bool operator!=(const ConstCustomAdvancementIterator<T> &other) const
        {
            return (m_current != other.end());
        }

        T &operator*()
        {
            return *m_current;
        }
    };
}