#include <stdlib.h>
#include <stdio.h>
#include<time.h> //Need to include time.h to record times for the workloads
//#include "mymalloc.h"
    
//Workload A
//This will malloc() one byte, and then immediately free() it.
//Number of Runs: 150
long workload_A(int number_of_runs) {
    struct timeval tv_start, tv_end;
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

//Workload B
//This will malloc() 1 byte and store it in an array, 50 times.
//Then it will free all the 50 1 byte pointers one by one.
//Number of Runs: 3
long workload_B(int number_of_runs) {
    struct timeval tv_start, tv_end;
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

//Workload C
//This will randomly choose between malloc() or free() of 1 byte size.
//Once the number of pointers reaches 50, it will free all of them.
//Number of Runs: 1
long workload_C(int number_of_runs) {
    struct timeval tv_start, tv_end;
    char* storage[50];
    int number_of_allocated = 0;
    gettimeofday(&tv_start, NULL);
    while (number_of_allocated != 50) {
        int random_num = (rand() % 2); //Generate a random number between 0 and 1.
        if (random_num == 0) { //If that number is 0, malloc(1)
            storage[number_of_allocated] = malloc(1);
            number_of_allocated++;
        } else { //Else free the most recent malloc(1);
            if (number_of_allocated > 0) { //If we can actually free anything.
                free(storage[number_of_allocated - 1]);
                number_of_allocated--;
            }
        }
    }
    //FREE everything when it gets to 50.
    int i = 0;
    for (i; i < 50; i++) {
        free(storage[i]);
    }
    gettimeofday(&tv_end, NULL);
    return (tv_end.tv_usec - tv_start.tv_usec);
}

//Workload D
//This will randomly choose between sizes in the range of 1 to 64 to malloc() or to just free();
//Number of Runs: 1
long workload_D(int number_of_runs) {
    struct timeval tv_start, tv_end;
    char* storage[50];
    int number_of_allocated = 0;
    int number_of_times_malloced = 0;
    gettimeofday(&tv_start, NULL);
    while (number_of_times_malloced != 50) {
        int random_num = (rand() % 2); //Generate a random number between 0 and 1.
        if (random_num == 0) { //If that number is 0, malloc();
            int random_size = (rand() % 64) + 1; //Generate between 1 and 64.
            //printf("Size: %d at pos: %d\n", random_size, number_of_allocated);
            storage[number_of_allocated] =  malloc(random_size);
            number_of_times_malloced++;
            number_of_allocated++;
        } else { //Else free();
            if (number_of_allocated > 0) {
                free(storage[number_of_allocated - 1]);
                number_of_allocated--;
            }
        }
    }

    int i = 0;
    for (i; i < number_of_allocated; i++) {
        free(storage[i]);
    }
    gettimeofday(&tv_end, NULL);
    return (tv_end.tv_usec - tv_start.tv_usec);
}

//Workload E
//This will malloc 51 1 byte pointers.
//It will then begin to free them from the middle towards the ends
//Number of Runs: 3
//
//This workload is mainly to compare between workload B and how efficient our merging of unused memory is.
long workload_E(int number_of_runs) {
    struct timeval tv_start, tv_end;
    int i = 0;
    char* storage[51];
    gettimeofday(&tv_start, NULL);
    while (i < number_of_runs) {
        //Create the 51 1 byte pointers.
        int j;
        for (j = 0; j < 51; j++) {
            storage[j] = malloc(1);
        }
        //Free those pointers from the inside out.
        for (j = 25; j < 25; j++) {
            free(storage[25 + j]);
            free(storage[25 - j]);
        }
        i++;
    }
    gettimeofday(&tv_end, NULL);
    return (tv_end.tv_usec - tv_start.tv_usec);
}

//Workload F
//This will malloc 50 1 byte pointers.
//It will then free them from backwards.
//Number of Runs: 150
//
//This workload is mainly to compare between workload A and how efficient our free method and merge is.
long workload_F(int number_of_runs) {
    struct timeval tv_start, tv_end;
    int i = 0;
    char* storage[50];
    gettimeofday(&tv_start, NULL);
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
    gettimeofday(&tv_end, NULL);
    return (tv_end.tv_usec - tv_start.tv_usec);
}


//Calculates the average time for the given array of workloads
long average_time(long* workload, int workload_iteration_count) {
    long total_workload = 0;
    int i = 0;
    for (i; i < workload_iteration_count; i++) {
        total_workload += workload[i];
    }
    return (total_workload / workload_iteration_count);
}

int main(int argc, char** argv) {
    int workload_iteration_count = 100;
    int workload_count = 0;
    long workload_A_times[workload_iteration_count];
    long workload_B_times[workload_iteration_count];
    long workload_C_times[workload_iteration_count];
    long workload_D_times[workload_iteration_count];
    long workload_E_times[workload_iteration_count];
    long workload_F_times[workload_iteration_count];

    while (workload_count < workload_iteration_count) {
        workload_A_times[workload_count] = workload_A(150);
        workload_B_times[workload_count] = workload_B(3);
        workload_C_times[workload_count] = workload_C(1);
        workload_D_times[workload_count] = workload_D(1);
        workload_E_times[workload_count] = workload_E(3);
        workload_F_times[workload_count] = workload_F(150);
        workload_count++;
    }
    printf("Workload A time: %ld\n", average_time(workload_A_times, workload_iteration_count));
    printf("Workload B time: %ld\n", average_time(workload_B_times, workload_iteration_count));
    printf("Workload C time: %ld\n", average_time(workload_C_times, workload_iteration_count));
    printf("Workload D time: %ld\n", average_time(workload_D_times, workload_iteration_count));
    printf("Workload E time: %ld\n", average_time(workload_E_times, workload_iteration_count));
    printf("Workload F time: %ld\n", average_time(workload_F_times, workload_iteration_count));
    return 0;
}
