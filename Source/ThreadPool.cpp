#include <assert.h>

#include "ThreadPool.h"
#include "MemPool.h"
#include "CodeAddress.h"

namespace ThreadLib
{
	ThreadPool::ThreadPool()
		:m_nMinThreadNum(0)
		,m_nThreadNum(0)
	{
		m_pTaskPool = new MemPool(sizeof(Task), 100);
		m_pContextPool = NULL;
	}

	ThreadPool::~ThreadPool()
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

	bool ThreadPool::Start(int nMinThreadNum)
	{
		m_nMinThreadNum = nMinThreadNum;
		m_pContextPool = new MemPool(sizeof(THREAD_CONTEXT), 50);
		return CreateThread(m_nMinThreadNum);
	}

	bool ThreadPool::CreateThread(unsigned int nNum)
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

	THREAD_CONTEXT *ThreadPool::CreateContext()
	{
		AutoLock l(&m_contextPoolMutex);
		THREAD_CONTEXT *pContext = new (m_pContextPool->Alloc())THREAD_CONTEXT;
		return pContext;
	}

	void ThreadPool::ReleaseContext(THREAD_CONTEXT *pContext)
	{
		AutoLock l(&m_contextPoolMutex);
		pContext->thread.~Thread();
		m_pContextPool->Free(pContext);
	}

	void ThreadPool::Stop()
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

	void ThreadPool::PushTask(DWORD pfn, DWORD pClassBase, DWORD pParam)
	{
		Task *pTask = CreateTask();
		pTask->PushTask(pfn, pClassBase, pParam);
		PushTask(pTask);
	}

	void ThreadPool::PushTask(DWORD pfn, DWORD pParam)
	{
		Task *pTask = CreateTask();
		pTask->PushTask(pfn, pParam);
		PushTask(pTask);
	}

	Task *ThreadPool::CreateTask()
	{
		AutoLock l(&m_taskPoolMutex);
		Task *pTask = new (m_pTaskPool->Alloc())Task;

		return pTask;
	}

	void ThreadPool::ReleaseTask(Task *pTask)
	{
		AutoLock l(&m_taskPoolMutex);
		pTask->~Task();
		m_pTaskPool->Free(pTask);
	}

	void ThreadPool::PushTask(Task *pTask)
	{
		AutoLock l(&m_threadsMutex);
		AutoLock lock(&m_tasksMutex);

		m_tasks.push_back(pTask);
		m_EventNewTask.Set();
	}

	Task *ThreadPool::PullTask()
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

	void *ThreadPool::ThreadFunc(void *pParam)
	{
		THREAD_CONTEXT *pContext = (THREAD_CONTEXT*)pParam;
		Task *pTask = NULL;

		while (pContext->bRun)
		{
			if (!pContext->bRun)/**< ע���л�*/
				break;

			pContext->bIdle = false;/**< �߳̽���æµ״̬*/

			while (pContext->bRun)/**< ����û��Ҫ����,�����жϻ���*/
			{
				pTask = PullTask();/**< ȡ����ʱ���м���*/
				if (pTask == NULL)
					break;

				pTask->ExecuteTask();/**< ִ������*/
				ReleaseTask(pTask);/**< �ͷŻ��������ڴ�*/
			}

			pContext->bIdle = true;/**< �߳��Ѿ��������,�������״̬,�ȴ���һ�ε��������*/

			if (!pContext->bRun)/**< Ҫ��ǰ�߳�ֹͣ����*/
				break;

			if (!m_EventNewTask.WaitEvent())/**< û���������ź�*/
				continue;
		}

		m_EventNewTask.Set();/**< �˴�֪ͨ��һ���߳��˳�*/
		return NULL;
	}
}