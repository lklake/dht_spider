
#include <dnet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "pool.h"
#include "str.h"
#include "tree.h"
#include "hashtable.h"
#include "bencode.h"
#include "g_macros.h"

#define MY_PORT 6554
#define MY_IP "192.168.8.100"
#define MAXBUF 1024


void socket_init(int* socket_fd){
    struct sockaddr_in my_addr, their_addr;


    if ((*socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket create failed ！");
        exit(1);
    }
    else
    {
        printf("socket create  success \n");
    }


    int opt=SO_REUSEADDR;
    setsockopt(*socket_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));


    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(MY_PORT);
    my_addr.sin_addr.s_addr = inet_addr(MY_IP);
    if (bind(*socket_fd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    else
    {
        printf("IP and port bind success \n");
    }
}

void socket_send(int socket_fd,string* send_data,struct sockaddr_in* client_addr,socklen_t client_len){
    int ret;
    ret = sendto(socket_fd, send_data->data, send_data->length, 0, (struct sockaddr *)&client_addr, client_len);
    if(ret<0)
        printf("消息发送失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
}

void socket_recv(int socket_fd,string* recv_data,struct sockaddr_in* client_addr,socklen_t * client_len){
    char recv_buf[MAXBUF];
    int  ret;
    bzero(recv_buf, MAXBUF);

    ret = recvfrom(socket_fd, recv_buf, MAXBUF, 0, (struct sockaddr *)client_addr, client_len);

    if (ret > 0){
        recv_data=malloc(sizeof(string));
        string_init(recv_data,recv_buf,ret);
    }
    else
    {
        printf("received failed! error code %d，message : %s \n",
               errno, strerror(errno));
    }
}

void dht_net(pool* my_pool,tree* my_tree){
    int socket_fd=0;
    struct sockaddr_in client_addr;
    socklen_t client_len=0;
    socket_init(&socket_fd);
    while(1){
        string* recv_data;
        socket_recv(socket_fd,recv_data,&client_addr,&client_len);
        dict* tmp_dict=bdecode(recv_data);
//        deal;
//        string* send_data;

//        socket_send(socket_fd,send_data,&client_addr,client_len);
    }
    close(socket_fd);
}





int main_main(){
    pool my_pool;
    pool_init(&my_pool,POOL_SIZE);
    tree my_tree;
    tree_init(&my_tree);
    dht_net(&my_pool,&my_tree);
}