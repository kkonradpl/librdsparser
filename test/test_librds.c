/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librds – Radio Data System parser library
 *  Copyright (C) 2023  Konrad Kosmatka
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
#include "librds.h"
#include "parser.h"

typedef struct {
    librds_t rds;
} test_context_t;

static int
group_setup(void **state)
{
    test_context_t *ctx = calloc(sizeof(test_context_t), 1);
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
    librds_init(&ctx->rds);
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
callback_pi(librds_t *rds,
            void     *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_pty(librds_t *rds,
             void     *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_tp(librds_t *rds,
            void     *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ta(librds_t *rds,
            void     *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ms(librds_t *rds,
            void     *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ecc(librds_t *rds,
             void     *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_af(librds_t *rds,
            uint8_t   new_af,
            void     *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ps(librds_t *rds,
            void     *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_rt(librds_t         *rds,
            librds_rt_flag_t  flag,
            void             *user_data)
{
    (void)user_data;
    function_called();
}

/* ------------------------------------------------ */

static void
librds_test_reset(void **state)
{
    test_context_t *ctx = *state;

    assert_int_equal(librds_get_pi(&ctx->rds), LIBRDS_PI_UNKNOWN);
    assert_int_equal(librds_get_pty(&ctx->rds), LIBRDS_PTY_UNKNOWN);
    assert_int_equal(librds_get_tp(&ctx->rds), LIBRDS_TP_UNKNOWN);
    assert_int_equal(librds_get_ta(&ctx->rds), LIBRDS_TA_UNKNOWN);
    assert_int_equal(librds_get_ms(&ctx->rds), LIBRDS_MS_UNKNOWN);
    assert_int_equal(librds_get_ecc(&ctx->rds), LIBRDS_ECC_UNKNOWN);

    const librds_af_t *af = librds_get_af(&ctx->rds);
    for (uint8_t i = 0; i < LIBRDS_AF_BUFFER_SIZE; i++)
    {
        assert_int_equal((*af)[i], 0);
    }

    const librds_string_t *string;
    const librds_string_char_t *content;
    const librds_string_error_t *errors;

    string = librds_get_ps(&ctx->rds);
    content = librds_string_get_content(string);
    errors = librds_string_get_errors(string);
    for (uint8_t i = 0; i < LIBRDS_PS_LENGTH; i++)
    {
        assert_int_equal(content[i], ' ');
        assert_int_equal(errors[i], LIBRDS_STRING_ERROR_UNCORRECTABLE);
    }

    string = librds_get_rt(&ctx->rds, LIBRDS_RT_FLAG_A);
    content = librds_string_get_content(string);
    errors = librds_string_get_errors(string);
    for (uint8_t i = 0; i < LIBRDS_RT_LENGTH; i++)
    {
        assert_int_equal(content[i], ' ');
        assert_int_equal(errors[i], LIBRDS_STRING_ERROR_UNCORRECTABLE);
    }

    string = librds_get_rt(&ctx->rds, LIBRDS_RT_FLAG_B);
    content = librds_string_get_content(string);
    errors = librds_string_get_errors(string);
    for (uint8_t i = 0; i < LIBRDS_RT_LENGTH; i++)
    {
        assert_int_equal(content[i], ' ');
        assert_int_equal(errors[i], LIBRDS_STRING_ERROR_UNCORRECTABLE);
    }
}

static void
librds_test_parse_string_null(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_parse_string(&ctx->rds, NULL), false);
}

static void
librds_test_parse_string_empty(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_parse_string(&ctx->rds, ""), false);
}

static void
librds_test_parse_string_valid(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_parse_string(&ctx->rds, "01234567890ABCDE"), true);
    assert_int_equal(librds_parse_string(&ctx->rds, "01234567890ABCDEF0"), true);
}

static void
librds_test_parse_string_invalid(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_parse_string(&ctx->rds, "X234567890123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1Y34567890123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12Z4567890123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123#567890123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234$67890123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345R7890123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123456S890123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567P90123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678:0123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123456789;123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890*23456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678901 3456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123456789012\1456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123\n56"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678901234\t6"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123456789012345~"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123456.0"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678901234560X"), false);
}

static void
librds_test_parse_string_short(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_parse_string(&ctx->rds, "123456789012345"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678901234"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123456789012"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678901"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123456789"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234563"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123453"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12323"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1233"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1"), false);
}

static void
librds_test_parse_string_long(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678901234581"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123458123"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678901234581234"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123456789012345812345"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123458123456"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "12345678901234581234567"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "123456789012345812345678"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123458123456789"), false);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123458123456789012345678901234567890"), false);
}

static void
librds_test_ps_info_correction(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_NONE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_INFO, LIBRDS_BLOCK_ERROR_SMALL);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_SMALL);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_INFO, LIBRDS_BLOCK_ERROR_LARGE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_LARGE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_INFO, LIBRDS_BLOCK_ERROR_NONE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_NONE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_INFO, LIBRDS_BLOCK_ERROR_UNCORRECTABLE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_LARGE);
}

static void
librds_test_ps_data_correction(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_NONE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_DATA, LIBRDS_BLOCK_ERROR_SMALL);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_SMALL);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_DATA, LIBRDS_BLOCK_ERROR_LARGE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_LARGE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_DATA, LIBRDS_BLOCK_ERROR_NONE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_NONE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_DATA, LIBRDS_BLOCK_ERROR_UNCORRECTABLE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_PS, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_LARGE);
}


static void
librds_test_rt_info_correction(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_NONE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_INFO, LIBRDS_BLOCK_ERROR_SMALL);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_SMALL);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_INFO, LIBRDS_BLOCK_ERROR_LARGE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_LARGE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_INFO, LIBRDS_BLOCK_ERROR_NONE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_NONE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_INFO, LIBRDS_BLOCK_ERROR_UNCORRECTABLE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_INFO), LIBRDS_BLOCK_ERROR_LARGE);
}

static void
librds_test_rt_data_correction(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_NONE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_DATA, LIBRDS_BLOCK_ERROR_SMALL);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_SMALL);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_DATA, LIBRDS_BLOCK_ERROR_LARGE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_LARGE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_DATA, LIBRDS_BLOCK_ERROR_NONE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_NONE);
    librds_set_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_DATA, LIBRDS_BLOCK_ERROR_UNCORRECTABLE);
    assert_int_equal(librds_get_text_correction(&ctx->rds, LIBRDS_TEXT_RT, LIBRDS_BLOCK_TYPE_DATA), LIBRDS_BLOCK_ERROR_LARGE);
}

static void
librds_test_ps_progressive(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_get_text_progressive(&ctx->rds, LIBRDS_TEXT_PS), false);
    librds_set_text_progressive(&ctx->rds, LIBRDS_TEXT_PS, true);
    assert_int_equal(librds_get_text_progressive(&ctx->rds, LIBRDS_TEXT_PS), true);
    librds_set_text_progressive(&ctx->rds, LIBRDS_TEXT_PS, false);
    assert_int_equal(librds_get_text_progressive(&ctx->rds, LIBRDS_TEXT_PS), false);
}

static void
librds_test_rt_progressive(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(librds_get_text_progressive(&ctx->rds, LIBRDS_TEXT_RT), false);
    librds_set_text_progressive(&ctx->rds, LIBRDS_TEXT_RT, true);
    assert_int_equal(librds_get_text_progressive(&ctx->rds, LIBRDS_TEXT_RT), true);
    librds_set_text_progressive(&ctx->rds, LIBRDS_TEXT_RT, false);
    assert_int_equal(librds_get_text_progressive(&ctx->rds, LIBRDS_TEXT_RT), false);
}

static void
librds_test_register_pi(void **state)
{
    test_context_t *ctx = *state;
    librds_register_pi(&ctx->rds, callback_pi);
    expect_function_call(callback_pi);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123458"), true);
}

static void
librds_test_register_tp(void **state)
{
    test_context_t *ctx = *state;
    librds_register_tp(&ctx->rds, callback_tp);
    expect_function_call(callback_tp);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123458"), true);
}

static void
librds_test_register_ta(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ta(&ctx->rds, callback_ta);
    expect_function_call(callback_ta);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234000890123458"), true);
}

static void
librds_test_register_ms(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ta(&ctx->rds, callback_ms);
    expect_function_call(callback_ms);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234001890123458"), true);
}

static void
librds_test_register_pty(void **state)
{
    test_context_t *ctx = *state;
    librds_register_pty(&ctx->rds, callback_pty);
    expect_function_call(callback_pty);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234567890123458"), true);
}

static void
librds_test_register_ecc(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ecc(&ctx->rds, callback_ecc);
    expect_function_call(callback_ecc);
    assert_int_equal(librds_parse_string(&ctx->rds, "3566100000E20000"), true);
}

static void
librds_test_register_af(void **state)
{
    test_context_t *ctx = *state;
    librds_register_af(&ctx->rds, callback_af);
    expect_function_calls(callback_af, 2);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234007890123458"), true);
}

static void
librds_test_register_ps(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ps(&ctx->rds, callback_ps);
    expect_function_call(callback_ps);
    assert_int_equal(librds_parse_string(&ctx->rds, "1234007890123458"), true);
}

static void
librds_test_register_rt(void **state)
{
    test_context_t *ctx = *state;
    librds_register_rt(&ctx->rds, callback_rt);
    expect_function_call(callback_rt);
    assert_int_equal(librds_parse_string(&ctx->rds, "34DB255F3420303000"), true);
    expect_function_call(callback_rt);
    assert_int_equal(librds_parse_string(&ctx->rds, "34DB254F3420303000"), true);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(librds_test_reset, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_parse_string_null, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_parse_string_empty, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_parse_string_valid, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_parse_string_invalid, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_parse_string_short, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_parse_string_long, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_ps_info_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_ps_data_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_rt_info_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_rt_data_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_ps_progressive, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_rt_progressive, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_register_pi, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_register_tp, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_register_ta, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_register_ms, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_register_pty, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_register_ecc, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_register_af, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_register_ps, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(librds_test_register_rt, test_setup, test_teardown)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
