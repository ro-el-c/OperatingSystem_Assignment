# OperatingSystem_Assignment
### 2022 3-1 운영체제 프로그래밍 과제

> #### consumer-producer
>> Process 방식
>    - fork 로 자식 프로세스 하나 생성시킨 후, 두 프로세스 간의 pipe 기능을 통해 producer-consumer 구현
>> Thread 방식
>    - pthread_create 로 thread 두 개(producer_thread, consumer_thread) 새로 생성시켜 두 thread 간에 전역변수를 통해 consumer-producer 구현