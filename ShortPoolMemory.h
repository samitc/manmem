#ifndef _SHORTPOOLMEMORY_H_
#define _SHORTPOOLMEMORY_H_
#include "GeneralTypes.h"
#if !defined(SHORT_POOL_MEM_SIZE)
    #define SHORT_POOL_MEM_SIZE 1024*1024
#endif
class ShortPoolMemory
{
public:
    ShortPoolMemory();
    ~ShortPoolMemory();
    void* alloc(uint32 size);
    void* allocAlign(intp align, uint32 size);
private:
    void *mem;
    void *curMem;
	void *maxMem;
};
#endif