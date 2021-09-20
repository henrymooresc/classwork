#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// TODO: Fix the magic numbers for calls and buffer size
#define MAX_CALLS 500

void __attribute__((constructor)) lib_init();
void __attribute__((destructor)) lib_destroy();

static void * (*original_malloc)(size_t size);
static void (*original_free)(void *ptr);

int numMallocs = 0;
int numFreed = 0;

void *mallocAddrs[MAX_CALLS];
void *freedAddrs[MAX_CALLS];
size_t memSizes[MAX_CALLS];

void lib_init()
{
    original_malloc = dlsym(RTLD_NEXT, "malloc");
    original_free = dlsym(RTLD_NEXT, "free");
}

// Upon destruction, runs through the void * arrays to find unfreed malloc calls
// Writes the resulting leak sizes to a text file for leakcount.c to read and output
void lib_destroy()
{
    char *buffStr = original_malloc(100000);
    int leaked;

    for (int i = 0; i < numMallocs; i++)
    {
        leaked = 1;

        for (int k = 0; k < numFreed; k++)
        {
            // Checks for the first matching address in the freed array
            // If found the freed address is removed to avoid reused address space causing matches
            if (mallocAddrs[i] == freedAddrs[k])
            {
                leaked = 0;
                freedAddrs[k] = NULL;
                break;
            }
        }

        // If no match is found, the leak is added to the buffStr
        if (leaked == 1)
        {
            char temp[20] = "";
            sprintf(temp, "%zd|", memSizes[i]);
            strcat(buffStr, temp);
        }
    }

    // Writes string of leaks found to output file for leakcount.c to read
    FILE *fp;
    fp = fopen("leaks_found.txt", "w");
    fputs(buffStr, fp);
    fclose(fp);
}

// Intercepts malloc calls to save the pointer and size allocated for later comparison
void *malloc(size_t size)
{
    void *p = original_malloc(size);

    mallocAddrs[numMallocs] = p;
    memSizes[numMallocs] = size;
    numMallocs++;

    return p;
}

// Intercepts free calls to save the pointer for later comparison
void free (void *ptr)
{
    freedAddrs[numFreed] = ptr;
    numFreed++;

    original_free(ptr);
}
