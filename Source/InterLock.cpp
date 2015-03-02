#include "InterLock.h"

namespace ThreadLib
{
	long InterLockIncre(void *var, long value)
	{
#ifdef WIN32
		return ::InterlockedExchangeAdd((long*)var, value);
#else
		return (long)__sync_fetch_and_add((unsigned int*)var, value);
#endif
	}

	long InterLockDecre(void *var, long value)
	{
#ifdef WIN32
		return ::InterlockedExchangeAdd((long*)var, value);
#else
		return (long)__sync_fetch_and_add((unsigned int*)var, value);
#endif
	}
}
