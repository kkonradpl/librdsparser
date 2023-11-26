#ifndef RDSPARSER_TEST_ASSERTS_H
#define RDSPARSER_TEST_ASSERTS_H
#include <wchar.h>

#ifdef RDSPARSER_DISABLE_UNICODE
#define assert_rds_string_equal(current, expected) \
    { \
        size_t length = wcslen(expected); \
        rdsparser_string_char_t *ascii = calloc(length + 1, 1); \
        for (size_t i = 0; i < length; i++) \
        { \
            ascii[i] = ((expected[i] >= 0x7F) ? ' ' : (char)expected[i]); \
        } \
        assert_string_equal((const char*)current, (const char*)ascii); \
        free(ascii); \
    }
#else
#define assert_rds_string_equal(current, expected) \
    if (wcscmp(current, expected) != 0) \
    { \
        fail_msg("\"%ls\" != \"%ls\"", current, expected); \
    }
#endif

#endif