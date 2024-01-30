#ifndef _DATACENTER_H_
#define _DATACENTER_H_
#include "DoubleLinkList.h"
void InitDataCenter();
int SetNewAccount(const char *name,const char* passwd);
bool FindUserTure(const char *name1,const char*passwd1);
bool Addfriend(const char *name1 ,const char *name2);
bool IsFriendExit(const char *name1 ,const char *name2);
bool Deletefriend(const char *name1,const char *name2);
bool CreateGroups(const char*mygroupid,const char *name);
bool JoinMyGroup(const char*mygroupid,const char *name);
bool IsGroupExit(const char*mygroupid,const char *name);
bool IsInGroup(const char*mygroupid,const char* name);
DLlist FindMyGroupElement(const char *name);
bool IsGroupCreater(const char*mygroupid,const char* name);
void GiveYouFlag(const char *name);
bool IsGroupManager(const char*mygroupid,const char* name);
void DeleteGroupElement (const char*mygroupid,const char *name);
void Dontspeack(const char*mygroupid,const char *name);
bool IsDontSpeak(const char*mygroupid,const char *name);
#endif