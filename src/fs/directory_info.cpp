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
        assert(m_exists && "Directory doesn't exist.");
        if (!m_exists)
        { return ""; }
        return m_path.filename().string();
    }

    std::string DirectoryInfo::get_absolute_path() const
    {
        assert(m_exists && "Directory doesn't exist.");
        return sfs::absolute(m_path).string();
    }

    DirectoryInfo DirectoryInfo::get_parent_directory() const
    {
        always_assert(m_exists && "Directory doesn't exist.");
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

    std::vector<FileInfo> DirectoryInfo::get_files(const bool followSymlinks) const
    {
        assert(m_exists && "Directory doesn't exist.");
        std::vector<FileInfo> files;
        if (!m_exists)
        {
            return files;
        }

        for (const sfs::directory_entry &entry : sfs::directory_iterator(m_path, followSymlinks
                                                                                 ? sfs::directory_options::follow_directory_symlink
                                                                                 : sfs::directory_options::none))
        {
#ifdef GCC_CXX17

            if (sfs::is_regular_file(entry))
#else
                if (sfs::is_regular_file(entry.path()))
#endif
            {
                files.emplace_back(entry.path());
            }
        }
        return files;
    }

    std::vector<DirectoryInfo> DirectoryInfo::get_subdirectories(const bool followSymlinks) const
    {
        assert(m_exists && "Directory doesn't exist.");
        std::vector<DirectoryInfo> directories;
        if (!m_exists)
        {
            return directories;
        }

        for (const sfs::directory_entry &entry : sfs::directory_iterator(m_path, followSymlinks
                                                                                 ? sfs::directory_options::follow_directory_symlink
                                                                                 : sfs::directory_options::none))
        {
#ifdef GCC_CXX17
            if (sfs::is_directory(entry))
#else
                if (sfs::is_directory(entry.path()))
#endif
            {
                directories.emplace_back(entry.path());
            }
        }
        return directories;
    }

    void DirectoryInfo::copy(const BasicStringView<char> &destination, const bool overwrite, const bool recursive) const
    {
        assert(m_exists && "Directory doesn't exist.");
        if (!m_exists)
        {
            return;
        }
        sfs::copy_options copyOptions = sfs::copy_options::none;
        if (overwrite)
        {
            copyOptions |= sfs::copy_options::overwrite_existing;
        }
        if (recursive)
        {
            copyOptions |= sfs::copy_options::recursive;
        }
        const sfs::path destinationPath(destination);
        sfs::copy(m_path, destinationPath, copyOptions);
    }

    void DirectoryInfo::internal_move_directory(const sfs::path &newPath)
    {
        always_assert(m_exists && "Directory doesn't exist.");
        sfs::rename(m_path, newPath);
        m_path = newPath;
    }

    void DirectoryInfo::rename_directory(const BasicStringView<char> &directoryName)
    {
        assert(m_exists && "Directory doesn't exist");
        if (!m_exists)
        {
            return;
        }
        const sfs::path parent = m_path.parent_path();
        const sfs::path newPath = parent / sfs::path(directoryName);
        internal_move_directory(newPath);
    }

    void DirectoryInfo::move_directory(const BasicStringView<char> &destinationPath)
    {
        assert(m_exists && "Directory doesn't exist");
        if (!m_exists)
        {
            return;
        }
        const sfs::path newPath = sfs::path(destinationPath);
        internal_move_directory(newPath);
    }

    bool DirectoryInfo::exists() const
    {
        return m_exists;
    }

    bool DirectoryInfo::delete_directory(const bool recursive)
    {
        if (recursive)
        {
            m_exists = !(sfs::remove_all(m_path) > 0);
        }
        else
        {
            m_exists = !sfs::remove(m_path);
        }
        return !(m_exists);
    }
}