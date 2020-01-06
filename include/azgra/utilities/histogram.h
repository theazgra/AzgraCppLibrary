#pragma once

#include <azgra/azgra.h>
#include <fstream>
#include <azgra/collection/enumerable_functions.h>

namespace azgra
{
    template<typename T>
    struct Bin
    {
        T min;
        T max;
        size_t count = 0;

        Bin()
        {}

        Bin(T _min, T _max) : min(_min), max(_max)
        {
            count = 0;
        }
    };

    template<typename T>
    std::vector<Bin<T>> create_histogram_from_numbers(const std::vector<T> &numbers, const size_t binCount)
    {
        std::pair<T, T> minMax = azgra::collection::min_max(numbers.begin(), numbers.end());
        T min = minMax.first;
        T max = minMax.second;

        T range = (min < 0) ? (abs(static_cast<double>(min)) + max) : (max - min);
        T binSize = range / binCount;

        std::vector<Bin<T>> bins(binCount);
        for (size_t i = 0; i < binCount; i++)
        {
            bins[i].min = min + (i * binSize) + 1;
            bins[i].max = bins[i].min + binSize;
        }
        bins[binCount - 1].max += 1;

        for (size_t i = 0; i < numbers.size(); i++)
        {
            for (Bin<T> &bin : bins)
            {
                if (numbers[i] >= bin.min && numbers[i] < bin.max)
                {
                    bin.count += 1;
                    break;
                }
            }
        }
        return bins;
    }

    template<typename T>
    void save_histogram_bins(const std::vector<Bin<T>> &bins, const char *fileName)
    {
        std::ofstream outStr(fileName, std::ios::out);
        always_assert(outStr.is_open());

        //outStr << "0 0\n";
        for (const Bin<T> &bin : bins)
        {
            outStr << bin.max << ' ' << bin.count << '\n';
        }
    }
}