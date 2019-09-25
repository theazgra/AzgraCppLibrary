#pragma once

#include <vector>
#include <functional>
#include <boost/optional.hpp>

#define REQUIRE_NUMERIC_TYPE static_assert(                         \
                                std::is_same<T, int>() ||           \
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


namespace azgra::collection::experimental_linq
{
    class EnumerableError : public std::runtime_error
    {
    public:
        explicit EnumerableError(const char *errorMessage) : std::runtime_error(errorMessage)
        {}
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

        Enumerable<T> where(std::function<bool(const T &)> predicate) const noexcept
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
        Enumerable<SelectType> select(std::function<SelectType(const T &)> selector) const noexcept
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

        T first(std::function<bool(const T &)> predicate) const noexcept(false)
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


        boost::optional<T> first_or_default(std::function<bool(const T &)> predicate) const noexcept
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

        T single(std::function<bool(const T &)> predicate) const noexcept(false)
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

        boost::optional<T> single_or_default(std::function<bool(const T &)> predicate) const noexcept(false)
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

        bool any(std::function<bool(const T &)> predicate) const noexcept
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

        bool all(std::function<bool(const T &)> predicate) const noexcept
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

        size_t count() const noexcept
        {
            return data.size();
        }

        size_t count(std::function<bool(const T &)> predicate) const noexcept
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

        bool contains(std::function<bool(const T &)> predicate) const noexcept
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


        double average() const
        {
            REQUIRE_NUMERIC_TYPE;
            double sum = 0.0;
            size_t count = 0;
            for (const T &item : data)
            {
                sum += item;
                ++count;
            }

            return (sum / static_cast<double>(count));
        }



        // std::function<bool(const T &)> predicate
        // std::function<SelectType(const T &)> selector
        // TODO:  Distinct, Concat, Except, OrderBy, OrderByDescending, Reverse, Repeat, Sum, Take, Skip, TakeWhile, TakeLast, ForEach,
        //  Cast?
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


}