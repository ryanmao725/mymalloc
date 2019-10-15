#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

int main(int argc, char** argv) {
    char* ptr1 = malloc(sizeof(char) * 3);
    char* ptr2 = malloc(sizeof(char) * 65);
    char* ptr3 = malloc(sizeof(char) * 100);
    free(ptr2);
    char* ptr4 = malloc(sizeof(char) * 63);
    char* ptr5 = malloc(sizeof(char) * 1);
    free(ptr1);
    free(ptr3);
    free(ptr4);
    free(ptr5);
    char* ptr6 = malloc(sizeof(char) * 4094);
    return 0;
}
