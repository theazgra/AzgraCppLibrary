#include <azgra/fs/path.h>

namespace azgra::fs
{
    std::string Path::concat_path(const std::vector<BasicStringView<char>> &paths)
    {
        if (paths.empty())
            return "";
        sfs::path resultPath(paths[0]);

        for (size_t i = 1; i < paths.size(); ++i)
        {
            resultPath /= paths[i];
        }
        return sfs::absolute(resultPath).string();
    }

    bool Path::is_regular_file(const BasicStringView<char> &path)
    {
        const sfs::path tmpPath(path);
        return sfs::is_regular_file(tmpPath);
    }

    bool Path::is_directory(const BasicStringView<char> &path)
    {
        const sfs::path tmpPath(path);
        return sfs::is_directory(tmpPath);
    }
}
