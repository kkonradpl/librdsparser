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

#ifndef RDSPARSER_AF_H
#define RDSPARSER_AF_H
#include <stdint.h>
#include <librdsparser.h>

bool rdsparser_af_set(rdsparser_af_t *af, uint8_t value);
bool rdsparser_af_get(const rdsparser_af_t *af, uint8_t value);
void rdsparser_af_clear(rdsparser_af_t *af);

#endif
