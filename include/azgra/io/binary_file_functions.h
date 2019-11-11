#pragma once

#include <azgra/azgra.h>
#include <fstream>

namespace azgra::io
{
    void dump_bytes(const ByteArray &bytes, const char *fileName);
}
