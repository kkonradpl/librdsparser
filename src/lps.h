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

#ifndef RDSPARSER_LPS_H
#define RDSPARSER_LPS_H
#include <librdsparser_private.h>

void rdsparser_lps_init(rdsparser_lps_t *lps);
void rdsparser_lps_update(rdsparser_lps_t *lps, uint8_t position, char value);
const char* rdsparser_lps_get(const rdsparser_lps_t *lps);

#endif