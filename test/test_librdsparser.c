/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
 *  Copyright (C) 2023-2024  Konrad Kosmatka
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
#include <librdsparser.h>
#include "parser.h"

typedef struct {
    rdsparser_t rds;
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
    rdsparser_init(&ctx->rds);
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
callback_pi(rdsparser_t *rds,
            void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_pty(rdsparser_t *rds,
             void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_tp(rdsparser_t *rds,
            void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ta(rdsparser_t *rds,
            void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ms(rdsparser_t *rds,
            void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ecc(rdsparser_t *rds,
             void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_country(rdsparser_t *rds,
                 void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_af(rdsparser_t *rds,
            uint32_t     new_af,
            void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ps(rdsparser_t *rds,
            void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_rt(rdsparser_t         *rds,
            rdsparser_rt_flag_t  flag,
            void                *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ptyn(rdsparser_t *rds,
              void        *user_data)
{
    (void)user_data;
    function_called();
}

static void
callback_ct(rdsparser_t          *rds,
            const rdsparser_ct_t *ct,
            void                 *user_data)
{
    (void)user_data;
    function_called();
}

/* ------------------------------------------------ */

static void
rdsparser_test_reset(void **state)
{
    test_context_t *ctx = *state;

    assert_int_equal(rdsparser_get_pi(&ctx->rds), RDSPARSER_PI_UNKNOWN);
    assert_int_equal(rdsparser_get_pty(&ctx->rds), RDSPARSER_PTY_UNKNOWN);
    assert_int_equal(rdsparser_get_tp(&ctx->rds), RDSPARSER_TP_UNKNOWN);
    assert_int_equal(rdsparser_get_ta(&ctx->rds), RDSPARSER_TA_UNKNOWN);
    assert_int_equal(rdsparser_get_ms(&ctx->rds), RDSPARSER_MS_UNKNOWN);
    assert_int_equal(rdsparser_get_ecc(&ctx->rds), RDSPARSER_ECC_UNKNOWN);
    assert_int_equal(rdsparser_get_country(&ctx->rds), RDSPARSER_COUNTRY_UNKNOWN);

    const rdsparser_af_t *af = rdsparser_get_af(&ctx->rds);
    for (uint8_t i = 0; i < RDSPARSER_AF_BUFFER_SIZE; i++)
    {
        assert_int_equal(af->buffer[i], 0);
    }

    const rdsparser_string_t *string;
    const rdsparser_string_char_t *content;
    const rdsparser_string_error_t *errors;

    string = rdsparser_get_ps(&ctx->rds);
    content = rdsparser_string_get_content(string);
    errors = rdsparser_string_get_errors(string);
    for (uint8_t i = 0; i < RDSPARSER_PS_LENGTH; i++)
    {
        assert_int_equal(content[i], ' ');
        assert_int_equal(errors[i], RDSPARSER_STRING_ERROR_UNCORRECTABLE);
    }

    string = rdsparser_get_rt(&ctx->rds, RDSPARSER_RT_FLAG_A);
    content = rdsparser_string_get_content(string);
    errors = rdsparser_string_get_errors(string);
    for (uint8_t i = 0; i < RDSPARSER_RT_LENGTH; i++)
    {
        assert_int_equal(content[i], ' ');
        assert_int_equal(errors[i], RDSPARSER_STRING_ERROR_UNCORRECTABLE);
    }

    string = rdsparser_get_rt(&ctx->rds, RDSPARSER_RT_FLAG_B);
    content = rdsparser_string_get_content(string);
    errors = rdsparser_string_get_errors(string);
    for (uint8_t i = 0; i < RDSPARSER_RT_LENGTH; i++)
    {
        assert_int_equal(content[i], ' ');
        assert_int_equal(errors[i], RDSPARSER_STRING_ERROR_UNCORRECTABLE);
    }

    string = rdsparser_get_ptyn(&ctx->rds);
    content = rdsparser_string_get_content(string);
    errors = rdsparser_string_get_errors(string);
    for (uint8_t i = 0; i < RDSPARSER_PTYN_LENGTH; i++)
    {
        assert_int_equal(content[i], ' ');
        assert_int_equal(errors[i], RDSPARSER_STRING_ERROR_UNCORRECTABLE);
    }
}

static void
rdsparser_test_parse_string_null(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_parse_string(&ctx->rds, NULL), false);
}

static void
rdsparser_test_parse_string_empty(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_parse_string(&ctx->rds, ""), false);
}

static void
rdsparser_test_parse_string_valid(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "01234567890ABCDE"), true);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "01234567890ABCDEF0"), true);
}

static void
rdsparser_test_parse_string_invalid(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "X234567890123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1Y34567890123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12Z4567890123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123#567890123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234$67890123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345R7890123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123456S890123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567P90123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678:0123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123456789;123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890*23456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678901 3456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123456789012\1456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123\n56"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678901234\t6"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123456789012345~"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123456.0"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678901234560X"), false);
}

static void
rdsparser_test_parse_string_short(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123456789012345"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678901234"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123456789012"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678901"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123456789"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234563"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123453"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12323"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1233"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1"), false);
}

static void
rdsparser_test_parse_string_long(void **state)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678901234581"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123458123"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678901234581234"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123456789012345812345"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123458123456"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "12345678901234581234567"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "123456789012345812345678"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123458123456789"), false);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123458123456789012345678901234567890"), false);
}

static void
test_correction(void                  **state,
                rdsparser_text_t        text,
                rdsparser_block_type_t  block_type)
{
    test_context_t *ctx = *state;

    assert_int_equal(rdsparser_get_text_correction(&ctx->rds, text, block_type), RDSPARSER_BLOCK_ERROR_NONE);
    rdsparser_set_text_correction(&ctx->rds, text, block_type, RDSPARSER_BLOCK_ERROR_SMALL);
    assert_int_equal(rdsparser_get_text_correction(&ctx->rds, text, block_type), RDSPARSER_BLOCK_ERROR_SMALL);
    rdsparser_set_text_correction(&ctx->rds, text, block_type, RDSPARSER_BLOCK_ERROR_LARGE);
    assert_int_equal(rdsparser_get_text_correction(&ctx->rds, text, block_type), RDSPARSER_BLOCK_ERROR_LARGE);
    rdsparser_set_text_correction(&ctx->rds, text, block_type, RDSPARSER_BLOCK_ERROR_NONE);
    assert_int_equal(rdsparser_get_text_correction(&ctx->rds, text, block_type), RDSPARSER_BLOCK_ERROR_NONE);
    rdsparser_set_text_correction(&ctx->rds, text, block_type, RDSPARSER_BLOCK_ERROR_UNCORRECTABLE);
    assert_int_equal(rdsparser_get_text_correction(&ctx->rds, text, block_type), RDSPARSER_BLOCK_ERROR_LARGE);
}

static void
rdsparser_test_ps_info_correction(void **state)
{
    test_correction(state, RDSPARSER_TEXT_PS, RDSPARSER_BLOCK_TYPE_INFO);
}

static void
rdsparser_test_ps_data_correction(void **state)
{

    test_correction(state, RDSPARSER_TEXT_PS, RDSPARSER_BLOCK_TYPE_DATA);
}

static void
rdsparser_test_rt_info_correction(void **state)
{
    test_correction(state, RDSPARSER_TEXT_RT, RDSPARSER_BLOCK_TYPE_INFO);
}

static void
rdsparser_test_rt_data_correction(void **state)
{
    test_correction(state, RDSPARSER_TEXT_RT, RDSPARSER_BLOCK_TYPE_DATA);
}

static void
rdsparser_test_ptyn_info_correction(void **state)
{
    test_correction(state, RDSPARSER_TEXT_PTYN, RDSPARSER_BLOCK_TYPE_INFO);
}

static void
rdsparser_test_ptyn_data_correction(void **state)
{
    test_correction(state, RDSPARSER_TEXT_PTYN, RDSPARSER_BLOCK_TYPE_DATA);
}

static void
test_progressive(void             **state,
                 rdsparser_text_t   text)
{
    test_context_t *ctx = *state;
    assert_int_equal(rdsparser_get_text_progressive(&ctx->rds, RDSPARSER_TEXT_PS), false);
    rdsparser_set_text_progressive(&ctx->rds, RDSPARSER_TEXT_PS, true);
    assert_int_equal(rdsparser_get_text_progressive(&ctx->rds, RDSPARSER_TEXT_PS), true);
    rdsparser_set_text_progressive(&ctx->rds, RDSPARSER_TEXT_PS, false);
    assert_int_equal(rdsparser_get_text_progressive(&ctx->rds, RDSPARSER_TEXT_PS), false);
}

static void
rdsparser_test_ps_progressive(void **state)
{
    test_progressive(state, RDSPARSER_TEXT_PS);
}

static void
rdsparser_test_rt_progressive(void **state)
{
    test_progressive(state, RDSPARSER_TEXT_RT);
}

static void
rdsparser_test_ptyn_progressive(void **state)
{
    test_progressive(state, RDSPARSER_TEXT_PTYN);
}

static void
rdsparser_test_register_pi(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_pi(&ctx->rds, callback_pi);
    expect_function_call(callback_pi);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123458"), true);
}

static void
rdsparser_test_register_tp(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_tp(&ctx->rds, callback_tp);
    expect_function_call(callback_tp);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123458"), true);
}

static void
rdsparser_test_register_ta(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_ta(&ctx->rds, callback_ta);
    expect_function_call(callback_ta);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234000890123458"), true);
}

static void
rdsparser_test_register_ms(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_ta(&ctx->rds, callback_ms);
    expect_function_call(callback_ms);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234001890123458"), true);
}

static void
rdsparser_test_register_pty(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_pty(&ctx->rds, callback_pty);
    expect_function_call(callback_pty);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234567890123458"), true);
}

static void
rdsparser_test_register_ecc(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_ecc(&ctx->rds, callback_ecc);
    expect_function_call(callback_ecc);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "3566100000E20000"), true);
}

static void
rdsparser_test_register_country(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_country(&ctx->rds, callback_country);
    expect_function_call(callback_country);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "34DB154000E2000000"), true);
}

static void
rdsparser_test_register_af(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_af(&ctx->rds, callback_af);
    expect_function_calls(callback_af, 2);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234007890123458"), true);
}

static void
rdsparser_test_register_ps(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_ps(&ctx->rds, callback_ps);
    expect_function_call(callback_ps);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "1234007890123458"), true);
}

static void
rdsparser_test_register_rt(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_rt(&ctx->rds, callback_rt);
    expect_function_call(callback_rt);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "34DB255F3420303000"), true);
    expect_function_call(callback_rt);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "34DB254F3420303000"), true);
}

static void
rdsparser_test_register_ptyn(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_ptyn(&ctx->rds, callback_ptyn);
    expect_function_call(callback_ptyn);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "34DBA5505241444900"), true);
}

static void
rdsparser_test_register_ct(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_register_ct(&ctx->rds, callback_ct);
    expect_function_call(callback_ct);
    assert_int_equal(rdsparser_parse_string(&ctx->rds, "3F444541D7500580"), true);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(rdsparser_test_reset, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_parse_string_null, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_parse_string_empty, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_parse_string_valid, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_parse_string_invalid, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_parse_string_short, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_parse_string_long, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_ps_info_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_ps_data_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_rt_info_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_rt_data_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_ptyn_info_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_ptyn_data_correction, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_ps_progressive, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_rt_progressive, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_ptyn_progressive, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_pi, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_tp, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_ta, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_ms, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_pty, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_ecc, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_country, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_af, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_ps, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_rt, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_ptyn, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(rdsparser_test_register_ct, test_setup, test_teardown)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
