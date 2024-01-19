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
#include "group4.c"

static void
group4a_test_get_mjd(void **state)
{
    rdsparser_data_t data;
    data[RDSPARSER_BLOCK_B] = 0x4541;
    data[RDSPARSER_BLOCK_C] = 0xD6E6;

    assert_int_equal(rdsparser_group4a_get_mjd(data), 60275);
}

static void
group4a_test_get_hour(void **state)
{
    rdsparser_data_t data;
    data[RDSPARSER_BLOCK_C] = 0xD6E6;
    data[RDSPARSER_BLOCK_D] = 0xB9C2;

    assert_int_equal(rdsparser_group4a_get_hour(data), 11);
}

static void
group4a_test_get_minute(void **state)
{
    rdsparser_data_t data;
    data[RDSPARSER_BLOCK_D] = 0xB9C2;

    assert_int_equal(rdsparser_group4a_get_minute(data), 39);
}

static void
group4a_test_get_time_offset(void **state)
{
    rdsparser_data_t data;
    data[RDSPARSER_BLOCK_D] = 0xB9C2;

    assert_int_equal(rdsparser_group4a_get_time_offset(data), 2);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(group4a_test_get_mjd, NULL, NULL),
    cmocka_unit_test_setup_teardown(group4a_test_get_hour, NULL, NULL),
    cmocka_unit_test_setup_teardown(group4a_test_get_minute, NULL, NULL),
    cmocka_unit_test_setup_teardown(group4a_test_get_time_offset, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
