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
		// Check if given path points to file.
		bool is_file(const std::string& pathToCheck)
		{
			auto file = boost::filesystem::path(pathToCheck);
			return (boost::filesystem::exists(file) && boost::filesystem::is_regular_file(file));
		}

		// Check if given path points to directory.
		bool is_directory(const std::string& pathToCheck)
		{
			return boost::filesystem::is_directory(boost::filesystem::path(pathToCheck));
		}

		// Create all missing directories in path.
		bool create_directory_path(const std::string& desiredPath)
		{
			return boost::filesystem::create_directories(boost::filesystem::path(desiredPath));
		}

		// Get name of the file, specified by its path.
		std::string get_filename(const std::string& selectedPath)
		{
			return boost::filesystem::path(selectedPath).filename().string();
		}

		// Get name, without extension, of the file, specified by its path.
		std::string get_filename_without_extension(const std::string& selectedPath)
		{
			boost::filesystem::path file(selectedPath);
			auto name = file.filename().string();
			auto ext = file.extension().string();
			return name.substr(0, name.length() - ext.length());
		}

		// Get files, whose names begin with prefix.
		std::vector<SmallFileInfo> get_files_with_same_prefix(const std::vector<SmallFileInfo>& files, const std::string& prefix)
		{
			std::vector<SmallFileInfo> result;

			for (const SmallFileInfo& file : files)
			{
				if (boost::algorithm::starts_with(file.name, prefix))
				{
					result.push_back(file);
				}
			}

			return result;
		}
		// Get all regular files from directory.
		std::vector<SmallFileInfo> get_files_in_directory(const std::string& directoryPath, const bool onlyCziFiles)
		{
			boost::filesystem::path refDirectory(directoryPath);
			always_assert(boost::filesystem::is_directory(refDirectory));

			std::vector<SmallFileInfo> files;

			boost::filesystem::path entryPath;
			for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(refDirectory))
			{
				entryPath = entry.path();
				if (boost::filesystem::is_regular_file(entryPath))
				{
					if (!onlyCziFiles || (boost::to_upper_copy(boost::filesystem::extension(entryPath)) == CziExtUpperCase))
					{
						SmallFileInfo fi = {};
						fi.name = entryPath.filename().string();
						fi.path = entryPath.string();
						files.push_back(fi);
					}
				}
			}

			return files;
		}

		std::string get_parent_directory(const std::string& filePath)
		{
			boost::filesystem::path refFile(filePath);
			always_assert(boost::filesystem::is_regular_file(refFile));
			always_assert(refFile.has_parent_path());

			boost::filesystem::path dir = refFile.parent_path();
			return dir.string();
		}

		// Get all regular files from parent directory of given file.
		std::vector<SmallFileInfo> get_files_in_parent_directory(const std::string& filePath, const bool onlyCziFiles)
		{
			boost::filesystem::path refFile(filePath);
			always_assert(boost::filesystem::is_regular_file(refFile));
			always_assert(refFile.has_parent_path());

			boost::filesystem::path dir = refFile.parent_path();

			return get_files_in_directory(dir.string(), onlyCziFiles);
		}
	}; // namespace fs
}; // namespace azgra