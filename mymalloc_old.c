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
/**
 * Malloc override method, that will be used instead of the default call to the malloc command
 */
void* mymalloc(size_t size, char* file, int line) {
    // Calculate the size that we need to store this memory
    size_t allocSize = 1;
    if (size >= 64) {
        allocSize += 1;
    }
    allocSize += size;
    // Initialize our variables for the loop
    unsigned int i = 0;
    while (i < MEM_SIZE) {
        // Analyze this memory chunk that we are at
        unsigned char* memchunk = &myblock[i];
        int inUse = (*memchunk & 1);
        int memSize = chunksize(memchunk);
        unsigned int bytesize = (*memchunk >> 1) & 1;
        if (inUse == 1) { // This memory is in use, skip over it by incrementing
            i += memSize + 1; // Increment by one regardless
            if (memSize >= 64) { // If we have a two byte meta data, increment once more
                i += 1;
            }
        } else { // This memory is not in use, analyze further
            // Here we check two things to accomodate to scenarios
            //  1. If we are at a free space between memory chunks, we ensure that we have enough space, or
            //  2. If we are at the end of the memory allocated (i.e. there are no more allocated memory chunks after this point), then ensure that we have enough remaining space to store our data
            if (memSize + 1 + bytesize >= allocSize || (memSize == 0 && MEM_SIZE >= allocSize + i)) {
                // If we are in between memory chunks, we need to augment the memory in two ways
                //  1. Give the user a pointer to the memory chunk
                //  2. Take SPACE - ALLOCATED and set that to FREE (in the case that there is too much available space)
                int newSize = -1;
                if (memSize != 0 && memSize + 1 + bytesize - allocSize > 0) {
                    // Get a pointer to the available space after the allocation
                    unsigned char* ptr = memchunk + allocSize;
                    newSize = memSize + bytesize - allocSize;
                    // Set the bits to [0, 0|1 (depending on meta byte size), ...size]
                    if (newSize < 64) {
                        *(ptr) = (newSize << 2);
                    } else {
                        *((short*)ptr) = (size << 2) + 2 + 256;
                    }
                }
                // If we have only 1 in our new size free, lets just give this to the user (we can't do anything with the 1 byte since it takes at least 1 byte for meta data)
                if (newSize == 0) {
                    newSize = 1;
                } else {
                    newSize = 0;
                }
                size += newSize;
                // Create the pointer that we will be sending to the user. Default it to the space after the meta data (we assume that the meta data size is 1 byte initially)
                unsigned char* returnPtr = memchunk + 1;
                // Actually calculate the return pointer based on the bytes, and set the meta data correctly
                //  - 1 Byte META: [1, 0, ...size]
                //  - 2 Byte META: [1, 1, ...size][0, ...size]
                if (size < 64) {
                    *(memchunk) = (size << 2) + 1;
                    returnPtr = memchunk + 1;
                } else {
                    *((short*)memchunk) = (size << 2) + 3 + 256;
                    returnPtr = memchunk + 2;
                }
                // Return the pointer to the user
                return returnPtr;
            } else {
                // Unfortunately, we don't have enough space in our current memory chunk, lets go to the next available memory chunk
                i += memSize + 1;
                if (memSize >= 64) {
                    i += 1;
                }
                // Check to see if we can still fit ANYTHING in our memory, and if not we will simply just skip to the end and terminate
                if (MEM_SIZE < allocSize) {
                    i = MEM_SIZE;
                }
            }
        }
    }
    // If we are here, we didn't find any memory slot big enough to store the information
    printf("OutOfMemory Error: \n    Attempted to allocate more memory than available in line %d, %s\n", line, file);
    return NULL; // If we can't allocate anything, simply return a NULL
}
/**
 * Free override method, that will be used instead of the default call to the free command
 */
void myfree(void* ptr, char* file, int line) {
    if (ptr == NULL) {
        printf("NullPointer Error: \n    Cannot deallocate null in line %d, %s\n", line, file);
        return;
    }
    // Loop through the memory to merge chunks that aren't in use
    int i = 0;
    int deleted = 0;
    // Create a pointer to the previous pointer location
    unsigned char* lastchunk = NULL;
    while (i < MEM_SIZE) {
        unsigned char* memchunk = &myblock[i];
        int inUse = (*memchunk & 1);
        int memSize = chunksize(memchunk);
        //printf("Memsize %d\n", memSize);
        unsigned int bytesize = (*memchunk >> 1) & 1;
        if (inUse == 0) {
            if (lastchunk == NULL) {
                lastchunk = memchunk;
            } else {
                unsigned int last_bytesize = (*lastchunk >> 1) & 1;
                int newMemSize = chunksize(lastchunk) + memSize + 1 + bytesize + last_bytesize;
                if (newMemSize < 64) {
                    *(lastchunk) = (newMemSize << 2);
                } else {
                    *((short*)lastchunk) = (newMemSize << 2) + 2 + 256;
                }
            }
            if (memchunk + 1 + bytesize == ptr && deleted != 1) {
                deleted = 1;
                printf("RedundantFree Error:\n    Cannot deallocate memory that was already deallocate in line %d, %s\n", line, file);
            }
        } else {
            // Verify that this isn't the memory pointer we were given
            if (memchunk + 1 + bytesize == ptr) {
                if (memSize < 64) {
                    *(memchunk) = (memSize << 2);
                } else {
                    *((short*)memchunk) = (memSize << 2) + 2 + 256;
                }
                deleted = 1;
                continue;
            }
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
    if (deleted != 1) {
        printf("Deallocation Error: \n    Attempted to deallocate an invalid pointer in line %d, %s\n", line, file);
    }
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
unsigned int chunksize(unsigned char* memchunk) {
    // Figure out the meta data byte size
    unsigned int bytesize = (*memchunk >> 1) & 1;
    if (bytesize == 0) {
        // If the meta data is 1 byte long, we can simply bit shift right twice
        return (*memchunk >> 2);
    } else {
        // If the meta data is 2 bytes long, we need to:
        //  - Go into the next byte (hence the unsigned short castig)
        //  - Subtract 256 from the new number (which will remove the middle 0 value)
        //  - Bit shift right twice once more, to remove the byte information
        return (*((unsigned short*)memchunk) - 256) >> 2;
    }
}
