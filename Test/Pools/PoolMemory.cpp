
#include "gtest/gtest.h"
#include "../../Src/Pools/PoolMemory.h"

TEST(PoolGeneralTest, generalTest)
{
	PoolMemory pm{};
	void * p = pm.alloc(5);
	ASSERT_NE(nullptr, p);
	pm.dealloc(p);
}
TEST(PoolGeneralTest, sizeTest)
{
	const uint32 POOL_SIZE = 5;
	PoolMemory pm{ POOL_SIZE };
	void * p = pm.alloc(POOL_SIZE);
	ASSERT_NE(nullptr, p);
	ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE));
}
TEST(PoolGeneralTest, sizeBTest)
{
	const uint32 POOL_SIZE = 5;
	PoolMemory pm{ POOL_SIZE };
	ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE+1));
}
TEST(PoolGeneralTest, allocTest)
{
	/*const uint32 POOL_SIZE = 100;
	char testData[POOL_SIZE];
	PoolMemory pm{ POOL_SIZE };
	void * p = pm.alloc(POOL_SIZE);
	const int VAL = 3;
	memset(p, VAL, POOL_SIZE);
	memset(testData, VAL, POOL_SIZE);
	ASSERT_NE(nullptr, p);
	ASSERT_EQ(nullptr, pm.alloc(5));
	ASSERT_EQ(0, memcmp(testData, p, POOL_SIZE));*/
}
TEST(PoolGeneralTest, multiAllocTest)
{
	const uint32 POOL_SIZE = 100;
	const int ALLOC_SIZE = 20;
	const int ALLOC_TIMES = 3;
	const int VAL = 3;
	char testData[ALLOC_SIZE];
	void *p[ALLOC_TIMES];
	PoolMemory pm{ POOL_SIZE };
	for (size_t i = 0; i < ALLOC_TIMES; i++)
	{
		p[i] = pm.alloc(ALLOC_SIZE);
	}
	for (size_t i = 0; i < ALLOC_TIMES; i++)
	{
		memset(p[i], VAL+i, ALLOC_SIZE);
	}
	for (size_t i = 0; i < ALLOC_TIMES; i++)
	{
		ASSERT_NE(nullptr, p[i]);
		for (size_t j = i+1; j < ALLOC_TIMES; j++)
		{
			ASSERT_NE(p[i], p[j]);
		}
		memset(testData, VAL+i, ALLOC_SIZE);
		ASSERT_EQ(0, memcmp(testData, p[i], ALLOC_SIZE));
	}
}
TEST(PoolGeneralTest, deallocNullTest)
{
	PoolMemory pm{ };
	pm.dealloc(nullptr);
}
TEST(PoolGeneralTest, deallocTest)
{
	const uint32 POOL_SIZE = 5;
	PoolMemory pm{ POOL_SIZE };
	void * p = pm.alloc(POOL_SIZE);
	ASSERT_NE(nullptr, p);
	ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE));
	pm.dealloc(p);
	ASSERT_NE(nullptr, pm.alloc(POOL_SIZE));
}
TEST(PoolGeneralTest, deallocFreeTest)
{
	const uint32 POOL_SIZE = 6;
	const int ALLOC_SIZE = 5;
	PoolMemory pm{ POOL_SIZE };
	void * p = pm.alloc(ALLOC_SIZE);
	ASSERT_NE(nullptr, p);
	ASSERT_EQ(nullptr, pm.alloc(ALLOC_SIZE));
	pm.dealloc(p);
	ASSERT_NE(nullptr, pm.alloc(POOL_SIZE));
}
TEST(PoolGeneralTest, deallocFullTest)
{
	const uint32 POOL_SIZE = 20;
	const int ALLOC_SIZE = 5;
	const int ITER = 3;
	PoolMemory pm{ POOL_SIZE };
	const int ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
	void *p[ALLOC_TIMES];
	for (size_t q = 0; q < ITER; q++)
	{
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			p[i] = pm.alloc(ALLOC_SIZE);
		}
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			pm.dealloc(p[i]);
		}
	}
	ASSERT_NE(nullptr, pm.alloc(POOL_SIZE));
}
TEST(PoolGeneralTest, deallocAlmostFullIntTest)
{
	const uint32 POOL_SIZE = 20;
	const int ALLOC_SIZE = 5;
	const int ITER = 3;
	PoolMemory pm{ POOL_SIZE };
	const int ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
	void *p[ALLOC_TIMES];
	for (size_t q = 0; q < ITER; q++)
	{
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			p[i] = pm.alloc(ALLOC_SIZE - i);
		}
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			pm.dealloc(p[i]);
		}
	}
	ASSERT_NE(nullptr, pm.alloc(POOL_SIZE));
}
TEST(PoolGeneralTest, deallocAlmostFullTest)
{
	const uint32 POOL_SIZE = 20;
	const int ALLOC_SIZE = 5;
	const int ITER = 3;
	PoolMemory pm{ POOL_SIZE };
	const int ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
	void *p[ALLOC_TIMES];
	for (size_t q = 0; q < ITER; q++)
	{
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			p[i] = pm.alloc(ALLOC_SIZE - q);
		}
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			pm.dealloc(p[i]);
		}
	}
	ASSERT_NE(nullptr, pm.alloc(POOL_SIZE));
}
TEST(PoolGeneralTest, deallocRFullTest)
{
	const uint32 POOL_SIZE = 20;
	const int ALLOC_SIZE = 5;
	PoolMemory pm{ POOL_SIZE };
	const int ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
	void *p[ALLOC_TIMES];
	for (size_t i = 0; i < ALLOC_TIMES; i++)
	{
		p[i] = pm.alloc(ALLOC_SIZE);
	}
	for (int i = ALLOC_TIMES - 1; i >= 0; i--)
	{
		pm.dealloc(p[i]);
	}
	ASSERT_NE(nullptr, pm.alloc(POOL_SIZE));
}
TEST(PoolGeneralTest, allocDeallocAllocFull)
{
	const uint32 POOL_SIZE = 5;
	PoolMemory pm{ POOL_SIZE };
	const int NUM_OF_ALLOC = 3;
	for (size_t i = 0; i < NUM_OF_ALLOC; i++)
	{
		void * p = pm.alloc(POOL_SIZE);
		ASSERT_NE(nullptr, p);
		ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE));
		pm.dealloc(p);
	}
	void * p1 = pm.alloc(POOL_SIZE);
	ASSERT_NE(nullptr, p1);
	ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE));
	pm.dealloc(p1);
}