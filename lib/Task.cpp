#include "stdafx.h"
#include "Task.h"

namespace ThreadLib
{
	Task::Task()
		:m_pClassBase(NULL)
		,m_pfn(NULL)
		,m_pParam(NULL)
	{
	}

	Task::~Task()
	{
	}

	void Task::PushTask(DWORD pfn, DWORD pClassBase, DWORD pParam)
	{
		m_pfn = pfn;
		m_pClassBase = pClassBase;
		m_pParam = pParam;
	}

	void Task::PushTask(DWORD pfn, DWORD pParam)
	{
		m_pfn = pfn;
		m_pClassBase = NULL;
		m_pParam = pParam;
	}

	void *Task::ExecuteTask()
	{
		if (m_pfn == NULL)
			return NULL;

		if (m_pClassBase != NULL)
		{
			return reinterpret_cast<void *(__fastcall *)(void *)>(m_pfn)((void*)m_pParam);
		}
		else
		{
			return reinterpret_cast<void *(*)(void*)>(m_pfn)((void*)m_pParam);
		}
	}
}