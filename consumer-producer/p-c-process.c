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

    if (pipe(fd) == -1)
    {
        perror("Failed to create the pipe\n");
        return 1;
    }

    for (int i = 0; i < 100; i++)
    {
        srand(time(NULL));
        pid = fork();
        if (pid == -1)
        {
            perror("Failed to fork\n");
            return 0;
        }

        if (pid == 0)
        {
            int temp = rand();

            fprintf(stderr, "%d : Process[%ld] has made random number %d\n", i + 1, (long)getpid(), temp);

            sprintf(bufs, "%d\n", temp);
            write(fd[1], bufs, BUFSIZE);
            exit(1);
        }
        else
        {
            read(fd[0], bufin, BUFSIZE);
            bufInt = atoi(bufin);
            fprintf(stderr, "Process [%ld] has got number from child : %d.\n\n\n", (long)getpid(), bufInt);
            usleep(150000); // ms 단위로 sleep
        }
    }
    return 0;
}