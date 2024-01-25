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

#ifndef RDSPARSER_H
#define RDSPARSER_H
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RDSPARSER_AF_BUFFER_SIZE 26
#define RDSPARSER_PS_LENGTH 8
#define RDSPARSER_RT_LENGTH 64
#define RDSPARSER_PTYN_LENGTH 8

typedef uint8_t rdsparser_block_t;
enum rdsparser_block
{
    RDSPARSER_BLOCK_A = 0,
    RDSPARSER_BLOCK_B = 1,
    RDSPARSER_BLOCK_C = 2,
    RDSPARSER_BLOCK_D = 3,
    RDSPARSER_BLOCK_COUNT
};

typedef uint8_t rdsparser_block_error_t;
enum rdsparser_block_error
{
    RDSPARSER_BLOCK_ERROR_NONE = 0,
    RDSPARSER_BLOCK_ERROR_SMALL = 1,
    RDSPARSER_BLOCK_ERROR_LARGE = 2,
    RDSPARSER_BLOCK_ERROR_UNCORRECTABLE = 3,
};

typedef uint8_t rdsparser_block_type_t;
enum rdsparser_block_type
{
    RDSPARSER_BLOCK_TYPE_INFO = 0,
    RDSPARSER_BLOCK_TYPE_DATA = 1,
    RDSPARSER_BLOCK_TYPE_COUNT
};

typedef uint8_t rdsparser_text_t;
enum rdsparser_text
{
    RDSPARSER_TEXT_PS = 0,
    RDSPARSER_TEXT_RT = 1,
    RDSPARSER_TEXT_PTYN = 2,
    RDSPARSER_TEXT_COUNT
};

typedef uint8_t rdsparser_string_error_t;
enum rdsparser_string_error
{
    RDSPARSER_STRING_ERROR_NONE = 0,
    RDSPARSER_STRING_ERROR_SMALLEST = 1,
    RDSPARSER_STRING_ERROR_X_SMALL = 2,
    RDSPARSER_STRING_ERROR_SMALL = 3,
    RDSPARSER_STRING_ERROR_MEDIUM_SMALL = 4,
    RDSPARSER_STRING_ERROR_MEDIUM = 5,
    RDSPARSER_STRING_ERROR_MEDIUM_LARGE = 6,
    RDSPARSER_STRING_ERROR_LARGE = 7,
    RDSPARSER_STRING_ERROR_X_LARGE = 8,
    RDSPARSER_STRING_ERROR_LARGEST = 9,
    RDSPARSER_STRING_ERROR_UNCORRECTABLE
};

typedef uint8_t rdsparser_rt_flag_t;
enum rdsparser_rt_flag
{
    RDSPARSER_RT_FLAG_A = 0,
    RDSPARSER_RT_FLAG_B = 1,
    RDSPARSER_RT_FLAG_COUNT
};

typedef struct librdsparser rdsparser_t;
typedef uint16_t rdsparser_data_t[RDSPARSER_BLOCK_COUNT];
typedef uint8_t rdsparser_error_t[RDSPARSER_BLOCK_COUNT];

typedef int32_t rdsparser_pi_t;
#define RDSPARSER_PI_UNKNOWN -1

typedef int8_t rdsparser_pty_t;
#define RDSPARSER_PTY_UNKNOWN -1

typedef int8_t rdsparser_tp_t;
enum rdsparser_tp
{
    RDSPARSER_TP_UNKNOWN = -1,
    RDSPARSER_TP_OFF = 0,
    RDSPARSER_TP_ON = 1
};

typedef int8_t rdsparser_ta_t;
enum rdsparser_ta
{
    RDSPARSER_TA_UNKNOWN = -1,
    RDSPARSER_TA_OFF = 0,
    RDSPARSER_TA_ON = 1
};

typedef int8_t rdsparser_ms_t;
enum rdsparser_ms
{
    RDSPARSER_MS_UNKNOWN = -1,
    RDSPARSER_MS_SPEECH = 0,
    RDSPARSER_MS_MUSIC = 1
};

typedef int16_t rdsparser_ecc_t;
#define RDSPARSER_ECC_UNKNOWN -1

typedef uint8_t rdsparser_country_t;
enum rdsparser_country
{
    RDSPARSER_COUNTRY_UNKNOWN = 0,
    RDSPARSER_COUNTRY_ALBANIA,
    RDSPARSER_COUNTRY_ESTONIA,
    RDSPARSER_COUNTRY_ALGERIA,
    RDSPARSER_COUNTRY_ETHIOPIA,
    RDSPARSER_COUNTRY_ANDORRA,
    RDSPARSER_COUNTRY_ANGOLA,
    RDSPARSER_COUNTRY_FINLAND,
    RDSPARSER_COUNTRY_ARMENIA,
    RDSPARSER_COUNTRY_FRANCE,
    RDSPARSER_COUNTRY_ASCENSION_ISLAND,
    RDSPARSER_COUNTRY_GABON,
    RDSPARSER_COUNTRY_AUSTRIA,
    RDSPARSER_COUNTRY_GAMBIA,
    RDSPARSER_COUNTRY_AZERBAIJAN,
    RDSPARSER_COUNTRY_GEORGIA,
    RDSPARSER_COUNTRY_GERMANY,
    RDSPARSER_COUNTRY_BAHREIN,
    RDSPARSER_COUNTRY_GHANA,
    RDSPARSER_COUNTRY_BELARUS,
    RDSPARSER_COUNTRY_GIBRALTAR,
    RDSPARSER_COUNTRY_BELGIUM,
    RDSPARSER_COUNTRY_GREECE,
    RDSPARSER_COUNTRY_BENIN,
    RDSPARSER_COUNTRY_GUINEA,
    RDSPARSER_COUNTRY_BOSNIA_HERZEGOVINA,
    RDSPARSER_COUNTRY_GUINEA_BISSAU,
    RDSPARSER_COUNTRY_BOTSWANA,
    RDSPARSER_COUNTRY_HUNGARY,
    RDSPARSER_COUNTRY_BULGARIA,
    RDSPARSER_COUNTRY_ICELAND,
    RDSPARSER_COUNTRY_BURKINA_FASO,
    RDSPARSER_COUNTRY_IRAQ,
    RDSPARSER_COUNTRY_BURUNDI,
    RDSPARSER_COUNTRY_IRELAND,
    RDSPARSER_COUNTRY_CABINDA,
    RDSPARSER_COUNTRY_ISRAEL,
    RDSPARSER_COUNTRY_CAMEROON,
    RDSPARSER_COUNTRY_ITALY,
    RDSPARSER_COUNTRY_JORDAN,
    RDSPARSER_COUNTRY_CAPE_VERDE,
    RDSPARSER_COUNTRY_KAZAKHSTAN,
    RDSPARSER_COUNTRY_CENTRAL_AFRICAN_REPUBLIC,
    RDSPARSER_COUNTRY_KENYA,
    RDSPARSER_COUNTRY_CHAD,
    RDSPARSER_COUNTRY_KOSOVO,
    RDSPARSER_COUNTRY_COMOROS,
    RDSPARSER_COUNTRY_KUWAIT,
    RDSPARSER_COUNTRY_DR_CONGO,
    RDSPARSER_COUNTRY_KYRGYZSTAN,
    RDSPARSER_COUNTRY_REPUBLIC_OF_CONGO,
    RDSPARSER_COUNTRY_LATVIA,
    RDSPARSER_COUNTRY_COTE_DIVOIRE,
    RDSPARSER_COUNTRY_LEBANON,
    RDSPARSER_COUNTRY_CROATIA,
    RDSPARSER_COUNTRY_LESOTHO,
    RDSPARSER_COUNTRY_CYPRUS,
    RDSPARSER_COUNTRY_LIBERIA,
    RDSPARSER_COUNTRY_CZECHIA,
    RDSPARSER_COUNTRY_LIBYA,
    RDSPARSER_COUNTRY_DENMARK,
    RDSPARSER_COUNTRY_LIECHTENSTEIN,
    RDSPARSER_COUNTRY_DJIBOUTIA,
    RDSPARSER_COUNTRY_LITHUANIA,
    RDSPARSER_COUNTRY_EGYPT,
    RDSPARSER_COUNTRY_LUXEMBOURG,
    RDSPARSER_COUNTRY_EQUATORIAL_GUINEA,
    RDSPARSER_COUNTRY_MACEDONIA,
    RDSPARSER_COUNTRY_ERITREA,
    RDSPARSER_COUNTRY_MADAGASCAR,
    RDSPARSER_COUNTRY_SEYCHELLES,
    RDSPARSER_COUNTRY_MALAWI,
    RDSPARSER_COUNTRY_SIERRA_LEONE,
    RDSPARSER_COUNTRY_MALI,
    RDSPARSER_COUNTRY_SLOVAKIA,
    RDSPARSER_COUNTRY_MALTA,
    RDSPARSER_COUNTRY_SLOVENIA,
    RDSPARSER_COUNTRY_MAURITANIA,
    RDSPARSER_COUNTRY_SOMALIA,
    RDSPARSER_COUNTRY_MAURITIUS,
    RDSPARSER_COUNTRY_SOUTH_AFRICA,
    RDSPARSER_COUNTRY_MOLDOVA,
    RDSPARSER_COUNTRY_SOUTH_SUDAN,
    RDSPARSER_COUNTRY_MONACO,
    RDSPARSER_COUNTRY_SPAIN,
    RDSPARSER_COUNTRY_MONGOLIA,
    RDSPARSER_COUNTRY_SUDAN,
    RDSPARSER_COUNTRY_MONTENEGRO,
    RDSPARSER_COUNTRY_SWAZILAND,
    RDSPARSER_COUNTRY_MOROCCO,
    RDSPARSER_COUNTRY_SWEDEN,
    RDSPARSER_COUNTRY_MOZAMBIQUE,
    RDSPARSER_COUNTRY_SWITZERLAND,
    RDSPARSER_COUNTRY_NAMIBIA,
    RDSPARSER_COUNTRY_SYRIA,
    RDSPARSER_COUNTRY_NETHERLANDS,
    RDSPARSER_COUNTRY_TAJIKISTAN,
    RDSPARSER_COUNTRY_NIGER,
    RDSPARSER_COUNTRY_TANZANIA,
    RDSPARSER_COUNTRY_NIGERIA,
    RDSPARSER_COUNTRY_TOGO,
    RDSPARSER_COUNTRY_NORWAY,
    RDSPARSER_COUNTRY_TUNISIA,
    RDSPARSER_COUNTRY_OMAN,
    RDSPARSER_COUNTRY_TURKEY,
    RDSPARSER_COUNTRY_PALESTINE,
    RDSPARSER_COUNTRY_TURKMENISTAN,
    RDSPARSER_COUNTRY_POLAND,
    RDSPARSER_COUNTRY_UGANDA,
    RDSPARSER_COUNTRY_PORTUGAL,
    RDSPARSER_COUNTRY_UKRAINE,
    RDSPARSER_COUNTRY_QATAR,
    RDSPARSER_COUNTRY_UNITED_ARAB_EMIRATES,
    RDSPARSER_COUNTRY_ROMANIA,
    RDSPARSER_COUNTRY_UNITED_KINGDOM,
    RDSPARSER_COUNTRY_RUSSIA,
    RDSPARSER_COUNTRY_UZBEKISTAN,
    RDSPARSER_COUNTRY_RWANDA,
    RDSPARSER_COUNTRY_VATICAN,
    RDSPARSER_COUNTRY_SAN_MARINO,
    RDSPARSER_COUNTRY_WESTERN_SAHARA,
    RDSPARSER_COUNTRY_SAO_TOME_AND_PRINCIPE,
    RDSPARSER_COUNTRY_YEMEN,
    RDSPARSER_COUNTRY_SAUDI_ARABIA,
    RDSPARSER_COUNTRY_ZAMBIA,
    RDSPARSER_COUNTRY_SENEGAL,
    RDSPARSER_COUNTRY_ZIMBABWE,
    RDSPARSER_COUNTRY_SERBIA,
    RDSPARSER_COUNTRY_ANGUILLA,
    RDSPARSER_COUNTRY_GUYANA,
    RDSPARSER_COUNTRY_ANTIGUA_AND_BARBUDA,
    RDSPARSER_COUNTRY_HAITI,
    RDSPARSER_COUNTRY_ARGENTINA,
    RDSPARSER_COUNTRY_HONDURAS,
    RDSPARSER_COUNTRY_ARUBA,
    RDSPARSER_COUNTRY_JAMAICA,
    RDSPARSER_COUNTRY_BAHAMAS,
    RDSPARSER_COUNTRY_MARTINIQUE,
    RDSPARSER_COUNTRY_BARBADOS,
    RDSPARSER_COUNTRY_MEXICO,
    RDSPARSER_COUNTRY_BELIZE,
    RDSPARSER_COUNTRY_MONTSERRAT,
    RDSPARSER_COUNTRY_BRAZIL_OR_BERMUDA,
    RDSPARSER_COUNTRY_BRAZIL_OR_NETHERLANDS_ANTILLES,
    RDSPARSER_COUNTRY_BOLIVIA,
    RDSPARSER_COUNTRY_NICARAGUA,
    RDSPARSER_COUNTRY_BRAZIL,
    RDSPARSER_COUNTRY_PANAMA,
    RDSPARSER_COUNTRY_CANADA,
    RDSPARSER_COUNTRY_PARAGUAY,
    RDSPARSER_COUNTRY_CAYMAN_ISLANDS,
    RDSPARSER_COUNTRY_PERU,
    RDSPARSER_COUNTRY_CHILE,
    RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
    RDSPARSER_COUNTRY_COLOMBIA,
    RDSPARSER_COUNTRY_ST_KITTS,
    RDSPARSER_COUNTRY_COSTA_RICA,
    RDSPARSER_COUNTRY_ST_LUCIA,
    RDSPARSER_COUNTRY_CUBA,
    RDSPARSER_COUNTRY_ST_PIERRE_AND_MIQUELON,
    RDSPARSER_COUNTRY_DOMINICA,
    RDSPARSER_COUNTRY_ST_VINCENT,
    RDSPARSER_COUNTRY_DOMINICAN_REPUBLIC,
    RDSPARSER_COUNTRY_SURINAME,
    RDSPARSER_COUNTRY_EL_SALVADOR,
    RDSPARSER_COUNTRY_TRINIDAD_AND_TOBAGO,
    RDSPARSER_COUNTRY_TURKS_AND_CAICOS_ISLANDS,
    RDSPARSER_COUNTRY_FALKLAND_ISLANDS,
    RDSPARSER_COUNTRY_GREENLAND,
    RDSPARSER_COUNTRY_URUGUAY,
    RDSPARSER_COUNTRY_GRENADA,
    RDSPARSER_COUNTRY_VENEZUELA,
    RDSPARSER_COUNTRY_GUADELOUPE,
    RDSPARSER_COUNTRY_VIRGIN_ISLANDS,
    RDSPARSER_COUNTRY_GUATEMALA,
    RDSPARSER_COUNTRY_AFGHANISTAN,
    RDSPARSER_COUNTRY_SOUTH_KOREA,
    RDSPARSER_COUNTRY_LAOS,
    RDSPARSER_COUNTRY_AUSTRALIA_CAPITAL_TERRITORY,
    RDSPARSER_COUNTRY_MACAO,
    RDSPARSER_COUNTRY_AUSTRALIA_NEW_SOUTH_WALES,
    RDSPARSER_COUNTRY_MALAYSIA,
    RDSPARSER_COUNTRY_AUSTRALIA_VICTORIA,
    RDSPARSER_COUNTRY_MALDIVES,
    RDSPARSER_COUNTRY_AUSTRALIA_QUEENSLAND,
    RDSPARSER_COUNTRY_MARSHALL_ISLANDS,
    RDSPARSER_COUNTRY_AUSTRALIA_SOUTH_AUSTRALIA,
    RDSPARSER_COUNTRY_MICRONESIA,
    RDSPARSER_COUNTRY_AUSTRALIA_WESTERN_AUSTRALIA,
    RDSPARSER_COUNTRY_MYANMAR,
    RDSPARSER_COUNTRY_AUSTRALIA_TASMANIA,
    RDSPARSER_COUNTRY_NAURU,
    RDSPARSER_COUNTRY_AUSTRALIA_NORTHERN_TERRITORY,
    RDSPARSER_COUNTRY_NEPAL,
    RDSPARSER_COUNTRY_BANGLADESH,
    RDSPARSER_COUNTRY_NEW_ZEALAND,
    RDSPARSER_COUNTRY_BHUTAN,
    RDSPARSER_COUNTRY_PAKISTAN,
    RDSPARSER_COUNTRY_BRUNEI_DARUSSALAM,
    RDSPARSER_COUNTRY_PAPUA_NEW_GUINEA,
    RDSPARSER_COUNTRY_CAMBODIA,
    RDSPARSER_COUNTRY_PHILIPPINES,
    RDSPARSER_COUNTRY_CHINA,
    RDSPARSER_COUNTRY_SAMOA,
    RDSPARSER_COUNTRY_SINGAPORE,
    RDSPARSER_COUNTRY_SOLOMON_ISLANDS,
    RDSPARSER_COUNTRY_FIJI,
    RDSPARSER_COUNTRY_SRI_LANKA,
    RDSPARSER_COUNTRY_HONG_KONG,
    RDSPARSER_COUNTRY_TAIWAN,
    RDSPARSER_COUNTRY_INDIA,
    RDSPARSER_COUNTRY_THAILAND,
    RDSPARSER_COUNTRY_INDONESIA,
    RDSPARSER_COUNTRY_TONGA,
    RDSPARSER_COUNTRY_IRAN,
    RDSPARSER_COUNTRY_VANUATU,
    RDSPARSER_COUNTRY_JAPAN,
    RDSPARSER_COUNTRY_VIETNAM,
    RDSPARSER_COUNTRY_KIRIBATI,
    RDSPARSER_COUNTRY_NORTH_KOREA,
    RDSPARSER_COUNTRY_BRAZIL_OR_EQUATOR,
    RDSPARSER_COUNTRY_COUNT
};

typedef struct rdsparser_af rdsparser_af_t;
typedef struct rdsparser_ct rdsparser_ct_t;

#ifndef RDSPARSER_DISABLE_UNICODE
#include <wchar.h>
typedef wchar_t rdsparser_string_char_t;
#else
typedef uint8_t rdsparser_string_char_t;
#endif
typedef rdsparser_string_char_t rdsparser_string_t;

#ifndef RDSPARSER_DISABLE_HEAP
rdsparser_t* rdsparser_new(void);
void rdsparser_free(rdsparser_t *rds);
#else
#include <librdsparser_private.h>
#endif

void rdsparser_init(rdsparser_t *rds);
void rdsparser_clear(rdsparser_t *rds);

void rdsparser_parse(rdsparser_t *rds, rdsparser_data_t data, rdsparser_error_t errors);
bool rdsparser_parse_string(rdsparser_t *rds, const char *input);

void rdsparser_set_extended_check(rdsparser_t *rds, bool value);
bool rdsparser_get_extended_check(const rdsparser_t *rds);

void rdsparser_set_text_correction(rdsparser_t *rds, rdsparser_text_t text, rdsparser_block_type_t type, rdsparser_block_error_t error);
rdsparser_block_error_t rdsparser_get_text_correction(const rdsparser_t *rds, rdsparser_text_t text, rdsparser_block_type_t type);

void rdsparser_set_text_progressive(rdsparser_t *rds, rdsparser_text_t string, bool state);
bool rdsparser_get_text_progressive(const rdsparser_t *rds, rdsparser_text_t string);

rdsparser_pi_t rdsparser_get_pi(const rdsparser_t *rds);
rdsparser_pty_t rdsparser_get_pty(const rdsparser_t *rds);
rdsparser_tp_t rdsparser_get_tp(const rdsparser_t *rds);
rdsparser_ta_t rdsparser_get_ta(const rdsparser_t *rds);
rdsparser_ms_t rdsparser_get_ms(const rdsparser_t *rds);
rdsparser_ecc_t rdsparser_get_ecc(const rdsparser_t *rds);
rdsparser_country_t rdsparser_get_country(const rdsparser_t *rds);
const rdsparser_af_t* rdsparser_get_af(const rdsparser_t *rds);
const rdsparser_string_t* rdsparser_get_ps(const rdsparser_t *rds);
const rdsparser_string_t* rdsparser_get_rt(const rdsparser_t *rds, rdsparser_rt_flag_t flag);
const rdsparser_string_t* rdsparser_get_ptyn(const rdsparser_t *rds);

void rdsparser_set_user_data(rdsparser_t *rds, void *user_data);

void rdsparser_register_pi(rdsparser_t *rds, void (*callback_pi)(rdsparser_t*, void*));
void rdsparser_register_pty(rdsparser_t *rds, void (*callback_pty)(rdsparser_t*, void*));
void rdsparser_register_tp(rdsparser_t *rds, void (*callback_tp)(rdsparser_t*, void*));
void rdsparser_register_ta(rdsparser_t *rds, void (*callback_ta)(rdsparser_t*, void*));
void rdsparser_register_ms(rdsparser_t *rds, void (*callback_ms)(rdsparser_t*, void*));
void rdsparser_register_ecc(rdsparser_t *rds, void (*callback_ecc)(rdsparser_t*, void*));
void rdsparser_register_country(rdsparser_t *rds, void (*callback_country)(rdsparser_t*, void*));
void rdsparser_register_af(rdsparser_t *rds, void (*callback_af)(rdsparser_t*, uint32_t, void*));
void rdsparser_register_ps(rdsparser_t *rds, void (*callback_ps)(rdsparser_t*, void*));
void rdsparser_register_rt(rdsparser_t *rds, void (*callback_rt)(rdsparser_t*, rdsparser_rt_flag_t, void*));
void rdsparser_register_ptyn(rdsparser_t *rds, void (*callback_ptyn)(rdsparser_t*, void*));
void rdsparser_register_ct(rdsparser_t *rds, void (*callback_ct)(rdsparser_t*, const rdsparser_ct_t*, void*));

uint8_t rdsparser_string_get_length(const rdsparser_string_t *string);
bool rdsparser_string_get_available(const rdsparser_string_t *string);
const rdsparser_string_char_t* rdsparser_string_get_content(const rdsparser_string_t *string);
const rdsparser_string_error_t* rdsparser_string_get_errors(const rdsparser_string_t *string);

uint16_t rdsparser_ct_get_year(const rdsparser_ct_t *ct);
uint8_t rdsparser_ct_get_month(const rdsparser_ct_t *ct);
uint8_t rdsparser_ct_get_day(const rdsparser_ct_t *ct);
uint8_t rdsparser_ct_get_hour(const rdsparser_ct_t *ct);
uint8_t rdsparser_ct_get_minute(const rdsparser_ct_t *ct);
int16_t rdsparser_ct_get_offset(const rdsparser_ct_t *ct);

const char* rdsparser_pty_lookup_name(rdsparser_pty_t pty, bool rbds);
const char* rdsparser_pty_lookup_short(rdsparser_pty_t pty, bool rbds);
const char* rdsparser_pty_lookup_long(rdsparser_pty_t pty, bool rbds);
const char* rdsparser_country_lookup_name(rdsparser_country_t country);
const char* rdsparser_country_lookup_iso(rdsparser_country_t country);

#ifdef __cplusplus
}
#endif

#endif
