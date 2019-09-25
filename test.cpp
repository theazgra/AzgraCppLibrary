#include <azgra/azgra.h>
#include <azgra/collection/linq.h>
#include <set>

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