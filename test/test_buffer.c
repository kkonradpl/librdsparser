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
#include "buffer.c"

typedef struct {
    rdsparser_buffer_t buffer;
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
    rdsparser_buffer_init(&ctx->buffer);
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
buffer_test_clear(void **state)
{
    test_context_t *ctx = *state;

    rdsparser_buffer_clear(&ctx->buffer);

    assert_int_equal(rdsparser_buffer_get_pi(&ctx->buffer), RDSPARSER_PI_UNKNOWN);
    assert_int_equal(rdsparser_buffer_get_pty(&ctx->buffer), RDSPARSER_PTY_UNKNOWN);
    assert_int_equal(rdsparser_buffer_get_tp(&ctx->buffer), RDSPARSER_TP_UNKNOWN);
    assert_int_equal(rdsparser_buffer_get_ta(&ctx->buffer), RDSPARSER_TA_UNKNOWN);
    assert_int_equal(rdsparser_buffer_get_ms(&ctx->buffer), RDSPARSER_MS_UNKNOWN);
    assert_int_equal(rdsparser_buffer_get_ecc(&ctx->buffer), RDSPARSER_ECC_UNKNOWN);
    assert_int_equal(rdsparser_buffer_get_country(&ctx->buffer), RDSPARSER_COUNTRY_UNKNOWN);

    const rdsparser_af_t *af = rdsparser_buffer_get_af(&ctx->buffer);
    for (uint8_t i = 0; i < RDSPARSER_AF_BUFFER_SIZE; i++)
    {
        assert_int_equal(af->buffer[i], 0);
    }
}

static void
buffer_test_extended_check(void **state)
{
    test_context_t *ctx = *state;

    assert_int_equal(rdsparser_buffer_get_extended_check(&ctx->buffer), false);
    rdsparser_buffer_set_extended_check(&ctx->buffer, true);
    assert_int_equal(rdsparser_buffer_get_extended_check(&ctx->buffer), true);
    rdsparser_buffer_set_extended_check(&ctx->buffer, false);
    assert_int_equal(rdsparser_buffer_get_extended_check(&ctx->buffer), false);
}

static void
buffer_test_update_pi(void **state)
{
    test_context_t *ctx = *state;
    
    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0xDEAD), true);
    assert_int_equal(rdsparser_buffer_get_pi(&ctx->buffer), 0xDEAD);
    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0xDEAD), false);
}

static void
buffer_test_update_pi_extended_check(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_buffer_set_extended_check(&ctx->buffer, true);

    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0xDEAD), false);
    assert_int_equal(rdsparser_buffer_get_pi(&ctx->buffer), RDSPARSER_PI_UNKNOWN);

    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0xDEAD), true);
    assert_int_equal(rdsparser_buffer_get_pi(&ctx->buffer), 0xDEAD);

    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0xDEAD), false);
    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0xBEEF), false);
    assert_int_equal(rdsparser_buffer_get_pi(&ctx->buffer), 0xDEAD);

    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0xBEEF), true);
    assert_int_equal(rdsparser_buffer_get_pi(&ctx->buffer), 0xBEEF);

    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0x1234), false);
    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0x5677), false);
    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0x0000), false);
    assert_int_equal(rdsparser_buffer_update_pi(&ctx->buffer, 0xFFFF), false);
    assert_int_equal(rdsparser_buffer_get_pi(&ctx->buffer), 0xBEEF);
}

static void
buffer_test_update_pty(void **state)
{
    test_context_t *ctx = *state;
    
    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 5), true);
    assert_int_equal(rdsparser_buffer_get_pty(&ctx->buffer), 5);
    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 5), false);
}

static void
buffer_test_update_pty_extended_check(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_buffer_set_extended_check(&ctx->buffer, true);

    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 4), false);
    assert_int_equal(rdsparser_buffer_get_pty(&ctx->buffer), RDSPARSER_PTY_UNKNOWN);

    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 4), true);
    assert_int_equal(rdsparser_buffer_get_pty(&ctx->buffer), 4);

    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 4), false);
    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 7), false);
    assert_int_equal(rdsparser_buffer_get_pty(&ctx->buffer), 4);

    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 7), true);
    assert_int_equal(rdsparser_buffer_get_pty(&ctx->buffer), 7);

    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 8), false);
    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 4), false);
    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 2), false);
    assert_int_equal(rdsparser_buffer_update_pty(&ctx->buffer, 0), false);
    assert_int_equal(rdsparser_buffer_get_pty(&ctx->buffer), 7);
}

static void
buffer_test_update_tp(void **state)
{
    test_context_t *ctx = *state;
    
    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_ON), true);
    assert_int_equal(rdsparser_buffer_get_tp(&ctx->buffer), RDSPARSER_TP_ON);
    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_ON), false);
}

static void
buffer_test_update_tp_extended_check(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_buffer_set_extended_check(&ctx->buffer, true);

    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_ON), false);
    assert_int_equal(rdsparser_buffer_get_tp(&ctx->buffer), RDSPARSER_TP_UNKNOWN);

    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_ON), true);
    assert_int_equal(rdsparser_buffer_get_tp(&ctx->buffer), RDSPARSER_TP_ON);

    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_ON), false);
    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_OFF), false);
    assert_int_equal(rdsparser_buffer_get_tp(&ctx->buffer), RDSPARSER_TP_ON);

    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_OFF), true);
    assert_int_equal(rdsparser_buffer_get_tp(&ctx->buffer), RDSPARSER_TP_OFF);

    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_ON), false);
    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_OFF), false);
    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_ON), false);
    assert_int_equal(rdsparser_buffer_update_tp(&ctx->buffer, RDSPARSER_TP_OFF), false);
    assert_int_equal(rdsparser_buffer_get_tp(&ctx->buffer), RDSPARSER_TP_OFF);
}

static void
buffer_test_update_ta(void **state)
{
    test_context_t *ctx = *state;
    
    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_ON), true);
    assert_int_equal(rdsparser_buffer_get_ta(&ctx->buffer), RDSPARSER_TA_ON);
    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_ON), false);
}

static void
buffer_test_update_ta_extended_check(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_buffer_set_extended_check(&ctx->buffer, true);

    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_ON), false);
    assert_int_equal(rdsparser_buffer_get_ta(&ctx->buffer), RDSPARSER_TA_UNKNOWN);

    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_ON), true);
    assert_int_equal(rdsparser_buffer_get_ta(&ctx->buffer), RDSPARSER_TA_ON);

    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_ON), false);
    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_OFF), false);
    assert_int_equal(rdsparser_buffer_get_ta(&ctx->buffer), RDSPARSER_TA_ON);

    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_OFF), true);
    assert_int_equal(rdsparser_buffer_get_ta(&ctx->buffer), RDSPARSER_TA_OFF);

    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_ON), false);
    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_OFF), false);
    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_ON), false);
    assert_int_equal(rdsparser_buffer_update_ta(&ctx->buffer, RDSPARSER_TA_OFF), false);
    assert_int_equal(rdsparser_buffer_get_ta(&ctx->buffer), RDSPARSER_TA_OFF);
}

static void
buffer_test_update_ms(void **state)
{
    test_context_t *ctx = *state;
    
    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_MUSIC), true);
    assert_int_equal(rdsparser_buffer_get_ms(&ctx->buffer), RDSPARSER_MS_MUSIC);
    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_MUSIC), false);
}

static void
buffer_test_update_ms_extended_check(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_buffer_set_extended_check(&ctx->buffer, true);

    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_MUSIC), false);
    assert_int_equal(rdsparser_buffer_get_ms(&ctx->buffer), RDSPARSER_MS_UNKNOWN);

    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_MUSIC), true);
    assert_int_equal(rdsparser_buffer_get_ms(&ctx->buffer), RDSPARSER_MS_MUSIC);

    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_MUSIC), false);
    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_SPEECH), false);
    assert_int_equal(rdsparser_buffer_get_ms(&ctx->buffer), RDSPARSER_MS_MUSIC);

    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_SPEECH), true);
    assert_int_equal(rdsparser_buffer_get_ms(&ctx->buffer), RDSPARSER_MS_SPEECH);

    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_MUSIC), false);
    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_SPEECH), false);
    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_MUSIC), false);
    assert_int_equal(rdsparser_buffer_update_ms(&ctx->buffer, RDSPARSER_MS_SPEECH), false);
    assert_int_equal(rdsparser_buffer_get_ms(&ctx->buffer), RDSPARSER_MS_SPEECH);
}

static void
buffer_test_update_ecc(void **state)
{
    test_context_t *ctx = *state;
    
    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xE2), true);
    assert_int_equal(rdsparser_buffer_get_ecc(&ctx->buffer), 0xE2);
    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xE2), false);
}

static void
buffer_test_update_ecc_extended_check(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_buffer_set_extended_check(&ctx->buffer, true);

    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xE3), false);
    assert_int_equal(rdsparser_buffer_get_ecc(&ctx->buffer), RDSPARSER_ECC_UNKNOWN);

    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xE3), true);
    assert_int_equal(rdsparser_buffer_get_ecc(&ctx->buffer), 0xE3);

    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xE3), false);
    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xE0), false);
    assert_int_equal(rdsparser_buffer_get_ecc(&ctx->buffer), 0xE3);

    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xE0), true);
    assert_int_equal(rdsparser_buffer_get_ecc(&ctx->buffer), 0xE0);

    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xD0), false);
    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xA0), false);
    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xA3), false);
    assert_int_equal(rdsparser_buffer_update_ecc(&ctx->buffer, 0xE4), false);
    assert_int_equal(rdsparser_buffer_get_ecc(&ctx->buffer), 0xE0);
}

static void
buffer_test_update_country(void **state)
{
    test_context_t *ctx = *state;
    
    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_POLAND), true);
    assert_int_equal(rdsparser_buffer_get_country(&ctx->buffer), RDSPARSER_COUNTRY_POLAND);
    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_POLAND), false);
}

static void
buffer_test_update_country_extended_check(void **state)
{
    test_context_t *ctx = *state;
    rdsparser_buffer_set_extended_check(&ctx->buffer, true);

    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_POLAND), false);
    assert_int_equal(rdsparser_buffer_get_country(&ctx->buffer), RDSPARSER_COUNTRY_UNKNOWN);

    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_POLAND), true);
    assert_int_equal(rdsparser_buffer_get_country(&ctx->buffer), RDSPARSER_COUNTRY_POLAND);

    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_POLAND), false);
    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_CZECHIA), false);
    assert_int_equal(rdsparser_buffer_get_country(&ctx->buffer), RDSPARSER_COUNTRY_POLAND);

    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_CZECHIA), true);
    assert_int_equal(rdsparser_buffer_get_country(&ctx->buffer), RDSPARSER_COUNTRY_CZECHIA);

    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_SWEDEN), false);
    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_FINLAND), false);
    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_ESTONIA), false);
    assert_int_equal(rdsparser_buffer_update_country(&ctx->buffer, RDSPARSER_COUNTRY_LATVIA), false);
    assert_int_equal(rdsparser_buffer_get_country(&ctx->buffer), RDSPARSER_COUNTRY_CZECHIA);
}

static void
buffer_test_add_af(void **state)
{
    test_context_t *ctx = *state;
    const rdsparser_af_t *af = rdsparser_buffer_get_af(&ctx->buffer);

    assert_int_equal(rdsparser_af_get(af, 123), false);
    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 123), true);
    assert_int_equal(rdsparser_af_get(af, 123), true);
    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 123), false);
}

static void
buffer_test_add_af_extended_check(void **state)
{
    test_context_t *ctx = *state;
    const rdsparser_af_t *af = rdsparser_buffer_get_af(&ctx->buffer);
    rdsparser_buffer_set_extended_check(&ctx->buffer, true);

    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 50), false);
    assert_int_equal(rdsparser_af_get(af, 50), false);

    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 50), true);
    assert_int_equal(rdsparser_af_get(af, 50), true);

    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 50), false);
    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 51), false);
    assert_int_equal(rdsparser_af_get(af, 51), false);

    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 51), true);
    assert_int_equal(rdsparser_af_get(af, 51), true);
    
    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 51), false);
    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 52), false);
    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 53), false);
    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 54), false);
    assert_int_equal(rdsparser_af_get(af, 50), true);
    assert_int_equal(rdsparser_af_get(af, 51), true);
    assert_int_equal(rdsparser_af_get(af, 52), false);
    assert_int_equal(rdsparser_af_get(af, 53), false);
    assert_int_equal(rdsparser_af_get(af, 54), false);

    assert_int_equal(rdsparser_buffer_add_af(&ctx->buffer, 52), true);
    assert_int_equal(rdsparser_af_get(af, 52), true);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(buffer_test_clear, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_extended_check, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_pi, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_pi_extended_check, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_pty, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_pty_extended_check, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_tp, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_tp_extended_check, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_ta, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_ta_extended_check, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_ms, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_ms_extended_check, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_ecc, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_ecc_extended_check, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_country, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_update_country_extended_check, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_add_af, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(buffer_test_add_af_extended_check, test_setup, test_teardown)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
