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

static inline uint8_t
rdsparser_group1a_get_variant(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_C] & 0x7000) >> 12;
}

static inline uint8_t
rdsparser_group1a0_get_ecc(const rdsparser_data_t data)
{
    return (uint8_t)data[RDSPARSER_BLOCK_C];
}

static inline void
rdsparser_group1a_parse(rdsparser_t             *rds,
                        const rdsparser_data_t   data,
                        const rdsparser_error_t  errors)
{
    if (errors[RDSPARSER_BLOCK_B] == 0 &&
        errors[RDSPARSER_BLOCK_C] == 0)
    {
        if (rdsparser_group1a_get_variant(data) == 0)
        {
            rdsparser_set_ecc(rds, rdsparser_group1a0_get_ecc(data));
        }
    }
}

void
rdsparser_group1_parse(rdsparser_t             *rds,
                       const rdsparser_data_t   data,
                       const rdsparser_error_t  errors,
                       rdsparser_group_flag_t   flag)
{
    if (flag == RDSPARSER_GROUP_FLAG_A)
    {
        rdsparser_group1a_parse(rds, data, errors);
    }
}
