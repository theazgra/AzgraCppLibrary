#include <catch2/catch.hpp>
#include <azgra/matrix.h>
#include <algorithm>

TEST_CASE("uniform dimension matrix constructor", "[azgra::matrix]")
{
    const azgra::Matrix<int> matrix(5);

    REQUIRE(matrix.rows() == 5);
    REQUIRE(matrix.cols() == 5);

    const auto matrixData = matrix.get_data();
    REQUIRE(matrixData.size() == (5 * 5));
}

TEST_CASE("non-uniform dimension matrix constructor", "[azgra::matrix]")
{
    const azgra::Matrix<int> matrix(3, 2);

    REQUIRE(matrix.rows() == 3);
    REQUIRE(matrix.cols() == 2);

    const auto matrixData = matrix.get_data();
    REQUIRE(matrixData.size() == (3 * 2));
}

TEST_CASE("non-uniform dimension matrix constructor with initial value",
          "[azgra::matrix]")
{
    const azgra::Matrix<int> matrix(3, 2, 99);

    REQUIRE(matrix.rows() == 3);
    REQUIRE(matrix.cols() == 2);

    const auto matrixData = matrix.get_data();
    REQUIRE(matrixData.size() == (3 * 2));

    REQUIRE(std::all_of(matrixData.begin(), matrixData.end(),
                        [](const int val) { return val == 99; }));
}

TEST_CASE("non-uniform dimension matrix constructor with data move",
          "[azgra::matrix]")
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const azgra::Matrix<int> matrix(3, 3, data);
    const auto matrixData = matrix.get_data();

    for (size_t i = 0; i < matrixData.size(); i++)
    {
        REQUIRE(matrixData[i] == (i + 1));
    }
}

/*
    Working with matrix
    1   2   3   4
    5   6   7   8
    9  10  11  12
    13 14  15  16
*/

TEST_CASE("at(row,col) function - row based matrix",
          "[azgra::matrix]")
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    const azgra::Matrix<int, true> matrix(4, 4, data);

    REQUIRE(matrix.at(0, 0) == 1);
    REQUIRE(matrix.at(0, 3) == 4);
    REQUIRE(matrix.at(2, 2) == 11);
    REQUIRE(matrix.at(2, 3) == 12);
    REQUIRE(matrix.at(3, 0) == 13);
    REQUIRE(matrix.at(3, 3) == 16);
}

TEST_CASE("at(row,col) function - column based matrix",
          "[azgra::matrix]")
{
    std::vector<int> data = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

    const azgra::Matrix<int, false> matrix(4, 4, data);

    REQUIRE(matrix.at(0, 0) == 1);
    REQUIRE(matrix.at(0, 3) == 4);
    REQUIRE(matrix.at(2, 2) == 11);
    REQUIRE(matrix.at(2, 3) == 12);
    REQUIRE(matrix.at(3, 0) == 13);
    REQUIRE(matrix.at(3, 3) == 16);
}

TEST_CASE("row iterator - row based matrix",
          "[azgra::matrix]")
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    const azgra::Matrix<int, true> matrix(4, 4, data);

    const std::vector<int> expected_row_0 = {1, 2, 3, 4};
    const std::vector<int> expected_row_2 = {9, 10, 11, 12};

    size_t index = 0;
    const auto row_0_end = matrix.row_cend(0);
    for (auto it = matrix.row_cbegin(0); it != row_0_end; ++it)
    {
        REQUIRE(*it == expected_row_0[index++]);
    }

    index = 0;
    const auto row_2_end = matrix.row_cend(2);
    for (auto it = matrix.row_cbegin(2); it != row_2_end; ++it)
    {
        REQUIRE(*it == expected_row_2[index++]);
    }
}

TEST_CASE("row iterator - column based matrix",
          "[azgra::matrix]")
{
    std::vector<int> data = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

    const azgra::Matrix<int, false> matrix(4, 4, data);

    const std::vector<int> expected_row_0 = {1, 2, 3, 4};
    const std::vector<int> expected_row_2 = {9, 10, 11, 12};

    size_t index = 0;
    const auto row_0_end = matrix.row_cend(0);
    for (auto it = matrix.row_cbegin(0); it != row_0_end; ++it)
    {
        REQUIRE(*it == expected_row_0[index++]);
    }

    index = 0;
    const auto row_2_end = matrix.row_cend(2);
    for (auto it = matrix.row_cbegin(2); it != row_2_end; ++it)
    {
        REQUIRE(*it == expected_row_2[index++]);
    }
}

TEST_CASE("column iterator - row based matrix",
          "[azgra::matrix]")
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    const azgra::Matrix<int, true> matrix(4, 4, data);

    const std::vector<int> expected_col_0 = {1, 5, 9, 13};
    const std::vector<int> expected_col_2 = {3, 7, 11, 15};

    size_t index = 0;
    const auto col_0_end = matrix.col_cend(0);
    for (auto it = matrix.col_cbegin(0); it != col_0_end; ++it)
    {
        REQUIRE(*it == expected_col_0[index++]);
    }

    index = 0;
    const auto col_2_end = matrix.col_cend(2);
    for (auto it = matrix.col_cbegin(2); it != col_2_end; ++it)
    {
        REQUIRE(*it == expected_col_2[index++]);
    }
}

TEST_CASE("column iterator - column based matrix",
          "[azgra::matrix]")
{
    std::vector<int> data = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

    const azgra::Matrix<int, false> matrix(4, 4, data);

    const std::vector<int> expected_col_0 = {1, 5, 9, 13};
    const std::vector<int> expected_col_2 = {3, 7, 11, 15};

    size_t index = 0;
    const auto col_0_end = matrix.col_cend(0);
    for (auto it = matrix.col_cbegin(0); it != col_0_end; ++it)
    {
        REQUIRE(*it == expected_col_0[index++]);
    }

    index = 0;
    const auto col_2_end = matrix.col_cend(2);
    for (auto it = matrix.col_cbegin(2); it != col_2_end; ++it)
    {
        REQUIRE(*it == expected_col_2[index++]);
    }
}

TEST_CASE("equals, operator==",
          "[azgra::matrix]")
{
    std::vector<int> data = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
    const azgra::Matrix<int, false> matrix(4, 4, data);

    std::vector<int> data2 = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
    const azgra::Matrix<int, false> matrix2(4, 4, data2);

    REQUIRE(matrix.equals(matrix2));
    REQUIRE(matrix2.equals(matrix));

    REQUIRE(matrix == matrix2);
    REQUIRE(matrix2 == matrix);
}

TEST_CASE("not equals, operator!=",
          "[azgra::matrix]")
{
    std::vector<int> data = {1, 4, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 7};
    const azgra::Matrix<int, false> matrix(4, 4, data);

    std::vector<int> data2 = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
    const azgra::Matrix<int, false> matrix2(4, 4, data2);

    REQUIRE(!matrix.equals(matrix2));
    REQUIRE(!matrix2.equals(matrix));

    REQUIRE(matrix != matrix2);
    REQUIRE(matrix2 != matrix);
}

TEST_CASE("operator+,operator-",
          "[azgra::matrix]")
{
    std::vector<int> data = {4, 4, 4, 4};
    azgra::Matrix<int> matrix(2, 2, data);

    std::vector<int> data2 = {2, 2, 2, 2};
    const azgra::Matrix<int> matrix2(2, 2, data2);

    SECTION("operator+")
    {
        const auto addResult = matrix + matrix2;

        REQUIRE(addResult.rows() == 2);
        REQUIRE(addResult.cols() == 2);

        const auto addResultData = addResult.get_data();

        REQUIRE(std::all_of(addResultData.begin(), addResultData.end(),
                            [](const int val) { return val == (4 + 2); }));
    }

    SECTION("operator-")
    {
        const auto subResult = matrix - matrix2;

        REQUIRE(subResult.rows() == 2);
        REQUIRE(subResult.cols() == 2);

        const auto subResultData = subResult.get_data();

        REQUIRE(std::all_of(subResultData.begin(), subResultData.end(),
                            [](const int val) { return val == (4 - 2); }));
    }
}

TEST_CASE("operator+=,operator-=",
          "[azgra::matrix]")
{
    std::vector<int> data = {4, 3, 2, 1};
    azgra::Matrix<int> matrix(2, 2, data);

    REQUIRE(matrix.at(0, 0) == 4);
    REQUIRE(matrix.at(0, 1) == 3);
    REQUIRE(matrix.at(1, 0) == 2);
    REQUIRE(matrix.at(1, 1) == 1);

    SECTION("operator+=")
    {
        const azgra::Matrix<int> addMatrix(2, 2, 5);

        matrix += addMatrix;

        REQUIRE(matrix.at(0, 0) == (4 + 5));
        REQUIRE(matrix.at(0, 1) == (3 + 5));
        REQUIRE(matrix.at(1, 0) == (2 + 5));
        REQUIRE(matrix.at(1, 1) == (1 + 5));
    }

    SECTION("operator-=")
    {
        const azgra::Matrix<int> subMatrix(2, 2, 7);

        matrix -= subMatrix;

        REQUIRE(matrix.at(0, 0) == (4 - 7));
        REQUIRE(matrix.at(0, 1) == (3 - 7));
        REQUIRE(matrix.at(1, 0) == (2 - 7));
        REQUIRE(matrix.at(1, 1) == (1 - 7));
    }
}
// operator*=
// row copy - row based
//          - col based
// set row  - row based
//          - col based
// col copy - row based
//          - col based
// set col  - row based
//          - col based