#include "gtest/gtest.h" 
#if IS_FIX_SIZE==1
#define CREATE_POOL_DEF(name,fixSize)	POOL_NAME name{fixSize}
#define CREATE_POOL(name,size,fixSize) POOL_NAME name{size,fixSize}
#else
#define CREATE_POOL(name,size,fixSize) POOL_NAME name{size}
#define CREATE_POOL_DEF(name,fixSize)	CREATE_POOL(name,,fixSize)
#endif
static void testPointer(void *p,int val,uint32 size)
{
	char *tempData = (char*)alloca(size);
    memset(tempData,val,size);
    ASSERT_EQ(0,memcmp(tempData,p,size));
}
static void testPointers(void **p,int numOfPointers,uint32 *sizes)
{
    const int VAL=5;
    for(int i=0;i<numOfPointers;i++)
    {
        ASSERT_NE(nullptr,p[i]);
        memset(p[i],VAL+i,sizes[i]);
    }
    for (int i = 0; i < numOfPointers; i++)
    {
        for (int j = i + 1; j < numOfPointers; j++)
        {
            ASSERT_NE(p[i],p[j]);
        }
    }
    for (int i = 0; i < numOfPointers; i++)
    {
       testPointer(p[i],VAL+i,sizes[i]);
    }
}
static void testPointers(void **p,int numOfPointers,uint32 size)
{
    uint32 *sizes=(uint32*)alloca(numOfPointers*sizeof(uint32));
    for (int i = 0; i < numOfPointers; i++)
    {
        sizes[i]=size;
    }
    testPointers(p,numOfPointers,sizes);
}
static void testPointersMaxSize(void **p,int maxSize, uint32 *sizes)
{
    int count = 0;
    for (; count < maxSize; count++)
    {
        if (p[count]==nullptr)
        {
            break;
        }
    }
    testPointers(p,count,sizes);
}
static void testPointersMaxSize(void **p,int numOfPointers,uint32 size)
{
    uint32 *sizes=new uint32[numOfPointers];
    for (int i = 0; i < numOfPointers; i++)
    {
        sizes[i]=size;
    }
    testPointersMaxSize(p,numOfPointers,sizes);
}
TEST(TEST_NAME, generalTest)
{
	CREATE_POOL_DEF(pm, 5);
	void * p = pm.alloc(5);
	ASSERT_NE(nullptr, p);
	pm.dealloc(p);
}
TEST(TEST_NAME, sizeTest)
{
	const uint32 POOL_SIZE = 5;
	CREATE_POOL(pm, POOL_SIZE, POOL_SIZE);
	void * p = pm.alloc(POOL_SIZE);
	ASSERT_NE(nullptr, p);
	ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE));
}
TEST(TEST_NAME, sizeBTest)
{
	const uint32 POOL_SIZE = 11;
	CREATE_POOL(pm, POOL_SIZE, POOL_SIZE);
	ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE+1));
}
#if IS_FIX_SIZE==0
TEST(TEST_NAME, allocTest)
{
	const uint32 POOL_SIZE = 100;
	CREATE_POOL(pm, POOL_SIZE, POOL_SIZE);
	void * p = pm.alloc(POOL_SIZE);
	testPointers(&p, 1, POOL_SIZE);
	ASSERT_EQ(nullptr, pm.alloc(5));
}
#endif
TEST(TEST_NAME, allocBigTest)
{
	const uint32 POOL_SIZE = 100;
	CREATE_POOL(pm, POOL_SIZE, POOL_SIZE);
	void * p = pm.alloc(POOL_SIZE);
	testPointers(&p, 1, POOL_SIZE);
	ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE*2));
}
TEST(TEST_NAME, allocTestFix)
{
	const uint32 POOL_SIZE = 100;
	CREATE_POOL(pm, POOL_SIZE, POOL_SIZE);
	void * p = pm.alloc(POOL_SIZE);
	testPointers(&p, 1, POOL_SIZE);
	ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE));
}
TEST(TEST_NAME, multiAllocTest)
{
	const uint32 POOL_SIZE = 100;
	const uint32 ALLOC_SIZE = 20;
	const int ALLOC_TIMES = 3;
	void *p[ALLOC_TIMES];
	CREATE_POOL(pm, POOL_SIZE, ALLOC_SIZE);
	for (size_t i = 0; i < ALLOC_TIMES; i++)
	{
		p[i] = pm.alloc(ALLOC_SIZE);
	}
    testPointers(p,ALLOC_TIMES,ALLOC_SIZE);
}
TEST(TEST_NAME, deallocNullTest)
{
	CREATE_POOL_DEF(pm, );
	pm.dealloc(nullptr);
}
TEST(TEST_NAME, deallocTest)
{
	const uint32 POOL_SIZE = 5;
	CREATE_POOL(pm, POOL_SIZE, POOL_SIZE);
	void * p = pm.alloc(POOL_SIZE);
    testPointers(&p,1,POOL_SIZE);
	ASSERT_EQ(nullptr, pm.alloc(POOL_SIZE));
	pm.dealloc(p);
	ASSERT_NE(nullptr, pm.alloc(POOL_SIZE));
}
TEST(TEST_NAME, deallocFreeTest)
{
	const uint32 POOL_SIZE = 6;
	const uint32 ALLOC_SIZE = 5;
	CREATE_POOL(pm, POOL_SIZE, ALLOC_SIZE);
	void * p = pm.alloc(ALLOC_SIZE);
	testPointers(&p,1,ALLOC_SIZE);
#if IS_FIX_SIZE==0
	ASSERT_EQ(nullptr, pm.alloc(ALLOC_SIZE));
#endif
	pm.dealloc(p);
	ASSERT_NE(nullptr, pm.alloc(ALLOC_SIZE));
}
TEST(TEST_NAME, deallocFullTest)
{
	const uint32 POOL_SIZE = 20;
	const uint32 ALLOC_SIZE = 5;
	const int ITER = 3;
	CREATE_POOL(pm, POOL_SIZE, ALLOC_SIZE);
	const int ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
	void *p[ALLOC_TIMES];
	for (size_t q = 0; q < ITER; q++)
	{
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			p[i] = pm.alloc(ALLOC_SIZE);
		}
        testPointersMaxSize(p,ALLOC_TIMES,ALLOC_SIZE);
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			pm.dealloc(p[i]);
		}
	}
	ASSERT_NE(nullptr, pm.alloc(ALLOC_SIZE));
}
#if IS_FIX_SIZE==0
TEST(TEST_NAME, deallocAlmostFullIntTest)
{
	const uint32 POOL_SIZE = 20;
	const int ALLOC_SIZE = 5;
	const int ITER = 3;
	CREATE_POOL(pm, POOL_SIZE, ALLOC_SIZE);
	const int ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
	void *p[ALLOC_TIMES];
    uint32 sizes[ALLOC_TIMES];
	for (size_t q = 0; q < ITER; q++)
	{
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			p[i] = pm.alloc(ALLOC_SIZE - i);
            sizes[i]=ALLOC_SIZE - i;
		}
        testPointersMaxSize(p,ALLOC_TIMES,sizes);
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			pm.dealloc(p[i]);
		}
	}
	ASSERT_NE(nullptr, pm.alloc(POOL_SIZE));
}
TEST(TEST_NAME, deallocAlmostFullTest)
{
	const uint32 POOL_SIZE = 20;
	const int ALLOC_SIZE = 5;
	const int ITER = 3;
	PoolMemory pm{ POOL_SIZE };
	const int ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
	void *p[ALLOC_TIMES];
    uint32 sizes[ALLOC_TIMES];
	for (size_t q = 0; q < ITER; q++)
	{
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			p[i] = pm.alloc(ALLOC_SIZE - q);
            sizes[i]=ALLOC_SIZE - q;
		}
        testPointersMaxSize(p,ALLOC_TIMES,sizes);
		for (size_t i = 0; i < ALLOC_TIMES; i++)
		{
			pm.dealloc(p[i]);
		}
	}
	ASSERT_NE(nullptr, pm.alloc(POOL_SIZE));
}
#endif
TEST(TEST_NAME, deallocRFullTest)
{
	const uint32 POOL_SIZE = 20;
	const uint32 ALLOC_SIZE = 5;
	CREATE_POOL(pm, POOL_SIZE, ALLOC_SIZE);
	const int ALLOC_TIMES = POOL_SIZE / ALLOC_SIZE;
	void *p[ALLOC_TIMES];
	for (size_t i = 0; i < ALLOC_TIMES; i++)
	{
		p[i] = pm.alloc(ALLOC_SIZE);
	}
    testPointersMaxSize(p,ALLOC_TIMES,ALLOC_SIZE);
	for (int i = ALLOC_TIMES - 1; i >= 0; i--)
	{
		pm.dealloc(p[i]);
	}
	ASSERT_NE(nullptr, pm.alloc(ALLOC_SIZE));
}
TEST(TEST_NAME, allocDeallocAllocFull)
{
	const uint32 POOL_SIZE = 5;
	CREATE_POOL(pm, POOL_SIZE, POOL_SIZE);
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
#if IS_FIX_SIZE==0
TEST(TEST_NAME, smallPool)
{
    const uint32 POOL_SIZE = 5;
    const uint32 ALLOC_SIZE = 1000;
	CREATE_POOL(pm, POOL_SIZE, ALLOC_SIZE);
    ASSERT_EQ(nullptr, pm.alloc(ALLOC_SIZE));
    pm.dealloc(nullptr);
    ASSERT_EQ(nullptr, pm.alloc(ALLOC_SIZE));
    pm.dealloc(nullptr);
    void *p = pm.alloc(POOL_SIZE);
    ASSERT_NE(nullptr, p);
    pm.dealloc(p);
}
TEST(TEST_NAME, fourAllOrder)
{
    const uint32 POOL_SIZE = 1000;
    const int ALLOC_TIMES = 5;
	CREATE_POOL(pm, POOL_SIZE, ALLOC_SIZE);
    void *p[ALLOC_TIMES];
    int sizes[ALLOC_TIMES];
    sizes[0]=550;
    sizes[1]=200;
    sizes[2]=123;
    sizes[3]=83;
    sizes[4]=82;
    for (int i = 0; i < ALLOC_TIMES; i++)
    {
        for (int j = 0; j < ALLOC_TIMES; j++)
        {
            for (int q = 0; q < ALLOC_TIMES; q++)
            {
                for (int w = 0; w < ALLOC_TIMES; w++) 
                {
                    for (int e = 0; e < ALLOC_TIMES; e++) 
                    {
                        if (i==j||i==q||i==w||i==e||j==q||j==w||j==e||q==w||q==e||w==e)
                        {
                            continue;
                        }
                        uint32 sizesC[ALLOC_TIMES];
                        sizesC[0]=sizes[i];
                        sizesC[1]=sizes[j];
                        sizesC[2]=sizes[q];
                        sizesC[3]=sizes[w];
                        sizesC[4]=sizes[e];
                        for (int pi = 0; pi < ALLOC_TIMES; pi++) 
                        {
                            p[pi]=pm.alloc(sizesC[pi]);
                        }
                        testPointersMaxSize(p,ALLOC_TIMES,sizesC);
                        for (int pi = 0; pi < ALLOC_TIMES; pi++)
                        {
                            pm.dealloc(p[pi]);
                        }
                    }
                }
            }
        }
    }
}
#endif