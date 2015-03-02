#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

#include "InterLock.h"
#include "Mutex.h"

/**< 每一块内存的头部信息*/
#define MEMORY_HEADER	8

namespace ThreadLib
{
	class MemPool
	{
	public:
		MemPool();
		MemPool(unsigned short nMemSize, unsigned short nMemCount);
		~MemPool();

		bool Init(unsigned short nMemSize, unsigned short nMemCount);
		
		void *Alloc();
		void Free(void *pObj);

	private:
		void *_Alloc();
		void _Free(unsigned char *pObj);

		MemPool *GetMemPool(unsigned char *pObj);

		unsigned short GetMemIndex(unsigned char *pObj);

	private:
		MemPool	*m_pNext;

		unsigned char*	m_pMemory;
		unsigned short	m_nMemSize;
		unsigned short	m_nMemCount;
		int				m_nFreeCount;
		Mutex*			m_pMutex;
	};
}

#endif
