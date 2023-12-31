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
#include "rdsparser.h"
#include "group.h"


static inline uint16_t
rdsparser_group_get_pi(const rdsparser_data_t data)
{
    return data[RDSPARSER_BLOCK_A];
}

static inline uint8_t
rdsparser_group_get_pty(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 0x03E0) >> 5;
}

static inline uint8_t
rdsparser_group_get_tp(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 0x400) >> 10;
}

void
rdsparser_group_parse(rdsparser_t             *rds,
                      const rdsparser_data_t   data,
                      const rdsparser_error_t  errors)
{
    if (errors[RDSPARSER_BLOCK_A] == 0)
    {
        rdsparser_set_pi(rds, rdsparser_group_get_pi(data));
    }

    if (errors[RDSPARSER_BLOCK_B] == 0)
    {
        rdsparser_set_pty(rds, rdsparser_group_get_pty(data));
        rdsparser_set_tp(rds, rdsparser_group_get_tp(data));
    }
}
