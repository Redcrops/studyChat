#include <stdlib.h>
#include <stdio.h>
#include "LinkQueue.h"
#include "DoubleLinkList.h"
struct LinkQueue
{
    DLlist list;
};
LQueue *InitLQueue()
{
    LQueue*q=(LQueue*)malloc(sizeof(LQueue));
    if (q==NULL)
    {
        return NULL;
    }
    InitDList(&q->list);
    return q;
}
void QPush(LQueue*q,ElementType element)
{
   InsertDLlistTail(&q->list,element);

}
void QPop(LQueue*q)
{
    RemoveByIndex(&q->list,0,NULL);

}
bool IsQueueEmpty(LQueue*q)
{
    return q->list.len==0;
}
void ClearLQueue(LQueue*q,void(*Clearptr)(ElementType))
{
    ClearDLlist(&q->list,Clearptr);
}
ElementType *GetFront(LQueue*q)
{
   return FindByIndex(&q->list,0);
}
void TravelLQueue(LQueue*q,void(*funcptr)(ElementType))
{
     TravelDLlist(&q->list,funcptr);

}
int GetQueuelen(LQueue*q)
{
    return q->list.len;
}