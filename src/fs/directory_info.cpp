#include <azgra/fs/directory_info.h>

namespace azgra::fs
{

    DirectoryInfo::DirectoryInfo(const BasicStringView<char> &directoryPath)
    {
        m_empty = false;
        m_path = sfs::path(directoryPath);
        m_exists = sfs::exists(m_path);
        always_assert((!m_exists || (m_exists && sfs::is_directory(m_path))) && "Directory path is invalid.");
    }

    std::string DirectoryInfo::get_working_directory()
    {
        return sfs::absolute(sfs::current_path()).string();
    }

    void DirectoryInfo::set_working_directory(const BasicStringView<char> &directoryPath)
    {
        sfs::current_path(sfs::path(directoryPath));
    }
}