//
// Created by Andrei Ioja on 09.11.2023.
//

#include <cstdio>
#include <omp.h>
#include "sort/muller_sort.h"

int main() {
    int32_t A[] = {2, 7, 6, 4, 3, 10, 1, 5};
    int32_t N = 8;

    muller_sort<__typeof(A[0])>(A, N);

    repeat(N) {
        printf("A(%d) = %d\n", A[i], N);
    }

    return 0;
}
