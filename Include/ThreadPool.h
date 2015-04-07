#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <vector>
#include <map>

#include "CodeAddress.h"
#include "MemPool.h"
#include "Thread.h"
#include "Mutex.h"
#include "Event.h"

#include <stddef.h>

namespace mt
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
		ThreadPool()
			:m_nMinThreadNum(0)
			,m_nThreadNum(0)
		{
			m_pTaskPool = new MemPool(sizeof(Task), 100);
			m_pContextPool = NULL;
		}
		~ThreadPool()
		{
			Stop();
			if (m_pTaskPool != NULL)
			{
				delete m_pTaskPool;
				m_pTaskPool = NULL;
			}

			if (m_pContextPool != NULL)
			{
				delete m_pContextPool;
				m_pContextPool = NULL;
			}
		}

		bool Start(int nMinThreadNum)
		{
			m_nMinThreadNum = nMinThreadNum;
			m_pContextPool = new MemPool(sizeof(THREAD_CONTEXT), 50);
			return CreateThread(m_nMinThreadNum);
		}
		void Stop()
		{
			AutoLock l(&m_threadsMutex);
			AutoLock lock(&m_tasksMutex);
			m_tasks.clear();

			ThreadMaps::iterator it = m_threads.begin();

			for (;it != m_threads.end(); it++)
			{
				it->second->bRun = false;
			}
			m_EventNewTask.Set();

			for (it = m_threads.begin(); it != m_threads.end(); it++)
			{
				it->second->thread.Stop(3000);
				ReleaseContext(it->second);
			}
			m_threads.clear();
			m_nThreadNum = 0;
		}

		/**< 向线程池添加一个新的任务,用于类成员函数*/
		void PushTask(DWORD pfn, DWORD pClassBase, DWORD pParam)
		{
			Task *pTask = CreateTask();
			pTask->PushTask(pfn, pClassBase, pParam);
			_PushTask(pTask);
		}

		/**< 向线程池添加一个新的任务,用于非类成员函数*/
		void PushTask(DWORD pfn, DWORD pParam)
		{
			Task *pTask = CreateTask();
			pTask->PushTask(pfn, pParam);
			_PushTask(pTask);
		}

	private:
		/**< 创建线程池*/
		bool CreateThread(unsigned int nNum)
		{
			AutoLock l(&m_threadsMutex);
			if (nNum <= 0)
				nNum = m_nMinThreadNum;

			if (m_nThreadNum + nNum < m_nMinThreadNum)
				nNum = m_nMinThreadNum - m_nThreadNum;

			THREAD_CONTEXT *pContext;
			for (unsigned int i = 0; i < nNum; i++)
			{
				pContext = CreateContext();
				pContext->bIdle = true;
				pContext->bRun = true;
				pContext->thread.Run((DWORD)GET_MEMBER_CALLBACK(ThreadPool,ThreadFunc), (DWORD)this, (DWORD)pContext);
				m_threads.insert(ThreadMaps::value_type(pContext->thread.GetThreadID(), pContext));
			}
			m_nThreadNum += nNum;
			return true;
		}

		/**< 所有线程的回调函数*/
		void* ThreadFunc(void* pParam)
		{
			THREAD_CONTEXT *pContext = (THREAD_CONTEXT*)pParam;
			Task *pTask = NULL;

			while (pContext->bRun)
			{
				if (!pContext->bRun)/**< 注意切换*/
					break;

				pContext->bIdle = false;/**< 线程进入忙碌状态*/

				while (pContext->bRun)/**< 这里没必要加锁,所以判断会多点*/
				{
					pTask = PullTask();/**< 取任务时已有加锁*/
					if (pTask == NULL)
						break;

					pTask->ExecuteTask();/**< 执行任务*/
					ReleaseTask(pTask);/**< 释放回收任务内存*/
				}

				pContext->bIdle = true;/**< 线程已经工作完毕,进入空闲状态,等待下一次的任务分配*/

				if (!pContext->bRun)/**< 要求当前线程停止工作*/
					break;

				if (!m_EventNewTask.WaitEvent())/**< 没有新任务信号*/
					continue;
			}

			m_EventNewTask.Set();/**< 此处通知下一个线程退出*/
			return NULL;
		}

		/**< 创建一个线程上下文*/
		THREAD_CONTEXT* CreateContext()
		{
			AutoLock l(&m_contextPoolMutex);
			THREAD_CONTEXT *pContext = new (m_pContextPool->Alloc())THREAD_CONTEXT;
			return pContext;
		}


		/**< 回收一个线程上下文*/
		void ReleaseContext(THREAD_CONTEXT *pContext)
		{
			AutoLock l(&m_contextPoolMutex);
			pContext->thread.~Thread();
			m_pContextPool->Free(pContext);
		}

		/**< 从内存池中申请并创建一个任务*/
		Task* CreateTask()
		{
			AutoLock l(&m_taskPoolMutex);
			Task *pTask = new (m_pTaskPool->Alloc())Task;

			return pTask;
		}

		/**< 回收一个任务内存*/
		void ReleaseTask(Task *pTask)
		{
			AutoLock l(&m_taskPoolMutex);
			pTask->~Task();
			m_pTaskPool->Free(pTask);
		}

		/**< 把一个有效内存压入等待执行的任务序列中*/
		void _PushTask(Task* pTask)
		{
			AutoLock l(&m_threadsMutex);
			AutoLock lock(&m_tasksMutex);

			m_tasks.push_back(pTask);
			m_EventNewTask.Set();
		}

		/**< 从待执行任务序列中取出一个有效的任务*/
		Task* PullTask()
		{
			Task *pTask = NULL;
			TaskVec::iterator it;
			AutoLock l(&m_tasksMutex);

			if (m_tasks.empty())
				return NULL;

			it = m_tasks.begin();
			pTask = *it;
			m_tasks.erase(it);

			return pTask;
		}

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