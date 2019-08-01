//
// Created by lklake on 2019/7/21.
//

#ifndef DHT_STR_H
#define DHT_STR_H


typedef struct _string string;

void l_enlarge(string* this);

void string_init(string* this,char* data,int length);
void string_append(string* this,char chr);
void string_nappend(string* this,string* that,int n);
void string_extend(string* this,string* that);


char string_i(string* this,int pos);
string* string_ii(string* this,int pos1,int pos2);
void string_destroy(string* this);
void string_print(string* this);

struct _string{
    char* data;
    int length;
    int max;
    void (*destroy)(string* this);
    char (*i)(string* this,int pos);
    string* (*ii)(string* this,int pos1,int pos2);
    void (*append)(string* this,char data);
    void (*nappend)(string* this,string* that,int n);
    void (*extend)(struct _string* this,struct _string* that);
    void (*print)(string* this);
};

#endif //DHT_STR_H
