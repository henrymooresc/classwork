#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PAGESIZE 4096
#define SMALL_OBJ_LIMIT 1024

void __attribute__((constructor)) lib_init();

typedef struct memory_page
{
    size_t obj_size;
    size_t amount_used;
    struct memory_page *next;
} memory_page;

typedef struct page_list
{
    struct memory_page *head, *tail;
} page_list;

memory_page *pageLookup(page_list *plist, size_t osize)
{
    memory_page *page;
    page = plist->head;

    while (page != NULL)
    {
        if (page->obj_size == osize)
        {
            return page;
        }
        page = page->next;
    }

    return NULL;
}

page_list *allocs;
page_list *freed;

void lib_init()
{
    allocs->head = NULL;
    allocs->tail = NULL;
    freed->head = NULL;
    freed->tail = NULL;
}

void *malloc(size_t size)
{
    if (0 < size <= SMALL_OBJ_LIMIT)
    {
        size_t new_size;

        if ((size & (size - 1)) != 0)
        {
            new_size = 1;
            while (new_size < size)
            {
                new_size *= 2;
            }
        }
        else
        {
            new_size = size;
        }

        memory_page *page_check = pageLookup(allocs, new_size);

        if (page_check == NULL)
        {
            memory_page *page = mmap(NULL, PAGESIZE, 
                                PROT_READ | PROT_WRITE, 
                                MAP_PRIVATE | MAP_ANONYMOUS,
                                -1, 0);
            
            page->obj_size = new_size;
            page->amount_used = new_size + sizeof(memory_page);
            page->next = NULL;

            if (allocs->head == NULL)
            {
                allocs->head = page;
                allocs->tail = page;
            }
            else
            {
                allocs->tail->next = page;
                allocs->tail = page;
            }

            return page + 1;
        }
        else
        {

        }
    }
}