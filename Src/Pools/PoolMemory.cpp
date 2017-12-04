#include "PoolMemory.h"
#include <cstdlib>
#define FREE_VALUE 1
#define NOT_FREE_VALUE 0
#define resetFlag(val) (val)=0
#define getIsFree(val)	((val)&FREE_VALUE)
#define setNotFree(val) (val)&=(!FREE_VALUE)
#define setFree(val) (val)|=FREE_VALUE
PoolMemory::PoolMemory():PoolMemory(1024*1024*10)
{
}
PoolMemory::PoolMemory(uint32 size)
{
	if (size>MAX_POOL_SIZE)
	{
		//TODO:
	}
	else
	{
		mem = malloc(size + sizeof(uint32) + sizeof(char)+1);
		maxMem = (char*)mem + size;
		char* endMemBarriar = (char*)maxMem + sizeof(uint32) + sizeof(char);
		resetFlag(*endMemBarriar);
		setNotFree(*endMemBarriar);
		resetFlag(*((char*)(mem)));
		setFree(*((char*)(mem)));
		*((uint32*)((char*)(mem) + 1)) = size;
	}
}
PoolMemory::~PoolMemory()
{
}
void * PoolMemory::alloc(uint32 size)
{
	char* curMem = (char*)mem;
	uint32 curSize;
	while (true)
	{
		int isFree = getIsFree(*curMem);
		curSize = *((uint32*)(curMem + 1));
		if (isFree == FREE_VALUE&&curSize > size)
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
	setNotFree(*curMem);
	curMem++;
	*((uint32*)(curMem)) = size;
	curMem += sizeof(uint32);
	char *nextMem = curMem + size;
	resetFlag(*nextMem);
	setFree(*nextMem);
	nextMem++;
	*((uint32*)(nextMem)) = curSize - size - sizeof(uint32) - sizeof(char);
	return curMem;
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
	while (getIsFree(*finalMem) == FREE_VALUE)
	{
		finalMem++;
		finalMem += *((uint32*)(finalMem)) + sizeof(uint32);
	}
	*((uint32*)(delPointer)) = finalMem - (char*)pointer;
}