#pragma once

#include "std_file_sytem.h"
#include <fstream>

namespace azgra::fs
{
    class File
    {
    public:
        static std::ofstream create_file(const azgra::BasicStringView<char> &filePath);

        static bool delete_file(const azgra::BasicStringView<char> &filePath);
    };
}
