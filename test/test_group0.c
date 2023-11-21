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
#include "group0.c"

static void
group0_test_get_ta_false(void **state)
{
    librds_data_t data;
    data[1] = 0x054F;

    assert_int_equal(librds_group0_get_ta(data), false);
}

static void
group0_test_get_ta_true(void **state)
{
    librds_data_t data;
    data[1] = 0x055F;

    assert_int_equal(librds_group0_get_ta(data), true);
}

static void
group0_test_get_ms_true(void **state)
{
    librds_data_t data;
    data[1] = 0x054F;

    assert_int_equal(librds_group0_get_ms(data), true);
}

static void
group0_test_get_ms_false(void **state)
{
    librds_data_t data;
    data[1] = 0x0547;

    assert_int_equal(librds_group0_get_ms(data), false);
}

static void
group0_test_get_af1(void **state)
{
    librds_data_t data;
    data[1] = 0x0547;
    data[2] = 0xE4A4;

    assert_int_equal(librds_group0_get_af1(data), 0xE4);
}

static void
group0_test_get_af2(void **state)
{
    librds_data_t data;
    data[1] = 0x0547;
    data[2] = 0xE4A4;

    assert_int_equal(librds_group0_get_af2(data), 0xA4);
}

static void
group0_test_get_ps_pos_0(void **state)
{
    librds_data_t data;
    data[1] = 0x0544;

    assert_int_equal(librds_group0_get_ps_pos(data), 0);
}

static void
group0_test_get_ps_pos_3(void **state)
{
    librds_data_t data;
    data[1] = 0x0547;

    assert_int_equal(librds_group0_get_ps_pos(data), 3);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(group0_test_get_ta_true, NULL, NULL),
    cmocka_unit_test_setup_teardown(group0_test_get_ta_false, NULL, NULL),
    cmocka_unit_test_setup_teardown(group0_test_get_ms_true, NULL, NULL),
    cmocka_unit_test_setup_teardown(group0_test_get_ms_false, NULL, NULL),
    cmocka_unit_test_setup_teardown(group0_test_get_af1, NULL, NULL),
    cmocka_unit_test_setup_teardown(group0_test_get_af2, NULL, NULL),
    cmocka_unit_test_setup_teardown(group0_test_get_ps_pos_0, NULL, NULL),
    cmocka_unit_test_setup_teardown(group0_test_get_ps_pos_3, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
