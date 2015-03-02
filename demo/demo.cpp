// demo.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <assert.h>
#include <map>
#include <vector>


#include "CodeAddress.h"
#include "ThreadPool.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "../lib/ThreadLib_d.lib")
#else
#pragma comment(lib, "../lib/ThreadLib.lib")
#endif
#else
#include <unistd.h>
#endif

class CTest
{
public:
	CTest()
		:m_Thread(NULL)
		,m_bStop(false)
		,m_nThreadParam(5)
		,m_nThreadPoolParam(10)
	{
	}

	~CTest()
	{
		if (m_Thread != NULL)
		{
			delete m_Thread;
			m_Thread = NULL;
		}
	}

	void *ThreadFunc(void* param)
	{
		int *pNum = (int*)param;
		while (!m_bStop)
		{
			printf("\n\tthe param is a number : \"%i\"\n", *pNum);
#ifdef WIN32
			Sleep(500);
#else
			usleep(500*1000);
#endif
		}
		return NULL;
	}

	void *ThreadPoolFunc(void* pParam)
	{
		int *pNum = (int*)pParam;
		while (!m_bStop)
		{
			printf("\n\tThreadPoolFunc Number is : \"%i\"\n", *pNum);
#ifdef WIN32
			Sleep(500);
#else
			usleep(500*1000);
#endif
		}
		return NULL;
	}

	void StartTest()
	{
		if (m_Thread == NULL)
		{
			int *param = &m_nThreadParam;

			m_Thread = new ThreadLib::Thread();
			m_Thread->Run((DWORD)GET_MEMBER_CALLBACK(CTest,ThreadFunc), (DWORD)this, (DWORD)param);
		}

		int *pool = &m_nThreadPoolParam;

		for (int i = 0; i < 2; i++)
			m_ThreadPool.PushTask((DWORD)GET_MEMBER_CALLBACK(CTest,ThreadPoolFunc), (DWORD)this, (DWORD)pool);

		m_ThreadPool.Start(2);
	}

	void StopTest()
	{
		m_bStop = true;
		m_ThreadPool.Stop();
	}

	ThreadLib::Thread *m_Thread;
	ThreadLib::ThreadPool m_ThreadPool;
	bool m_bStop;
	int	m_nThreadParam;
	int	m_nThreadPoolParam;
};


int main(int argc, char* argv[])
{
	CTest a;
	a.StartTest();
	getchar();
	a.StopTest();

	return 0;
}

