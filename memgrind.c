/**
 * memgrind.c
 *
 * @authors Shivan Modha, Ryan Mao
 * @description Stress test of custom memory simulation
 * @date 10/15/2019
 * version 1.0.0
 * Copyright 2019 Shivan/Ryan
 */
//Include the necessary headers to run the script
#include <stdlib.h>
#include <stdio.h>
#include<time.h> //Need to include time.h to record times for the workloads
#include "mymalloc.h" //Include our header file
    
/**
 * Workload A
 *  Will malloc() 1 byte, and then immediately free() it.
 *
 * parameters:
 *  The number of runs of the workload
 *
 * returns:
 *  The total time took to run the workload at the number of runs in microseconds
 */
long workload_A(int number_of_runs) {
    struct timeval tv_start, tv_end; //Create a timeval start and timeval end struct
    int i = 0;
    gettimeofday(&tv_start, NULL); //Get the current time and store it in the timeval start struct
    while (i < number_of_runs) { //Loop through the number of runs.
        char* one_byte = malloc(1); //Create a pointer and malloc a byte.
        free(one_byte); //Free the memory at the pointer.
        i++;
    }
    gettimeofday(&tv_end, NULL); //Get the current time and store it in the timeval end struct.
    return (tv_end.tv_usec - tv_start.tv_usec); //Return the time in microseconds
}

/**
 * Workload B
 *  Will malloc() 1 byte, and store it in an array.
 *  Repeat this 50 times, and then free all the previously malloc'ed bytes.
 *
 * parameters:
 *  The number of runs of the workload
 *
 * returns:
 *  The total time took to run the workload at the number of runs in microseconds
 */
long workload_B(int number_of_runs) {
    struct timeval tv_start, tv_end; //Create a timeval start and timeval end struct.
    char* storage[50]; //Initialize the storage array to store the pointers.
    int i = 0;
    gettimeofday(&tv_start, NULL); //Get the current time and store it in the timeval start struct.
    while (i < number_of_runs) {
        int j;
        for (j = 0; j < 50; j++) { //Malloc 50 1 bytes.
            storage[j] = malloc(1);
        }
        for (j = 0; j < 50; j++) { //Free all 50 1 byte pointers.
            free(storage[j]);
        }
        i++;
    }
    gettimeofday(&tv_end, NULL); //Get the current time and store it in the timeval end struct.
    return (tv_end.tv_usec - tv_start.tv_usec); //Return the time in microseconds
}

/**
 * Workload C
 *  Will randomly choose between malloc() 1 byte or free().
 *  We will store the malloc() bytes in a storage.
 *  Whenever the free() method is called, it will automatically free() the last one in the array to stop fragmentation.
 *  This will automatically free everything once the storage array is full (50 stored mallocs)
 *  
 * parameters:
 *  The number of runs of the workload
 *
 * returns:
 *  The total time took to run the workload at the number of runs in microseconds
 */
long workload_C(int number_of_runs) {
    struct timeval tv_start, tv_end; //Create a timeval start and timeval end struct.
    char* storage[50]; //Initialize the storage array to store the pointers.
    int number_of_allocated = 0; //Keep track of the number_of_allocated so we can loop until we fill the storage.
    gettimeofday(&tv_start, NULL); //Get the current time and store it in the timeval
    while (number_of_allocated != 50) {
        int random_num = (rand() % 2); //Generate a random number between 0 and 1.
        if (random_num == 0) { //If that number is 0, malloc(1)
            storage[number_of_allocated] = malloc(1);
            number_of_allocated++; //Increment the number_of_allocated
        } else { //Else free the most recent malloc(1);
            if (number_of_allocated > 0) { //If we can actually free anything.
                free(storage[number_of_allocated - 1]); //Free the last one stored.
                number_of_allocated--;//Decrement the number_of_allocated
            }
        }
    }
    //FREE everything when it gets to 50.
    int i = 0;
    for (i; i < 50; i++) {
        free(storage[i]);
    }
    gettimeofday(&tv_end, NULL);//Get the current time and store it in the timeval end.
    return (tv_end.tv_usec - tv_start.tv_usec); //Return the time in microseconds
}

/**
 * Workload D
 *  Similar to workload C, however, the size of each memory chunk allocated is also randomized (between 1 and 65).
 * 
 * parameters:
 *  The number of runs of the workload
 *
 * returns:
 *  The total time to run the workload at the number of runs in microseconds
 */
long workload_D(int number_of_runs) {
    struct timeval tv_start, tv_end; //Create a timeval start and end struct.
    char* storage[50]; //Create a storage array
    int number_of_allocated = 0; //Keep track for the number_of_allocated in the array.
    int number_of_times_malloced = 0; //Keep track of the number of times allocated.
    gettimeofday(&tv_start, NULL); //Get the current time of day and store it in the timeval start struct.
    while (number_of_times_malloced != 50) { //Loop until the number of times alloacted is 50.
        int random_num = (rand() % 2); //Generate a random number between 0 and 1.
        if (random_num == 0) { //If that number is 0, malloc();
            int random_size = (rand() % 64) + 1; //Generate between 1 and 64.
            storage[number_of_allocated] =  malloc(random_size);
            number_of_times_malloced++; //Increment the number of times malloced
            number_of_allocated++; //Increment number of allocated in array
        } else { //Else free();
            if (number_of_allocated > 0) {
                free(storage[number_of_allocated - 1]); //Free the last element stored in the array.
                number_of_allocated--; //Decrement the number of allocated in array
            }
        }
    }
    // Free everything in the storage array
    int i = 0;
    for (i; i < number_of_allocated; i++) {
        free(storage[i]);
    }
    gettimeofday(&tv_end, NULL); //Get the current time of day and store it in the timeval end struct
    return (tv_end.tv_usec - tv_start.tv_usec); //Return the time in microseconds
}

/**
 * Workload E
 *  This will malloc 51 1 byte pointers.
 *  It will then begin to free them from the middle towards the ends
 *  This workload is mainly to compare between workload B and how efficient our merging of unused memory is.
 * 
 * parameters:
 *  The number of runs of the workload
 *
 * returns:
 *  The time in microseconds to run the workload
 */
long workload_E(int number_of_runs) {
    struct timeval tv_start, tv_end; //Create timeval start and end structs
    int i = 0;
    char* storage[51]; //Initialize the storage array
    gettimeofday(&tv_start, NULL); //Get the time of day and store it in the start timeval struct
    while (i < 1) {
        //Create the 51 1 byte pointers.
        int j;
        for (j = 0; j < 51; j++) {
            storage[j] = malloc(1);
        }
        //Free those pointers from the inside out.
        for (j = 0; j <= 25; j++) {
            free(storage[25 + j]);
            if (j != 0) { //If j is 0, then this will get freed twice, so we have this condition here.
                free(storage[25 - j]);
            }
        }
        i++;
    }
    gettimeofday(&tv_end, NULL); //Get the time of day and store it in the end timeval struct.
    return (tv_end.tv_usec - tv_start.tv_usec); //Return the time in microseconds
}

/**
 * Workload F
 *  This will malloc 50 1 byte pointers.
 *  It will then free them from backwards.
 *  This workload is mainly to compare between workload A and how efficient our free method and merge is.
 *
 * parameters:
 *  The number of runs the workload would run.
 *
 * returns:
 *  The time in microseconds for the workload to run at the number of runs.
 */
long workload_F(int number_of_runs) {
    struct timeval tv_start, tv_end; //Create timeval start and end structs.
    int i = 0;
    char* storage[50]; //Create a storage array
    gettimeofday(&tv_start, NULL); //Get the current time of day and store it in the start timeval struct.
    while (i < number_of_runs) {
        //Create storage looping forwards
        int j;
        for (j = 0; j < 50; j++) {
            storage[j] = malloc(1);
        }
        //Remove storage looping backwards
        for (j = 49; j >= 0; j--) {
            free(storage[j]);
        }
        i++;
    }
    gettimeofday(&tv_end, NULL); //Get the current time and store it in the timeval end struct.
    return (tv_end.tv_usec - tv_start.tv_usec); //Return the time in microseconds
}


/**
 * Function that calculates the average time for the given array of workloads
 *
 * parameters:
 *  The array of workload times
 *  The number of loops each workload was run.
 *
 * returns:
 *  The average time of all the workloads
 */
long average_time(long* workload, int workload_iteration_count) {
    long total_workload = 0;
    int i = 0;
    for (i; i < workload_iteration_count; i++) { //Loop through all workload times and all them all together
        total_workload += workload[i];
    }
    return (total_workload / workload_iteration_count); //Return the total workload times divided by the total number all workloads were run.
}

//Main
int main(int argc, char** argv) {
    int workload_iteration_count = 100; //Total number of workloads
    int workload_count = 0; //Iterator count to run through all the workloads
    //Initialize all workload time arrays
    long workload_A_times[workload_iteration_count];
    long workload_B_times[workload_iteration_count];
    long workload_C_times[workload_iteration_count];
    long workload_D_times[workload_iteration_count];
    long workload_E_times[workload_iteration_count];
    long workload_F_times[workload_iteration_count];
    while (workload_count < workload_iteration_count) { //Loop through and run all workloads at their respective amounts.
        workload_A_times[workload_count] = workload_A(150);
        workload_B_times[workload_count] = workload_B(3);
        workload_C_times[workload_count] = workload_C(1);
        workload_D_times[workload_count] = workload_D(1);
        workload_E_times[workload_count] = workload_E(3);
        workload_F_times[workload_count] = workload_F(150);
        workload_count++;
    }
    //Print all average times of workloads in microseconds.
    printf("Workload A time in microseconds: %ld\n", average_time(workload_A_times, workload_iteration_count));
    printf("Workload B time in microseconds: %ld\n", average_time(workload_B_times, workload_iteration_count));
    printf("Workload C time in microseconds: %ld\n", average_time(workload_C_times, workload_iteration_count));
    printf("Workload D time in microseconds: %ld\n", average_time(workload_D_times, workload_iteration_count));
    printf("Workload E time in microseconds: %ld\n", average_time(workload_E_times, workload_iteration_count));
    printf("Workload F time in microseconds: %ld\n", average_time(workload_F_times, workload_iteration_count));
    return 0;
}
