// Actual implementation of the function prototypes declared in the header file "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

static char myblock[MEM_SIZE];

void* mymalloc(size_t size, char* file, int line) {
    //*((short*)myblock) = 253;
    //*((short*)myblock) = 16387;
    //int sizes = chunksize(0);
    // Calculate the size that we need to store this memory
    size_t allocSize = 1;
    if (size >= 64) {
        allocSize += 1;
    }
    allocSize += size;
    printf("Allocation Size: %d\n", allocSize);
    // Initialize our variables
    unsigned int i = 0;
    while (i < MEM_SIZE) {
        printf("ITERATING at byte %d\n", i);
        // Analyze this memory chunk
        unsigned char* memchunk = &myblock[i];
        int inUse = (*memchunk & 1);
        int memSize = chunksize(i);
        if (inUse == 1) { // This memory is in use, skip over it
            i += memSize + 1;
            if (memSize >= 64) {
                i += 1;
            }
        } else { // This memory is not in use, analyze further
            if (memSize >= allocSize || (memSize == 0 && MEM_SIZE >= allocSize + i)) {
                // ALLOCATE THIS MEM CHUNK
                printf("ALLOCATING: %d\n", i);
                if (size < 64) {
                    printf("ALLOCATING at byte %d, with memsize %d, with value %d\n", i, size, (size << 2) + 1);
                    *(memchunk) = (size << 2) + 1;
                    return memchunk + 1;
                } else {
                    printf("ALLOCATING at byte %d, with memsize %d, with value %d\n", i, size, (size << 2) + 3 + 256);
                    *((short*)memchunk) = (size << 2) + 3 + 256;
                    return memchunk + 2;
                }
            } else {
                // Unfortunately, we don't have enough space, lets go to the next available memory chunk
                i += memSize + 1;
                if (memSize >= 64) {
                    i += 1;
                }
                // Check to see if we can still fit our memory, and if not we will terminate
                if (MEM_SIZE < allocSize) {
                    i = MEM_SIZE;
                }
            }
        }
    }
    // If we are here, we didn't find any memory slot big enough
    printf("OutOfMemory Error: \n    Attempted to allocate more memory than available in line %d, %s\n", line, file);
    return NULL;
}

void myfree(void* ptr, char* file, int line) {
    
}


int chunksize(int index) {
    unsigned char* memchunk = &myblock[index];
    unsigned int bytesize = (*memchunk >> 1) & 1;
    if (bytesize == 0) {
        return (*memchunk >> 2);
    } else {
        return (*((unsigned short*)memchunk) - 256) >> 2;
    }
}
