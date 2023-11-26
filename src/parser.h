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

#ifndef RDSPARSER_PARSER_H
#define RDSPARSER_PARSER_H
#include <librdsparser_private.h>

void rdsparser_parser_process(rdsparser_t *rds, const rdsparser_data_t data, const rdsparser_error_t errors);
bool rdsparser_parser_update_string(rdsparser_t *rds, rdsparser_string_t *string, rdsparser_text_t text, rdsparser_block_t data_block, const rdsparser_data_t data, const rdsparser_error_t errors, uint8_t position);

#endif