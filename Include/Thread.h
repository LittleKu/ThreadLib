#ifndef __THREAD_H__
#define __THREAD_H__

#include "Task.h"

#ifdef WIN32
#include <Windows.h>
#include <process.h>
#include <assert.h>
#else
#include <pthread.h>
#include <signal.h>
typedef unsigned long DWORD;
#endif

namespace ThreadLib
{

#ifdef WIN32
	typedef DWORD ThreadID;
#else
	typedef pthread_t ThreadID;
#endif

	class Thread
	{
	public:
		Thread();
		virtual ~Thread();

	public:
		/**
		 *< 用于类成员函数,格式为void *func(void *param)
		 */
		bool Run(DWORD pfn, DWORD pClassBase, DWORD pParam);

		/**
		 *< 用于非类成员函数,格式为void *func(void *param)
		 */
		bool Run(DWORD pfn, DWORD pParam);

		void Stop(DWORD lMillSeconds = 0);
		void Join();
		ThreadID GetThreadID();

	protected:
		bool _Run();
#ifdef WIN32
		static unsigned int __stdcall ThreadProc(void *pParam);
#else
		static void *ThreadProc(void *pParam);
#endif

		ThreadID m_dwThreadID;
		Task m_task;
		bool m_bIsRun;

#ifdef WIN32
		HANDLE m_hThread;
#else
		pthread_cond_t	m_exit;
		pthread_mutex_t	m_exitMutex;
		bool			m_bIsStop;
#endif
	};
}
#endif
