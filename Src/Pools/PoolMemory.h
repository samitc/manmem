#ifndef _POOLMEMORY_H_
#define _POOLMEMORY_H_
#include "../../GeneralTypes.h"
#if !defined(POOL_MEM_SIZE)
	#define POOL_MEM_SIZE_WAS_NOT_DEFINE
    #define POOL_MEM_SIZE 1024*1024*1024*50
#endif
class PoolMemory
{
public:
	PoolMemory();
	PoolMemory(uint32 size);
    ~PoolMemory();
    void* alloc(uint32 size);
    void* allocAlign(intp align, uint32 size);
	void dealloc(intp pointer);
private:
	void init(uint32 size);
    void *mem;
    void *curMem;
	void *maxMem;
	char* occupiedMem;
	char* occupiedMaxMem
};
#endif