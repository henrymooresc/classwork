#include "threads_data.h"

thread_list *makeList()
{
    thread_list *t_list;
    t_list = (thread_list *)malloc(sizeof(thread_list));

    t_list->front = NULL;
    t_list->back = NULL;

    return t_list;
}

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
        t_list->back = t_info;
        t_list->back->next = t_info;
        t_info->next = NULL;
    }
}

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