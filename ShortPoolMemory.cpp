#include "ShortPoolMemory.h"
#include <cstdlib>
ShortPoolMemory::ShortPoolMemory() : mem(malloc(SHORT_POOL_MEM_SIZE)), curMem(mem), maxMem((void*)((char*)mem+SHORT_POOL_MEM_SIZE)) {
}
ShortPoolMemory::~ShortPoolMemory() {
	free(mem);
}
void *ShortPoolMemory::alloc(uint32 size) {
	if ((char*)curMem + size > (char*)maxMem) {
		curMem = (void*)((char*)mem + size);
		return mem;
	}
	void *ret = curMem;
	curMem = (void*)((char*)curMem + size);
	return ret;
}
void *ShortPoolMemory::allocAlign(intp align, uint32 size) {
	if ((char*)curMem + size > (char*)maxMem) {
		curMem = mem;
	}
	int offset = align - 1;
	void *ret = (void *) (((size_t) curMem + offset) & ~(offset));
	curMem =  (void*)((char*)ret + size);
	return ret;
}