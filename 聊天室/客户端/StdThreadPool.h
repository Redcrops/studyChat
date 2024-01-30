#ifndef _STDTHREADPOOL_H__
#define _STDTHREADPOOL_H__

struct StdThreadPool;
typedef struct StdThreadPool ThreadP;
ThreadP*InitThreadPool(int MaxThreadNumber,int MinThreadNumber,int MaxQueueSize,int second);
void AddPoolTask(ThreadP*p,void*(*funcPtr)(void*),void*arg);
void ClearThreadPool(ThreadP*p);
#endif
