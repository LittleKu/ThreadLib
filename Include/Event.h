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
		Event();
		virtual	~Event();

		bool Set();
		bool WaitEvent(unsigned long lMillSecond = 0xffffffff);

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