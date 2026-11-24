/*
 * Experiment 3: Synchronization Mechanisms in Parallel Programs
 * File: compare_sync.c
 * Objective: Systematically benchmark and compare execution time, contention
 *            overhead, and output correctness across synchronization mechanisms:
 *            1. None (Unsynchronized / Data Race)
 *            2. Critical Section (#pragma omp critical)
 *            3. Atomic Operation (#pragma omp atomic)
 *            4. OpenMP Lock (omp_lock_t)
 *            across 1, 2, 4, and 8 threads for 10,000,000 increments.
 */

#include <stdio.h>
#include <omp.h>

#define N 10000000L

long benchmark_none(int nthreads) {
    long counter = 0;
    omp_set_num_threads(nthreads);

    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        counter++;
    }

    return counter;
}

long benchmark_critical(int nthreads) {
    long counter = 0;
    omp_set_num_threads(nthreads);

    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        #pragma omp critical
        {
            counter++;
        }
    }

    return counter;
}

long benchmark_atomic(int nthreads) {
    long counter = 0;
    omp_set_num_threads(nthreads);

    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        #pragma omp atomic
        counter++;
    }

    return counter;
}

long benchmark_lock(int nthreads) {
    long counter = 0;
    omp_lock_t lock;

    omp_init_lock(&lock);
    omp_set_num_threads(nthreads);

    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        omp_set_lock(&lock);
        counter++;
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);
    return counter;
}

void evaluate_mechanism(const char *mechanism_name, long (*benchmark_fn)(int), int nthreads) {
    double t_start = omp_get_wtime();
    long result = benchmark_fn(nthreads);
    double elapsed = omp_get_wtime() - t_start;

    const char *status = (result == N) ? "Correct" : "INCORRECT";

    printf("%-12s | %7d | %12ld | %10s | %10.4f s\n",
           mechanism_name, nthreads, result, status, elapsed);
}

int main(void) {
    int thread_counts[] = {1, 2, 4, 8};
    int total_configs = sizeof(thread_counts) / sizeof(thread_counts[0]);

    printf("========================================================================\n");
    printf("   Experiment 3: Synchronization Mechanisms Performance Benchmark      \n");
    printf("   Workload: %ld Shared Counter Increments                            \n", N);
    printf("========================================================================\n");
    printf("Mechanism    | Threads |  Final Count | Correctness |   Time (s)  \n");
    printf("------------------------------------------------------------------------\n");

    for (int idx = 0; idx < total_configs; idx++) {
        int t = thread_counts[idx];

        evaluate_mechanism("None", benchmark_none, t);
        evaluate_mechanism("Critical", benchmark_critical, t);
        evaluate_mechanism("Atomic", benchmark_atomic, t);
        evaluate_mechanism("Lock", benchmark_lock, t);

        printf("------------------------------------------------------------------------\n");
    }

    printf("\nBenchmark Completed. Target Correct Counter = %ld\n", N);
    return 0;
}
