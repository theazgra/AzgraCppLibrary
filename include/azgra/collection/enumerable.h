#pragma once

#include "enumerable_functions.h"
#include <random>

namespace azgra::collection
{
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
            result.data = azgra::collection::where(data.begin(), data.end(), predicate);
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

        std::optional<T> first_or_default() const noexcept
        {
            if (data.size() > 0)
            {
                return data[0];
            }
            else
            {
                return std::nullopt;
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

        std::optional<T> last_or_default() const noexcept
        {
            if (data.size() > 0)
            {
                return data[data.size() - 1];
            }
            else
            {
                return std::nullopt;
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
        std::optional<T> first_or_default(const Predicate &predicate) const noexcept
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
            return std::nullopt;
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
        std::optional<T> single_or_default(const Predicate &predicate) const noexcept(false)
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
                    return std::nullopt;
                }
            }
            else
            {
                return std::nullopt;
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
            return azgra::collection::count(data.begin(), data.end(), predicate);
        }

        template<typename Predicate>
        bool contains(const Predicate &predicate) const noexcept
        {
            return azgra::collection::contains(data.begin(), data.end(), predicate);
        }

        double average() const noexcept
        {
            static_assert(std::is_arithmetic_v<T>, "T must be numeric type");
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
            return result;
        }

        Enumerable<T> except(const Enumerable<T> &exceptSrc) const noexcept
        {
            return except(exceptSrc.data);
        }

        Enumerable<T> distinct() const noexcept
        {
            Enumerable<T> result(azgra::collection::distinct(data.begin(), data.end()));
            return result;
        }


        Enumerable<T> copy_part(const size_t size) const
        {
            if (size > data.size())
            {
                throw std::runtime_error("Requested copy of size larger than data size.");
            }

            Enumerable<T> result(data.begin(), data.begin() + size);
            return result;
        }


        Enumerable<T> copy_part(const size_t from, const size_t size) const
        {
            if ((from > data.size()) || (size > (data.size() - from)))
            {
                throw std::runtime_error("Requested copy of size larger than data size.");
            }
            const auto fromIt = data.begin() + from;
            Enumerable<T> result(fromIt, fromIt + size);
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

        void shuffle_in_place()
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(data.begin(), data.end(), gen);
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
            static_assert(std::is_integral_v<T>);

            std::vector<T> items(exclusiveTo - inclusiveFrom);
            for (auto i = inclusiveFrom; i < exclusiveTo; ++i)
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
