
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include <assert.h>

#include "aq.h"
#include "test_own.h"

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

void * producer (void * arg) {
    put_normal(q, 1);
    put_alarm(q, 2);
    printf("!! Attempting to put new alarm message with value 3 !!\n");
    put_alarm(q, 3);

    return 0;
}

void * consumer(void * arg) {
    printf("-- Consumer thread sleeping for 5 sec --\n");
    msleep(5000);
    printf("-- Consumer thread waking up --\n");
    get(q);
    get(q);

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

    void * res1;
    void * res2;

    printf("----------------\n");

    /* Fork threads */
    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    /* Join with all threads */
    pthread_join(t1, &res1);
    pthread_join(t2, &res2);

    printf("----------------\n");
    printf("Threads terminated with %ld, %ld\n", (uintptr_t) res1, (uintptr_t) res2);

    print_sizes(q);



    return 0;
}

