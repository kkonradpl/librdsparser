/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
 *  Copyright (C) 2023-2024  Konrad Kosmatka
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

bool
rdsparser_af_set(rdsparser_af_t *af,
                 uint8_t         value)
{
    if (value >= 1 &&
        value <= 204)
    {
        const uint8_t pos = value / 8;
        const uint8_t bitPos = value % 8;
        af->buffer[pos] |= (0x80 >> bitPos);
        return true;
    }

    return false;
}

bool
rdsparser_af_get(const rdsparser_af_t *af,
                 uint8_t               value)
{
    if (value >= 1 &&
        value <= 204)
    {
        const uint8_t pos = value / 8;
        const uint8_t bitPos = value % 8;
        return af->buffer[pos] & (0x80 >> bitPos);
    }

    return false;
}

void
rdsparser_af_clear(rdsparser_af_t *af)
{
    for (uint8_t i = 0; i < RDSPARSER_AF_BUFFER_SIZE; i++)
    {
        af->buffer[i] = 0;
    }
}

