#include "StdSqlite.h"
#include <stdio.h>
#include<sqlite3.h>
#include<stdlib.h>
#include <stdbool.h>
struct StdSqllite
{
    sqlite3*dp;
};
SQL* InitSqlite(const char*filename)
{
    SQL*s=(SQL*)malloc(sizeof(SQL));
    if (s==NULL)
    {
        return NULL;
    }
    if (sqlite3_open(filename,&s->dp)!=SQLITE_OK)
    {
         printf("error msg:msg %s",sqlite3_errmsg(s->dp));
         free(s);
         return NULL;

    }
    return s;
}
bool SqliteExec(SQL*s,const char*sql)
{
    if ((sqlite3_exec(s->dp,sql,NULL,NULL,NULL)!=SQLITE_OK))
    {
        printf("error msg:msg %s",sqlite3_errmsg(s->dp));
        return false;
    }
    return true;
    
}
bool GetTableInfo(SQL*s,const char *sql,MyStrList*list,int *column)
{
    char **result;//存储形式是指针数组
    int row=0;
    if (sqlite3_get_table(s->dp,sql,&result,&row,column,NULL)!=SQLITE_OK)
    {
        printf("error msg: %s\n",sqlite3_errmsg(s->dp));
        return false;
    }
    if (result[*column]== NULL)
    {
        return false;
    }
    
    for (int i = 1; i <= row; i++)
    {
        for (int j = 0; j < *column; j++)
        {
            if(result[i*(*column)+j]!=NULL);
            list->InsertTail(list,result[i*(*column)+j]);
        }
        
    }
    sqlite3_free_table(result);
    return true;

}
void ClearSqlite(SQL*s)
{
    if (s==NULL)
    {
        return ;
    }
    sqlite3_close(s->dp);
    free(s);
    

}
