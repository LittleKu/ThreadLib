// ThreadLib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Thread.h"


namespace ThreadLib
{
	Thread::Thread()
		:m_bIsRun(false)
		,m_hThread(NULL)
	{
	}

	Thread::~Thread()
	{
		Stop();
	}

	bool Thread::Run(DWORD pfn, DWORD pClaseBase, DWORD pParam)
	{
		if (m_bIsRun)
			return false;
		m_task.PushTask(pfn, pClaseBase, pParam);

		assert(m_hThread == NULL);
		m_hThread = (HANDLE)::_beginthreadex(NULL, 0, &Thread::ThreadProc, (void*)this, 0, (unsigned int*)&m_dwThreadID);
		if (m_hThread != NULL)
			m_bIsRun = true;

		return m_bIsRun;
	}

	bool Thread::Run(DWORD pfn, DWORD pParam)
	{
		if (m_bIsRun)
			return false;

		m_task.PushTask(pfn, pParam);

		assert(m_hThread == NULL);
		m_hThread = (HANDLE)::_beginthreadex(NULL, 0, &Thread::ThreadProc, (void*)this, 0, (unsigned int*)&m_dwThreadID);
		if (m_hThread != NULL)
			m_bIsRun = true;

		return m_bIsRun;
	}

	void Thread::Stop(DWORD lMillSeconds)
	{
		if (m_hThread == NULL)
			return;

		if (::WaitForSingleObject(m_hThread, lMillSeconds) != WAIT_OBJECT_0)
		{
			::TerminateThread(m_hThread, 0L);
		}
		::CloseHandle(m_hThread);
		m_hThread = NULL;
		m_bIsRun = false;
	}

	void Thread::Join()
	{
		::WaitForSingleObject(m_hThread, INFINITE);
	}

	DWORD Thread::GetThreadID()
	{
		return m_dwThreadID;
	}

	unsigned int Thread::ThreadProc(void* pParam)
	{
		Thread *pThis = (Thread*)pParam;

		void *pRet = NULL;

		pRet = pThis->m_task.ExecuteTask();
		
		pThis->m_bIsRun = false;

		return 0;
	}
}
