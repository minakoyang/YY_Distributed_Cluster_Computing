#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) //argv 是一个数组 在c里面是指针 *argv = argv[]
{
   int i, count, sum; //points inside the unite quarter circle
   double x, y; // coordinates of points
   int samples; //samples number of points to generate
   double pi; //estimate of pi
   int rank, numProcs;
   double start=0.0, end=0.0; //time

   //initialize the MPI evironment

   MPI_Init(&argc, &argv);

   //Get the number of processes

   MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

   //Get the rank of the process

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   //Synchronize all processes and get the begin time
   MPI_Barrier(MPI_COMM_WORLD);
   start = MPI_Wtime();
    
   srand((int)time(0));

   samples = atoi(argv[1]); //./a.out input number

   int samples_proc = (int)(samples/numProcs);

   count = 0;
   for (i = 0; i < samples_proc; i++) {

      x = (double) rand() / RAND_MAX;
      y = (double) rand() / RAND_MAX;
      if (x*x + y*y <= 1)
         count++;
   }

   printf("Before Reduce: total sum of all counts at procs %2d is %2d \n", rank, count);


   MPI_Reduce(&count, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   printf("After Reduce: total sum of all counts at procs %2d is %2d \n", rank, sum);

   //Synchronize all processes and get the end time
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();


    if (rank==0)
    {
      //pi = 4.0 * (double)count/(double)samples;
        pi = 4.0 * (double)sum / (double)samples;
      printf("Final Results: np=%2d, proc_rank=%2d, Reducedsum=%2d, Samples = %d, Estimate of pi = %7.5f, Time=%fs\n",numProcs,rank, sum, samples, pi, end-start);
   
    }
   
   MPI_Finalize();
   return 0;
}