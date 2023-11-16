#include <mpi.h>
#include <cstdio>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

int main(int argc, char** argv) {

    int32_t a[] = {2, 7, 6, 4, 3, 10, 1, 5};
    int32_t b[8] = { 0 };
    int32_t n = 8;

    // Initialize the MPI environment

    MPI_Init(&argc, &argv);

    // Get the rank of the process
    int32_t rank, count;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // get current value
    int32_t r = a[rank];

    for(int32_t faza = 1; faza <= n; faza++) {
        if((faza & 1) && rank >= 0 && rank < 2 * (n / 2)) {
            if(!(rank & 1)) {
                //printf("Par %d trimite valoarea %d catre %d\n", rank, r, rank + 1);
                MPI_Send(&r, 1, MPI_INT, rank + 1, 99, MPI_COMM_WORLD);
                int32_t v;
                MPI_Recv(&v, 1, MPI_INT, rank + 1, 99, MPI_COMM_WORLD, &status);
                r = min(r, v);
            } else {
                //printf("Impar %d trimite valoarea %d catre %d\n", rank, r, rank - 1);
                MPI_Send(&r, 1, MPI_INT, rank - 1, 99, MPI_COMM_WORLD);
                int32_t v;
                MPI_Recv(&v, 1, MPI_INT, rank - 1, 99, MPI_COMM_WORLD, &status);
                r = max(r, v);
            }
        } else if(!(faza & 1) && rank >= 1 && rank <= 2 * ((n - 1) / 2)) {
            if(rank & 1) {
                //printf("Impar %d trimite valoarea %d catre %d\n", rank, r, rank + 1);
                MPI_Send(&r, 1, MPI_INT, rank + 1, 99, MPI_COMM_WORLD);
                int32_t v;
                MPI_Recv(&v, 1, MPI_INT, rank + 1, 99, MPI_COMM_WORLD, &status);
                r = min(r, v);
            } else {
                //printf("Par %d trimite valoarea %d catre %d\n", rank, r, rank - 1);
                MPI_Send(&r, 1, MPI_INT, rank - 1, 99, MPI_COMM_WORLD);
                int32_t v;
                MPI_Recv(&v, 1, MPI_INT, rank - 1, 99, MPI_COMM_WORLD, &status);
                r = max(r, v);
            }
        }
    }

    printf("B(%d) = %d\n", rank, r);

    // Finalize the MPI environment.

    MPI_Finalize();
}