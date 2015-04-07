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
		Thread()
			:m_bIsRun(false)
#ifdef WIN32
			,m_hThread(NULL)
#endif
		{
#ifndef WIN32
			m_bIsStop = true;
			int nError = 0;
			pthread_mutexattr_t	attr;

			if ((nError = pthread_mutexattr_init(&attr)) != 0)	return;
			if ((nError = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_TIMED_NP)) != 0)	return;
			if ((nError = pthread_mutex_init(&m_exitMutex, &attr)) != 0)	return;
			if ((nError = pthread_mutexattr_destroy(&attr)) != 0)	return;

			pthread_cond_init(&m_exit, NULL);
#endif
		}
		virtual ~Thread()
		{
			Stop();
#ifndef WIN32
			pthread_cond_destroy(&m_exit);
			pthread_mutex_destroy(&m_exitMutex);
#endif
		}

	public:
		/**
		*< 用于类成员函数,格式为void *func(void *param)
		*/
		bool Run(DWORD pfn, DWORD pClassBase, DWORD pParam)
		{
			if (m_bIsRun)
				return false;

			m_task.PushTask(pfn, pClassBase, pParam);

			m_bIsRun = _Run();

			return m_bIsRun;
		}

		/**
		*< 用于非类成员函数,格式为void *func(void *param)
		*/
		bool Run(DWORD pfn, DWORD pParam)
		{
			if (m_bIsRun)
				return false;

			m_task.PushTask(pfn, pParam);

			m_bIsRun = _Run();

			return m_bIsRun;
		}

		void Stop(DWORD lMillSeconds = 0)
		{
#ifdef WIN32
			if (m_hThread == NULL)
				return;

			if (::WaitForSingleObject(m_hThread, lMillSeconds) != WAIT_OBJECT_0)
			{
				::TerminateThread(m_hThread, 0L);
			}
			::CloseHandle(m_hThread);
			m_hThread = NULL;
#else
			if (m_bIsStop)
			{
				m_bIsRun = false;
				return;
			}

			if (lMillSeconds > 0)
				lMillSeconds = 3;

			int nSecond = lMillSeconds / 1000;
			int nNSecond = (lMillSeconds - nSecond * 1000) * 1000;
			timespec	timeout;
			timeout.tv_sec = time(NULL) + nSecond;
			timeout.tv_nsec = nNSecond;
			pthread_mutex_lock(&m_exitMutex);
			if (pthread_cond_timedwait(&m_exit, &m_exitMutex, &timeout) != 0)
				pthread_kill(m_dwThreadID, 1);

			pthread_mutex_unlock(&m_exitMutex);
#endif
			m_bIsRun = false;
		}

		void Join()
		{
#ifdef WIN32
			::WaitForSingleObject(m_hThread, INFINITE);
#else
			pthread_mutex_lock(&m_exitMutex);
			pthread_cond_wait(&m_exit, &m_exitMutex);
			pthread_mutex_unlock(&m_exitMutex);
#endif
		}
		ThreadID GetThreadID()
		{
			return m_dwThreadID;
		}

	protected:
		bool _Run()
		{
#ifdef WIN32
			m_hThread = (HANDLE)::_beginthreadex(NULL, 0, &Thread::ThreadProc, (void*)this, 0, (unsigned int*)&m_dwThreadID);
			if (m_hThread == NULL)
				return false;
#else
			m_bIsStop = false;
			pthread_attr_t	attr;
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			pthread_create(&m_dwThreadID, &attr, ThreadProc, this);
			pthread_attr_destroy(&attr);
#endif
			return true;
		}
#ifdef WIN32
		static unsigned int __stdcall ThreadProc(void *pParam)
		{
			Thread *pThis = (Thread*)pParam;

			void *pRet = NULL;

			pRet = pThis->m_task.ExecuteTask();

			pThis->m_bIsRun = false;

			return 0;
		}
#else
		static void *ThreadProc(void *pParam)
		{
			Thread *pThis = (Thread*)pParam;

			void *pRet = NULL;
			pRet = pThis->m_task.ExecuteTask();

			pThis->m_bIsStop = true;
			pthread_cond_broadcast(&pThis->m_exit);

			pThis->m_bIsRun = false;

			return pRet;
		}
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
