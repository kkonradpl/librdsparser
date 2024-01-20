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
#include "ecc.c"

static void
ecc_test_lookup_unknown_pi(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(RDSPARSER_PI_UNKNOWN, 0xE2), RDSPARSER_COUNTRY_UNKNOWN);
}

static void
ecc_test_lookup_unknown_pi_ecc(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(RDSPARSER_PI_UNKNOWN, 0x00), RDSPARSER_COUNTRY_UNKNOWN);
}

static void
ecc_test_lookup_unknown_ecc(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x3211, 0x00), RDSPARSER_COUNTRY_UNKNOWN);
}

static void
ecc_test_lookup_poland(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x3211, 0xE2), RDSPARSER_COUNTRY_POLAND);
}

static void
ecc_test_lookup_denmark(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x9203, 0xE1), RDSPARSER_COUNTRY_DENMARK);
}

static void
ecc_test_lookup_hungary(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xB205, 0xE0), RDSPARSER_COUNTRY_HUNGARY);
}

static void
ecc_test_lookup_croatia(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xCDEF, 0xE3), RDSPARSER_COUNTRY_CROATIA);
}

static void
ecc_test_lookup_serbia(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xDADD, 0xE2), RDSPARSER_COUNTRY_SERBIA);
}

static void
ecc_test_lookup_tajikistan(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x5555, 0xE3), RDSPARSER_COUNTRY_TAJIKISTAN);
}

static void
ecc_test_lookup_macedonia(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x3456, 0xE4), RDSPARSER_COUNTRY_MACEDONIA);
}

static void
ecc_test_lookup_uzbekistan(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xBEEF, 0xE4), RDSPARSER_COUNTRY_UZBEKISTAN);
}

static void
ecc_test_lookup_bosnia(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xFFFF, 0xE4), RDSPARSER_COUNTRY_BOSNIA);
}

static void
ecc_test_lookup_kosovo(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x7000, 0xE4), RDSPARSER_COUNTRY_KOSOVO);
}

static void
ecc_test_lookup_kyrgyzstan(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x3000, 0xE5), RDSPARSER_COUNTRY_KYRGYZSTAN);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(ecc_test_lookup_unknown_pi, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_unknown_pi_ecc, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_unknown_ecc, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_poland, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_denmark, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_hungary, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_croatia, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_serbia, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_tajikistan, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_macedonia, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_uzbekistan, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_bosnia, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_kosovo, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_kyrgyzstan, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
