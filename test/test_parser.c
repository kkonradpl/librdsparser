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
#include "parser.c"

static void
parser_test_get_group_2(void **state)
{
    rdsparser_data_t data;
    data[1] = 0x2556;

    assert_int_equal(rdsparser_parser_get_group(data), 2);
}

static void
parser_test_get_group_15(void **state)
{
    rdsparser_data_t data;
    data[1] = 0xF808;

    assert_int_equal(rdsparser_parser_get_group(data), 15);
}

static void
parser_test_get_flag_a(void **state)
{
    rdsparser_data_t data;
    data[1] = 0xE00E;

    assert_int_equal(rdsparser_parser_get_flag(data), RDSPARSER_GROUP_FLAG_A);
}

static void
parser_test_get_flag_b(void **state)
{
    rdsparser_data_t data;
    data[1] = 0xE800;

    assert_int_equal(rdsparser_parser_get_flag(data), RDSPARSER_GROUP_FLAG_B);
}


const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(parser_test_get_group_2, NULL, NULL),
    cmocka_unit_test_setup_teardown(parser_test_get_group_15, NULL, NULL),
    cmocka_unit_test_setup_teardown(parser_test_get_flag_a, NULL, NULL),
    cmocka_unit_test_setup_teardown(parser_test_get_flag_b, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
