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

#ifndef LIBRDS_PRIVATE_H
#define LIBRDS_PRIVATE_H
#include <stdlib.h>
#include <stdbool.h>
#include "librds.h"

typedef enum librds_block
{
    LIBRDS_BLOCK_A = 0,
    LIBRDS_BLOCK_B = 1,
    LIBRDS_BLOCK_C = 2,
    LIBRDS_BLOCK_D = 3,
    LIBRDS_BLOCK_COUNT
} librds_block_t;

typedef enum librds_group_flag
{
    LIBRDS_GROUP_FLAG_A = 0,
    LIBRDS_GROUP_FLAG_B = 1
} librds_group_flag_t;

typedef uint16_t librds_data_t[LIBRDS_BLOCK_COUNT];
typedef uint8_t librds_error_t[LIBRDS_BLOCK_COUNT];

struct librds
{
    int32_t pi;
    int8_t tp;
    int8_t ta;
    int8_t ms;
    int8_t pty;
    librds_af_t af;
    int16_t ecc;
    char ps[LIBRDS_PS_LENGTH+1];
    librds_string_error_t ps_errors[LIBRDS_PS_LENGTH];
    char rt[LIBRDS_RT_FLAG_COUNT][LIBRDS_RT_LENGTH+1];
    librds_string_error_t rt_errors[LIBRDS_RT_FLAG_COUNT][LIBRDS_RT_LENGTH];
    int8_t last_rt_flag;

    bool progressive[LIBRDS_STRING_COUNT];
    librds_block_error_t correction[LIBRDS_STRING_COUNT][LIBRDS_BLOCK_TYPE_COUNT];

    void *user_data;
    void (*callback_pi)(uint16_t, void*);
    void (*callback_pty)(uint8_t, void*);
    void (*callback_tp)(bool, void*);
    void (*callback_ta)(bool, void*);
    void (*callback_ms)(bool, void*);
    void (*callback_af)(uint8_t, void*);
    void (*callback_ecc)(uint8_t, void*);
    void (*callback_ps)(const char*, const librds_string_error_t[LIBRDS_PS_LENGTH], void*);
    void (*callback_rt)(const char*, const librds_string_error_t[LIBRDS_RT_LENGTH], librds_rt_flag_t, void*);
};

#endif