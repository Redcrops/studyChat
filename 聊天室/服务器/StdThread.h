#ifndef __STDTHREAD_H__
#define __STDTHREAD_H__

//线程结构体
struct StdThread;
typedef struct StdThread Thread;

//初始化线程
Thread* InitThread(void*(*funcPtr)(void*),void *arg);
//线程结合
void* ThreadJoin(Thread *t);
//线程分离
void ThreadDetach(Thread *t);
//线程取消
void ThreadCancel(Thread *t);
//获取线程ID
unsigned long int GetThreadID(Thread *t);
//清除线程
void ClearThread(void *t);
//返回当前线程的线程号
unsigned long int GetSelfThrdID();

struct StdMutex;
typedef struct StdMutex Mutex;
//初始化锁
Mutex* InitMutex();
//上锁
void MutexLock(Mutex *m);
//解锁
void MutexUnlock(Mutex *m);
//销毁锁
void ClearMutex(Mutex* m);

struct StdCond;
typedef struct StdCond Cond;
//初始化
Cond* InitCond();
//等待条件变量
void CondWait(Cond *c,Mutex *m);
//
void CondSignal(Cond* c);
//
void CondBroadcast(Cond* c);
//销毁条件变量
void ClearCond(Cond *c);

#endif