//
// Created by lklake on 2019/7/7.
//

#ifndef TREE_POOL_H
#define TREE_POOL_H
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "queue.h"

typedef struct _pool{
    pthread_mutex_t lock;
    pthread_cond_t condition;
    pthread_t* thread_list;
    int power;
    int thread_num;
    queue* wait_queue;
    void (*start)(struct _pool* this);
    void (*add_task)(struct _pool* this,void* data,void(*)(void*));
    void (*distroy)(struct _pool* this);
}pool;

typedef struct _task{
    void (*func)(void* data);
    void* data;
}task;

void pool_init(pool*,int num);
void* worker(void *arg);
void pool_start(pool* );
void pool_distroy(pool* pool_instance);
void add_task(pool*,void* data,void (*)(void*));

#endif //TREE_POOL_H
