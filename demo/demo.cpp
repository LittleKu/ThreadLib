// demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

class CTest
{
public:
	CTest()
		:m_Thread(NULL)
		,m_bStop(false)
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
		char *str = (char*)param;
		while (!m_bStop)
		{
			printf("\n\tthe param is a string : \"%s\"\n", str);
			Sleep(500);
		}
		return NULL;
	}

	void *ThreadPoolFunc(void* pParam)
	{
		char *str = (char*)pParam;
		while (!m_bStop)
		{
			printf("\n\tThreadPoolFunc string : \"%s\"\n", str);
			Sleep(500);
		}
		return NULL;
	}

	void StartTest()
	{
		if (m_Thread == NULL)
		{
			char *param = "test";

			m_Thread = new ThreadLib::Thread();
			m_Thread->Run(GET_MEMBER_CODE(CTest::ThreadFunc), (DWORD)this, (DWORD)param);
		}

		char *pool = "this is thread pool running";

		for (int i = 0; i < 2; i++)
			m_ThreadPool.PushTask(GET_MEMBER_CODE(CTest::ThreadPoolFunc), (DWORD)this, (DWORD)pool);

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
};


int _tmain(int argc, _TCHAR* argv[])
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

	CTest a;
	a.StartTest();
	getchar();
	a.StopTest();

	return 0;
}

