#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

int main(int argc, char** argv) {
    int rank, size, interval;
    double pi_ref = 3.141592653589793238462643;
    double rand_x, rand_y, origin_dist, pi;
    int local_inside_circle = 0, total_inside_circle = 0, points_generated = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <number of intervals>\n", argv[0]);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    interval = atoi(argv[1]);
    int local_interval = interval / size;  // Assuming interval is divisible by size

    // Seed random number generator differently on each process
    srand(time(NULL) + rank);

    double start_time;
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    // Each process performs its part of the simulation
    for (int i = 0; i < (local_interval * local_interval); i++) {
        rand_x = (double)(rand() % (interval + 1)) / interval;
        rand_y = (double)(rand() % (interval + 1)) / interval;

        origin_dist = rand_x * rand_x + rand_y * rand_y;
        if (origin_dist <= 1)
            local_inside_circle++;
    }

    // Gather all results at the root
    MPI_Reduce(&local_inside_circle, &total_inside_circle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Calculate and print the final results at the root
    if (rank == 0) {
        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;
        points_generated = local_interval * local_interval * size;
        pi = 4.0 * total_inside_circle / points_generated;
        double error = fabs(pi - pi_ref);

        printf("Calculated π: %.25f\n", pi);
        printf("Elapsed time: %f seconds\n", elapsed_time);
        printf("Error is: %.25f\n", error);
    }

    MPI_Finalize();
    return 0;
}