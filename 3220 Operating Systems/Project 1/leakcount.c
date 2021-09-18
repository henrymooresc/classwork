#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    char *args[argc];

    for (int i = 1; i < argc; i++)
    {
        args[i - 1] = argv[i];
    }

    args[argc - 1] = NULL;

    char *const envs[] = {"LD_PRELOAD=./memory_shim.so", NULL};

    if (fork() == 0)
    {
        printf("Running in the child\n");

        int i = execvpe(args[0], args, envs);
        if (i < 0)
            perror("execvp");
    }
    wait(NULL);

    printf("Done\n");
}