//
// Created by lklake on 2019/7/7.
//

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "pool.h"
#include "queue.h"


//工作函数
void* worker(void *arg){
    pool* my_pool=(pool*)arg;
    while(1){
        pthread_mutex_lock(&my_pool->lock);
        while(my_pool->wait_queue->isempty(my_pool->wait_queue)){
            pthread_cond_wait(&my_pool->condition,&my_pool->lock);
            if(!my_pool->power)
                break;
        }
        if(!my_pool->power)
            break;
        task* current_task=(task *)my_pool->wait_queue->top(my_pool->wait_queue);
        my_pool->wait_queue->dequeue(my_pool->wait_queue);
        pthread_mutex_unlock(&my_pool->lock);

        void (*func)(void* data);
        func=current_task->func;
        func(current_task->data);
    }
}

void pool_init(pool* this,int num){

    this->start=&pool_start;
    this->add_task = &add_task;
    this->distroy=&pool_distroy;

    this->wait_queue=malloc(sizeof(queue));
    queue_init(this->wait_queue);
    this->power=1;
    this->thread_num=num;
    pthread_mutex_init(&this->lock,NULL);
    pthread_cond_init(&this->condition,NULL);
    this->thread_list=(pthread_t*)malloc(sizeof(pthread_t)*num);
}

//线程池内加入线程
void pool_start(pool* this){
    for(int i=0;i<this->thread_num;i++){
        pthread_create(this->thread_list+i,NULL,worker,this);
        pthread_detach(*(this->thread_list+i));
    }
}

//线程池销毁
void pool_distroy(pool* this){
    this->power=0;
    pthread_cond_broadcast(&this->condition);
    printf("waiting for threads to exit\n");
    sleep(1);
    free(this->thread_list);
    pthread_mutex_destroy(&this->lock);
    pthread_cond_destroy(&this->condition);
    this->wait_queue->destroy(this->wait_queue);
    printf("pool destoryed\n");
}

void add_task(pool* this,void* data,void (*func)(void*)){
    task* tmp=malloc(sizeof(task));
    tmp->data=data;
    tmp->func=func;
    this->wait_queue->enqueue(this->wait_queue,(void*)tmp);
    pthread_cond_broadcast(&this->condition);//TODO
}

void func(void* data){
    sleep(1);
    printf("%d\n",*((int*)data));
}


int pool_main() {
    pool my_pool;
    pool_init(&my_pool, 5);
    my_pool.start(&my_pool);
    int tmp[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int tmp2[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    for (int i = 0; i < 10; i++) {
        my_pool.add_task(&my_pool, tmp + i, &func);
    }
    sleep(1);
    for (int i = 0; i < 10; i++) {
        my_pool.add_task(&my_pool, tmp2 + i, &func);
    }
    sleep(11);
    my_pool.distroy(&my_pool);
}