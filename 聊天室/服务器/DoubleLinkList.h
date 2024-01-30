#ifndef __DOUBLELINKLIST__H_
#define __DOUBLELINKLIST__H_
#define ElementType void*
#include<stdbool.h>
#define INITDLLIST(list)  DLlist list;InitDList(&list); 
//节点
struct DoubleLinkNode
{
    ElementType data;
    struct DoubleLinkNode *next;
    struct DoubleLinkNode *prev;
};
typedef struct DoubleLinkNode DLNode;
struct DoubleLinkList
{
    DLNode*head;
    DLNode*tail;
    int len;
 };
typedef struct DoubleLinkList DLlist  ;
void InitDList(DLlist*list);
DLNode* CreatNode(ElementType element);
void InsertDLlistTail(DLlist*list,ElementType element);
void  TravelDLlist(DLlist*list,void(*funcPtr)(ElementType));
void InsertDLlistHead(DLlist*list,ElementType element);
void RemoveByIndex(DLlist*list,int index,void(*freeptr)(ElementType));
void RemoveByElement(DLlist*list,ElementType element,bool(*Operptr)(ElementType,ElementType),void(*freeptr)(ElementType));
ElementType*FindByIndex(DLlist*list,int index);
DLlist FindByElement(DLlist*list,ElementType element,bool(*Operptr)(ElementType,ElementType));
void DLlistSort(DLlist*list,bool(*funcptr)(ElementType,ElementType));
void ClearDLlist(DLlist*list,void(*Clearptr)(ElementType));











#endif
