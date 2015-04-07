// demo.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <assert.h>
#include <map>
#include <vector>



#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "CodeAddress.h"
#include "ThreadPool.h"

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

		while (true)
		{
			bool bRet = m_Event.WaitEvent(1000);
			if (bRet)
			{
				printf("\n\t thread func break\n");
				break;
			}else{
				printf("\n\tthe param is a number : \"%i\"\n", *pNum);
			}
		}
		return NULL;
	}

	void *ThreadPoolFunc(void* pParam)
	{
		int *pNum = (int*)pParam;

		{
			printf("\n\tThreadPoolFunc Number is : \"%i\"\n", *pNum);
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
		m_Event.Set();
		m_ThreadPool.Stop();
	}

	ThreadLib::Thread *m_Thread;
	ThreadLib::ThreadPool m_ThreadPool;
	ThreadLib::Event	m_Event;
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

	getchar();
	return 0;
}

