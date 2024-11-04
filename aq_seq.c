/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include "aq.h"
#include <stdlib.h>

AlarmQueue aq_create( ) {
    aq_frame * aq = malloc(sizeof(aq_frame));
    aq->size = 0;
    aq->alarms = 0;
    aq->alarm_msg = NULL;
    aq->head = NULL;
    aq->tail = NULL;
    return aq;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
    /*
     * =========== ERROR HANDLING ===========
     */
    // Check if the queue is initialized
    if (aq == NULL) {
        return AQ_UNINIT;
    }

    // Check if the message is NULL
    if (msg == NULL) {
        return AQ_NULL_MSG;
    }

    /*
     * =========== ADDING MESSAGE ===========
     */
    return insert_msg(aq, msg, k);
}

int aq_recv( AlarmQueue aq, void * * msg) {
    /*
     * =========== ERROR HANDLING ===========
     */
    // Check if the queue is initialized
    if (aq == NULL) {
        return AQ_UNINIT;
    }

    // Check if there are messages
    if (((aq_frame *)aq)->head == NULL && aq_alarms(aq) == 0) {
        return AQ_NO_MSG;
    }

    /*
     * =========== PULLING MESSAGE ===========
     */

    aq_frame * frame = (aq_frame*) aq;
    if (aq_alarms(frame) > 0){
        *msg = frame->alarm_msg;
        frame->alarms--;
        return AQ_ALARM;
    } else {
        aq_node * pulled = frame->head;
        // Update the head of the queue
        if(aq_size(aq) == 1){ // If only one message in the queue
            frame->head = NULL;
            frame->tail = NULL;
        } else { // If more than one message in the queue
            frame->head = pulled->next;
        }

        // Decrement the size of the queue and alarms if the message is an alarm
        frame->size--;

        *msg = pulled->msg;

        free(pulled);
        return AQ_NORMAL;
    }
}

int aq_size( AlarmQueue aq) {
    // Check if the queue is initialized
    if (aq == NULL) {
        return AQ_UNINIT;
    }
    return ((aq_frame *)aq)->size + ((aq_frame *)aq)->alarms;
}

int aq_alarms( AlarmQueue aq) {
    // Check if the queue is initialized
    if (aq == NULL) {
        return AQ_UNINIT;
    }
    return ((aq_frame *)aq)->alarms;
}

/*
 * =========== INTERNAL FUNCTIONS ===========
 */

int insert_alarm(aq_frame * frame, void * msg){
    if(aq_alarms(frame) == 0){
        frame->alarm_msg = msg;
        frame->alarms++;
        return 0;
    } else {
        return AQ_NO_ROOM;
    }
}

int insert_tail(aq_frame * frame, aq_node * new_node){
    new_node->next = NULL;
    if(frame->head == NULL) { // If list is empty, also make head
        frame->head = new_node;
    } else { // Cycle to end of list
        aq_node* current = frame->head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    frame->tail = new_node;

    // Increment the size of the queue
    frame->size++;

    return 0;
}

int insert_msg(AlarmQueue aq, void * msg, MsgKind k){
    aq_frame * frame = (aq_frame*) aq;
    if (k == AQ_ALARM) { // If is alarm, insert as head
        return insert_alarm(frame, msg);
    } else { // If normal message, insert as tail
        aq_node * new_node = malloc(sizeof(aq_node));
        new_node->msg = msg;
        return insert_tail(frame, new_node);
    }
}