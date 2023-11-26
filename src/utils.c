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
#include <string.h>
#include <librdsparser_private.h>

bool
rdsparser_utils_convert(const char        *input,
                        rdsparser_data_t   data_out,
                        rdsparser_error_t  errors_out)
{
    const size_t block_string_length = 4;
    const size_t error_string_length = 2;

    const size_t rds_len = RDSPARSER_BLOCK_COUNT * block_string_length;
    const size_t input_len = strlen(input);
    char *end;

    if (input_len == rds_len)
    {
        errors_out[RDSPARSER_BLOCK_A] = 0;
        errors_out[RDSPARSER_BLOCK_B] = 0;
        errors_out[RDSPARSER_BLOCK_C] = 0;
        errors_out[RDSPARSER_BLOCK_D] = 0;
    }
    else if (input_len == rds_len + error_string_length)
    {
        const char *ptr = input + RDSPARSER_BLOCK_COUNT * block_string_length;
        uint8_t buffer = (uint8_t)strtol(ptr, &end, 16);

        if (*end != '\0')
        {
            return false;
        }

        errors_out[RDSPARSER_BLOCK_A] = (buffer & 192) >> 6;
        errors_out[RDSPARSER_BLOCK_B] = (buffer & 48) >> 4;
        errors_out[RDSPARSER_BLOCK_C] = (buffer & 12) >> 2;
        errors_out[RDSPARSER_BLOCK_D] = (buffer & 3);
    }
    else
    {
        return false;
    }

    for (uint8_t block = 0; block < RDSPARSER_BLOCK_COUNT; block++)
    {
        char buffer[block_string_length + 1];
        for (uint8_t i = 0; i < block_string_length; i++)
        {
            buffer[i] = input[block * block_string_length + i];
        }

        buffer[block_string_length] = '\0';
        data_out[block] = (uint16_t)strtol(buffer, &end, 16);

        if (*end != '\0')
        {
            return false;
        }
    }

    return true;
}
