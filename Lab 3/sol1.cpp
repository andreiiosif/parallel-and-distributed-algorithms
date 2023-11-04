#include <mpi.h>
#include <cstdio>
#include <cstring>

int main(int argc, char** argv) {
    /* Initialize data for algorithm */
    char M[] = "APD este distractiv!";
    int32_t d = 3;
    int32_t mask = (1 << d) - 1;

    // Initialize the MPI environment
    int32_t myrank;
    MPI_Status status;
    MPI_Init(nullptr, nullptr);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // Start of algorithm
    for(int32_t k = d - 1; k >= 0; k--) {
        mask = mask ^ (1 << k);
        if(!(myrank & mask)) {
            if(!(myrank & (1 << k))) {
                int32_t destination = myrank ^ (1 << k);
                MPI_Send(M, (int32_t)strlen(M) + 1, MPI_CHAR, destination, 99, MPI_COMM_WORLD);
            } else {
                int32_t source = myrank ^ (1 << k);
                char recv_message[25];
                MPI_Recv(recv_message, 25, MPI_CHAR, source, 99, MPI_COMM_WORLD, &status);
                printf("Mesaj primit de %d de la %d: %s\n", myrank, source, recv_message);
            }
        }
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}