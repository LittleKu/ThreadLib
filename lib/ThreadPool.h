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

		/**< ���̳߳����һ���µ�����,�������Ա����*/
		void PushTask(DWORD pfn, DWORD pClassBase, DWORD pParam);

		/**< ���̳߳����һ���µ�����,���ڷ����Ա����*/
		void PushTask(DWORD pfn, DWORD pParam);

	private:
		/**< �����̳߳�*/
		bool CreateThread(unsigned int nNum);

		/**< �����̵߳Ļص�����*/
		void* ThreadFunc(void* pParam);

		/**< ����һ���߳�������*/
		THREAD_CONTEXT* CreateContext();

		/**< ����һ���߳�������*/
		void ReleaseContext(THREAD_CONTEXT *pContext);
		
		/**< ���ڴ�������벢����һ������*/
		Task* CreateTask();

		/**< ����һ�������ڴ�*/
		void ReleaseTask(Task *pTask);

		/**< ��һ����Ч�ڴ�ѹ��ȴ�ִ�е�����������*/
		void PushTask(Task* pTask);

		/**< �Ӵ�ִ������������ȡ��һ����Ч������*/
		Task* PullTask();

	protected:
		MemoryPool* m_pTaskPool;
		Mutex m_taskPoolMutex;
		MemoryPool* m_pContextPool;
		Mutex m_contextPoolMutex;

		unsigned short m_nMinThreadNum;
		unsigned short m_nThreadNum;
		ThreadMaps m_threads;//�̱߳�
		Mutex m_threadsMutex;//�̱߳��̰߳�ȫ��
		TaskVec m_tasks;//�����
		Mutex m_tasksMutex;//������̰߳�ȫ��
		Event m_EventNewTask;//�������ź�
	};
}

#endif