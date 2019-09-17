#pragma once

#include <azgra/azgra.h>
#include <azgra/collection/vector_utilities.h>
#include <set>

namespace azgra::collection
{
    template<typename T>
    static void power_set_internal(std::vector<std::vector<T>> &powerset,
                                   const std::vector<T> &originalSet,
                                   std::vector<T> currentSubset,
                                   int currentIndex = -1)
    {
        int n = originalSet.size();
        if (currentIndex == n)
            return;

        powerset.push_back(currentSubset);

        for (int i = currentIndex + 1; i < n; i++)
        {
            currentSubset.push_back(originalSet[i]);
            power_set_internal(powerset, originalSet, currentSubset, i);
            currentSubset.erase(currentSubset.end() - 1);
        }
    }


    template<typename T>
    std::vector<std::vector<T>> generate_powerset(const std::vector<T> &set)
    {
        std::vector<std::vector<T>> powerset;
        std::vector<T> tmp;
        power_set_internal(powerset, set, tmp);

        auto finalExpectedSize = (size_t) pow(2, set.size());
        always_assert(powerset.size() == finalExpectedSize);

        return powerset;
    }

    template<typename T>
    std::vector<std::vector<T>> generate_subsets_of_size(const std::vector<T> &pool, const int requestedSubsetSize)
    {
        std::vector<std::vector<T>> subsets;

        // pool = set
        size_t n = pool.size();

        assert(requestedSubsetSize <= n);
        if (requestedSubsetSize > n)
            return subsets;

        std::vector<size_t> indices = range(requestedSubsetSize);

        {
            std::vector<T> subset(requestedSubsetSize);
            for (const size_t i : indices)
                subset[i] = pool[i];

            subsets.push_back(subset);
        }

        while (true)
        {
            bool breakLoopExit = false;
            int i = requestedSubsetSize - 1;
            for (i = requestedSubsetSize - 1; i >= 0; --i)
            {
                if (indices[i] != (i + n - requestedSubsetSize))
                {
                    breakLoopExit = true;
                    break;
                }
            }
            if (!breakLoopExit)
            {
                return subsets;
            }
            else
            {
                indices[i] += 1;
                for (int j = i + 1; j < requestedSubsetSize; ++j)
                {
                    indices[j] = indices[j - 1] + 1;
                }

                {
                    std::vector<T> subset(requestedSubsetSize);
                    for (size_t index = 0; index < indices.size(); ++index)
                    {
                        subset[index] = pool[indices[index]];
                    }
                    subsets.push_back(subset);
                }
            }
        }
    }

    template <typename T>
    std::set<T> vector_as_set(const std::vector<T> &vec)
    {
        std::set<T> result(vec.begin(), vec.end());
        return result;
    }

}
