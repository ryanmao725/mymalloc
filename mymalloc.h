#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_
#define malloc(x) mymalloc(x, __LINE__, __FILE__)
#define free(x) myfree(x, __LINE__, __FILE__)
#endif
