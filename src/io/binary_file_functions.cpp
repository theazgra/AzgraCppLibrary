#include <azgra/io/binary_file_functions.h>

namespace azgra::io
{
    void dump_bytes(const ByteArray &bytes, const char *fileName)
    {
        std::ofstream dumpStream(fileName, std::ios::out | std::ios::binary);
        always_assert(dumpStream.is_open() && "Failed to open binary dump stream.");
        dumpStream.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());
        dumpStream.flush();
        dumpStream.close();
    }
}