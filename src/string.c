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
#include <librdsparser_private.h>
#include "string.h"

void
rdsparser_string_init(rdsparser_string_t *string,
                      uint8_t             max_length)
{
    *string = max_length;
}

static uint8_t
rdsparser_string_get_size(const rdsparser_string_t *string)
{
    return (uint8_t)string[0];
}

uint8_t
rdsparser_string_get_length(const rdsparser_string_t *string)
{
    const uint8_t size = rdsparser_string_get_size(string);
    for (uint8_t i = 0; i < size; i++)
    {
        const rdsparser_string_char_t *content = rdsparser_string_get_content(string);
        if (content[i] == '\0')
        {
            return i;
        }
    }

    return size;
}

const rdsparser_string_char_t*
rdsparser_string_get_content(const rdsparser_string_t *string)
{
    return (string + 1);
}

const rdsparser_string_error_t*
rdsparser_string_get_errors(const rdsparser_string_t *string)
{
    const uint8_t size = rdsparser_string_get_size(string);
    return (uint8_t*)(string + 1 + size + 1);
}

bool
rdsparser_string_get_available(const rdsparser_string_t *string)
{
    const uint8_t size = rdsparser_string_get_size(string);
    const rdsparser_string_error_t *errors = rdsparser_string_get_errors(string);

    for (uint8_t i = 0; i < size; i++)
    {
        if (errors[i] != RDSPARSER_STRING_ERROR_UNCORRECTABLE)
        {
            return true;
        }
    }

    return false;
}

void
rdsparser_string_clear(rdsparser_string_t *string)
{
    const uint8_t size = rdsparser_string_get_size(string);
    rdsparser_string_char_t *content = (rdsparser_string_char_t*)rdsparser_string_get_content(string);
    rdsparser_string_error_t *errors = (rdsparser_string_error_t*)rdsparser_string_get_errors(string);

    for (uint8_t i = 0; i < size; i++)
    {
        content[i] = ' ';
        errors[i] = RDSPARSER_STRING_ERROR_UNCORRECTABLE;
    }
}

static rdsparser_string_error_t
rdsparser_string_calculate_error(rdsparser_block_error_t info_error,
                                 rdsparser_block_error_t data_error)
{
    const uint8_t value = 2 * info_error + 3 * data_error;
    return (rdsparser_string_error_t)(value ? value - 1 : 0);
}

static rdsparser_string_char_t
rdsparser_string_convert(uint8_t input)
{
    if (input == '\r')
    {
        return '\0';
    }

#ifdef RDSPARSER_DISABLE_UNICODE
    return input;
#else
    const uint8_t offset = 0x20;
    const wchar_t charset[] =
    {
        L' ', L'!', L'"', L'#', L'¤', L'%', L'&', L'\'',
        L'(', L')', L'*', L'+', L',', L'-', L'.', L'/',
        L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7',
        L'8', L'9', L':', L';', L'<', L'=', L'>', L'?',
        L'@', L'A', L'B', L'C', L'D', L'E', L'F', L'G',
        L'H', L'I', L'J', L'K', L'L', L'M', L'N', L'O',
        L'P', L'Q', L'R', L'S', L'T', L'U', L'V', L'W',
        L'X', L'Y', L'Z', L'[', L'\\',L']', L'―', L'_',
        L'‖', L'a', L'b', L'c', L'd', L'e', L'f', L'g',
        L'h', L'i', L'j', L'k', L'l', L'm', L'n', L'o',
        L'p', L'q', L'r', L's', L't', L'u', L'v', L'w',
        L'x', L'y', L'z', L'{', L'|', L'}', L'¯', L' ',
        L'á', L'à', L'é', L'è', L'í', L'ì', L'ó', L'ò',
        L'ú', L'ù', L'Ñ', L'Ç', L'Ş', L'β', L'¡', L'Ĳ',
        L'â', L'ä', L'ê', L'ë', L'î', L'ï', L'ô', L'ö',
        L'û', L'ü', L'ñ', L'ç', L'ş', L'ǧ', L'ı', L'ĳ',
        L'ª', L'α', L'©', L'‰', L'Ǧ', L'ě', L'ň', L'ő',
        L'π', L'€', L'£', L'$', L'←', L'↑', L'→', L'↓',
        L'º', L'¹', L'²', L'³', L'±', L'İ', L'ń', L'ű',
        L'µ', L'¿', L'÷', L'°', L'¼', L'½', L'¾', L'§',
        L'Á', L'À', L'É', L'È', L'Í', L'Ì', L'Ó', L'Ò',
        L'Ú', L'Ù', L'Ř', L'Č', L'Š', L'Ž', L'Ð', L'Ŀ',
        L'Â', L'Ä', L'Ê', L'Ë', L'Î', L'Ï', L'Ô', L'Ö',
        L'Û', L'Ü', L'ř', L'č', L'š', L'ž', L'đ', L'ŀ',
        L'Ã', L'Å', L'Æ', L'Œ', L'ŷ', L'Ý', L'Õ', L'Ø',
        L'Þ', L'Ŋ', L'Ŕ', L'Ć', L'Ś', L'Ź', L'Ŧ', L'ð',
        L'ã', L'å', L'æ', L'œ', L'ŵ', L'ý', L'õ', L'ø',
        L'þ', L'ŋ', L'ŕ', L'ć', L'ś', L'ź', L'ŧ', L' '
    };

    /* TODO: Implement SO and LS2 tables (and charset detection) */
    const wchar_t fallback_char = L' ';
    return (input < offset ? fallback_char : charset[input - offset]);
#endif
}

static bool
rdsparser_string_update_single(rdsparser_string_t      *string,
                               uint8_t                  input,
                               rdsparser_block_error_t  info_error,
                               rdsparser_block_error_t  data_error,
                               uint8_t                  position,
                               bool                     progressive,
                               bool                     allow_eol)
{
    rdsparser_string_char_t *output = (rdsparser_string_char_t*)rdsparser_string_get_content(string);
    rdsparser_string_error_t *output_errors = (rdsparser_string_error_t*)rdsparser_string_get_errors(string);
    rdsparser_string_error_t error = rdsparser_string_calculate_error(info_error, data_error);

    if (progressive &&
        output_errors[position] < error)
    {
        /* Ignore larger correction in progressive mode */
        return false;
    }

    if (input == '\r')
    {
        if (!allow_eol ||
            info_error != RDSPARSER_BLOCK_ERROR_NONE ||
            data_error != RDSPARSER_BLOCK_ERROR_NONE)
        {
            /* Only error-free line endings */
            return false;
        }
    }
    else if (input < 0x20)
    {
        /* Not printable character */
        return false;
    }

    if (input >= 0x7F)
    {
        if (info_error != RDSPARSER_BLOCK_ERROR_NONE ||
            data_error != RDSPARSER_BLOCK_ERROR_NONE)
        {
            /* Special characters are used rarely,
               so use only error-free info and data */
            return false;
        }
#ifdef RDSPARSER_DISABLE_UNICODE
        input = ' ';
#endif
    }

    rdsparser_string_char_t character = rdsparser_string_convert(input);
    if (output[position] == character &&
        output_errors[position] <= error)
    {
        /* Ignore the same data with same or larger error correction */
        return false;
    }

    output[position] = character;
    output_errors[position] = error;
    return true;
}

bool
rdsparser_string_update(rdsparser_string_t      *string,
                        const char               input[2],
                        rdsparser_block_error_t  info_error,
                        rdsparser_block_error_t  data_error,
                        uint8_t                  position,
                        bool                     progressive,
                        bool                     allow_eol)
{
    const uint8_t chunk_length = 2;
    bool changed = false;

    for (uint8_t i = 0; i < chunk_length; i++)
    {
        changed |= rdsparser_string_update_single(string,
                                                  input[i],
                                                  info_error,
                                                  data_error,
                                                  position + i,
                                                  progressive,
                                                  allow_eol);
    }

    return changed;
}
