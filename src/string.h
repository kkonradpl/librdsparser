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

#ifndef RDSPARSER_STRING_H
#define RDSPARSER_STRING_H

void rdsparser_string_init(rdsparser_string_t *string, uint8_t size);
void rdsparser_string_clear(rdsparser_string_t *string);
bool rdsparser_string_update(rdsparser_string_t *string, const char input[2], rdsparser_block_error_t info_error, rdsparser_block_error_t data_error, uint8_t position, bool progressive, bool allow_eol);

#endif