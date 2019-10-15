// Actual implementation of the function prototypes declared in the header file "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

static char myblock[MEM_SIZE];

void printMem() {
    int i = 0;
    while (i < MEM_SIZE) {
        unsigned char* memchunk = &myblock[i];
        int inUse = (*memchunk & 1);
        int memSize = chunksize(memchunk);
        unsigned int bytesize = (*memchunk >> 1) & 1;
        printf("ITERATING at byte %d. USE=%d, SIZE=%d, ALLOCSIZE=%d\n", i, inUse, memSize, memSize + 1 + bytesize);
        i += memSize + 1;
        if (memSize >= 64) {
            i += 1;
        }
        if (memSize == 0) {
            break;
        }
    }
}



void* mymalloc(size_t size, char* file, int line) {
    printf("\n\n");
    printMem();
    // Calculate the size that we need to store this memory
    size_t allocSize = 1;
    if (size >= 64) {
        allocSize += 1;
    }
    allocSize += size;
    // Initialize our variables
    unsigned int i = 0;
    while (i < MEM_SIZE) {
        // Analyze this memory chunk
        unsigned char* memchunk = &myblock[i];
        int inUse = (*memchunk & 1);
        int memSize = chunksize(memchunk);
        unsigned int bytesize = (*memchunk >> 1) & 1;
        if (inUse == 1) { // This memory is in use, skip over it
            i += memSize + 1;
            if (memSize >= 64) {
                i += 1;
            }
        } else { // This memory is not in use, analyze further
            if (memSize + 1 + bytesize >= allocSize || (memSize == 0 && MEM_SIZE >= allocSize + i)) {
                // ALLOCATE THIS MEM CHUNK
                int newSize = -1;
                if (memSize != 0 && memSize + 1 + bytesize - allocSize > 0) {
                    unsigned char* ptr = memchunk + allocSize;
                    newSize = memSize + bytesize - allocSize;
                    printf("%d", newSize);
                    if (newSize < 64) {
                        *(ptr) = (newSize << 2);
                    } else {
                        *((short*)ptr) = (size << 2) + 2 + 256;
                    }
                }
                if (newSize == 0) {
                    newSize = 1;
                } else {
                    newSize = 0;
                }
                size += newSize;
                unsigned char* returnPtr = memchunk + 1;
                if (size < 64) {
                    printf("ALLOCATING at byte %d, with memsize %zu, with value %zu\n", i, size, (size << 2) + 1);
                    *(memchunk) = (size << 2) + 1;
                    returnPtr = memchunk + 1;
                } else {
                    printf("ALLOCATING at byte %d, with memsize %zu, with value %zu\n", i, size, (size << 2) + 3 + 256);
                    *((short*)memchunk) = (size << 2) + 3 + 256;
                    returnPtr = memchunk + 2;
                }
                return returnPtr;
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
    printf("\n\n");
    printMem();
    // Find the start of the meta data
    unsigned char* memchunk = ptr - 1;
    if ((*memchunk & 1) == 0) {
        memchunk = ptr - 2;
    }
    // Reset the value
    unsigned int memSize = chunksize(memchunk);
    if (memSize < 64) {
        *(memchunk) = (memSize << 2);
    } else {
        *((short*)memchunk) = (memSize << 2) + 2 + 256;
    }
    // Loop through the memory to merge chunks that aren't in use
    int i = 0;
    // Create a pointer to the previous pointer location
    unsigned char* lastchunk = NULL;
    while (i < MEM_SIZE) {
        unsigned char* memchunk = &myblock[i];
        int inUse = (*memchunk & 1);
        int memSize = chunksize(memchunk);
        unsigned int bytesize = (*memchunk >> 1) & 1;
        if (inUse == 0) {
            if (lastchunk == NULL) {
                lastchunk = memchunk;
            } else {
                int newMemSize = chunksize(lastchunk) + memSize + 1 + bytesize;
                if (newMemSize < 64) {
                    *(lastchunk) = (newMemSize << 2);
                } else {
                    *((short*)lastchunk) = (newMemSize << 2) + 2 + 256;
                }
            }
        } else {
            lastchunk = NULL;
        }
        // iterate through the memory
        i += memSize + 1;
        if (memSize >= 64) {
            i += 1;
        }
        if (memSize == 0) {
            break;
        }
    }
    if (lastchunk != NULL) {
        *(lastchunk) = 0;
    }
    printf("\n");
    printMem();
}

unsigned int chunksize(unsigned char* memchunk) {
    unsigned int bytesize = (*memchunk >> 1) & 1;
    if (bytesize == 0) {
        return (*memchunk >> 2);
    } else {
        return (*((unsigned short*)memchunk) - 256) >> 2;
    }
}
