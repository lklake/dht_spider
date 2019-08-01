//
// Created by lklake on 2019/7/29.
//

#ifndef TREE_HASHTABLE_H
#define TREE_HASHTABLE_H

#define NAME_SIZE 10

#include<string.h>
#include <malloc.h>
typedef struct _item{
    char key[NAME_SIZE];
    void* value;
    int flag;
    int loop;
}item;
typedef struct _dict{
    item* table;
    int size;
    int num;
    void (*add)(struct _dict* this, char* key,void* value,int loop);
    void* (*get)(struct _dict* this,char* key);
    void (*remove)(struct _dict* this,char* key);
    void (*destroy)(struct _dict* this);
    void (*show)(struct _dict* this);
}dict;

int hash_func(const char* name);
void hash_table_init(dict* this,int size);
void hash_table_add(dict* this,char* key,void* value,int loop);

void* hash_table_get(dict* this,char* key);

void hash_table_remove(dict* this,char* key);

void hash_table_destroy(dict* this);
void hash_table_show(dict* this);
#endif //TREE_HASHTABLE_H
