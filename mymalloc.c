// Actual implementation of the function prototypes declared in the header file "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

static char myblock[MEM_SIZE];

void* mymalloc(size_t size, char* file, int line) {
    printf("%d\n", myblock);
    //*((short*)myblock) = 253;
    //*((short*)myblock) = 16387;
    //int sizes = chunksize(0);
    // Calculate the size that we need to store this memory
    size_t allocSize = 1;
    if (size >= 64) {
        allocSize += 1;
    }
    allocSize += size;
    // Initialize our variables
    int i = 0;
    while (i < MEM_SIZE) {
        // Analyze this memory chunk
        unsigned char* memchunk = &myblock[i];
        int inUse = (*memchunk >> 1);
        int memSize = chunksize(i);
        if (inUse == 1) { // This memory is in use, skip over it
            i += memSize + 1;
            if (memSize >= 64) {
                i += 1;
            }
        } else { // This memory is not in use, analyze further
            if (memSize == 0) { // We are on unallocated memory territory (i.e. nothing after this point has been allocated yet)
                // Verify that we have enough space in our memory
                if (MEM_SIZE - i - 1 >= size) {
                    // ALLOCATE THIS MEM CHUNK
                    printf("ALLOCATING: %d\n", i);
                    if (size < 64) {
                        *(memchunk) = (size << 2) + 1;
                        return memchunk + 1;
                    } else {
                        *((short*)memchunk) = (size << 2) + 3;
                        return memchunk + 2;
                    }
                } else {
                    // WE WILL GO OVER!
                }
            } else { // We are on previously allocated memory territory, check if we can reuse this region
                if (memSize >= allocSize) {
                    // ALLOCATE THIS MEM CHUNK
                    printf("ALLOCATING: %d\n", i);
                    if (size < 64) {
                        *(memchunk) = (size << 2) + 1;
                        return memchunk + 1;
                    } else {
                        *((short*)memchunk) = (size << 2) + 3;
                        return memchunk + 2;
                    }
                } else {
                    // Unfortunately, we don't have enough space, lets go to the next available memory chunk
                    i += memSize + 1;
                    if (memSize >= 64) {
                        i += 1;
                    }
                }
            }
        }
    }
    // WE WILL GO OVER!
}

int usage(int index) {
    return (*((unsigned char*)myblock) & 1);
}

int chunksize(int index) {
    unsigned char* memchunk = &myblock[index];
    unsigned int bytesize = (*memchunk >> 1) & 1;
    if (bytesize == 0) {
        return (*memchunk >> 2);
    } else {
        return *((short*)memchunk) >> 2;
    }
}
