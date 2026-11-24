/*
 * Experiment 3: Synchronization Mechanisms in Parallel Programs
 * File: barrier.c
 * Objective: Demonstrate thread coordination and barrier synchronization
 *            using OpenMP `#pragma omp barrier`.
 */

#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main(void) {
    int num_threads = 4;
    omp_set_num_threads(num_threads);

    printf("=== Experiment 3: Barrier Synchronization ===\n");
    printf("Number of threads: %d\n\n", num_threads);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        // Phase 1: Work before barrier
        printf("[Phase 1] Thread %d starting preliminary work\n", tid);
        if (tid % 2 == 1) {
            usleep(20000); // 20ms delay on odd threads to induce phase skew
        }
        printf("[Phase 1] Thread %d arrived at barrier point\n", tid);

        // Barrier synchronization: all threads must arrive here before any thread proceeds
        #pragma omp barrier

        // Phase 2: Work after barrier
        printf("[Phase 2] Thread %d passed barrier and proceeding to next phase\n", tid);
    }

    printf("\nAll threads synchronized successfully at barrier.\n");
    return 0;
}
