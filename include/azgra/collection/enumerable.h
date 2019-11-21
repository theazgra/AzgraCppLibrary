#pragma once

#include <azgra/azgra.h>
#include <vector>
#include <set>
#include <functional>
#include <algorithm>
#include <numeric>
#include <boost/optional.hpp>

#define REQUIRE_NUMERIC_TEMPLATE(T) static_assert( \
    std::is_same<T, int>() ||               \
        std::is_same<T, long>() ||          \
        std::is_same<T, size_t>() ||        \
        std::is_same<T, float>() ||         \
        std::is_same<T, double>() ||        \
        std::is_same<T, azgra::byte>() ||   \
        std::is_same<T, azgra::u16>() ||    \
        std::is_same<T, azgra::u32>() ||    \
        std::is_same<T, azgra::u64>() ||    \
        std::is_same<T, azgra::i16>() ||    \
        std::is_same<T, azgra::i32>() ||    \
        std::is_same<T, azgra::i64>(),      \
    "Type must be numeric.")

namespace azgra::collection
{
    template<
            typename It,
            typename SelectorFunction,
            typename T = typename std::iterator_traits<It>::value_type,
            typename SelectType = typename std::result_of<SelectorFunction &(T)>::type
    >
    std::vector<SelectType> select(const It begin, const It end, SelectorFunction selector)
    {
        std::vector<SelectType> result;
        const size_t size = std::distance(begin, end);
        result.resize(size);
        std::transform(begin, end, result.begin(), selector);
        return result;
    }

    template<
            typename It,
            typename SelectorFunction,
            typename T = typename std::iterator_traits<It>::value_type,
            typename SelectType = typename std::result_of<SelectorFunction &(T)>::type
    >
    auto sum(const It begin, const It end, SelectorFunction selector)
    {
        REQUIRE_NUMERIC_TEMPLATE(SelectType);
        auto sum = 0.0;
        for (It from = begin; from != end; ++from)
        {
            sum += selector(*from);
        }
        return sum;
    }

    template<
            typename It,
            typename It2,
            typename T = typename std::iterator_traits<It>::value_type,
            typename T2 = typename std::iterator_traits<It2>::value_type
    >
    std::vector<T> except(const It begin, const It end, const It2 exceptBegin, const It2 exceptEnd)
    {
        static_assert(std::is_same_v<T, T2>);
        std::vector<T> result;

        std::copy_if(begin, end, std::back_inserter(result), [exceptBegin, exceptEnd](const T &value)
        {
            return (std::find(exceptBegin, exceptEnd, value) == exceptEnd);
        });

        return result;
    }

    template<
            typename It,
            typename T = typename std::iterator_traits<It>::value_type
    >
    T max(const It begin, const It end)
    {
        const auto maxValue = *std::max_element(begin, end);
        return maxValue;
    }

    template<
            typename It,
            typename T = typename std::iterator_traits<It>::value_type
    >
    T min(const It begin, const It end)
    {
        const auto minValue = *std::min_element(begin, end);
        return minValue;
    }

    template<
            typename It,
            typename T = typename std::iterator_traits<It>::value_type
    >
    std::pair<T, T> min_max(const It begin, const It end)
    {

        const auto minMaxValue = *std::minmax_element(begin, end);
        return minMaxValue;
    }

    //min,max,minmax

    class EnumerableError : public std::runtime_error
    {
    public:
        explicit EnumerableError(const char *errorMessage) : std::runtime_error(errorMessage)
        {
        }
    };

    template<typename T>
    class Enumerable
    {
        template<typename U>
        friend
        class Enumerable;

    private:
        std::vector<T> data;

    public:
        Enumerable() = default;

        explicit Enumerable(std::vector<T> dataSrc)
        {
            data = std::vector<T>(std::move(dataSrc));
        }

        template<typename CopyIt>
        Enumerable(CopyIt srcBegin, CopyIt srcEnd)
        {
            data = std::vector<T>(srcBegin, srcEnd);
        }

        template<typename Predicate>
        Enumerable<T> where(const Predicate &predicate) const noexcept
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

        template<typename SelectorFunction, typename SelectType = typename std::result_of<SelectorFunction &(T)>::type>
        Enumerable<SelectType> select(SelectorFunction selector) const noexcept
        {
            Enumerable<SelectType> result;
            result.data = azgra::collection::select(data.begin(), data.end(), selector);
            return result;
        }

        std::vector<T> to_vector() const noexcept
        {
            return data;
        }

        T first() const noexcept(false)
        {
            if (data.size() > 0)
            {
                return data[0];
            }
            else
            {
                throw EnumerableError("Enumerable is empty.");
            }
        }

        boost::optional<T> first_or_default() const noexcept
        {
            if (data.size() > 0)
            {
                return data[0];
            }
            else
            {
                return boost::none;
            }
        }

        T last() const noexcept(false)
        {
            if (data.size() > 0)
            {
                return data[data.size() - 1];
            }
            else
            {
                throw EnumerableError("Enumerable is empty.");
            }
        }

        boost::optional<T> last_or_default() const noexcept
        {
            if (data.size() > 0)
            {
                return data[data.size() - 1];
            }
            else
            {
                return boost::none;
            }
        }

        template<typename Predicate>
        T first(const Predicate &predicate) const noexcept(false)
        {
            if (data.size() > 0)
            {
                for (const T &item : data)
                {
                    if (predicate(item))
                    {
                        return item;
                    }
                }
                throw EnumerableError("Didn't find requested item.");
            }
            else
            {
                throw EnumerableError("Enumerable is empty.");
            }
        }

        template<typename Predicate>
        boost::optional<T> first_or_default(const Predicate &predicate) const noexcept
        {
            if (data.size() > 0)
            {
                for (const T &item : data)
                {
                    if (predicate(item))
                    {
                        return item;
                    }
                }
            }
            return boost::none;
        }

        template<typename Predicate>
        T single(const Predicate &predicate) const noexcept(false)
        {
            T result;
            bool found = false;
            if (data.size() > 0)
            {
                for (const T &item : data)
                {
                    if (predicate(item))
                    {
                        if (found)
                        {
                            throw EnumerableError("More than one item satisfy the condition.");
                        }
                        result = item;
                        found = true;
                    }
                }
                if (found)
                {
                    return result;
                }
                else
                {
                    throw EnumerableError("No item satisfies the condition.");
                }
            }
            else
            {
                throw EnumerableError("Enumerable is empty.");
            }
        }

        template<typename Predicate>
        boost::optional<T> single_or_default(const Predicate &predicate) const noexcept(false)
        {
            T result;
            bool found = false;
            if (data.size() > 0)
            {
                for (const T &item : data)
                {
                    if (predicate(item))
                    {
                        if (found)
                        {
                            throw EnumerableError("More than one item satisfy the condition.");
                        }
                        result = item;
                        found = true;
                    }
                }
                if (found)
                {
                    return result;
                }
                else
                {
                    return boost::none;
                }
            }
            else
            {
                return boost::none;
            }
        }

        bool any() const noexcept
        {
            return data.size() > 0;
        }

        template<typename Predicate>
        bool any(const Predicate &predicate) const noexcept
        {
            if (data.size() > 0)
            {
                for (const T &item : data)
                {
                    if (predicate(item))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        template<typename Predicate>
        bool all(const Predicate &predicate) const noexcept
        {
            for (const T &item : data)
            {
                if (!predicate(item))
                {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] size_t count() const noexcept
        {
            return data.size();
        }

        template<typename Predicate>
        size_t count(const Predicate &predicate) const noexcept
        {
            size_t result = 0;
            for (const T &item : data)
            {
                if (predicate(item))
                {
                    ++result;
                }
            }
            return result;
        }

        template<typename Predicate>
        bool contains(const Predicate &predicate) const noexcept
        {
            for (const T &item : data)
            {
                if (predicate(item))
                {
                    return true;
                }
            }
            return false;
        }

        double average() const noexcept
        {
            REQUIRE_NUMERIC_TEMPLATE(T);
            double sum = 0.0;
            size_t count = 0;
            for (const T &item : data)
            {
                sum += item;
                ++count;
            }

            return (sum / static_cast<double>(count));
        }

        auto sum() const noexcept
        {
            return std::accumulate(data.begin(), data.end(), 0.0);
        }

        template<typename SelectorFunction, typename SelectType = typename std::result_of<SelectorFunction &(T)>::type>
        auto sum(SelectorFunction selector) const noexcept
        {
            return collection::sum(data.begin(), data.end(), selector);
        }

        Enumerable<T> reverse() const noexcept
        {
            std::vector<T> reversedData(data.begin(), data.end());
            std::reverse(reversedData.begin(), reversedData.end());
            Enumerable<T> result(reversedData);
            return result;
        }

        Enumerable<T> take(const size_t count) const noexcept
        {
            size_t takeSize = count < data.size() ? count : data.size();
            Enumerable<T> result;
            result.data.resize(takeSize);
            for (size_t i = 0; i < takeSize; ++i)
            {
                result.data[i] = data[i];
            }
            return result;
        }

        template<typename Predicate>
        Enumerable<T> take_while(const Predicate &predicate) const noexcept
        {
            Enumerable<T> result;
            for (const T &item : data)
            {
                if (!predicate(item))
                {
                    break;
                }
                result.data.push_back(item);
            }
            return result;
        }

        Enumerable<T> skip(const size_t skipCount) const noexcept
        {
            Enumerable<T> result;
            if (skipCount >= data.size())
            {
                return result;
            }

            for (size_t i = skipCount; i < data.size(); ++i)
            {
                result.data.push_back(data[i]);
            }
            return result;
        }

        Enumerable<T> except(const std::vector<T> &exceptSrc) const noexcept
        {
            auto exceptData = collection::except(data.begin(), data.end(), exceptSrc.begin(), exceptSrc.end());
            Enumerable<T> result(exceptData);
            return exceptData;
        }

        Enumerable<T> except(const Enumerable<T> &exceptSrc) const noexcept
        {
            return except(exceptSrc.data);
        }

        Enumerable<T> distinct() const noexcept
        {
            std::set<T> distinctSet(data.begin(), data.end());
            Enumerable<T> result(distinctSet.begin(), distinctSet.end());
            return result;
        }

        template<typename ResultType, typename MapFunction>
        Enumerable<ResultType> for_each(const MapFunction &fn) const noexcept
        {
            Enumerable<ResultType> result;
            result.data.resize(data.size());
            std::transform(data.begin(), data.end(), result.data.begin(), fn);
            return result;
        }

        static Enumerable<T> repeat(const size_t count, const T item)
        {
            std::vector<T> items(count);
            for (size_t i = 0; i < count; ++i)
            {
                items[i] = item;
            }
            return Enumerable(items);
        }

        static Enumerable<T> range(const T inclusiveFrom, const T exclusiveTo)
        {
            static_assert(std::is_same<T, int>::value || std::is_same<T, uint>::value || std::is_same<T, size_t>::value
                          || std::is_same<T, long>::value || std::is_same<T, short>::value);

            std::vector<T> items(exclusiveTo - inclusiveFrom);
            for (size_t i = inclusiveFrom; i < exclusiveTo; ++i)
            {
                items[i - inclusiveFrom] = i;
            }
            return Enumerable(items);
        }

        auto begin()
        {
            return data.begin();
        }

        auto end()
        {
            return data.end();
        }

        auto begin() const
        {
            return data.begin();
        }

        auto end() const
        {
            return data.end();
        }
    };


} // namespace azgra::collection
