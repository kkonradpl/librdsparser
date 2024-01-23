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
    "A20120017420696E02",
    "A201E0153475A20313",
    "A20130100064CD4615",
    "A2018001D9F579E800",
    "A2010019032B4F4500",
    "A201200220D7313A00",
    "A201E015067BA20318",
    "A201001A2B5F203114",
    "A2018001D9F579E801",
    "A201001F1F2B202001",
    "A201E0157907A20300",
    "A2010018E703202000",
    "A20120032044696500",
    "A2018001D9F579E834",
    "A2010019032B4F4500",
    "A201E015640DA20300",
    "A201001A0320203100",
    "A20180014E91920000",
    "A201001F035F202000",
    "A201E015026AA20310",
    "A2010018833120200C",
    "A20120042066656D04",
    "A2014001D758F84233",
    "A20180014E91920010",
    "A2010019015F4F4511",
    "A2012005696E697321",
    "A201E0153079A20314",
    "A2013010580CCD463D",
    "A20180014E91920004",
    "A201001AE51E203100",
    "A20120067469736300",
    "A201E0152078A20300",
    "A201001F1E5F202024",
    "A2018002DE418CBE01",
    "A2010018021E202000",
    "A201E0151F8CA2031C",
    "A2010019E5024F4502",
    "A20120076865204200",
    "A2018002DE418CBE00",
    "A201001A025F203102",
    "A201E0150F6AA20308",
    "A201001F021E202002",
    "A2018002DE418CBE14",
    "A2010018E32220200C",
    "A201E0155F13A20329",
    "A2010019225F4F4500",
    "A201200861726F6300",
    "A20180025418EE9000",
    "A201001AE921203100",
    "A20120096B64696310",
    "A201E0153A05A20300",
    "A20130100064CD4600",
    "A20180025418EE9000",
    "A201001F215F202000",
    "A201200A6874657200",
    "A201E01D0000A20301",
    "A20100181C21202000",
    "A20180025418EE9000",
    "A201001921314F4500",
    "A201E0002020A21300",
    "A201001A2123203102",
    "A201200B696E205300",
    "A20180020CA0000002",
    "A201001FE51C202005",
    "A201E001464DA21300",
    "A20100181C21202002",
    "A20180020CA0000001",
    "A20100191C5F4F4500",
    "A201E0023420A21304",
    "A201001AE931203130",
    "A201200C6962796C00",
    "A20180020CA0000008",
    "A201001F2131202000",
    "A201200D6C61205310",
    "A201E0032020A21310",
    "A20130104000CD4612",
    "A201800843F4A68735",
    "A2010018315F202000",
    "A201200E6368776100",
    "A201E0055F71A21316",
    "A201001925314F4524",
    "A201800843F4A68704",
    "A201001A3180203101",
    "A201E0050192A21300",
    "A201001FE90F202004",
    "A201200F727A202000",
    "A201800843F4A68708",
    "A20100180F5F202011",
    "A201E005348BA21300",
    "A20100190F1E4F451A",
    "A2018003C191BF2C20",
    "A201001A0F23203127",
    "A201E0052DA3A21304",
    "A201001F0F32202001",
    "A20120005269747A08",
    "A2018003C191BF2C06",
    "A2010018E51E202000",
    "A20120017420696E00",
    "A2013834F68DA2133C",
    "A2018003C191BF2C09",
    "A20100191E5F4F4514",
    "A201200220D7313A00",
    "A201E00564A5A21320",
    "A201001A0F1E203100",
    "A2018003415D2C8C04",
    "A201001FF95F202001",
    "A201E00D0001A21300",
    "A20100182D5F202010",
    "A2012003204469650C",
    "A2018003415D2C8C3C",
    "A20100195F644F4500",
    "A20130100064CD4606",
    "A201E0005241A60210",
    "A201064A345F203122",
    "A2018003415D2C8C0C",
    "A201001F1E5F202000",
    "A201E0014449A60214",
    "A2010018225F202000",
    "A20120042066656D00",
    "A20180085065A0FC30",
    "A2010019375F4F4504",
    "A2012005696E697330",
    "A201E62A492EA6022C",
    "A20180085065A0FC00",
    "A201001A305F203110",
    "A2012006176F73630C",
    "A201E0034E20A60201",
    "A201001F3A5F20200C",
    "A20180085065A14F3B",
    "A20100180F5F202020",
    "A201F9B5C754A60235",
    "A2010019025F4F4501",
    "A201200768C320422C",
    "A2018004B87D8DB010",
    "A201001A015F203101",
    "A20130104000CD4603",
    "A201E0051E39A602C0",
    "A201001F215F202000",
    "A201800488658DB000",
    "A2010018F15F202020",
    "A201E0051E3EA60204",
    "A20100192B5F40872F",
    "A201200861726F6300",
    "A201800488658DB0D4",
    "A201001A5F792031C0",
    "A20120096B64696306",
    "A201E0053452A60208",
    "A20180045BA000002F",
    "A201001F065F202000",
    "A201200A6874657237",
    "A201E005A47AA60204",
    "A2010018265F202000",
    "A2018004421000000D",
    "A2010019395F4F4510",
    "A201E0055F28A60200",
    "A201001A035F203130",
    "A201200B696E205304",
    "A2018004A20800001C",
    "A201001F1C5F202010",
    "A201051B0064CD4634",
    "A201E6062D68A6023E",
    "A2010018315F202002",
    "A20180058991C1E110",
    "A2010019E5374F4543",
    "A201E0053C4EA60204",
    "A201001A2D37203100",
    "A201200C6962796C00",
    "A20180058991C1E100",
    "A201001F375F202000",
    "A201200D6C61205300",
    "A201E005223FA60200",
    "A20130104000CD4634",
    "A20180058991C1E100",
    "A2010018E330202030",
    "A201200E6F6E440107",
    "A201E0052661A6022C",
    "A2010019305F4F451B",
    "A20180054E96460000",
    "A201001AE534203100",
    "A201A0005445535400", // PTYN
    "A201A0015445535400", // PTYN
    "A2014001D750018200", // CT
    "A201100000E0000000", // ECC
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
    printf("ECC: 0x%02X\n", rdsparser_get_ecc(rds));
}

static void
callback_country(rdsparser_t *rds,
                 void        *user_data)
{
    rdsparser_country_t country = rdsparser_get_country(rds);
    printf("Country: %s (%s)\n",
           rdsparser_country_lookup_name(country),
           rdsparser_country_lookup_iso(country));
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
    const uint8_t *ps_errors = rdsparser_string_get_errors(ps);
    const uint8_t length = rdsparser_string_get_length(ps);
#ifdef RDSPARSER_DISABLE_UNICODE
    printf("PS: %s", ps_content);
#else
    printf("PS: %ls", ps_content);
#endif
    printf(" (");
    for (uint8_t i = 0; i < length; i++)
    {
        printf("%d", ps_errors[i]);
        if (i != length - 1)
            printf(",");
    }
    printf(")\n");
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

    rdsparser_set_text_correction(rds, RDSPARSER_TEXT_PS, RDSPARSER_BLOCK_TYPE_INFO, RDSPARSER_BLOCK_ERROR_LARGE);
    rdsparser_set_text_correction(rds, RDSPARSER_TEXT_PS, RDSPARSER_BLOCK_TYPE_DATA, RDSPARSER_BLOCK_ERROR_LARGE);
    rdsparser_set_text_correction(rds, RDSPARSER_TEXT_RT, RDSPARSER_BLOCK_TYPE_INFO, RDSPARSER_BLOCK_ERROR_LARGE);
    rdsparser_set_text_correction(rds, RDSPARSER_TEXT_RT, RDSPARSER_BLOCK_TYPE_DATA, RDSPARSER_BLOCK_ERROR_LARGE);

    rdsparser_set_text_progressive(rds, RDSPARSER_TEXT_PS, true);
    rdsparser_set_text_progressive(rds, RDSPARSER_TEXT_RT, true);

    rdsparser_register_pi(rds, callback_pi);
    rdsparser_register_pty(rds, callback_pty);
    rdsparser_register_tp(rds, callback_tp);
    rdsparser_register_ta(rds, callback_ta);
    rdsparser_register_ms(rds, callback_ms);
    rdsparser_register_ecc(rds, callback_ecc);
    rdsparser_register_country(rds, callback_country);
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
