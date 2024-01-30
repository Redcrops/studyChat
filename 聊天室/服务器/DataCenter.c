#include"StdSqlite.h"
#include"DataCenter.h"
#include <stdlib.h>
#include<stdio.h>
#include<sqlite3.h>
#include "DoubleLinkList.h"
#define path "/home/lvguanzhong/suqian252/chatroom/chatroom.db"
struct StdSqllite
{
    sqlite3*dp;
};
void InitDataCenter()
{
    SQL*s=InitSqlite(path);
    SqliteExec(s,"create table if not exists user(id intger primary key ,name text ,passwd text)");
    SqliteExec(s,"create table if not exists friendsship (name text,friends text)");
    SqliteExec(s,"create table if not exists groups(groupid text,name text,id integer)");
    ClearSqlite(s);
    
}
int GetMaxAccount()
{
    SQL*s=InitSqlite(path);
    MyStrList list;
    InitMyStringList(&list);
    int column=0;
    int maxID=0;
    GetTableInfo(s,"select max(id) from user;",&list,&column);
    if (list.len==0)
    {
        maxID=999;
    }
    else
    {
        maxID=atoi(list.stringList[0].string);
    }
    list.ClearMyStringList(&list);
    ClearSqlite(s);
    return maxID;
}
int SetNewAccount(const char *name,const char* passwd)
{
    SQL*s=InitSqlite(path);
    int ID=GetMaxAccount()+1;
    char sql[1024];
    sprintf(sql,"insert into user values(%d,'%s','%s')",ID,name,passwd);
    if(SqliteExec(s,sql)==false)
    {
        ID=0;
    }   
    ClearSqlite(s);
    return ID;
}
bool FindUserTure(const char *name1,const char*passwd1)
{
    SQL*s=InitSqlite(path);
    MyStrList list;
    InitMyStringList(&list);
    int column=0;
    char sq2[1024];
    sprintf(sq2,"select name  from user where name='%s' and passwd ='%s'",name1,passwd1);
    if (GetTableInfo(s,sq2,&list,&column)==true)
    {
        return true;
    }
    else
    {
        return false;
    }
    ClearSqlite(s);
    
}
bool Addfriend(const char *name1 ,const char *name2)
{
    SQL*s=InitSqlite(path);
    char sq3[1024];
    sprintf(sq3,"insert into friendsship values('%s','%s')",name1,name2);
    char sq4[1024];
    sprintf(sq4,"insert into friendsship values('%s','%s')",name2,name1);
    if (SqliteExec(s,sq3)==true&&SqliteExec(s,sq4)==true)
    {
    
        return true;
    }
    else
    {
        return false ;
    }
    ClearSqlite(s);
}
bool IsFriendExit(const char *name1 ,const char *name2)
{
    SQL*s=InitSqlite(path);
    MyStrList list;
    InitMyStringList(&list);
    int column=0;
    char sq5[1024];
    sprintf(sq5,"select name  from friendsship where name='%s' and friends ='%s'",name1,name2);
    if (GetTableInfo(s,sq5,&list,&column)==true)
    {
        
        return true;
    }
    else
    {
        return false;
    }
    ClearSqlite(s);
}
bool Deletefriend(const char *name1,const char *name2)
{
    SQL*s=InitSqlite(path);
    char sq4[1024];
    sprintf(sq4,"delete from friendsship where name='%s' and friends='%s'",name1,name2);
    char sq8[1024];
    sprintf(sq8,"delete from friendsship where name='%s' and friends='%s'",name2,name1);
    if (SqliteExec(s,sq4)==true&&SqliteExec(s,sq8)==true)
    {
    
        return true;
    }
    else
    {
        return false ;
    }
    ClearSqlite(s);

}
bool CreateGroups(const char*mygroupid,const char *name)
{
    SQL*s=InitSqlite(path);
    char sq7[1024];
    sprintf(sq7,"insert into groups values('%s','%s',%d)",mygroupid,name,2);
    if (SqliteExec(s,sq7)==true)
    {
        return true;
    }
    else
    {
        return false;
    }
    ClearSqlite(s);
}
bool JoinMyGroup(const char*mygroupid,const char *name)
{
    SQL*s=InitSqlite(path);
    char sq8[1024];
    sprintf(sq8,"insert into groups values('%s','%s',%d)",mygroupid,name,0);
    if (SqliteExec(s,sq8)==true)
    {
        return true;
    }
    else
    {
        return false;
    }
    ClearSqlite(s);
}
bool IsGroupExit(const char*mygroupid,const char *name)
{
    SQL*s=InitSqlite(path);
    MyStrList list;
    InitMyStringList(&list);
    int column=0;
    char sq5[1024];
    sprintf(sq5,"select name  from groups where groupid='%s' and name ='%s'",mygroupid,name);
    if (GetTableInfo(s,sq5,&list,&column)==true)
    {
        
        return true;
    }
    else
    {
        return false;
    }
    ClearSqlite(s);

}
bool IsInGroup(const char*mygroupid,const char* name)
{
    SQL*s=InitSqlite(path);
    MyStrList list;
    InitMyStringList(&list);
    int column=0;
    char sq5[1024];
    sprintf(sq5,"select name  from groups where groupid='%s' and name ='%s'",mygroupid,name);
    if (GetTableInfo(s,sq5,&list,&column)==true)
    {
        
        return true;
    }
    else
    {
        return false;
    }
    ClearSqlite(s);
}
DLlist FindMyGroupElement(const char *name)
{
    SQL*s=InitSqlite(path);
    DLlist list;
    InitDList(&list);
    char **result;//存储形式是指针数组
    int row=0;
    int column=0;
    char sq1 [1024];
    sprintf(sq1,"select name  from groups where groupid='%s'",name);
    if (sqlite3_get_table(s->dp,sq1,&result,&row,&column,NULL)==SQLITE_OK&&result[column]!= NULL)
    {
        for (int i = 1; i <= row; i++)
        {
                for (int j = 0; j < column; j++)
            {
                if(result[i*column+j]!=NULL);
                //printf("%s\n",result[i*column+j]);
                InsertDLlistTail(&list,result[i*column+j]);
            }
        
        }
    }
    return list;
    sqlite3_free_table(result);
    ClearSqlite(s);
}
bool IsGroupCreater(const char*mygroupid,const char* name)
{
    SQL*s=InitSqlite(path);
    MyStrList list;
    InitMyStringList(&list);
    int column=0;
    char sq5[1024];
    sprintf(sq5,"select name  from groups where groupid='%s' and name ='%s'and id=2",mygroupid,name);
    if (GetTableInfo(s,sq5,&list,&column)==true)
    {
        
        return true;
    }
    else
    {
        return false;
    }
    ClearSqlite(s);

}
void GiveYouFlag(const char *name)
{
    SQL*s=InitSqlite(path);
    char sq4[1024];
    sprintf(sq4,"update groups set id = 1 where name='%s'",name);
    SqliteExec(s,sq4);
    ClearSqlite(s);

}
bool IsGroupManager(const char*mygroupid,const char* name)
{
    SQL*s=InitSqlite(path);
    MyStrList list;
    InitMyStringList(&list);
    int column=0;
    char sq5[1024];
    sprintf(sq5,"select name  from groups where groupid='%s' and name ='%s' and id between 1 and 2",mygroupid,name);
    if (GetTableInfo(s,sq5,&list,&column)==true)
    {
        
        return true;
    }
    else
    {
        return false;
    }
    ClearSqlite(s);

}

void DeleteGroupElement (const char*mygroupid,const char *name)
{
    SQL*s=InitSqlite(path);
    char sq4[1024];
    sprintf(sq4,"delete from groups where groupid='%s' and name='%s'",mygroupid,name);
    SqliteExec(s,sq4);
    ClearSqlite(s);

}

void Dontspeack(const char*mygroupid,const char *name)
{
    SQL*s=InitSqlite(path);
    char sq4[1024];
    sprintf(sq4,"update groups set id = 4 where groupid='%s' and name='%s'and id=0",mygroupid,name);
    SqliteExec(s,sq4);
    ClearSqlite(s);
}

bool IsDontSpeak(const char*mygroupid,const char *name)
{
    SQL*s=InitSqlite(path);
    MyStrList list;
    InitMyStringList(&list);
    int column=0;
    char sq5[1024];
    sprintf(sq5,"select name  from groups where groupid='%s' and name ='%s'and id=4",mygroupid,name);
    if (GetTableInfo(s,sq5,&list,&column)==true)
    {
        
        return true;
    }
    else
    {
        return false;
    }
    ClearSqlite(s);

}