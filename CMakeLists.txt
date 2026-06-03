cmake_minimum_required(VERSION 3.10)
project(librdsparser)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused -pedantic")

option(RDSPARSER_BUILD_STATIC "Build a static library" OFF)

option(RDSPARSER_DISABLE_HEAP "Disable heap allocator (rdsparser_new/free)" OFF)
option(RDSPARSER_DISABLE_UNICODE "Disable unicode support" OFF)

option(RDSPARSER_DISABLE_TESTS "Disable tests" OFF)
option(RDSPARSER_DISABLE_EXAMPLES "Disable examples" OFF)

add_subdirectory(src)

target_include_directories(rdsparser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT RDSPARSER_DISABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(NOT RDSPARSER_DISABLE_EXAMPLES)
    add_subdirectory(examples)
endif()
