/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include "aq.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  void *normal_queue;
  void *alarm_message; // Storer en enkelt alarm besked
  int normal_count;    // Antal normal beskeder
  int has_alarm;       // Boolean om der er en alarm besked
} AlarmQueueStruct;


typedef struct QueueNode {
  void* message;
  struct QueueNode* next;
} QueueNode;

typedef struct {
  QueueNode* front;
  QueueNode* rear;
  int count;
} NormalQueue;



NormalQueue* create_normal_queue(void) {
  NormalQueue* queue = (NormalQueue*)malloc(sizeof(NormalQueue));
  if (!queue) return NULL;     // Return NULL hvis der er fejl i allocation

  queue->front = NULL;
  queue->rear = NULL;
  queue->count = 0;
  return queue;
}


AlarmQueue aq_create() {
  AlarmQueueStruct *q = malloc(sizeof(AlarmQueueStruct));
  if (!q) return NULL;  // Return NULL hvis der er fejl i allocation

  q->normal_queue = create_normal_queue();  // Lav normal kø
  q->alarm_message = NULL;                  // Ingen alarm besked
  q->normal_count = 0;
  q->has_alarm = 0;
  return q;
}



void enqueue_normal_message(NormalQueue* queue, void* msg) {
  if (!queue) return;

  QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
  if (!new_node) return;        // Allocation failure

  new_node->message = msg;
  new_node->next = NULL;

  if (queue->rear) {
    queue->rear->next = new_node;  // Link ny node til rear
  } else {
    queue->front = new_node;       // Hvis køen er tom sæt front til new node
  }

  queue->rear = new_node;            // Opdaterer rear pointer
  queue->count++;
}



int aq_send(AlarmQueue aq, void *msg, MsgKind kind) {
  AlarmQueueStruct *q = (AlarmQueueStruct *)aq;
  if (kind == AQ_ALARM) {
    if (q->has_alarm) {
      return AQ_NO_ROOM;  // Retrun error hvis der allerede er en alarm
    }
    q->alarm_message = msg;
    q->has_alarm = 1;
  } else {  // Normal message
    enqueue_normal_message(q->normal_queue, msg);  // Tilføj til normal kø
    q->normal_count++;
  }
  return AQ_SUCCESS;
}


void* dequeue_normal_message(NormalQueue* queue) {
  if (!queue || queue->count == 0) return NULL;

  QueueNode* temp = queue->front;
  void* msg = temp->message;

  queue->front = temp->next;        //Flyt pointer til næste ndoe
  if (!queue->front) {              // Hvis køen er tom
    queue->rear = NULL;
  }

  free(temp);                       // Fri den fjernede node
  queue->count--;

  return msg;
}


int aq_recv(AlarmQueue aq, void **msg) {
  AlarmQueueStruct *q = (AlarmQueueStruct *)aq;

  if (q->has_alarm) {  // Prio alarm beskeder
    *msg = q->alarm_message;
    q->alarm_message = NULL;
    q->has_alarm = 0;
    return AQ_ALARM;
  } else if (q->normal_count > 0) {  // Normal besked hvis ingen alarm
    *msg = dequeue_normal_message(q->normal_queue);
    q->normal_count--;
    return AQ_NORMAL;
  } else {
    return AQ_NO_MSG;  // Kø tom
  }
}

int aq_size(AlarmQueue aq) {
  AlarmQueueStruct *q = (AlarmQueueStruct *)aq;
  return q->normal_count + (q->has_alarm ? 1 : 0);
}

int aq_alarms(AlarmQueue aq) {
  AlarmQueueStruct *q = (AlarmQueueStruct *)aq;
  return q->has_alarm;
}


void destroy_normal_queue(NormalQueue* queue) {
  if (!queue) return;

  QueueNode* current = queue->front;
  while (current) {
    QueueNode* next = current->next;
    free(current->message); //fri besked dynamsik
    free(current);
    current = next;
  }

  free(queue);
}



