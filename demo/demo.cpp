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
		
	}

	void *ThreadFunc(void* param)
	{
		while (!m_bStop)
		{
			printf("\n\tthe param is a string : \"%s\"\n", param);
			Sleep(500);
		}
		return NULL;
	}

	void StartTest()
	{
		if (m_Thread == NULL)
		{
			char *param = "test";

			m_Thread = CREATE_CLASS_THREAD(CTest::ThreadFunc, this, param);
		}
	}

	void StopTest()
	{
		m_bStop = true;
	}

	ThreadLib::Thread *m_Thread;
	bool m_bStop;
};


int _tmain(int argc, _TCHAR* argv[])
{
	CTest a;
	a.StartTest();
	getchar();
	a.StopTest();

	return 0;
}

