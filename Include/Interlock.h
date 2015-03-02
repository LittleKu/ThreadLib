#ifndef __INTERLOCK_H__
#define __INTERLOCK_H__

#ifdef WIN32
#include <Windows.h>
#endif

namespace ThreadLib
{
	long InterLockIncre(void *var, long value);
	long InterLockDecre(void *var, long value);
}

#endif