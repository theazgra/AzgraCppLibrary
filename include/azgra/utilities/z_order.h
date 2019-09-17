#pragma once
#include <azgra/azgra.h>
#include <algorithm>
#include <azgra/collection/vector_utilities.h>
/*
    Interleave implementation based on:
        https://lemire.me/blog/2018/01/08/how-fast-can-you-bit-interleave-32-bit-integers/
*/

namespace azgra
{

static inline azgra::u64 interleave_uint_with_zeros(azgra::u32 input)
{
    azgra::u64 word = input;
    word = (word ^ (word << 16)) & 0x0000ffff0000ffff;
    word = (word ^ (word << 8)) & 0x00ff00ff00ff00ff;
    word = (word ^ (word << 4)) & 0x0f0f0f0f0f0f0f0f;
    word = (word ^ (word << 2)) & 0x3333333333333333;
    word = (word ^ (word << 1)) & 0x5555555555555555;
    return word;
}

static inline azgra::u32 deinterleave_lowuint32(azgra::u64 word)
{
    word &= 0x5555555555555555;
    word = (word ^ (word >> 1)) & 0x3333333333333333;
    word = (word ^ (word >> 2)) & 0x0f0f0f0f0f0f0f0f;
    word = (word ^ (word >> 4)) & 0x00ff00ff00ff00ff;
    word = (word ^ (word >> 8)) & 0x0000ffff0000ffff;
    word = (word ^ (word >> 16)) & 0x00000000ffffffff;
    return (azgra::u32)word;
}

inline azgra::u64 interleave(azgra::u32 x, azgra::u32 y)
{
    return interleave_uint_with_zeros(x) | (interleave_uint_with_zeros(y) << 1);
}

inline std::pair<azgra::u32, azgra::u32> deinterleave(azgra::u64 input)
{
    azgra::u32 x = deinterleave_lowuint32(input);
    azgra::u32 y = deinterleave_lowuint32(input >> 1);
    return std::make_pair(x, y);
}

struct PointWithIndex
{
    azgra::u32 x;
    azgra::u32 y;
    size_t bufferPosition;
    azgra::u64 z;

    PointWithIndex() {}

    // Create interleaved Z order index from X and Y coordinate.
    PointWithIndex(azgra::u32 _x, azgra::u32 _y, size_t _bufferPosition)
    {
        x = _x;
        y = _y;
        bufferPosition = _bufferPosition;
        z = interleave(x, y);
    }

    bool operator==(const PointWithIndex &other) const { return (z == other.z); }
    bool operator!=(const PointWithIndex &other) const { return (z != other.z); }
    bool operator<(const PointWithIndex &other) const { return (z < other.z); }
    bool operator<=(const PointWithIndex &other) const { return (z <= other.z); }
    bool operator>(const PointWithIndex &other) const { return (z > other.z); }
    bool operator>=(const PointWithIndex &other) const { return (z >= other.z); }
};

std::vector<PointWithIndex> generate_ordered_z_order_indices(const azgra::u32 colCount, const azgra::u32 rowCount, const azgra::u32 componentCount);

ByteArray reorder_bytes_to_z_order(const ByteArray &bytes, const std::vector<PointWithIndex> &zIndices,
                                   const azgra::u32 componentSize);
ByteArray reorder_bytes_from_z_order(const ByteArray &zOrderedBytes, const std::vector<PointWithIndex> &zIndices,
                                     const azgra::u32 componentSize);
} // namespace azgra