#ifndef __CODE_ADDRESS_H__
#define __CODE_ADDRESS_H__

/**
 *	来自Sourcemod框架,了解更多请点击:https://github.com/alliedmodders/sourcemod
 */

namespace ThreadLib
{
	class GenericClass {};
	typedef void (GenericClass::*VoidFunc)();

	inline void *GetCodeAddr(VoidFunc mfp)
	{
		return *(void **)&mfp;
	}
}

#define GetCodeAddress(mfp)	ThreadLib::GetCodeAddr(reinterpret_cast<ThreadLib::VoidFunc>(mfp))

#define GET_MEMBER_CALLBACK(classname, name)	(void*)GetCodeAddress(&classname::name)
#define GET_STATIC_CALLBACK(name)	(void*)&name

#endif