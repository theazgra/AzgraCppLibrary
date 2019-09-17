#pragma once

#include <azgra/azgra.h>
#include <azgra/string/smart_string_view.h>
#include <fstream>
#include <functional>

namespace azgra::io
{
    template<typename T>
    std::vector<std::vector<T>> read_csv_file(const basic_string_view__<char> &fileName,
                                              const basic_string_view__<char> &separator,
                                              std::function<T(const azgra::string::SmartStringView<char> &cell)> castFunction)
    {
        std::ifstream inputTextStream(fileName.data(), std::ios::in);
        always_assert(inputTextStream.is_open() && "Failed to open input stream.");

        std::vector<std::vector<T>> lines;

        std::string line;
        while (std::getline(inputTextStream, line))
        {
            azgra::string::SmartStringView smartLine(line.c_str());
            auto cells = smartLine.split(separator);

            std::vector<T> values(cells.size());
            for (int i = 0; i < cells.size(); ++i)
            {
                values[i] = castFunction(cells[i]);
            }
            lines.push_back(values);
        }
        return lines;
    }
}
