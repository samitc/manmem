#ifndef _POOLMEMORY_H_
#define _POOLMEMORY_H_
#include "../../GeneralTypes.h"
#define MAX_POOL_SIZE 0xffffffff
class PoolMemory
{
public:
    PoolMemory();
	PoolMemory(uint32 size);
	// if maxPoolSize is true the size is the size of the pool and allocation of this size may fail
	// if maxPoolSize is false the size is the size of max allocation size in first allocatino only(can succcess also in sub calls) and the pool can be larger
	PoolMemory(uint32 size, bool isMaxPoolSize);
    ~PoolMemory();
    void* alloc(uint32 size);
    void* allocAlign(intp align, uint32 size);
	void dealloc(void* pointer);
private:
	void* pAlloc(uint32 size);
    void *mem;
	void *maxMem;
#if defined(DEBUG_BUILD)
	char *endMemBarriar;
#endif
};
#endif