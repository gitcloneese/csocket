//
//  server.c
//  MyFistXcodeC
//
//  Created by panll on 2021/4/7.
//  Copyright © 2021 panll. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Using:./server port\nExample:./server 5005\n\n");
        return -1;
    }
   
    //listen socket
    int listenfd;
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        return -1;
    }
    
    //2
    struct sockaddr_in servaddr;
    memset(&servaddr, 0,sizeof(servaddr));
    
    servaddr.sin_family = AF_INET; // tcp只能用AF_INET
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //将主机数转换成无符号长整型的网络字节顺序。本函数将一个32位数从主机字节顺序转换成网络字节顺序。
        
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //将十进制点分形式转换为二进制形式
    servaddr.sin_port = htons(atoi(argv[1])); //指定端口
    
    if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0){
        perror("bind");
        close(listenfd);
        return -1;
    }
    
    
    //3   把socket设置为监听模式
    if(listen(listenfd, 5) != 0){
        perror("listen");
        close(listenfd);
        return -1;
    }
    //4 接口客服端的连接
    int clientfd;   //客户端socket
    int socklen = sizeof(struct sockaddr_in);  // struct sockaddr_in的大小
    struct sockaddr_in clientaddr;   //客户端的地址信息
    clientfd = accept(listenfd,(struct sockaddr *)&clientaddr, (socklen_t*)&socklen);
    printf("客服端(%s) 已经连接。\n", inet_ntoa(clientaddr.sin_addr));
    
    //5， 与客户端通信， 接收客户端发过来的报文后，回复ok
    
    char buffer[1024];
    
    while (1) {
        int iret;
        memset(buffer, 0,sizeof(buffer));
        if((iret = recv(clientfd,buffer,sizeof(buffer), 0)) <=0) //接收客户端请求报文
        {
            printf("iret = %d\n", iret);
            break;
        }
        printf("接收：%s\n", buffer);
        
        strcpy(buffer,"ok");
        if((iret = send(clientfd, buffer, strlen(buffer), 0)) <= 0){
            perror("send");
            break;
        }
        printf("发送: %s\n", buffer);
        }
    //6 关闭socket释放ziyuan
    close(clientfd);
}
