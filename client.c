//
//  client.c
//  MyFistXcodeC
//
//  Created by panll on 2021/4/7.
//  Copyright © 2021 panll. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Using:./client ip port\nExample:./client 127.0.0.1 5005\n\n");
        return -1;
        
    }
    //1,  创建客户端socket
    int sockfd;  //文件描述符
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        return -1;
    }
   //2, 向服务器发起连接请求
    struct hostent* h;
    if((h = gethostbyname(argv[1])) == 0){
        printf("gethostbyname failed.\n");
        close(sockfd);
        return -1;
    }
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    memcpy(&servaddr.sin_addr, h->h_addr,h->h_length);
    
    printf("%d\n", servaddr.sin_addr.s_addr);

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) //向服务器发起连接请求
    {
        perror("connect");
        close(sockfd);
        return -1;
    }
    
    char buffer[1024];
    
    for(int i = 0 ; i < 5; i++){
        int iret;
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer,"这是第%d个超级女生，编号%03d。", i+1, i+1);
        if((iret = send(sockfd, buffer, strlen(buffer),0)) <= 0) //想服务器发送报文
        {
            perror("send");
            break;
        }
        printf("发送：%s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
        
        if((iret = recv(sockfd, buffer, sizeof(buffer), 0)) <= 0) //接收服务端消息
        {
            printf("iret = %d\n", iret);
            break;
        }
        
        printf("接收：%s\n", buffer);
    }
    
    close(sockfd);
}











