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

typedef struct rdsparser_af rdsparser_af_t;
typedef struct rdsparser_ct rdsparser_ct_t;

#ifndef RDSPARSER_DISABLE_UNICODE
#include <wchar.h>
typedef wchar_t rdsparser_string_char_t;
#else
typedef uint8_t rdsparser_string_char_t;
#endif
typedef rdsparser_string_char_t rdsparser_string_t;

typedef enum rdsparser_country
{
    RDSPARSER_COUNTRY_UNKNOWN = -1,
    RDSPARSER_COUNTRY_GERMANY = 0,
    RDSPARSER_COUNTRY_ALGERIA,
    RDSPARSER_COUNTRY_ANDORRA,
    RDSPARSER_COUNTRY_ISRAEL,
    RDSPARSER_COUNTRY_ITALY,
    RDSPARSER_COUNTRY_BELGIUM,
    RDSPARSER_COUNTRY_RUSSIA,
    RDSPARSER_COUNTRY_PALESTINE,
    RDSPARSER_COUNTRY_ALBANIA,
    RDSPARSER_COUNTRY_AUSTRIA,
    RDSPARSER_COUNTRY_HUNGARY,
    RDSPARSER_COUNTRY_MALTA,
    RDSPARSER_COUNTRY_EGYPT,
    RDSPARSER_COUNTRY_GREECE,
    RDSPARSER_COUNTRY_CYPRUS,
    RDSPARSER_COUNTRY_SAN_MARINO,
    RDSPARSER_COUNTRY_SWITZERLAND,
    RDSPARSER_COUNTRY_JORDAN,
    RDSPARSER_COUNTRY_FINLAND,
    RDSPARSER_COUNTRY_LUXEMBURG,
    RDSPARSER_COUNTRY_BULGARIA,
    RDSPARSER_COUNTRY_DENMARK,
    RDSPARSER_COUNTRY_GIBRALTAR,
    RDSPARSER_COUNTRY_IRAQ,
    RDSPARSER_COUNTRY_UNITED_KINGDOM,
    RDSPARSER_COUNTRY_LIBYA,
    RDSPARSER_COUNTRY_ROMANIA,
    RDSPARSER_COUNTRY_FRANCE,
    RDSPARSER_COUNTRY_MOROCCO,
    RDSPARSER_COUNTRY_CZECHIA,
    RDSPARSER_COUNTRY_POLAND,
    RDSPARSER_COUNTRY_VATICAN,
    RDSPARSER_COUNTRY_SLOVAKIA,
    RDSPARSER_COUNTRY_SYRIA,
    RDSPARSER_COUNTRY_TUNISIA,
    RDSPARSER_COUNTRY_LICHTENSTEIN,
    RDSPARSER_COUNTRY_ICELAND,
    RDSPARSER_COUNTRY_MONACO,
    RDSPARSER_COUNTRY_LITHUANIA,
    RDSPARSER_COUNTRY_SERBIA,
    RDSPARSER_COUNTRY_SPAIN,
    RDSPARSER_COUNTRY_NORWAY,
    RDSPARSER_COUNTRY_MONTENEGRO,
    RDSPARSER_COUNTRY_IRELAND,
    RDSPARSER_COUNTRY_TURKEY,
    RDSPARSER_COUNTRY_TAJIKISTAN,
    RDSPARSER_COUNTRY_NETHERLANDS,
    RDSPARSER_COUNTRY_LATVIA,
    RDSPARSER_COUNTRY_LEBANON,
    RDSPARSER_COUNTRY_AZERBAIJAN,
    RDSPARSER_COUNTRY_CROATIA,
    RDSPARSER_COUNTRY_KAZAKHSTAN,
    RDSPARSER_COUNTRY_SWEDEN,
    RDSPARSER_COUNTRY_BELARUS,
    RDSPARSER_COUNTRY_MOLDOVA,
    RDSPARSER_COUNTRY_ESTONIA,
    RDSPARSER_COUNTRY_MACEDONIA,
    RDSPARSER_COUNTRY_UKRAINE,
    RDSPARSER_COUNTRY_KOSOVO,
    RDSPARSER_COUNTRY_PORTUGAL,
    RDSPARSER_COUNTRY_SLOVENIA,
    RDSPARSER_COUNTRY_ARMENIA,
    RDSPARSER_COUNTRY_UZBEKISTAN,
    RDSPARSER_COUNTRY_GEORGIA,
    RDSPARSER_COUNTRY_TURKMENISTAN,
    RDSPARSER_COUNTRY_BOSNIA,
    RDSPARSER_COUNTRY_KYRGYZSTAN,
    RDSPARSER_COUNTRY_COUNT
} rdsparser_country_t;

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

const char* rdsparser_pty_lookup_short(rdsparser_pty_t pty, bool rbds);
const char* rdsparser_pty_lookup_long(rdsparser_pty_t pty, bool rbds);
const char* rdsparser_country_lookup_iso(rdsparser_country_t country);
const char* rdsparser_country_lookup_name(rdsparser_country_t country);

#ifdef __cplusplus
}
#endif

#endif
