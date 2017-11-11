#include "PoolMemory.h"
#include <cstdlib>
uint32 sized8(uint32 size)
{
	return size + (8 - size % 8);
}
const char MAX_CHAR = 0xffff;
PoolMemory::PoolMemory() : PoolMemory(POOL_MEM_SIZE){
}
PoolMemory::PoolMemory(uint32 size)
{
	init(sized8(size));
}
PoolMemory::~PoolMemory() {
	free(mem);
}
void *PoolMemory::alloc(uint32 size)
{
	uint32 sizeO = sized8(size);
	if (sizeO>=15)
	{

	}
}
void *PoolMemory::allocAlign(intp align, uint32 size)
{

}
void PoolMemory::dealloc(intp pointer)
{
}
void PoolMemory::init(uint32 size)
{
	mem = malloc(size + size / 8);
	curMem = mem;
	maxMem = (char*)mem + size;
	occupiedMem = (char*)maxMem + 1;
	occupiedMaxMem = occupiedMem + size / 8;
}
#if defined(POOL_MEM_SIZE_WAS_NOT_DEFINE)
#undef POOL_MEM_SIZE
#undef POOL_MEM_SIZE_WAS_NOT_DEFINE
#endif