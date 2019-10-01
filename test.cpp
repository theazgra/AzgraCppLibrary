#include <azgra/azgra.h>
#include <azgra/collection/linq.h>
#include <set>
#include <azgra/geometry/plot.h>

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
    using namespace azgra::geometry;

    std::vector<std::vector<Point3D<double>>> surfacePoints;
    std::vector<std::vector<double>> x, y, z;
    for (double i = -5; i <= 5; i += 1.0)
    {
        std::vector<Point3D<double>> rowPoints;
        for (double j = -5; j <= 5; j += 1.0)
        {
            rowPoints.push_back(Point3D<double>(i,j,::std::sin(::std::hypot(i, j))));
        }
        surfacePoints.push_back(rowPoints);
    }
    Plot::surface_plot(surfacePoints, "result.png");

    return 0;
}
