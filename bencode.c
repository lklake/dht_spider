//
// Created by lklake on 2019/7/30.
//

#include <ctype.h>
#include<stdio.h>
#include <malloc.h>
#include <string.h>

#include "str.h"
#include "bencode.h"


void* decode(string* raw,int* index){
    if(raw->i(raw,*index)=='d') {//字典
        return d_dict(raw, index);
    }
    if(raw->i(raw,*index)=='i') {//整数
        return d_int(raw, index);
    }
    if(isdigit(raw->i(raw,*index))) {//字符串
        return d_str(raw, index);
    }
    if(raw->i(raw,*index)=='l') {//列表
        return d_list(raw, index);
    }
}
dict* bdecode(string* raw) {
    int index=0;
    return (dict*)d_dict(raw,&index);
}

dict* d_dict(string* raw,int* index){
    int loop=0;
    (*index)++;//'d'
    dict* mydict=malloc(sizeof(dict));
    hash_table_init(mydict,20);
    while(raw->i(raw,*index)!='e'){
        loop=0;
        char* key=d_cstr(raw,index);
        if(raw->i(raw,*index)=='d')
            loop=1;
        string* value=decode(raw,index);
        mydict->add(mydict,key,value,loop);
    }
    (*index)++;//'e'
    return mydict;
}

string* d_int(string* raw,int* index){
    (*index)++;//'i'
    string* data=malloc(sizeof(string));
    string_init(data,NULL,5);
    while(isdigit(raw->i(raw,*index))){
        data->append(data,raw->i(raw,*index));
        (*index)++;
    }
    (*index)++;//'e'
    return data;
}


string* d_list(string* raw,int* index){
    string* data=malloc(sizeof(string));
    string_init(data,NULL,5);
    (*index)++;//'l'
    while(raw->i(raw,*index)!='e'){
        data->extend(data, (void*)decode(raw,index));
    }
    (*index)++;//'e'
    return data;
}
char* d_cstr(string* raw,int* index){
    int num = 0;
    while (isdigit(raw->i(raw,*index))){
        num *= 10;
        num += raw->i(raw,*index) - '0';
        (*index) ++;
    }
    (*index) ++;//':'
    char* data=malloc(sizeof(char)*(num+1));
    memcpy(data,raw->data+*index,num);
    data[num]='\0';
    (*index)+=num;
    return data;
}

string* d_str(string* raw,int* index) {
    int num = 0;
    while (isdigit(raw->i(raw,*index))){
        num *= 10;
        num += raw->i(raw,*index) - '0';
        (*index) ++;
    }
    (*index) ++;//':'
    string* data=raw->ii(raw,*index,*index+num);
    (*index)+=num;
    return data;
}


#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define DEST_PORT 6881
#define DSET_IP_ADDRESS  "67.215.246.10"


int bencode_main()
{

    int sock_fd;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0)
    {
        perror("socket");
        exit(1);
    }


    struct sockaddr_in addr_serv;
    int len;
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = inet_addr(DSET_IP_ADDRESS);
    addr_serv.sin_port = htons(DEST_PORT);
    len = sizeof(addr_serv);


    int send_num;
    int recv_num;
    char send_buf[8024+1] = "d1:ad2:id20:abcdefghij01234567899:info_hash20:1c5d2a3c1d2e6ff5d4ace1:q9:get_peers1:t2:aa1:y1:qe";
    char recv_buf[9024+1];

    printf("client send: %s\n", send_buf);

    send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_serv, len);

    if(send_num < 0)
    {
        perror("sendto error:");
        exit(1);
    }

    recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)&len);

    if(recv_num < 0)
    {
        perror("recvfrom error:");
        exit(1);
    }

    recv_buf[recv_num] = '\0';
    printf("client receive %d bytes\n", recv_num);
    string str1;
    dict* dict1;
    string_init(&str1,recv_buf,recv_num);
    dict1=bdecode(&str1);
    dict1->show(dict1);
    str1.destroy(&str1);
    dict1->destroy(dict1);

    close(sock_fd);

    return 0;
}