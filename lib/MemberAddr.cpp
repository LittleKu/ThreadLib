#include "stdafx.h"
#include "MemberAddr.h"

namespace ThreadLib
{
	void *GetClassFuncAddr(...)
	{
		DWORD address;

		__asm
		{
			lea eax,address
				mov edx, [ebp + 8]
			mov [eax], edx
		}

		return (void *)address;
	}
}
