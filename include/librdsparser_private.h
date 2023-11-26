/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
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

#ifndef RDSPARSER_PRIVATE_H
#define RDSPARSER_PRIVATE_H
#include <stdlib.h>
#include <stdbool.h>
#include <librdsparser.h>

#define RDSPARSER_STRING_SIZE(len) (1 + (len) + 1 + \
                              (len) / sizeof(rdsparser_string_char_t))

typedef enum rdsparser_group_flag
{
    RDSPARSER_GROUP_FLAG_A = 0,
    RDSPARSER_GROUP_FLAG_B = 1
} rdsparser_group_flag_t;

typedef struct rdsparser_af
{
    uint8_t buffer[RDSPARSER_AF_BUFFER_SIZE];
} rdsparser_af_t;

struct librdsparser
{
    /* Data buffers */
    rdsparser_pi_t pi;
    rdsparser_pty_t pty;
    rdsparser_tp_t tp;
    rdsparser_ta_t ta;
    rdsparser_ms_t ms;
    rdsparser_ecc_t ecc;
    rdsparser_af_t af;
    rdsparser_string_t ps[RDSPARSER_STRING_SIZE(RDSPARSER_PS_LENGTH)];
    rdsparser_string_t rt[RDSPARSER_RT_FLAG_COUNT][RDSPARSER_STRING_SIZE(RDSPARSER_RT_LENGTH)];

    /* Settings */
    bool progressive[RDSPARSER_TEXT_COUNT];
    rdsparser_block_error_t correction[RDSPARSER_TEXT_COUNT][RDSPARSER_BLOCK_TYPE_COUNT];

    /* Callbacks */
    void *user_data;
    void (*callback_pi)(rdsparser_t*, void*);
    void (*callback_pty)(rdsparser_t*, void*);
    void (*callback_tp)(rdsparser_t*, void*);
    void (*callback_ta)(rdsparser_t*, void*);
    void (*callback_ms)(rdsparser_t*, void*);
    void (*callback_ecc)(rdsparser_t*, void*);
    void (*callback_af)(rdsparser_t*, uint32_t, void*);
    void (*callback_ps)(rdsparser_t*, void*);
    void (*callback_rt)(rdsparser_t*, rdsparser_rt_flag_t, void*);

    /* Other data */
    int8_t last_rt_flag;
};

#endif