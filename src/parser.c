/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
 *  Copyright (C) 2023-2025  Konrad Kosmatka
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
#include <librdsparser_private.h>
#include "af.h"
#include "group.h"
#include "group0.h"
#include "group1.h"
#include "group2.h"
#include "group4.h"
#include "group10.h"
#include "group15.h"
#include "string.h"

#define RDSPARSER_PROGRESSIVE_THRESHOLD_INSTANT 6
#define RDSPARSER_PROGRESSIVE_THRESHOLD_DELAYED 20

#define CLAMP(x, lower, upper) ((x) < (lower) ? (lower) : ((x) > (upper) ? (upper) : (x)))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static inline uint8_t
rdsparser_parser_get_group(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 0xF000) >> 12;
}

static inline rdsparser_group_flag_t
rdsparser_parser_get_flag(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 0x0800) >> 11;
}

void
rdsparser_parser_process(rdsparser_t             *rds,
                         const rdsparser_data_t   data,
                         const rdsparser_error_t  errors)
{
    rdsparser_group_parse(rds, data, errors);

    rdsparser_group_flag_t flag = rdsparser_parser_get_flag(data);
    uint8_t group = rdsparser_parser_get_group(data);
    switch (group)
    {
        case 0:
            rdsparser_group0_parse(rds, data, errors, flag);
            break;

        case 1:
            rdsparser_group1_parse(rds, data, errors, flag);
            break;

        case 2:
            rdsparser_group2_parse(rds, data, errors, flag);
            break;

        case 4:
            rdsparser_group4_parse(rds, data, errors, flag);
            break;

        case 10:
            rdsparser_group10_parse(rds, data, errors, flag);
            break;

        case 15:
            rdsparser_group15_parse(rds, data, errors, flag);
            break;
    }
}

static inline rdsparser_string_error_t
rdsparser_parser_string_error(rdsparser_block_error_t info_error,
                              rdsparser_block_error_t data_error)
{
    const uint8_t value = 2 * info_error + 3 * data_error;
    return (rdsparser_string_error_t)(value ? value - 1 : 0);
}

bool
rdsparser_parser_update_string(rdsparser_t             *context,
                               rdsparser_string_t      *string,
                               rdsparser_text_t         text,
                               rdsparser_block_t        data_block,
                               const rdsparser_data_t   data,
                               const rdsparser_error_t  errors,
                               uint8_t                  position)
{
    rdsparser_string_error_t curr_error = rdsparser_parser_string_error(errors[RDSPARSER_BLOCK_B], errors[data_block]);
    context->adaptive_ps_errors += (curr_error == RDSPARSER_STRING_ERROR_NONE) ? -1 : 1;
    context->adaptive_ps_errors = CLAMP(context->adaptive_ps_errors, 0, 4);

    if (errors[RDSPARSER_BLOCK_B] > context->correction[text][RDSPARSER_BLOCK_TYPE_INFO] ||
        errors[data_block] > context->correction[text][RDSPARSER_BLOCK_TYPE_DATA])
    {
        return false;
    }

    rdsparser_string_error_t prev_error = MAX(rdsparser_string_get_errors(string)[position],
                                              rdsparser_string_get_errors(string)[position + 1]);

    bool changed = false;
    const uint8_t chunk_length = 2;
    for (uint8_t i = 0; i < chunk_length; i++)
    {
        const char input = (i ? (uint8_t)data[data_block] : data[data_block] >> 8);
        const bool allow_eol = (text == RDSPARSER_TEXT_RT);

        changed |= rdsparser_string_update(string,
                                           position + i,
                                           input,
                                           curr_error,
                                           context->progressive[text],
                                           allow_eol,
                                           false);
    }

    if (text == RDSPARSER_TEXT_PS &&
        context->adaptive_ps &&
        curr_error == RDSPARSER_STRING_ERROR_NONE &&
        prev_error == RDSPARSER_STRING_ERROR_NONE)
    {
        if (changed)
        {
            if (context->progressive[text])
            {
                /* An error-free chunk is about to override another error-free chunk */
                if (context->adaptive_ps_counter >= RDSPARSER_PROGRESSIVE_THRESHOLD_DELAYED)
                {
                    if (context->adaptive_ps_errors)
                    {
                        /* Although decoder thinks it is error-free, we think it is not */
                        context->adaptive_ps_counter = RDSPARSER_PROGRESSIVE_THRESHOLD_INSTANT;
                        return false;
                    }
                }
            }

            /* Maybe it is a dynamic PS, so we should disable the progressive correction */
            context->progressive[text] = false;
            context->adaptive_ps_counter = 0;
        }
        else if (context->adaptive_ps_counter != RDSPARSER_PROGRESSIVE_THRESHOLD_DELAYED)
        {
            context->adaptive_ps_counter++;

            if (!context->progressive[text] &&
                context->adaptive_ps_counter == RDSPARSER_PROGRESSIVE_THRESHOLD_INSTANT)
            {
                /* The RDS PS seems to be static, se we should enable the progressive correction */
                context->progressive[text] = true;

                /* Trigger the update callback to refresh the progressive correction state */
                changed = true;
            }
        }
    }

    for (uint8_t i = 0; i < chunk_length; i++)
    {
        const char input = (i ? (uint8_t)data[data_block] : data[data_block] >> 8);
        const bool allow_eol = (text == RDSPARSER_TEXT_RT);

        changed |= rdsparser_string_update(string,
                                           position + i,
                                           input,
                                           curr_error,
                                           context->progressive[text],
                                           allow_eol,
                                           true);
    }

    return changed;
}
