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
        /**
         * Check if file specified by path is regular file.
         * @param path Requested path.
         * @return True if path points to regular file.
         */
        [[nodiscard]] static bool is_regular_file(const BasicStringView<char> &path);

        /**
         * Check if file specified by path is directory.
         * @param path Requested path.
         * @return True if path points to directory.
         */
        [[nodiscard]] static bool is_directory(const BasicStringView<char> &path);

        /**
         * Get path to the temporary directory.
         * @return Temporary directory path.
         */
        [[nodiscard]] static sfs::path get_temporary_directory_path();

        /**
         * Get path to the temporary file.
         * @return Temporary file path.
         */
        [[nodiscard]] static sfs::path get_temporary_file_path();

        typedef std::vector<BasicStringView < char>> VectorOfStringViews;

        /**
         * Perform std::filesystem path concatenation on strings.
         * @param paths Strings to be concatenated.
         * @return Resulted path.
         */
        [[nodiscard]] static sfs::path concat_path(const VectorOfStringViews &paths);
    };
}