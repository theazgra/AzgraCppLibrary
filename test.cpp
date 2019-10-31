#include <azgra/azgra.h>
#include <azgra/collection/enumerable.h>


struct Human
{
    int age;
    int height;
};

int main(int, char **)
{
#ifndef MATPLOTLIB_CPP
    fprintf(stdout,"matplotlib-cpp disabled\n");
    return 0;
#endif

    using namespace azgra;
    using namespace azgra::collection;

    auto humans = Enumerable<Human>({{1,  52},
                                     {2,  80},
                                     {20, 175},
                                     {30, 181},
                                     {50, 160}});

    std::function<int(const Human &)> fn = [](const Human &h)
    { return h.height; };

    auto heighs = humans.select<int>(fn);

    return 0;
}
