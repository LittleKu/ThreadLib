#include "MemPool.h"

namespace ThreadLib
{
	MemPool::MemPool()
		:m_pMemory(NULL)
	{
		m_pMutex = new Mutex();
	}

	MemPool::MemPool(unsigned short nMemSize, unsigned short nMemCount)
		:m_pMemory(NULL)
	{
		m_pMutex = new Mutex();
		if (!Init(nMemSize, nMemCount))
			throw;
	}

	MemPool::~MemPool()
	{
		if (m_pNext != NULL)
		{
			delete m_pNext;
			m_pNext = NULL;
		}

		if (m_pMemory != NULL)
		{
			delete [] m_pMemory;
			m_pMemory = NULL;
		}

		m_nFreeCount = 0;
		
		if (m_pMutex != NULL)
		{
			delete m_pMutex;
			m_pMutex = NULL;
		}
	}

	bool MemPool::Init(unsigned short nMemSize, unsigned short nMemCount)
	{
		if (nMemCount <= 0 || nMemSize <= 0)
			return false;

		unsigned int nThisSize = sizeof(this);

		m_nMemSize = nMemSize;
		m_nMemCount = nMemCount;

		m_pMemory = new unsigned char[8 + (MEMORY_HEADER + m_nMemSize) * m_nMemCount];

		if (m_pMemory == NULL)
			return false;

		unsigned int nIndex = 0;
		unsigned long long uThis = (unsigned long long)this;

		if (nThisSize == 8)/**< 支持64bit寻址*/
		{
			m_pMemory[nIndex++] = (unsigned char)(uThis >> 56);
			m_pMemory[nIndex++] = (unsigned char)(uThis >> 48);
			m_pMemory[nIndex++] = (unsigned char)(uThis >> 40);
			m_pMemory[nIndex++] = (unsigned char)(uThis >> 32);
		}
		else/**< 32bit*/
		{
			m_pMemory[nIndex++] = 0;
			m_pMemory[nIndex++] = 0;
			m_pMemory[nIndex++] = 0;
			m_pMemory[nIndex++] = 0;
		}
		m_pMemory[nIndex++] = (unsigned char)(uThis >> 24);
		m_pMemory[nIndex++] = (unsigned char)(uThis >> 16);
		m_pMemory[nIndex++] = (unsigned char)(uThis >> 8);
		m_pMemory[nIndex++] = (unsigned char)(uThis);

		unsigned short i;
		for (i = 0; i < m_nMemCount; i++)
		{
			/**< 标记状态*/
			m_pMemory[nIndex++] = 0;
			m_pMemory[nIndex++] = 0;
			m_pMemory[nIndex++] = 0;
			m_pMemory[nIndex++] = 0;

			/**< 预留*/
			m_pMemory[nIndex++] = 0;
			m_pMemory[nIndex++] = 0;

			/**< 索引*/
			m_pMemory[nIndex++] = (unsigned char)(i>>8);
			m_pMemory[nIndex++] = (unsigned char)i;

			nIndex += m_nMemSize;/**< 实际使用的内存*/
		}

		m_pNext = NULL;
		
		m_nFreeCount = m_nMemCount;

		return true;
	}

	void *MemPool::_Alloc()
	{
		void *pObj = NULL;
		int i = 0;
		int nPos = 8 + i;

		for (i = 0; i < m_nMemCount; i++)
		{
			if (m_pMemory[nPos] == 0)/**< 未使用内存*/
			{
				if (InterLockIncre((void*)&m_pMemory[nPos], 1) == 0)
				{
					nPos += MEMORY_HEADER;
					pObj = &m_pMemory[nPos];
					break;
				}
			}
			nPos += (MEMORY_HEADER + m_nMemSize);
		}
		return pObj;
	}

	void *MemPool::Alloc()
	{
		MemPool *pPool = this;
		for (; pPool != NULL; pPool = pPool->m_pNext)
		{
			if (InterLockDecre((void*)&pPool->m_nFreeCount, 1) > 0)
				break;
			InterLockIncre((void*)&pPool->m_nFreeCount, 1);

			if (pPool->m_pNext == NULL)
			{
				AutoLock l(m_pMutex);
				if (pPool->m_pNext == NULL)/**< 多线程上下文切换*/
				{
					pPool->m_pNext = new MemPool(m_nMemSize, m_nMemCount);
				}
			}
		}
		return pPool->_Alloc();
	}

	void MemPool::_Free(unsigned char *pObj)
	{
		pObj = pObj - MEMORY_HEADER;
		
#ifdef WIN32
		::InterlockedExchange((long *)pObj, 0);
		::InterlockedIncrement((long *)&m_nFreeCount);
#else
		__sync_lock_test_and_set((unsigned int*)pObj, 0);
		__sync_add_and_fetch((unsigned int*)&m_nFreeCount, 1);
#endif
	}

	void MemPool::Free(void *pObj)
	{
		unsigned char *pObject = (unsigned char*)pObj;
		MemPool *pPool = GetMemPool(pObject);
		pPool->_Free(pObject);
	}

	MemPool *MemPool::GetMemPool(unsigned char *pObj)
	{
		unsigned short nIndex = GetMemIndex(pObj);
		unsigned char *pMem = pObj - (nIndex * (m_nMemSize + MEMORY_HEADER)) - MEMORY_HEADER - 8;
		unsigned long long pPool = 0;

		pPool = pMem[0];
		pPool = (pPool << 8) + pMem[1];
		pPool = (pPool << 8) + pMem[2];
		pPool = (pPool << 8) + pMem[3];
		pPool = (pPool << 8) + pMem[4];
		pPool = (pPool << 8) + pMem[5];
		pPool = (pPool << 8) + pMem[6];
		pPool = (pPool << 8) + pMem[7];

		return (MemPool*)pPool;
	}

	unsigned short MemPool::GetMemIndex(unsigned char *pObj)
	{
		unsigned char *pMem = pObj - MEMORY_HEADER;
		unsigned short nIndex = 0;
		nIndex = pMem[6];
		nIndex = (nIndex << 8) + pMem[7];

		return nIndex;
	}
}
