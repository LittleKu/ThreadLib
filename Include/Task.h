#ifndef __TASK_H__
#define __TASK_H__

#ifndef NULL
#define NULL	0
#endif

namespace mt
{
	class Task
	{
	public:
		Task()
			:m_pClassBase(NULL)
			,m_pfn(NULL)
			,m_pParam(NULL)
		{
		}
		virtual ~Task(){}

	public:
		void PushTask(unsigned long pfn, unsigned long pClassBase, unsigned long pParam)
		{
			m_pfn = pfn;
			m_pClassBase = pClassBase;
			m_pParam = pParam;
		}
		void PushTask(unsigned long pfn, unsigned long pParam)
		{
			m_pfn = pfn;
			m_pClassBase = NULL;
			m_pParam = pParam;
		}

		void *ExecuteTask()
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

	private:
		unsigned long m_pClassBase;
		unsigned long m_pfn;
		unsigned long m_pParam;
	};
}

#endif