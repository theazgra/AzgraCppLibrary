#pragma once
#include <azgra/azgra.h>
#include <fstream>

namespace azgra
{
inline void write_bytes_to_file(const ByteArray &bytes, const char *fileName)
{
    std::ofstream binaryStream(fileName, std::ios::binary | std::ios::out);
    always_assert(binaryStream.is_open());
    binaryStream.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());
}
} // namespace azgra