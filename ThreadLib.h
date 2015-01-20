#ifndef __THREAD_LIB_H__
#define __THREAD_LIB_H__

#ifdef _WIN32
#pragma once
#endif

#ifndef THREADLIB_EXPORTS
#define THREADLIB_API	__declspec(dllimport)
#else
#define THREADLIB_API	__declspec(dllexport)
#endif

#include "MemPool.h"
#include "MemberAddr.h"
#include "Mutex.h"
#include "Event.h"
#include "Task.h"
#include "Thread.h"
#include "ThreadPool.h"

#define GET_MEMBER_CODE(name)	(DWORD)ThreadLib::GetClassFuncAddr(&name)
#define GET_GLOBAL_CODE(name)	(DWORD)(&name)

#endif