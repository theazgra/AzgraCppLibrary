#include "azgra/fs/file_info.h"

namespace azgra::fs
{
    FileInfo::FileInfo(const azgra::BasicStringView<char> &filePath)
    {
        m_empty = false;
        m_path = sfs::path(filePath);
        m_exists = sfs::exists(m_path);
        always_assert((!m_exists || (m_exists && sfs::is_regular_file(m_path))) && "Requests file is not regular file.");
    }

    bool FileInfo::exists() const
    {
        return m_exists;
    }

    bool FileInfo::create_file()
    {
        if (m_exists)
        {
            return false;
        }

        std::ofstream fileStream(m_path);
        fileStream.flush();
        fileStream.close();
        m_exists = true;
        return m_exists;
    }

    bool FileInfo::delete_file()
    {
        if (sfs::remove(m_path))
        {
            m_exists = false;
            return true;
        }
        return false;
    }

    std::string FileInfo::get_extension() const
    {
        return m_path.extension();
    }

    size_t FileInfo::get_file_size() const
    {
        if (m_exists)
        {
            return static_cast<size_t>(sfs::file_size(m_path));
        }
        return 0;
    }

    void FileInfo::change_file_size(const size_t newSize) const
    {
        sfs::resize_file(m_path, newSize);
    }

    void FileInfo::rename_file(const BasicStringView<char> &newName)
    {
        const sfs::path currentFileDir = m_path.parent_path();
        const sfs::path newPath = currentFileDir / sfs::path(newName);
        internal_move_file(newPath);
    }

    void FileInfo::move_to_path(const BasicStringView<char> &newAbsolutePath)
    {
        const sfs::path newPath(newAbsolutePath);
        internal_move_file(newPath);
    }

    void FileInfo::move_to_directory(const BasicStringView<char> &directoryPath)
    {
        const sfs::path destDirPath(directoryPath);
        always_assert(sfs::is_directory(destDirPath) && "Destination directory is invalid");
        const sfs::path fileName = m_path.filename();
        const sfs::path newPath = destDirPath / fileName;
        internal_move_file(newPath);
    }

    void FileInfo::internal_move_file(const sfs::path &newPath)
    {
        sfs::rename(m_path, newPath);
        m_path = newPath;
    }

    std::string FileInfo::get_filename() const
    {
        return m_path.filename().string();
    }

    bool FileInfo::copy_file(const BasicStringView<char> &destination, bool overwrite)
    {
        const sfs::path destinationPath(destination);
        const bool copyStatus = sfs::copy_file(m_path, destinationPath,
                                               overwrite ? sfs::copy_options::overwrite_existing : sfs::copy_options::none);
        return copyStatus;
    }

    std::string FileInfo::get_absolute_path() const
    {
        return sfs::absolute(m_path).string();
    }
}