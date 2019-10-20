/**
 * mymalloc.c
 *
 * @authors Shivan Modha, Ryan Mao
 * @description C dynamic memory implementation simulation
 * @date 10/15/2019
 * @version 1.0.0
 * Copyright 2019 Shivan/Ryan
 */
// Include the necessary headers to run the script
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h" // Reference our defined header file
// Define our memory block location
static char myblock[MEM_SIZE];
void printMem() {
    unsigned int i = 0;
    while (i < MEM_SIZE) {
        unsigned char* memchunk = (unsigned char*)&myblock[i];
        printf("INDEX: %d, ADDRESS: %d\n    USE: %d, BYTESIZE: %d, SIZE: %d\n", i, memchunk, inUse(memchunk), byteWidth(memchunk) + 1, chunkSize(memchunk));
        if (chunkSize(memchunk) > 0) {
            i += actualSize(chunkSize(memchunk));
        } else {
            i = MEM_SIZE;
        }
    }
    printf("\n\n");
}
/**
 * Malloc override method, that will be used instead of the default call to the malloc command
 */
void* mymalloc(size_t size, char* file, int line) {
    unsigned short allocationSize = 0;
    if (size >= 64) {
        allocationSize = 1;
    }
    unsigned int i = 0;
    while (i < MEM_SIZE) {
        unsigned char* memchunk = (unsigned char*)&myblock[i];
        if (inUse(memchunk) == 0) {
            unsigned short cSize = chunkSize(memchunk);
            if (cSize == 0) {
                if (i + actualSize(size) <= MEM_SIZE) {
                    setChunk(memchunk, 1, size);
                    //printf("USE: %d, BYTESIZE: %d, SIZE: %d\n", inUse(memchunk), byteWidth(memchunk) + 1, chunkSize(memchunk));
                    return memchunk + byteWidth(memchunk) + 1;
                } else {
                    break;
                }
            } else {
                if (actualSize(size) <= actualSize(cSize)) {
                    setChunk(memchunk, 1, size);
                    //printf("USE: %d, BYTESIZE: %d, SIZE: %d\n", inUse(memchunk), byteWidth(memchunk) + 1, chunkSize(memchunk));
                    return memchunk + byteWidth(memchunk) + 1;
                }
            }
        }
        i += actualSize(chunkSize(memchunk));
    }
    printf("OutOfMemory Error: \n    Attempted to allocate more memory than available in line %d, %s\n", line, file);
    return NULL;
}
/**
 * Free override method, that will be used instead of the default call to the free command
 */
void myfree(void* ptr, char* file, int line) {
    unsigned short removed = 0;
    unsigned int i = 0;
    unsigned char* previousFree = NULL;
    while (i < MEM_SIZE) {
        unsigned char* memchunk = (unsigned char*)&myblock[i];
        if (inUse(memchunk) == 0) {
            unsigned short cSize = chunkSize(memchunk);
            if (cSize == 0) {
                if (previousFree != NULL) {
                    setChunk(previousFree, 0, 0);
                }
                break;
            } else {
                if (memchunk + 1 + byteWidth(memchunk) == ptr && removed == 0) {
                    printf("RedundantFree Error: \n    Cannot deallocated memory that was already deallocated in line %d, %s\n", line, file);
                    removed = 1;
                    break;
                }
                if (previousFree != NULL) {
                    int newActualSize = actualSize(chunkSize(previousFree)) + actualSize(chunkSize(memchunk));
                    int newSize = 0;
                    if (newActualSize <= 64) {
                        newSize = newActualSize - 1;
                    } else {
                        newSize = newActualSize - 2;
                    }
                    setChunk(previousFree, 0, newSize);
                }
                i += actualSize(chunkSize(memchunk));
            }
            if (previousFree == NULL) {
                previousFree = memchunk;
            }
        } else {
            if (memchunk + 1 + byteWidth(memchunk) == ptr) {
                removeChunk(memchunk);
                removed = 1;
            } else  {
                previousFree = NULL;
                i += actualSize(chunkSize(memchunk));
            }
        }
    }
    if (removed == 0) {
        printf("Deallocation Error: \n    Attempted to deallocate an invalid pointer in line %d, %s\n", line, file);
    }
}
/**
 * Helper function to extract the inUse flag of a memory chunk
 * parameters
 *  unsigned char* memchunk: Pointer to the beginning of the memory chunk
 *
 * returns
 *  integer representing a boolean value of whether or not the chunk is in use
 */
unsigned short inUse(unsigned char* memchunk) {
    return *memchunk & 1;
}
/**
 * Helper function byte width
 * parameters
 *  unsigned char* memchunk: Pointer to the beginning of the memory chunk
 *
 * returns
 *  integer representing the allocation byte size of the meta data
 */
unsigned short byteWidth(unsigned char* memchunk) {
    return (*memchunk >> 1) & 1;
}
/**
 * Helper function to extract the chunk size given a pointer to the start of a memory slot
 * 
 * parameters
 *  unsigned char* memchunk: Pointer to the beginning of the memory chunk
 * 
 * returns
 *  integer representing the byte size of the corresponding memory chunk
 */
unsigned short chunkSize(unsigned char* memchunk) {
    // Figure out the meta data byte size
    unsigned short bytesize = byteWidth(memchunk);
    if (bytesize == 0) {
        // If the meta data is 1 byte long, we can simply bit shift right twice
        return (*memchunk >> 2);
    } else {
        // If the meta data is 2 bytes long, we need to:
        //  - Go into the next byte (hence the unsigned short castig)
        //  - Subtract 256 from the new number (which will remove the middle 0 value)
        //  - Bit shift right twice once more, to remove the byte information
        return (*(unsigned short*)memchunk) >> 2;
    }
}
/**
 * Given a memory size, find the actual number of bytes we need to store the data as well as the meta data
 */
unsigned short actualSize(unsigned short size) {
    if (size < 64) {
        return size + 1;
    } else {
        return size + 2;
    }
}
/**
 * Given a memory size, set the cooresponding meta data for the memory chunk
 */
void setChunk(unsigned char* memchunk, unsigned short inuse, unsigned short size) {
    if (size < 64) {
        *(memchunk) = // Set the value of the 1 byte meta data
            (size << 2) // Shift over the size information by 2 bits, since we need this for the inuse/bytewidth flags
            + inuse; // Set the inuse flag
    } else {
        *((short*)memchunk) = // Set the value of the 2 byte meta data
            (size << 2) // Shift over the size information by 2 bits, since we need this for the inuse/bytewidth flags
            + inuse // Set the inuse flag
            + 2; // Set the bytewidth flag
    }
}
/**
 * Given a memory location, mark it not in use
 */
void removeChunk(unsigned char* memchunk) {
    setChunk(memchunk, 0, chunkSize(memchunk));
}
