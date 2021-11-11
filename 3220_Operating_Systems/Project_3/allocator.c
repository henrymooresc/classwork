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

typedef struct linked_list
{
    char *ptr;
    int is_free;
    struct linked_list *next;
} linked_list;

typedef struct memory_page
{
    size_t obj_size;
    int free_blocks;
    linked_list *head;
} memory_page;

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

memory_page *pages[10];

void *malloc(size_t size)
{
    if (size <= 0)
    {
        return NULL;
    }

    if (size <= SMALL_OBJ_LIMIT)
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

        int temp = (int)new_size;
        int index = 0;

        while (temp != 2)
        {
            temp = temp / 2;
            index += 1;
        }
        
        memory_page *page = pages[index];

        linked_list *node = find_first_free(page->head);

        if (node == NULL)
        {
            perror("no free node");
            return NULL;
        }

        void *space = node->ptr;
        node->is_free = 0;
        page->free_blocks -= 1;

        return space;
    }
    else
    {
        size_t new_size = size + sizeof(memory_page);

        memory_page *page = mmap(NULL, new_size, 
                        PROT_READ | PROT_WRITE, 
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);

        page->obj_size = size;

        return page + 1;
    }
}

void *calloc(size_t num_objs, size_t size)
{
    if (num_objs == 0 || size == 0)
    {
        return NULL;
    }

    size_t real_size = size * num_objs;

    void *ptr = malloc(real_size);

    memset(ptr, 0, real_size);

    return ptr;
}

void free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    memory_page *header = (memory_page *)((size_t)ptr & ~((1 << 12) - 1));

    if (header->obj_size <= SMALL_OBJ_LIMIT)
    {
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

        temp->is_free = 1;
        header->free_blocks += 1;
    }
    else
    {
        size_t real_size = header->obj_size + sizeof(memory_page);

        munmap(header, real_size);
    }
}

void lib_init()
//int main()
{
    size_t size = PAGESIZE + sizeof(memory_page);

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

        size_t list_size = sizeof(linked_list) * (PAGESIZE/power);

        linked_list *list = mmap(NULL, list_size,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);

        pages[i]->obj_size = (size_t)power;
        
        pages[i]->head = list;

        list->ptr = (char *)(pages[i] + 1);
        list->is_free = 1;
        pages[i]->free_blocks = 1;

        for(int k = 1; k < PAGESIZE/power; k++)
        {
            char *temp = list->ptr + power;
            list->next = list + 1;
            list = list->next;
            list->ptr = temp;
            list->is_free = 1;
            pages[i]->free_blocks += 1;
            
        }
    }

    // printf("header size: %lu\n", sizeof(memory_page));

    // int *ptr = ccalloc(4, sizeof(int));

    // for (int i = 0; i < 18; i++)
    // {
    //     printf("%p %d\n", (void *)(ptr + i), ptr[i]);
    // }

    // uint8_t *bufs[3];

    // for (int i = 0; i < 3; i++)
    // {
    //     bufs[i] = mmalloc(bufsizes[i]);
    //     printf("addr: %p | ", bufs[i]);

    //     memset(bufs[i], i, bufsizes[i]);

    //     for (int j = 0; j < bufsizes[i]; j++)
    //     {
    //         printf("%d ", *(bufs[i] + j));
    //     }
    //     printf("\n");
    // }
}



