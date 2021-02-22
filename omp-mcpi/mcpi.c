/**
 * mcpi.c
 *
 * CS 470 OpenMP Lab
 * Based on IPP Programming Assignment 5.2
 *
 * Names: Alexander Castro, Garrett Christian, Thomas Seeley
 *
 * Link to full write up:
 *	https://docs.google.com/document/d/1oZa9g-7YNiAHL9-RqKXavpZadSkC4kp9FJuDd7yi6xI/edit?usp=sharing
 *
 * What changes did you make?
 * 	 We added an import statement omp.h wrapped in an ifdef _OPENMP to compile only if 
 *	 we utilized OpenMP. We added a pragma omp parallel right before our call to throw
 *	 darts. Then in throw darts we added a local darts variable to update at the end 
 *	 protected by a pragma omp critical. Then right before each thread calculates the
 *	 darts we added another ifdef that will seed with the thread id gets how many threads
 *	 we have for the loop later.
 *
 * Does your new version scale well?
 * 	Yes we see an increase in speed as we add more threads. The solution scales 
 * 	strongly with a higher thread count.
 *
 * Are the estimates of pi accurate?
 * 	We have correct estimates for pi each time.
 *
 * How difficult was it to parallelize with OpenMP compared to Pthreads?
 * 	It was far easier to use OpenMP since we don't have to worry about the lower level 
 * 	details such as mutexes, creating, and joining.
 *
 * Does one of them perform better than the other?
 * 	The OpenMP version performed slightly better. We were surprised, since we assumed 
 * 	that it would perform worse than our P-Threads version due to the overhead of utilizing
 * 	a library.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

#ifdef _OPENMP
#include <omp.h>
#endif

long total_darts = 0;           // dart count
long darts_in_circle = 0;       // number of hits

void throw_darts()
{
    {
        // seed pseudo-random number generator
	long local_darts_in_circle = 0;
 
 	#ifdef _OPENMP
	int num_threads = omp_get_num_threads();
	unsigned long seed = omp_get_thread_num();
	#else
	int num_threads = 1;
        unsigned long seed = 0;
	#endif

        for (long dart = 0; dart < total_darts / num_threads; dart++) {

            // throw a dart by generating a random (x,y) coordinate pair
            // using a basic linear congruential generator (LCG) algorithm
            // (see https://en.wikipedia.org/wiki/Linear_congruential_generator)
            //
            seed = (1103515245*seed + 12345) % (1<<31);
            double x = (double)seed / (double)ULONG_MAX;
            seed = (1103515245*seed + 12345) % (1<<31);
            double y = (double)seed / (double)ULONG_MAX;
            double dist_sq = x*x + y*y;

            // update hit tracker
            if (dist_sq <= 1.0) {
                local_darts_in_circle++;
            }
        }

	#pragma omp critical
	darts_in_circle += local_darts_in_circle;

    }
}

int main(int argc, char* argv[])
{
    // check and parse command-line arguments
    if (argc != 2) {
        printf("Usage: %s <num-darts>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    total_darts = strtoll(argv[1], NULL, 10);

    START_TIMER(darts)

    // simulate dart throws
    #pragma omp parallel 
    throw_darts();

    STOP_TIMER(darts)

    // calculate pi
    double pi_est = 4 * darts_in_circle / ((double)total_darts);
    printf("Estimated pi: %e   Time elapsed: %.3lfs\n",
            pi_est, GET_TIMER(darts));

    // clean up
    return EXIT_SUCCESS;
}


