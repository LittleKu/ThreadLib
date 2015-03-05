#include <time.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <semaphore.h>
#endif

namespace ThreadLib
{
	class Semaphore
	{
	public:
		Semaphore();
		virtual ~Semaphore();
	public:
		bool Create(int nInitCount);
		bool Wait(int timeout = 0);
		bool Post();
	private:
#ifdef WIN32
		HANDLE m_hSem;
#else
		sem_t m_hSem;
		bool  m_bIsCreate;
#endif

	};
}
