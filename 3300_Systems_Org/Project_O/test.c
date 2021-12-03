#include <stdio.h>

#define NUM 0x160

int main()
{
    int addr = (int)NUM;
    int ind = (addr >> 6) & 0x3f;
    int tag = addr >> 12;

    printf("a:%d | i:%d | t:%d\n", addr, ind, tag);
    printf("size:%ld\n", sizeof(NUM));

    return 0;
}