cmake_minimum_required(VERSION 3.6)
project(librds)
option(LIBRDS_DISABLE_HEAP "Disable heap allocator (librds_new/free)" OFF)

if(LIBRDS_DISABLE_HEAP)
    add_definitions(-DLIBRDS_DISABLE_HEAP)
endif()

include_directories(librds PRIVATE include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")

enable_testing()
add_subdirectory(test)
add_subdirectory(src)
add_subdirectory(examples)
