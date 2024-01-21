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
    assert_int_equal(rdsparser_ecc_lookup(0xFFFF, 0xE4), RDSPARSER_COUNTRY_BOSNIA_HERZEGOVINA);
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

static void
ecc_test_lookup_greenland(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xFAFA, 0xA1), RDSPARSER_COUNTRY_GREENLAND);
}

static void
ecc_test_lookup_argentina(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xAAAA, 0xA2), RDSPARSER_COUNTRY_ARGENTINA);
}

static void
ecc_test_lookup_panama(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x9876, 0xA3), RDSPARSER_COUNTRY_PANAMA);
}

static void
ecc_test_lookup_haiti(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xDEEF, 0xA4), RDSPARSER_COUNTRY_HAITI);
}

static void
ecc_test_lookup_mexico(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xBBBC, 0xA5), RDSPARSER_COUNTRY_MEXICO);
}

static void
ecc_test_lookup_st_pierre(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xF519, 0xA6), RDSPARSER_COUNTRY_ST_PIERRE_AND_MIQUELON);
}

static void
ecc_test_lookup_benin(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xEDFA, 0xD0), RDSPARSER_COUNTRY_BENIN);
}

static void
ecc_test_lookup_ghana(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x3513, 0xD1), RDSPARSER_COUNTRY_GHANA);
}

static void
ecc_test_lookup_guinea_bissau(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xA4A2, 0xD2), RDSPARSER_COUNTRY_GUINEA_BISSAU);
}

static void
ecc_test_lookup_rwanda(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x5355, 0xD3), RDSPARSER_COUNTRY_RWANDA);
}

static void
ecc_test_lookup_south_sudan(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xA000, 0xD4), RDSPARSER_COUNTRY_SOUTH_SUDAN);
}

static void
ecc_test_lookup_afghanistan(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xA000, 0xF0), RDSPARSER_COUNTRY_AFGHANISTAN);
}

static void
ecc_test_lookup_south_korea(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xECCA, 0xF1), RDSPARSER_COUNTRY_SOUTH_KOREA);
}

static void
ecc_test_lookup_macao(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x6660, 0xF2), RDSPARSER_COUNTRY_MACAO);
}

static void
ecc_test_lookup_yemen(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xB000, 0xF3), RDSPARSER_COUNTRY_YEMEN);
}

static void
ecc_test_lookup_china(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x958D, 0xF4), RDSPARSER_COUNTRY_CHINA);
}

static void
ecc_test_lookup_multi1(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xDDDD, 0xA2), RDSPARSER_COUNTRY_BRAZIL_OR_NETHERLANDS_ANTILLES);
}

static void
ecc_test_lookup_multi2(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0xCCCC, 0xA2), RDSPARSER_COUNTRY_BRAZIL_OR_BERMUDA);
}

static void
ecc_test_lookup_multi3(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x3133, 0xA2), RDSPARSER_COUNTRY_BRAZIL_OR_EQUATOR);
}

static void
ecc_test_lookup_multi4(void **state)
{
    assert_int_equal(rdsparser_ecc_lookup(0x1000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0x2000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0x3000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0x4000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0x5000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0x6000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0x7000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0x8000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0x9000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0xA000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0xB000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0xD000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
    assert_int_equal(rdsparser_ecc_lookup(0xE000, 0xA0), RDSPARSER_COUNTRY_USA_OR_VI_OR_PR);
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
    cmocka_unit_test_setup_teardown(ecc_test_lookup_kyrgyzstan, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_greenland, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_argentina, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_panama, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_haiti, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_mexico, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_st_pierre, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_benin, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_ghana, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_guinea_bissau, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_rwanda, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_south_sudan, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_afghanistan, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_south_korea, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_macao, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_yemen, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_macao, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_china, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_multi1, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_multi2, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_multi3, NULL, NULL),
    cmocka_unit_test_setup_teardown(ecc_test_lookup_multi4, NULL, NULL)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
