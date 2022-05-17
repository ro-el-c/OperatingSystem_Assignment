#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

// 4번 조건 _ buffer 크기 32 이하, 생성 및 소비 data 1000개 이상
#define MAX_BUF 32
#define MAX_LOOP 200 // 200개씩 5 스레드 -> 총 data 1000개 ()

// 4번 조건 _ circular buffer는 하나로, buffer 크기는 위에서 지정함
int buffer[MAX_BUF];   // circular buffer
int counter = 0;       // 현재 buffer 안에 있는 data 수
int in = -1, out = -1; // data 저장 및 빼낼 위치

// 1번 조건 mutex 생성 _ 강의 자료의 cs semaphore 대신 사용
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
/* semaphore 사용으로 필요 X */
// pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER;
// pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;

// 2번 조건 semaphore 생성
// buffer의 상태에 다른 대기 및 실행 재개 설정
sem_t full, empty;

void *producer(void *arg)
{
    int i;
    int data; // 난수 담을 변수

    for (i = 0; i < MAX_LOOP; i++)
    {
        long tid = (long)arg;

        // 5번 조건 _ mutex, semnaphore 효과 보이도록
        usleep(500000);

        // P연산 _ 세마포어 값 감소
        // 대기하다가 빈 셀이 있으면 아래 코드 실행
        sem_wait(&full);

        pthread_mutex_lock(&mutex); // 공유 자원 접근을 위한 임계구역 설정

        /* semaphore로 buffer의 full, empty 처리해주기 때문에 필요 X
        if (counter == MAX_BUF)
            pthread_cond_wait(&buffer_has_space, &mutex);
        */

        data = rand() % 0xFFFF; // 난수 생성 및 저장

        in = (in + 1) % MAX_BUF; // circular buffer를 위한 mod 연산
        buffer[in] = data;

        counter++; // cicular buffer 내 data 증가

        printf("producer # %ld => in: %d, counter: %d, data: %d\n", tid, in, counter, data);

        /* semaphore로 buffer의 full, empty 처리해주기 때문에 필요 X
        pthread_cond_signal(&buffer_has_data);
        */

        pthread_mutex_unlock(&mutex); // 임계 구역 종료

        // V연산 _세마포어 값 증가
        // empty가 0 이상 -> 다른 스레드의 sem_wait(empty)통해 대기 중인 스레드 깨움
        sem_post(&empty);
    }
}

void *consumer(void *arg)
{
    long tid = (long)arg;
    int i;
    int data;

    for (i = 0; i < MAX_LOOP; i++)
    {
        // 5번 조건 _ mutex, semnaphore 효과 보이도록
        usleep(900000);

        // P연산 _ 세마포어 값 감소
        // 대기하다가 저장된 자원이 셀이 있으면 아래 코드 실행
        sem_wait(&empty);

        pthread_mutex_lock(&mutex); // circular buffer를 위한 mod 연산

        /* semaphore로 buffer의 full, empty 처리해주기 때문에 필요 X
        if (counter == 0)
            pthread_cond_wait(&buffer_has_data, &mutex);
        */

        out = (out + 1) % MAX_BUF; // 공유 자원 접근을 위한 임계구역 설정
        data = buffer[out];

        counter--; // cicular buffer 내 data 감소

        printf("consumer # %ld => out: %d, counter: %d, data: %d\n", tid, out, counter, data);

        /* semaphore로 buffer의 full, empty 처리해주기 때문에 필요 X
        pthread_cond_signal(&buffer_has_space);
        */

        pthread_mutex_unlock(&mutex);

        // V연산 _ 세마포어 값 증가
        // sem_wait(empty)통해 대기 중인 스레드 깨움
        sem_post(&full);
    }
}

int main(void)
{
    int i;
    srand(time(NULL));

    // 3번 조건 _ producer, consumer 스레드 각 5개 이상 -> 5개씩 총 10개

    pthread_t threads[10];

    // semaphore 초기화
    sem_init(&full, 0, MAX_BUF); // 32개의 빈 셀이 없어지면 full
    sem_init(&empty, 0, 0);      // 자료가 있는 셀이 0 개면 empty

    // producer 스레드 생성
    for (i = 0; i < 5; i++)
        pthread_create(&threads[i], NULL, &producer, (void *)((long)i));

    // consumer 스레드 생성
    for (i = 0; i < 5; i++)
        pthread_create(&threads[i + 5], NULL, &consumer, (void *)((long)i));

    // 스레드 종료 기다림
    for (i = 0; i < 10; i++)
        pthread_join(threads[i], NULL);

    // semaphore 관련 리소스 소멸
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}