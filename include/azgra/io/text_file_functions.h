#pragma once

#include <azgra/azgra.h>
#include <azgra/string/smart_string_view.h>
#include <fstream>
#include <functional>

namespace azgra::io
{
    namespace
    {
        /**
         * Open file in text mode and check if open was successful.
         * @param fileName Path to file.
         * @return Opened input file stream.
         */
        static std::ifstream open_text_file(const basic_string_view__<char> &fileName)
        {
            std::ifstream inputTextStream(fileName.data(), std::ios::in);
            always_assert(inputTextStream.is_open() && "Failed to open input stream.");
            return inputTextStream;
        }
    }


    template<typename CellType>
    std::vector<std::vector<CellType>> csv_parse_cells(const basic_string_view__<char> &fileName,
                                                       const basic_string_view__<char> &separator,
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

    template<typename LineType>
    std::vector<LineType> parse_by_lines(const basic_string_view__<char> &fileName,
                                         std::function<LineType(const azgra::string::SmartStringView<char> &line)> lineParseFunction)
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

    std::vector<std::string> read_lines(const basic_string_view__<char> &fileName)
    {
        std::ifstream inputTextStream = open_text_file(fileName);
        std::vector<std::string> lines;

        std::string line;
        while (std::getline(inputTextStream, line))
        {
            lines.push_back(line);
        }
        return lines;
    }

}
