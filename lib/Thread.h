#ifndef __THREAD_H__
#define __THREAD_H__

namespace ThreadLib
{
	typedef struct 
	{
		void* pfnCallback;
		void* pClass;
		void* param;
	}ThreadInfo;

	class THREADLIB_API Thread
	{
	public:
		Thread(DWORD pfnCallback, DWORD pClassBase, DWORD pParam);
		~Thread();

		void Start();
		void Join();

	private:
		static unsigned int __stdcall _ThreadProc(void *param);

	private:
		HANDLE m_hThread;
		DWORD m_pfnCallback;
		DWORD m_pClassBase;
		DWORD m_pParam;
	};
}
#endif