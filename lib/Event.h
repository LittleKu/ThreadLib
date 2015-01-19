#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef _WIN32
#pragma once
#endif

namespace ThreadLib
{
	class THREADLIB_API Event
	{
	public:
		Event();
		virtual ~Event();

		BOOL Set();
		BOOL Wait(DWORD lMillSeconds = INFINITE);

	private:
		HANDLE m_hEvent;
	};
}
#endif