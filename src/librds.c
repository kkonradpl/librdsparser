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

#include <string.h>
#include <stdbool.h>
#include "librds_private.h"
#include "af.h"
#include "parser.h"
#include "utils.h"
#include "string.h"

#ifndef LIBRDS_DISABLE_HEAP
librds_t*
librds_new(void)
{
    librds_t *context = malloc(sizeof(librds_t));
    if (context)
    {
        librds_init(context);
    }

    return context;
}

void
librds_free(librds_t *context)
{
    if (context)
    {
        free(context);
    }
}
#endif

void
librds_init(librds_t *context)
{
    memset(context, 0, sizeof(librds_t));
    librds_string_init(context->ps, LIBRDS_PS_LENGTH);
    librds_string_init(context->rt[0], LIBRDS_RT_LENGTH);
    librds_string_init(context->rt[1], LIBRDS_RT_LENGTH);
    librds_clear(context);
}

void
librds_clear(librds_t *context)
{
    context->pi = -1;
    context->tp = -1;
    context->ta = -1;
    context->ms = -1;
    context->pty = -1;
    context->ecc = -1;
    librds_af_clear(context->af);
    librds_string_clear(context->ps);
    librds_string_clear(context->rt[0]);
    librds_string_clear(context->rt[1]);
    context->last_rt_flag = -1;
}

void
librds_parse(librds_t       *context,
             librds_data_t   data,
             librds_error_t  errors)
{
    librds_parser_process(context, data, errors);
}

bool
librds_parse_string(librds_t   *context,
                    const char *input)
{
    librds_data_t data;
    librds_error_t errors;

    if (input &&
        librds_utils_convert(input, data, errors))
    {
        librds_parser_process(context, data, errors);
        return true;
    }

    return false;
}

void
librds_set_text_correction(librds_t             *context,
                           librds_text_t         text,
                           librds_block_type_t   type,
                           librds_block_error_t  error)
{
    const librds_block_error_t max_error = LIBRDS_BLOCK_ERROR_UNCORRECTABLE - 1;
    context->correction[text][type] = (error < max_error ? error : max_error);
}

librds_block_error_t
librds_get_text_correction(const librds_t      *context,
                           librds_text_t        text,
                           librds_block_type_t  type)
{
    return context->correction[text][type];
}

void
librds_set_text_progressive(librds_t        *context,
                            librds_text_t    text,
                            bool             state)
{
    context->progressive[text] = state;
}

bool
librds_get_text_progressive(const librds_t  *context,
                            librds_text_t    text)
{
    return context->progressive[text];
}

int32_t
librds_get_pi(const librds_t *context)
{
    return context->pi;
}

int8_t
librds_get_tp(const librds_t *context)
{
    return context->tp;
}

int8_t
librds_get_ta(const librds_t *context)
{
    return context->ta;
}

int8_t
librds_get_ms(const librds_t *context)
{
    return context->ms;
}

int8_t
librds_get_pty(const librds_t *context)
{
    return context->pty;
}

int16_t
librds_get_ecc(const librds_t *context)
{
    return context->ecc;
}

const librds_af_t*
librds_get_af(const librds_t *context)
{
    return (const librds_af_t*)&context->af;
}

const librds_string_t*
librds_get_ps(const librds_t *context)
{
    return context->ps;
}

const librds_string_t*
librds_get_rt(const librds_t   *context,
              librds_rt_flag_t  flag)
{
    return context->rt[!!flag];
}

void
librds_set_user_data(librds_t *context,
                     void     *user_data)
{
    context->user_data = user_data;
}

void
librds_register_pi(librds_t  *context,
                   void     (*callback_pi)(uint16_t, void*))
{
    context->callback_pi = callback_pi;
}

void
librds_register_pty(librds_t  *context,
                    void     (*callback_pty)(uint8_t, void*))
{
    context->callback_pty = callback_pty;
}

void
librds_register_tp(librds_t  *context,
                   void     (*callback_tp)(bool, void*))
{
    context->callback_tp = callback_tp;
}

void
librds_register_ta(librds_t  *context,
                   void     (*callback_ta)(bool, void*))
{
    context->callback_ta = callback_ta;
}

void
librds_register_ms(librds_t  *context,
                   void     (*callback_ms)(bool, void*))
{
    context->callback_ms = callback_ms;
}

void
librds_register_ecc(librds_t  *context,
                    void     (*callback_ecc)(uint8_t, void*))
{
    context->callback_ecc = callback_ecc;
}

void
librds_register_af(librds_t  *context,
                   void     (*callback_af)(uint8_t, void*))
{
    context->callback_af = callback_af;
}

void
librds_register_ps(librds_t  *context,
                   void     (*callback_ps)(const librds_string_t*, void*))
{
    context->callback_ps = callback_ps;
}

void
librds_register_rt(librds_t  *context,
                   void     (*callback_rt)(const librds_string_t*, librds_rt_flag_t, void*))
{
    context->callback_rt = callback_rt;
}

