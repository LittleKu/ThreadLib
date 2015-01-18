// ThreadLib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Event.h"
#include "Thread.h"

namespace ThreadLib
{
	Thread::Thread(DWORD pfnCallback, DWORD pClassBase, DWORD pParam)
		:m_hThread(NULL)
	{
		m_pfnCallback = pfnCallback;
		m_pClassBase = pClassBase;
		m_pParam = pParam;

		DWORD dwThreadID;

		m_hThread = (HANDLE)::_beginthreadex(NULL, 0, &Thread::_ThreadProc, this, 0, (unsigned int*)&dwThreadID);
		assert(m_hThread);
	}

	Thread::~Thread()
	{
	}

	void Thread::Join()
	{
		::WaitForSingleObject(m_hThread, INFINITE);
	}

	unsigned int Thread::_ThreadProc(void *param)
	{
		Thread *pThis = (Thread*)param;
		if (pThis->m_pClassBase != NULL)
		{
			reinterpret_cast<void *(__fastcall *)(void*, int, void*)>(pThis->m_pfnCallback)((void*)pThis->m_pClassBase, 0, (void*)pThis->m_pParam);
		}
		else
		{
			reinterpret_cast<void *(*)(void*)>(pThis->m_pfnCallback)((void*)pThis->m_pParam);
		}
		return 0;
	}
}
