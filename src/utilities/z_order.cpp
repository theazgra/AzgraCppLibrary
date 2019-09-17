#include <azgra/utilities/z_order.h>

namespace azgra
{
std::vector<PointWithIndex> generate_ordered_z_order_indices(const azgra::u32 colCount, const azgra::u32 rowCount, const azgra::u32 componentCount)
{
    std::vector<PointWithIndex> result;
    result.resize((colCount * componentCount) * rowCount);

    // This doesnt count with bytes per pixel
    size_t index = 0;
    for (azgra::u32 row = 0; row < rowCount; row++)
    {
        for (azgra::u32 col = 0; col < (colCount * componentCount); col++)
        {
            result[index] = PointWithIndex(col, row, index);
            index++;
        }
    }

    always_assert(((azgra::u64)(colCount * componentCount) * (azgra::u64)rowCount) == result.size());
    std::sort(result.begin(), result.end());

    return result;
}

ByteArray reorder_bytes_to_z_order(const ByteArray &bytes, const std::vector<PointWithIndex> &zIndices,
                                   const azgra::u32 componentSize)
{
    ByteArray zOrderedBytes;
    zOrderedBytes.resize(bytes.size());
    size_t it = 0;
    size_t from, pos;
    for (const PointWithIndex &index : zIndices)
    {
        from = (index.bufferPosition * componentSize);
        //zOrderedBytes.insert(zOrderedBytes.end(), fromIt, toIt);
        pos = it * componentSize;
        azgra::collection::vector_insert_at(zOrderedBytes, bytes, pos, from, componentSize);
        it++;
    }

    always_assert(zOrderedBytes.size() == bytes.size());
    return zOrderedBytes;
}

ByteArray reorder_bytes_from_z_order(const ByteArray &zOrderedBytes, const std::vector<PointWithIndex> &zIndices,
                                     const azgra::u32 componentSize)
{
    ByteArray bytes;
    bytes.resize(zOrderedBytes.size());

    size_t it = 0;
    size_t insertAt, copyFrom;
    for (const PointWithIndex &index : zIndices)
    {
        insertAt = index.bufferPosition * componentSize;
        copyFrom = it * componentSize;
        azgra::collection::vector_insert_at(bytes, zOrderedBytes, insertAt, copyFrom, componentSize);
        ++it;
    }
    always_assert(bytes.size() == zOrderedBytes.size());

    return bytes;
}
}