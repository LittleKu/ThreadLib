#include "Mutex.h"

namespace ThreadLib
{
	Mutex::Mutex()
	{
#ifdef WIN32
		::InitializeCriticalSection(&m_Mutex);
#else
		/************************
		 *	PTHREAD_MUTEX_RECURSIVE_NP	*< 递归型,同一线程里可递归上锁*
		 ************************/
		pthread_mutexattr_t attr;
		int nErrCode = 0;

		if ((nErrCode = pthread_mutexattr_init(&attr)) != 0)	return;/**< 初始化属性*/
		if ((nErrCode = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP)) != 0)	return;/**< 指定互斥锁属性*/
		if ((nErrCode = pthread_mutex_init(&m_Mutex, &attr)) != 0)	return;/**< 初始化互斥锁属性*/
		if ((nErrCode = pthread_mutexattr_destroy(&attr)) != 0)	return;/**< 销毁互斥锁属性项*/
#endif
	}

	Mutex::~Mutex()
	{
#ifdef WIN32
		::DeleteCriticalSection(&m_Mutex);
#else
		pthread_mutex_destroy(&m_Mutex);
#endif
	}

	void Mutex::Lock()
	{
#ifdef WIN32
		::EnterCriticalSection(&m_Mutex);
#else
		pthread_mutex_lock(&m_Mutex);
#endif
	}

	void Mutex::Unlock()
	{
#ifdef WIN32
		::LeaveCriticalSection(&m_Mutex);
#else
		pthread_mutex_unlock(&m_Mutex);
#endif
	}

	AutoLock::AutoLock(Mutex *pMutex)
		:m_pMutex(pMutex)
	{
		m_pMutex->Lock();
		m_bIsLock = true;
	}

	AutoLock::~AutoLock()
	{
		Unlock();
	}

	void AutoLock::Unlock()
	{
		if (m_bIsLock)
		{
			m_pMutex->Unlock();
			m_bIsLock = false;
		}
	}
}