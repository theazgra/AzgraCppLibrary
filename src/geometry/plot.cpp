#include <azgra/geometry/plot.h>
#ifdef MATPLOTLIB_CPP
#include <azgra/geometry/matplotlibcpp.h>

namespace plt = matplotlibcpp;
namespace azgra::geometry
{
    Plot::Plot(azgra::BasicStringView<char> title, const azgra::u16 plotWidth, const azgra::u16 plotHeight)
    {
        m_plotTitle = std::string(title);
        m_plotWidth = plotWidth;
        m_plotHeight = plotHeight;
        plt::figure_size(m_plotWidth, m_plotHeight);
    }

    Plot Plot::add_surface_plot(const std::vector<std::vector<Point3D<double>>> &coords)
    {
        m_3d = true;
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
        plt::figure_size(m_plotWidth, m_plotHeight);
        plt::plot_surface(x, y, z);
        plt::title(m_plotTitle);
        return *this;
    }

    Plot Plot::add_2d_point(const Point2D<f64> &point)
    {
        std::vector<double> x(1), y(1);
        x[0] = point.x;
        y[0] = point.y;
        plt::plot(x, y, "o");
        return *this;
    }

    Plot Plot::add_2d_points(const std::vector<Point2D<f64>> &points)
    {
        size_t count = points.size();
        std::vector<double> x(count), y(count);
        for (size_t i = 0; i < count; ++i)
        {
            x[i] = points[i].x;
            y[i] = points[i].y;
        }

        plt::plot(x, y, "o");
        return *this;
    }

    Plot Plot::add_line(const std::vector<Point2D<double>> &coords, const azgra::BasicStringView<char> &lineName)
    {
        always_assert(!m_3d);

        const size_t coordCount = coords.size();
        std::vector<double> x(coordCount), y(coordCount);
        for (size_t i = 0; i < coordCount; ++i)
        {
            x[i] = coords[i].x;
            y[i] = coords[i].y;
        }

        if (lineName.length() > 0)
        {
            plt::named_plot(std::string(lineName), x, y);
            m_enableLegend = true;
        }
        else
        {
            plt::plot(x, y);
        }

        return *this;
    }

    Plot::Plot(azgra::BasicStringView<char> title)
    {
        m_plotTitle = std::string(title);
    }

    Plot Plot::set_x_lims(const f64 xMin, const f64 xMax)
    {
        m_setXLims = true;
        m_xMin = xMin;
        m_xMax = xMax;
        return *this;
    }

    Plot Plot::set_y_lims(const f64 yMin, const f64 yMax)
    {
        m_setYLims = true;
        m_yMin = yMin;
        m_yMax = yMax;
        return *this;
    }

    Plot Plot::set_x_ticks(const std::vector<f64> &xTicks, const std::vector<std::string> &xTickLabels)
    {
        m_xTicks = xTicks;
        m_xTickLabels = xTickLabels;
        return *this;
    }

    Plot Plot::set_y_ticks(const std::vector<f64> &yTicks, const std::vector<std::string> &yTickLabels)
    {
        m_yTicks = yTicks;
        m_yTickLabels = yTickLabels;
        return *this;
    }

    void Plot::prepare_plot()
    {
        if (m_setXLims)
        {
            plt::xlim(m_xMin, m_xMax);
        }
        if (m_setYLims)
        {
            plt::ylim(m_yMin, m_yMax);
        }
        if (!m_xTicks.empty())
        {
            plt::xticks(m_xTicks, m_xTickLabels);
        }
        if (!m_yTicks.empty())
        {
            plt::xticks(m_yTicks, m_yTickLabels);
        }

        plt::title(m_plotTitle);

        if (m_enableLegend)
        {
            plt::legend();
        }
    }

    void Plot::save(azgra::BasicStringView<char> fileName)
    {
        prepare_plot();
        plt::save(std::string(fileName));
    }

    void Plot::display_window()
    {
        prepare_plot();
        plt::show();
    }

    void Plot::animate_lines(const std::vector<std::vector<Point2D<f64>>> &linesInTime, const float animationDelay,
                             const size_t animationStep)
    {
        if (linesInTime.empty())
            return;
        plt::show();
        const size_t pointCount = linesInTime[0].size();
        std::vector<double> x(pointCount), y(pointCount), xp(pointCount), yp(pointCount);

        for (size_t i = 0; i < pointCount; ++i)
        {
            x[i] = xp[i] = linesInTime[0][i].x;
            y[i] = yp[i] = linesInTime[0][i].y;
        }
        plt::Plot animatedPlot("Animation", x, y);
        plt::plot(xp, yp, "or");

        for (size_t timePoint = 1; timePoint < linesInTime.size(); timePoint += animationStep)
        {
            const auto &points = linesInTime[timePoint];
            always_assert(points.size() == pointCount);
            for (size_t i = 0; i < pointCount; ++i)
            {
                x[i] = xp[i] = points[i].x;
                y[i] = yp[i] = points[i].y;
            }

            animatedPlot.update(x, y);
            plt::pause(animationDelay);
        }
        plt::pause(100);
    }

}
#endif
namespace azgra::geometry
{
    void dump_3d_points_history(const std::vector<std::vector<Point3D<f64>>> &pointsHistory, const char *path)
    {
        if (pointsHistory.size() <= 0)
            return;
        std::ofstream stream(path, std::ios::out);
        always_assert(stream.is_open());
        const size_t iterationCount = pointsHistory.size();

        stream << "IterationCount=" << iterationCount << "\n";

        for (const auto &points : pointsHistory)
        {
            stream << "PointCount=" << points.size() << "\n";
            for (const auto &point : points)
            {
                stream << point.x << ';' << point.y << ';' << point.z << '\n';
            }
        }
    }
}