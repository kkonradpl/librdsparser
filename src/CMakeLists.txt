cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
        af.c
        af.h
        group.c
        group.h
        group0.c
        group0.h
        group1.c
        group1.h
        group2.c
        group2.h
        librds.c
        parser.c
        parser.h
        string.c
        string.h
        utils.c
        utils.h)

add_library(rds SHARED ${SOURCE_FILES})
set_target_properties(rds PROPERTIES PUBLIC_HEADER librds.h)

add_library(rds_static STATIC ${SOURCE_FILES})
set_target_properties(rds_static PROPERTIES PUBLIC_HEADER librds.h)

if(LIBRDS_DISABLE_HEAP)
    set_target_properties(rds PROPERTIES PUBLIC_HEADER librds_private.h)
    set_target_properties(rds_static PROPERTIES PUBLIC_HEADER librds_private.h)
endif()