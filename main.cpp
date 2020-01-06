#include <sstream>
#include <azgra/matrix.h>

#include <algorithm>
#include <numeric>

template<typename T>
static void print_vector(const std::vector<T> &v)
{
    std::stringstream ss;

    for (const T &x : v)
    {
        ss << x << ' ';
    }
    fprintf(stdout, "%s\n", ss.str().c_str());
}

template<typename T, bool X>
static void print_matrix(const azgra::Matrix<T, X> &mat)
{
    std::stringstream ss;

    for (size_t r = 0; r < mat.rows(); ++r)
    {
        for (size_t c = 0; c < mat.cols(); ++c)
        {
            ss << mat.at(r, c) << ' ';
        }
        ss << '\n';
    }

    fprintf(stdout, "%s", ss.str().c_str());
}


int main(int argc, char **argv)
{
    using namespace azgra;

    std::vector<int> vals = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    Matrix<int, false> m(3, 3, vals);

    print_matrix(m);


//    const size_t row = 1;
//    const size_t col = 1;
//
//    std::stringstream ss;
//    ss << "Row: " << row << '\n';
//    const auto rowEnd = m.row_cend(row);
//    for (auto rowIt = m.row_cbegin(row); rowIt != rowEnd; ++rowIt)
//    {
//        ss << *rowIt << ' ';
//    }
//    ss << '\n';
//
//    ss << "Col: " << col << '\n';
//    const auto colEnd = m.col_cend(col);
//    for (auto colIt = m.col_cbegin(col); colIt != colEnd; ++colIt)
//    {
//        ss << *colIt << ' ';
//    }
//    fprintf(stdout, "%s\n\n", ss.str().c_str());
//    //////////////////////////////////////////////
//    const auto rowEnd2 = m.row_end(row);
//    for (auto rowIt = m.row_begin(row); rowIt != rowEnd2; ++rowIt)
//    {
//        *rowIt = 15;
//    }
//
//    const auto colEnd2 = m.col_cend(col);
//    for (auto colIt = m.col_begin(col); colIt != colEnd2; ++colIt)
//    {
//        *colIt = 55;
//    }
//    print_matrix(m);
    fprintf(stdout, "\n");
    std::vector<int> set_last_row = {77, 77, 77};
    m.row(2, set_last_row.begin(), set_last_row.end());
    const auto lastRow = m.row(2);
    print_vector(lastRow);

    std::vector<int> set_last_col = {99, 99, 99};
    m.col(2, set_last_col.begin(), set_last_col.end());

    fprintf(stdout, "\n");
    const auto lastCol = m.col(2);
    print_vector(lastCol);
    print_matrix(m);

    Matrix<int, false> m2 = std::move(m);

    return 0;
}
