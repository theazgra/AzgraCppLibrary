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
    using namespace azgra;
    using namespace azgra::collection;
    using namespace azgra::geometry;

    auto ps = {Point2D<f64>(10, 10), Point2D<f64>(10, 20), Point2D<f64>(20, 10)};

    Plot().add_2d_points(ps).display_window();

    return 0;
}
