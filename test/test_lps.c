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
#include "lps.c"

typedef struct {
    rdsparser_lps_t lps;
} test_context_t;

static int
group_setup(void **state)
{
    test_context_t *ctx = malloc(sizeof(test_context_t));
    *state = ctx;
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
    rdsparser_lps_init(&ctx->lps);
    (void)ctx;
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
lps_test_none(void **state)
{
    test_context_t *ctx = *state;
    assert_null(rdsparser_lps_get(&ctx->lps));
}

static void
lps_test_empty(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_lps_update(&ctx->lps, 0, '\r');
    assert_string_equal(rdsparser_lps_get(&ctx->lps), "");
}

static void
lps_test_incomplete(void **state)
{
    test_context_t *ctx = *state;

    for (int i = 0; i < RDSPARSER_LPS_MAX_LENGTH; i++)
    {
        assert_null(rdsparser_lps_get(&ctx->lps));
        if (i != 13)
        {
            rdsparser_lps_update(&ctx->lps, i, 'x');
        }
    }

    assert_null(rdsparser_lps_get(&ctx->lps));
}

static void
lps_test_max_length(void **state)
{
    test_context_t *ctx = *state;
    for (int i = 0; i < RDSPARSER_LPS_MAX_LENGTH; i++)
    {
        assert_null(rdsparser_lps_get(&ctx->lps));
        rdsparser_lps_update(&ctx->lps, i, 'x');
    }

    assert_string_equal(rdsparser_lps_get(&ctx->lps), "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(lps_test_none, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(lps_test_empty, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(lps_test_incomplete, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(lps_test_max_length, test_setup, test_teardown),
};

int
main(void)
{
    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
