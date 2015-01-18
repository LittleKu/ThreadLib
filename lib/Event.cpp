#include "stdafx.h"
#include "Event.h"

namespace ThreadLib
{
	Event::Event()
		:m_hEvent(NULL)
	{
		m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);/**< 自动回复状态,初始状态为无信号*/
		assert(m_hEvent);
	}

	Event::~Event()
	{
		if (m_hEvent != NULL)
		{
			::CloseHandle(m_hEvent);
			m_hEvent = NULL;
		}
	}

	BOOL Event::Set()
	{
		return ::SetEvent(m_hEvent);
	}

	BOOL Event::Wait(DWORD lMillSeconds)
	{
		BOOL bHasSingle = TRUE;

		DWORD dwObj = ::WaitForSingleObject(m_hEvent, lMillSeconds);
		if (dwObj == WAIT_TIMEOUT || (WAIT_ABANDONED_0 <= dwObj && dwObj <= WAIT_ABANDONED_0 + 1))
			bHasSingle = FALSE;

		return bHasSingle;
	}
}