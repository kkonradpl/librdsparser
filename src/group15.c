/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
 *  Copyright (C) 2025  Konrad Kosmatka
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
#include "lps.h"

static inline uint8_t
rdsparser_group15a_get_lps_pos(const rdsparser_data_t data)
{
    return data[RDSPARSER_BLOCK_B] & 7;
}

static inline void
rdsparser_group15a_parse(rdsparser_t             *rds,
                         const rdsparser_data_t   data,
                         const rdsparser_error_t  errors)
{
    if (errors[RDSPARSER_BLOCK_B] != RDSPARSER_BLOCK_ERROR_NONE)
    {
        /* This is UTF-8 string, accept only reliable data */
        return;
    }

    const uint8_t position = 4 * rdsparser_group15a_get_lps_pos(data);
    bool available = rdsparser_lps_get(&rds->lps) != NULL;

    if (errors[RDSPARSER_BLOCK_C] == RDSPARSER_BLOCK_ERROR_NONE)
    {
        rdsparser_lps_update(&rds->lps, position + 0, data[RDSPARSER_BLOCK_C] >> 8);
        rdsparser_lps_update(&rds->lps, position + 1, data[RDSPARSER_BLOCK_C] & 0xFF);
    }

    if (errors[RDSPARSER_BLOCK_D] == RDSPARSER_BLOCK_ERROR_NONE)
    {
        rdsparser_lps_update(&rds->lps, position + 2, data[RDSPARSER_BLOCK_D] >> 8);
        rdsparser_lps_update(&rds->lps, position + 3, data[RDSPARSER_BLOCK_D] & 0xFF);
    }

    if (!available &&
        rdsparser_lps_get(&rds->lps) != NULL &&
        rds->callback_lps)
    {
        rds->callback_lps(rds, rds->user_data);
    }
}

void
rdsparser_group15_parse(rdsparser_t             *rds,
                        const rdsparser_data_t   data,
                        const rdsparser_error_t  errors,
                        rdsparser_group_flag_t   flag)
{
    if (flag == RDSPARSER_GROUP_FLAG_A)
    {
        rdsparser_group15a_parse(rds, data, errors);
    }
}
