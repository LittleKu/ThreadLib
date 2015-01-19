#ifndef __TASK_H__
#define __TASK_H__

#ifdef _WIN32
#pragma once
#endif

namespace ThreadLib
{
	class THREADLIB_API Task
	{
	public:
		Task();
		virtual ~Task();

	public:
		void PushTask(DWORD pfn, DWORD pClassBase, DWORD pParam);
		void PushTask(DWORD pfn, DWORD pParam);

		void *ExecuteTask();

	private:
		DWORD m_pClassBase;
		DWORD m_pfn;
		DWORD m_pParam;
	};
}

#endif