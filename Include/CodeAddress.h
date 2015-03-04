#ifndef __CODE_ADDRESS_H__
#define __CODE_ADDRESS_H__



namespace ThreadLib
{
#ifdef WIN32
	/**
	 *	����Nagist��Metahook
	 */
	inline void *GetCodeAddr(...)
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
#else
	/**
	 *	����Sourcemod���,�˽��������:https://github.com/alliedmodders/sourcemod
	 */
	class GenericClass {};
	typedef void (GenericClass::*VoidFunc)();

	inline void *GetCodeAddr(VoidFunc mfp)
	{
		return *(void **)&mfp;
	}
#endif
}

#ifdef WIN32
#define GetCodeAddress(mfp)	ThreadLib::GetCodeAddr(mfp)
#else
#define GetCodeAddress(mfp)	ThreadLib::GetCodeAddr(reinterpret_cast<ThreadLib::VoidFunc>(mfp))
#endif

#define GET_MEMBER_CALLBACK(classname, name)	(void*)GetCodeAddress(&classname::name)
#define GET_STATIC_CALLBACK(name)	(void*)&name

#endif