// Actual implementation of the function prototypes declared in the header file "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

int main(int argc, char** argv) {
    int* k = (int*)malloc(sizeof(int) * 8);
}


static char myblock[4096];

void* mymalloc(size_t size, char* file, int line) {
    printf("%u", size);
}
