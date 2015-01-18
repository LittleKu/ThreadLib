#ifndef __MUTEX_H__
#define __MUTEX_H__

namespace ThreadLib
{
	class THREADLIB_API Mutex
	{
	public:
		Mutex();
		~Mutex();

		void Lock();
		void Unlock();

	private:
		CRITICAL_SECTION m_cs;
	};

	class THREADLIB_API AutoLock
	{
	public:
		AutoLock(Mutex *pMutex);
		~AutoLock();

		void Unlock();

	private:
		Mutex *m_pMutex;
		bool m_bIsLock;
	};
}
#endif