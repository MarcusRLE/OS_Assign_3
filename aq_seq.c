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
    aq->head = NULL;
    aq->tail = NULL;
    return aq;
}

int insert_tail(aq_frame * frame, aq_node * new_node){
    new_node->next = NULL;
    if(frame->head == NULL) { // If list is empty, also make head
        frame->head = new_node;
        new_node->prev = NULL;
    } else { // Cycle to end of list
        aq_node* current = frame->head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
        new_node->prev = current;
    }
    frame->tail = new_node;

    // Increment the size of the queue
    frame->size++;

    return 0;
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

    // Check if there is room for the message if alarm
    if (k == AQ_ALARM && aq_alarms(aq) > 0) {
        return AQ_NO_ROOM;
    }

    /*
     * =========== ADDING MESSAGE ===========
     */
    aq_frame * frame = (aq_frame*) aq;
    aq_node * new_node = malloc(sizeof(aq_node));
    new_node->msg = msg;
    new_node->kind = k;

    if (k == AQ_ALARM) { // If is alarm, insert as head
        insert_head(frame, new_node);
    } else { // If normal message, insert as tail
        insert_tail(frame, new_node);
    }
    return 0;
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
    if (((aq_frame *)aq)->head == NULL) {
        return AQ_NO_MSG;
    }

    /*
     * =========== PULLING MESSAGE ===========
     */
    // Pull the node from head
    aq_frame * frame = (aq_frame*) aq;
    aq_node * pulled = frame->head;

    // Update the head of the queue
    if(aq_size(aq) == 1){ // If only one message in the queue
        frame->head = NULL;
        frame->tail = NULL;
    } else { // If more than one message in the queue
        frame->head = pulled->next;
        frame->head->prev = NULL;
    }

    // Decrement the size of the queue and alarms if the message is an alarm
    frame->size--;
    if(pulled->kind == AQ_ALARM){
        frame->alarms--;
    }

    *msg = pulled->msg;
    char kind = pulled->kind;
    free(pulled);
    return kind;
}

int aq_size( AlarmQueue aq) {
    // Check if the queue is initialized
    if (aq == NULL) {
        return AQ_UNINIT;
    }
    return ((aq_frame *)aq)->size;
}

int aq_alarms( AlarmQueue aq) {
    // Check if the queue is initialized
    if (aq == NULL) {
        return AQ_UNINIT;
    }
    return ((aq_frame *)aq)->alarms;
}

int insert_head(aq_frame * frame, aq_node * new_node){
    new_node->prev = NULL;
    if(frame->head == NULL) { // If list is empty, also make tail
        frame->tail = new_node;
        new_node->next = NULL;
    } else { // Insert as head
        new_node->next = frame->head;
        frame->head->prev = new_node;
    }
    frame->head = new_node;

    // Increment the size of the queue
    frame->size++;

    // Increment the number of alarms if the message is an alarm
    if(new_node->kind == AQ_ALARM){
        frame->alarms++;
    }

    return 0;
}

