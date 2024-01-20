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
#include "country.c"

static void
country_test_lookup_unknown(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_UNKNOWN), "??");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_UNKNOWN), "Unknown");
}

static void
country_test_lookup_invalid(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_COUNT), "??");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_COUNT), "Unknown");
}

static void
country_test_lookup_poland(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_POLAND), "PL");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_POLAND), "Poland");
}

static void
country_test_lookup_germany(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_GERMANY), "DE");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_GERMANY), "Germany");
}

static void
country_test_lookup_czechia(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_CZECHIA), "CZ");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_CZECHIA), "Czechia");
}

static void
country_test_lookup_france(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_FRANCE), "FR");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_FRANCE), "France");
}

static void
country_test_lookup_serbia(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_SERBIA), "RS");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_SERBIA), "Serbia");
}

static void
country_test_lookup_tajikistan(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_TAJIKISTAN), "TJ");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_TAJIKISTAN), "Tajikistan");
}

static void
country_test_lookup_macedonia(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_MACEDONIA), "MK");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_MACEDONIA), "Macedonia");
}

static void
country_test_lookup_uzbekistan(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_UZBEKISTAN), "UZ");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_UZBEKISTAN), "Uzbekistan");
}

static void
country_test_lookup_bosnia(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_BOSNIA), "BA");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_BOSNIA), "Bosnia");
}

static void
country_test_lookup_kosovo(void **state)
{
    assert_string_equal(rdsparser_country_lookup_iso(RDSPARSER_COUNTRY_KOSOVO), "XK");
    assert_string_equal(rdsparser_country_lookup_name(RDSPARSER_COUNTRY_KOSOVO), "Kosovo");
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(country_test_lookup_unknown, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_invalid, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_poland, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_germany, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_czechia, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_france, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_serbia, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_tajikistan, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_macedonia, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_uzbekistan, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_bosnia, NULL, NULL),
    cmocka_unit_test_setup_teardown(country_test_lookup_kosovo, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
