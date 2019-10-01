#pragma once

#include <cstring>
#include <vector>
#include <azgra/always_on_assert.h>
#include <memory>
#include <limits>
#include <azgra/utilities/print_helpers.h>
#include <azgra/azgra_math.h>

#ifndef NDEBUG
#define DEBUG 1
#endif

#if __GNUC__ >= 7
#define GCC_CXX17
#endif

#define INVALID_CASE always_assert(false && "Wrong/missing case in switch statement.");

namespace azgra
{
    typedef unsigned char byte;
    typedef std::vector<byte> ByteArray;
    // Azgra integer types. Because every platform has different idea about how big the type is...

    // Azgra short types.
    typedef int16_t i16;
    typedef uint16_t u16;
    // Azgra int types.
    typedef int32_t i32;
    typedef uint32_t u32;

    // Azgra long types.
#ifdef _WIN32
    typedef signed __int64 i64;
    typedef unsigned __int64 u64;
#else
    typedef int64_t i64;
    typedef uint64_t u64;
#endif

    // Azgra float types
    typedef float f32;
    typedef double f64;


    constexpr size_t int64_t_size = sizeof(int64_t);

    static_assert(sizeof(byte) == static_cast<size_t>(1));
    static_assert(sizeof(i16) == static_cast<size_t>(2));
    static_assert(sizeof(u16) == static_cast<size_t>(2));
    static_assert(sizeof(i32) == static_cast<size_t>(4));
    static_assert(sizeof(u32) == static_cast<size_t>(4));
    static_assert(sizeof(i64) == static_cast<size_t>(8));
    static_assert(sizeof(u64) == static_cast<size_t>(8));
    static_assert(sizeof(float) == static_cast<size_t>(4));
    static_assert(sizeof(f32) == static_cast<size_t>(4));
    static_assert(sizeof(double) == static_cast<size_t>(8));
    static_assert(sizeof(f64) == static_cast<size_t>(8));

    // Since we are using also older GCC than 7 we have to define this.
#ifdef GCC_CXX17
#include <string_view>
    template<typename CharType>
    using basic_string_view__ = std::basic_string_view<CharType>;
#else
    #include <experimental/string_view>
template<typename CharType>
using basic_string_view__ = std::experimental::basic_string_view<CharType>;
#endif

};

namespace azgra
{


    template<typename T>
    inline void max(T &src, const T test)
    { src = (test > src) ? test : src; }

    template<typename T>
    inline void min(T &src, const T test)
    { src = (test < src) ? test : src; }

#define AZGRA_NAME_OF(var) (#var)

}; // namespace azgra