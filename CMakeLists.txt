cmake_minimum_required(VERSION 3.6)
project(manmem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES
        Src/Pools/ShortPoolMemory.cpp
        Src/Pools/PoolMemory.cpp
        Src/Pools/ShortPoolMemory.h
        Src/Pools/PoolMemory.h
        GeneralTypes.h)
set(TEST_SOURCE_FILES
		Test/Main.cpp
		Test/Pools/PoolMemory.cpp
)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_BUILD")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
INCLUDE(AddGTest)
if (AddGTest_FOUND)
    add_executable(manmemTesting ${TEST_SOURCE_FILES})
	
    add_dependencies(manmemTesting manmem)
    add_dependencies(manmemTesting gtest)
    target_link_libraries(manmemTesting
            debug ${GTEST_DEBUG_LIBRARIES}
            optimized ${GTEST_RELEASE_LIBRARIES}
            )
	target_link_libraries(manmemTesting manmem)
endif (AddGTest_FOUND)

ADD_LIBRARY(manmem STATIC ${SOURCE_FILES})