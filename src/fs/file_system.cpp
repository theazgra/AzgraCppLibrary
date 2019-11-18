#include <azgra/fs/file_system.h>


/*
* Copyright Moravec Vojtech 2019.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)
*/

namespace azgra
{

    namespace fs
    {

#ifdef GCC_8
        namespace stdFs = std::filesystem;
#else
        namespace stdFs = std::experimental::filesystem;
#endif

        // Check if given path points to file.
        bool is_file(const std::string &pathToCheck)
        {
            auto file = stdFs::path(pathToCheck);
            return (stdFs::exists(file) && stdFs::is_regular_file(file));
        }

        // Check if given path points to directory.
        bool is_directory(const std::string &pathToCheck)
        {
            return stdFs::is_directory(stdFs::path(pathToCheck));
        }

        // Create all missing directories in path.
        bool create_directory_path(const std::string &desiredPath)
        {
            return stdFs::create_directories(stdFs::path(desiredPath));
        }

        // Get name of the file, specified by its path.
        std::string get_filename(const std::string &selectedPath)
        {
            return stdFs::path(selectedPath).filename().string();
        }

        // Get name, without extension, of the file, specified by its path.
        std::string get_filename_without_extension(const std::string &selectedPath)
        {
            stdFs::path file(selectedPath);
            auto name = file.filename().string();
            auto ext = file.extension().string();
            return name.substr(0, name.length() - ext.length());
        }

        // Get files, whose names begin with prefix.
        std::vector<SmallFileInfo> get_files_with_same_prefix(const std::vector<SmallFileInfo> &files, const std::string &prefix)
        {
            std::vector<SmallFileInfo> result;

            for (const SmallFileInfo &file : files)
            {
                if (boost::algorithm::starts_with(file.name, prefix))
                {
                    result.push_back(file);
                }
            }

            return result;
        }

//        // Get all regular files from directory.
//        std::vector<SmallFileInfo> get_files_in_directory(const std::string &directoryPath, const bool onlyCziFiles)
//        {
//            stdFs::path refDirectory(directoryPath);
//            always_assert(stdFs::is_directory(refDirectory));
//
//            std::vector<SmallFileInfo> files;
//
//            stdFs::path entryPath;
//            for (auto &entry : stdFs::directory_iterator(refDirectory))
//            {
//                entryPath = entry.path();
//                if (stdFs::is_regular_file(entryPath))
//                {
//                    if (!onlyCziFiles || (boost::to_upper_copy(stdFs::extension(entryPath)) == CziExtUpperCase))
//                    {
//                        SmallFileInfo fi = {};
//                        fi.name = entryPath.filename().string();
//                        fi.path = entryPath.string();
//                        files.push_back(fi);
//                    }
//                }
//            }
//
//            return files;
//        }

        std::string get_parent_directory(const std::string &filePath)
        {
            stdFs::path refFile(filePath);
            always_assert(stdFs::is_regular_file(refFile));
            always_assert(refFile.has_parent_path());

            stdFs::path dir = refFile.parent_path();
            return dir.string();
        }

        // Get all regular files from parent directory of given file.
        std::vector<SmallFileInfo> get_files_in_parent_directory(const std::string &filePath, const bool onlyCziFiles)
        {
            stdFs::path refFile(filePath);
            always_assert(stdFs::is_regular_file(refFile));
            always_assert(refFile.has_parent_path());

            stdFs::path dir = refFile.parent_path();

            return get_files_in_directory(dir.string(), onlyCziFiles);
        }
    }; // namespace fs
}; // namespace azgra