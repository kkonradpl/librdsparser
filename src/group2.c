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

#include "rdsparser_private.h"

uint8_t
rdsparser_group2_get_rt_pos(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 15);
}

bool
rdsparser_group2_get_rt_flag(const rdsparser_data_t data)
{
    return (data[RDSPARSER_BLOCK_B] & 16) >> 4;
}