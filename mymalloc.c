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
 * Debugging function that will allow the developer to see what is actually in memory
 *
 * parameters:
 *  nothing
 *
 * returns:
 *  nothing
 */
void printMem() {
    // Create a looper variable for us to traverse with
    unsigned int i = 0;
    // Traverse through the memory
    while (i < MEM_SIZE) {
        // Get a pointer to the current memory chunk that we are at
        unsigned char* memchunk = (unsigned char*)&myblock[i];
        // Print out all information that we have about this memory chunk
        printf("INDEX: %d, ADDRESS: %d\n    USE: %d, BYTESIZE: %d, SIZE: %d\n", i, memchunk, inUse(memchunk), byteWidth(memchunk) + 1, chunkSize(memchunk));
        // Increment or stop based on end conditions
        if (chunkSize(memchunk) > 0 || inUse(memchunk)) {
            i += chunkSize(memchunk);
        } else {
            i = MEM_SIZE;
        }
    }
    // Add extra line spacings so that things can look prettier
    printf("\n\n");
}
/**
 * Malloc override method, that will be used instead of the default call to the malloc command
 *
 * parameters
 *  size_t size: requested size of the memory chunk
 *  char* file: File name that invoked this method
 *  int line: Current line number of the application
 * returns
 *  pointer to the allocated memory chunk, or NULL if no memory chunks could be allocated correctly
 */
void* mymalloc(size_t size, char* file, int line) {
    // Calculate the allocated memory size that needs to be stored
    unsigned int allocatedSize = size + 1;
    if (allocatedSize >= 64) {
        // We have a two byte meta data instance
        allocatedSize++;
    }
    // Initialize looper variables
    unsigned int i = 0;
    // Traverse
    while (i < MEM_SIZE) {
        // Get a pointer to the current memory chunk that we will investigate
        unsigned char* memchunk = (unsigned char*)&myblock[i];
        // Verify whether or not the memory is in use. We will only analyze memory that is not in use
        if (inUse(memchunk) == 0) {
            // If we are at a point that is not allocated, we could be in two cases
            //  1. We are at the end, meaning after this point, there is NO allocated memory left
            //  2. We are in between chunks of used memory
            // Therefore, we verify based on the size.
            unsigned short cSize = chunkSize(memchunk);
            if (cSize == 0) { // We are at the end
                if (i + allocatedSize <= MEM_SIZE) { // Verify that we have enough memory to add
                    setChunk(memchunk, 1, allocatedSize); // Set the current memory address
                    setChunk(memchunk + allocatedSize, 0, 0); // Preemptively set the block after this memory to 0 (since there shouldn't be anything allocated after it.)
                    return memchunk + byteWidth(memchunk) + 1; // Return the memory chunk
                } else { // Since we don't have enough memory, break out of the loop
                    break;
                }
            } else { // We are in between memory chunks
                if (allocatedSize <= cSize && allocatedSize > 0) { // Verify that we can fit within this open block
                    // In setting the current memory address, we have to be wary that we don't potentially need to take all of the memory. We can potentially have space for 10 bytes, but use up 3 bytes.
                    setChunk(memchunk, 1, allocatedSize); // Set the current memory address
                    if (cSize - allocatedSize > 0) { // If we have leftover space in the memory, we need to set that to not being used.
                        setChunk(memchunk + allocatedSize, 0, cSize - allocatedSize);
                    }
                    return memchunk + byteWidth(memchunk) + 1; // Return the memory chunk
                }
            }
        }
        // If we haven't returned yet, we are iterating!
        i += chunkSize(memchunk);
    }
    // If this line ever executes, we know that we broke from the loop (or traversed all of it already)
    printf("OutOfMemory Error: \n    Attempted to allocate more memory than available in line %d, %s\n", line, file);
    return NULL;
}
/**
 * Free override method, that will be used instead of the default call to the free command
 *
 * parameters
 *  void* ptr: Pointer to the address to be freed
 *  char* file: File name that invoked this method
 *  int line: Current line number of the application
 *
 * returns
 *  Nothing.
 */
void myfree(void* ptr, char* file, int line) {
    // Verify that the pointer is not NULL. If it is, simply return
    if (ptr == NULL) {
        return;
    }
    // Instantiate looper variables and flags
    unsigned short removed = 0;
    unsigned int i = 0;
    unsigned char* previousFree = NULL; // Keep a record of the last memory chunk, if it is free. Otherwise, it will be NULL
    // Traverse. This loop will handle two things:
    //  1. Physically removing the value from the memory
    //  2. Merging consecutive free chunks of memory together
    while (i < MEM_SIZE) {
        // Get a pointer to the current memory chunk that we will investigate
        unsigned char* memchunk = (unsigned char*)&myblock[i];
        // Verify whether or not the memory chunk is in use
        if (inUse(memchunk) == 0) {
            // Get a handle to the size of the memory chunk, so that we can check if we want to merge it
            unsigned short cSize = chunkSize(memchunk);
            if (cSize == 0) {
                if (previousFree != NULL) {
                    setChunk(previousFree, 0, 0); // Since these conditions mean that we are at the end of our allocated memory spaces, we can simply blanketly set these to blank
                }
                break;
            } else {
                // Verify that the pointer doesn't refer here. If it does, we know that the location has already been deallocated; therefore, we know that someone attempted to refree a freed up memory slot
                if (memchunk + 1 + byteWidth(memchunk) == ptr && removed == 0) {
                    printf("RedundantFree Error: \n    Cannot deallocate memory that was already deallocated in line %d, %s\n", line, file);
                    removed = 1; // Stop the second error message from printing
                    break;
                }
                // If we have the previousFree variable populated, we know that we can merge
                if (previousFree != NULL) {
                    // Calculate the new size of the memory chunk that we are merging. It is the total size of the previous memory chunk added with the total size of the current memory chunk.
                    int newSize = chunkSize(previousFree) + chunkSize(memchunk);
                    setChunk(previousFree, 0, newSize);
                }
                // Traverse through the loop
                i += chunkSize(memchunk);
            }
            // If we haven't already had a previousFree memory chunk, then we know that this is the first one (in a potentially consecutive arrangement), therefore we can set it
            if (previousFree == NULL) {
                previousFree = memchunk;
            }
        } else { // The memory is in use!
            // Verify that we are talking about the same pointers
            if (memchunk + 1 + byteWidth(memchunk) == ptr) {
                if (actualSize(chunkSize(memchunk)) != 0) {
                    // Since we are pointing to the same memory address as the requestd, we can remove this memory chunk
                    removeChunk(memchunk);
                }
                // Set the flag
                removed = 1;
            } else  {
                // Since we are at a non-free block, reset the previousFree pointer
                previousFree = NULL;
                // Iterate
                i += chunkSize(memchunk);
            }
        }
    }
    // If we haven't removed anything, we know that we have an error
    if (removed == 0) {
        printf("Deallocation Error: \n    Attempted to deallocate an invalid pointer in line %d, %s\n", line, file);
    }
}
/**
 * Helper function to extract the inUse flag of a memory chunk
 *
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
 *
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
 *
 * parameters
 *  unsigned short size: Given chunksize
 *
 * returns
 *  integer representation of the actual memory size (chunk size minus the size of the meta data)
 */
unsigned short actualSize(unsigned short size) {
    if (size < 64) {
        return size - 1;
    } else {
        return size - 2;
    }
}
/**
 * Given a memory size, set the cooresponding meta data for the memory chunk
 *
 * parameters
 *  unsigned char* memchunk: Pointer to the memory chunk that we are setting
 *  unsigned short inuse: Bit flag specifying whether or not the memory is in use
 *  unsigned short size: Variable representing the size of the memory block
 *
 * returns
 *  Nothing.
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
 *
 * parameters:
 *  unsigned char* memchunk: Pointer to the memory chunk that we are setting
 * 
 * returns:
 *  Nothing.
 */
void removeChunk(unsigned char* memchunk) {
    setChunk(memchunk, 0, chunkSize(memchunk)); // Forward call setChunk with the input parameters
}
