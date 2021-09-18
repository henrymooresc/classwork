#define _GNU_SOURCE

#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void __attribute__((constructor)) lib_init();
void __attribute__((destructor)) lib_destroy();

static void * (*original_malloc)(size_t size);
static void *(*original_free)(void *ptr);

void lib_init()
{
    original_malloc = dlsym(RTLD_NEXT, "malloc");
    printf("Initializing Library\n");
}

void lib_destroy()
{
    printf("Unloading Library\n");
}

void *malloc(size_t size)
{
    void *p = original_malloc(size);
    return p;
}

void free (void *ptr)
{
    original_free(ptr);
}
