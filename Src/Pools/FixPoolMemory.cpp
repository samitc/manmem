#include "FixPoolMemory.h"
#include <cstdlib>
#define MAX_POOL_SIZE 0xffffffff
FixPoolMemory::FixPoolMemory():FixPoolMemory(DEFAULT_CHUNK_SIZE)
{
}
FixPoolMemory::FixPoolMemory(uint32 sizeOfChunk) :FixPoolMemory(DEFAULT_POOL_SIZE, sizeOfChunk)
{
}
//TODO optimize
int calcSize(uint32 size, uint32 sizeOfChunk)
{
	return size + (sizeOfChunk - size % sizeOfChunk);
}
FixPoolMemory::FixPoolMemory(uint32 size, uint32 sizeOfChunk) : sizeOfChunk(sizeOfChunk > sizeof(void*) ? sizeOfChunk : sizeof(void*)), firstFreeMem(malloc(calcSize(size, this->sizeOfChunk))), lastFreeMem((char*)firstFreeMem + calcSize(size, this->sizeOfChunk) - this->sizeOfChunk), mem(firstFreeMem)
{
	char* maxMem = (char*)lastFreeMem + sizeOfChunk;
	char* curMem = (char*)mem;
	while (curMem != maxMem)
	{
		*((char**)curMem) = curMem + this->sizeOfChunk;
		curMem += this->sizeOfChunk;
	}
}

FixPoolMemory::~FixPoolMemory()
{
	free(mem);
}

void * FixPoolMemory::alloc(uint32 size)
{
	if (firstFreeMem == lastFreeMem || size != sizeOfChunk)
	{
		return nullptr;
	}
	else
	{
		void *retMem = firstFreeMem;
		firstFreeMem = *(void**)firstFreeMem;
		return retMem;
	}
}

void FixPoolMemory::dealloc(void * pointer)
{
	if (pointer==nullptr)
	{
		return;
	}
	*(void**)lastFreeMem = pointer;
	lastFreeMem = pointer;
}
