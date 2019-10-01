#ifdef MATPLOTLIB_CPP

#include <azgra/geometry/plot.h>
#include <azgra/geometry/matplotlibcpp.h>

namespace plt = matplotlibcpp;
namespace azgra::geometry
{
    void Plot::surface_plot(const std::vector<std::vector<Point3D<double>>> &coords, const std::string_view &fileName)
    {
        std::vector<std::vector<double>> x, y, z;

        for (const std::vector<Point3D<double>> &rowCoords : coords)
        {
            std::vector<double> x_row, y_row, z_row;
            for (const Point3D<double> &point : rowCoords)
            {
                x_row.push_back(point.x);
                y_row.push_back(point.y);
                z_row.push_back(point.z);
            }
            x.push_back(x_row);
            y.push_back(y_row);
            z.push_back(z_row);
        }
        plt::plot_surface(x, y, z);
        plt::save(std::string(fileName));
    }

    void Plot::line(const std::vector<Point2D<double>> &coords, const std::string_view &fileName)
    {

    }
}
#endif