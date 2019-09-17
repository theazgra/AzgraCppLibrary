#pragma once
#include <azgra/utilities/vector_linq.h>
#include <azgra/azgra.h>

namespace azgra
{

enum MappingType
{
    // Negative values are stored after positive values.
    MappingType_NegativeAfterPositive,
    // Negative values are stored at even places, while positive at odd.
    MappingType_NegativeToEven
};

/**
 * @brief Provide mapping utilites from signed types to unsigned types.
 * 
 * @tparam SourceType Signed type to map.
 * @tparam TargetType Unsigned result type.
 */
template <typename SourceType, typename TargetType, MappingType MappingMethod>
class TypeMapper
{
private:
    MappingType _mappingType = MappingMethod;

    /**
     * @brief Map `SourceType` vector to `TargetType` vector. Saving mapping point as first element. 
     * Negative values are stored after the maximum positive value.
     * 
     * @param data Data to map.
     * @return std::vector<TargetType> Mapped data.
     */
    std::vector<TargetType> map_negative_after_positive(const std::vector<SourceType> &data)
    {
        always_assert(_mappingType == MappingType_NegativeAfterPositive);

        auto limits = linq::min_max(data);
        SourceType min = limits.first;
        SourceType max = limits.second;

        TargetType targetMaxValue = std::numeric_limits<TargetType>::max();
        always_assert(max < 0 || (size_t)max < (size_t)targetMaxValue);
        always_assert((size_t)(abs(min) + max) < (size_t)targetMaxValue);

        TargetType mappingPoint = max;

        std::vector<TargetType> result;
        result.resize(data.size() + 1);

        // Save mapping point as first value.
        result[0] = mappingPoint;

        for (size_t i = 0; i < data.size(); i++)
        {
            if (data[i] >= 0)
            {
                result[i + 1] = (TargetType)data[i];
            }
            else
            {
                result[i + 1] = mappingPoint + abs(data[i]);
            }
        }

        return result;
    }

    /**
     * @brief Map back `TargetType` vector to `SourceType` vector.
     * 
     * @param data Data to map back.
     * @return std::vector<SourceType> Original unrolled data.
     */
    std::vector<SourceType> map_back_negative_after_positive(const std::vector<TargetType> &data)
    {
        always_assert(_mappingType == MappingType_NegativeAfterPositive);

        std::vector<SourceType> result;
        result.resize(data.size() - 1);

        // Retrieve mapping point.
        TargetType mappingPoint = data[0];

        for (size_t i = 1; i < data.size(); i++)
        {
            if (data[i] <= mappingPoint)
            {
                // Positive values
                result[i - 1] = data[i];
            }
            else
            {
                // Negative values
                result[i - 1] = -(data[i] - mappingPoint);
            }
        }
        return result;
    }

    std::vector<TargetType> map_negative_to_even(const std::vector<SourceType> &data)
    {
        auto limits = linq::min_max(data);
        TargetType ttMax = std::numeric_limits<TargetType>::max();

        always_assert((limits.second < 0 || (size_t)((limits.second * 2) - 1) <= (size_t)ttMax) &&
                      ((size_t)(2 * std::abs(limits.first)) <= (size_t)ttMax));

        std::vector<TargetType> result;
        result.resize(data.size());
        /*
        -7  -6  -5  -4  -3  -2  -1  0   1   2   3   4   5   6   7
        14  12  10   8   6   4   2  0   1   3   5   7   9   11  13
        */
        for (size_t i = 0; i < data.size(); i++)
        {
            if (data[i] == 0)
            {
                result[i] = 0;
            }
            else if (data[i] < 0)
            {
                result[i] = 2 * std::abs(data[i]);
            }
            else // data[i] > 0
            {
                result[i] = (2 * data[i]) - 1;
            }
        }
        return result;
    }

    std::vector<SourceType> map_back_negative_to_even(const std::vector<TargetType> &data)
    {
        std::vector<SourceType> result;
        result.resize(data.size());

        /*
        14  12  10   8   6   4   2  0   1   3   5   7   9   11  13
        -7  -6  -5  -4  -3  -2  -1  0   1   2   3   4   5   6   7
        */
        for (size_t i = 0; i < data.size(); i++)
        {
            if (data[i] == 0)
            {
                result[i] = 0;
            }
            else if (data[i] % 2 == 0)
            {
                result[i] = -(data[i] / 2);
            }
            else // data[i] % 2 != 0
            {
                result[i] = (data[i] / 2) + 1;
            }
        }
        return result;
    }

public:
    std::vector<TargetType> map(const std::vector<SourceType> &data)
    {
        switch (_mappingType)
        {
        case MappingType_NegativeAfterPositive:
            return map_negative_after_positive(data);
        case MappingType_NegativeToEven:
            return map_negative_to_even(data);
        default:
            INVALID_CASE;
            return std::vector<TargetType>();
        }
    }
    std::vector<SourceType> map_back(const std::vector<TargetType> &data)
    {
        switch (_mappingType)
        {
        case MappingType_NegativeAfterPositive:
            return map_back_negative_after_positive(data);
        case MappingType_NegativeToEven:
            return map_back_negative_to_even(data);
        default:
            INVALID_CASE;
            return std::vector<SourceType>();
        }
    }
};
} // namespace azgra