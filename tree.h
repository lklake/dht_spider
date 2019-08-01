//
// Created by lklake on 2019/7/5.
//

#ifndef DHT_TREE_H
#define DHT_TREE_H


typedef struct _Node{
    char addr[6];
    unsigned char signature[20];
    long in_time;
}Node;

typedef struct _treeNode{
    int layer;
    unsigned char mask;
    struct _treeNode* lchild;
    struct _treeNode* rchild;
    int hasChild;
    int llen;
    int rlen;
    Node* nodes0[8];
    Node* nodes1[8];
}treeNode;

typedef struct _tree{
    treeNode* head;
    void(*add_node)(struct _tree*,Node*);
    void(*add_char)(struct _tree*,unsigned char*);
    char* (*get_bucket)(struct _tree*,const unsigned char*);
    void (*save)(struct _tree* this);
    void (*destroy)(struct _tree* this);
    void (*rebuilt)(struct _tree* this);
}tree;


int l_exist(Node* newNode,Node* nodeList[],int len);
void l_remove_old(Node* list[8],unsigned char* in);
void l_split(treeNode* treenode);
void l_my_free(void *p);

void tree_init(tree* my_tree);

void tree_add_node(tree* this,Node* newNode);
void tree_add_char(tree* this,unsigned char* a);
char* tree_get_bucket(tree* this,const unsigned char* a);
void tree_save(tree* this);
void tree_destroy(tree* this);
void tree_rebuilt(tree* this);


#endif //DHT_TREE_H
