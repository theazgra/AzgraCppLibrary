#include <azgra/fs/directory_info.h>

namespace azgra::fs
{

    DirectoryInfo::DirectoryInfo(const sfs::path &dirPath)
    {
        m_empty = false;
        m_path = dirPath;
        m_exists = sfs::exists(m_path);
        always_assert((!m_exists || (m_exists && sfs::is_directory(m_path))) && "Directory path is invalid.");
    }

    DirectoryInfo::DirectoryInfo(const BasicStringView<char> &directoryPath)
            : DirectoryInfo(sfs::path(directoryPath))
    {
    }

    std::string DirectoryInfo::get_working_directory()
    {
        return sfs::absolute(sfs::current_path()).string();
    }

    void DirectoryInfo::set_working_directory(const BasicStringView<char> &directoryPath)
    {
        sfs::current_path(sfs::path(directoryPath));
    }

    std::string DirectoryInfo::get_directory_name() const
    {
        assert(m_exists);
        return m_path.filename().string();
    }

    std::string DirectoryInfo::get_absolute_path() const
    {
        assert(m_exists);
        return sfs::absolute(m_path).string();
    }

    DirectoryInfo DirectoryInfo::get_parent_directory() const
    {
        assert(m_exists);
        return DirectoryInfo(m_path.parent_path());
    }

    bool DirectoryInfo::create_directory()
    {
        if (m_exists)
        {
            return false;
        }
        if (sfs::create_directory(m_path))
        {
            m_exists = true;
            return true;
        }
        return false;
    }

    bool DirectoryInfo::create_directory_path()
    {
        if (m_exists)
        {
            return false;
        }
        if (sfs::create_directories(m_path))
        {
            m_exists = true;
            return true;
        }
        return false;
    }

    std::vector<FileInfo> DirectoryInfo::get_directory_files() const
    {
        assert(m_exists);
        if (!m_exists)
        {
            return std::vector<FileInfo>();
        }
        always_assert(false && "Missing implementation!");
    }

    std::vector<DirectoryInfo> DirectoryInfo::get_subdirectories() const
    {
        assert(m_exists);
        if (!m_exists)
        {
            return std::vector<DirectoryInfo>();
        }
        always_assert(false && "Missing implementation!");
    }


}