#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

int main(int argc, char** argv) {
    char* k = malloc(sizeof(char) * 2);
    k[0] = 'C';
    k[1] = 'H';
    printf("%s\n", k);
    char* j = malloc(sizeof(char) * 4);
    return 0;
}
