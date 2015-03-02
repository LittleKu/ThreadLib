#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <vector>
#include <map>

#include "Thread.h"
#include "Mutex.h"
#include "Event.h"

#include <stddef.h>

namespace ThreadLib
{
	class MemPool;

	typedef struct THREAD_CONTEXT
	{
		Thread thread;	
		bool bIdle;
		bool bRun;
	}THREAD_CONTEXT;

	typedef std::map<DWORD,THREAD_CONTEXT*> ThreadMaps;
	typedef std::vector<Task*> TaskVec;

	class ThreadPool
	{
	public:
		ThreadPool();
		~ThreadPool();

		bool Start(int nMinThreadNum);
		void Stop();

		/**< 向线程池添加一个新的任务,用于类成员函数*/
		void PushTask(DWORD pfn, DWORD pClassBase, DWORD pParam);

		/**< 向线程池添加一个新的任务,用于非类成员函数*/
		void PushTask(DWORD pfn, DWORD pParam);

	private:
		/**< 创建线程池*/
		bool CreateThread(unsigned int nNum);

		/**< 所有线程的回调函数*/
		void* ThreadFunc(void* pParam);

		/**< 创建一个线程上下文*/
		THREAD_CONTEXT* CreateContext();

		/**< 回收一个线程上下文*/
		void ReleaseContext(THREAD_CONTEXT *pContext);
		
		/**< 从内存池中申请并创建一个任务*/
		Task* CreateTask();

		/**< 回收一个任务内存*/
		void ReleaseTask(Task *pTask);

		/**< 把一个有效内存压入等待执行的任务序列中*/
		void PushTask(Task* pTask);

		/**< 从待执行任务序列中取出一个有效的任务*/
		Task* PullTask();

	protected:
		MemPool* m_pTaskPool;
		Mutex m_taskPoolMutex;
		MemPool* m_pContextPool;
		Mutex m_contextPoolMutex;

		unsigned short m_nMinThreadNum;
		unsigned short m_nThreadNum;
		ThreadMaps m_threads;//线程表
		Mutex m_threadsMutex;//线程表线程安全锁
		TaskVec m_tasks;//任务表
		Mutex m_tasksMutex;//任务表线程安全锁
		Event m_EventNewTask;//新任务信号
	};
}

#endif