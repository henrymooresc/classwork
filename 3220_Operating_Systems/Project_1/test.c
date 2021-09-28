#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{   
    
    void *p1, *p2, *p3, *p4;
    p1 = malloc(120);
    p2 = malloc(80);
    p3 = malloc(250);
    p4 = malloc(69);

    //printf("p1: %p --- p2: %p --- p3: %p\n", p1, p2, p3);
    free(p1);
    free(p3);

}