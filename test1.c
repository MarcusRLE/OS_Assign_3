
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include "aq.h"

#include "aux.h"

/**
 * Concurrent program that sends and receives a few integer messages
 * in order to demonstrate the basic use of the thread-safe Alarm Queue Library
 *
 * By using sleeps we (try to) control the scheduling of the threads in
 * order to obtain specific execution scenarios.  But there is no guarantee.
 *
 */

static AlarmQueue q;

void * prod1(void * arg) {
    printf("-- Producer 1 sleeping for 2.5 sec --\n");
    msleep(2500);
    printf("-- Producer 1 waking up --\n");
    put_normal(q, 1);
    put_normal(q, 2);
    printf("-- Producer 1 finished sending 2 normal messages --\n");
    return 0;
}

void * prod2(void * arg) {
    printf("!! Producer 2 attempting to put alarm message with value %d !!\n", (-1));
    put_alarm(q, -1);
    printf("!! Producer 2 attempting to put alarm message with value %d !!\n", (-2));
    put_alarm(q, -2);
    put_normal(q, -3);
    printf("-- Producer 2 finished sending 2 alarm messages 1 normal messages --\n");
    return 0;
}

void * cons(void * arg) {
    printf("-- Consumer thread sleeping for 5 sec --\n");
    msleep(5000);
    printf("-- Consumer thread waking up --\n");
    for(int i = 0; i < 5; i++){
        get(q);
    }
    printf("-- Consumer finished receiving 5 messages --\n");
    return 0;
}

int main(int argc, char ** argv) {
    int ret;

    q = aq_create();

    if (q == NULL) {
        printf("Alarm queue could not be created\n");
        exit(1);
    }

    pthread_t t1;
    pthread_t t2;
    pthread_t t3;

    void * res1;
    void * res2;
    void * res3;

    printf("----------------\n");

    /* Fork threads */
    pthread_create(&t1, NULL, prod1, NULL);
    pthread_create(&t2, NULL, prod2, NULL);
    pthread_create(&t3, NULL, cons, NULL);

    /* Join with all threads */
    pthread_join(t1, &res1);
    pthread_join(t2, &res2);
    pthread_join(t3, &res3);

    printf("----------------\n");
    printf("Threads terminated with %ld, %ld, %ld\n", (uintptr_t) res1, (uintptr_t) res2, (uintptr_t) res3);

    print_sizes(q);



    return 0;
}


