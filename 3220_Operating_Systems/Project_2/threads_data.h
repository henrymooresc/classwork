#include "mythreads.h"
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct thread_info 
{
    int id;
    ucontext_t *cont;
    thFuncPtr fPtr;
    void *args;
    void *results;
    struct thread_info *next;
} thread_info;

typedef struct thread_list
{
    struct thread_info *front, *back;
} thread_list;

thread_list *makeList();
void addToList(thread_list *, thread_info *);
void addToFront(thread_list *, thread_info *);
thread_info *popFromList(thread_list *);
thread_info *threadLookup(thread_list *, int);