#include <azgra/fs/path.h>

namespace azgra::fs
{
    sfs::path Path::concat_path(const VectorOfStringViews &paths)
    {
        if (paths.empty())
            return "";
        sfs::path resultPath(paths[0]);

        for (size_t i = 1; i < paths.size(); ++i)
        {
            resultPath /= paths[i];
        }
        return resultPath;
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

    sfs::path Path::get_temporary_directory_path()
    {
        return sfs::temp_directory_path();
    }

    sfs::path Path::get_temporary_file_path()
    {
        std::random_device rd;
        std::mt19937 mtGen(rd());
        std::uniform_int_distribution<azgra::u16> rand;
        const sfs::path dir = sfs::temp_directory_path();
        sfs::path tmpFile(dir / ("tmp." + std::to_string(rand(mtGen))));
        while (sfs::exists(tmpFile))
        {
            tmpFile = (dir / ("tmp." + std::to_string(rand(mtGen))));
        }
        return tmpFile;
    }
}
