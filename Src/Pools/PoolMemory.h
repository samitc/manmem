#ifndef _POOLMEMORY_H_
#define _POOLMEMORY_H_
#include "../../GeneralTypes.h"
#define MAX_POOL_SIZE 0xffffffff
class PoolMemory
{
public:
    PoolMemory();
	PoolMemory(uint32 size);
    ~PoolMemory();
    void* alloc(uint32 size);
    void* allocAlign(intp align, uint32 size);
	void dealloc(void* pointer);
private:
    void *mem;
	void *maxMem;
};
#endif