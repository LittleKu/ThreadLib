#ifndef __THREAD_LIB_H__
#define __THREAD_LIB_H__

#pragma once

#ifndef THREADLIB_EXPORTS
#define THREADLIB_API	__declspec(dllimport)
#else
#define THREADLIB_API	__declspec(dllexport)
#endif

#include "MemberAddr.h"
#include "Mutex.h"
#include "Event.h"
#include "Thread.h"

#define GET_MEMBER_CODE(name)	(DWORD)ThreadLib::GetClassFuncAddr(&name)
#define GET_GLOBAL_CODE(name)	(DWORD)(&name)

#define CREATE_CLASS_THREAD(pfn, classbase, param)	new ThreadLib::Thread(GET_MEMBER_CODE(pfn), (DWORD)classbase, (DWORD)param);
#define CREATE_GLOBAL_THREAD(pfn, param)	new ThreadLib::Thread(GET_GLOBAL_CODE(pfn), NULL, (DWORD)param);

#endif