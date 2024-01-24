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
pty_test_get_rds_short_unknown(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_short(-1, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_short(64, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_short(32, false), "Unknown");
}

static void
pty_test_get_rbds_short_unknown(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_short(-1, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_short(-53, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_short(32, true), "Unknown");
}

static void
pty_test_get_rds_long_unknown(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_long(-1, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_long(127, false), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_long(32, false), "Unknown");
}

static void
pty_test_get_rbds_long_unknown(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_long(-1, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_long(-128, true), "Unknown");
    assert_string_equal(rdsparser_pty_lookup_long(32, true), "Unknown");
}

static void
pty_test_get_rds_short_light(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_short(13, false), "Light M");
}

static void
pty_test_get_rds_long_light(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_long(13, false), "Light Classics M");
}

static void
pty_test_get_rbds_short_nostalgia(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_short(13, true), "Nostalga");
}

static void
pty_test_get_rbds_long_nostalgia(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_long(13, true), "Nostalgia");
}

static void
pty_test_get_rds_short_alarm(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_short(31, false), "Alarm !");
}

static void
pty_test_get_rds_long_alarm(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_long(31, false), "Alarm - Alarm !");
}

static void
pty_test_get_rbds_short_alert(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_short(31, true), "ALERT !");
}

static void
pty_test_get_rbds_long_alert(void **state)
{
    assert_string_equal(rdsparser_pty_lookup_long(31, true), "ALERT! ALERT!");
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(pty_test_get_rds_short_unknown, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_short_unknown, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rds_long_unknown, NULL, NULL),
    cmocka_unit_test_setup_teardown(pty_test_get_rbds_long_unknown, NULL, NULL),
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
