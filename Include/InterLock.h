#ifndef __INTERLOCK_H__
#define __INTERLOCK_H__

#ifdef WIN32
#include <Windows.h>
#endif

namespace ThreadLib
{
	inline long InterLockIncre(void *var, long value)
	{
#ifdef WIN32
		return ::InterlockedExchangeAdd((long*)var, value);
#else
		return (long)__sync_fetch_and_add((unsigned int*)var, value);
#endif
	}

	inline long InterLockDecre(void *var, long value)
	{
		value = value * -1;
#ifdef WIN32
		return ::InterlockedExchangeAdd((long*)var, value);
#else
		return (long)__sync_fetch_and_add((unsigned int*)var, value);
#endif
	}
}

#endif