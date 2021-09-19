#define _GNU_SOURCE

#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_CALLS 50

void __attribute__((constructor)) lib_init();
void __attribute__((destructor)) lib_destroy();

static void * (*original_malloc)(size_t size);
static void (*original_free)(void *ptr);

int numMallocs = 0;
int numFreed = 0;

void *mallocAddrs[MAX_CALLS];
size_t memSizes[MAX_CALLS];
void *freedAddrs[MAX_CALLS];

void lib_init()
{
    original_malloc = dlsym(RTLD_NEXT, "malloc");
    original_free = dlsym(RTLD_NEXT, "free");
}

void lib_destroy()
{
    char *buffStr = original_malloc(100000);
    int leaked;

    for (int i = 1; i < numMallocs; i++)
    {
        leaked = 1;
        for (int k = 0; k < numFreed; k++)
        {
            if (mallocAddrs[i] == freedAddrs[k])
            {
                leaked = 0;
            }
        }

        if (leaked == 1)
        {
            char temp[20] = "";
            sprintf(temp, "%zd|", memSizes[i]);
            strcat(buffStr, temp);
        }
    }

    FILE *fp;
    fp = fopen("leaks_found.txt", "w");
    fputs(buffStr, fp);
    fclose(fp);

    /*
    char *bufstrM = original_malloc(1024);

    for (int i = 1; i < numMallocs; i++)
    {
        char temp[21] = "";
        sprintf(temp, "%p:%zd | ", mallocAddrs[i], memSizes[i]);
        strcat(bufstrM, temp);
    }
    
    char *bufstrF = original_malloc(1024);

    for (int k = 0; k < numFreed; k++)
    {
        char temp2[17] = "";
        sprintf(temp2, "%p | ", freedAddrs[k]);
        strcat(bufstrF, temp2);
    }
    
    FILE *fp;

    fp = fopen("malloc_list.txt", "w");
    fputs(bufstrM, fp);
    fclose(fp);

    fp = fopen("freed_list.txt", "w");
    fputs(bufstrF, fp);
    fclose(fp);
    */
}

void *malloc(size_t size)
{
    void *p = original_malloc(size);

    mallocAddrs[numMallocs] = p;
    memSizes[numMallocs] = size;
    numMallocs++;

    return p;
}

void free (void *ptr)
{
    freedAddrs[numFreed] = ptr;
    numFreed++;

    original_free(ptr);
}
