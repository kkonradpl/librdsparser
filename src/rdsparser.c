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

#include <string.h>
#include <stdbool.h>
#include <librdsparser_private.h>
#include "af.h"
#include "parser.h"
#include "utils.h"
#include "string.h"

#ifndef RDSPARSER_DISABLE_HEAP
rdsparser_t*
rdsparser_new(void)
{
    rdsparser_t *rds = malloc(sizeof(rdsparser_t));
    if (rds)
    {
        rdsparser_init(rds);
    }

    return rds;
}

void
rdsparser_free(rdsparser_t *rds)
{
    if (rds)
    {
        free(rds);
    }
}
#endif

void
rdsparser_init(rdsparser_t *rds)
{
    memset(rds, 0, sizeof(rdsparser_t));
    rdsparser_string_init(rds->ps, RDSPARSER_PS_LENGTH);
    rdsparser_string_init(rds->rt[0], RDSPARSER_RT_LENGTH);
    rdsparser_string_init(rds->rt[1], RDSPARSER_RT_LENGTH);
    rdsparser_string_init(rds->ptyn, RDSPARSER_PTYN_LENGTH);
    rdsparser_clear(rds);
}

void
rdsparser_clear(rdsparser_t *rds)
{
    rds->pi = RDSPARSER_PI_UNKNOWN;
    rds->pty = RDSPARSER_PTY_UNKNOWN;
    rds->tp = RDSPARSER_TP_UNKNOWN;
    rds->ta = RDSPARSER_TA_UNKNOWN;
    rds->ms = RDSPARSER_MS_UNKNOWN;
    rds->ecc = RDSPARSER_ECC_UNKNOWN;
    rdsparser_af_clear(&rds->af);
    rdsparser_string_clear(rds->ps);
    rdsparser_string_clear(rds->rt[0]);
    rdsparser_string_clear(rds->rt[1]);
    rdsparser_string_clear(rds->ptyn);
    rds->last_rt_flag = -1;
}

void
rdsparser_parse(rdsparser_t       *rds,
                rdsparser_data_t   data,
                rdsparser_error_t  errors)
{
    rdsparser_parser_process(rds, data, errors);
}

bool
rdsparser_parse_string(rdsparser_t *rds,
                       const char  *input)
{
    rdsparser_data_t data;
    rdsparser_error_t errors;

    if (input &&
        rdsparser_utils_convert(input, data, errors))
    {
        rdsparser_parser_process(rds, data, errors);
        return true;
    }

    return false;
}

void
rdsparser_set_text_correction(rdsparser_t             *rds,
                              rdsparser_text_t         text,
                              rdsparser_block_type_t   type,
                              rdsparser_block_error_t  error)
{
    const rdsparser_block_error_t max_error = RDSPARSER_BLOCK_ERROR_UNCORRECTABLE - 1;
    rds->correction[text][type] = (error < max_error ? error : max_error);
}

rdsparser_block_error_t
rdsparser_get_text_correction(const rdsparser_t      *rds,
                              rdsparser_text_t        text,
                              rdsparser_block_type_t  type)
{
    return rds->correction[text][type];
}

void
rdsparser_set_text_progressive(rdsparser_t      *rds,
                               rdsparser_text_t  text,
                               bool              state)
{
    rds->progressive[text] = state;
}

bool
rdsparser_get_text_progressive(const rdsparser_t *rds,
                               rdsparser_text_t   text)
{
    return rds->progressive[text];
}

void
rdsparser_set_pi(rdsparser_t    *rds,
                 rdsparser_pi_t  pi)
{
    if (rds->pi != pi)
    {
        rds->pi = pi;
        if (rds->callback_pi)
        {
            rds->callback_pi(rds, rds->user_data);
        }
    }
}

rdsparser_pi_t
rdsparser_get_pi(const rdsparser_t *rds)
{
    return rds->pi;
}

void
rdsparser_set_pty(rdsparser_t     *rds,
                  rdsparser_pty_t  pty)
{
    if (rds->pty != pty)
    {
        rds->pty = pty;
        if (rds->callback_pty)
        {
            rds->callback_pty(rds, rds->user_data);
        }
    }
}

rdsparser_pty_t
rdsparser_get_pty(const rdsparser_t *rds)
{
    return rds->pty;
}

void
rdsparser_set_tp(rdsparser_t     *rds,
                  rdsparser_tp_t  tp)
{
    if (rds->tp != tp)
    {
        rds->tp = tp;
        if (rds->callback_tp)
        {
            rds->callback_tp(rds, rds->user_data);
        }
    }
}

rdsparser_tp_t
rdsparser_get_tp(const rdsparser_t *rds)
{
    return rds->tp;
}

void
rdsparser_set_ta(rdsparser_t     *rds,
                  rdsparser_ta_t  ta)
{
    if (rds->ta != ta)
    {
        rds->ta = ta;
        if (rds->callback_ta)
        {
            rds->callback_ta(rds, rds->user_data);
        }
    }
}

rdsparser_ta_t
rdsparser_get_ta(const rdsparser_t *rds)
{
    return rds->ta;
}

void
rdsparser_set_ms(rdsparser_t    *rds,
                 rdsparser_ms_t  ms)
{
    if (rds->ms != ms)
    {
        rds->ms = ms;
        if (rds->callback_ms)
        {
            rds->callback_ms(rds, rds->user_data);
        }
    }
}

rdsparser_ms_t
rdsparser_get_ms(const rdsparser_t *rds)
{
    return rds->ms;
}

void
rdsparser_set_ecc(rdsparser_t     *rds,
                  rdsparser_ecc_t  ecc)
{
    if (rds->ecc != ecc)
    {
        rds->ecc = ecc;
        if (rds->callback_ecc)
        {
            rds->callback_ecc(rds, rds->user_data);
        }
    }
}

rdsparser_ecc_t
rdsparser_get_ecc(const rdsparser_t *rds)
{
    return rds->ecc;
}

void
rdsparser_set_af(rdsparser_t *rds,
                 uint8_t      new_af)
{
    if (rdsparser_af_add(&rds->af, new_af) &&
        rds->callback_af)
    {
        const uint32_t frequency = 87500 + (uint32_t)new_af * 100;
        rds->callback_af(rds, frequency, rds->user_data);
    }
}

const rdsparser_af_t*
rdsparser_get_af(const rdsparser_t *rds)
{
    return &rds->af;
}

const rdsparser_string_t*
rdsparser_get_ps(const rdsparser_t *rds)
{
    return rds->ps;
}

const rdsparser_string_t*
rdsparser_get_rt(const rdsparser_t   *rds,
                 rdsparser_rt_flag_t  flag)
{
    return rds->rt[!!flag];
}

const rdsparser_string_t*
rdsparser_get_ptyn(const rdsparser_t *rds)
{
    return rds->ptyn;
}

void
rdsparser_set_user_data(rdsparser_t *rds,
                        void        *user_data)
{
    rds->user_data = user_data;
}

void
rdsparser_register_pi(rdsparser_t  *rds,
                      void        (*callback_pi)(rdsparser_t*, void*))
{
    rds->callback_pi = callback_pi;
}

void
rdsparser_register_pty(rdsparser_t  *rds,
                       void        (*callback_pty)(rdsparser_t*, void*))
{
    rds->callback_pty = callback_pty;
}

void
rdsparser_register_tp(rdsparser_t  *rds,
                      void        (*callback_tp)(rdsparser_t*, void*))
{
    rds->callback_tp = callback_tp;
}

void
rdsparser_register_ta(rdsparser_t  *rds,
                      void        (*callback_ta)(rdsparser_t*, void*))
{
    rds->callback_ta = callback_ta;
}

void
rdsparser_register_ms(rdsparser_t  *rds,
                      void        (*callback_ms)(rdsparser_t*, void*))
{
    rds->callback_ms = callback_ms;
}

void
rdsparser_register_ecc(rdsparser_t  *rds,
                       void        (*callback_ecc)(rdsparser_t*, void*))
{
    rds->callback_ecc = callback_ecc;
}

void
rdsparser_register_af(rdsparser_t  *rds,
                      void        (*callback_af)(rdsparser_t*, uint32_t, void*))
{
    rds->callback_af = callback_af;
}

void
rdsparser_register_ps(rdsparser_t  *rds,
                      void        (*callback_ps)(rdsparser_t*, void*))
{
    rds->callback_ps = callback_ps;
}

void
rdsparser_register_rt(rdsparser_t  *rds,
                      void        (*callback_rt)(rdsparser_t*, rdsparser_rt_flag_t, void*))
{
    rds->callback_rt = callback_rt;
}

void
rdsparser_register_ptyn(rdsparser_t  *rds,
                        void        (*callback_ptyn)(rdsparser_t*, void*))
{
    rds->callback_ptyn = callback_ptyn;
}

