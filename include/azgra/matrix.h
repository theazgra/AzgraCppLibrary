#pragma once

#include <azgra/azgra.h>
#include <azgra/utilities/custom_advancement_iterator.h>

namespace azgra
{
    /**
     * Matrix data structure. Elements are accessible through row and column indices.
     * @tparam T Type of matrix element.
     * @tparam RowBased True if elements are saved by rows, otherwise elements are saved by columns.
     */
    template<typename T, bool RowBased = true>
    class Matrix
    {
        template<typename U, bool SaveByRow_>
        friend
        class Matrix;

    private:
        /**
         * Matrix row count.
         */
        size_t m_rowCount = 0;

        /**
         * Matrix column count.
         */
        size_t m_colCount = 0;

        /**
         * Matrix data.
         */
        std::vector<T> m_data;


        /**
         * Access matrix element reference by 1d index.
         * @param index Index into the 1d vector.
         * @return Reference of the element at the index.
         */
        T &at(const size_t &index)
        {
            return m_data[index];
        }

        /**
         * Access matrix element reference by 1d index.
         * @param index Index into the 1d vector.
         * @return Constant reference of the element at the index.
         */
        const T &at(const size_t &index) const
        {
            return m_data[index];
        }


        /**
         * Calculate the 1d index of the 2d matrix element.
         * @param row Zero based row index of the element.
         * @param col Zero based column index of the element.
         * @return Index of the element into the 1d array.
         */
        [[nodiscard]] constexpr size_t index(const size_t row, const size_t col) const
        {
            if (RowBased)
            {
                return ((row * m_colCount) + col);
            }
            else
            {
                return ((col * m_rowCount) + row);
            }
        }

    public:
        /**
         * Default constructor, without any initialization.
         */
        Matrix() = default;

        /**
         * Copy constructor, copy the vector data.
         * @param copySrc Source matrix.
         */
        Matrix(const Matrix<T> &copySrc)
        {
            m_rowCount = copySrc.m_rowCount;
            m_colCount = copySrc.m_colCount;
            m_data = std::vector<T>(copySrc.m_data.begin(), copySrc.m_data.end());
        }

        /**
         * Move constructor, copy the vector data.
         * @param moveSrc Source matrix.
         */
        Matrix(Matrix &&moveSrc) noexcept
        {
            m_rowCount = std::move(moveSrc.m_rowCount);
            m_colCount = std::move(moveSrc.m_colCount);
            m_data = std::move(moveSrc.m_data);
        }

        Matrix &operator=(Matrix &&other) noexcept = default;

        /**
         * Initialize matrix by dimension size in every dimension.
         * @param dimensionSize Size of all dimensions.
         */
        explicit Matrix(size_t dimensionSize)
        {
            m_rowCount = dimensionSize;
            m_colCount = dimensionSize;
            m_data.resize(dimensionSize * dimensionSize);
        }

        /**
         * Initialize matrix by dimensions.
         * @param rowCount Matrix row count.
         * @param colCount Matrix column count.
         */
        explicit Matrix(size_t rowCount, size_t colCount)
        {
            this->m_rowCount = rowCount;
            this->m_colCount = colCount;
            m_data.resize(rowCount * colCount);
        }

        /**
         * Initialize matrix by dimensions and set the initial value.
         * @param rowCount Matrix row count.
         * @param colCount Matrix column count.
         * @param initialValue Value set for all elements.
         */
        explicit Matrix(size_t rowCount, size_t colCount, const T &initialValue)
        {
            this->m_rowCount = rowCount;
            this->m_colCount = colCount;
            const size_t dataCount = rowCount * colCount;
            m_data.resize(dataCount);
            for (size_t i = 0; i < dataCount; ++i)
            {
                m_data[i] = initialValue;
            }
        }

        /**
         * Initialize matrix by dimensions and move the initial data.
         * @param rowCount Matrix row count.
         * @param colCount Matrix column count.
         * @param dataToMove Initial data to be moved to this matrix.
         */
        explicit Matrix(size_t rowCount, size_t colCount, std::vector<T> &dataToMove)
        {
            this->m_rowCount = rowCount;
            this->m_colCount = colCount;
            always_assert((rowCount * colCount) == dataToMove.size());
            m_data = std::move(dataToMove);
        }

        /**
         * Get number of rows of the matrix.
         * @return Number of rows.
         */
        [[nodiscard]] size_t rows() const noexcept
        {
            return m_rowCount;
        }

        /**
         * Get number of columns of the matrix.
         * @return Number of columns.
         */
        [[nodiscard]] size_t cols() const noexcept
        {
            return m_colCount;
        }

        /**
         * Access reference of the element at given row and column.
         * @param row Zero based row index.
         * @param col Zero based column index.
         * @return Reference to the matrix element.
         */
        T &at(const size_t &row, const size_t &col)
        {
            return m_data[index(row, col)];
        }

        /**
         * Access const reference of the element at given row and column.
         * @param row Zero based row index.
         * @param col Zero based column index.
         * @return Const reference to the matrix element.
         */
        const T &at(const size_t &row, const size_t &col) const
        {
            return m_data[index(row, col)];
        }

        /**
         * Get constant iterator for row beginning.
         * @param row Zero based row index.
         * @return Constant input iterator.
         */
        auto row_cbegin(const size_t row) const
        {
            if (RowBased)
            {
                return ConstCustomAdvancementIterator((m_data.data() + (row * m_colCount)),
                                                      (m_data.data() + (row * m_colCount) + m_colCount),
                                                      1);
            }
            else
            {
                const auto start = (m_data.data() + row);
                const auto end = (m_data.data() + ((m_colCount - 1) * m_rowCount) + row + 1);
                return ConstCustomAdvancementIterator(start, end, m_colCount);
            }
        }

        /**
        * Get constant iterator for row end.
        * @param row Zero based row index.
        * @return Constant input iterator.
        */
        auto row_cend(const size_t row) const
        {
            if (RowBased)
            {
                //return (data.begin() + (row * colCount) + colCount);
                return ConstCustomAdvancementIterator((m_data.data() + (row * m_colCount) + m_colCount), 1);
            }
            else
            {
                const auto end = (m_data.data() + ((m_colCount - 1) * m_rowCount) + row + 1);
                return ConstCustomAdvancementIterator(end, m_colCount);
            }
        }

        /**
        * Get iterator for row beginning.
        * @param row Zero based row index.
        * @return Forward iterator
        */
        auto row_begin(const size_t row)
        {
            if (RowBased)
            {
                //return (data.begin() + (row * colCount));
                return CustomAdvancementIterator((m_data.data() + (row * m_colCount)),
                                                 (m_data.data() + (row * m_colCount) + m_colCount),
                                                 1);
            }
            else
            {
                const auto start = (m_data.data() + row);
                const auto end = (m_data.data() + ((m_colCount - 1) * m_rowCount) + row + 1);
                return CustomAdvancementIterator(start, end, m_colCount);
            }
        }

        /**
        * Get iterator for row end.
        * @param row Zero based row index.
        * @return Forward iterator.
        */
        auto row_end(const size_t row)
        {
            if (RowBased)
            {
                //return (data.begin() + (row * colCount) + colCount);
                return CustomAdvancementIterator((m_data.data() + (row * m_colCount) + m_colCount), 1);
            }
            else
            {
                const auto end = (m_data.data() + ((m_colCount - 1) * m_rowCount) + row + 1);
                return CustomAdvancementIterator(end, m_colCount);
            }
        }

        /**
         * Get constant iterator for column beginning.
         * @param col Zero based column index.
         * @return Constant input iterator.
         */
        auto col_cbegin(const size_t col) const
        {
            if (RowBased)
            {
                const auto start = (m_data.data() + col);
                const auto end = (m_data.data() + ((m_rowCount - 1) * m_colCount) + col + 1);
                return ConstCustomAdvancementIterator(start, end, m_colCount);
            }
            else
            {
                //return (data.begin() + (col * rowCount));
                return ConstCustomAdvancementIterator((m_data.data() + (col * m_rowCount)),
                                                      (m_data.data() + (col * m_rowCount) + m_rowCount),
                                                      1);
            }
        }

        /**
         * Get constant iterator for column end.
         * @param col Zero based column index.
         * @return Constant input iterator.
         */
        auto col_cend(const size_t col) const
        {
            if (RowBased)
            {
                const auto end = (m_data.data() + ((m_rowCount - 1) * m_colCount) + col + 1);
                return ConstCustomAdvancementIterator(end, m_colCount);
            }
            else
            {
                //return (data.begin() + (col * rowCount) + rowCount);
                return ConstCustomAdvancementIterator((m_data.data() + (col * m_rowCount) + m_rowCount),
                                                      1);
            }
        }

        /**
         * Get iterator for column beginning.
         * @param col Zero based column index.
         * @return Forward iterator.
         */
        auto col_begin(const size_t col)
        {
            if (RowBased)
            {
                auto start = (m_data.data() + col);
                const auto end = (m_data.data() + ((m_rowCount - 1) * m_colCount) + col + 1);
                return CustomAdvancementIterator(start, end, m_colCount);
            }
            else
            {
                //return (data.begin() + (col * rowCount));
                return CustomAdvancementIterator((m_data.data() + (col * m_rowCount)),
                                                 (m_data.data() + (col * m_rowCount) + m_rowCount),
                                                 1);
            }
        }

        /**
         * Get iterator for column end.
         * @param col Zero based column index.
         * @return Forward iterator.
         */
        auto col_end(const size_t col)
        {
            if (RowBased)
            {
                const auto end = (m_data.data() + ((m_rowCount - 1) * m_colCount) + col + 1);
                return CustomAdvancementIterator(end, m_colCount);
            }
            else
            {
                //return (data.begin() + (col * rowCount) + rowCount);
                return CustomAdvancementIterator((m_data.data() + (col * m_rowCount) + m_rowCount),
                                                 1);
            }
        }

        /**
         * Check whether two matrices are same. All their values are equal.
         * @param mat Another matrix.
         * @return True if dimensions match and all values are equal.
         */
        bool equals(const Matrix &mat) const
        {
            if (mat.m_rowCount != m_rowCount || mat.m_colCount != m_colCount)
            {
                return false;
            }
            const size_t dataCount = (m_rowCount * m_colCount);
            for (size_t i = 0; i < dataCount; ++i)
            {
                if (m_data[i] != mat.m_data[i])
                {
                    return false;
                }
            }
            return true;
        }

        Matrix<T> operator+(const Matrix &other) const
        {
            always_assert(m_rowCount == other.m_rowCount && m_colCount == other.m_colCount);
            Matrix<T> result(m_rowCount, m_colCount);
            for (size_t i = 0; i < (m_rowCount * m_colCount); ++i)
            {
                result.m_data[i] = m_data[i] + other.m_data[i];
            }
            return result;
        }

        Matrix<T> &operator+=(const Matrix &other)
        {
            always_assert(m_rowCount == other.m_rowCount && m_colCount == other.m_colCount);
            for (size_t i = 0; i < (m_rowCount * m_colCount); ++i)
            {
                m_data[i] += other.m_data[i];
            }
            return *this;
        }

        Matrix<T> operator-(const Matrix &other) const
        {
            always_assert(m_rowCount == other.m_rowCount && m_colCount == other.m_colCount);
            Matrix<T> result(m_rowCount, m_colCount);
            for (size_t i = 0; i < (m_rowCount * m_colCount); ++i)
            {
                result.m_data[i] = m_data[i] - other.m_data[i];
            }
            return result;
        }

        Matrix<T> &operator-=(const Matrix &other)
        {
            always_assert(m_rowCount == other.m_rowCount && m_colCount == other.m_colCount);
            for (size_t i = 0; i < (m_rowCount * m_colCount); ++i)
            {
                m_data[i] -= other.m_data[i];
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

        /**
         * Return copy of the row elements.
         * @param rowIndex Zero based row index.
         * @return Vector of row elements.
         */
        std::vector<T> row(const size_t &rowIndex) const
        {
            always_assert(rowIndex < m_rowCount);
            std::vector<T> rowData(m_colCount);
            std::copy(row_cbegin(rowIndex), row_cend(rowIndex), rowData.begin());
            return rowData;
        }

        /**
         * Return copy of column elements.
         * @param colIndex Zero based column index.
         * @return Vector of column elements.
         */
        std::vector<T> col(const size_t &colIndex) const
        {
            always_assert(colIndex < m_colCount);
            std::vector<T> colData(m_rowCount);
            std::copy(col_cbegin(colIndex), col_cend(colIndex), colData.begin());
            return colData;
        }

        /**
         * Copy values from begin to end iterators to the matrix column.
         * @tparam It Input iterator with values to copy.
         * @param colIndex Zero based column index.
         * @param begin Copy iterator beginning.
         * @param end Copy iterator end.
         */
        template<typename It>
        void col(const size_t &colIndex, const It begin, const It end)
        {
            static_assert(std::is_same_v<typename std::iterator_traits<It>::value_type, T>);

            always_assert(colIndex < m_colCount);
            always_assert(static_cast<size_t> (std::distance(begin, end)) == m_rowCount);
            std::copy(begin, end, col_begin(colIndex));
        }

        /**
         * Copy values from begin to end iterators to the matrix row.
         * @tparam It Input iterator with values to copy.
         * @param rowIndex Zero based row index.
         * @param begin Copy iterator beginning.
         * @param end Copy iterator end.
         */
        template<typename It>
        void row(const size_t &rowIndex, const It begin, const It end)
        {
            static_assert(std::is_same_v<typename std::iterator_traits<It>::value_type, T>);

            always_assert(rowIndex < m_rowCount);
            always_assert(static_cast<size_t> (std::distance(begin, end)) == m_colCount);
            std::copy(begin, end, row_begin(rowIndex));
        }

        /**
         * Get constant reference to the matrix data.
         * @return Constant reference to vector data.
         */
        std::vector<T> const &get_data() const
        {
            return m_data;
        }
    };
}
