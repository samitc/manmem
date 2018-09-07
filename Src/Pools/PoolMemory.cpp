#include "PoolMemory.h"
#include <cstdlib>
#define FREE_VALUE 1
#define NOT_FREE_VALUE 0
#define MAX_POOL_SIZE 0xffffffff
#define resetFlag(val) (val)=0
#define getIsFree(val)	((val)&FREE_VALUE)
#define setNotFree(val) (val)&=(!FREE_VALUE)
#define setFree(val) (val)|=FREE_VALUE
#define compactMemory(mem)\
 while (getIsFree(*mem) == FREE_VALUE)\
	{\
	mem++;\
	mem += *((uint32*)(mem)) + sizeof(uint32);\
	}
void compactAll(void *startMem, void *endMem)
{
	char *curMem = (char*)startMem;
	while (curMem < endMem)
	{
		if (getIsFree(*curMem) == FREE_VALUE)
		{
			char *tempMem = curMem;
			compactMemory(curMem);
			if (curMem>endMem)
			{
				curMem = (char*)endMem;
			}
			*((uint32*)(tempMem + 1)) = curMem - (char*)tempMem - sizeof(char) - sizeof(uint32);
		}
		else
		{
			curMem += sizeof(char) + sizeof(uint32) + *((uint32*)(curMem + 1));
		}
	}
}
PoolMemory::PoolMemory():PoolMemory(DEFAULT_POOL_SIZE)
{
}
PoolMemory::PoolMemory(uint32 size) : PoolMemory(size, false)
{
}
PoolMemory::PoolMemory(uint32 size, bool isMaxPoolSize)
{
	const int END_PADDING_STOP_DEALLOC = sizeof(uint32) + sizeof(char) + 1;
	const int START_PADDING_FOR_POOL_SIZE = sizeof(uint32) + sizeof(char);
	if (isMaxPoolSize)
	{
		size -= END_PADDING_STOP_DEALLOC;
	}
	else
	{
		size += START_PADDING_FOR_POOL_SIZE;
	}
	if (size>MAX_POOL_SIZE)
	{
		//TODO:
	}
	else
	{
		mem = malloc(size + END_PADDING_STOP_DEALLOC);
		maxMem = (char*)mem + size;
#if !defined(DEBUG_BUILD)
		char* endMemBarriar;
#endif
		endMemBarriar = (char*)maxMem + sizeof(uint32) + sizeof(char);
		for (char * i = (char*)maxMem; i <= endMemBarriar; i++)
		{
#if _MSC_VER >= 1912 // TODO: need due bug in vs compiler in release build. remove when fix.
			*i = 0;
#else
			resetFlag(*i);
			setNotFree(*i);
#endif
		}
		resetFlag(*((char*)(mem)));
		setFree(*((char*)(mem)));
		*((uint32*)((char*)(mem)+1)) = size - START_PADDING_FOR_POOL_SIZE;
	}
}
PoolMemory::~PoolMemory()
{
	free(mem);
}
void * PoolMemory::alloc(uint32 size)
{
	void *mem = pAlloc(size);
	if (mem==nullptr)
	{
		compactAll(this->mem, maxMem);
		mem = pAlloc(size);
	}
	return mem;
}
void * PoolMemory::allocAlign(intp align, uint32 size)
{
	//TODO
	return nullptr;
}
void PoolMemory::dealloc(void* pointer)
{
	if (pointer==nullptr)
	{
		return;
	}
	char* delPointer = (char*)pointer - sizeof(uint32) - sizeof(char);
	setFree(*delPointer);
	delPointer++;
	char *finalMem = delPointer + *((uint32*)(delPointer)) + sizeof(uint32);
	compactMemory(finalMem);
	if (finalMem>maxMem)
	{
		finalMem = (char*)maxMem;
	}
	*((uint32*)(delPointer)) = finalMem - (char*)pointer;
}
void * PoolMemory::pAlloc(uint32 size)
{
	char* curMem = (char*)mem;
	uint32 curSize;
	while (true)
	{
		int isFree = getIsFree(*curMem);
		curSize = *((uint32*)(curMem + 1));
		if (isFree == FREE_VALUE&&curSize >= size)
		{
			break;
		}
		else
		{
			curMem += curSize + sizeof(uint32) + sizeof(char);
			if (curMem > maxMem)
			{
				return nullptr;
			}
		}
	}
	if (curMem + size + sizeof(uint32) + sizeof(char) > maxMem)
	{
		return nullptr;
	}
	setNotFree(*curMem);
	if (curSize < size + sizeof(uint32) + sizeof(char))
	{
		return curMem + sizeof(uint32) + sizeof(char);
	}
	curMem++;
	*((uint32*)(curMem)) = size;
	curMem += sizeof(uint32);
	char *nextMem = curMem + size;
	if (nextMem < maxMem)
	{
		resetFlag(*nextMem);
		setFree(*nextMem);
		nextMem++;
		*((uint32*)(nextMem)) = curSize - (size)-sizeof(uint32) - sizeof(char);
	}
	return curMem;
}
