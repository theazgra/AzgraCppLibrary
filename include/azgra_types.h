#pragma once

#include <cstddef>
#include <vector>


namespace azgra
{
    typedef uint8_t byte;

    // TODO(Moravec): Should we maybe rename this type to something like ByteVector?
    typedef std::vector<byte> ByteArray;

    // Azgra integer types. Because every platform has different idea about how big the type is...
    typedef int16_t i16;
    typedef uint16_t u16;

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
    typedef float_t f32;
    typedef double_t f64;

    // NOTE(Moravec): Users of this library expect the size of types to be as promised.
    static_assert(sizeof(byte) == static_cast<size_t>(1));
    static_assert(sizeof(i16) == static_cast<size_t>(2));
    static_assert(sizeof(u16) == static_cast<size_t>(2));
    static_assert(sizeof(i32) == static_cast<size_t>(4));
    static_assert(sizeof(u32) == static_cast<size_t>(4));
    static_assert(sizeof(i64) == static_cast<size_t>(8));
    static_assert(sizeof(u64) == static_cast<size_t>(8));
    static_assert(sizeof(float_t) == static_cast<size_t>(4));
    static_assert(sizeof(f32) == static_cast<size_t>(4));
    static_assert(sizeof(double_t) == static_cast<size_t>(8));
    static_assert(sizeof(f64) == static_cast<size_t>(8));

#ifdef AZGRA_CPP17
    template<typename CharType = char>
    using BasicStringView = std::basic_string_view<CharType>;
#else
    template<typename CharType>
    using BasicStringView = std::experimental::basic_string_view<CharType>;
#endif
    typedef BasicStringView<char> StringView;
}
