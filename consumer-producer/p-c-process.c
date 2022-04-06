// 프로세스

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#define BUFSIZE 256

int main(int argc, char *argv[])
{
    pid_t pid;
    char bufin[BUFSIZE];
    char bufs[BUFSIZE];
    int fd[2];
    int bufInt;

    // pipe -> file descriptors fd[0], fd[1] set up / 실패시 오류
    if (pipe(fd) == -1)
    {
        perror("Failed to create the pipe\n");
        return 1;
    }

    // 매번 다른 난수를 생성하기 위하여 srand 함수에 현재 시간을 인자로 전달
    srand(time(NULL));

    // fork한 리턴값 pid 에 저정
    // 부모 프로세스는 자식 프로세스의 pid를 받고,
    // 자식 프로세스는 0
    pid = fork();

    if (pid == -1) // fork 실패
    {
        perror("Failed to fork\n");
        return 0;
    }

    if (pid == 0) // 자식 프로세스가 수행할 코드
    {
        for (int i = 0; i < 100; i++)
        {
            // 난수 생성
            int temp = rand();

            // 몇 번째 수인지, 자식 프로세스의 pid와 함께 생성한 난수 출력
            fprintf(stderr, "%d - Process[%ld] has made random number : %d\n", i + 1, (long)getpid(), temp);

            sprintf(bufs, "%d\n", temp);
            // fd[1]에 적기
            write(fd[1], bufs, BUFSIZE);
            usleep(150000); // ms 단위로 sleep
        }
        exit(1);
    }
    else // pid != -1 && pid != 0 : 부모 프로세스가 수행할 코드
    {
        for (int i = 0; i < 100; i++)
        {
            // fd[0]로 읽기
            read(fd[0], bufin, BUFSIZE);
            // 숫자로 형 변경
            bufInt = atoi(bufin);

            // 부모 프로세스의 pid 값과 pipe를 통해 전달 받은 난수 출력
            fprintf(stderr, "Process [%ld] has got number from child : %d.\n\n\n", (long)getpid(), bufInt);
        }
    }
    return 0;
}