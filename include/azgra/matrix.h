#pragma once

#include <azgra/azgra.h>

namespace azgra
{
    template<typename T>
    class Matrix
    {
    private:
        size_t rowCount = 0;
        size_t colCount = 0;
        std::vector<T> data;


        T &at(const size_t &index)
        {
            return data[index];
        }

        const T &at(const size_t &index) const
        {
            return data[index];
        }

    public:
        Matrix() = default;

        explicit Matrix(const Matrix<T> &copySrc)
        {
            rowCount = copySrc.rowCount;
            colCount = copySrc.colCount;
            data = std::vector<T>(copySrc.data.begin(), copySrc.data.end());
        }

        explicit Matrix(size_t dimension)
        {
            rowCount = dimension;
            colCount = dimension;
            data.resize(dimension * dimension);
        }

        explicit Matrix(size_t rowCount, size_t colCount)
        {
            this->rowCount = rowCount;
            this->colCount = colCount;
            data.resize(rowCount * colCount);
        }

        explicit Matrix(size_t rowCount, size_t colCount, const T &initialValue)
        {
            this->rowCount = rowCount;
            this->colCount = colCount;
            data.resize(rowCount * colCount);
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                data[i] = initialValue;
            }
        }

        [[nodiscard]] size_t rows() const noexcept
        {
            return rowCount;
        }

        [[nodiscard]] size_t cols() const noexcept
        {
            return colCount;
        }

        T &at(const size_t &row, const size_t &col)
        {
            return data[((row * this->colCount) + col)];
        }

        auto row_begin(const size_t row) const
        {
            return (data.begin() + (row * colCount));
        }

        auto row_end(const size_t row) const
        {
            return (data.begin() + (row * colCount) + colCount);
        }

        const T &at(const size_t &row, const size_t &col) const
        {
            return data[((row * this->colCount) + col)];
        }

        bool equals(const Matrix<T> &mat) const
        {
            if (mat.rowCount != rowCount || mat.colCount != colCount)
            {
                return false;
            }
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                if (data[i] != mat.data[i])
                {
                    return false;
                }
            }
            return true;
        }

        Matrix<T> operator+(const Matrix<T> &other) const
        {
            always_assert(rowCount == other.rowCount && colCount == other.colCount);
            Matrix<T> result(rowCount, colCount);
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                result.data[i] = data[i] + other.data[i];
            }
            return result;
        }

        Matrix<T> &operator+=(const Matrix<T> &other)
        {
            always_assert(rowCount == other.rowCount && colCount == other.colCount);
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                data[i] += other.data[i];
            }
            return *this;
        }

        Matrix<T> operator-(const Matrix<T> &other) const
        {
            always_assert(rowCount == other.rowCount && colCount == other.colCount);
            Matrix<T> result(rowCount, colCount);
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                result.data[i] = data[i] - other.data[i];
            }
            return result;
        }

        Matrix<T> &operator-=(const Matrix<T> &other)
        {
            always_assert(rowCount == other.rowCount && colCount == other.colCount);
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                data[i] -= other.data[i];
            }
            return *this;
        }

        bool operator==(const Matrix<T> &other) const
        {
            return equals(other);
        }

        bool operator!=(const Matrix<T> &other) const
        {
            return !(equals(other));
        }

        std::vector<T> row(const size_t &rowIndex) const
        {
            always_assert(rowIndex < rowCount);
            std::vector<T> rowData(colCount);
            for (size_t col = 0; col < colCount; ++col)
            {
                rowData[col] = at(rowIndex, col);
            }
            return rowData;
        }

        std::vector<T> col(const size_t &colIndex) const
        {
            always_assert(colIndex < colCount);
            std::vector<T> colData(rowCount);
            for (size_t row = 0; row < rowCount; ++row)
            {
                colData[row] = at(row, colIndex);
            }
            return colData;
        }
    };
}
