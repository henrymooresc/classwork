#include "threads_data.h"

// Linked list of threads that are ready to run, finished running, or are waiting on a lock
// Note that the first element of ready_list is the currently running thread.
thread_list *ready_list, *finished_list;
thread_list *waiting_list[NUM_LOCKS][CONDITIONS_PER_LOCK];

// Array that holds lock status, 0 unlocked 1 locked
int LOCKS_STATUS[NUM_LOCKS];

// Counter for threads created and used as the thread ID
int NUM_THREADS = 0;

int interruptsAreDisabled;

// Wrapper function to make telling when the function finishes easier
void wrapFunc(thread_info *t_info)
{
    thFuncPtr func = t_info->fPtr;
    void *results;

    interruptsAreDisabled = 0;
    results = func(t_info->args);
    interruptsAreDisabled = 1;

    threadExit(results);
}

// Initializes linked lists and logs the main thread's information
void threadInit()
{
    interruptsAreDisabled = 1;

    // Initializing lists
    ready_list = makeList();
    finished_list = makeList();

    for (int i = 0; i < NUM_LOCKS; i++)
    {
        for (int j = 0; j < CONDITIONS_PER_LOCK; j++)
        {
            waiting_list[i][j] = makeList();
        }
    }

    for (int k = 0; k < NUM_LOCKS; k++)
        LOCKS_STATUS[k] = 0;

    // Allocating memory for main thread and logging info
    thread_info *main_thread;
    main_thread = malloc(sizeof(thread_info));
    main_thread->cont = malloc(sizeof(ucontext_t));

    getcontext(main_thread->cont);
    main_thread->id = NUM_THREADS++;

    addToList(ready_list, main_thread);

    interruptsAreDisabled = 0;
}

// Handles creation of a new thread and returns the ID of the new thread
int threadCreate(thFuncPtr funcPtr, void *argPtr)
{
    interruptsAreDisabled = 1;

    // Creates new thread_info object and allocates memory for its context and stack
    thread_info *current_thread, *new_thread;
    new_thread = malloc(sizeof(thread_info));
    new_thread->cont = malloc(sizeof(ucontext_t));

    getcontext(new_thread->cont);
    new_thread->cont->uc_stack.ss_sp = (void *)malloc(STACK_SIZE);
    new_thread->cont->uc_stack.ss_size = STACK_SIZE;
    new_thread->cont->uc_stack.ss_flags = 0;

    // Logs thread ID, function to run, and its arguments into the object
    new_thread->id = NUM_THREADS++;
    new_thread->fPtr = funcPtr;
    new_thread->args = argPtr;

    // Moves current thread to end of the ready to run list
    current_thread = popFromList(ready_list);
    addToList(ready_list, current_thread);

    // Adds the new thread to the front of the ready list
    addToFront(ready_list, new_thread);
    
    // Creates new context using wrapper function and thread obj
    makecontext(new_thread->cont, (void (*)(void))wrapFunc, 1, new_thread);

    // Swaps context to new running thread
    swapcontext(current_thread->cont, new_thread->cont);

    interruptsAreDisabled = 0;
    return new_thread->id;
}

// Voluntarily yields CPU to the next ready to run thread
void threadYield()
{
    interruptsAreDisabled = 1;

    // Creates new thread_info object to save the yielding thread
    thread_info *t_info;

    // Moves the yielding thread to the end of the ready list
    t_info = popFromList(ready_list);
    addToList(ready_list, t_info);

    //printf("Trying to yield id:%d to id:%d\n", t_info->id, ready_list->front->id);

    if (t_info->id != ready_list->front->id)
    {
        // Swaps context to the next thread in the ready list
        swapcontext(t_info->cont, ready_list->front->cont);
    }

    interruptsAreDisabled = 0;
}

// Joins finished threads and saves result of the finished thread
void threadJoin(int thread_id, void **result)
{
    printf("trying to join\n");
    interruptsAreDisabled = 1;

    // If the thread does not exist, returns immediately
    if ((thread_id < 0) || (thread_id >= NUM_THREADS))
        return;

    int is_finished = 0;
    thread_info *t_info;
    
    // Loops until the thread with specified ID finishes
    while (!is_finished)
    {
        // Checks if the thread is already in the finished list
        t_info = threadLookup(finished_list, thread_id);

        if (t_info == NULL)
        {
            // If thread isn't finished, yield and check again later
            //printf("thread %d not done, yielding\n", thread_id);
            threadYield();
        }
        else
        {
            // If thread is finished, check if a result is required and log it if so
            if (result != NULL)
                *result = t_info->results;
            
            is_finished = 1;
        }
    }

    interruptsAreDisabled = 0;
}

// Handles exiting threads
void threadExit(void *result)
{
    interruptsAreDisabled = 1;

    // Removes running thread from ready_list
    thread_info *current_thread;
    current_thread = popFromList(ready_list);

    // Logs the results returned from the ran function into the thread_info object
    current_thread->results = result;

    // Frees the stack allocated for the thread
    free(current_thread->cont->uc_stack.ss_sp);
    current_thread->cont->uc_stack.ss_size = 0;

    // Adds thread_info object to finished_list
    addToList(finished_list, current_thread);

    // Swaps context to next thread if there is one to run
    if (ready_list->front != NULL)
        swapcontext(current_thread->cont, ready_list->front->cont);
    
    interruptsAreDisabled = 0;
}

// Handles requests to acquire locks
void threadLock(int lockNum)
{
    interruptsAreDisabled = 1;

    int lock_acquired = 0;

    // Loops until lock is acquired
    while(!lock_acquired)
    {
        // Checks if lock is available
        if (LOCKS_STATUS[lockNum])
        {
            // If not, yields to try again later
            threadYield();
        }
        else
        {
            // If available, updates lock to acquired and proceeds
            LOCKS_STATUS[lockNum] = 1;
            lock_acquired = 1;
        }
    }

    interruptsAreDisabled = 0;
}

// Handles unlock requests
void threadUnlock(int lockNum)
{
    interruptsAreDisabled = 1;

    // Ensures lock is set to available
    LOCKS_STATUS[lockNum] = 0;

    interruptsAreDisabled = 0;
}

// Handles threads that need to wait on another thread
void threadWait(int lockNum, int conditionNum)
{
    interruptsAreDisabled = 1;

    // Ensures wait was not called on an unlocked lock
    if (LOCKS_STATUS[lockNum] != 1)
    {
        fprintf(stderr, "Unable to wait on unlocked lock");
        exit(1);
    }

    // Pops running thread that needs to wait
    thread_info *waiting_thread;
    waiting_thread = popFromList(ready_list);

    // Releases lock and adds waiting thread to waiting_list based on the lock and condition variable
    threadUnlock(lockNum);
    addToList(waiting_list[lockNum][conditionNum], waiting_thread);

    // Swaps context to next available thread to run
    swapcontext(waiting_thread->cont, ready_list->front->cont);

    // Reacquires lock once thread is signaled and returns to running
    threadLock(lockNum);

    interruptsAreDisabled = 0;
}

// Handles sending signals to threads that they may continue
void threadSignal(int lockNum, int conditionNum)
{
    interruptsAreDisabled = 1;

    // Removes signaled thread from waiting_list
    thread_info *signal_thread;
    signal_thread = popFromList(waiting_list[lockNum][conditionNum]);

    // If it is a valid thread, it is added to ready_list so that it can run
    if (signal_thread != NULL)
        addToList(ready_list, signal_thread);

    interruptsAreDisabled = 0;
}