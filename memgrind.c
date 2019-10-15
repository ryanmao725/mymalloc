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
    free(ptr3);
    free(ptr4);
    free(ptr5);
    free(ptr1);
    printf("\n\n\n");
    ptr1 = malloc(sizeof(char) * 3);
    ptr2 = malloc(sizeof(char) * 3);
    ptr3 = malloc(sizeof(char) * 3);
    free(ptr2);
    ptr4 = malloc(sizeof(char) * 1);
    ptr5 = malloc(sizeof(char) * 3);
    free(ptr3);
    free(ptr4);
    free(ptr5);
    free(ptr1);

    return 0;
}
