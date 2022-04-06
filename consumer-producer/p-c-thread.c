// 스레드

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_THREAD 2 // producer와 consumer 총 2개 스레드

// 두 스레드가 공유하는 전역 변수
int shareNum;

// producer_thread : 난수 생성 스레드
void *producer_thread();
// consumer_thread : 난수 받아오는 스레드
void *consumer_thread();

int main()
{
    int num = 0;
    int rc = 0, i = 0;
    int result[NUM_THREAD];

    // thread id - 2개
    pthread_t thread[NUM_THREAD];
    // producer, consumer 각각 thread 생성
    pthread_create(&thread[0], NULL, &producer_thread, (void *)&num);
    pthread_create(&thread[1], NULL, &consumer_thread, (void *)&num);

    // 매번 다른 난수를 생성하기 위하여 srand 함수에 현재 시간을 인자로 전달
    srand(time(NULL));

    for (i = 0; i < NUM_THREAD; i++)
    {
        // pthread_join 함수로 타겟 스레드가 종료될 때까지 다른 call을 막음
        rc = pthread_join(thread[1], (void **)&result[i]);
        if (rc != 0) // 오류
        {
            fprintf(stderr, "Error\n");
            // 스레드 종료
            exit(1);
        }
    }
    return 0;
}

// 난수 생성 스레드
void *producer_thread()
{
    while (1)
    {
        // 전역 변수(공유 변수)에 난수 생성
        shareNum = rand();

        // consumer 스레드 id, 생성한 난수 출력
        fprintf(stderr, "[Producer thread %ld] I gave number : %d\n", (long)pthread_self(), shareNum);

        // ms 단위로 sleep
        usleep(1900000);
    }
}

// 난수 받는 스레드
void *consumer_thread()
{
    while (1)
    {
        // consumer 스레드 id, producer 스레드가 생성한 난수 출력
        fprintf(stderr, "[Consumer thread %ld] Number I got : %d\n\n", (long)pthread_self(), shareNum);

        usleep(2000000);
    }
}