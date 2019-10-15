#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

#include <stdlib.h>
#include <stdio.h>

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
#define MEM_SIZE 4096

void* mymalloc(size_t, char*, int);
void myfree(void*, char*, int);

unsigned int chunksize(unsigned char* memchunk);

#endif
