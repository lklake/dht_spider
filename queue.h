//
// Created by lklake on 2019/7/5.
//

#ifndef DHT_QUEUE_H
#define DHT_QUEUE_H

#include <pthread.h>


typedef struct _queue_node{
    struct _queue_node* next;
    void* data;
}queue_node;

typedef struct _queue{
    struct _queue_node* head;
    struct _queue_node* tail;
    int size;
    pthread_mutex_t mutex;

    void(*enqueue) (struct _queue* this,void* data);
    void (*dequeue) (struct _queue* this);
    void*(*top)(struct _queue* this);
    void(*destroy)(struct _queue* this);
    int(*isempty)(struct _queue* this);

}queue;

void queue_init(queue* this);
void queue_enqueue(queue* this,void* data);
void queue_dequeue(queue* this);
void* queue_top(queue* this);
void queue_destroy(queue* this);
int queue_isempty(queue* this);
#endif //DHT_QUEUE_H
