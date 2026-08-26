#include <unistd.h>
#include <stdlib.h>

int process_stream_open(const char *file, char *const argv[], char mode)
{
    int fd[2];
    pid_t pid;

    if (!file || !argv || (mode != 'r' && mode != 'w'))
        return (-1);

    if (pipe(fd) == -1)
        return (-1);

    pid = fork();
    if (pid == -1)
    {
        close(fd[0]);
        close(fd[1]);
        return (-1);
    }

    if (pid == 0)
    {
        if (mode == 'r')
        {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
        }
        else
        {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
        }
        execvp(file, argv);
        exit(1);
    }

    if (mode == 'r')
    {
        close(fd[1]);
        return (fd[0]);
    }
    else
    {
        close(fd[0]);
        return (fd[1]);
    }
}