#pragma once
#ifdef MATPLOTLIB_CPP

#include "matplotlibcpp.h"

namespace azgra::plotting
{
    void test_plot()
    {
        std::vector<std::vector<double>> x, y, z;
        for (double i = -5; i <= 5; i += 0.25)
        {
            std::vector<double> x_row, y_row, z_row;
            for (double j = -5; j <= 5; j += 0.25)
            {
                x_row.push_back(i);
                y_row.push_back(j);
                z_row.push_back(::std::sin(::std::hypot(i, j)));
            }
            x.push_back(x_row);
            y.push_back(y_row);
            z.push_back(z_row);
        }

        matplotlibcpp::plot_surface(x, y, z);
        matplotlibcpp::save("surface.png");
    }
}
#endif
