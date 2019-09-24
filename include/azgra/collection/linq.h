#pragma once

#include <vector>
#include <functional>

namespace azgra::collection::experimental_linq
{
    template<typename T>
    class Enumerable
    {
        template<typename U>
        friend class Enumerable;

    private:
        std::vector<T> data;

    public:
        Enumerable() = default;

        template<typename CopyIt>
        Enumerable(CopyIt srcBegin, CopyIt srcEnd)
        {
            data = std::vector<T>(srcBegin, srcEnd);
        }

        Enumerable<T> where(std::function<bool(const T &)> predicate)
        {
            Enumerable result;
            for (const T &item : data)
            {
                if (predicate(item))
                {
                    result.data.push_back(item);
                }
            }
            return result;
        }

        template<typename SelectType>
        Enumerable<SelectType> select(std::function<SelectType(const T &)> selector)
        {
            Enumerable<SelectType> result;
            result.data.resize(data.size());
            size_t index = 0;
            for (const T &item : data)
            {
                result.data[index++] = selector(item);
            }
            return result;
        }
    };


    template<typename It, typename T = typename std::iterator_traits<It>::value_type>
    std::vector<T> where(const It srcBegin, const It srcEnd, std::function<bool(const T &)> condition)
    {
        std::vector<T> result;
        for (It it = srcBegin; it != srcEnd; ++it)
        {
            if (condition(*it))
            {
                result.push_back(*it);
            }
        }
        return result;
    }

    // TODO: First,FirstOrDefault,Single,SingleOrDefault,Any,All,ForEach,Count,Count(predicate),Cast?

}