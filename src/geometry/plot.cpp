#ifdef MATPLOTLIB_CPP

#include <azgra/geometry/plot.h>
#include <azgra/geometry/matplotlibcpp.h>

namespace plt = matplotlibcpp;
namespace azgra::geometry
{
    Plot::Plot(azgra::basic_string_view__<char> title, const azgra::u16 plotWidth, const azgra::u16 plotHeight)
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

    Plot Plot::add_line(const std::vector<Point2D<double>> &coords, const azgra::basic_string_view__<char> &lineName)
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

        //TODO:
        // Set x-axis to interval [0,1000000]
        //plt::xlim(0, 1000 * 1000);
        return *this;
    }

    Plot::Plot(azgra::basic_string_view__<char> title)
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

    void Plot::save(azgra::basic_string_view__<char> fileName)
    {
        prepare_plot();
        plt::save(std::string(fileName));
    }

    void Plot::display_window()
    {
        prepare_plot();
        plt::show();
    }
}
#endif