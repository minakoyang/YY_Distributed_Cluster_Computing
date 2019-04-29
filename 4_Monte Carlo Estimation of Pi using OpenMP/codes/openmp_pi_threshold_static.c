#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int threshold_omp = 100;

void main(int argc, char *argv[]) {
	int i;
    int count = 0;
	double x, y;
	int samples, nthreads;
	double pi;

	samples = atoi(argv[1]);
	nthreads = atoi(argv[2]);
    omp_set_num_threads(nthreads);
    double start = omp_get_wtime();
    
        #pragma omp parallel for if (samples > threshold_omp) default(none) schedule(static) private(x, y, i) shared(samples) reduction(+:count)
        for (i = 0; i < samples; i++) {
            x = (double)random() / RAND_MAX;
            y = (double)random() / RAND_MAX;
            if (x*x + y*y <= 1){
                count++;
            }
        }
    
	double end = omp_get_wtime();
	printf("elapsed time: %.16g\n", end - start);

	pi = 4.0 * ((double)count/(double)samples);
 	printf("Count = %d, Sample = %d, Estimate of pi = %7.5f\n", count, samples, pi);
}