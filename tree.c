
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "tree.h"
#include "queue.h"


extern char mask[8];

void l_remove_old(Node* list[8],unsigned char* in){
    long in_time=list[0]->in_time;
    int index=0;
    for(int i=1;i<8;i++){
        if(list[i]->in_time<in_time)
            index=i;
    }
    list[index]->in_time=time(NULL);
    memcpy(list[index]->signature,in,20);
    memcpy(list[index]->addr,in+20,6);
}


void l_split(treeNode* treenode) {
    treenode->hasChild=1;
    treenode->lchild = (treeNode *) malloc(sizeof(treeNode));
    treenode->rchild = (treeNode *) malloc(sizeof(treeNode));
    treenode->lchild->layer = treenode->layer + 1;
    treenode->rchild->layer = treenode->layer + 1;
    treenode->lchild->mask = mask[treenode->lchild->layer % 8];
    treenode->rchild->mask = mask[treenode->rchild->layer % 8];
    for (int i = 0; i < treenode->llen; i++) {
        unsigned char tmp = treenode->nodes0[i]->signature[treenode->lchild->layer % 8];
        if (( tmp & treenode->lchild->mask) == tmp) {
            treenode->lchild->nodes0[treenode->lchild->llen] = treenode->nodes0[i];
            treenode->lchild->llen++;
        } else {
            treenode->lchild->nodes1[treenode->lchild->rlen] = treenode->nodes0[i];
            treenode->lchild->rlen++;
        }
    }
    for (int i = 0; i < treenode->rlen; i++) {
        unsigned char tmp = treenode->nodes1[i]->signature[treenode->rchild->layer % 8];
        if ((tmp & treenode->rchild->mask) == tmp) {
            treenode->rchild->nodes0[treenode->rchild->llen] = treenode->nodes1[i];
            treenode->rchild->llen++;
        } else {
            treenode->rchild->nodes1[treenode->rchild->rlen] = treenode->nodes1[i];
            treenode->rchild->rlen++;
        }
    }
}

void l_my_free(void *p){
    void **tp=(void **)p;
    if(NULL==*tp)
        return ;
    free(*tp);
    *tp=NULL;
}

int l_exist(Node* newNode,Node* nodeList[],int len){
    int flag=0;
    for(int i=0;i<len;i++){
        if(memcmp(nodeList[i]->addr,newNode->addr,20)==0)
            flag=1;
    }
    return flag;
}

void tree_add_node(tree* this,Node* newNode){
    treeNode* tmp=this->head;
    while(tmp->hasChild){
        int layer=tmp->layer;
        if((newNode->signature[layer/8]&tmp->mask)==newNode->signature[layer/8]){
            tmp=tmp->rchild;
        }
        else
            tmp=tmp->lchild;
    }
    //没有子节点
    int layer=tmp->layer;
    if((newNode->signature[layer/8]|tmp->mask)!=newNode->signature[layer/8]){
        if(l_exist(newNode,tmp->nodes0,tmp->llen))
            return ;
        if(tmp->rlen==0){
            if(tmp->llen==8)
                l_remove_old(tmp->nodes0,newNode->signature);
            else {
                tmp->nodes0[tmp->llen] = newNode;
                tmp->llen++;
            }
        }
        else//另一支不为空
        {
            if(tmp->llen==8) {//要分裂了
                l_split(tmp);
                tmp = tmp->lchild;
            }
            //插入就好
            tmp->nodes0[tmp->llen] = newNode;
            tmp->llen++;
        }
    }
    else{
        if(l_exist(newNode,tmp->nodes1,tmp->rlen))
            return ;
        if(tmp->llen==0){
            if(tmp->rlen==8)
                l_remove_old(tmp->nodes1,newNode->signature);
            else {
                tmp->nodes1[tmp->rlen] = newNode;
                tmp->rlen++;
            }
        }
        else//另一支不为空
        {
            if(tmp->rlen==8) {//要分裂了
                l_split(tmp);
                tmp = tmp->rchild;
            }
            //插入就好
            tmp->nodes1[tmp->rlen] = newNode;
            tmp->rlen++;
        }
    }
}



void tree_add_char(tree* this,unsigned char* a){
    Node* newNode=(Node*)malloc(sizeof(Node));
    memcpy(newNode->signature,a,20);
    memcpy(newNode->addr,a+20,6);
    newNode->in_time=time(NULL);
    treeNode* tmp=this->head;
    while(tmp->hasChild){
        int layer=tmp->layer;
        if((a[layer/8]&tmp->mask)==a[layer/8]){
            tmp=tmp->rchild;
        }
        else
            tmp=tmp->lchild;
    }
    //没有子节点
    int layer=tmp->layer;
    if((a[layer/8]|tmp->mask)!=a[layer/8]){
        if(l_exist(newNode,tmp->nodes0,tmp->llen))
            return ;
        if(tmp->rlen==0){
            if(tmp->llen==8)
                l_remove_old(tmp->nodes0,a);
            else {
                tmp->nodes0[tmp->llen] = newNode;
                tmp->llen++;
            }
        }
        else//另一支不为空
        {
            if(tmp->llen==8) {//要分裂了
                l_split(tmp);
                tmp = tmp->lchild;
            }
            //插入就好
            tmp->nodes0[tmp->llen] = newNode;
            tmp->llen++;
        }
    }
    else{
        if(l_exist(newNode,tmp->nodes1,tmp->rlen))
            return ;
        if(tmp->llen==0){
            if(tmp->rlen==8)
                l_remove_old(tmp->nodes1,a);
            else {
                tmp->nodes1[tmp->rlen] = newNode;
                tmp->rlen++;
            }
        }
        else//另一支不为空
        {
            if(tmp->rlen==8) {//要分裂了
                l_split(tmp);
                tmp = tmp->rchild;
            }
            //插入就好
            tmp->nodes1[tmp->rlen] = newNode;
            tmp->rlen++;
        }
    }
    free(newNode);
}


void tree_init(tree* my_tree){
    my_tree->head=(treeNode*)malloc(sizeof(treeNode));
    my_tree->head->hasChild=0;
    my_tree->head->lchild=NULL;
    my_tree->head->rchild=NULL;
    my_tree->head->llen=0;
    my_tree->head->rlen=0;
    my_tree->head->layer=0;
    my_tree->head->mask=mask[my_tree->head->layer%8];

    my_tree->destroy=&tree_destroy;
    my_tree->add_char=&tree_add_char;
    my_tree->add_node=&tree_add_node;
    my_tree->get_bucket=&tree_get_bucket;
    my_tree->save=&tree_save;
    my_tree->rebuilt=&tree_rebuilt;
}

char* tree_get_bucket(tree* this,const unsigned char* a){//a为21位字符串
    char* data;
    treeNode* iterator=this->head;
    while(iterator->hasChild){
        if((a[iterator->layer/8]|iterator->mask)!=a[iterator->layer/8]){
            iterator=iterator->lchild;
        }
        else
            iterator=iterator->rchild;
    }
    if((a[iterator->layer/8]|iterator->mask)!=a[iterator->layer/8]){
        data=malloc(sizeof(char)*iterator->llen*26+1);
        for(int i=0;i<iterator->llen;i++){
            memcpy(data+i*26,  iterator->nodes0[i]->signature, 20);
            memcpy(data+i*26+20,  iterator->nodes0[i]->addr, 6);
        }
        *(data+iterator->llen*26)='\0';
    }
    else{
        data=malloc(sizeof(char)*iterator->rlen*26+1);
        for(int i=0;i<iterator->rlen;i++){
            memcpy(data+i*26,  iterator->nodes1[i]->signature, 20);
            memcpy(data+i*26+20,  iterator->nodes1[i]->addr, 6);
        }
        *(data+iterator->rlen*26)='\0';
    }
    return data;
}

void tree_save(tree* this){
    FILE *fp;
    fp=fopen("tree.dat","wb");
    treeNode* iterator=this->head;
    queue* tree_node_queue=(queue*)malloc(sizeof(queue));
    tree_node_queue->enqueue(tree_node_queue,iterator);
    while(!tree_node_queue->isempty(tree_node_queue)){
        iterator=(treeNode*)tree_node_queue->top(tree_node_queue);
        tree_node_queue->dequeue(tree_node_queue);
                if(iterator->hasChild){
                    if(iterator->lchild)
                        tree_node_queue->enqueue(tree_node_queue,iterator->lchild);
                    if(iterator->rchild)
                        tree_node_queue->enqueue(tree_node_queue,iterator->rchild);
        }
        if(iterator->llen){
            for(int i=0;i<iterator->llen;i++)
                fwrite(&(iterator->nodes0[i]), sizeof(Node),1,fp);
        }
        if(iterator->rlen){
            for(int i=0;i<iterator->rlen;i++)
                fwrite(&(iterator->nodes1[i]), sizeof(Node),1,fp);
        }
    }
    fclose(fp);
}

void tree_destroy(tree* this){
    treeNode* iterator=this->head;
    queue* tree_node_queue=(queue*)malloc(sizeof(queue));
    tree_node_queue->enqueue(tree_node_queue,iterator);
    while(!tree_node_queue->isempty(tree_node_queue)) {
        iterator = (treeNode*)tree_node_queue->top(tree_node_queue);
        tree_node_queue->dequeue(tree_node_queue);
        if (iterator->hasChild) {
            if (iterator->lchild)
                tree_node_queue->enqueue(tree_node_queue,iterator->lchild);
            if (iterator->rchild)
                tree_node_queue->enqueue(tree_node_queue,iterator->rchild);
        }
        if (iterator->llen) {
            for (int i = 0; i < iterator->llen; i++) {
                free(iterator->nodes0[i]);
                iterator->nodes0[i]=NULL;
            }
            free(iterator->nodes0);
        }
        if (iterator->rlen) {
            for (int i = 0; i < iterator->rlen; i++) {
                free(iterator->nodes1[i]);
                iterator->nodes1[i]=NULL;
            }
            free(iterator->nodes1);
        }
        free(iterator);
    }
    tree_node_queue->destroy(tree_node_queue);
}

void tree_rebuilt(tree* this){
    FILE *fp;
    fp=fopen("tree.dat","rb");
    Node* tmp=(Node*)malloc(sizeof(Node));
    fread(&tmp,sizeof(Node),1,fp);
    while(!feof(fp)){
        this->add_node(this,tmp);
        fread(&tmp,sizeof(Node),1,fp);
    }
    fclose(fp);
}

//TODO 通过分叉实现读写并发