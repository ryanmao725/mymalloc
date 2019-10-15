/**
 * mymalloc.h
 *
 * @authors Shivan Modha, Ryan Mao
 * @description C dynamic memory implementation simulation
 * @date 10/15/2019
 * @version 1.0.0
 * Copyright 2019 Shivan/Ryan
 */
// Include our guard definition to prevent multiple instantiations
#ifndef MYMALLOC_H_
#define MYMALLOC_H_
// Include the necessary libraries to run the library
#include <stdlib.h>
#include <stdio.h>
// Redefine malloc and free to use our simulated version
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
// Auxilary variables
#define MEM_SIZE 4096
#define DEBUG 1
// Function definition headers
void* mymalloc(size_t, char*, int);
void myfree(void*, char*, int);
// Helper function definition headers
unsigned int chunksize(unsigned char* memchunk);
// End the guard definition
#endif  // MYMALLOC_H_
