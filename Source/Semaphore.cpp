#include "Semaphore.h"

namespace ThreadLib
{
	Semaphore::Semaphore()
	{
#ifdef WIN32
		m_hSem = NULL;
#else
		m_bIsCreate = false;
#endif

	}
	Semaphore::~Semaphore()
	{
#ifdef WIN32
		if (m_hSem != NULL)
		{
			::CloseHandle(m_hSem);
			m_hSem = NULL;
		}
#else
		if (m_bIsCreate)
		{
			m_bIsCreate = false;
			sem_destroy(&m_hSem);
		}
#endif

	}

	bool Semaphore::Create(int initcount)
	{
		if (initcount < 0)
		{
			initcount = 0;
		}
#ifdef WIN32
		if (initcount > 64)
			return false;

		m_hSem = ::CreateSemaphore(NULL, initcount, 64, NULL);
		if (m_hSem == NULL)
			return false;
#else
		if (sem_init(&m_hSem, 0, initcount) != 0)
			return false;

		m_bIsCreate = true;
#endif

		return true;
	}
	bool Semaphore::Wait(int timeout)
	{
#ifdef WIN32
		if (timeout <= 0)
		{
			timeout = INFINITE;
		}
		if (::WaitForSingleObject(m_hSem, timeout) != WAIT_OBJECT_0)
		{
			return false;
		}
#else
		if (timeout <= 0)
			return (sem_wait(&m_hSem) == 0);
		else
		{
			struct timeval tm;
			gettimeofday(&tm, NULL);
			long long endtime = tm.tv_sec *1000 + tm.tv_usec/1000 + timeout;

			do 
			{
				usleep(50*1000);
				int ret = sem_trywait(&m_hSem);
				if (ret == 0)
					return true;

				struct timeval tv_cur;
				gettimeofday(&tv_cur, NULL);

				if (tv_cur.tv_sec*1000 + tv_cur.tv_usec/1000 > endtime)
					return false;

				if (ret == -1 && errno == EAGAIN)
					continue;

				else
					return false;

			} while (true);
			return false;
		}
#endif
		return true;
	}

	bool Semaphore::Post()
	{
#ifdef WIN32
		return ::ReleaseSemaphore(m_hSem, 1, NULL) ? true : false;
#else
		return (sem_post(&m_hSem) == 0);
#endif

	}
}
