#include <azgra/fs/file_system.h>

int main(int argc, char **argv)
{
    using namespace azgra::fs;

    fprintf(stdout, "Temporary dir path: %s\n", Path::get_temporary_directory_path().c_str());
    fprintf(stdout, "Temporary file path: %s\n", Path::get_temporary_file_path().c_str());

    return 0;
}
