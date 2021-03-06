#ifndef __CODE_ADDRESS_H__
#define __CODE_ADDRESS_H__



namespace mt
{
#ifdef WIN32
	/**
	 *	来自Nagist的Metahook
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
	 *	来自Sourcemod框架,了解更多请点击:https://github.com/alliedmodders/sourcemod
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
#define GetCodeAddress(mfp)	mt::GetCodeAddr(mfp)
#else
#define GetCodeAddress(mfp)	mt::GetCodeAddr(reinterpret_cast<mt::VoidFunc>(mfp))
#endif

#define GET_MEMBER_CALLBACK(classname, name)	(void*)GetCodeAddress(&classname::name)
#define GET_STATIC_CALLBACK(name)	(void*)&name

#endif