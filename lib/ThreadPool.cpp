#include "stdafx.h"
#include "ThreadPool.h"

namespace ThreadLib
{
	ThreadPool::ThreadPool()
		:m_nMinThreadNum(0)
		,m_nThreadNum(0)
	{
		m_pTaskPool = new MemoryPool(sizeof(Task), 100);
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
		m_pContextPool = new MemoryPool(sizeof(THREAD_CONTEXT), 50);
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
		for (int i = 0; i < nNum; i++)
		{
			pContext = CreateContext();
			pContext->bIdle = true;
			pContext->bRun = true;
			pContext->thread.Run((DWORD)GetClassFuncAddr(&ThreadPool::ThreadFunc), (DWORD)this, (DWORD)pContext);
			m_threads.insert(ThreadMaps::value_type(pContext->thread.GetThreadID(), pContext));
		}
		m_nThreadNum += nNum;
		return true;
	}
	
	THREAD_CONTEXT *ThreadPool::CreateContext()
	{
		AutoLock l(&m_contextPoolMutex);
		THREAD_CONTEXT *pContext = (THREAD_CONTEXT*)m_pContextPool->Alloc(sizeof(THREAD_CONTEXT));
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

	}
}