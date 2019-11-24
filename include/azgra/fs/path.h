#pragma once

#include "std_file_sytem.h"
#include <random>

namespace azgra::fs
{
    class Path
    {
    private:
        Path() = default;

    public:
        [[nodiscard]] static sfs::path concat_path(const std::vector<BasicStringView < char>>

        &paths);

        [[nodiscard]] static bool is_regular_file(const BasicStringView<char> &path);

        [[nodiscard]] static bool is_directory(const BasicStringView<char> &path);

        [[nodiscard]] static sfs::path get_temporary_directory_path();

        [[nodiscard]] static sfs::path get_temporary_file_path();

    };
}