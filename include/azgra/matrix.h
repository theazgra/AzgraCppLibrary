#pragma once

#include <azgra/azgra.h>
#include <azgra/utilities/custom_advancement_iterator.h>

namespace azgra
{


    template<typename T, bool SaveByRow = true>
    class Matrix
    {
        template<typename U, bool SaveByRow_>
        friend
        class Matrix;

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


        [[nodiscard]] constexpr size_t index(const size_t row, const size_t col) const
        {
            if (SaveByRow)
            {
                return ((row * colCount) + col);
            }
            else
            {
                return ((col * rowCount) + row);
            }
        }

    public:
        Matrix() = default;

        Matrix(const Matrix<T> &copySrc)
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
            const size_t dataCount = rowCount * colCount;
            data.resize(dataCount);
            for (size_t i = 0; i < dataCount; ++i)
            {
                data[i] = initialValue;
            }
        }

        explicit Matrix(size_t rowCount, size_t colCount, std::vector<T> &dataToMove)
        {
            this->rowCount = rowCount;
            this->colCount = colCount;
            data.resize(rowCount * colCount);
            always_assert((rowCount * colCount) == dataToMove.size());
            data = std::move(dataToMove);
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
            return data[index(row, col)];
        }

        const T &at(const size_t &row, const size_t &col) const
        {
            return data[index(row, col)];
        }

        auto row_cbegin(const size_t row) const
        {
            if (SaveByRow)
            {
                //return (data.begin() + (row * colCount));
                return ConstCustomAdvancementIterator((data.data() + (row * colCount)),
                                                      (data.data() + (row * colCount) + colCount),
                                                      1);
            }
            else
            {
                const auto start = (data.data() + row);
                const auto end = (data.data() + ((colCount - 1) * rowCount) + row + 1);
                return ConstCustomAdvancementIterator(start, end, colCount);
            }
        }

        auto row_cend(const size_t row) const
        {
            if (SaveByRow)
            {
                //return (data.begin() + (row * colCount) + colCount);
                return ConstCustomAdvancementIterator((data.data() + (row * colCount) + colCount), 1);
            }
            else
            {
                const auto end = (data.data() + ((colCount - 1) * rowCount) + row + 1);
                return ConstCustomAdvancementIterator(end, colCount);
            }
        }

        auto row_begin(const size_t row)
        {
            if (SaveByRow)
            {
                //return (data.begin() + (row * colCount));
                return CustomAdvancementIterator((data.data() + (row * colCount)),
                                                 (data.data() + (row * colCount) + colCount),
                                                 1);
            }
            else
            {
                const auto start = (data.data() + row);
                const auto end = (data.data() + ((colCount - 1) * rowCount) + row + 1);
                return CustomAdvancementIterator(start, end, colCount);
            }
        }

        auto row_end(const size_t row)
        {
            if (SaveByRow)
            {
                //return (data.begin() + (row * colCount) + colCount);
                return CustomAdvancementIterator((data.data() + (row * colCount) + colCount), 1);
            }
            else
            {
                const auto end = (data.data() + ((colCount - 1) * rowCount) + row + 1);
                return CustomAdvancementIterator(end, colCount);
            }
        }

        auto col_cbegin(const size_t col) const
        {
            if (SaveByRow)
            {
                const auto start = (data.data() + col);
                const auto end = (data.data() + ((rowCount - 1) * colCount) + col + 1);
                return ConstCustomAdvancementIterator(start, end, colCount);
            }
            else
            {
                //return (data.begin() + (col * rowCount));
                return ConstCustomAdvancementIterator((data.data() + (col * rowCount)),
                                                      (data.data() + (col * rowCount) + rowCount),
                                                      1);
            }
        }

        auto col_cend(const size_t col) const
        {
            if (SaveByRow)
            {
                const auto end = (data.data() + ((rowCount - 1) * colCount) + col + 1);
                return ConstCustomAdvancementIterator(end, colCount);
            }
            else
            {
                //return (data.begin() + (col * rowCount) + rowCount);
                return ConstCustomAdvancementIterator((data.data() + (col * rowCount) + rowCount),
                                                      1);
            }
        }

        auto col_begin(const size_t col)
        {
            if (SaveByRow)
            {
                auto start = (data.data() + col);
                const auto end = (data.data() + ((rowCount - 1) * colCount) + col + 1);
                return CustomAdvancementIterator(start, end, colCount);
            }
            else
            {
                //return (data.begin() + (col * rowCount));
                return CustomAdvancementIterator((data.data() + (col * rowCount)),
                                                 (data.data() + (col * rowCount) + rowCount),
                                                 1);
            }
        }

        auto col_end(const size_t col)
        {
            if (SaveByRow)
            {
                const auto end = (data.data() + ((rowCount - 1) * colCount) + col + 1);
                return CustomAdvancementIterator(end, colCount);
            }
            else
            {
                //return (data.begin() + (col * rowCount) + rowCount);
                return CustomAdvancementIterator((data.data() + (col * rowCount) + rowCount),
                                                 1);
            }
        }


        bool equals(const Matrix &mat) const
        {
            if (mat.rowCount != rowCount || mat.colCount != colCount)
            {
                return false;
            }
            const size_t dataCount = (rowCount * colCount);
            for (size_t i = 0; i < dataCount; ++i)
            {
                if (data[i] != mat.data[i])
                {
                    return false;
                }
            }
            return true;
        }

        Matrix<T> operator+(const Matrix &other) const
        {
            always_assert(rowCount == other.rowCount && colCount == other.colCount);
            Matrix<T> result(rowCount, colCount);
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                result.data[i] = data[i] + other.data[i];
            }
            return result;
        }

        Matrix<T> &operator+=(const Matrix &other)
        {
            always_assert(rowCount == other.rowCount && colCount == other.colCount);
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                data[i] += other.data[i];
            }
            return *this;
        }

        Matrix<T> operator-(const Matrix &other) const
        {
            always_assert(rowCount == other.rowCount && colCount == other.colCount);
            Matrix<T> result(rowCount, colCount);
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                result.data[i] = data[i] - other.data[i];
            }
            return result;
        }

        Matrix<T> &operator-=(const Matrix &other)
        {
            always_assert(rowCount == other.rowCount && colCount == other.colCount);
            for (size_t i = 0; i < (rowCount * colCount); ++i)
            {
                data[i] -= other.data[i];
            }
            return *this;
        }

        bool operator==(const Matrix &other) const
        {
            return equals(other);
        }

        bool operator!=(const Matrix &other) const
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

        void col(const size_t &colIndex, const std::vector<T> &values)
        {
            always_assert(values.size() == rowCount);
            for (size_t row = 0; row < rowCount; ++row)
            {
                at(row, colIndex) = values[row];
            }
        }

        std::vector<T> const &get_data() const
        {
            return data;
        }
    };
}
