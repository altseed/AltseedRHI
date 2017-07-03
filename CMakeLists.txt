cmake_minimum_required(VERSION 2.8)
project(AltseedRHI CXX)

option(BUILD_TEST "Build test" OFF)

if (MSVC)
	#
else()
	set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC" ${CMAKE_CXX_FLAGS})
endif()

add_subdirectory(src)

if (BUILD_TEST)
add_subdirectory(src_test)
endif()

