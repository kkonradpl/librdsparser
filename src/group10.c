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
rdsparser_group10a_get_ptyn_pos(const rdsparser_data_t data)
{
    return data[RDSPARSER_BLOCK_B] & 1;
}

static inline void
rdsparser_group10a_parse(rdsparser_t             *rds,
                         const rdsparser_data_t   data,
                         const rdsparser_error_t  errors)
{
    uint8_t position = 4 * rdsparser_group10a_get_ptyn_pos(data);

    bool changed = false;
    changed |= rdsparser_parser_update_string(rds,
                                              rds->ptyn,
                                              RDSPARSER_TEXT_PTYN,
                                              RDSPARSER_BLOCK_C,
                                              data,
                                              errors,
                                              position);

    changed |= rdsparser_parser_update_string(rds,
                                              rds->ptyn,
                                              RDSPARSER_TEXT_PTYN,
                                              RDSPARSER_BLOCK_D,
                                              data,
                                              errors,
                                              position + 2);

    if (changed &&
        rds->callback_ptyn)
    {
        rds->callback_ptyn(rds, rds->user_data);
    }
}

void
rdsparser_group10_parse(rdsparser_t             *rds,
                        const rdsparser_data_t   data,
                        const rdsparser_error_t  errors,
                        rdsparser_group_flag_t   flag)
{
    if (flag == RDSPARSER_GROUP_FLAG_A)
    {
        rdsparser_group10a_parse(rds, data, errors);
    }
}
