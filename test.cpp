#include <azgra/geometry/plot.h>
#include <azgra/collection/enumerable.h>
#include <azgra/fs/file_system.h>

using namespace azgra;
using namespace azgra::geometry;
using namespace azgra::collection;

int main(int argc, char **argv)
{
    Enumerable<int> ints({1, 2, 10, 3, 4, 5, 6});
    auto maybeTen = ints.first_or_default([](const int &i)
                                          { return i == 10; });


    if (maybeTen)
    {
        fprintf(stdout, "Found 10\n");
    }
    else
    {
        fprintf(stdout, "NOT Found 10\n");
    }

    return 0;
}
