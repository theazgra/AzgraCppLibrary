#pragma once

#include <type_traits>
#include <azgra/azgra.h>
#include <vector>
#include <set>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <numeric>

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
        // NOTE(Moravec): We don't have to use std::back_inserter because result vector is preallocated.
        std::transform(begin, end, result.begin(), selector);
        return result;
    }

    template<
            typename It,
            typename PredicateFunction,
            typename T = typename std::iterator_traits<It>::value_type,
            typename PredicateResultType = typename std::result_of<PredicateFunction &(T)>::type
    >
    std::vector<T> where(const It begin, const It end, PredicateFunction selector)
    {
        static_assert(std::is_same<PredicateResultType, bool>::value && "PredicateFunction must return boolean value.");
        std::vector<T> result;
        std::copy_if(begin, end, std::back_inserter(result), selector);
        return result;
    }

    template<
            typename It,
            typename T
    >
    auto sum(const It begin, const It end, const T initialValue)
    {
        return std::accumulate(begin, end, initialValue);
    }

    template<
            typename It,
            typename SelectorFunction,
            typename T = typename std::iterator_traits<It>::value_type,
            typename SelectType = typename std::result_of<SelectorFunction &(T)>::type
    >
    auto sum(const It begin, const It end, SelectorFunction selector, const SelectType initialValue)
    {
        static_assert(std::is_arithmetic<SelectType>::value, "T must be numeric type");
        SelectType sum = initialValue;
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
        static_assert(std::is_same<T, T2>::value);
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

        const auto minMaxValue = std::minmax_element(begin, end);
        return std::make_pair(*minMaxValue.first, *minMaxValue.second);
    }

    template<
            typename It,
            typename T = typename std::iterator_traits<It>::value_type
    >
    size_t count(const It begin, const It end, const T &value)
    {
        size_t result = std::count(begin, end, value);
        return result;
    }

    template<
            typename It,
            typename PredicateFunction,
            typename T = typename std::iterator_traits<It>::value_type,
            typename PredicateResultType = typename std::result_of<PredicateFunction &(T)>::type
    >
    size_t count_if(const It begin, const It end, PredicateFunction predicate)
    {
        static_assert(std::is_same<PredicateResultType, bool>::value && "PredicateFunction must return boolean value.");
        size_t result = std::count_if(begin, end, predicate);
        return result;
    }

    template<
            typename It,
            typename PredicateFunction,
            typename T = typename std::iterator_traits<It>::value_type,
            typename PredicateResultType = typename std::result_of<PredicateFunction &(T)>::type
    >
    bool all(const It begin, const It end, PredicateFunction predicate)
    {
        static_assert(std::is_same<PredicateResultType, bool>::value && "PredicateFunction must return boolean value.");
        return std::all_of(begin, end, predicate);
    }

    template<
            typename It,
            typename PredicateFunction,
            typename T = typename std::iterator_traits<It>::value_type,
            typename PredicateResultType = typename std::result_of<PredicateFunction &(T)>::type
    >
    bool any(const It begin, const It end, PredicateFunction predicate)
    {
        static_assert(std::is_same<PredicateResultType, bool>::value && "PredicateFunction must return boolean value.");
        return std::any_of(begin, end, predicate);
    }

    template<
            typename It,
            typename T = typename std::iterator_traits<It>::value_type
    >
    bool contains(const It begin, const It end, const T &value)
    {
        const It itPos = std::find(begin, end, value);
        return (itPos != end);
    }

    template<
            typename It,
            typename PredicateFunction,
            typename T = typename std::iterator_traits<It>::value_type,
            typename PredicateResultType = typename std::result_of<PredicateFunction &(T)>::type
    >
    bool contains_if(const It begin, const It end, PredicateFunction predicate)
    {
        static_assert(std::is_same<PredicateResultType, bool>::value && "PredicateFunction must return boolean value.");
        const It itPos = std::find(begin, end, predicate);
        return (itPos != end);
    }

    template<
            typename It,
            typename T = typename std::iterator_traits<It>::value_type
    >
    std::vector<T> distinct(const It begin, const It end)
    {
        std::unordered_set<T> set(begin, end);
        std::vector<T> result(set.begin(), set.end());
        return result;
//        std::sort(result);
//        const auto lastItPos = std::unique(result.begin(), result.end());
//        result.erase(lastItPos, result.end());
//        return result;
    }

    template<
            typename It,
            typename T = typename std::iterator_traits<It>::value_type
    >
    auto find_index(const It begin, const It end, const T &value)
    {
        const auto &valueIt = std::find(begin, end, value);
        return std::distance(begin, valueIt);
    }
}