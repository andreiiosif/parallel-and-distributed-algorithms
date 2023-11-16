//
// Created by Andrei Ioja on 14.11.2023.
//

#ifndef LAB_7_MULLER_SORT_H
#define LAB_7_MULLER_SORT_H

#include <cstdint>
#include "zip_next.h"
#include <omp.h>
#include <cassert>
#include <memory>

#define repeat(n) for(int32_t i = 0; i < (n); i++)

/// Implementation of Muller-Preparata algorithm using a shared memory model
/// \tparam T is type of data
/// \param array is the input to be sorted
/// \param ssize is the length of array
template <typename T>
void muller_sort(T* array, int32_t ssize) {
    assert(ssize > 0);

    /* Memory allocation for R table */
    T** R = new T*[ssize];
    repeat(ssize) {
        R[i] = new T[2 * ssize - 1];
    }
    /* Memory allocation for P array */
    std::unique_ptr<T[]> P(new T[ssize]);

    /* generate R table */
#pragma omp parallel for private(j) collapse(2)
    for(int32_t i = 0; i < ssize; i++) {
        for(int32_t j = 0; j < ssize; j++) {
            R[j][i + ssize - 1] = (array[i] < array[j]);
        }
    }

    /* Compress each line */
#pragma omp parralel for num_threads(4) shared(R, ssize)
    for(int32_t i = 0; i < ssize; i++) {
        zip_next<T>(R[i], ssize, [](const T a, const T b) {
            return a + b;
        });
        /* Copy to P(i) the compressed value of array R(i) */
        P[i] = R[i][0];
        R[i][0] = 0;
    }

    /* Calculate root of every node */
    int32_t connected_comp = 0;
    for(int32_t i = 0; i < ssize; i++)
        if(!R[i][0]) {
            // start from i, while P[i] != i
            R[i][0] = i;
            R[connected_comp++][1] = i;
            int32_t son = P[i];
            while(son != R[i][0]) {
                R[son][0] = i;
                son = P[son];
            }
        }

    /* Change order of elements */
#pragma omp parralel for num_threads(4) shared(R, A, P)
    for(int32_t i = 0; i < connected_comp; i++) {
        // Start from i, while P[i] != i
        int32_t son = R[i][1], aux = array[P[son]];
        while(P[son] != i) {
            array[P[son]] = son;
            son = P[son];
        }
        array[P[son]] = aux;
    }

    /* free memory of R table */
    repeat(ssize) {
        delete[] R[i];
    }
    delete[] R;
}

#endif //LAB_7_MULLER_SORT_H
