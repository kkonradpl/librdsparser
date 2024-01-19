/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
 *  Copyright (C) 2024  Konrad Kosmatka
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
#include "ct.h"

static inline uint32_t
rdsparser_group4a_get_mjd(const rdsparser_data_t data)
{
    uint32_t mjd = (uint32_t)((data[RDSPARSER_BLOCK_B] & 3)) << 15;
    mjd |= (data[RDSPARSER_BLOCK_C] >> 1);

    return mjd;
}

static inline uint8_t
rdsparser_group4a_get_hour(const rdsparser_data_t data)
{
    uint8_t hour = (data[RDSPARSER_BLOCK_C] & 1) << 4;
    hour |= (data[RDSPARSER_BLOCK_D] & 0xF000) >> 12;

    return hour;
}

static inline uint8_t
rdsparser_group4a_get_minute(const rdsparser_data_t data)
{
    uint8_t minute = (data[RDSPARSER_BLOCK_D] & 0xFC0) >> 6;

    return minute;
}

static inline int8_t
rdsparser_group4a_get_time_offset(const rdsparser_data_t data)
{
    int8_t offset = (data[RDSPARSER_BLOCK_D] & 0x1F);

    return (data[RDSPARSER_BLOCK_D] & 0x20) ? -offset : offset;
}

static inline void
rdsparser_group4a_parse(rdsparser_t             *rds,
                        const rdsparser_data_t   data,
                        const rdsparser_error_t  errors)
{
    if (errors[RDSPARSER_BLOCK_B] == 0 &&
        errors[RDSPARSER_BLOCK_C] == 0 &&
        errors[RDSPARSER_BLOCK_D] == 0)
    {
        uint32_t mjd = rdsparser_group4a_get_mjd(data);
        int8_t hour = rdsparser_group4a_get_hour(data);
        int8_t minute = rdsparser_group4a_get_minute(data);
        int8_t offset = rdsparser_group4a_get_time_offset(data);

        rdsparser_ct_t ct;
        if (rds->callback_ct &&
            rdsparser_ct_init(&ct, mjd, hour, minute, offset))
        {
            rds->callback_ct(rds, &ct, rds->user_data);
        }
    }
}

void
rdsparser_group4_parse(rdsparser_t             *rds,
                       const rdsparser_data_t   data,
                       const rdsparser_error_t  errors,
                       rdsparser_group_flag_t   flag)
{
    if (flag == RDSPARSER_GROUP_FLAG_A)
    {
        rdsparser_group4a_parse(rds, data, errors);
    }
}
