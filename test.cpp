#include <azgra/azgra.h>
#include <azgra/collection/enumerable.h>
#include <set>
#include <azgra/geometry/plot.h>
#include <functional>

struct Number
{
    int n;

    Number() = default;

    explicit Number(int _n) : n(_n)
    {}

    bool operator<(const Number &x) const
    {
        return n < x.n;
    }
};


int main(int, char **)
{
#ifndef MATPLOTLIB_CPP
    fprintf(stdout,"matplotlib-cpp disabled\n");
    return 0;
#endif

    using namespace std;
    using namespace azgra::collection;

    std::vector<int> numbers = {0,1,2,3,4,5,6,7,8,9};
    vector<int> filtered = where_test(numbers, [](const int &i){ return (i % 2 == 0);});

    return 0;
}
