#include <stdlib.h>
#include <stdio.h>
#include<time.h> //Need to include time.h to record times for the workloads
#include "mymalloc.h"

long workload_A(int number_of_runs) {
    struct timeval tv_start, tv_end;
    //A
    //This will malloc() one byte, and then immediately free() it.
    //Number of Runs: 150
    int i = 0;
    gettimeofday(&tv_start, NULL);
    while (i < number_of_runs) {
        char* one_byte = malloc(1);
        free(one_byte);
        i++;
    }
    gettimeofday(&tv_end, NULL);
    return (tv_end.tv_usec - tv_start.tv_usec);
}

long workload_B(int number_of_runs) {
    struct timeval tv_start, tv_end;
    //B
    //This will malloc() 1 byte and store it in an array, 50 times.
    //Then it will free all the 50 1 byte pointers one by one.
    //Number of Runs: 3
    char* storage[50];
    int i = 0;
    gettimeofday(&tv_start, NULL);
    while (i < number_of_runs) {
        int j;
        for (j = 0; j < 50; j++) {
            storage[j] = malloc(1);
        }
        for (j = 0; j < 50; j++) {
            free(storage[j]);
        }
        i++;
    }
    gettimeofday(&tv_end, NULL);
    return (tv_end.tv_usec - tv_start.tv_usec);
}

long workload_C(int number_of_runs) {
    struct timeval tv_start, tv_end;
    //C
    //This will randomly choose between malloc() or free() of 1 byte size.
    //Once the number of pointers reaches 50, it will free all of them.
    //Number of Runs: 1
    char* storage[50];
    int number_of_allocated = 0;
    int i = 0;
    gettimeofday(&tv_start, NULL);
    while (number_of_allocated != 50) {

    }
}

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

    int workload_iteration_count = 100;
    int workload_count = 0;
    long workload_A_times[workload_iteration_count];
    long workload_B_times[workload_iteration_count];
    long workload_C_times[workload_iteration_count];
    long workload_D_times[workload_iteration_count];
    long workload_E_times[workload_iteration_count];

    while (workload_count < workload_iteration_count) {
        workload_A_times[workload_count] = workload_A(150);
        workload_B_times[workload_count] = workload_B(3);
        //printf("Workload A time was: %ld\n", workload_A());
        workload_count++;
    }
    int i = 0;
    for (i; i < workload_iteration_count; i++) {
        printf("Workload A time: %ld\n", workload_A_times[i]);
        printf("Workload B time: %ld\n", workload_B_times[i]);
    }

    return 0;
}
