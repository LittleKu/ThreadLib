#include "stdafx.h"
#include "Mutex.h"

namespace ThreadLib
{
	Mutex::Mutex()
	{
		::InitializeCriticalSection(&m_cs);
	}

	Mutex::~Mutex()
	{
		::DeleteCriticalSection(&m_cs);
	}

	void Mutex::Lock()
	{
		::EnterCriticalSection(&m_cs);
	}

	void Mutex::Unlock()
	{
		::LeaveCriticalSection(&m_cs);
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