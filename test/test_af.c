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
#include "af.c"

typedef struct {
    rdsparser_af_t af;
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
    rdsparser_af_clear(&ctx->af);
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
af_test_set_each_frequency(void **state)
{
    test_context_t *ctx = *state;
    uint8_t i = 0;

    do
    {
        assert_int_equal(rdsparser_af_get(&ctx->af, i), false);
        bool ret = (i >= 1 && i <= 204);
        assert_int_equal(rdsparser_af_set(&ctx->af, i), ret);
        assert_int_equal(rdsparser_af_get(&ctx->af, i), ret);
    } while (++i > 0);
}

static void
af_test_set_one_frequency(void **state)
{
    test_context_t *ctx = *state;

    const uint8_t id = 123;
    assert_int_equal(rdsparser_af_get(&ctx->af, id), false);
    rdsparser_af_set(&ctx->af, id);
    assert_int_equal(rdsparser_af_get(&ctx->af, id), true);

    for (uint8_t i = 0; i < RDSPARSER_AF_BUFFER_SIZE; i++)
    {
        if (i == (id / 8))
        {
            assert_int_equal(ctx->af.buffer[i], 0x80 >> (id % 8));
        }
        else
        {
            assert_int_equal(ctx->af.buffer[i], 0);
        }
    }
}

static void
af_test_set_clear(void **state)
{
    test_context_t *ctx = *state;

    const uint8_t id = 200;
    assert_int_equal(rdsparser_af_get(&ctx->af, id), false);
    assert_int_equal(rdsparser_af_set(&ctx->af, id), true);
    assert_int_equal(rdsparser_af_get(&ctx->af, id), true);
    rdsparser_af_clear(&ctx->af);
    assert_int_equal(rdsparser_af_get(&ctx->af, id), false);
}

static void
af_test_set_invalid_0(void **state)
{
    test_context_t *ctx = *state;

    const uint8_t id = 0;
    assert_int_equal(rdsparser_af_set(&ctx->af, id), false);

    for (uint8_t i = 0; i < RDSPARSER_AF_BUFFER_SIZE; i++)
    {
        assert_int_equal(ctx->af.buffer[i], 0);
    }
}

static void
af_test_set_invalid(void **state)
{
    test_context_t *ctx = *state;

    for (uint8_t id = 205; id != 0; id++)
    {
        assert_int_equal(rdsparser_af_set(&ctx->af, id), false);
    }

    for (uint8_t i = 0; i < RDSPARSER_AF_BUFFER_SIZE; i++)
    {
        assert_int_equal(ctx->af.buffer[i], 0);
    }
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(af_test_set_each_frequency, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_set_one_frequency, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_set_clear, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_set_invalid_0, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(af_test_set_invalid, test_setup, test_teardown)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
