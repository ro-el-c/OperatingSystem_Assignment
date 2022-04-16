#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_MESSAGE 64
#define MAX_LOOP 10
#define READ 0
#define WRITE 1

int main()
{
    int fd[2];
    pid_t pid;
    char buf[MAX_MESSAGE];

    if (pipe(fd) < 0)
    {
        printf("pipe oerror\n");
        exit(1);
    }
    if ((pid = fork()) < 0)
    {
        printf("fork error\n");
        exit(1);
    }

    if (pid > 0)
    {
        close(fd[READ]);
        for (int i = 0; i < MAX_LOOP; i++)
        {
            sprintf(buf, "message #%d\n", i + 1);
            write(fd[WRITE], buf, strlen(buf));
            sleep(1);
        }
    }
    else
    {
        close(fd[WRITE]);
        for (int i = 0; i < MAX_LOOP; i++)
        {
            read(fd[READ], buf, MAX_MESSAGE);
            printf("[message from parent] %s\n", buf);
            fflush(stdout);
            // sleep(1); // 없애도 똑같음 (기다림)
        }
    }
    exit(0);
}