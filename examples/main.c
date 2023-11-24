/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librds – Radio Data System parser library
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

#include <stdio.h>
#include <locale.h>
#include "librds.h"

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
    "34DB2542616D793A00"
};

static void
callback_pi(uint16_t  pi,
            void     *user_data)
{
    printf("PI: %04X\n", pi);
}

static void
callback_pty(uint8_t  pty,
             void    *user_data)
{
    printf("PTY: %d\n", pty);
}

static void
callback_tp(bool  tp,
            void *user_data)
{
    printf("TP: %d\n", tp);
}

static void
callback_ta(bool  tp,
            void *user_data)
{
    printf("TA: %d\n", tp);
}

static void
callback_ms(bool  tp,
            void *user_data)
{
    printf("MS: %d\n", tp);
}

static void
callback_ecc(uint8_t  ecc,
             void    *user_data)
{
    printf("ECC: %d\n", ecc);
}

static void
callback_af(uint8_t  af,
            void    *user_data)
{
    printf("AF: %d\n", af*100 + 87500);
}

static void
callback_ps(const librds_string_t *ps,
            void                  *user_data)
{
    const librds_string_char_t *ps_content = librds_string_get_content(ps);
#ifdef LIBRDS_DISABLE_UNICODE
    printf("PS: %s\n", ps_content);
#else
    printf("PS: %ls\n", ps_content);
#endif
}

static void
callback_rt(const librds_string_t *rt,
            librds_rt_flag_t       flag,
            void                  *user_data)
{
    const librds_string_char_t *rt_content = librds_string_get_content(rt);
#ifdef LIBRDS_DISABLE_UNICODE
    printf("RT%d: %s\n", flag, rt_content);
#else
    printf("RT%d: %ls\n", flag, rt_content);
#endif
}

int
main(int   argc,
     char* argv[])
{
#ifdef LIBRDS_DISABLE_HEAP
    librds_t buffer;
    librds_init(&buffer);
    librds_t *rds = &buffer;
#else
    librds_t *rds = librds_new();
    if (rds == NULL)
    {
        return -1;
    }
#endif

#ifndef LIBRDS_DISABLE_UNICODE
    setlocale(LC_ALL, "");
    printf("Version: Unicode\n");
#else
    printf("Version: ASCII\n");
#endif

    librds_register_pi(rds, callback_pi);
    librds_register_pty(rds, callback_pty);
    librds_register_tp(rds, callback_tp);
    librds_register_ta(rds, callback_ta);
    librds_register_ms(rds, callback_ms);
    librds_register_ecc(rds, callback_ecc);
    librds_register_af(rds, callback_af);
    librds_register_ps(rds, callback_ps);
    librds_register_rt(rds, callback_rt);

    for (size_t i = 0; i < sizeof(rds_data) / sizeof(char*); i++)
    {
        librds_parse_string(rds, rds_data[i]);
    }

#ifndef LIBRDS_DISABLE_HEAP
    librds_free(rds);
#endif
    return 0;
}
