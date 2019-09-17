#pragma once

#include <azgra/azgra.h>
#include <vector>
#include <algorithm>
#include <limits>
#include <map>

namespace azgra
{
    namespace collection
    {
        template<typename T = size_t>
        std::vector<T> range(const size_t stop)
        {
            std::vector<T> indices(stop);
            for (size_t i = 0; i < stop; ++i)
            {
                indices[i] = i;
            }
            return indices;
        }


        // Insert `vecToCopyFrom` data into `vecToInsert` at `insertAtPos`, copy from `copyFromPos` `copySize` number of elements.
        template<typename T>
        inline void vector_insert_at(std::vector<T> &vecToInsertInto, const std::vector<T> &vecToCopyFrom,
                                     const size_t insertAtPos, const size_t copyFromPos, const size_t copySize)
        {
            for (size_t i = 0; i < copySize; i++)
            {
                vecToInsertInto[insertAtPos + i] = vecToCopyFrom[copyFromPos + i];
            }
        }

        template<typename T>
        inline T *last_element_pointer(std::vector<T> *vector)
        {
            return (vector->empty() ? vector->data() : &vector->back());
        }

        template<typename T, typename DiffType>
        std::vector<DiffType> diff_vectors(const std::vector<T> &ref, const std::vector<T> &current)
        {
            always_assert(ref.size() == current.size());

            std::vector<DiffType> result;
            result.resize(ref.size());
            for (size_t i = 0; i < ref.size(); i++)
            {
                result[i] = (DiffType) (current[i] - ref[i]);
            }

            return result;
        }

    }; // namespace vecUtil
} // namespace azgra