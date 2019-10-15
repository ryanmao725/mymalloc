#include <stdlib.h>
#include <stdio.h>
#include<time.h> //Need to include time.h to record times for the workloads
#include "mymalloc.h"

int main(int argc, char** argv) {
    char* ptr1 = malloc(sizeof(char) * 3);
    char* ptr2 = malloc(sizeof(char) * 3);
    char* ptr3 = malloc(sizeof(char) * 3);
    free(ptr2);
    char* ptr4 = malloc(sizeof(char) * 1);
    char* ptr5 = malloc(sizeof(char) * 3);
    free(ptr3);
    free(ptr4);
    free(ptr5);
    free(ptr1);
    printf("\n\n\n");
    ptr1 = malloc(sizeof(char) * 3);
    ptr2 = malloc(sizeof(char) * 3);
    ptr3 = malloc(sizeof(char) * 3);
    free(ptr2);
    ptr4 = malloc(sizeof(char) * 1);
    ptr5 = malloc(sizeof(char) * 3);
    free(ptr3);
    free(ptr4);
    free(ptr5);
    free(ptr1);
    
    struct timeval tv;

    //A
    //This will malloc() one byte, and then immediately free() it.
    //Number of Runs: 150
    int i = 0;
    gettimeofday(&tv, NULL);
    time_t start_time = tv.tv_usec;
    while (i < 150) {
        char* one_byte = malloc(1);
        free(one_byte);
        i++;
    }
    gettimeofday(&tv, NULL);
    time_t end_time = tv.tv_usec;

    printf("The time to execute A was: %ld\n", end_time - start_time);


    return 0;
}
