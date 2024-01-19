cmake_minimum_required(VERSION 3.6)

find_package(PkgConfig REQUIRED)

find_package(CMOCKA REQUIRED)
include_directories(${CMOCKA_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(Valgrind valgrind --error-exitcode=1 --read-var-info=yes --track-origins=yes --leak-check=full)

function(ADD_RDSPARSER_TEST TEST_NAME)
    add_executable(${TEST_NAME} ${TEST_NAME}.c)
    add_dependencies(${TEST_NAME} ${TEST_NAME} rdsparser)
    add_test(${TEST_NAME} ${TEST_NAME})
    add_test(${TEST_NAME}_valgrind ${Valgrind} ./${TEST_NAME})
    target_link_libraries(${TEST_NAME} rdsparser cmocka ${LIBRARIES})
endfunction()

add_rdsparser_test(test_af)
add_rdsparser_test(test_ct)
add_rdsparser_test(test_group)
add_rdsparser_test(test_group0)
add_rdsparser_test(test_group1)
add_rdsparser_test(test_group2)
add_rdsparser_test(test_group4)
add_rdsparser_test(test_group10)
add_rdsparser_test(test_librdsparser)
add_rdsparser_test(test_parser)
add_rdsparser_test(verification)
