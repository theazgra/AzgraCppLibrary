#pragma once

#include "file_info.h"

namespace azgra::fs
{
    class DirectoryInfo
    {
    private:
        bool m_empty = true;
        bool m_exists = false;
        sfs::path m_path{};

    public:
        /**
         * Default constructor for containers.
         */
        DirectoryInfo() = default;

        /**
         * Create DirectoryInfo from directory path.
         * @param directoryPath Path to the directory.
         */
        explicit DirectoryInfo(const BasicStringView<char> &directoryPath);

        /**
         * Get current working directory path.
         * @return Absolute path of working directory.
         */
        [[nodiscard]] static std::string get_working_directory();

        /**
         * Set current working directory.
         * @param directoryPath Working directory path.
         */
        static void set_working_directory(const BasicStringView<char> &directoryPath);

        //parentdir,dirfiles,movedir,copydir,removedir,absolutepath,dirname,dirfileswithfilter,createdirpath,createdir
    };
}