//
// Created by lklake on 2019/7/21.
//

#include <malloc.h>
#include<string.h>

#include "str.h"


void string_init(string* this,char* data,int length){
    this->i=&string_i;
    this->ii=&string_ii;
    this->append=&string_append;
    this->nappend=&string_nappend;
    this->extend=&string_extend;
    this->destroy=&string_destroy;
    this->print=&string_print;

    if(data==NULL){
        this->length=0;
        this->max=length;
        this->data=malloc(sizeof(char)*this->max);
        return;
    }
    this->max=length;
    char* tmp=(char*)malloc(sizeof(char)*this->max);
    memcpy(tmp,data,length);
    this->data=tmp;
    this->length=length;
}

void string_destroy(string* this){
    if(this->data!=NULL) {
        free(this->data);
        this->data = NULL;
    }
}

void string_extend(string* this, string* that){
    if(this->length==0){
        *this=*that;
        return;
    }
    if(that->length==0){
        return;
    }
    while(this->max<this->length+that->length)
        l_enlarge(this);
    memcpy(this->data+this->length,that->data,that->length);
    this->length+=that->length;
}
void string_nappend(string* this, string* that,int n){
    if(this->length==0){
        *this=*that;
        return;
    }
    if(that->length==0){
        return;
    }
    n=that->length<n?that->length:n;
    while(this->max<this->length+n)
        l_enlarge(this);
    memcpy(this->data+this->length,that->data,n);
    this->length+=n;
}


void string_append(string* this,char chr){
    if(this->max-this->length<1)
        l_enlarge(this);
    *(this->data+this->length)=chr;
    this->length++;
}

char string_i(string* this,int pos){
    if(pos>=this->length||pos<-this->length){
        printf("index out of range//len:%d,index:%d\n",this->length,pos);
        return '\0';
    }
    if(pos<0)
        return *(this->data+this->length+pos);
    return *(this->data+pos);
}


string* string_ii(string* this,int pos1,int pos2){
    if(pos1>=this->length||pos1<-this->length||pos2>=this->length||pos2<-this->length){
        printf("index out of range//len:%d,index1:%d,index2:%d\n",this->length,pos1,pos2);
        return NULL;
    }
    int tmp1,tmp2;
    tmp1=(pos1+this->length)%this->length;
    tmp2=(pos2+this->length)%this->length;
    pos1=tmp1>=tmp2?tmp2:tmp1;
    pos2=tmp1>=tmp2?tmp1:tmp2;

    string* str=malloc(sizeof(string));
    string_init(str,this->data+pos1,pos2-pos1);
    return str;
}

void string_print(string* this){
    for(int i=0;i<this->length;i++){
        putchar(*(this->data+i));
    }
    fflush(stdout);//debug
}

void enlarge(string* this){
    char* str=(char*)malloc(sizeof(char)*this->max*3/2+1);
    this->max=this->max*3/2+1;
    memcpy(str,this->data,this->length);
    if(this->data!=NULL) {
        free(this->data);
        this->data = NULL;
    }
    this->data=str;
}
int str_main(){
    string a,b;
    string_init(&a,"lklake",6);
    string_init(&b,"hahahaha",8);
    string* str=a.ii(&a,1,2);
    str->print(str);
    a.destroy(&a);
    b.destroy(&b);
}

