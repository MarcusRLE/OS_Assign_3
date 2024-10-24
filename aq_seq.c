/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include "aq.h"
#include <stdlib.h>

AlarmQueue aq_create( ) {
    aq_header * aq = malloc(sizeof(struct aq_header));
    aq->size = 0;
    aq->alarms = 0;
    aq->head = NULL;
    return aq;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
    // Check if the queue is initialized
    if (aq == NULL) {
        return AQ_UNINIT;
    }

    // Check if the message is NULL
    if (msg == NULL) {
        return AQ_NULL_MSG;
    }

    // Check if there is room for the message if alarm
    if (k == AQ_ALARM && ((aq_header*) aq)->alarms > 0) {
        return AQ_NO_ROOM;

    }

    aq_header * header = (aq_header*) aq;
    aq_node * new_node = malloc(sizeof(struct aq_node));
    new_node->msg = msg;
    new_node->kind = k;
    new_node->next = NULL;

    // If is alarm, add to the head
    if (k == AQ_ALARM) {
        new_node->next = header->head;
        header->head = new_node;
        header->alarms++;
        return 0;
    }

    // If list is empty, add to the head
    if(header->head == NULL) {
        header->head = new_node;
        return 0;
    }

    // Add to the end of the list
    aq_node* current = ((aq_header*) aq)->head;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
  return 0;
}

int aq_recv( AlarmQueue aq, void * * msg) {
  return AQ_NOT_IMPL;
}

int aq_size( AlarmQueue aq) {
  return 0;
}

int aq_alarms( AlarmQueue aq) {
  return 0;
}



