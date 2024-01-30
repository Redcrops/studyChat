#ifndef __STDTCP_H__
#define __STDTCP_H__
#include <stdbool.h>
#include <stddef.h>

struct StdTcpServer;
typedef struct StdTcpServer TcpS;

//初始化
TcpS* InitTcpServer(const char * ip,unsigned short port);
//接受客户端的连接
int Accept(TcpS *s);
//是否成功接收
bool TcpServerRecv(int clientSock,void *ptr,size_t size);
//是否成功发送
bool TcpServerSend(int clientSock,void *ptr,size_t size);
//清除服务器
void ClearTcpServer(TcpS *S);

struct StdTcpClient;
typedef struct StdTcpClient TcpC;
//初始化客户端
TcpC* InitTcpClient(const char * ServerIP,unsigned short ServerPort);
//
bool TcpClientRecv(TcpC *c, void *ptr, size_t size);
//
bool TcpClientSend(TcpC *c, void *ptr, size_t size);
//清除客户端
void ClearTcpClient(TcpC *c);

#endif
