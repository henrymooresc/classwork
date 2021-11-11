#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main()
{
    uint8_t *ptr = calloc(4, 4);

    uint8_t *ptr2 = malloc(16);
    memset(ptr2, 4, 16);

    for (int i = 0; i < 18; i++)
    {
        printf("%d - %p: %d\n", i, &ptr[i], ptr[i]);
    }
}
