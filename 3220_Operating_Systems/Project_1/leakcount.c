#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int BUFFER_SIZE = 1024;

int main(int argc, char **argv)
{
    // Creating arg array to pass to execvpe
    char *args[argc];
    for (int i = 1; i < argc; i++)
    {
        args[i - 1] = argv[i];
    }
    args[argc - 1] = NULL;

    // Creates environment array to load the shim when running execvpe
    char *const envs[] = {"LD_PRELOAD=./memory_shim.so", NULL};

    // Creates a child to run the program being inspected
    if (fork() == 0)
    {
        int i = execvpe(args[0], args, envs);
        if (i < 0)
            perror("error running execvpe");
    }
    wait(NULL);

    // Reads leak data from shim output file
    char buffStr[BUFFER_SIZE];
    char *data = malloc(sizeof(char));
    FILE *fp = fopen("leaks_found.txt", "r");

    while (fgets(buffStr, sizeof(buffStr), fp) != NULL)
    {
        data = realloc(data, sizeof(data) + sizeof(buffStr));
        strcat(data, buffStr);
    }

    fclose(fp);

    int num = 0;
    int totalSize = 0;
    char *leak;

    // Chops up the string of data for printing and calculation of total leak size
    leak = strtok(data, "|");
    while (leak != NULL)
    {
        fprintf(stderr, "LEAK\t%s\n", leak);
        num++;
        totalSize += atoi(leak);
        leak = strtok(NULL, "|");
    }

    fprintf(stderr, "TOTAL %d %d\n", num, totalSize);

    // Cleanup
    free(data);
    free(leak);
}