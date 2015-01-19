
#include "stdafx.h"
#include "MemPool.h"


namespace ThreadLib
{
	MemoryPool::MemoryPool(int blockSize, int numElements)
	{
		m_blocksPerBlob = numElements;
		m_blockSize = blockSize;
		m_numBlobs = 0;
		m_numElements = 0;

		AddNewBlob();

		m_peakAlloc = 0;
		m_blocksAllocated = 0;
	}

	MemoryPool::~MemoryPool(void)
	{
		for (int i = 0; i < m_numBlobs; i++)
			::free(m_memBlob[i]);
	}

	void *MemoryPool::Alloc(unsigned int amount)
	{
		void *returnBlock;

		if (amount > (unsigned int)m_blockSize)
			return NULL;

		m_blocksAllocated++;
		m_peakAlloc = max(m_peakAlloc, m_blocksAllocated);

		if (m_blocksAllocated >= m_numElements)
			AddNewBlob();

		if (!m_headOfFreeList)
			DebugBreak();

		returnBlock = m_headOfFreeList;
		m_headOfFreeList = *((void **)m_headOfFreeList);

		return returnBlock;
	}

	void MemoryPool::Free(void *memblock)
	{
		if (!memblock)
			return;

#ifdef _DEBUG
		memset(memblock, 0xDD, m_blockSize);
#endif
		m_blocksAllocated--;
		*((void **)memblock) = m_headOfFreeList;
		m_headOfFreeList = memblock;
	}

	void MemoryPool::AddNewBlob(void)
	{
		int sizeMultiplier = pow(2.0, m_numBlobs);
		int nElements = m_blocksPerBlob * sizeMultiplier;
		int blobSize = nElements * m_blockSize;

		m_memBlob[m_numBlobs] = malloc(blobSize);

		if (!m_memBlob[m_numBlobs])
			DebugBreak();

		m_headOfFreeList = m_memBlob[m_numBlobs];

		if (!m_headOfFreeList)
			DebugBreak();

		void **newBlob = (void **)m_headOfFreeList;

		for (int j = 0; j < nElements - 1; j++)
		{
			newBlob[0] = (char *)newBlob + m_blockSize;
			newBlob = (void **)newBlob[0];
		}

		newBlob[0] = NULL;

		m_numElements += nElements;
		m_numBlobs++;

		if (m_numBlobs >= MAX_BLOBS - 1)
			DebugBreak();
	}

}