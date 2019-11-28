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

    }
}