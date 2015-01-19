#ifndef __THREAD_H__
#define __THREAD_H__

#ifdef _WIN32
#pragma once
#endif

namespace ThreadLib
{
	class THREADLIB_API Thread
	{
	public:
		Thread();
		virtual ~Thread();

	public:
		bool Run(DWORD pfn, DWORD pClassBase, DWORD pParam);
		bool Run(DWORD pfn, DWORD pParam);

		void Stop(DWORD lMillSeconds = 0);
		void Join();
		DWORD GetThreadID();

	protected:
		static unsigned int __stdcall ThreadProc(void *pParam);

		DWORD m_dwThreadID;
		Task m_task;
		bool m_bIsRun;
		HANDLE m_hThread;
	};
}
#endif