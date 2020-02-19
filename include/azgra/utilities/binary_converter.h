#pragma once
/*
* Copyright Moravec Vojtech 2019.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)
*
* This implementation is inspired by BitConverter from .NET
* https://referencesource.microsoft.com/#mscorlib/system/bitconverter.cs
*
* EVERY CONVERSION DONE HERE WORKS ONLY WITH LITTLE ENDIAN AT THIS MOMENT!
*/

#define AZGRA_LITTLE_ENDIAN

#ifndef AZGRA_LITTLE_ENDIAN
#error "Big endian is not supported yet!"
#endif // !LITTLE_ENDIAN


#include <azgra/azgra.h>
#include <string>
#include <boost/locale.hpp>
#include <locale>
#include <codecvt>
#include <bitset>

namespace azgra
{

    // Convert two bytes, from specified index, to azgra::i16.
    azgra::i16 bytes_to_i16(const ByteArray &bytes, const azgra::u64 fromIndex = 0);

    // Convert four bytes, from specified index, to azgra::i32.
    azgra::i32 bytes_to_i32(const ByteArray &bytes, const azgra::u64 fromIndex = 0);

    // Convert eight bytes, from specified index, to azgra::i64.
    azgra::i64 bytes_to_i64(const ByteArray &bytes, const azgra::u64 fromIndex = 0);

    // Convert 4 bytes, from specified index, to float.
    float bytes_to_float(const ByteArray &bytes, const azgra::u64 fromIndex = 0);

    // Convert 8 bytes, from specified index, to double.
    double bytes_to_double(const ByteArray &bytes, const azgra::u64 fromIndex = 0);

    // Convert two bytes, from specified index, to azgra::u16.
    azgra::u16 bytes_to_u16(const ByteArray &bytes, const azgra::u64 fromIndex = 0);

    // Convert four bytes, from specified index, to azgra::u32.
    azgra::u32 bytes_to_u32(const ByteArray &bytes, const azgra::u64 fromIndex = 0);

    // Convert eight bytes, from specified index, to azgra::u64.
    azgra::u64 bytes_to_u64(const ByteArray &bytes, const azgra::u64 fromIndex = 0);

    // Convert UTF-8 bytes to string.
    std::string utf8bytes_to_string(const ByteArray &bytes);

    // Convert number of UTF-8  bytes, from specified index, to string.
    std::string utf8bytes_to_string(const ByteArray &bytes, const azgra::u64 fromIndex, const azgra::u32 byteCount);

    // Convert azgra::i16 into 2 bytes.
    ByteArray short16_to_bytes(const azgra::i16 &value);

    // Convert azgra::u16 into 2 bytes.
    ByteArray ushort16_to_bytes(const azgra::u16 &value);

    // Convert azgra::i32 into 4 bytes.
    ByteArray int32_to_bytes(const azgra::i32 &value);

    // Convert azgra::u32 into 4 bytes.
    ByteArray uint32_to_bytes(const azgra::u32 &value);

    // Convert azgra::i64 into 8 bytes.
    ByteArray long64_to_bytes(const azgra::i64 &value);

    // Convert azgra::u64 into 8 bytes.
    ByteArray ulong64_to_bytes(const azgra::u64 &value);

    // Convert float into 4 bytes.
    ByteArray float_to_bytes(const float &value);

    // Convert double into 8 bytes.
    ByteArray double_to_bytes(const double &value);

    // Convert string to bytes.
    ByteArray string_to_bytes(const std::string &text);

    // Convert string to null terminated bytes.
    ByteArray string_to_bytes_null_terminated(const std::string &text);

    // Convert azgra::i32 into 4 bytes, which will be added into buffer.
    void int32_to_bytes(const azgra::i32 &i, ByteArray &buffer, size_t bufferPosition);

    // Convert azgra::u16 into 2 bytes, which will be added into buffer.
    void ushort16_to_bytes(const azgra::u16 &x, ByteArray &buffer, size_t bufferPosition);

    std::vector<azgra::i16> bytes_to_short_array(const ByteArray &data);

    std::vector<azgra::u16> bytes_to_ushort_array(const ByteArray &data);

    std::vector<azgra::i32> bytes_to_int_array(const ByteArray &data);

    std::vector<azgra::u32> bytes_to_uint_array(const ByteArray &data);

    std::vector<azgra::i64> bytes_to_long_array(const ByteArray &data);

    std::vector<azgra::u64> bytes_to_ulong_array(const ByteArray &data);

    std::vector<azgra::f32> bytes_to_float_array(const ByteArray &data);

    std::vector<azgra::f64> bytes_to_double_array(const ByteArray &data);

    ByteArray int_array_to_bytes(const std::vector<azgra::i32> &data);

    ByteArray uint_array_to_bytes(const std::vector<azgra::u32> &data);

    ByteArray ushort_array_to_bytes(const std::vector<azgra::u16> &data);


    template<
            typename T,
            typename = std::enable_if_t<std::is_integral<T>::value>
    >
    auto to_binary_representation(const T &value)
    {
        const auto bits = std::bitset<sizeof(T) * CHAR_BIT>(value);
        return bits;
    }

} // namespace azgra