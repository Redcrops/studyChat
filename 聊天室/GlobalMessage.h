#ifndef _GLOBALMESSAGE_H
#define _GLOBALMESSAGE_H
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#define NameSize 32
#define ConSize 1024
#define Maxsize 32
struct ClientInfo
{
    char account[Maxsize];//账号信息
    int sock;
};
typedef enum
{
    signUp,
    login,//登录
    chat,//私聊
    allchat,//群聊
    sendFile,//发文件
    // sendFile,
    FileStart,
    addfriend,
    agreefriend,
    deletefriend,
    creategroups,
    joingroup,
    giveflag,
    outgroup,
    dontspeack
    
}Func;
typedef enum
{
    b,
    g,
    v,
    p,
    z,
    h
}A;
struct ClientInfo;
typedef struct ClientInfo CInfo;
CInfo*CreatClientInfo(const char *account,int sock);
void ClearClientInfo(void*c);
bool IsNameEqual(void*ptr1,void*ptr2);

//全局消息
typedef struct GlobalMessage
{
    char fromName[NameSize];//发消息的人·
    char ToName[NameSize];//收消息的人
    char Content[ConSize];//消息内容
    Func flag;//功能选择
    bool IsSuccess;
    A Con;
    long fileSize;
    char pathname[128];
    int ChunkNumber;
    int BytesInchunk;
}GLMSG;



#endif