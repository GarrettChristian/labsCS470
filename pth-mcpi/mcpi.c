/**
 * mcpi.c
 *
 * CS 470 Pthreads Lab
 * Based on IPP Programming Assignment 4.2
 *
 * Names: 
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "timer.h"

int thread_count;               // thread count
long total_darts = 0;           // dart count
long darts_in_circle = 0;       // number of hits

void* throw_darts(void* arg)
{
    // seed pseudo-random number generator (TODO: use thread ID as seed)
    unsigned long seed = 0;

    for (long dart = 0; dart < total_darts; dart++) {

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
            darts_in_circle++;
        }
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    // check and parse command-line arguments
    if (argc != 3) {
        printf("Usage: %s <num-darts> <num-threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    total_darts = strtoll(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);

    START_TIMER(darts)

    // simulate dart throws (TODO: spawn multiple threads)
    pthread_t thread_handle;
    pthread_create(&thread_handle, NULL, throw_darts, (void*)0);
    pthread_join(thread_handle, NULL);

    STOP_TIMER(darts)

    // calculate pi
    double pi_est = 4 * darts_in_circle / ((double)total_darts);
    printf("Estimated pi: %e   Time elapsed: %.3lfs  w/  %d thread(s)\n",
            pi_est, GET_TIMER(darts), thread_count);

    // clean up
    return EXIT_SUCCESS;
}


