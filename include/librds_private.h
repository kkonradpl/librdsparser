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

#define LIBRDS_STRING_SIZE(len) (1 + (len) + 1 + \
                                 (len) / sizeof(librds_string_char_t))

typedef enum librds_group_flag
{
    LIBRDS_GROUP_FLAG_A = 0,
    LIBRDS_GROUP_FLAG_B = 1
} librds_group_flag_t;

struct librds
{
    /* Data buffers */
    int32_t pi;
    int8_t tp;
    int8_t ta;
    int8_t ms;
    int8_t pty;
    int16_t ecc;
    librds_af_t af;
    librds_string_t ps[LIBRDS_STRING_SIZE(LIBRDS_PS_LENGTH)];
    librds_string_t rt[LIBRDS_RT_FLAG_COUNT][LIBRDS_STRING_SIZE(LIBRDS_RT_LENGTH)];

    /* Settings */
    bool progressive[LIBRDS_TEXT_COUNT];
    librds_block_error_t correction[LIBRDS_TEXT_COUNT][LIBRDS_BLOCK_TYPE_COUNT];

    /* Callbacks */
    void *user_data;
    void (*callback_pi)(uint16_t, void*);
    void (*callback_pty)(uint8_t, void*);
    void (*callback_tp)(bool, void*);
    void (*callback_ta)(bool, void*);
    void (*callback_ms)(bool, void*);
    void (*callback_ecc)(uint8_t, void*);
    void (*callback_af)(uint8_t, void*);
    void (*callback_ps)(const librds_string_t*, void*);
    void (*callback_rt)(const librds_string_t*, librds_rt_flag_t, void*);

    /* Other data */
    int8_t last_rt_flag;
};

#endif