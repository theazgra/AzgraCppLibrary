#pragma once

#include <vector>
#include <functional>
#include <limits>
#include <azgra/azgra.h>

using namespace std;
namespace azgra
{
    namespace linq
    {
        template<typename T>
        vector<T> where_ref(const std::vector<T> &src, function<bool(const T &)> predicate)
        {
            vector<T> result;
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
        vector<T> where(const std::vector<T> &src, function<bool(const T)> predicate)
        {
            vector<T> result;
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
        bool any_ref(const vector<T> &src, function<bool(const T &)> predicate)
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
        bool any(const vector<T> &src, function<bool(const T)> predicate)
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
        void for_each_ref(const vector<T> &src, function<void(const T &)> work)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                work(src[i]);
            }
        }

        template<typename T>
        void for_each(const vector<T> &src, function<void(const T)> work)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                work(src[i]);
            }
        }

        template<typename T>
        bool all_ref(const vector<T> &src, function<bool(const T &)> predicate)
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
        bool all(const vector<T> &src, function<bool(const T)> predicate)
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
        bool contains(const vector<T> &src, const T &element)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (src[i] == element)
                    return true;
            }
            return false;
        }

        template<typename T>
        bool contains(const vector<T> &src, const T &element, std::function<bool(const T &, const T &)> cmp)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (cmp(src[i], element))
                    return true;
            }
            return false;
        }

        template<typename T>
        azgra::i64 get_index(const vector<T> &src, const T &element)
        {
            for (size_t i = 0; i < src.size(); i++)
            {
                if (src[i] == element)
                    return i;
            }
            return -1;
        }

        template<typename T>
        size_t count(const vector<T> &src, const T &element)
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
        vector<T> except(const vector<T> &src, const vector<T> &except)
        {
            vector<T> result;
            for (size_t i = 0; i < src.size(); i++)
            {
                if (!contains(except, src[i]))
                {
                    result.push_back(src[i]);
                }
            }
            return result;
        }

        template<typename T>
        T min(const vector<T> &src)
        {
            T min = std::numeric_limits<T>::max();

            for (size_t i = 0; i < src.size(); i++)
            {
                if (src[i] < min)
                {
                    min = src[i];
                }
            }
            return min;
        }

        template<typename T>
        T max(const vector<T> &src)
        {
            T max = std::numeric_limits<T>::min();

            for (size_t i = 0; i < src.size(); i++)
            {
                if (src[i] > max)
                {
                    max = src[i];
                }
            }
            return max;
        }

        template<typename T>
        std::pair<T, T> min_max(const vector<T> &src)
        {
            T min = std::numeric_limits<T>::max();
            T max = std::numeric_limits<T>::min();

            for (size_t i = 0; i < src.size(); i++)
            {
                if (src[i] < min)
                {
                    min = src[i];
                }
                if (src[i] > max)
                {
                    max = src[i];
                }
            }
            return make_pair(min, max);
        }

        template<typename T>
        T sum(const vector<T> &src)
        {
            T sum = 0;
            for (size_t i = 0; i < src.size(); i++)
            {
                sum += src[i];
            }
            return sum;
        }

        template<typename TargetType, typename SourceType>
        vector<TargetType> cast_to(const std::vector<SourceType> &src)
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

    }; // namespace linq
}; // namespace azgra