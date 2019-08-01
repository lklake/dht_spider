//
// Created by lklake on 2019/7/29.
//

#include "hashtable.h"
#include "str.h"


int hash_func(const char* name){
    return name[0];
}

void hash_table_init(dict* this,int size){
    this->size=size;
    this->num=0;
    this->table=(item*) malloc(sizeof(item)*size);
    for(int i=0;i<this->size;i++){
        this->table[i].flag=0;
        this->table[i].value=NULL;
    }

    this->destroy=&hash_table_destroy;
    this->add=&hash_table_add;
    this->remove=&hash_table_remove;
    this->get=&hash_table_get;
    this->show=&hash_table_show;
}
void hash_table_add(dict* this,char* key,void* value,int loop){
    if(this->num>=this->size){
        printf("full,failed to add\n");
        return;
    }
    item item1;
    item1.flag=1;
    item1.loop=loop;
    strcpy(item1.key,key);
    item1.value=value;
    int index=hash_func(item1.key);
    if(this->table[index%this->size].flag==0){
        this->table[index%this->size]=item1;
    }else{
        while(this->table[++index%this->size].flag!=0);
        this->table[index%this->size]=item1;
    }
    this->num++;
}

void* hash_table_get(dict* this,char* key){
    int index=hash_func(key);
    if(this->table[index%this->size].flag==0){
        printf("no such key\n");
        return NULL;
    }else{
        int flag=0;
        int record=index%this->size;
        while(this->table[index%this->size].flag==0||(strcmp(this->table[index%this->size].key,key)==0)) {
            index++;
            if(index%this->size==record){
                flag=1;
                break;
            }
        }
        if(flag){
            printf("no such key\n");
            return NULL;
        }
        return this->table[index%this->size].value;
    }
}

void hash_table_remove(dict* this,char* key){
    int index=hash_func(key);
    if(this->table[index%this->size].flag==0){
        printf("no such key\n");
        return;
    }else{
        int flag=0;
        int record=index%this->size;
        while(this->table[index%this->size].flag==0||(strcmp(this->table[index%this->size].key,key)!=0)) {
            index++;
            if(index%this->size==record){
                flag=1;
                break;
            }
        }
        if(flag){
            printf("no such key\n");
            return;
        }
        this->table[index%this->size].flag=0;
        if(this->table[index%this->size].loop==1)
            ((dict*)(this->table[index%this->size].value))->destroy(((dict*)(this->table[index%this->size].value)));
        else {
            free(this->table[index % this->size].value);
            this->table[index % this->size].value=NULL;
        }
    }
}

void hash_table_destroy(dict* this){
    for(int i=0;i<this->size;i++){
        if(this->table[i].flag) {
            if (this->table[i].loop)
                hash_table_destroy((dict *) (this->table[i].value));
            else
                free(this->table[i].value);
        }
    }
    free(this->table);
}

void hash_table_show(dict* this){
    putchar('{');
    for(int i=0;i<this->size;i++){
        if(this->table[i].flag==1){
            if(!this->table[i].loop){
                printf("%s:",this->table[i].key);
                ((string*)this->table[i].value)->print((string*)this->table[i].value);
                putchar(',');
            }
            else{
                hash_table_show((dict*)this->table[i].value);
            }
        }
    }
    putchar('}');
}

int hash_table_main(){
    dict my_dict;
    string* str1=malloc(sizeof(string));
    string* str2=malloc(sizeof(string));
    string_init(str1,"lklake",6);
    string_init(str2,"10",2);
    hash_table_init(&my_dict,10);
    my_dict.add(&my_dict,"name",str1,0);
    my_dict.add(&my_dict,"age",str2,0);
    my_dict.remove(&my_dict,"age");
    my_dict.show(&my_dict);
    my_dict.destroy(&my_dict);
}