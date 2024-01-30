#include "StdThread.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//线程结构体
struct StdThread
{
    pthread_t ThreadID;
};
//初始化线程
Thread *InitThread(void *(*funcPtr)(void *), void *arg)
{
    Thread *t = (Thread*)malloc(sizeof(Thread));
    if(t == NULL)
    {
        printf("thread malloc error!\n");
        return NULL;
    }
    pthread_create(&t->ThreadID,NULL,funcPtr,arg);
    return t;
}
//线程结合
void* ThreadJoin(Thread *t)
{
    void *value;
    pthread_join(t->ThreadID,NULL);
    return value;
}
//线程分离
void ThreadDetach(Thread *t)
{
    pthread_detach(t->ThreadID);
}
//线程取消
void ThreadCancel(Thread *t)
{
    pthread_cancel(t->ThreadID);
}
//获取线程ID
unsigned long int GetThreadID(Thread *t)
{
    return t->ThreadID;
}
//清除线程
void ClearThread(void *t)
{
    if(t == NULL)
        return ;
    free(t);
}
unsigned long int GetSelfThrdID()
{
    return pthread_self();
}

//锁结构体
struct StdMutex
{
    pthread_mutex_t mutex;
};
//初始化锁
Mutex *InitMutex()
{
    Mutex *m = (Mutex*)malloc(sizeof(Mutex));
    if(m == NULL)
        return NULL;

    pthread_mutex_init(&m->mutex,NULL);
    return m;
}
//上锁
void MutexLock(Mutex *m)
{
    pthread_mutex_lock(&m->mutex);
}
//解锁
void MutexUnlock(Mutex *m)
{
    pthread_mutex_unlock(&m->mutex);
}
//销毁锁
void ClearMutex(Mutex *m)
{
    if(m == NULL)
        return ;
    pthread_mutex_destroy(&m->mutex);
    free(m);
}

//条件变量结构体
struct StdCond
{
    pthread_cond_t cond;
};
//初始化
Cond *InitCond()
{
    Cond *c = (Cond*)malloc(sizeof(Cond));
    if(c == NULL)
        return NULL;

    pthread_cond_init(&c->cond,NULL);
    return c;
}
//等待条件变量
void CondWait(Cond *c, Mutex *m)
{
    pthread_cond_wait(&c->cond,&m->mutex);
}
//接受条件变量
void CondSignal(Cond *c)
{
    pthread_cond_signal(&c->cond);
}
//广播条件变量
void CondBroadcast(Cond *c)
{
    pthread_cond_broadcast(&c->cond);
}

// 销毁条件变量
void ClearCond(Cond *c)
{
    if(c == NULL)
        return ;

    pthread_cond_destroy(&c->cond);
}










