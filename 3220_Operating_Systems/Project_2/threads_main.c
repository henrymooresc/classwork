#include "threads_data.h"

thread_list *ready_list, *finished_list;
thread_list *waiting_list[NUM_LOCKS][CONDITIONS_PER_LOCK];
int LOCKS_STATUS[NUM_LOCKS];
int NUM_THREADS = 0;
int interruptsAreDisabled;

void wrapFunc(thread_info *t_info)
{
    thFuncPtr func = t_info->fPtr;
    void *results;

    interruptsAreDisabled = 0;
    results = func(t_info->args);
    interruptsAreDisabled = 1;

    threadExit(results);
}

void threadInit()
{
    interruptsAreDisabled = 1;

    ready_list = make_list();
    finished_list = make_list();

    for (int i = 0; i < NUM_LOCKS; i++)
    {
        for (int j = 0; j < CONDITIONS_PER_LOCK; j++)
        {
            waiting_list[i][j] = make_list();
        }
    }

    for (int k = 0; k < NUM_LOCKS; k++)
        LOCKS_STATUS[k] = 0;

    thread_info *main_thread;
    main_thread = malloc(sizeof(thread_info));
    main_thread->cont = malloc(sizeof(ucontext_t));

    getcontext(main_thread->cont);
    main_thread->id = NUM_THREADS++;

    add_to_list(ready_list, main_thread);

    interruptsAreDisabled = 0;
}

int threadCreate(thFuncPtr funcPtr, void *argPtr)
{
    interruptsAreDisabled = 1;

    thread_info *current_thread, *new_thread;
    new_thread = malloc(sizeof(thread_info));
    new_thread->cont = malloc(sizeof(ucontext_t));

    getcontext(new_thread->cont);
    new_thread->cont->uc_stack.ss_sp = (void *)malloc(STACK_SIZE);
    new_thread->cont->uc_stack.ss_size = STACK_SIZE;
    new_thread->cont->uc_stack.ss_flags = 0;
    new_thread->id = NUM_THREADS++;
    new_thread->fPtr = funcPtr;
    new_thread->args = argPtr;

    current_thread = popFromList(ready_list);
    addToList(ready_list, current_thread);

    addToFront(ready_list, new_thread);    
    
    makecontext(new_thread->cont, (void (*)(void))wrapFunc, 1, new_thread);

    swapcontext(current_thread->cont, new_thread->cont);

    interruptsAreDisabled = 0;
    return new_thread->id;
}

void threadYield()
{
    interruptsAreDisabled = 1;

    thread_info *t_info;

    t_info = popFromList(ready_list);
    addToList(ready_list, t_info);

    swapcontext(t_info->cont, ready_list->front->cont);

    interruptsAreDisabled = 0;
}

void threadJoin(int thread_id, void **result)
{
    interruptsAreDisabled = 1;

    int is_finished = 0;
    thread_info *t_info;
    
    while (!is_finished)
    {
        t_info = threadLookup(finished_list, thread_id);

        if (t_info == NULL)
        {
            if (threadLookup(ready_list, thread_id) == NULL)
                is_finished = 1;
            else
                threadYield();
        }
        else
        {
            if (result != NULL)
                *result = t_info->results;
            
            is_finished = 1;
        }
    }

    interruptsAreDisabled = 0;
}

void threadExit(void *result)
{
    interruptsAreDisabled = 1;

    thread_info *current_thread;
    current_thread = popFromList(ready_list);

    current_thread->results = result;

    free(current_thread->cont->uc_stack.ss_sp);
    current_thread->cont->uc_stack.ss_size = 0;

    addToList(finished_list, current_thread);

    if (ready_list->front != NULL)
        swapcontext(current_thread->cont, ready_list->front->cont);
    
    interruptsAreDisabled = 0;
}

void threadLock(int lockNum)
{
    interruptsAreDisabled = 1;

    int lock_acquired = 0;

    while(!lock_acquired)
    {
        if (LOCKS_STATUS[lockNum])
        {
            threadYield();
        }
        else
        {
            LOCKS_STATUS[lockNum] = 1;
            lock_acquired = 1;
        }
    }

    interruptsAreDisabled = 0;
}

void threadUnlock(int lockNum)
{
    interruptsAreDisabled = 1;

    LOCKS_STATUS[lockNum] = 0;

    interruptsAreDisabled = 0;
}

void threadWait(int lockNum, int conditionNum)
{
    interruptsAreDisabled = 1;

    if (LOCKS_STATUS[lockNum] != 1)
    {
        fprintf(stderr, "Unable to wait on unlocked lock");
        exit(1);
    }

    thread_info *waiting_thread;
    waiting_thread = popFromList(ready_list);

    threadUnlock(lockNum);
    addToList(waiting_list[lockNum][conditionNum], waiting_thread);
    swapcontext(waiting_thread->cont, ready_list->front->cont);
    threadLock(lockNum);

    interruptsAreDisabled = 0;
}

void threadSignal(int lockNum, int conditionNum)
{
    interruptsAreDisabled = 1;

    thread_info *signal_thread;
    signal_thread = popFromList(waiting_list[lockNum][conditionNum]);

    if (signal_thread != NULL)
        addToList(ready_list, signal_thread);

    interruptsAreDisabled = 0;
}