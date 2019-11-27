#pragma once

#include "std_file_sytem.h"
#include <fstream>

namespace azgra::fs
{
    class FileInfo
    {
    private:
        bool m_empty = true;
        bool m_exists = false;
        sfs::path m_path{};

        void internal_move_file(const sfs::path &newPath);

    public:
        /**
         * Default constructor for containers.
         */
        FileInfo() = default;

        /**
         * Create FileInfo from file path.
         * @param filePath File path.
         */
        explicit FileInfo(const azgra::BasicStringView<char> &filePath);

        /**
         * Create fileinfo from std::filesystem::path.
         * @param filePath File path.
         */
        explicit FileInfo(const sfs::path &filePath);

        /**
         * Check whether specified file exists.
         * @return True if file exists.
         */
        [[nodiscard]] bool exists() const;

        /**
         * Create file if does'nt exists
         * @return True if file was created.
         */
        bool create_file();

        /**
         * Tries to delete file.
         * @return True if file was deleted. False if file doesn't exist.
         */
        bool delete_file();

        /**
         * Get file extension with dot.
         * @return File extension string with dot.
         */
        [[nodiscard]] std::string get_extension() const;

        /**
         * Get the name of the file.
         * @return Filename.
         */
        [[nodiscard]] std::string get_filename() const;

        /**
         * Get absolute path of the file.
         * @return Absolute path.
         */
        [[nodiscard]] std::string get_absolute_path() const;

        /**
         * Get the file size, 0 if file doesn't exists.
         * @return The size of the file, in bytes.
         */
        [[nodiscard]] size_t get_file_size() const;


        /**
         * Change the file size by zero filling or truncating original file.
         */
        void change_file_size(const size_t newSize) const; // NOLINT(readability-avoid-const-params-in-decls)

        /**
         * Rename the file. Only filename without path must be passed.
         * @param newName New file name. Only name.
         */
        void rename_file(const BasicStringView<char> &newName);

        /**
         * Move the file to new location with new name.
         * @param newAbsolutePath Absolute file path with file name.
         */
        void move_to_path(const BasicStringView<char> &newAbsolutePath);

        /**
         * Move the file to different directory.
         * @param directoryPath Path of destination directory.
         */
        void move_to_directory(const BasicStringView<char> &directoryPath);

        /**
         * Copy current file to destination path.
         * @param destination Full path to the copy.
         * @param overwrite True if overwrite existing file.
         * @return True if file was copied.
         */
        [[nodiscard]] bool copy_file(const BasicStringView<char> &destination, bool overwrite) const;

        /**
         * Get parent directory path.
         * @return Path of the parent directory.
         */
        [[nodiscard]] sfs::path get_directory_path() const;

        /**
         * Open stream for writing to this file.
         * @return std::ofstream of this file.
         */
        [[nodiscard]] std::ofstream get_stream_for_writing() const;

        /**
         * Open stream for reading of this file.
         * @return std::ifstream of this file.
         */
        [[nodiscard]] std::ifstream get_stream_for_reading() const;
    };
}
