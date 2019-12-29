#pragma once

#include <azgra/matrix.h>
#include <azgra/collection/enumerable_functions.h>
#include <azgra/string/smart_string_view.h>
#include <fstream>
#include <functional>

namespace azgra::io
{
    /**
     * Open file in text mode and check if open was successful.
     * @param fileName Path to file.
     * @return Opened input file stream.
     */
    std::ifstream open_text_file(const BasicStringView<char> &fileName);

    /**
     * Read file lines.
     * @param fileName Path to file.
     * @return Vector of csv cells.
     */
    std::vector<std::string> read_lines(const BasicStringView<char> &fileName);

    template <typename T>
    void save_matrix_to_csv(const azgra::Matrix<T> &matrix, const char separator, const char *outFile)
    {
        std::ofstream out(outFile, std::ios::out);
        always_assert(out.is_open() && "Failed to open out stream");

        for (size_t r = 0; r < matrix.rows(); ++r)
        {
            for (size_t c = 0; c < matrix.cols(); ++c)
            {
                out << matrix.at(r,c) << separator;
            }
            out << '\n';
        }
    }

    /**
     * Read csv file cells
     * @param fileName Path to file.
     * @param separator CSV separator.
     * @return Vector of file lines.
     */
    std::vector<std::vector<std::string>>
    read_csv_cells(const BasicStringView<char> &fileName, const BasicStringView<char> &separator,
                   bool skipLineWithMissingValue);


    template<typename CellType>
    std::vector<std::vector<CellType>> csv_parse_cells(const BasicStringView<char> &fileName,
                                                       const BasicStringView<char> &separator,
                                                       std::function<CellType(
                                                               const azgra::string::SmartStringView<char> &cell)> cellParseFunction)
    {

        std::ifstream inputTextStream = open_text_file(fileName);

        std::vector<std::vector<CellType>> lines;

        std::string line;
        while (std::getline(inputTextStream, line))
        {
            azgra::string::SmartStringView smartLine(line.c_str());
            auto cells = smartLine.split(separator);

            std::vector<CellType> values(cells.size());
            for (size_t i = 0; i < cells.size(); ++i)
            {
                values[i] = cellParseFunction(cells[i]);
            }
            lines.push_back(values);
        }
        return lines;
    }

    template<typename CellType, typename CellParseFunction>
    std::vector<std::vector<CellType>> csv_parse_cells2(const BasicStringView<char> &fileName,
                                                        const BasicStringView<char> &separator,
                                                        CellParseFunction cellParseFunction)
    {

        std::ifstream inputTextStream = open_text_file(fileName);

        std::vector<std::vector<CellType>> lines;

        std::string line;
        while (std::getline(inputTextStream, line))
        {
            azgra::string::SmartStringView smartLine(line.c_str());
            auto cells = smartLine.split(separator);

            std::vector<CellType> values(cells.size());
            for (size_t i = 0; i < cells.size(); ++i)
            {
                values[i] = cellParseFunction(cells[i]);
            }
            lines.push_back(values);
        }
        return lines;
    }


    template<typename LineType, typename LineParseFunction>
    std::vector<LineType> parse_by_lines(const BasicStringView<char> &fileName, LineParseFunction lineParseFunction)
    {
        std::ifstream inputTextStream = open_text_file(fileName);

        std::vector<LineType> lines;

        std::string line;
        while (std::getline(inputTextStream, line))
        {
            azgra::string::SmartStringView smartLine(line.c_str());
            lines.push_back(lineParseFunction(smartLine));
        }
        return lines;
    }


}
