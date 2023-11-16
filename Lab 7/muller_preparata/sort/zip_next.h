//
// Created by Andrei Ioja on 14.11.2023.
//

#ifndef LAB_7_ZIP_NEXT_H
#define LAB_7_ZIP_NEXT_H

#include <cstdint>
#include <cmath>
#include <cassert>
#include <numeric>

/// Compress data using a tree structure
/// \tparam T is the data type
/// \param array is the address where data will be stored
/// \param ssize is the length of the input
/// \param op is the aggregation operation
template <typename T>
void zip_next(T* array, int32_t ssize, const std::function<T(T, T)> &op) {
    assert(array != nullptr);

    /* Calculate log2 of length */
    auto m = static_cast<int32_t>(log2(ssize));

    /* Reduce array */
    for(int32_t k = m - 1; k >= 0; k--) {
#pragma omp parallel for num_threads(4) shared(k, array, m) default(none)
        for(int32_t j = (1<<k); j < (1<<(k + 1)) - 1; j++) {
            array[j - 1] = op(array[2 * j - 1], array[2 * j]);
        }
    }
}

#endif //LAB_7_ZIP_NEXT_H
