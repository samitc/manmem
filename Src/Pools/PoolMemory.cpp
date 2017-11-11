#include "PoolMemory.h"
#include <cstdlib>
uint32 sized8(uint32 size)
{
	return size + (8 - size % 8);
}
const char MAX_CHAR = 0xffff;
const char MIN_CHAR = 0x0000;
PoolMemory::PoolMemory() : PoolMemory(POOL_MEM_SIZE){
}
PoolMemory::PoolMemory(uint32 size)
{
	init(sized8(size));
}
PoolMemory::~PoolMemory() {
	free(mem);
}
char* findEmptyBlocks(char* ocMem, char* ocMaxMem, uint32 emptyBlocks)
{
	char* curOcc = ocMem;
	do
	{
		while (curOcc<ocMaxMem&&*curOcc != MIN_CHAR)
		{
			curOcc++;
		}
		if (emptyBlocks==1)
		{
			return curOcc;
		}
		else if (emptyBlocks==2)
		{
			if (curOcc+1==MIN_CHAR)
			{
				return curOcc;
			}
			else
			{
				curOcc++;
			}
		}
		else
		{
			if (curOcc+emptyBlocks<ocMaxMem)
			{
				return nullptr;
			}
			char* nextOcc = curOcc + 1;
			char* finalOcc = curOcc + emptyBlocks-1;
			while (nextOcc!=finalOcc)
			{
				if (*nextOcc!=MIN_CHAR)
				{
					break;
				}
				nextOcc++;
			}
			if (*nextOcc==MIN_CHAR)
			{
				return curOcc;
			}
			else
			{
				curOcc++;
			}
		}
	} while (curOcc<ocMaxMem);
	if (*curOcc != MIN_CHAR)
	{
		return nullptr;
	}
}
void occupiedBytes(char* ocMem, uint32 size)
{
	char* fOcMem = ocMem + size;
	while (ocMem<fOcMem)
	{
		*ocMem = MAX_CHAR;
	}
}
constexpr uint32 count0Bit(char num)
{
	uint32 maxZero = 0;
	uint32 curZero = 0;
	const uint32 numBitCount = 8;
	for (size_t i = 0; i < numBitCount; i++)
	{
		if (num&1==0)
		{
			curZero++;
		}
		else if (curZero>maxZero)
		{
			maxZero = curZero;
		}
		num >> 1;
	}
	return maxZero;
}
int32 findEmpty(char* ocMem, char* ocMaxMem, uint32 size)
{
	if (size >= 15)
	{
		while (true)
		{
			uint32 leftByte = MAX_CHAR >> (size % 8);
			uint32 rightByte = MAX_CHAR << (size % 8);
			char* mem = findEmptyBlocks(ocMem, ocMaxMem, size / 8);
			if (mem==nullptr)
			{
				return -1;
			}
			//TODO we can split the free space between left and right
			if (*(mem - 1)&leftByte == leftByte)
			{
				*(mem - 1) |= leftByte;
				occupiedBytes(mem, size / 8);
				return mem - ocMem - size % 8;
			}
			else if (*(mem + -1)&rightByte == rightByte)
			{
				*(mem + 1) |= rightByte;
				occupiedBytes(mem, size / 8);
				return mem - ocMem + size % 8;
			}
		}
	}
	else
	{

	}
}
void *PoolMemory::alloc(uint32 size)
{
	uint32 freeMem = findEmpty(occupiedMem, occupiedMaxMem, size);
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