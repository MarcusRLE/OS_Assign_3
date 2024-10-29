
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

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

int main(int argc, char ** argv) {
    test(1);
    test(2);
    return 0;
}

int test(int testNr){
    printf("============= TEST #%d =============\n", testNr);
    printf("\n");
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

    /* Fork threads */
    if(testNr == 1){
        pthread_create(&t1, NULL, p1, NULL);
        pthread_create(&t2, NULL, c1, NULL);
    } else if(testNr == 2){
        pthread_create(&t1, NULL, p2, NULL);
        pthread_create(&t2, NULL, c2, NULL);
    }

    /* Join with all threads */
    pthread_join(t1, &res1);
    pthread_join(t2, &res2);

    printf("----------------\n");
    printf("Threads terminated with %ld, %ld\n", (uintptr_t) res1, (uintptr_t) res2);

    print_sizes(q);
    printf("====================================\n");
    printf("\n");
    printf("\n");
    return 0;
}

void * p1 (void * arg) {
    put_normal(q, 1);
    put_alarm(q, 2);
    put_alarm(q, 3);

    return 0;
}

void * c1(void * arg) {
    get(q);
    msleep(2000);
    get(q);
    get(q);

    return 0;
}

void * c2(void * arg){
    get(q);
    return 0;
}
void * p2(void * arg){
    put_normal(q, 1);
    put_alarm(q, 2);
    put_normal(q, 3);
    return 0;
}