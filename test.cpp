#include <azgra/fs/file_system.h>
#include <azgra/collection/enumerable.h>

int main(int argc, char **argv)
{
    using namespace azgra;
    using namespace azgra::fs;
    using namespace azgra::collection;
    if (argc < 2)
        return 0;

    BasicStringView<char> path(argv[1]);
    printf("Selected path %s\n", path.data());

    DirectoryInfo di(path);

    const auto directoryFiles = di.get_files();
    const auto pdfFiles = where(directoryFiles.begin(), directoryFiles.end(), [](const FileInfo &file)
    {
        return (file.get_extension() == ".pdf");
    });

    for (const auto &fi : pdfFiles)
    {
        printf("Found file %s of size %lu\n", fi.get_filename().c_str(), fi.get_file_size());
    }

    const auto fileExtensions = select(directoryFiles.begin(), directoryFiles.end(), [](const FileInfo &file)
    {
        return file.get_extension();
    });

    for (const auto &extension : fileExtensions)
    {
        printf("Found extension: %s\n", extension.c_str());
    }

//    fprintf(stdout, "Temporary dir path: %s\n", Path::get_temporary_directory_path().c_str());
//    fprintf(stdout, "Temporary file path: %s\n", Path::get_temporary_file_path().c_str());

    return 0;
}
