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
#include <stdbool.h>
#include "af.h"

bool
rdsparser_af_add(rdsparser_af_t buffer,
                 uint8_t        new_af)
{
    if (new_af == 0 ||
        new_af > 204)
    {
        return false;
    }

    const uint8_t pos = new_af / 8;
    const uint8_t bitPos = new_af % 8;

    const bool exists = buffer[pos] & (0x80 >> bitPos);
    if (!exists)
    {
        buffer[pos] |= (0x80 >> bitPos);
        return true;
    }

    return false;
}

void
rdsparser_af_clear(rdsparser_af_t buffer)
{
    for (uint8_t i = 0; i < RDSPARSER_AF_BUFFER_SIZE; i++)
    {
        buffer[i] = 0;
    }
}

