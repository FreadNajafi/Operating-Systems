/* 
 * File:   main.c
 * Author: a4najafi
 *
 */

#include <stdio.h>      /* printf */
#include <sys/types.h> /* pid_t */
#include <unistd.h>     /* get_pid */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <sys/wait.h>  /* wait */
#include <pthread.h>
#include <sys/stat.h>

void * count(void *);

int globalNumber = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
    int i;
    pthread_t counterThread[5];

    for (i = 0; i < 5; i++) {
        pthread_create(&counterThread[i], NULL, count, NULL);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(counterThread[i], NULL);
    }

    printf("Final value of globalNumber: %d\n", globalNumber);

    return (0);
}

void * count(void * junk) {
    int loopCount = 0;
    pthread_mutex_lock(&mutex1);
    while (loopCount < 10) {
        int tmpNumber = globalNumber;
        printf("counter: %d, Thread: %ld, PID: %d\n",
                tmpNumber, (long)pthread_self(), getpid());
        tmpNumber = globalNumber;
        tmpNumber++;
        usleep(random() % 2);
        globalNumber = tmpNumber;
        loopCount++;
    }
    pthread_mutex_unlock(&mutex1);
    return (0);
}
