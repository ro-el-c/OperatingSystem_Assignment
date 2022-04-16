#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_BUF 8
#define MAX_LOOP 16
#define NUM_THREAD 2

int buffer[MAX_BUF];
int in = 0, out = 0;

void *producer(void *arg)
{
    int i, j;
    int data;

    for (i = 0; i < MAX_LOOP; i++)
    {
        sleep(1);

        data = rand() % 0xFFFF;

        while (((in + 1) % MAX_BUF) == out)
            usleep(1000);

        buffer[in] = data;

        in++;
        in %= MAX_BUF;

        printf("in: %d, data: %06d\n", in, data);
    }
}

void *consumer(void *arg)
{
    int i, j;
    int data;

    for (i = 0; i < MAX_LOOP; i++)
    {
        while (in == out)
            usleep(1000);

        data = (unsigned long)buffer[out];
        out++;
        out %= MAX_BUF;

        printf("out: %d, data: %06d\n", out, data);

        sleep(1);
    }
}

int main()
{
    int i, rc = 0;
    int result[NUM_THREAD];

    pthread_t threads[NUM_THREAD];

    srand(time(NULL));

    pthread_create(&threads[0], NULL, &producer, NULL);
    pthread_create(&threads[1], NULL, &consumer, NULL);

    for (i = 0; i < NUM_THREAD; i++)
    {
        rc = pthread_join(threads[1], (void **)&result[i]);
        if (rc != 0)
        {
            printf("Error in thread[%d]: %d\n", i, rc);
            exit(1);
        }
    }
    return 0;
}