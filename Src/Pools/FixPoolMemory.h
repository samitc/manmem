#ifndef _FIXPOOLMEMORY_H_
#define _FIXPOOLMEMORY_H_
#include "../../GeneralTypes.h"
#define DEFAULT_POOL_SIZE	1024*1024*10
#define DEFAULT_CHUNK_SIZE	sizeof(void*)
#define MINIMUM_CHUNK_SIZE	sizeof(void*)
class FixPoolMemory
{
public:
	FixPoolMemory();
	FixPoolMemory(uint32 sizeOfChunk);
	FixPoolMemory(uint32 size, uint32 sizeOfChunk);
    ~FixPoolMemory();
    void* alloc(uint32 size);
	void dealloc(void* pointer);
private:
	uint32 sizeOfChunk;
	void *firstFreeMem;
	void* lastFreeMem;
	void *mem;
};
#endif