#include <iostream>
#include "gtest/gtest.h"
#include "../../Src/Pools/PoolMemory.h"
#include "Performance.h"
TEST(PoolPerformanceTest, fullTest)
{
	const uint32 POOL_SIZE = 50000000;
	PoolMemory pm{ POOL_SIZE };
	const uint32 ALLOC_SIZE = 1000;
	const int MAX_ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
#if defined(DEBUG_BUILD)
	const int ITERATION = 1;
#else
	const int ITERATION = 100;
#endif
	void *p[MAX_ALLOC_TIMES];
	startPerformance(fullTest);
	for (int i = 0; i < ITERATION; i++)
	{
		std::cout << "iteration number:" << i << std::endl;
		int count = 0;
		do {
			p[count] = pm.alloc(ALLOC_SIZE);
		} while (p[count++] != nullptr);
		for (int j = 0; j < count; j++)
		{
			pm.dealloc(p[j]);
		}
	}
	endPerformance(fullTest, "PoolPerformanceTest::fullTest", ITERATION);
}
TEST(PoolPerformanceTest, allocDeallocTest)
{
	const uint32 POOL_SIZE = 50000000;
	PoolMemory pm{ POOL_SIZE };
	const uint32 ALLOC_SIZE = 735;
	const int MAX_ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
#if defined(DEBUG_BUILD)
	const int ITERATION = 1;
#else
	const int ITERATION = 100;
#endif
	void *p[MAX_ALLOC_TIMES];
	startPerformance(alloc);
	startPerformance(dealloc);
	for (int i = 0; i < ITERATION; i++)
	{
		std::cout << "iteration number:" << i << std::endl;
		resumePerformance(alloc);
		int count = 0;
		do {
			p[count] = pm.alloc(ALLOC_SIZE);
		} while (p[count++] != nullptr);
		puasePerformance(alloc);
		resumePerformance(dealloc);
		for (int j = 0; j < count; j++)
		{
			pm.dealloc(p[j]);
		}
		puasePerformance(dealloc);
	}
	endPerformance(alloc, "PoolPerformanceTest::allocTest", ITERATION);
	endPerformance(dealloc, "PoolPerformanceTest::deallocTest", ITERATION);
}