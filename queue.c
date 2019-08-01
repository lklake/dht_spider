//
// Created by lklake on 2019/7/5.
//

#include "queue.h"
#include <malloc.h>
void queue_init(queue* this){
    this->destroy=&queue_destroy;
    this->enqueue=&queue_enqueue;
    this->isempty=&queue_isempty;
    this->top=&queue_top;
    this->dequeue=&queue_dequeue;

    this->head=(queue_node*)malloc(sizeof(queue_node));
    this->tail=(queue_node*)malloc(sizeof(queue_node));
    this->head->next=this->tail;
    this->tail->next=NULL;
    this->size=0;
    pthread_mutex_init(&this->mutex,NULL);
}
void queue_enqueue(queue* this,void* data){
    pthread_mutex_lock(&this->mutex);
    this->tail->data=data;
    this->tail->next=(queue_node*)malloc(sizeof(queue_node));
    this->tail=this->tail->next;
    this->tail->next=NULL;
    this->size++;
    pthread_mutex_unlock(&this->mutex);
}

void queue_dequeue(queue* this){
    if(this->size>0){
        pthread_mutex_lock(&this->mutex);
        queue_node* tmp=this->head;
        this->head=this->head->next;
        free(tmp);
        tmp=NULL;
        this->size--;
        pthread_mutex_unlock(&this->mutex);
    }
}
void* queue_top(queue* this){
    if(this->size>0)
        return this->head->next->data;
    else{
        printf("empty queue\n");
    }
}
void queue_destroy(queue* this){
    pthread_mutex_lock(&this->mutex);
    queue_node* tmp=this->head;
    while(tmp->next){
        this->head=this->head->next;
        free(tmp);
        tmp=this->head;
    }
    pthread_mutex_unlock(&this->mutex);
    pthread_mutex_destroy(&this->mutex);
}
int queue_isempty(queue* this){
    if(this->size==0)
        return 1;
    return 0;
}