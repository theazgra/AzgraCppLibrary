#pragma once

/*
* Copyright Moravec Vojtech 2019.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)
*/

#include <azgra/azgra.h>
#include "boost/algorithm/string.hpp"

#ifdef GCC_8
#include <filesystem>
#else
#include <experimental/filesystem>
#endif


namespace azgra
{
    namespace fs
    {
        constexpr char CziExtUpperCase[] = ".CZI";

        struct SmallFileInfo
        {
            // Name of the file.
            std::string name;
            // Whole path to file.
            std::string path;
        };

        // Check if given path points to file.
        bool is_file(const std::string &pathToCheck);

        // Check if given path points to directory.
        bool is_directory(const std::string &pathToCheck);

        // Create all missing directories in path.
        bool create_directory_path(const std::string &desiredPath);

        // Get name of the file, specified by its path.
        std::string get_filename(const std::string &selectedPath);

        // Get name, without extension, of the file, specified by its path.
        std::string get_filename_without_extension(const std::string &selectedPath);

        // Get files, whose names begin with prefix.
        std::vector<SmallFileInfo> get_files_with_same_prefix(const std::vector<SmallFileInfo> &files, const std::string &prefix);

        // Get all regular files from directory.
        std::vector<SmallFileInfo> get_files_in_directory(const std::string &directoryPath, const bool onlyCziFiles);

        std::string get_parent_directory(const std::string &filePath);

        // Get all regular files from parent directory of given file.
//        std::vector<SmallFileInfo> get_files_in_parent_directory(const std::string &filePath, const bool onlyCziFiles);
    }; // namespace fs
}; // namespace azgra