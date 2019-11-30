#include <azgra/fs/file.h>

namespace azgra::fs
{
    std::ofstream File::create_file(const azgra::BasicStringView<char> &filePath)
    {
        const sfs::path pth(filePath);
        std::ofstream fileStream(pth);
        return fileStream;
    }

    bool File::delete_file(const azgra::BasicStringView<char> &filePath)
    {
        const sfs::path pth(filePath);
        return sfs::remove(pth);
    }
}
