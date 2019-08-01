//
// Created by lklake on 2019/7/5.
//

#ifndef TREE_TREE_H
#define TREE_TREE_H
typedef struct _Node{
    char addr[6];
    char signature[20];
    long in_time;
}Node;

typedef struct _treeNode{
    int layer;
    char mask;
    struct _treeNode* lchild;
    struct _treeNode* rchild;
    int hasChild;
    int llen;
    int rlen;
    Node* nodes0[8];
    Node* nodes1[8];
}treeNode;

typedef struct _Tree{
    treeNode* head;
    void(*add_node)(struct _Tree*,Node*);
    void(*add_char)(struct _Tree*,char*);
    char* (*get_bucket)(struct _Tree*,const char*);
    void (*save)(struct _Tree* this);
    void (*destroy)(struct _Tree* this);
    void (*rebuilt)(struct _Tree* this);
}Tree;


int l_exist(Node* newNode,Node* nodeList[],int len);
void l_remove_old(Node* list[8],char* in);
void l_split(treeNode* treenode);
void l_my_free(void *p);

void tree_init(Tree* my_tree);

void tree_add_node(Tree* this,Node* newNode);
void tree_add_char(Tree* this,char* a);
char* tree_get_bucket(Tree* this,const char* a);
void tree_save(Tree* this);
void tree_destroy(Tree* this);
void tree_rebuilt(Tree* this);


#endif //TREE_TREE_H
