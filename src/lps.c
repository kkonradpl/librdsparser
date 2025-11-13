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
#include <stdio.h>

void
rdsparser_lps_init(rdsparser_lps_t *lps)
{
    lps->data[RDSPARSER_LPS_MAX_LENGTH] = '\0';
    lps->valid = 0;
}

void
rdsparser_lps_update(rdsparser_lps_t *lps,
                     uint8_t          position,
                     char             value)
{
    lps->data[position] = (value == '\r' ? '\0' : value);
    lps->valid |= (uint32_t)1 << position;
}

const char*
rdsparser_lps_get(const rdsparser_lps_t *lps)
{
    for (uint8_t i = 0; i < RDSPARSER_LPS_MAX_LENGTH; i++)
    {
        if ((lps->valid & ((uint32_t)1 << i)) == 0)
        {
            return NULL;
        }

        if (lps->data[i] == '\0')
        {
            /* Shorter than 32 bytes */
            break;
        }
    }

    return lps->data;
}
