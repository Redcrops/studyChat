#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include "StdThread.h"
#include "StdTcp.h"
#include "../GlobalMessage.h"
#include"LinkQueue.h"
#include "DoubleLinkList.h"
#include "StdThreadPool.h"
#include "DataCenter.h"
#define IP "192.168.59.129"
#define PORT 7071
DLlist ClientList;
void ChooseSelect(GLMSG *m,int sock)
{
    switch (m->flag)
    {
        case signUp:
        if (FindUserTure(m->fromName,m->Content)==true)
        {
            m->Con=p;
            printf("该账号密码已经被注册了,注册失败\n");
        }
        else
        {
            int newID=SetNewAccount(m->fromName,m->Content);
            if (newID==0)
            {
                m->Con=h;
                printf("注册失败\n");
            }
            else
            {
                m->Con=z;
                memset(m->Content,0,sizeof(m->Content));
                sprintf(m->Content,"%d",newID);
                printf("注册成功\n");
            }
        }
        TcpServerSend(sock,m,sizeof(GLMSG));
        break;
        case login:
        if (FindUserTure(m->fromName,m->Content)==true)
        {
            
            InsertDLlistTail(&ClientList,CreatClientInfo(m->fromName,sock));
            DLlist listw =  FindByElement(&ClientList,m->fromName,IsNameEqual);
            if (listw.len==1)
            {
                m->Con=p;
                printf("恭喜成功登录:账号:%s 套接字:%d\n",m->fromName,sock);
            }
            else
            {
                m->Con=h;
                printf("该账号已经登录,不能重复登录\n");
            }
        }
        else
        {
            m->Con=v;
            printf("登录失败\n"); 
        }
            //InsertDLlistTail(&ClientList,CreatClientInfo(m->fromName,sock));
            //printf("登录了一个新的客户端:账号:%s 套接字:%d\n",m->fromName,sock);
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        case chat:
            if (IsFriendExit(m->fromName,m->ToName)==false)
            {
                m->Con=b;
                printf("私聊失败\n");
                TcpServerSend(sock,m,sizeof(GLMSG));
                break;
            }
            DLlist list =  FindByElement(&ClientList,m->ToName,IsNameEqual);
            if(list.len == 0)
                printf("没找到相应的客户端:%s\n",m->ToName);
            else
            {
                m->Con=z;
                //转发
                CInfo *c = (CInfo*)list.head->next->data;
                TcpServerSend(c->sock,m,sizeof(GLMSG));
            }
            ClearDLlist(&list,NULL);
            break;
        case allchat:
            if (IsDontSpeak(m->ToName,m->fromName)==true)
            {
                m->Con=b;
                TcpServerSend(sock,m,sizeof(GLMSG));
                printf("群发消息失败\n");
                break;
            }
            //除自己之外每个人都发
            DLlist listc=FindMyGroupElement(m->ToName);
            DLlist lista;
            DLNode* travelPoint = listc.head->next;
            while(travelPoint != NULL)
            {
                //printf("%s\n",(char*)travelPoint->data);
             lista =  FindByElement(&ClientList,travelPoint->data,IsNameEqual);
            DLNode*travelPoint1=lista.head->next;
            while (travelPoint1!=NULL)
            {
                CInfo *c = (CInfo*)travelPoint1->data;
                if(strcmp(c->account,m->fromName) != 0)
                {
                    m->Con=z;
                    TcpServerSend(c->sock,m,sizeof(GLMSG));
                }
                travelPoint1 = travelPoint1->next;
            }
                travelPoint = travelPoint->next;
            }
            ClearDLlist(&listc,NULL);
            ClearDLlist(&lista,NULL);
            break;
        case FileStart:
        case sendFile:
            DLlist listF =  FindByElement(&ClientList,m->ToName,IsNameEqual);
            if(listF.len == 0)
                printf("没找到相应的客户端:%s\n",m->ToName);
            else
            {
                //转发
                CInfo *c = (CInfo*)listF.head->next->data;
                TcpServerSend(c->sock,m,sizeof(GLMSG));
            }
            ClearDLlist(&listF,NULL);
            break;
        case addfriend:
        if (IsFriendExit(m->fromName,m->ToName)==true)
        {
            m->IsSuccess=false;
            TcpServerSend(sock,m,sizeof(GLMSG));
            
        }
        else
        {
            m->IsSuccess=true;
            DLlist listk =  FindByElement(&ClientList,m->ToName,IsNameEqual);
            if(listk.len == 0)
                    printf("没找到相应的客户端:%s\n",m->ToName);
            else
                {
                    //转发
                    CInfo *c = (CInfo*)listk.head->next->data;
                    TcpServerSend(c->sock,m,sizeof(GLMSG));
                }
                ClearDLlist(&listk,NULL);
        }
        break;
        case agreefriend:
        if (IsFriendExit(m->fromName,m->ToName)==true)
        {
             m->Con=h;
             printf("添加好友失败\n");
             TcpServerSend(sock,m,sizeof(GLMSG));
             break;
        }
        if (Addfriend(m->ToName,m->fromName)==true)
        {
            m->Con=z;
            printf("添加好友成功\n");
        }
        TcpServerSend(sock,m,sizeof(GLMSG));
        break;
        case deletefriend:
        if (IsFriendExit(m->fromName,m->ToName)==false)
        {
             m->IsSuccess=false;
             printf("删除好友失败\n");
             TcpServerSend(sock,m,sizeof(GLMSG));
             break;
        }
        if (Deletefriend(m->fromName,m->ToName)==true)
        {
            m->IsSuccess=true;
            printf("删除好友%s成功\n",m->ToName);
            TcpServerSend(sock,m,sizeof(GLMSG));
        }
        break;
        case creategroups:
        if (CreateGroups(m->Content,m->fromName)==true)
        {
            //printf("%s\n",m->fromName);
            m->IsSuccess=true;
            printf("成功建立群聊%s\n",m->Content);
        }
        else
        {
            m->IsSuccess=false;
            printf("创建群聊失败\n");
        }
        break;
        case joingroup:
        if (IsGroupExit(m->Content,m->fromName)==false)
        {
            m->Con=b;
            printf("拉群失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        if (IsInGroup(m->Content,m->ToName)==true)
        {
            m->Con=g;
            printf("拉群失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        DLlist listo =  FindByElement(&ClientList,m->ToName,IsNameEqual);
        if(listo.len == 0)
        {
             m->Con=p;
            printf("没找到相应的客户端:%s\n",m->ToName);

        }
        else
        {
            if (JoinMyGroup(m->Content,m->ToName)==true)
            {
                m->Con=v;
                printf("把%s拉进群成功\n",m->ToName);
                //转发
                CInfo *c = (CInfo*)listo.head->next->data;
                TcpServerSend(c->sock,m,sizeof(GLMSG));

            }
        }
        ClearDLlist(&listo,NULL);
        break;
        case giveflag:
        if (IsGroupCreater(m->Content,m->fromName)==false)
        {
            m->Con=b;
            printf("设置管理员失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        if (IsInGroup(m->Content,m->ToName)==false)
        {
            m->Con=g;
            printf("设置管理员失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        if (IsGroupManager(m->Content,m->ToName)==true)
        {
            m->Con=v;
            printf("设置管理员失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        DLlist listm =  FindByElement(&ClientList,m->ToName,IsNameEqual);
        if(listm.len == 0)
        {
            m->Con=p;
            printf("没找到相应的客户端:%s\n",m->ToName);
        }
        else
        {
            GiveYouFlag(m->ToName);
            m->Con=z;
            printf("设置管理员成功\n");
            CInfo *c = (CInfo*)listm.head->next->data;
            TcpServerSend(c->sock,m,sizeof(GLMSG));
        }
        ClearDLlist(&listm,NULL);
        break;
        case outgroup:
        if (IsGroupExit(m->Content,m->fromName)==false)
        {
            m->Con=b;
            printf("踢人失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        if (IsGroupManager(m->Content,m->fromName)==false)
        {
            m->Con=g;
            printf("踢人失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        if (IsInGroup(m->Content,m->ToName)==false)
        {
            m->Con=v;
            printf("踢人失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        if (IsGroupManager(m->Content,m->ToName)==true)
        {
            m->Con=p;
            printf("踢人失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        DLlist listg=FindByElement(&ClientList,m->ToName,IsNameEqual);
        if(listg.len == 0)
        {
            m->Con=h;
            printf("没找到相应的客户端:%s\n",m->ToName);
        }
        else
        {
            DeleteGroupElement(m->Content,m->ToName);
            m->Con=z;
            printf("移除群成员|%s|成功\n",m->ToName);
            CInfo *c = (CInfo*)listg.head->next->data;
            TcpServerSend(c->sock,m,sizeof(GLMSG));
        }
        ClearDLlist(&listg,NULL);
        break;
        case dontspeack:
        if (IsGroupExit(m->Content,m->fromName)==false)
        {
            m->Con=b;
            TcpServerSend(sock,m,sizeof(GLMSG));
            printf("禁言失败\n");
            break;
        }
        if (IsGroupManager(m->Content,m->fromName)==false)
        {
            m->Con=g;
            printf("禁言失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        if (IsInGroup(m->Content,m->ToName)==false)
        {
            m->Con=v;
            printf("禁言失败\n");
            TcpServerSend(sock,m,sizeof(GLMSG));
            break;
        }
        if (IsGroupManager(m->Content,m->ToName)==true)
        {
            m->Con=h;
            TcpServerSend(sock,m,sizeof(GLMSG));
            printf("禁言失败\n");
            break;
        }
        DLlist listi=FindByElement(&ClientList,m->ToName,IsNameEqual);
        if(listi.len == 0)
        {
            m->Con=p;
            printf("没找到相应的客户端:%s\n",m->ToName);
        }
        else
        {
            Dontspeack(m->Content,m->ToName);
            m->Con=z;
            printf("设置禁言成功\n");
            CInfo *c = (CInfo*)listi.head->next->data;
            TcpServerSend(c->sock,m,sizeof(GLMSG));
        }
        ClearDLlist(&listi,NULL);
        break;
        default:
            break;
    }
}

void* Thread_Handler(void *arg)
{
    int clientSock = *(int*)arg;
    while(1)
    {
        GLMSG m;
        if(TcpServerRecv(clientSock,&m,sizeof(m)) == false)
        {
            printf("客户端已下线!\n");
            return NULL;
        }
        ChooseSelect(&m,clientSock);
    }
    close(clientSock);
}

int main()
{
    TcpS *s = InitTcpServer(IP,PORT);
    InitDataCenter();
    InitDList(&ClientList);
    ThreadP*p=InitThreadPool(10,20,10,60);

    //阻塞函数
    int clientSock = 0;
    while( (  clientSock = Accept(s) ) >= 0)
    {
        AddPoolTask(p,Thread_Handler,(void*)&clientSock);//服务器不收信息，只实现转发或者实现对客户端的处理
        //所有的功能都在服务器实现
    }
    ClearThreadPool(p);
   
    return 0;
}