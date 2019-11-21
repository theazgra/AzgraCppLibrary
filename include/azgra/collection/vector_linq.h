#pragma once

#include <azgra/azgra.h>
#include <vector>
#include <functional>
#include <limits>

namespace azgra
{
    namespace collection
    {
        template<typename T>
        std::vector<T> where_ref(const std::vector<T> &src, std::function<bool(const T &)> predicate)
        {
            std::vector<T> result;
            for (size_t i = 0; i < src.size(); i++)
            {
                if (predicate(src[i]))
                {
                    result.push_back(src[i]);
                }
            }
            return result;
        }

        template<typename T>
        std::vector<T> where(const std::vector<T> &src, std::function<bool(const T)> predicate)
        {
            std::vector<T> result;
            for (size_t i = 0; i < src.size(); i++)
            {
                if (predicate(src[i]))
                {
                    result.push_back(src[i]);
                }
            }
            return result;
        }

        template<typename T>
        bool any_ref(const std::vector<T> &src, std::function<bool(const T &)> predicate)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (predicate(src[i]))
                {
                    return true;
                }
            }
            return false;
        }

        template<typename T>
        bool any(const std::vector<T> &src, std::function<bool(const T)> predicate)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (predicate(src[i]))
                {
                    return true;
                }
            }
            return false;
        }

        template<typename T>
        void for_each_ref(const std::vector<T> &src, std::function<void(const T &)> work)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                work(src[i]);
            }
        }

        template<typename T>
        void for_each(const std::vector<T> &src, std::function<void(const T)> work)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                work(src[i]);
            }
        }

        template<typename T>
        bool all_ref(const std::vector<T> &src, std::function<bool(const T &)> predicate)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (!predicate(src[i]))
                {
                    return false;
                }
            }
            return true;
        }

        template<typename T>
        bool all(const std::vector<T> &src, std::function<bool(const T)> predicate)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (!predicate(src[i]))
                {
                    return false;
                }
            }
            return true;
        }

        template<typename T>
        bool contains(const std::vector<T> &src, const T &element)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (src[i] == element)
                    return true;
            }
            return false;
        }

        template<typename It, typename T = typename std::iterator_traits<It>::value_type>
        bool contains(It srcBegin, It srcEnd, const T &element)
        {
            for (It iterator = srcBegin; iterator != srcEnd; ++iterator)
            {
                if (*iterator == element)
                {
                    return true;
                }
            }
            return false;
        }

        template<typename T>
        bool contains(const std::vector<T> &src, const T &element, std::function<bool(const T &, const T &)> cmp)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (cmp(src[i], element))
                    return true;
            }
            return false;
        }

        template<typename T>
        azgra::i64 get_index(const std::vector<T> &src, const T &element)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (src[i] == element)
                    return i;
            }
            return -1;
        }

        template<typename T>
        size_t count(const std::vector<T> &src, const T &element)
        {
            size_t result = 0;
            for (size_t i = 0; i < src.size(); i++)
            {
                if (src[i] == element)
                    result += 1;
            }
            return result;
        }

        template<typename T>
        std::vector<T> except(const std::vector<T> &src, const std::vector<T> &except)
        {
            std::vector<T> result;
            for (size_t i = 0; i < src.size(); i++)
            {
                if (!contains(except, src[i]))
                {
                    result.push_back(src[i]);
                }
            }
            return result;
        }

        template<typename TargetType, typename SourceType>
        std::vector<TargetType> cast_to(const std::vector<SourceType> &src)
        {
            std::vector<TargetType> result(src.size());
            for (size_t i = 0; i < src.size(); i++)
            {
                result[i] = static_cast<TargetType>(src[i]);
            }
            return result;
        }

        template<typename T>
        std::vector<size_t> add_together(std::vector<T> &result, const std::vector<T> &add)
        {
            always_assert(result.size() == add.size());
            for (size_t i = 0; i < result.size(); i++)
            {
                result[i] += add[i];
            }
            return result;
        }

        template<typename T>
        std::vector<size_t> div_by(std::vector<T> &result, size_t div)
        {

            for (size_t i = 0; i < result.size(); i++)
            {
                result[i] = result[i] / div;
            }
            return result;
        }

        template<typename T>
        bool equals(const std::vector<T> &a, const std::vector<T> &b)
        {
            if (a.size() != b.size())
                return false;

            for (size_t i = 0; i < a.size(); i++)
            {
                if (a[i] != b[i])
                    return false;
            }

            return true;
        }

        template<typename T>
        bool equals_memcmp(const std::vector<T> &a, const std::vector<T> &b)
        {
            if (a.size() != b.size())
                return false;

            return (memcmp(a.data(), b.data(), sizeof(T) * a.size()) == 0);
        }

    }; // namespace collection
}; // namespace azgra