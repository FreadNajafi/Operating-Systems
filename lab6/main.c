/* 
 * File:   main.c
 * Author: a4najafi
 *
 */

/**
 *
 * COE 628 - Operating Systems
 * Lab 6
 *
 **/

#include <stdlib.h>     /* NULL */
#include  <stdio.h>	/* printf */
#include  <sys/types.h>	/* pid_t */
#include <unistd.h>	/* get_pid */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <sys/wait.h> 	/* wait */
#include <pthread.h>

#define PRODUCER_NO 5	//Number of producers, can change here
#define NUM_PRODUCED 20 //2000 //Number of items to be produced, can change here

void *generator_function(void*);
void *print_function(void*);
long sum; /* Sum of generated values*/
long finished_producers; /* number of the producer that finished producing */

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; //C: Mutex declaration and initialization
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER; //F: Condition variable declaration and initialization

int main(void) {
    int i;
    /* initialize random seed: */
    srand(time(NULL));
    sum = 0;
    finished_producers = 0;
    
    pthread_t generators[PRODUCER_NO]; // Array to hold generator threads
    pthread_t printer; // Thread for printing
    
    //A: Creates five generator thread
    for (i = 0; i < PRODUCER_NO; i++) {
        pthread_create(&generators[i], NULL, generator_function, NULL);
    }
    
    //D: Creates print thread
    pthread_create(&printer, NULL, print_function, NULL);
    
    //B: Makes sure that all generator threads have finished before proceeding
    for (i = 0; i < PRODUCER_NO; i++) {
        pthread_join(generators[i], NULL);
    }
    
    //E: Makes sure that print thread has finished before proceeding
    pthread_join(printer, NULL);
    
    return (0);
}

void * generator_function(void* junk) {
    pthread_mutex_lock(&mutex1); // lock
    
    long counter = 0;
    long sum_this_generator = 0;
   
    while (counter < NUM_PRODUCED) {
        long tmpNumber = sum;
        long rnd_number = 1; //rand() % 10;
        // can make = 1, output should be 100 (20 loops*5 threads)
        printf("current sum of the generated number up to now is %ld going to add %ld to it.\n", tmpNumber, rnd_number);
        sum = tmpNumber + rnd_number;
        counter++;
        sum_this_generator += rnd_number;
        usleep(1000);
    }
    
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
    printf("The sum of produced items for this number generator at the end is: %ld \n", sum_this_generator);
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
    finished_producers++;
    pthread_mutex_unlock(&mutex1); // unlock
    
    //H: If all generators have finished fire signal for condition variable
    if (finished_producers == PRODUCER_NO) {
        pthread_cond_signal(&cond1);
    }
    
    return (0);
}

void *print_function(void* junk) {
    pthread_mutex_lock(&mutex1); // lock
    
    //G: Wait until all generators have finished
    while (finished_producers < PRODUCER_NO) {
        pthread_cond_wait(&cond1, &mutex1);
    }
    
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("The value of sum at the end is: %ld \n", sum);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    
    pthread_mutex_unlock(&mutex1); // unlock
}
