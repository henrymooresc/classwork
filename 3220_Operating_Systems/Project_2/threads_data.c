#include "threads_data.h"

// Initializer for linked list
thread_list *makeList()
{
    thread_list *t_list;
    t_list = (thread_list *)malloc(sizeof(thread_list));

    t_list->front = NULL;
    t_list->back = NULL;

    return t_list;
}

// Adds thread_info object to back of the linked list
void addToList(thread_list *t_list, thread_info *t_info)
{
    if (t_list->front == NULL)
    {
        t_list->front = t_info;
        t_list->back = t_info;
        t_info->next = NULL;
    }
    else
    {
        t_list->back->next = t_info;
        t_list->back = t_info;
        t_info->next = NULL;
    }
}

// Adds thread_info object to front of the linked list
void addToFront(thread_list *t_list, thread_info *t_info)
{
    thread_info *temp;

    if (t_list->front == NULL)
    {
        t_list->front = t_info;
        t_list->back = t_info;
        t_info->next = NULL;
    }
    else
    {
        temp = t_list->front;
        t_list->front = t_info;
        t_info->next = temp;
    }
}

// Pops the front element from the list
thread_info *popFromList(thread_list *t_list)
{
    thread_info *t_info;

    if (t_list->front == NULL)
    {
        return NULL;
    }
    else if (t_list->front == t_list->back)
    {
        t_info = t_list->front;
        t_list->front = NULL;
        t_list->back = NULL;
    }
    else
    {
        t_info = t_list->front;
        t_list->front = t_info->next;
        t_info->next = NULL;
    }

    return t_info;
}

// Finds and returns thread_info object in list from given ID
thread_info *threadLookup(thread_list *t_list, int id)
{
    thread_info *t_info;

    t_info = t_list->front;

    while (t_info != NULL)
    {
        if (t_info->id == id)
        {
            return t_info;
        }
        t_info = t_info->next;
    }

    return NULL;
}