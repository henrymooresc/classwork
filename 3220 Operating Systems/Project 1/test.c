#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    printf("In test\n");
    
    char *p = malloc(sizeof(argv[1]));

    p = argv[1];

    printf("test: %s\n", p);

    free(p);

    printf("ran test\n");
    return 1;
}