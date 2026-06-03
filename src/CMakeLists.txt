if (RDSPARSER_BUILD_STATIC)
    add_library(rdsparser STATIC)
else()
    add_library(rdsparser SHARED)
endif()

target_sources(rdsparser PRIVATE
        af.c
        buffer.c
        country.c
        ct.c
        ecc.c
        group.c
        group0.c
        group1.c
        group2.c
        group4.c
        group10.c
        group15.c
        lps.c
        parser.c
        pty.c
        rdsparser.c
        string.c
        utils.c
)

if(RDSPARSER_DISABLE_HEAP)
    target_compile_definitions(rdsparser PUBLIC RDSPARSER_DISABLE_HEAP)    
endif()

if(RDSPARSER_DISABLE_UNICODE)
    target_compile_definitions(rdsparser PUBLIC RDSPARSER_DISABLE_UNICODE)
endif()
