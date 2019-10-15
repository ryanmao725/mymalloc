#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

int main(int argc, char** argv) {
    char* ptr1 = malloc(sizeof(char) * 3);
    char* ptr2 = malloc(sizeof(char) * 3);
    char* ptr3 = malloc(sizeof(char) * 3);
    free(ptr2);
    char* ptr4 = malloc(sizeof(char) * 1);
    char* ptr5 = malloc(sizeof(char) * 3);
    return 0;
}
