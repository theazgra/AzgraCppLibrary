#pragma once
#ifdef MATPLOTLIB_CPP

#include <azgra/azgra.h>
#include <string_view>
#include "point.h"

namespace azgra::geometry
{
    class Plot
    {
    private:
        bool m_3d = false;
    public:
//        Plot() = default;
//        Plot(az)
        static void surface_plot(const std::vector<std::vector<Point3D<double>>> &coords, const std::string_view &fileName);
        static void line(const std::vector<Point2D<double>> &coords, const std::string_view &fileName);
    };
}
#endif
