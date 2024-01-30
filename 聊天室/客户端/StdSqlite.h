#ifndef _STDSQLITE_H_
#define _STDSQLITE_H_
#include<stdbool.h>
#include <MyString.h>
struct StdSqllite;
typedef  struct StdSqllite SQL;

SQL* InitSqlite(const char*filename);
bool SqliteExec(SQL*s,const char*sql);
bool GetTableInfo(SQL*s,const char *sql,MyStrList*list,int *column);
void ClearSqlite(SQL*s);
#endif