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
        bool m_enableLegend = false;

        bool m_setXLims = false;
        bool m_setYLims = false;
        f64 m_xMin = 0.0;
        f64 m_xMax = 0.0;
        f64 m_yMin = 0.0;
        f64 m_yMax = 0.0;

        std::vector<f64> m_xTicks;
        std::vector<f64> m_yTicks;
        std::vector<std::string> m_xTickLabels;
        std::vector<std::string> m_yTickLabels;

        std::string m_plotTitle;
        azgra::u16 m_plotWidth = 640;
        azgra::u16 m_plotHeight = 480;

        void prepare_plot();

    public:
        Plot() = default;

        explicit Plot(azgra::BasicStringView<char> title);

        explicit Plot(azgra::BasicStringView<char> title, const azgra::u16 plotWidth, const azgra::u16 plotHeight);

        Plot add_surface_plot(const std::vector<std::vector<Point3D<f64>>> &coords);

        Plot add_line(const std::vector<Point2D<f64>> &coords, const BasicStringView<char> &lineName = "");

        Plot add_2d_point(const Point2D<f64> &point);

        Plot add_2d_points(const std::vector<Point2D<f64>> &points);


        Plot set_x_lims(const f64 xMin, const f64 xMax);

        Plot set_y_lims(const f64 yMin, const f64 yMax);

        Plot set_x_ticks(const std::vector<f64> &xTicks, const std::vector<std::string> &xTickLabels);

        Plot set_y_ticks(const std::vector<f64> &yTicks, const std::vector<std::string> &yTickLabels);

        void save(azgra::BasicStringView<char> fileName);

        void display_window();

        static void animate_lines(const std::vector<std::vector<Point2D<f64>>> &linesInTime,
                                  const float animationDelay, const size_t animationStep = 1);
    };
}
#endif
