#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

#ifdef _WIN32
#pragma once
#endif

namespace ThreadLib
{
	class THREADLIB_API MemoryPool
	{
	public:
		MemoryPool(int blockSize, int numElements);
		~MemoryPool(void);

	public:
		void *Alloc(unsigned int amount);
		void Free(void *memblock);
		int Count(void) { return m_blocksAllocated; }
		void AddNewBlob(void);

	public:
		enum
		{
			MAX_BLOBS = 16
		};

		int m_blockSize;
		int m_blocksPerBlob;
		int m_numElements;
		void *m_memBlob[MAX_BLOBS];
		void *m_headOfFreeList;
		int m_numBlobs;
		int m_peakAlloc;
		int m_blocksAllocated;
	};

}
#endif