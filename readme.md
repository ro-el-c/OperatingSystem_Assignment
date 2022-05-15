# OperatingSystem_Assignment
### 2022 3-1 운영체제 프로그래밍 과제

> ### consumer-producer (=producer-consumer)
>> Process 방식
>    - fork 로 자식 프로세스 하나 생성시킨 후, 두 프로세스 간의 pipe 기능을 통해 producer-consumer 구현
>> Thread 방식
>    - pthread_create 로 thread 두 개(producer_thread, consumer_thread) 새로 생성시켜 두 thread 간에 전역변수를 통해 consumer-producer 구현
<br>

---

<br>

> ### mutex_semaphore_producer-consumer
>  - pthread의 mutex 이용하여 circular buffer 운용 (counter 변수 이용)
>  - Buffer의 full/empty는 pthread의 semaphore 활용 (sem_init, sem_wait, sem_post)
>  - producer, consumer 쓰레드는 각각 5개 이상 동시 수행
>  - circular buffer 하나, 버퍼 크기 32 이하, 데이터 생성/소비 1000개 이상
>  - printf, sleep 적절히 사용 -> 효과 보이도록