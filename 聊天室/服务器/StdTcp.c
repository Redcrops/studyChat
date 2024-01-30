#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "StdThread.h"
#include <unistd.h>
#include "StdTcp.h"
//服务器结构体
struct StdTcpServer 
{
    int sock;
};
//初始化
TcpS *InitTcpServer(const char *ip, unsigned short port)
{
    TcpS* s = (TcpS*)malloc(sizeof(TcpS));
    if(s == NULL)
        return NULL;

    s->sock = socket(AF_INET,SOCK_STREAM,0);
    if(s->sock < 0)
    {
        perror("socket");
        free(s);
        return NULL;
    }
    //设置端口复用
    int use=1;
    setsockopt(s->sock,SOL_SOCKET,SO_REUSEADDR,&use,sizeof(use));

     //设置服务器地址和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    //绑定套接字到本地地址和端口
    if(bind(s->sock,(struct sockaddr*)&addr,sizeof(addr)) < 0)
    {
        perror("bind");
        close(s->sock);
        free(s);
        return NULL;
    }
    //监听函数：确定连接
    //面试常问//第二个参数：最大等待队列。
    if(listen(s->sock,10) < 0)
    {
        perror("listen");
        close(s->sock);
        free(s);
        return NULL;
    }
    return s;
}
//接收客户端的连接
int Accept(TcpS *s)
{
    struct sockaddr_in ClientAddr;
    socklen_t len = sizeof(ClientAddr);
    //将已经建立连接的客户端，从等待队列中的清除
    int clientSock = accept(s->sock,(struct sockaddr*)&ClientAddr,&len);
    printf("连接进来的客户端socket:%d 地址：%s 端口%d\n",
            clientSock,inet_ntoa(ClientAddr.sin_addr),ntohs(ClientAddr.sin_port));
    
    return clientSock;
}
//是否成功接收
bool TcpServerRecv(int clientSock, void *ptr, size_t size)
{
    if(recv(clientSock,ptr,size,0) <= 0)
        return false;
    return true;
}
//是否成功发送
bool TcpServerSend(int clientSock, void *ptr, size_t size)
{
    if(send(clientSock,ptr,size,0) <= 0)
        return false;
    return true;
}
//清除服务器
void ClearTcpServer(TcpS *s)
{
    if(s == NULL)
        return ;
    close(s->sock);
    free(s);
}
//客户端结构体
struct StdTcpClient
{
    int sock;
};
//初始化客户端
TcpC *InitTcpClient(const char *ServerIP, unsigned short ServerPort)
{
    TcpC* c = (TcpC*)malloc(sizeof(TcpC));
    if(c == NULL)
        return NULL;

    c->sock = socket(AF_INET,SOCK_STREAM,0);
    if(c->sock < 0)
    {
        perror("socket");
        free(c);
        return NULL;
    }

    //设置服务器地址和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ServerPort);
    addr.sin_addr.s_addr = inet_addr(ServerIP);
    //连接服务器
    if(connect(c->sock,(struct sockaddr*)&addr,sizeof(addr)) < 0)
    {
        perror("connect");
        close(c->sock);
        free(c);
        return NULL;
    }
    printf("客户端连接成功!\n");
    return c;
}
bool TcpClientRecv(TcpC *c, void *ptr, size_t size)
{
    if(recv(c->sock,ptr,size,0) <= 0)
        return false;
    return true;
}

bool TcpClientSend(TcpC *c, void *ptr, size_t size)
{
    if(send(c->sock,ptr,size,0) <= 0)
        return false;
    return true;
}

//清除客户端
void ClearTcpClient(TcpC *c)
{
    if(c == NULL)
        return ;
    close(c->sock);
    free(c);
}
