cmake_minimum_required(VERSION 3.6)

add_executable(rds-example main.c)

target_include_directories(rds-example PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(rds-example rdsparser)

