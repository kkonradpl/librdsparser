/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librds – Radio Data System parser library
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
#include "librds.h"


librds_string_error_t
librds_string_error(librds_block_error_t pos_error,
                    librds_block_error_t data_error)
{
    const uint8_t value = 2 * pos_error + 3 * data_error;
    return (librds_string_error_t)(value ? value - 1 : 0);
}

void
librds_string_clear(char                  *string,
                    librds_string_error_t *errors,
                    uint8_t                length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        string[i] = ' ';
        errors[i] = LIBRDS_STRING_ERROR_UNCORRECTABLE;
    }
}

bool
librds_string_available(const librds_string_error_t *errors,
                        uint8_t                      length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        if (errors[i] != LIBRDS_STRING_ERROR_UNCORRECTABLE)
        {
            return true;
        }
    }

    return false;
}

static bool
librds_string_update_single(char                  *output,
                            librds_string_error_t *output_errors,
                            char                   input,
                            librds_string_error_t  input_error,
                            uint8_t                position,
                            bool                   progressive,
                            bool                   allow_eol)
{
    if (output[position] == input &&
        output_errors[position] <= input_error)
    {
        /* Ignore the same data with same or larger error correction */
        return false;
    }

    if (progressive &&
        output_errors[position] < input_error)
    {
        /* Ignore larger correction in progressive mode */
        return false;
    }

    if (allow_eol &&
        input == '\r' &&
        input_error == LIBRDS_STRING_ERROR_NONE)
    {
        /* End of RT message line */
        input = '\0';
    }
    else if (input < 0x20 ||
             input >= 0x7F)
    {
        /* Not printable character */
        return false;
    }

    output[position] = input;
    output_errors[position] = input_error;
    return true;
}

bool
librds_string_update(char                  *output,
                     librds_string_error_t *output_errors,
                     const char             input[2],
                     librds_string_error_t  input_error,
                     uint8_t                position,
                     bool                   progressive,
                     bool                   allow_eol)
{
    const uint8_t chunk_length = 2;
    bool changed = false;

    for (uint8_t i = 0; i < chunk_length; i++)
    {
        changed |= librds_string_update_single(output,
                                               output_errors,
                                               input[i],
                                               input_error,
                                               position + i,
                                               progressive,
                                               allow_eol);
    }

    return changed;
}
