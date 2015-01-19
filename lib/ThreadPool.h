#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

namespace ThreadLib
{

	typedef struct THREAD_CONTEXT
	{
		Thread thread;	
		bool bIdle;
		bool bRun;
	}THREAD_CONTEXT;

	typedef std::map<DWORD,THREAD_CONTEXT*> ThreadMaps;
	typedef std::vector<Task*> TaskVec;

	class THREADLIB_API ThreadPool
	{
	public:
		ThreadPool();
		~ThreadPool();

		bool Start(int nMinThreadNum);
		void Stop();

		void PushCallback(DWORD pfn, DWORD pClassBase, DWORD pParam);
		void PushCallback(DWORD pfn, DWORD pParam);

	protected:
		bool CreateThread(unsigned int nNum);
		void* ThreadFunc(void* pParam);

		THREAD_CONTEXT* CreateContext();
		void ReleaseContext(THREAD_CONTEXT *pContext);
		

		Task* CreateTask();
		void ReleaseTask(Task *pTask);
		void PushTask(Task* pTask);
		Task* PullTask();

		void StopIdle();

	protected:
		MemoryPool* m_pTaskPool;
		Mutex m_taskPoolMutex;
		MemoryPool* m_pContextPool;
		Mutex m_contextPoolMutex;

		unsigned short m_nMinThreadNum;
		unsigned short m_nThreadNum;
		ThreadMaps m_threads;//线程表
		Mutex m_threadsMutex;//线程表线程安全锁
		TaskVec m_tasks;//任务表
		Mutex m_tasksMutex;//任务表线程安全锁
		Event m_EventNewTask;//新任务信号
	};
}

#endif