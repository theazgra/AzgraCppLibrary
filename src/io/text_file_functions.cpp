#include <azgra/io/text_file_functions.h>

namespace azgra::io
{

    std::ifstream open_text_file(const BasicStringView<char> &fileName)
    {
        std::ifstream inputTextStream(fileName.data(), std::ios::in);
        always_assert(inputTextStream.is_open() && "Failed to open input stream.");
        return inputTextStream;
    }

    std::vector<std::string> read_lines(const BasicStringView<char> &fileName)
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

    std::vector<std::vector<std::string>>
    read_csv_cells(const BasicStringView<char> &fileName, const BasicStringView<char> &separator, bool skipLineWithMissingValue)
    {
        std::ifstream inputTextStream = open_text_file(fileName);

        std::vector<std::vector<std::string>> csvCells;

        std::string line;
        while (std::getline(inputTextStream, line))
        {
            azgra::string::SmartStringView smartLine(line.c_str());
            const auto lineCellViews = smartLine.split(separator);

            if (skipLineWithMissingValue)
            {
                const bool missingValue = azgra::collection::any(
                        lineCellViews.begin(),
                        lineCellViews.end(),
                        [](const azgra::string::SmartStringView<char> &cellView)
                        {
                            return (cellView.is_empty() || cellView.equals(""));
                        });
                if (missingValue)
                {
                    continue;
                }
            }
            const auto lineCells = azgra::collection::select(lineCellViews.begin(),
                                                             lineCellViews.end(),
                                                             [](const azgra::string::SmartStringView<char> &cellView)
                                                             {
                                                                 return std::string(cellView.string_view());
                                                             });

            csvCells.push_back(lineCells);
        }
        return csvCells;
    }
}