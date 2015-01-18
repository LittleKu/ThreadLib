#ifndef __THREAD_H__
#define __THREAD_H__

namespace ThreadLib
{
	class THREADLIB_API Thread
	{
	public:
		Thread(DWORD pfnCallback, DWORD pClassBase, DWORD pParam);
		~Thread();

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