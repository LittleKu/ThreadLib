#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif

namespace ThreadLib
{
	class Event
	{
	public:
		Event()
		{
#ifdef WIN32
		m_hHandler = ::CreateEvent(NULL, FALSE, FALSE, NULL);
#else
		m_nWaitCount = 0;
		sem_init(&m_hHandler, 0, 0);
#endif
		}
		virtual	~Event()
		{
#ifdef WIN32
		if (m_hHandler != NULL)
		{
			::CloseHandle(m_hHandler);
			m_hHandler = NULL;
		}
#else
		sem_destroy(&m_hHandler);
#endif
		}

		bool Set()
		{
#ifdef WIN32
		::SetEvent(m_hHandler);
#else
		sem_post(&m_hHandler);
#endif
		return true;
		}
		bool WaitEvent(unsigned long lMillSecond = 0xffffffff)
		{
			bool bRet = true;
#ifdef WIN32
			DWORD dwObj = ::WaitForSingleObject(m_hHandler, lMillSecond);
			if (dwObj == WAIT_TIMEOUT || (WAIT_ABANDONED_0 <= dwObj && dwObj <= WAIT_ABANDONED_0 + 1))
				bRet = false;
#else
			__sync_fetch_and_add((unsigned int*)&m_nWaitCount, 1);
			unsigned long infinite = 0xffffffff;
			if (lMillSecond == infinite)
			{
				sem_wait(&m_hHandler);
			}
			else
			{
				int nSecond = lMillSecond/1000;
				int nNSecond = (lMillSecond - nSecond * 1000) * 1000;
				timespec timeout;
				timeout.tv_sec = time(NULL) + nSecond;
				timeout.tv_nsec = nNSecond;

				if (sem_timedwait(&m_hHandler, &timeout) != 0)
					bRet = false;
			}
			if (__sync_fetch_and_add((unsigned int*)&m_nWaitCount, -1))
			{
				sem_init(&m_hHandler, 1, 0);
			}
#endif
			return bRet;
	}

	private:
#ifdef WIN32
		HANDLE	m_hHandler;
#else
		int		m_nWaitCount;
		sem_t	m_hHandler;
#endif
	};
}

#endif