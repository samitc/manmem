cmake_minimum_required(VERSION 3.6)
project(manmem)
option(BUILD_TESTS "Builds the test" ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES
        Src/Pools/ShortPoolMemory.cpp
        Src/Pools/PoolMemory.cpp
		Src/Pools/FixPoolMemory.cpp
        Src/Pools/ShortPoolMemory.h
        Src/Pools/PoolMemory.h
		Src/Pools/FixPoolMemory.h
        GeneralTypes.h)
set(TEST_SOURCE_FILES
		Test/Main.cpp
		Test/Pools/GeneralPoolMemory.cpp
		Test/Pools/FixPoolMemory.cpp
)
set(PERFORMANCE_SOURCE_FILES
        Test/Main.cpp
        Test/Pools/PoolMemoryPerformance.cpp
        Test/Pools/Performance.cpp
)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_BUILD")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
if(BUILD_TESTS)
	INCLUDE(AddGTest)
	if (AddGTest_FOUND)
	    add_executable(manmemTesting ${TEST_SOURCE_FILES})
	    add_executable(manmemPerformanceTesting ${PERFORMANCE_SOURCE_FILES})
	    add_dependencies(manmemTesting manmem)
	    add_dependencies(manmemTesting gtest)
	    add_dependencies(manmemPerformanceTesting manmem)
	    add_dependencies(manmemPerformanceTesting gtest)
	    target_link_libraries(manmemTesting
	            debug ${GTEST_DEBUG_LIBRARIES}
	            optimized ${GTEST_RELEASE_LIBRARIES}
	            )
		target_link_libraries(manmemTesting manmem)
	    target_link_libraries(manmemPerformanceTesting
	            debug ${GTEST_DEBUG_LIBRARIES}
	            optimized ${GTEST_RELEASE_LIBRARIES}
	            )
		target_link_libraries(manmemPerformanceTesting manmem)
	endif (AddGTest_FOUND)
endif(BUILD_TESTS)
ADD_LIBRARY(manmem STATIC ${SOURCE_FILES})
