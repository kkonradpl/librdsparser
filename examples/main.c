/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
 *  Copyright (C) 2023-2024  Konrad Kosmatka
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

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <librdsparser.h>

static const char *rds_data[] =
{
    "34DB054A76CD445000",
    "34DB25504A757A2000",
    "34DB054FE4A42A2000",
    "34DB355800004BD700",
    "34DB054CA829202A00",
    "34DBC5400000000000",
    "34DB054976CD4B5200",
    "34DB25517A61206300",
    "34DB054AE4A4445000",
    "34DB25526877696C00",
    "34DB054FA8292A2000",
    "34DB2553653A204200",
    "34DB054C76CD202A00",
    "34DB255475646B6100",
    "34DB0549E4A44B5200",
    "34DB25552053756600",
    "34DB054AA829445000",
    "34DB25566C65726100",
    "34DB054F76CD2A2000",
    "34DB2557202D204300",
    "34DB054CE4A4202A00",
    "34DB25587A61732000",
    "34DB0549A8294B5200",
    "34DB25594F5F4F7700",
    "34DB054A76CD445000",
    "34DB255A69750D0D00",
    "34DB054FE4A42A2000",
    "34DB25405465726100",
    "34DB054CA829202A00",
    "34DB355800004BD700",
    "34DB054976CD4B5200",
    "34DBC558301821AF00",
    "34DB054AE4A4445000",
    "34DB25417A20677200",
    "34DB054FA8292A2000",
    "34DB2542616D793A00",
    "34DB054C76CD202A00",
    "34DB254320476F6C00",
    "34DB0549E4A44B5200",
    "34DB25446563205500",
    "34DB054AA829445000",
    "34DB25456F726B6900",
    "34DB054F76CD2A2000",
    "34DB25466573747200",
    "34DB054CE4A4202A00",
    "34DB254761202D2000",
    "34DB0549A8294B5200",
    "34DB25484E69652000",
    "34DB054A76CD445000",
    "34DB25494761732000",
    "34DB054FE4A42A2000",
    "34DB254A4475636800",
    "34DB054CA829202A00",
    "34DB254B61200D0D00",
    "34DB054976CD4B5200",
    "34DB25405465726100",
    "34DB054AE4A4445000",
    "34DB355800004BD700",
    "34DB054FA8292A2000",
    "34DBC548301821AF00",
    "34DB054C76CD202A00",
    "34DB25417A20677200",
    "34DB0549E4A44B5200",
    "34DB2542616D793A00",
    "34DB054AA829445000",
    "34DB254320476F6C00",
    "34DB054F76CD2A2000",
    "34DB25446563205500",
    "34DB054CE4A4202A00",
    "34DB25456F726B6900",
    "34DB0549A8294B5200",
    "34DB25466573747200",
    "34DB054A76CD445000",
    "34DB254761202D2000",
    "34DB054FE4A42A2000",
    "34DB25484E69652000",
    "34DB054CA829202A00",
    "34DB25494761732000",
    "34DB054976CD4B5200",
    "34DB254A4475636800",
    "34DB054AE4A4445000",
    "34DB254B61200D0D00",
    "34DB054FA8292A2000",
    "34DB25405465726100",
    "34DB054C76CD202A00",
    "34DB355800004BD700",
    "34DB0549E4A44B5200",
    "34DBC548301821AF00",
    "34DB054AA829445000",
    "34DB25417A20677200",
    "34DB054F76CD2A2000",
    "34DB2542616D793A00",
    "34DBA5505241444900",
    "34DBA5514F20372000",
    "34DB4541D750018200"
};

static void
callback_pi(rdsparser_t *rds,
            void        *user_data)
{
    printf("PI: %04X\n", rdsparser_get_pi(rds));
}

static void
callback_pty(rdsparser_t *rds,
             void        *user_data)
{
    rdsparser_pty_t pty = rdsparser_get_pty(rds);
    printf("PTY: %s (%d)\n", rdsparser_pty_lookup_long(pty, false), pty);
}

static void
callback_tp(rdsparser_t *rds,
            void        *user_data)
{
    printf("TP: %d\n", rdsparser_get_tp(rds));
}

static void
callback_ta(rdsparser_t *rds,
            void        *user_data)
{
    printf("TA: %d\n", rdsparser_get_ta(rds));
}

static void
callback_ms(rdsparser_t *rds,
            void        *user_data)
{
    printf("MS: %d\n", rdsparser_get_ms(rds));
}

static void
callback_ecc(rdsparser_t *rds,
             void        *user_data)
{
    printf("ECC: %d\n", rdsparser_get_ecc(rds));
}

static void
callback_af(rdsparser_t *rds,
            uint32_t     new_af,
            void        *user_data)
{
    printf("AF: %d kHz\n", new_af);
}

static void
callback_ps(rdsparser_t *rds,
            void        *user_data)
{
    const rdsparser_string_t *ps = rdsparser_get_ps(rds);
    const rdsparser_string_char_t *ps_content = rdsparser_string_get_content(ps);
#ifdef RDSPARSER_DISABLE_UNICODE
    printf("PS: %s\n", ps_content);
#else
    printf("PS: %ls\n", ps_content);
#endif
}

static void
callback_rt(rdsparser_t         *rds,
            rdsparser_rt_flag_t  flag,
            void                *user_data)
{
    const rdsparser_string_t *rt = rdsparser_get_rt(rds, flag);
    const rdsparser_string_char_t *rt_content = rdsparser_string_get_content(rt);
#ifdef RDSPARSER_DISABLE_UNICODE
    printf("RT%d: %s\n", flag, rt_content);
#else
    printf("RT%d: %ls\n", flag, rt_content);
#endif
}

static void
callback_ptyn(rdsparser_t *rds,
              void        *user_data)
{
    const rdsparser_string_t *ptyn = rdsparser_get_ptyn(rds);
    const rdsparser_string_char_t *ptyn_content = rdsparser_string_get_content(ptyn);
#ifdef RDSPARSER_DISABLE_UNICODE
    printf("PTYN: %s\n", ptyn_content);
#else
    printf("PTYN: %ls\n", ptyn_content);
#endif
}

static void
callback_ct(rdsparser_t          *rds,
            const rdsparser_ct_t *ct,
            void                 *user_data)
{
    int16_t offset = rdsparser_ct_get_offset(ct);

    printf("CT: %04d-%02d-%02d %02d:%02d (%c%02d:%02d)\n",
           rdsparser_ct_get_year(ct),
           rdsparser_ct_get_month(ct),
           rdsparser_ct_get_day(ct),
           rdsparser_ct_get_hour(ct),
           rdsparser_ct_get_minute(ct),
           (offset >= 0) ? '+' : '-',
           abs(offset / 60),
           abs(offset % 60));
}

int
main(int   argc,
     char* argv[])
{
#ifdef RDSPARSER_DISABLE_HEAP
    rdsparser_t buffer;
    rdsparser_init(&buffer);
    rdsparser_t *rds = &buffer;
#else
    rdsparser_t *rds = rdsparser_new();
    if (rds == NULL)
    {
        return -1;
    }
#endif

#ifndef RDSPARSER_DISABLE_UNICODE
    setlocale(LC_ALL, "");
    printf("Version: Unicode\n");
#else
    printf("Version: ASCII\n");
#endif

    rdsparser_register_pi(rds, callback_pi);
    rdsparser_register_pty(rds, callback_pty);
    rdsparser_register_tp(rds, callback_tp);
    rdsparser_register_ta(rds, callback_ta);
    rdsparser_register_ms(rds, callback_ms);
    rdsparser_register_ecc(rds, callback_ecc);
    rdsparser_register_af(rds, callback_af);
    rdsparser_register_ps(rds, callback_ps);
    rdsparser_register_rt(rds, callback_rt);
    rdsparser_register_ptyn(rds, callback_ptyn);
    rdsparser_register_ct(rds, callback_ct);

    for (size_t i = 0; i < sizeof(rds_data) / sizeof(char*); i++)
    {
        rdsparser_parse_string(rds, rds_data[i]);
    }

#ifndef RDSPARSER_DISABLE_HEAP
    rdsparser_free(rds);
#endif
    return 0;
}
