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

#include <stdint.h>
#include "librds_private.h"
#include "af.h"
#include "group.h"
#include "group0.h"
#include "group1.h"
#include "group2.h"
#include "string.h"

static bool
librds_parse_string_chunk(librds_t              *context,
                          const librds_data_t    data,
                          const librds_error_t   errors,
                          uint8_t                position,
                          librds_block_t         data_block,
                          librds_text_t          text,
                          librds_string_t       *string)
{
    if (errors[LIBRDS_BLOCK_B] <= context->correction[text][LIBRDS_BLOCK_TYPE_INFO] &&
        errors[data_block] <= context->correction[text][LIBRDS_BLOCK_TYPE_DATA])
    {
        char block[2];
        block[0] = data[data_block] >> 8;
        block[1] = (uint8_t)data[data_block];

        return librds_string_update(string,
                                    block,
                                    errors[LIBRDS_BLOCK_B],
                                    errors[data_block],
                                    position,
                                    context->progressive[text],
                                    true);
    }

    return false;
}

static inline void
librds_parse_group(librds_t             *context,
                   const librds_data_t   data,
                   const librds_error_t  errors)
{
    if (errors[LIBRDS_BLOCK_A] == 0)
    {
        uint16_t pi = librds_group_get_pi(data);
        if (context->pi != pi)
        {
            context->pi = pi;
            if (context->callback_pi)
            {
                context->callback_pi(pi, context->user_data);
            }
        }
    }

    if (errors[LIBRDS_BLOCK_B] == 0)
    {
        uint8_t pty = librds_group_get_pty(data);
        if (context->pty != pty)
        {
            context->pty = pty;
            if (context->callback_pty)
            {
                context->callback_pty(pty, context->user_data);
            }
        }

        bool tp = librds_group_get_tp(data);
        if (context->tp != tp)
        {
            context->tp = tp;
            if (context->callback_tp)
            {
                context->callback_tp(tp, context->user_data);
            }
        }
    }
}

static inline void
librds_parse_group0(librds_t             *context,
                    const librds_data_t   data,
                    const librds_error_t  errors)
{
    if (errors[LIBRDS_BLOCK_B] == 0)
    {
        uint8_t ta = librds_group0_get_ta(data);
        if (context->ta != ta)
        {
            context->ta = ta;
            if (context->callback_ta)
            {
                context->callback_ta(ta, context->user_data);
            }
        }

        bool ms = librds_group0_get_ms(data);
        if (context->ms != ms)
        {
            context->ms = ms;
            if (context->callback_ms)
            {
                context->callback_ms(ms, context->user_data);
            }
        }
    }

    if (errors[LIBRDS_BLOCK_B] == 0 &&
        errors[LIBRDS_BLOCK_C] == 0)
    {
        uint8_t af1 = librds_group0_get_af1(data);
        if (librds_af_add(context->af, af1))
        {
            if (context->callback_af)
            {
                context->callback_af(af1, context->user_data);
            }
        }

        uint8_t af2 = librds_group0_get_af2(data);
        if (librds_af_add(context->af, af2))
        {
            if (context->callback_af)
            {
                context->callback_af(af2, context->user_data);
            }
        }
    }


    bool changed;
    const uint8_t position = 2 * librds_group0_get_ps_pos(data);
    changed = librds_parse_string_chunk(context,
                                        data,
                                        errors,
                                        position,
                                        LIBRDS_BLOCK_D,
                                        LIBRDS_TEXT_PS,
                                        context->ps);

    if (changed &&
        context->callback_ps)
    {
        context->callback_ps(context->ps,
                             context->user_data);
    }
}

static inline void
librds_parse_group1a(librds_t             *context,
                     const librds_data_t   data,
                     const librds_error_t  errors)
{
    if (errors[LIBRDS_BLOCK_B] == 0 &&
        errors[LIBRDS_BLOCK_C] == 0)
    {
        if (librds_group1a_get_variant(data) == 0)
        {
            uint8_t ecc = librds_group1a0_get_ecc(data);
            if (context->ecc != ecc)
            {
                context->ecc = ecc;
                if (context->callback_ecc)
                {
                    context->callback_ecc(ecc, context->user_data);
                }
            }
        }
    }
}

static inline void
librds_parse_group2(librds_t             *context,
                    const librds_data_t   data,
                    const librds_error_t  errors,
                    librds_group_flag_t   flag)
{
    librds_rt_flag_t rt_flag = librds_group2_get_rt_flag(data);
    uint8_t rt_pos = librds_group2_get_rt_pos(data);
    bool changed = false;

    if (errors[LIBRDS_BLOCK_B] == 0 &&
        rt_flag != context->last_rt_flag)
    {
        if (context->last_rt_flag != -1 &&
            librds_string_get_available(context->rt[rt_flag]))
        {
            librds_string_clear(context->rt[rt_flag]);
            changed = true;
        }

        context->last_rt_flag = rt_flag;
    }

    if (errors[LIBRDS_BLOCK_B] != 0 &&
        rt_flag != context->last_rt_flag &&
        context->last_rt_flag != -1)
    {
        /* Possible bit-flip of the RDS RT flag in BLOCK_B, ignore the group */
        return;
    }

    if (flag == LIBRDS_GROUP_FLAG_A)
    {
        const uint8_t position = 4 * rt_pos;
        changed |= librds_parse_string_chunk(context,
                                             data,
                                             errors,
                                             position,
                                             LIBRDS_BLOCK_C,
                                             LIBRDS_TEXT_RT,
                                             context->rt[rt_flag]);

        changed |= librds_parse_string_chunk(context,
                                             data,
                                             errors,
                                             position + 2,
                                             LIBRDS_BLOCK_D,
                                             LIBRDS_TEXT_RT,
                                             context->rt[rt_flag]);
    }
    else
    {
        const uint8_t position = 2 * rt_pos;
        changed |= librds_parse_string_chunk(context,
                                             data,
                                             errors,
                                             position,
                                             LIBRDS_BLOCK_D,
                                             LIBRDS_TEXT_RT,
                                             context->rt[rt_flag]);
    }

    if (changed &&
        context->callback_rt)
    {
        context->callback_rt(context->rt[rt_flag],
                             rt_flag,
                             context->user_data);
    }
}

void
librds_parser_process(librds_t             *context,
                      const librds_data_t   data,
                      const librds_error_t  errors)
{
    uint8_t group = librds_group_get_group(data);
    librds_group_flag_t flag = librds_group_get_flag(data);

    librds_parse_group(context, data, errors);

    if (group == 0)
    {
        librds_parse_group0(context, data, errors);
    }
    else if (group == 1)
    {
        if (flag == LIBRDS_GROUP_FLAG_A)
        {
            librds_parse_group1a(context, data, errors);
        }
    }
    else if (group == 2)
    {
        librds_parse_group2(context, data, errors, flag);
    }
}
