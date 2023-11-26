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
#include "group1.c"


static void
group1a_test_get_variant(void **state)
{
    rdsparser_data_t data;
    data[2] = 0x00E2;

    assert_int_equal(rdsparser_group1a_get_variant(data), 0);
}

static void
group1a0_test_get_ecc(void **state)
{
    rdsparser_data_t data;
    data[2] = 0x00E2;

    assert_int_equal(rdsparser_group1a0_get_ecc(data), 0xE2);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(group1a_test_get_variant, NULL, NULL),
    cmocka_unit_test_setup_teardown(group1a0_test_get_ecc, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
