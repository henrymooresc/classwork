#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
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
        int i = execvpe(args[0], args, envs);
        if (i < 0)
            perror("execvp");
    }
    wait(NULL);

    char data[100000];
    FILE *fp = fopen("leaks_found.txt", "r");
    fgets(data, sizeof(data), fp);

    fclose(fp);

    int num = 0;
    int totalSize = 0;

    char *leak;

    leak = strtok(data, "|");
    while (leak != NULL)
    {
        fprintf(stderr, "LEAK\t%s\n", leak);
        num++;
        totalSize += atoi(leak);
        leak = strtok(NULL, "|");
    }

    fprintf(stderr, "TOTAL %d %d\n", num, totalSize);
}