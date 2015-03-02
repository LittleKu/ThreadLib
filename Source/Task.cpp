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

	void Task::PushTask(unsigned long pfn, unsigned long pClassBase, unsigned long pParam)
	{
		m_pfn = pfn;
		m_pClassBase = pClassBase;
		m_pParam = pParam;
	}

	void Task::PushTask(unsigned long pfn, unsigned long pParam)
	{
		m_pfn = pfn;
		m_pClassBase = NULL;
		m_pParam = pParam;
	}

	void *Task::ExecuteTask()
	{
		if (m_pfn == NULL)/**< 回调地址为空*/
			return NULL;

		if (m_pClassBase != NULL)
		{
#ifdef WIN32
			return reinterpret_cast<void *(__fastcall *)(void *, int, void*)>(m_pfn)((void*)m_pClassBase, 0, (void*)m_pParam);
#else
			return reinterpret_cast<void *(*)(void *, void*)>(m_pfn)((void*)m_pClassBase, (void*)m_pParam);
#endif
		}
		else
		{
			return reinterpret_cast<void *(*)(void*)>(m_pfn)((void*)m_pParam);
		}
	}
}