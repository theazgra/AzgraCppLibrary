#include <azgra/fs/file_system.h>

int main(int argc, char **argv)
{
    using namespace azgra::fs;
    using namespace azgra;;
    if (argc < 2)
        return 0;

    BasicStringView<char> path(argv[1]);
    printf("Selected path %s\n", path.data());

    DirectoryInfo di(path);

    for (const auto &fi : di.get_files())
    {
        printf("Found file %s of size %lu\n", fi.get_filename().c_str(), fi.get_file_size());
    }



    fprintf(stdout, "Temporary dir path: %s\n", Path::get_temporary_directory_path().c_str());
    fprintf(stdout, "Temporary file path: %s\n", Path::get_temporary_file_path().c_str());

    return 0;
}
