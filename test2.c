#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include <assert.h>

#include "aq.h"
#include "test_own.h"

#include "aux.h"

//
// Created by lemse on 05/11/2024.
//

static AlarmQueue q;

void * consumer (void * arg) {
    // Waiting for producer to put messages in queue, and alarm message as last
    printf("-- Consumer thread sleeping for 5 sec --\n");
    msleep(5000);
    printf("-- Consumer thread waking up --\n");
    int first = get(q);
    int second = get(q);
    printf("First messaged pulled: '%d', Second message pulled: '%d'\n", first, second);
    // Should take alarm with value '6' out first'
    assert(first == 6);
    // Should take normal message with value '1' out second
    assert(second == 1);
    return 0;
}

void * producer(void * arg) {
    for(int i = 1; i < 6; i++){
        put_normal(q, i);
    }
    put_alarm(q, 6);
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