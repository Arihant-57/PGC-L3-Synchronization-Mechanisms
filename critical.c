/*
 * Experiment 3: Synchronization Mechanisms in Parallel Programs
 * File: critical.c
 * Objective: Resolve race condition using OpenMP critical sections
 *            to ensure mutual exclusion on a shared counter.
 */

#include <stdio.h>
#include <omp.h>

#define TOTAL_INCREMENTS 10000000L

int main(void) {
    long counter = 0;
    int num_threads = 4;

    omp_set_num_threads(num_threads);

    printf("=== Experiment 3: Critical Section Synchronization ===\n");
    printf("Target iterations : %ld\n", TOTAL_INCREMENTS);
    printf("Number of threads : %d\n", num_threads);

    double start_time = omp_get_wtime();

    #pragma omp parallel for
    for (long i = 0; i < TOTAL_INCREMENTS; i++) {
        #pragma omp critical
        {
            // Only one thread at a time enters this critical region
            counter++;
        }
    }

    double elapsed_time = omp_get_wtime() - start_time;

    printf("Final Counter     : %ld\n", counter);
    printf("Expected Value    : %ld\n", TOTAL_INCREMENTS);
    printf("Status            : %s\n", (counter == TOTAL_INCREMENTS) ? "Correct (Thread-Safe)" : "INCORRECT");
    printf("Execution Time    : %.4f seconds\n", elapsed_time);

    return 0;
}
