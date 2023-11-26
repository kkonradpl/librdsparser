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

static inline uint8_t
rdsparser_group2_get_rt_pos(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 15);
}

static inline bool
rdsparser_group2_get_rt_flag(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 16) >> 4;
}

void
rdsparser_group2_parse(rdsparser_t             *rds,
                       const rdsparser_data_t   data,
                       const rdsparser_error_t  errors,
                       rdsparser_group_flag_t   flag)
{
    rdsparser_rt_flag_t rt_flag = rdsparser_group2_get_rt_flag(data);
    bool changed = false;

    if (errors[RDSPARSER_BLOCK_B] == 0 &&
        rt_flag != rds->last_rt_flag)
    {
        if (rds->last_rt_flag != -1 &&
            rdsparser_string_get_available(rds->rt[rt_flag]))
        {
            rdsparser_string_clear(rds->rt[rt_flag]);
            changed = true;
        }

        rds->last_rt_flag = rt_flag;
    }

    if (errors[RDSPARSER_BLOCK_B] != 0 &&
        rt_flag != rds->last_rt_flag &&
        rds->last_rt_flag != -1)
    {
        /* Possible bit-flip of the RDS RT flag
           in BLOCK_B, ignore the group */
        return;
    }

    uint8_t position;
    if (flag == RDSPARSER_GROUP_FLAG_A)
    {
        position = 4 * rdsparser_group2_get_rt_pos(data);
        changed |= rdsparser_parser_update_string(rds,
                                                  rds->rt[rt_flag],
                                                  RDSPARSER_TEXT_RT,
                                                  RDSPARSER_BLOCK_C,
                                                  data,
                                                  errors,
                                                  position);
        position += 2;
    }
    else
    {
        position = 2 * rdsparser_group2_get_rt_pos(data);
    }

    changed |= rdsparser_parser_update_string(rds,
                                              rds->rt[rt_flag],
                                              RDSPARSER_TEXT_RT,
                                              RDSPARSER_BLOCK_D,
                                              data,
                                              errors,
                                              position);

    if (changed &&
        rds->callback_rt)
    {
        rds->callback_rt(rds,
                         rt_flag,
                         rds->user_data);
    }
}
