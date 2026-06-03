set(SOURCE_FILES
        af.c
        af.h
        buffer.c
        buffer.h
        country.c
        ct.c
        ct.h
        ecc.c
        ecc.h
        group.c
        group.h
        group0.c
        group0.h
        group1.c
        group1.h
        group2.c
        group2.h
        group4.c
        group4.h
        group10.c
        group10.h
        group15.c
        group15.h
        lps.c
        lps.h
        rdsparser.c
        parser.c
        parser.h
        pty.c
        string.c
        string.h
        utils.c
        utils.h)

if (RDSPARSER_BUILD_STATIC)
    add_library(rdsparser STATIC ${SOURCE_FILES})
else()
    add_library(rdsparser SHARED ${SOURCE_FILES})
endif()

if(RDSPARSER_DISABLE_HEAP)
    target_compile_definitions(rdsparser PUBLIC RDSPARSER_DISABLE_HEAP)    
endif()

if(RDSPARSER_DISABLE_UNICODE)
    target_compile_definitions(rdsparser PUBLIC RDSPARSER_DISABLE_UNICODE)
endif()
