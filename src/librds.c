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
    librds_t *rds = malloc(sizeof(librds_t));
    if (rds)
    {
        librds_init(rds);
    }

    return rds;
}

void
librds_free(librds_t *rds)
{
    if (rds)
    {
        free(rds);
    }
}
#endif

void
librds_init(librds_t *rds)
{
    memset(rds, 0, sizeof(librds_t));
    librds_string_init(rds->ps, LIBRDS_PS_LENGTH);
    librds_string_init(rds->rt[0], LIBRDS_RT_LENGTH);
    librds_string_init(rds->rt[1], LIBRDS_RT_LENGTH);
    librds_clear(rds);
}

void
librds_clear(librds_t *rds)
{
    rds->pi = LIBRDS_PI_UNKNOWN;
    rds->pty = LIBRDS_PTY_UNKNOWN;
    rds->tp = LIBRDS_TP_UNKNOWN;
    rds->ta = LIBRDS_TA_UNKNOWN;
    rds->ms = LIBRDS_MS_UNKNOWN;
    rds->ecc = LIBRDS_ECC_UNKNOWN;
    librds_af_clear(rds->af);
    librds_string_clear(rds->ps);
    librds_string_clear(rds->rt[0]);
    librds_string_clear(rds->rt[1]);
    rds->last_rt_flag = -1;
}

void
librds_parse(librds_t       *rds,
             librds_data_t   data,
             librds_error_t  errors)
{
    librds_parser_process(rds, data, errors);
}

bool
librds_parse_string(librds_t   *rds,
                    const char *input)
{
    librds_data_t data;
    librds_error_t errors;

    if (input &&
        librds_utils_convert(input, data, errors))
    {
        librds_parser_process(rds, data, errors);
        return true;
    }

    return false;
}

void
librds_set_text_correction(librds_t             *rds,
                           librds_text_t         text,
                           librds_block_type_t   type,
                           librds_block_error_t  error)
{
    const librds_block_error_t max_error = LIBRDS_BLOCK_ERROR_UNCORRECTABLE - 1;
    rds->correction[text][type] = (error < max_error ? error : max_error);
}

librds_block_error_t
librds_get_text_correction(const librds_t      *rds,
                           librds_text_t        text,
                           librds_block_type_t  type)
{
    return rds->correction[text][type];
}

void
librds_set_text_progressive(librds_t      *rds,
                            librds_text_t  text,
                            bool           state)
{
    rds->progressive[text] = state;
}

bool
librds_get_text_progressive(const librds_t *rds,
                            librds_text_t   text)
{
    return rds->progressive[text];
}

librds_pi_t
librds_get_pi(const librds_t *rds)
{
    return rds->pi;
}

librds_pty_t
librds_get_pty(const librds_t *rds)
{
    return rds->pty;
}

librds_tp_t
librds_get_tp(const librds_t *rds)
{
    return rds->tp;
}

librds_ta_t
librds_get_ta(const librds_t *rds)
{
    return rds->ta;
}

librds_ms_t
librds_get_ms(const librds_t *rds)
{
    return rds->ms;
}

librds_ecc_t
librds_get_ecc(const librds_t *rds)
{
    return rds->ecc;
}

const librds_af_t*
librds_get_af(const librds_t *rds)
{
    return (const librds_af_t*)&rds->af;
}

const librds_string_t*
librds_get_ps(const librds_t *rds)
{
    return rds->ps;
}

const librds_string_t*
librds_get_rt(const librds_t   *rds,
              librds_rt_flag_t  flag)
{
    return rds->rt[!!flag];
}

void
librds_set_user_data(librds_t *rds,
                     void     *user_data)
{
    rds->user_data = user_data;
}

void
librds_register_pi(librds_t  *rds,
                   void     (*callback_pi)(librds_t*, void*))
{
    rds->callback_pi = callback_pi;
}

void
librds_register_pty(librds_t  *rds,
                    void     (*callback_pty)(librds_t*, void*))
{
    rds->callback_pty = callback_pty;
}

void
librds_register_tp(librds_t  *rds,
                   void     (*callback_tp)(librds_t*, void*))
{
    rds->callback_tp = callback_tp;
}

void
librds_register_ta(librds_t  *rds,
                   void     (*callback_ta)(librds_t*, void*))
{
    rds->callback_ta = callback_ta;
}

void
librds_register_ms(librds_t  *rds,
                   void     (*callback_ms)(librds_t*, void*))
{
    rds->callback_ms = callback_ms;
}

void
librds_register_ecc(librds_t  *rds,
                    void     (*callback_ecc)(librds_t*, void*))
{
    rds->callback_ecc = callback_ecc;
}

void
librds_register_af(librds_t  *rds,
                   void     (*callback_af)(librds_t*, uint8_t, void*))
{
    rds->callback_af = callback_af;
}

void
librds_register_ps(librds_t  *rds,
                   void     (*callback_ps)(librds_t*, void*))
{
    rds->callback_ps = callback_ps;
}

void
librds_register_rt(librds_t  *rds,
                   void     (*callback_rt)(librds_t*, librds_rt_flag_t, void*))
{
    rds->callback_rt = callback_rt;
}

