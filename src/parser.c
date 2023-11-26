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

#include <stdint.h>
#include <librdsparser_private.h>
#include "af.h"
#include "group.h"
#include "group0.h"
#include "group1.h"
#include "group2.h"
#include "string.h"

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
    }
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
    if (errors[RDSPARSER_BLOCK_B] <= context->correction[text][RDSPARSER_BLOCK_TYPE_INFO] &&
        errors[data_block] <= context->correction[text][RDSPARSER_BLOCK_TYPE_DATA])
    {
        char block[2];
        block[0] = data[data_block] >> 8;
        block[1] = (uint8_t)data[data_block];

        return rdsparser_string_update(string,
                                       block,
                                       errors[RDSPARSER_BLOCK_B],
                                       errors[data_block],
                                       position,
                                       context->progressive[text],
                                       true);
    }

    return false;
}

