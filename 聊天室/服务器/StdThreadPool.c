#include "StdThreadPool.h"
#include "StdThread.h"
#include "LinkQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "DoubleLinkList.h"
struct StdThreadPool
{
    DLlist threads;//线程链表
    LQueue* taskQueue;//任务队列
    int MaxThreadNumber;
    int MinThreadNumber;
    int MaxQueueSize;
    int BusyThreadNum;//忙碌线程数量
    Mutex*BsyThrdMtx;//忙碌线程锁
    int minusNum;//需要被退出的线程数量
    Mutex*poolMutex;//任务队列的锁(线程池的锁?)
    Cond*queueNotEmpty;
    Cond*queueNotFull;
    Thread*manager;//管理者线程
    int sleeptime;//管理者线程的检查周期
    bool shutDown;
};
typedef struct
{
    void*(*funcPtr)(void*);
    void *arg;

}Task;
Task*CreatTask(void*(*funcPtr)(void*),void*arg)
{
    Task*t=(Task*)malloc(sizeof(Task));
    if (t==NULL)
    {
        return NULL;
    }
    t->arg=arg;
    t->funcPtr=funcPtr;
    return t;
}
void FreeTask(void*t)
{
    Task*task=(Task*)t;
    free(task);
}
bool IsThreadEqual(void*ptr1,void*ptr2)
{
    Thread*t=(Thread*)ptr1;
    unsigned long int ID=(unsigned long int )ptr2;
    if (GetThreadID(t)==ID)
    {
        return true;
    }
    return false;
}
void* ThreadHandler(void*arg)
{
    ThreadP*p=(ThreadP*)arg;
    while (1)
    {
        MutexLock(p->poolMutex);
        while (IsQueueEmpty(p->taskQueue)==true&p->shutDown==false)
        {
            CondWait(p->queueNotEmpty,p->poolMutex);
            //线程自杀
            MutexLock(p->BsyThrdMtx);
            if (p->minusNum==0)
            {
                MutexUnlock(p->BsyThrdMtx);
            }
            else
            {
                RemoveByElement(&p->threads,(void*)GetSelfThrdID(),IsThreadEqual,ClearThread);
                p->minusNum--;
                MutexUnlock(p->BsyThrdMtx);
                MutexUnlock(p->poolMutex);
                //线程退出
                return NULL;
            }
            
        }
        //线程池准备退出，线程退出
        if (p->shutDown==true)
        {
            MutexUnlock(p->poolMutex);
            return  NULL;
        }
        
        //从任务队列中将任务取出
        Task*t=(Task*)*GetFront(p->taskQueue);
        QPop(p->taskQueue);
        CondBroadcast(p->queueNotFull);
        MutexUnlock(p->poolMutex);
        //准备忙任务
        MutexLock(p->BsyThrdMtx);
        p->BusyThreadNum++;
        MutexUnlock(p->BsyThrdMtx);
        //做任务
        t->funcPtr(t->arg);
        FreeTask(t);
        //线程闲下来了
        MutexLock(p->BsyThrdMtx);
        p->BusyThreadNum--;
        MutexUnlock(p->BsyThrdMtx);

    }
}
void* ThreadManager(void*arg)
{
    ThreadP*p=(ThreadP*)arg;
    while (p->shutDown==false)
    {
        sleep(p->sleeptime);
        MutexLock(p->poolMutex);
        //加线程（任务数量等于线程数量）
        if (GetQueuelen(p->taskQueue)>p->threads.len&&p->threads.len<p->MaxThreadNumber)
        {
            int MaxAddNum=p->MaxThreadNumber-p->threads.len;
            int ShouldAddNum= GetQueuelen(p->taskQueue)/2;                            
            int addNum=MaxAddNum>ShouldAddNum?ShouldAddNum:MaxAddNum;
            for (int i = 0; i < addNum; i++)
            {
                InsertDLlistTail(&p->threads,InitThread(ThreadHandler,p));
            }
        }
        //减线程
        MutexLock(p->BsyThrdMtx);
        if (p->BusyThreadNum*2<p->threads.len&&p->threads.len>p->MinThreadNumber)
        {
            int MaxMinusNum=p->threads.len-p->MinThreadNumber;
            int ShouldMinusNum=(p->threads.len-p->BusyThreadNum)/2;
            int minusNum=MaxMinusNum>ShouldMinusNum?ShouldMinusNum:MaxMinusNum;
            for (int i = 0; i < minusNum; i++)
            {
                CondSignal(p->queueNotEmpty);
            }//先叫醒再自杀
        }
        MutexUnlock(p->BsyThrdMtx);
        MutexUnlock(p->poolMutex);
    }
}
ThreadP*InitThreadPool(int MaxThreadNumber,int MinThreadNumber,int MaxQueueSize,int second)
{
    ThreadP*p=(ThreadP*)malloc(sizeof(ThreadP));
    if (p==NULL)
    {
        return NULL;
    }
    InitDList(&p->threads);
    p->taskQueue=InitLQueue();
    p->MaxQueueSize=MaxQueueSize;
    p->MaxThreadNumber=MaxThreadNumber;
    p->MinThreadNumber=MinThreadNumber;
    p->poolMutex=InitMutex();
    p->queueNotEmpty=InitCond();
    p->queueNotFull=InitCond();
    p->sleeptime=second;
    p->minusNum=0;
    p->BusyThreadNum=0;
    p->shutDown=false;
    p->BsyThrdMtx=InitMutex();
    for (int i = 0; i < p->MaxThreadNumber; i++)
    {
        InsertDLlistTail(&p->threads,InitThread(ThreadHandler,p));
    }
    p->manager=InitThread(ThreadManager,p);

    return p;
}
void AddPoolTask(ThreadP*p,void*(*funcPtr)(void*),void*arg)
{
    MutexLock(p->poolMutex);
    //任务队列满的时候
    while (GetQueuelen(p->taskQueue)==p->MaxQueueSize&&p->shutDown==false)
    {
        //等待队列不满的条件
        CondWait(p->queueNotFull,p->poolMutex);
    }
    if (p->shutDown==true)
    {
        MutexUnlock(p->poolMutex);
        return ;
    }
    QPush(p->taskQueue,CreatTask(funcPtr,arg));
    CondSignal(p->queueNotEmpty);
                
    MutexUnlock(p->poolMutex);

}
void thrdJoinCallback(void*ptr)
{
    Thread*t=(Thread*)ptr;
    ThreadJoin(t);
}
void ClearThreadPool(ThreadP*p)
{
    if (p==NULL)
    {
        return;
    }
    p->shutDown=true;
    //先回收管理者线程
    ThreadJoin(p->manager);
    free(p->manager);
    for (int i = 0; i < p->threads.len; i++)
    {
        CondSignal(p->queueNotEmpty);
    }
    //依次等待线程回收
    TravelDLlist(&p->threads,thrdJoinCallback);
    //线程列表清空
    ClearDLlist(&p->threads,ClearThread);
    //清空任务队列
    ClearLQueue(p->taskQueue,FreeTask);
    ClearMutex(p->BsyThrdMtx);
    ClearMutex(p->poolMutex);
    ClearCond(p->queueNotFull);
    ClearCond(p->queueNotEmpty);
    free(p);

    
}