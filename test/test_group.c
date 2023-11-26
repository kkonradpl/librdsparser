/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
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
#include "group.c"

static void
group_test_get_pi(void **state)
{
    rdsparser_data_t data;
    data[0] = 0x1234;

    assert_int_equal(rdsparser_group_get_pi(data), data[0]);
}

static void
group_test_get_pty_0(void **state)
{
    rdsparser_data_t data;
    data[1] = 0x040F;

    assert_int_equal(rdsparser_group_get_pty(data), 0);
}

static void
group_test_get_pty_10(void **state)
{
    rdsparser_data_t data;
    data[1] = 0x054A;

    assert_int_equal(rdsparser_group_get_pty(data), 10);
}

static void
group_test_get_tp_true(void **state)
{
    rdsparser_data_t data;
    data[1] = 0x2556;

    assert_int_equal(rdsparser_group_get_tp(data), true);
}

static void
group_test_get_tp_false(void **state)
{
    rdsparser_data_t data;
    data[1] = 0xE800;

    assert_int_equal(rdsparser_group_get_tp(data), false);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(group_test_get_pi, NULL, NULL),
    cmocka_unit_test_setup_teardown(group_test_get_pty_0, NULL, NULL),
    cmocka_unit_test_setup_teardown(group_test_get_pty_10, NULL, NULL),
    cmocka_unit_test_setup_teardown(group_test_get_tp_true, NULL, NULL),
    cmocka_unit_test_setup_teardown(group_test_get_tp_false, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
