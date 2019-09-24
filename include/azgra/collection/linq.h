#pragma once

#include <vector>
#include <functional>

namespace azgra::collection::experimental_linq
{
    //template<typename It, typename T = typename std::iterator_traits<It>::value_type>
    template<typename It, typename T = typename std::iterator_traits<It>::value_type, typename TKey>
    std::vector<T> where(const It srcBegin, const It srcEnd, std::function<TKey(const T &)> keySelector, TKey key)
    {
        std::vector<T> result;
        for (It it = srcBegin; it != srcEnd; ++it)
        {
            if (keySelector(*it) == key)
            {
                result.push_back(*it);
            }
        }
        return result;
    }
}