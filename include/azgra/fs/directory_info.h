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
         * Create DirectoryInfo from std::filesystem::path.
         * @param directoryPath Path to the directory.
         */
        explicit DirectoryInfo(const sfs::path &dirPath);

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

        /**
         * Get the name of directory.
         * @return Name of directory.
         */
        [[nodiscard]] std::string get_directory_name() const;

        /**
         * Get absolute path to this directory.
         * @return Absolute directory path.
         */
        [[nodiscard]] std::string get_absolute_path() const;

        /**
         * Get DirectoryInfo of parent directory.
         * @return
         */
        [[nodiscard]] DirectoryInfo get_parent_directory() const;

        /**
         * Create this directory. Parent directory must exist.
         * @return True if new directory was created.
         */
        bool create_directory();

        /**
         * Create this directory and all missing parent directories.
         * @return True if new directory was created.
         */
        bool create_directory_path();

        /**
         * Get vector of files inside this directory.
         * @return Vector of FileInfos
         */

        [[nodiscard]] std::vector<FileInfo> get_directory_files() const;

        /**
         * Get vector of subdirectories inside this directory.
         * @return Vector of DirectoryInfos
         */
        [[nodiscard]] std::vector<DirectoryInfo> get_subdirectories() const;

        //dirfiles,movedir,copydir,removedir,dirfileswithfilter
    };
}