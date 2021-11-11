#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define PAGESIZE 4096
#define SMALL_OBJ_LIMIT 1024

void __attribute__((constructor)) lib_init();

/*
    structs used to create the page headers and their
    associated linked list of free "blocks"
*/
typedef struct linked_list
{
    char *ptr;
    int is_free;
    struct linked_list *next;
} linked_list;

typedef struct memory_page
{
    size_t obj_size;
    linked_list *head;
} memory_page;

/*
    Helper function to find the first free "block"
    Iterates through linked list until it finds a
    block with a positive is_free flag
*/
linked_list *find_first_free(linked_list *head)
{
    linked_list *temp = head;

    while (temp != NULL)
    {
        if (temp->is_free == 1)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Array of the "small object" pages
memory_page *pages[10];


// My bad version of malloc.
void *malloc(size_t size)
{
    // Checks for valid size
    if (size == 0)
    {
        return NULL;
    }

    // Checks if size given is "small" or "big"
    if (size <= SMALL_OBJ_LIMIT)
    {
        /*
            Janky calculation to convert the size given to the next
            highest power of 2. Makes finding the correct page easier.
        */
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

        // Janky way of finding the correct index in small pages array
        int temp = (int)new_size;
        int index = 0;

        while (temp != 2)
        {
            temp = temp / 2;
            index += 1;
        }
        
        // Gets the correct page and attempts to find a free "block"
        memory_page *page = pages[index];

        linked_list *node = find_first_free(page->head);

        if (node == NULL)
        {
            perror("no free node");
            return NULL;
        }

        // Updates struct information and returns found free "block"
        void *space = node->ptr;
        node->is_free = 0;

        return space;
    }
    else
    {
        // If the given size was "big" it gets its own page made
        size_t new_size = size + sizeof(memory_page);

        memory_page *page = mmap(NULL, new_size, 
                        PROT_READ | PROT_WRITE, 
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);

        page->obj_size = size;

        return page + 1;
    }
}

// My bad version of calloc
void *calloc(size_t num_objs, size_t size)
{
    // Checks if the inputs are valid
    if (num_objs == 0 || size == 0)
    {
        return NULL;
    }

    // Calculates the total size needed and calls malloc
    size_t real_size = size * num_objs;

    void *ptr = malloc(real_size);

    // Initialize bytes to 0
    memset(ptr, 0, real_size);

    return ptr;
}

// My bad version of realloc
void *realloc(void *ptr, size_t new_size)
{
    // If null pointer is given, just calls malloc
    if (ptr == NULL)
    {
        return malloc(new_size);
    }

    // My lovely janky calculation of the power of 2 again
    size_t real_size;

    if ((new_size & (new_size - 1)) != 0)
    {
        real_size = 1;
        while (real_size < new_size)
        {
            real_size *= 2;
        }
    }
    else
    {
        real_size = new_size;
    }

    // Finds the header address by zeroing the last 3 hex digits of given address
    memory_page *header = (memory_page *)((size_t)ptr & ~((1 << 12) - 1));

    // Iterates through free list to check if the pointed to block is free
    linked_list *temp = header->head;
    if (temp != NULL)
    {
        while (temp->ptr != ptr)
        {
            temp = temp->next;
        }

        if (temp->is_free == 1)
        {
            return NULL;
        }
    }

    // Checks if the given new size is the same object size as the page
    if (real_size != header->obj_size)
    {
        // Creates new allocation
        void *new_ptr = malloc(new_size);

        // If no memory was available, return NULL
        if (new_ptr == NULL)
        {
            return NULL;
        }
        
        // Sets the size of data to copy to the smallest of the two
        size_t copy_size;
        if (real_size < header->obj_size)
        {
            copy_size = real_size;
        }
        else
        {
            copy_size = header->obj_size;
        }

        // Copies old data to new pointer, frees old pointer, and returns new pointer
        memcpy(new_ptr, ptr, copy_size);
        free(ptr);
        return new_ptr;
    }
    else
    {
        // If the new size fits in the same block, just returns the original pointer
        return ptr;
    }
}

// My bad version of free
void free(void *ptr)
{
    // If given NULL, nothing to free
    if (ptr == NULL)
    {
        return;
    }

    // Finds the header address by zeroing the last 3 hex digits of given address
    memory_page *header = (memory_page *)((size_t)ptr & ~((1 << 12) - 1));

    // Checks if it is a "small" or "big" allocation
    if (header->obj_size <= SMALL_OBJ_LIMIT)
    {
        // Iterates through the block list to find the given pointer
        linked_list *temp = header->head;

        while (temp->ptr != ptr)
        {
            if (temp->next == NULL)
            {
                perror("Can't find addr to free");
                return;
            }
            temp = temp->next;
        }

        // Updates struct info for newly freed "small" block
        temp->is_free = 1;
    }
    else
    {
        // If "big" allocation, calculate size to free and call munmap
        size_t real_size = header->obj_size + sizeof(memory_page);

        munmap(header, real_size);
    }
}

// Library intializer
void lib_init()
{
    // Calculates size of set "small" pages + header
    size_t size = PAGESIZE + sizeof(memory_page);

    // Populates the small pages array with a page for each object size (2 - 1024)
    for(int i = 0; i < 10; i++)
    {
        pages[i] = mmap(NULL, size, 
                        PROT_READ | PROT_WRITE, 
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);

        int power = 1;

        for(int j = 0; j < i+1; j++)
        {
            power *= 2;
        }

        // Calculates size of the linked list and maps it
        size_t list_size = sizeof(linked_list) * (PAGESIZE/power);

        linked_list *list = mmap(NULL, list_size,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);

        // Sets struct info
        pages[i]->obj_size = (size_t)power;
        pages[i]->head = list;

        list->ptr = (char *)(pages[i] + 1);
        list->is_free = 1;

        // Iterates through the page to separate the "blocks"
        for(int k = 1; k < PAGESIZE/power; k++)
        {
            // I used char * to be able to add single bytes to pointers
            char *temp = list->ptr + power;
            list->next = list + 1;
            list = list->next;
            list->ptr = temp;
            list->is_free = 1;
        }
    }
}