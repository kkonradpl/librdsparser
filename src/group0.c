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

#include <librdsparser_private.h>
#include "rdsparser.h"
#include "parser.h"
#include "string.h"

static inline bool
rdsparser_group0_get_ta(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 0x10) >> 4;
}

static inline bool
rdsparser_group0_get_ms(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 0x8) >> 3;
}

static inline uint8_t
rdsparser_group0_get_ps_pos(const rdsparser_data_t data)
{
    return data[RDSPARSER_BLOCK_B] & 3;
}

static inline uint8_t
rdsparser_group0a_get_af1(const rdsparser_data_t data)
{
    return data[RDSPARSER_BLOCK_C] >> 8;
}

static inline uint8_t
rdsparser_group0a_get_af2(const rdsparser_data_t data)
{
    return (uint8_t)data[RDSPARSER_BLOCK_C];
}

static inline void
rdsparser_group0a_parse(rdsparser_t             *rds,
                        const rdsparser_data_t   data,
                        const rdsparser_error_t  errors)
{
    if (errors[RDSPARSER_BLOCK_B] == 0 &&
        errors[RDSPARSER_BLOCK_C] == 0)
    {
        uint8_t af1 = rdsparser_group0a_get_af1(data);
        uint8_t af2 = rdsparser_group0a_get_af2(data);

        if (af1 == 250)
        {
            /* LF/MF frequency follows */
            /* Currently unsupported */
            return;
        }

        rdsparser_set_af(rds, af1);
        rdsparser_set_af(rds, af2);
    }
}

void
rdsparser_group0_parse(rdsparser_t             *rds,
                       const rdsparser_data_t   data,
                       const rdsparser_error_t  errors,
                       rdsparser_group_flag_t   flag)
{
    if (errors[RDSPARSER_BLOCK_B] == 0)
    {
        rdsparser_set_ta(rds, rdsparser_group0_get_ta(data));
        rdsparser_set_ms(rds, rdsparser_group0_get_ms(data));
    }

    const uint8_t position = 2 * rdsparser_group0_get_ps_pos(data);
    bool changed = rdsparser_parser_update_string(rds,
                                                  rds->ps,
                                                  RDSPARSER_TEXT_PS,
                                                  RDSPARSER_BLOCK_D,
                                                  data,
                                                  errors,
                                                  position);

    if (changed &&
        rds->callback_ps)
    {
        rds->callback_ps(rds,
                         rds->user_data);
    }

    if (flag == RDSPARSER_GROUP_FLAG_A)
    {
        rdsparser_group0a_parse(rds, data, errors);
    }
}


