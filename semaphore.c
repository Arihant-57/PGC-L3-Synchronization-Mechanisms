/*
 * Experiment 3: Synchronization Mechanisms in Parallel Programs
 * File: semaphore.c
 * Objective: Demonstrate binary semaphore synchronization using POSIX
 *            pthreads and semaphores (`sem_t`, `sem_wait`, `sem_post`).
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define ITERATIONS_PER_THREAD 1000000L

sem_t mutex_sem;
long shared_counter = 0;

void *worker_task(void *arg) {
    long thread_id = (long)arg;

    for (long i = 0; i < ITERATIONS_PER_THREAD; i++) {
        // Wait (P operation): decrement semaphore, block if 0
        sem_wait(&mutex_sem);

        // Critical section
        shared_counter++;

        // Signal (V operation): increment semaphore, unblock waiting thread
        sem_post(&mutex_sem);
    }

    pthread_exit(NULL);
}

int main(void) {
    pthread_t threads[2];
    struct timespec start, end;

    printf("=== Experiment 3: POSIX Semaphore Synchronization ===\n");
    printf("Number of threads: 2\n");
    printf("Iterations / thread: %ld\n", ITERATIONS_PER_THREAD);
    printf("Expected total   : %ld\n", 2 * ITERATIONS_PER_THREAD);

    // Initialize unnamed binary semaphore (value = 1)
    sem_init(&mutex_sem, 0, 1);

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (long t = 0; t < 2; t++) {
        pthread_create(&threads[t], NULL, worker_task, (void *)t);
    }

    for (int t = 0; t < 2; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Final Counter    : %ld\n", shared_counter);
    printf("Status           : %s\n", (shared_counter == 2 * ITERATIONS_PER_THREAD) ? "Correct (Thread-Safe)" : "INCORRECT");
    printf("Execution Time   : %.4f seconds\n", elapsed);

    // Destroy semaphore resource
    sem_destroy(&mutex_sem);

    return 0;
}
