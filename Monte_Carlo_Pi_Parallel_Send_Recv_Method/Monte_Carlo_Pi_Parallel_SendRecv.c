#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int i, count, sum;
    double x, y;
    int samples = atoi(argv[1]);
    double pi, start=0.0, end=0.0;
    int rank, numProcs;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    int samples_proc = (int)(samples / numProcs);    

    count = 0;
    
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    srand((int)time(0));
    
    for (i = 0; i < samples_proc; i++) {
        
        x = (double) rand() / RAND_MAX;
        y = (double) rand() / RAND_MAX;
        
        if (x * x + y * y <= 1) {
            count++;
        }
    }

    if (rank == 0) {
        printf("Count for process 0 is %d out of %d\n", count, samples_proc);
        sum = count;
        for (i = 1; i < numProcs; i++) {
            MPI_Recv(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += count;
        }
    }
    else {
        printf("Count for process %d is %d out of %d\n", rank, count, samples_proc);
        MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    
    if (rank == 0) {
        pi = (double) sum / samples * 4;   
//         printf("Total count is %d out of %d\n", count, number);
//         printf("Estimation of PI is %f\n", pi);
//         printf("Total time is %f seconds\n", end_time - begin_time);
        printf("Final Results: np=%2d, proc_rank=%2d, Reducedsum=%2d, Samples = %d, Estimate of pi = %7.5f, Time=%fs\n",numProcs,rank, sum, samples, pi, end-start);
    }

    MPI_Finalize();    

    return 0;
}