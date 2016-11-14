cmake_minimum_required(VERSION 3.6)
project(manmem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES
        ShortPoolMemory.cpp
        ShortPoolMemory.h
        GeneralTypes.h)

ADD_LIBRARY(manmem STATIC ${SOURCE_FILES})