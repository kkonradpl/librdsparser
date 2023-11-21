cmake_minimum_required(VERSION 3.6)

find_package(PkgConfig REQUIRED)

find_package(CMOCKA REQUIRED)
include_directories(${CMOCKA_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(Valgrind valgrind --error-exitcode=1 --read-var-info=yes --track-origins=yes --leak-check=full)

function(ADD_LIBRDS_TEST TEST_NAME)
    add_executable(${TEST_NAME} ${TEST_NAME}.c)
    add_dependencies(${TEST_NAME} ${TEST_NAME} rds)
    add_test(${TEST_NAME} ${TEST_NAME})
    add_test(${TEST_NAME}_valgrind ${Valgrind} ./${TEST_NAME})
    target_link_libraries(${TEST_NAME} rds cmocka ${LIBRARIES})
endfunction()

add_librds_test(test_af)
add_librds_test(test_group)
add_librds_test(test_group0)
add_librds_test(test_group1)
add_librds_test(test_group2)
add_librds_test(test_librds)
add_librds_test(verification)
