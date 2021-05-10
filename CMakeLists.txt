cmake_minimum_required(VERSION 3.10)
project(c-little-parser C)

set(CMAKE_C_STANDARD_REQUIRED 90)
set(CMAKE_C_STANDARD 99)

if (MSVC)
	set(CMAKE_C_FLAGS_DEBUG "/TC /Z7 /Od")
	set(CMAKE_C_FLAGS_RELEASE "/Os")
else()
	set(CMAKE_C_FLAGS_DEBUG "-Wall -O0 -g -fPIC")
	set(CMAKE_C_FLAGS_RELEASE "-Os -fPIC")
endif()

add_subdirectory(lib/cvector)
include_directories(src/include lib/cvector/src/include)
add_subdirectory(src)
