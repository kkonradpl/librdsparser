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
#include "pty.c"

static void
pty_test_get_rds_short_invalid(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(-1, false, false), "Invalid");
    assert_string_equal(rdsparser_pty_lookup(64, false, false), "Invalid");
    assert_string_equal(rdsparser_pty_lookup(32, false, false), "Invalid");
}

static void
pty_test_get_rbds_short_invalid(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(-1, true, false), "Invalid");
    assert_string_equal(rdsparser_pty_lookup(-53, true, false), "Invalid");
    assert_string_equal(rdsparser_pty_lookup(32, true, false), "Invalid");
}

static void
pty_test_get_rds_long_invalid(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(-1, false, true), "Invalid");
    assert_string_equal(rdsparser_pty_lookup(127, false, true), "Invalid");
    assert_string_equal(rdsparser_pty_lookup(32, false, true), "Invalid");
}

static void
pty_test_get_rbds_long_invalid(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(-1, true, true), "Invalid");
    assert_string_equal(rdsparser_pty_lookup(-128, true, true), "Invalid");
    assert_string_equal(rdsparser_pty_lookup(32, true, true), "Invalid");
}

static void
pty_test_get_rds_short_light(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(13, false, true), "Light M");
}

static void
pty_test_get_rds_long_light(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(13, false, false), "Light Classics M");
}

static void
pty_test_get_rbds_short_nostalgia(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(13, true, true), "Nostalga");
}

static void
pty_test_get_rbds_long_nostalgia(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(13, true, false), "Nostalgia");
}

static void
pty_test_get_rds_short_alarm(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(31, false, true), "Alarm !");
}

static void
pty_test_get_rds_long_alarm(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(31, false, false), "Alarm - Alarm !");
}

static void
pty_test_get_rbds_short_alert(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(31, true, true), "ALERT !");
}

static void
pty_test_get_rbds_long_alert(void **state)
{
    assert_string_equal(rdsparser_pty_lookup(31, true, false), "ALERT! ALERT!");
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(pty_test_get_rds_short_invalid, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_short_invalid, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rds_long_invalid, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_long_invalid, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rds_short_light, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rds_long_light, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_short_nostalgia, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_long_nostalgia, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rds_short_alarm, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rds_long_alarm, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_short_alert, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_long_alert, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
