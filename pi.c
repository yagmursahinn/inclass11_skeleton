#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    long interval, local_interval;
    double pi_ref = 3.141592653589793238462643;
    double local_pi = 0.0, pi = 0.0, x;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of intervals>\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    interval = atol(argv[1]);
    
    // Distribute intervals to each process
    local_interval = interval / size;

    double h = 1.0 / (double) interval;

    // Start timing on the root process
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    // Local computation of π
    for (int i = rank * local_interval + 1; i <= (rank + 1) * local_interval; i++) {
        x = h * (i - 0.5);
        local_pi += 4.0 / (1.0 + x * x);
    }

    local_pi *= h;

    // Gather all partial π results at the root process
    MPI_Reduce(&local_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Finalization and output on root process
    if (rank == 0) {
        end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;
        double error = fabs(pi - pi_ref);

        printf("Calculated π: %.25f\n", pi);
        printf("Elapsed time: %f seconds\n", elapsed_time);
        printf("Error is: %.25f\n", error);
    }

    MPI_Finalize();
    return 0;
}