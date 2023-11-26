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

#ifndef LIBRDS_STRING_H
#define LIBRDS_STRING_H

void librds_string_init(librds_string_t *string, uint8_t size);
void librds_string_clear(librds_string_t *string);
bool librds_string_update(librds_string_t *string, const char input[2], librds_block_error_t info_error, librds_block_error_t data_error, uint8_t position, bool progressive, bool allow_eol);

#endif