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

#ifndef LIBRDS_H
#define LIBRDS_H
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBRDS_AF_BUFFER_SIZE 26
#define LIBRDS_PS_LENGTH 8
#define LIBRDS_RT_LENGTH 64

typedef uint8_t librds_block_t;
enum librds_block
{
    LIBRDS_BLOCK_A = 0,
    LIBRDS_BLOCK_B = 1,
    LIBRDS_BLOCK_C = 2,
    LIBRDS_BLOCK_D = 3,
    LIBRDS_BLOCK_COUNT
};

typedef uint8_t librds_block_error_t;
enum librds_block_error
{
    LIBRDS_BLOCK_ERROR_NONE = 0,
    LIBRDS_BLOCK_ERROR_SMALL = 1,
    LIBRDS_BLOCK_ERROR_LARGE = 2,
    LIBRDS_BLOCK_ERROR_UNCORRECTABLE = 3,
};

typedef uint8_t librds_block_type_t;
enum librds_block_type
{
    LIBRDS_BLOCK_TYPE_INFO = 0,
    LIBRDS_BLOCK_TYPE_DATA = 1,
    LIBRDS_BLOCK_TYPE_COUNT
};

typedef uint8_t librds_text_t;
enum librds_text
{
    LIBRDS_TEXT_PS = 0,
    LIBRDS_TEXT_RT = 1,
    LIBRDS_TEXT_COUNT
};

typedef uint8_t librds_string_error_t;
enum librds_string_error
{
    LIBRDS_STRING_ERROR_NONE = 0,
    LIBRDS_STRING_ERROR_SMALLEST = 1,
    LIBRDS_STRING_ERROR_X_SMALL = 2,
    LIBRDS_STRING_ERROR_SMALL = 3,
    LIBRDS_STRING_ERROR_MEDIUM_SMALL = 4,
    LIBRDS_STRING_ERROR_MEDIUM = 5,
    LIBRDS_STRING_ERROR_MEDIUM_LARGE = 6,
    LIBRDS_STRING_ERROR_LARGE = 7,
    LIBRDS_STRING_ERROR_X_LARGE = 8,
    LIBRDS_STRING_ERROR_LARGEST = 9,
    LIBRDS_STRING_ERROR_UNCORRECTABLE
};

typedef uint8_t librds_rt_flag_t;
enum librds_rt_flag
{
    LIBRDS_RT_FLAG_A = 0,
    LIBRDS_RT_FLAG_B = 1,
    LIBRDS_RT_FLAG_COUNT
};

typedef struct librds librds_t;
typedef uint16_t librds_data_t[LIBRDS_BLOCK_COUNT];
typedef uint8_t librds_error_t[LIBRDS_BLOCK_COUNT];

typedef int32_t librds_pi_t;
#define LIBRDS_PI_UNKNOWN -1

typedef int8_t librds_pty_t;
#define LIBRDS_PTY_UNKNOWN -1

typedef int8_t librds_tp_t;
enum librds_tp
{
    LIBRDS_TP_UNKNOWN = -1,
    LIBRDS_TP_OFF = 0,
    LIBRDS_TP_ON = 1
};

typedef int8_t librds_ta_t;
enum librds_ta
{
    LIBRDS_TA_UNKNOWN = -1,
    LIBRDS_TA_OFF = 0,
    LIBRDS_TA_ON = 1
};

typedef int8_t librds_ms_t;
enum librds_ms
{
    LIBRDS_MS_UNKNOWN = -1,
    LIBRDS_MS_MUSIC = 0,
    LIBRDS_MS_SPEECH = 1
};

typedef int16_t librds_ecc_t;
#define LIBRDS_ECC_UNKNOWN -1

typedef uint8_t librds_af_t[LIBRDS_AF_BUFFER_SIZE];

#ifndef LIBRDS_DISABLE_UNICODE
#include <wchar.h>
typedef wchar_t librds_string_char_t;
#else
typedef uint8_t librds_string_char_t;
#endif
typedef librds_string_char_t librds_string_t;

#ifndef LIBRDS_DISABLE_HEAP
librds_t* librds_new(void);
void librds_free(librds_t *rds);
#else
#include "librds_private.h"
#endif

void librds_init(librds_t *rds);
void librds_clear(librds_t *rds);

void librds_parse(librds_t *rds, librds_data_t data, librds_error_t errors);
bool librds_parse_string(librds_t *rds, const char *input);

void librds_set_text_correction(librds_t *rds, librds_text_t text, librds_block_type_t type, librds_block_error_t error);
librds_block_error_t librds_get_text_correction(const librds_t *rds, librds_text_t text, librds_block_type_t type);

void librds_set_text_progressive(librds_t *rds, librds_text_t string, bool state);
bool librds_get_text_progressive(const librds_t *rds, librds_text_t string);

librds_pi_t librds_get_pi(const librds_t *rds);
librds_pty_t librds_get_pty(const librds_t *rds);
librds_tp_t librds_get_tp(const librds_t *rds);
librds_ta_t librds_get_ta(const librds_t *rds);
librds_ms_t librds_get_ms(const librds_t *rds);
librds_ecc_t librds_get_ecc(const librds_t *rds);
const librds_af_t* librds_get_af(const librds_t *rds);
const librds_string_t* librds_get_ps(const librds_t *rds);
const librds_string_t* librds_get_rt(const librds_t *rds, librds_rt_flag_t flag);

void librds_set_user_data(librds_t *rds, void *user_data);

void librds_register_pi(librds_t *rds, void (*callback_pi)(librds_t*, void*));
void librds_register_pty(librds_t *rds, void (*callback_pty)(librds_t*, void*));
void librds_register_tp(librds_t *rds, void (*callback_tp)(librds_t*, void*));
void librds_register_ta(librds_t *rds, void (*callback_ta)(librds_t*, void*));
void librds_register_ms(librds_t *rds, void (*callback_ms)(librds_t*, void*));
void librds_register_ecc(librds_t *rds, void (*callback_ecc)(librds_t*, void*));
void librds_register_af(librds_t *rds, void (*callback_af)(librds_t*, uint8_t, void*));
void librds_register_ps(librds_t *rds, void (*callback_ps)(librds_t*, void*));
void librds_register_rt(librds_t *rds, void (*callback_rt)(librds_t*, librds_rt_flag_t, void*));

uint8_t librds_string_get_length(const librds_string_t *string);
bool librds_string_get_available(const librds_string_t *string);
const librds_string_char_t* librds_string_get_content(const librds_string_t *string);
const librds_string_error_t* librds_string_get_errors(const librds_string_t *string);

#ifdef __cplusplus
}
#endif

#endif