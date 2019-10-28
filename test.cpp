#include <azgra/azgra.h>
#include <azgra/collection/enumerable.h>
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

//    std::vector<std::vector<Point3D<double>>> surfacePoints;
//    std::vector<std::vector<double>> x, y, z;
//    for (double i = -5; i <= 5; i += 0.25)
//    {
//        std::vector<Point3D<double>> rowPoints;
//        for (double j = -5; j <= 5; j += 0.25)
//        {
//            rowPoints.push_back(Point3D<double>(i, j, ::std::sin(::std::hypot(i, j))));
//        }
//        surfacePoints.push_back(rowPoints);
//    }
//    Plot p("surface plot", 1240, 768);
//    p.surface_plot(surfacePoints, "result.png");

    std::vector<Point2D<double>> line = {Point2D<double>(0, 0), Point2D<double>(1, 1), Point2D<double>(3, 3), Point2D<double>(5, 5)};
    std::vector<Point2D<double>> line2 = {Point2D<double>(0, 5), Point2D<double>(1, 4), Point2D<double>(3, 3), Point2D<double>(5, 0)};
    //Plot("line plot", 1280, 720).set_x_lims(-10, 10).set_y_lims(-10, 10).add_line(line, "rostouci").add_line(line2, "klesajici").save
    //("line.png");
    Plot("line plot", 1280, 720)
         .set_x_lims(-10, 10)
         .set_y_lims(-10, 10)
         .add_line(line, "rostouci")
         .add_line(line2, "klesajici")
         .display_window();

    return 0;
}
