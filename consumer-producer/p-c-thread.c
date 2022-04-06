#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_THREAD 2

int shareNum;

void *producer_thread();
void *consumer_thread();

int main()
{
    int num = 0;
    int rc = 0, i = 0;
    int result[NUM_THREAD];

    pthread_t thread[NUM_THREAD];
    pthread_create(&thread[0], NULL, &producer_thread, (void *)&num);
    pthread_create(&thread[1], NULL, &consumer_thread, (void *)&num);

    srand(time(NULL));
    for (i = 0; i < NUM_THREAD; i++)
    {
        rc = pthread_join(thread[1], (void **)&result[i]);
        if (rc != 0)
        {
            fprintf(stderr, "---------- Done ----------\n");
            exit(1);
        }
    }
    return 0;
}

void *producer_thread()
{
    while (1)
    {
        shareNum = rand();

        fprintf(stderr, "[Producer thread] I gave number : %d\n", shareNum);
        usleep(1900000);
    }
}

void *consumer_thread()
{
    while (1)
    {
        fprintf(stderr, "[Consumer thread] Number I got : %d\n\n", shareNum);
        usleep(2000000);
    }
}