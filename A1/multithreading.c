#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000000000
#define NUM_THREADS 4

// struct
struct ThreadData {
    int threadID;
    int start;
    int end;
    long long sum;
};

// function for calculating the sum of the numbers given
void* calculateSum(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    long long sum = 0;
    for (int i = data->start; i < data->end; i++) {
        sum += i;
    }
    data->sum = sum;

    printf("Thread %d: Calculated sum from %d to %d: %lld\n",
           data->threadID, data->start, data->end, data->sum);

    pthread_exit(NULL);
}

int main() {
    // Create the thread array
    pthread_t threads[NUM_THREADS];

    // We divide the work among the threads now
    struct ThreadData threadData[NUM_THREADS];
    int chunkSize = N / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].threadID = i;
        threadData[i].start = i * chunkSize;
        threadData[i].end = (i + 1) * chunkSize;
    }

    // Create and start the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Creating Thread %d\n", i);
        pthread_create(&threads[i], NULL, calculateSum, &threadData[i]);
    }

    // Wait for the threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d has finished\n", i);
    }

    // Now calculate the partition sums
    long long finalSum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        finalSum += threadData[i].sum;
    }

    // Print the final result
    printf("Sum of numbers from 0 to %d is %lld\n", N, finalSum);

    return 0;
}
