/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
 *  Copyright (C) 2025  Konrad Kosmatka
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <locale.h>
#include <librdsparser_private.h>
#include "dataset/tef6687_static_ps.h"
#include "asserts.h"

typedef struct {
    rdsparser_t rds;
    wchar_t ps[9];
} test_context_t;

static int
group_setup(void **state)
{
    test_context_t *ctx = calloc(sizeof(test_context_t), 1);
    *state = ctx;

#ifndef RDSPARSER_DISABLE_UNICODE
    setlocale(LC_ALL, "");
#endif

    return 0;
}

static int
group_teardown(void **state)
{
    test_context_t *ctx = *state;
    free(ctx);
    return 0;
}

static int
test_setup(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_init(&ctx->rds);
    rdsparser_set_user_data(&ctx->rds, ctx);
    return 0;
}

static int
test_teardown(void **state)
{
    test_context_t *ctx = *state;
    (void)ctx;
    return 0;
}

static void
callback_static_ps(rdsparser_t *rds,
                   void        *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    const rdsparser_string_t *string = rdsparser_get_ps(&ctx->rds);
    const uint8_t length = rdsparser_string_get_length(string);
    const rdsparser_string_char_t *content = rdsparser_string_get_content(string);
    const rdsparser_string_error_t *errors = rdsparser_string_get_errors(string);

    function_called();

    char pre = (rdsparser_get_text_progressive(&ctx->rds, RDSPARSER_TEXT_PS) ? '(' : '[');
    char post = (rdsparser_get_text_progressive(&ctx->rds, RDSPARSER_TEXT_PS) ? ')' : ']');

#ifdef RDSPARSER_DISABLE_UNICODE
    printf("PS: %c%s%c", pre, content, post);
#else
    printf("PS: %c%ls%c", pre, content, post);
#endif

    printf(" (");
    for (uint8_t i = 0; i < length; i++)
    {
        printf("%d", errors[i]);
        if (i != length - 1)
            printf(",");
    }
    printf(")\n");

    bool error = false;
    for (uint8_t i = 0; i < rdsparser_string_get_length(string); i++)
    {
        if (errors[i] != RDSPARSER_STRING_ERROR_NONE)
        {
            error = true;
            break;
        }
    }

    if (!error)
    {
        assert_rds_string_equal(content, ctx->ps);
    }
}

static void
check_static_ps(test_context_t *ctx,
                const char     *dataset[],
                const wchar_t  *expected)
{
    expect_function_call(callback_static_ps);
    swprintf(ctx->ps, sizeof(ctx->ps), expected);

    expect_function_call_any(callback_static_ps);
    for (const char **ptr = dataset; *ptr != NULL; ptr++)
    {
        assert_int_equal(rdsparser_parse_string(&ctx->rds, *ptr), true);
    }
}

static void
adaptive_ps_test_static_ps(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_ps(&ctx->rds, callback_static_ps);

    rdsparser_set_text_correction(&ctx->rds, RDSPARSER_TEXT_PS, RDSPARSER_BLOCK_TYPE_INFO, RDSPARSER_BLOCK_ERROR_LARGE);
    rdsparser_set_text_correction(&ctx->rds, RDSPARSER_TEXT_PS, RDSPARSER_BLOCK_TYPE_DATA, RDSPARSER_BLOCK_ERROR_LARGE);
    rdsparser_set_text_progressive(&ctx->rds, RDSPARSER_TEXT_PS, RDSPARSER_PROGRESSIVE_AUTO);

    check_static_ps(ctx, rdsparser_test_dataset_tef6687_static_ps, L"RADIO 7 ");
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(adaptive_ps_test_static_ps, test_setup, test_teardown)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
