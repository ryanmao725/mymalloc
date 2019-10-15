// Actual implementation of the function prototypes declared in the header file "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

int main(int argc, char** argv) {
    char* k = (char*)malloc(sizeof(char) * 2);
    k[0] = "c";
    k[1] = "h";
    printf("ch");
}

static char myblock[4096];

void* mymalloc(size_t size, char* file, int line) {
    printf("%u\n%u", size, sizeof(int));
    int i = 0; 
}
