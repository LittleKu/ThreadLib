#ifndef __MUTEX_H__
#define __MUTEX_H__

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace ThreadLib
{
	class Mutex
	{
	public:
		Mutex();
		~Mutex();

		void Lock();
		void Unlock();

	private:
#ifdef WIN32
		CRITICAL_SECTION m_Mutex;
#else
		pthread_mutex_t m_Mutex;
#endif
	};

	class AutoLock
	{
	public:
		AutoLock(Mutex *pMutex);
		~AutoLock();

		void Unlock();

	private:
		Mutex *m_pMutex;
		bool m_bIsLock;
	};
}

#endif