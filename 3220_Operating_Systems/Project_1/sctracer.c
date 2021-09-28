#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/reg.h>

// Function to use in combo with qsort to order the system call numbers
int cmp_func (const void * x, const void * y)
{
   return (*(int*)x - *(int*)y);
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

    // Counts the total args passed for the program being traced
    int argNum = 1;
    char *inputPtr = input;
    
    while (*inputPtr)
        if (*inputPtr++ == ' ')
            argNum++;

    // Creating a new argv to pass to execvp
    char **newArgv = malloc((argNum + 1) * sizeof(char *));
    
    // Using strtok to parse the args given for the program being traced
    char *tempStr;
    tempStr = strtok(input, " ");
    
    size_t length = strlen(tempStr) + 1;
    newArgv[0] = malloc(length);
    memcpy(newArgv[0], tempStr, length);
    
    // Loops to populate the new argv with all given arguments
    for (int i = 1; i < argNum; i++)
    {
        tempStr = strtok(NULL, " ");
        length = strlen(tempStr) + 1;
        newArgv[i] = malloc(length);
        memcpy(newArgv[i], tempStr, length);
    }

    pid_t child = fork();

    // Sets up the trace, waits for the parent to be ready, and runs the program given
    if (child == 0)
    {
        ptrace(PTRACE_TRACEME);

        kill(getpid(), SIGSTOP);

        execvp(newArgv[0], newArgv);
    }
    else
    {
        // Sets up variables for reporting and storing calls
        int status, syscallNum;
        int numCalls = 0;
        int *syscallArray = malloc(sizeof(int));

        waitpid(child, &status, 0);

        ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

        // Loops until the process aborts or terminates
        while (!WIFEXITED(status))
        {
            do
            {
                // Waits for a call and the child to change state
                ptrace(PTRACE_SYSCALL, child, 0, 0);
                waitpid(child, &status, 0);
            
            // Loops until the process stops, bit 7 is set, or the process exits
            } while (!(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) && !(WIFEXITED(status)));

            // Logs the system call
            if (!(WIFEXITED(status)))
            {
                syscallNum = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*ORIG_RAX, NULL);
                
                numCalls++;
                syscallArray = realloc(syscallArray, sizeof(int) * numCalls);
                syscallArray[numCalls - 1] = syscallNum;
            }
        }

        FILE *fp = fopen(outputFile, "w");

        // Sorts the array of system calls
        qsort(syscallArray, numCalls, sizeof(int), cmp_func);

        // Loops through the array, printing the number of each call to the output file specified
        for (int i = 0; i < numCalls;)
        {
            int count = 1;
            int num = syscallArray[i++];

            for (; i < numCalls && syscallArray[i] == num; i++, count++);

            // Bad way of dealing with the array logging entry and exit
            if (count % 2 == 0)
                count = count / 2;

            fprintf(fp, "%d\t%d\n", num, count);
        }
        fclose(fp);
        free(syscallArray);
    }
    free(newArgv);
}