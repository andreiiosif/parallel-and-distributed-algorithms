#include <mpi.h>
#include <cstdio>
#include <cstring>

int main(int argc, char** argv) {

    int32_t nodes_number = 8;
    int32_t edges[] = {
            4,
            6,
            4, 7, 8,
            4, 6,
            4, 2, 3, 0,
            6,
            3, 1, 5,
            2,
            2
    };
    int32_t index[] = {1, 2, 5, 7, 11, 12, 15, 16, 17};
    char message[] = "Message from the root";

    // Initialize the MPI environment

    MPI_Init(&argc, &argv);


    MPI_Comm tree_comm;
    MPI_Graph_create(MPI_COMM_WORLD, nodes_number, index, edges, true, &tree_comm);

    // Get the rank of the process
    int32_t rank, count;
    MPI_Status status;

    MPI_Comm_size(tree_comm, &count);
    MPI_Comm_rank(tree_comm, &rank);

    // Get neighbours number
    int32_t neighbors_count;
    MPI_Graph_neighbors_count(tree_comm, rank, &neighbors_count);

    // Get neighbours list
    auto *neighbors = new int32_t[neighbors_count];

    // Get neighbours
    MPI_Graph_neighbors(tree_comm, rank, neighbors_count, neighbors);

    // Apply BFS on current state

    // If the current node is the root, send message to all
    if(rank == neighbors[0]) {
        for(int32_t i = 1; i < neighbors_count; i++) {
            // Send message to all nodes
            MPI_Send(message, (int32_t)strlen(message) + 1, MPI_CHAR, neighbors[i], 99, tree_comm);
        }
    }
        // If the current node is a leaf, he receives the message
    else {
        const int32_t message_len = 30;
        char message_received[message_len];

        MPI_Recv(message_received, message_len, MPI_CHAR, neighbors[0], 99, tree_comm, &status);
        printf("Node %d received message | from node: %d, info: %s\n", rank, neighbors[0], message_received);

        // Forward the message
        for(int32_t i = 1; i < neighbors_count; i++)
            MPI_Send(message_received, (int32_t)strlen(message_received) + 1, MPI_CHAR, neighbors[i], 99, tree_comm);
    }

    // Finalize the MPI environment.

    MPI_Finalize();
}