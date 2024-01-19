/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
 *  Copyright (C) 2024  Konrad Kosmatka
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
#include "ct.c"

typedef struct {
    rdsparser_ct_t ct;
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
af_test_ct(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60369, 0, 0, 0), true);
    assert_int_equal(rdsparser_ct_get_year(&ctx->ct), 2024);
    assert_int_equal(rdsparser_ct_get_month(&ctx->ct), 2);
    assert_int_equal(rdsparser_ct_get_day(&ctx->ct), 29);
    assert_int_equal(rdsparser_ct_get_hour(&ctx->ct), 0);
    assert_int_equal(rdsparser_ct_get_minute(&ctx->ct), 0);
    assert_int_equal(rdsparser_ct_get_offset(&ctx->ct), 0);
}

static void
af_test_ct_invalid_hour(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 24, 30, 0), false);
}

static void
af_test_ct_invalid_minute(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 20, 60, 0), false);
}

static void
af_test_ct_zero_offset(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 11, 39, 0), true);
    assert_int_equal(rdsparser_ct_get_year(&ctx->ct), 2023);
    assert_int_equal(rdsparser_ct_get_month(&ctx->ct), 11);
    assert_int_equal(rdsparser_ct_get_day(&ctx->ct), 27);
    assert_int_equal(rdsparser_ct_get_hour(&ctx->ct), 11);
    assert_int_equal(rdsparser_ct_get_minute(&ctx->ct), 39);
    assert_int_equal(rdsparser_ct_get_offset(&ctx->ct), 0);
}

static void
af_test_ct_cet_offset(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 11, 39, 2), true);
    assert_int_equal(rdsparser_ct_get_year(&ctx->ct), 2023);
    assert_int_equal(rdsparser_ct_get_month(&ctx->ct), 11);
    assert_int_equal(rdsparser_ct_get_day(&ctx->ct), 27);
    assert_int_equal(rdsparser_ct_get_hour(&ctx->ct), 12);
    assert_int_equal(rdsparser_ct_get_minute(&ctx->ct), 39);
    assert_int_equal(rdsparser_ct_get_offset(&ctx->ct), 60);
}

static void
af_test_ct_positive_day_offset(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 23, 30, 2), true);
    assert_int_equal(rdsparser_ct_get_year(&ctx->ct), 2023);
    assert_int_equal(rdsparser_ct_get_month(&ctx->ct), 11);
    assert_int_equal(rdsparser_ct_get_day(&ctx->ct), 28);
    assert_int_equal(rdsparser_ct_get_hour(&ctx->ct), 0);
    assert_int_equal(rdsparser_ct_get_minute(&ctx->ct), 30);
    assert_int_equal(rdsparser_ct_get_offset(&ctx->ct), 60);
}

static void
af_test_ct_negative_day_offset(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 0, 30, -2), true);
    assert_int_equal(rdsparser_ct_get_year(&ctx->ct), 2023);
    assert_int_equal(rdsparser_ct_get_month(&ctx->ct), 11);
    assert_int_equal(rdsparser_ct_get_day(&ctx->ct), 26);
    assert_int_equal(rdsparser_ct_get_hour(&ctx->ct), 23);
    assert_int_equal(rdsparser_ct_get_minute(&ctx->ct), 30);
    assert_int_equal(rdsparser_ct_get_offset(&ctx->ct), -60);
}

static void
af_test_ct_positive_hour_offset(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 20, 30, 1), true);
    assert_int_equal(rdsparser_ct_get_year(&ctx->ct), 2023);
    assert_int_equal(rdsparser_ct_get_month(&ctx->ct), 11);
    assert_int_equal(rdsparser_ct_get_day(&ctx->ct), 27);
    assert_int_equal(rdsparser_ct_get_hour(&ctx->ct), 21);
    assert_int_equal(rdsparser_ct_get_minute(&ctx->ct), 0);
    assert_int_equal(rdsparser_ct_get_offset(&ctx->ct), 30);
}

static void
af_test_ct_negative_hour_offset(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 20, 0, -1), true);
    assert_int_equal(rdsparser_ct_get_year(&ctx->ct), 2023);
    assert_int_equal(rdsparser_ct_get_month(&ctx->ct), 11);
    assert_int_equal(rdsparser_ct_get_day(&ctx->ct), 27);
    assert_int_equal(rdsparser_ct_get_hour(&ctx->ct), 19);
    assert_int_equal(rdsparser_ct_get_minute(&ctx->ct), 30);
    assert_int_equal(rdsparser_ct_get_offset(&ctx->ct), -30);
}

static void
af_test_ct_positive_day_and_hour_offset(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 19, 30, 9), true);
    assert_int_equal(rdsparser_ct_get_year(&ctx->ct), 2023);
    assert_int_equal(rdsparser_ct_get_month(&ctx->ct), 11);
    assert_int_equal(rdsparser_ct_get_day(&ctx->ct), 28);
    assert_int_equal(rdsparser_ct_get_hour(&ctx->ct), 0);
    assert_int_equal(rdsparser_ct_get_minute(&ctx->ct), 0);
    assert_int_equal(rdsparser_ct_get_offset(&ctx->ct), 270);
}

static void
af_test_ct_negative_day_and_hour_offset(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_ct_init(&ctx->ct, 60275, 2, 30, -6), true);
    assert_int_equal(rdsparser_ct_get_year(&ctx->ct), 2023);
    assert_int_equal(rdsparser_ct_get_month(&ctx->ct), 11);
    assert_int_equal(rdsparser_ct_get_day(&ctx->ct), 26);
    assert_int_equal(rdsparser_ct_get_hour(&ctx->ct), 23);
    assert_int_equal(rdsparser_ct_get_minute(&ctx->ct), 30);
    assert_int_equal(rdsparser_ct_get_offset(&ctx->ct), -180);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(af_test_ct, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_invalid_hour, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_invalid_minute, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_zero_offset, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_cet_offset, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_positive_day_offset, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_negative_day_offset, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_positive_hour_offset, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_negative_hour_offset, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_positive_day_and_hour_offset, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_ct_negative_day_and_hour_offset, test_setup, test_teardown)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
