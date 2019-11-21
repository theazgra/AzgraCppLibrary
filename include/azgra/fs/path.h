#pragma once

#include "std_file_sytem.h"

namespace azgra::fs
{
    class Path
    {
    private:
        Path() = default;

    public:
        [[nodiscard]] static std::string concat_path(const std::vector<BasicStringView<char>> &paths);

        [[nodiscard]] static bool is_regular_file(const BasicStringView<char> &path);

        [[nodiscard]] static bool is_directory(const BasicStringView<char> &path);
    };
}