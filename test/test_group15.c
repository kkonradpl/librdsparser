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
#include "group15.c"

static void
group15a_test_get_lps_pos_0(void **state)
{
    rdsparser_data_t data;
    data[RDSPARSER_BLOCK_B] = 0xF460;

    assert_int_equal(rdsparser_group15a_get_lps_pos(data), 0);
}

static void
group15a_test_get_lps_pos_1(void **state)
{
    rdsparser_data_t data;
    data[RDSPARSER_BLOCK_B] = 0xF461;

    assert_int_equal(rdsparser_group15a_get_lps_pos(data), 1);
}

static void
group15a_test_get_lps_pos_7(void **state)
{
    rdsparser_data_t data;
    data[RDSPARSER_BLOCK_B] = 0xF467;

    assert_int_equal(rdsparser_group15a_get_lps_pos(data), 7);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(group15a_test_get_lps_pos_0, NULL, NULL),
    cmocka_unit_test_setup_teardown(group15a_test_get_lps_pos_1, NULL, NULL),
    cmocka_unit_test_setup_teardown(group15a_test_get_lps_pos_7, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
