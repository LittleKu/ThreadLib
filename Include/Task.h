#ifndef __TASK_H__
#define __TASK_H__

#ifndef NULL
#define NULL	0
#endif

namespace ThreadLib
{
	class Task
	{
	public:
		Task();
		virtual ~Task();

	public:
		void PushTask(unsigned long pfn, unsigned long pClassBase, unsigned long pParam);
		void PushTask(unsigned long pfn, unsigned long pParam);

		void *ExecuteTask();

	private:
		unsigned long m_pClassBase;
		unsigned long m_pfn;
		unsigned long m_pParam;
	};
}

#endif