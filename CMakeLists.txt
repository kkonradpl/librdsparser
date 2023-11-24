cmake_minimum_required(VERSION 3.6)
project(librds)
option(LIBRDS_DISABLE_HEAP "Disable heap allocator (librds_new/free)" OFF)
option(LIBRDS_DISABLE_UNICODE "Disable unicode support" OFF)
option(LIBRDS_DISABLE_TESTS "Disable tests" OFF)
option(LIBRDS_DISABLE_EXAMPLES "Disable examples" OFF)

if(LIBRDS_DISABLE_HEAP)
    add_definitions(-DLIBRDS_DISABLE_HEAP)
endif()

if(LIBRDS_DISABLE_UNICODE)
    add_definitions(-DLIBRDS_DISABLE_UNICODE)
endif()

include_directories(librds PRIVATE include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")

if(NOT LIBRDS_DISABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(NOT LIBRDS_DISABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

add_subdirectory(src)
