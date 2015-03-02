#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <vector>
#include <map>

#include "Thread.h"
#include "Mutex.h"
#include "Event.h"

#include <stddef.h>

namespace ThreadLib
{
	class MemPool;

	typedef struct THREAD_CONTEXT
	{
		Thread thread;	
		bool bIdle;
		bool bRun;
	}THREAD_CONTEXT;

	typedef std::map<DWORD,THREAD_CONTEXT*> ThreadMaps;
	typedef std::vector<Task*> TaskVec;

	class ThreadPool
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
		MemPool* m_pTaskPool;
		Mutex m_taskPoolMutex;
		MemPool* m_pContextPool;
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