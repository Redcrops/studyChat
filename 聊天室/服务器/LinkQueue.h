#ifndef __LINKQUEUE_H__
#define __LINKQUEUE_H__
#include "DoubleLinkList.h"
struct LinkQueue;
typedef struct LinkQueue LQueue;
LQueue *InitLQueue();
//入队列
void QPush(LQueue*q,ElementType element);
//出队列
void QPop(LQueue*q);
//判断队列是否为空
bool IsQueueEmpty(LQueue*q);
//清空队列
void ClearLQueue(LQueue*q,void(*Clearptr)(ElementType));
//获取对头元素
ElementType *GetFront(LQueue*q);
void TravelLQueue(LQueue*q,void(*funcptr)(ElementType));
//获取队列数量
int GetQueuelen(LQueue*q);
#endif


