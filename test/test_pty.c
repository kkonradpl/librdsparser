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
pty_test_get_rds_unknown(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_name(-1, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_name(-99, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_name(32, false), "Unknown");

    assert_string_equal(rdsparser_pty_lookup_short(-1, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_short(64, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_short(32, false), "Unknown");

    assert_string_equal(rdsparser_pty_lookup_long(-1, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_long(127, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_long(32, false), "Unknown");
}

static void
pty_test_get_rbds_unknown(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_name(-1, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_name(-67, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_name(32, true), "Unknown");

    assert_string_equal(rdsparser_pty_lookup_short(-1, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_short(-53, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_short(32, true), "Unknown");

    assert_string_equal(rdsparser_pty_lookup_long(-1, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_long(-128, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_long(32, true), "Unknown");
}

static void
pty_test_get_rds_light(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_name(13, false), "Light classical");
    assert_string_equal(rdsparser_pty_lookup_short(13, false), "Light M");
    assert_string_equal(rdsparser_pty_lookup_long(13, false), "Light classics m");
}

static void
pty_test_get_rbds_nostalgia(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_name(13, true), "Nostalgia");
    assert_string_equal(rdsparser_pty_lookup_short(13, true), "Nostalga");
    assert_string_equal(rdsparser_pty_lookup_long(13, true), "Nostalgia");
}

static void
pty_test_get_rds_alarm(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_name(31, false), "Alarm");
    assert_string_equal(rdsparser_pty_lookup_short(31, false), "Alarm !");
    assert_string_equal(rdsparser_pty_lookup_long(31, false), "Alarm - Alarm !");
}

static void
pty_test_get_rbds_alert(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_name(31, true), "Emergency");
    assert_string_equal(rdsparser_pty_lookup_short(31, true), "ALERT !");
    assert_string_equal(rdsparser_pty_lookup_long(31, true), "ALERT! ALERT!");
}

static void
pty_test_get_rds_none(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_name(0, false), "No programme type or undefined");
    assert_string_equal(rdsparser_pty_lookup_short(0, false), "");
    assert_string_equal(rdsparser_pty_lookup_long(0, false), "");
}

static void
pty_test_get_rbds_none(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_name(0, true), "No program type or undefined");
    assert_string_equal(rdsparser_pty_lookup_short(0, true), "None");
    assert_string_equal(rdsparser_pty_lookup_long(0, true), "None");
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(pty_test_get_rds_unknown, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_unknown, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rds_light, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_nostalgia, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rds_alarm, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_alert, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rds_none, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_none, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
