//理解UDP和TCP的区别（关于有没有连接）
#include <stdio.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<StdFile.h>
#include<time.h>
#define IP "192.168.59.129"
#include "StdTcp.h"
#include "StdThread.h"
#include "DataCenter.h"
#include "StdThreadPool.h"
#include "../GlobalMessage.h"
#include"LinkQueue.h"
#include<unistd.h>
#define PORT 7071
char Account [NameSize]={0};
char Account1[NameSize]={0};
char Account2[NameSize]={0};
int a=0;
void SignUp(TcpC*c)
{
    GLMSG msg;
    msg.flag=signUp;
    printf("请输入你要的用户名：");
    scanf("%s",msg.fromName);
    while(getchar() != '\n');
    printf("请输入你的密码：");
    scanf("%s",msg.Content);
    while(getchar() != '\n');
    strcpy(Account,msg.fromName);
    TcpClientSend(c,&msg,sizeof(msg));
}
void Login(TcpC *c)
{
    GLMSG msg;
    msg.flag = login;
    printf("请输入你的账号:");
    scanf("%s",msg.fromName);
    while(getchar() != '\n');
    printf("请输入你的密码：");
    scanf("%s",msg.Content);
    while(getchar() != '\n');
    strcpy(Account,msg.fromName);
    TcpClientSend(c,&msg,sizeof(msg));
}

void* Thread_Read(void*arg)
{
    TcpC *c = (TcpC*)arg;
    while(1)
    {
        GLMSG m;
        if(TcpClientRecv(c,&m,sizeof(m)) == false)
        {
            printf("与服务器断开连接!\n");
            exit(-1);
            break;
        }
        if (m.flag==signUp)
        {
            if (m.Con==p)
            {
                printf("注册失败,你的账号密码或许已经被注册过了,请输入新的账号密码\n");
                exit(0);
            }
            if (m.Con==h)
            {
                printf("注册失败\n");
                exit(0);
            }
            if (m.Con==z)
            {
                printf("注册成功\n");
            }
        }
        if (m.flag==login)
        {
            if (m.Con==p)
            {
                printf("登录成功\n");
                
            }
            if (m.Con==v)
            {
                printf("登录失败\n");
                exit(0);
            }
            if(m.Con==h)
            {
                printf("你的账号已经登录,你不能登录\n");
                exit(0);
            }
            
        }
        if(m.flag == FileStart)
        {
           FILE*file=fopen("./temp.mp4","wb");
            if (file==NULL)
            {
                printf("无法创建文件\n");
                return NULL;
            }
            //WriteToFile("./temp.txt",m.Content,strlen(m.Content));
        }
        if(m.flag == sendFile)
        {
            AppendToFile("./temp.mp4",m.Content,/*strlen(m.Content)*/m.BytesInchunk);
        }
        if (m.flag==joingroup)
        {
            if (m.Con==b)
            {
               printf("没有找到这个群聊\n");
            }
            if (m.Con==g)
            {
                printf("对方已经在群聊中了,你无需拉\n");
            }
            if (m.Con==v)
            {
                printf("你被|%s|拉入群|%s|中\n",m.fromName,m.Content);
            }
            
        }
        if (m.flag==addfriend)
        {
            if (m.IsSuccess==false)
            {
                printf("你已经添加了该好友,不能进行添加\n");
            }
            if(m.IsSuccess==true)
            {
                printf("接收到来自|%s|的好友验证消息:%s\n",m.fromName,m.Content);
                printf("请按5选择是否添加他为好友\n");
                strcpy(Account1,m.fromName);
                a=1;
            }
        }
        if (m.flag==agreefriend)
        {
            if (m.Con==h)
            {
                printf("添加好友失败,你可能已经添加过对方了\n");
            }
            if (m.Con==z)
            {
                printf("添加好友成功了\n");
            }
            
        }
        if (m.flag==deletefriend)
        {
           if (m.IsSuccess==false)
            {
                printf("删除好友失败,你和对方本来就不是好友\n");
            }
            if (m.IsSuccess==true)
            {
                printf("删除好友成功了\n");
            }
        }
        if (m.flag==giveflag)
        {
            if (m.Con==b)
            {
                printf("你不是群主不能设置管理员\n");
            }
            if (m.Con==g)
            {
                printf("群里没有找到|%s|\n",m.ToName);
            }
            if (m.Con==v)
            {
                printf("对方已经是管理员了,不能添加为管理\n");
            }
            if (m.Con==z)
            {
                printf("你被|%s|设置为|%s|的管理员\n",m.fromName,m.Content);
            }
        }
        
        if (m.flag==chat)
        {
            if (m.Con==b)
            {
                 printf("对方不是你的好友,你不能给他发信息\n");
            }
            if (m.Con==z)
            {
                printf("接收到来自|%s|的消息:%s\n",m.fromName,m.Content);
                time_t t=time(NULL);
                struct tm*k=gmtime(&t);
                char str3[100]={0};
                sprintf(str3,"%d年%d月%d日 %d时%d分%d秒",k->tm_year+1900,k->tm_mon+1,k->tm_mday,(k->tm_hour+8)%24,k->tm_min,k->tm_sec);
                AppendToFile("聊天记录.txt",str3,strlen(str3));
                char str2[2]={0};
                strcpy(str2,"\n");
                AppendToFile("聊天记录.txt",str2,strlen(str2));
                AppendToFile("聊天记录.txt",m.fromName,strlen(m.fromName));
                char str[2]={0};
                strcpy(str,":");
                AppendToFile("聊天记录.txt",str,strlen(str));
                AppendToFile("聊天记录.txt",str2,strlen(str2));
                AppendToFile("聊天记录.txt",m.Content,strlen(m.Content));
                AppendToFile("聊天记录.txt",str2,strlen(str2));
            }
            
        }
        if (m.flag==allchat)
        {
            if (m.Con==b)
            {
                printf("你被禁言了,不能发消息\n");
            }
            if (m.Con==z)
            {
                printf("接收到来自|%s|的群发消息:%s\n",m.fromName,m.Content);
                time_t t=time(NULL);
                struct tm*k=gmtime(&t);
                char str3[100]={0};
                sprintf(str3,"%d年%d月%d日 %d时%d分%d秒",k->tm_year+1900,k->tm_mon+1,k->tm_mday,(k->tm_hour+8)%24,k->tm_min,k->tm_sec);
                AppendToFile("聊天记录.txt",str3,strlen(str3));
                char str2[2]={0};
                strcpy(str2,"\n");
                AppendToFile("聊天记录.txt",str2,strlen(str2));
                AppendToFile("聊天记录.txt",m.ToName,strlen(m.ToName));
                AppendToFile("聊天记录.txt",str2,strlen(str2));
                AppendToFile("聊天记录.txt",m.fromName,strlen(m.fromName));
                char str[2]={0};
                strcpy(str,":");
                AppendToFile("聊天记录.txt",str,strlen(str));
                AppendToFile("聊天记录.txt",str2,strlen(str2));
                AppendToFile("聊天记录.txt",m.Content,strlen(m.Content));
                AppendToFile("聊天记录.txt",str2,strlen(str2));

            }
            
        }
        if (m.flag==outgroup)
        {
            if (m.Con==b)
            {
                printf("没有找到这个群聊\n");
            }
            if (m.Con==g)
            {
               printf("你不是群管理或者群主,不能进行踢人\n");
            }
            if (m.Con==v)
            {
                printf("群里没有找到|%s|\n",m.ToName);
            }
            if (m.Con==p)
            {
                printf("踢人失败,你要踢的人是本群的群主或者其他管理员\n");
            }
            if (m.Con==z)
            {
                printf("你已经被移除群聊|%s|\n",m.Content);
            }
         }
        if (m.flag==dontspeack)
        {
            if ( m.Con==b)
            {
                printf("没有找到这个群聊\n");
            }
            if (m.Con==g)
            {
                printf("你不是群管理或者群主,不能进行禁言\n");
            }
            if (m.Con==v)
            {
                printf("群里没有找到|%s|\n",m.ToName);
            }
            if (m.Con==h)
            {
                printf("禁言失败,你要禁言的人是本群的群主或者其他管理员\n");
            }
            if (m.Con==z)
            {
                printf("你被|%s|在|%s|禁言了\n",m.fromName,m.Content);
            }
            
           
        }
        
        
    }
}

void ChooseSelect(GLMSG *m,TcpC *c)
{
    printf("请输入选项:1、私聊 2、群发消息 3、发文件 4、加好友 5、是否同意加好友 6、删好友 7、建群 8、拉人入群 9、设置群管理员 10、踢人 11、设置禁言\n");
    int choice;
    scanf("%d",&choice);
    while(getchar() != '\n');
    switch (choice)
    {
    case 1:
        m->flag = chat;
        printf("输入你要发消息的账号:");
        scanf("%s",m->ToName);
        printf("请输入你要发的内容:");
        scanf("%s",m->Content);
        while(getchar() != '\n');
        TcpClientSend(c,m,sizeof(GLMSG));
       
        break; 
    case 2:
        m->flag = allchat;
        printf("请输入你要群发的群聊名称：");
        scanf("%s",m->ToName);
        while(getchar() != '\n');
        printf("请输入你要群发的内容:");
        scanf("%s",m->Content);
        while(getchar() != '\n');
        TcpClientSend(c,m,sizeof(GLMSG));
        break;
    case 3:
        printf("请输入你要发送的文件路径:");
        char path[512] = {0};
        scanf("%s",path);
        while(getchar() != '\n');
        printf("输入你要发文件的账号:");
        scanf("%s",m->ToName);
        while(getchar() != '\n');
        FILE *f = fopen(path,"rb");
        if(f == NULL)
        {
            printf("文件打开失败!\n");
            return;
        }
        fseek(f,0,SEEK_END);
        long fileSize=ftell(f);
        fseek(f,0,SEEK_SET);
        m->flag = FileStart;
        m->fileSize=fileSize;
        strcpy(m->pathname,path);
        //fread(m->Content,sizeof(m->Content),1,f);
        TcpClientSend(c,m,sizeof(GLMSG));
        char buffer[1024];
        size_t bytesRead;
        int chunkCount=0;
        // while(feof(f) == 0 && bytesRead=fread(buffer,1,sizeof(buffer),f)>0)
        // {

        // }
        
        while(feof(f) == 0 &&(bytesRead=fread(buffer,1,sizeof(buffer),f))>0)
        {
            m->flag = sendFile;
            m->ChunkNumber=chunkCount++;//发的包数
            //memset(m->Content,0,sizeof(m->Content));
            memcpy(m->Content,buffer,bytesRead);
            m->BytesInchunk=bytesRead;
            //fread(m->Content,sizeof(m->Content),1,f);

            TcpClientSend(c,m,sizeof(GLMSG));
            usleep(10000);
        }
        fclose(f);
        break;
    case 4:
    m->flag=addfriend;
    printf("请输入你要加的好友名称:");
    scanf("%s",m->ToName);
    while(getchar() != '\n');
    printf("请输入你要向该好友发送的验证信息:");
    scanf("%s",m->Content);
    while(getchar() != '\n');
    TcpClientSend(c,m,sizeof(GLMSG));
    break;
    case 5:
    m->flag=agreefriend;
    if (a==0)
    {
        printf("目前没有人向你发送好友申请,你不能加好友\n");
        break;
    }
    else
    {
        printf("是否选择添加|%s|为好友\n",Account1);
        printf("请做出选择1.同意2.不同意\n");
        int flag=0;
        scanf("%d",&flag);
        while(getchar() != '\n');
        if (flag==1)
        {
            sprintf(m->ToName,"%s",Account1);
            TcpClientSend(c,m,sizeof(GLMSG));
            break;
        }
        if (flag==2)
        {
            printf("你不同意添加对方为好友\n");
            break;
        }
    }
    case 6:
    m->flag=deletefriend;
    printf("请输入你要删除的好友名称:");
    scanf("%s",m->ToName);
    while(getchar() != '\n');
    TcpClientSend(c,m,sizeof(GLMSG));
    break;
    case 7:
    m->flag=creategroups;
    printf("请输入你要创建的群聊名称:\n");
    scanf("%s",m->Content);
    while(getchar() != '\n');
    //printf("%s\n",m->fromName);
    TcpClientSend(c,m,sizeof(GLMSG));
    break;
    case 8:
    m->flag=joingroup;
    printf("请输入你要邀请别人加入的群聊\n");
    scanf("%s",m->Content);
    while(getchar() != '\n');
    printf("你要邀请加入群聊的人\n");
    scanf("%s",m->ToName);
    while(getchar() != '\n');
    TcpClientSend(c,m,sizeof(GLMSG));
    break;
    case 9:
    m->flag=giveflag;
    printf("请输入你要设置管理员的群聊\n");
    scanf("%s",m->Content);
    while(getchar() != '\n');
    printf("你要添加的管理员是谁\n");
    scanf("%s",m->ToName);
    while(getchar() != '\n');
    TcpClientSend(c,m,sizeof(GLMSG));
    break;
    case 10:
    m->flag=outgroup;
    printf("你要移除人的群聊：\n");
    scanf("%s",m->Content);
    while(getchar() != '\n');
    printf("你要踢的人是谁\n");
    scanf("%s",m->ToName);
    while(getchar() != '\n');
    TcpClientSend(c,m,sizeof(GLMSG));
    break;
    case 11:
    m->flag=dontspeack;
    printf("请输入你要禁言人的群聊\n");
    scanf("%s",m->Content);
    while(getchar() != '\n');
    printf("你要禁言的人是谁\n");
    scanf("%s",m->ToName);
    while(getchar() != '\n');
    TcpClientSend(c,m,sizeof(GLMSG));
    break;
    default:
        break;
    }
   
}
void SendMsg(TcpC *c)
{
    while(1)
    {
        GLMSG m;
        strcpy(m.fromName,Account);
        ChooseSelect(&m,c);
        
    }
}
int main()
{
    TcpC *c = InitTcpClient(IP,PORT);
    printf("请选择功能1.注册2.登录");
    int choice;
    scanf("%d",&choice);
    while(getchar() != '\n');
    switch (choice)
    {
    case 1:
    SignUp(c);
        break;
    case 2:
    Login(c);
    default:
        break;
    }
    Thread *readT = InitThread(Thread_Read,c);//开多线程来收信息，防止堵塞
    SendMsg(c);
    return 0;
}


