#include <sys/socket.h>
#include <netinet/in.h>
#include <dnet.h>
#include <errno.h>
#include "pool.h"
#include "str.h"
#include "tree.h"
#include "hashtable.h"
#include "bencode.h"
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


    /*设置socket属性，端口可以重用*/
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

void socket_recv(int socket_fd){
    char recvbuf[MAXBUF];
    char sendbuf[MAXBUF];
    int  ret;
    struct sockaddr_in client_addr;
    socklen_t cli_len=sizeof(client_addr);

    /* 开始处理每个新连接上的数据收发 */
    bzero(recvbuf, MAXBUF);
    bzero(sendbuf, MAXBUF);
    strcpy(sendbuf,"123456789");

    /* 接收客户端的消息 */
    ret = recvfrom(socket_fd, recvbuf, MAXBUF, 0, (struct sockaddr *)&client_addr, &cli_len);
    if (ret > 0){
        printf("socket %d recv from : %s : %d message: %s ，%d bytes\n",
               socket_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recvbuf, ret);

        ret = sendto(socket_fd, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&client_addr, cli_len);
        if(ret<0)
            printf("消息发送失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));


    }
    else
    {
        printf("received failed! error code %d，message : %s \n",
               errno, strerror(errno));
    }
}

void udp_net(){
    int socket_fd=0;
    socket_init(&socket_fd);
    while(1){
        socket_recv(socket_fd);
    }
}
int main_main(){
    udp_net();
}