#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/reg.h>

int cmp_func (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Wrong input format\n");
        exit(1);
    }

    char *input = argv[1];
    char *outputFile = argv[2];

    char *temp = input;

    int argNum = 1;
    
    while (*temp)
        if (*temp++ == ' ')
            argNum++;

    char **newArgv = malloc((argNum + 1) * sizeof(char *));
    
    char *temp2;
    temp2 = strtok(input, " ");
    
    size_t length = strlen(temp2) + 1;
    newArgv[0] = malloc(length);
    memcpy(newArgv[0], temp2, length);
    
    for (int i = 1; i < argNum; i++)
    {
        temp2 = strtok(NULL, " ");
        length = strlen(temp2) + 1;
        newArgv[i] = malloc(length);
        memcpy(newArgv[i], temp2, length);
    }

    pid_t child = fork();

    if (child == 0)
    {
        ptrace(PTRACE_TRACEME);

        kill(getpid(), SIGSTOP);

        execvp(newArgv[0], newArgv);
    }
    else
    {
        int status, syscallNum;

        int numCalls = 0;
        int *syscallArray = malloc(sizeof(int));

        waitpid(child, &status, 0);

        ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

        while (1)
        {
            ptrace(PTRACE_SYSCALL, child, 0, 0);
            waitpid(child, &status, 0);

            if (WIFEXITED(status) || WIFSIGNALED(status))
                break;

            if (!(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80))
            {
                syscallNum = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*ORIG_RAX, NULL);
                
                numCalls++;
                syscallArray = realloc(syscallArray, sizeof(int) * numCalls);
                syscallArray[numCalls - 1] = syscallNum;
            }
            ptrace(PTRACE_CONT, child, NULL, NULL);
        }

        FILE *fp = fopen(outputFile, "w");

        qsort(syscallArray, numCalls, sizeof(int), cmp_func);
        for (int i = 0; i < numCalls;)
        {
            int count = 1;
            int num = syscallArray[i++];

            for (; i < numCalls && syscallArray[i] == num; i++, count++);

            fprintf(fp, "%d\t%d\n", num, count);
        }
        fclose(fp);
        printf("Finished tracing\n");
        
    }
}