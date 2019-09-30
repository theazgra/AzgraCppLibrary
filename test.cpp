#include <azgra/azgra.h>
#include <azgra/collection/linq.h>
#include <set>

#ifdef MATPLOTLIB_CPP
    #include "matplotlib-cpp/matplotlibcpp.h"
#endif
using namespace azgra::collection::experimental_linq;

struct Number
{
    int n;

    Number() = default;

    Number(int _n) : n(_n)
    {}

    bool operator<(const Number &x) const
    {
        return n < x.n;
    }
};


int main(int, char **)
{

#ifdef MATPLOTLIB_CPP
    fprintf(stdout,"matplotlib-cpp enabled\n");
    std::vector<std::vector<double>> x, y, z;
    for (double i = -5; i <= 5;  i += 0.25) {
        std::vector<double> x_row, y_row, z_row;
        for (double j = -5; j <= 5; j += 0.25) {
            x_row.push_back(i);
            y_row.push_back(j);
            z_row.push_back(::std::sin(::std::hypot(i, j)));
        }
        x.push_back(x_row);
        y.push_back(y_row);
        z.push_back(z_row);
    }

    matplotlibcpp::plot_surface(x, y, z);
    matplotlibcpp::save("/mnt/d/tmp/surface.png");
#else
    fprintf(stdout,"matplotlib-cpp disabled\n");
#endif
    return 0;

    std::vector<Number> nums = {Number(0), Number(1), Number(2), Number(3), Number(4), Number(5), Number(6), Number(7), Number(8)};
//    std::set<Number> nums = {Number(0), Number(1), Number(2), Number(3), Number(4), Number(5), Number(6), Number(7), Number(8)};
//    auto x = where(nums.begin(), nums.end(), static_cast<std::function<bool(const Number &)>>([](const Number &x)
//    { return x.n == 7; }));
//
//    always_assert(x[0].n == 7);
    std::function<bool(const Number &)> even = [](const Number &n)
    { return (n.n % 2 == 0); };
    Enumerable<Number> en(nums.begin(), nums.end());
    Enumerable<Number> emptyEn;

    try
    {
        Number first = emptyEn.first();
        fprintf(stdout, "First value is %i\n", first.n);
    }
    catch (azgra::collection::experimental_linq::EnumerableError &e)
    {
        fprintf(stderr, "%s\n", e.what());
    }
    std::vector<azgra::u16> vec={5,45,89,78,64,21,45,45,4};
    Enumerable<azgra::u16> xxx(vec);

    double avg = xxx.average();


    return 0;
}
