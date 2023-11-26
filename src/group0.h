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

#ifndef RDSPARSER_GROUP0_H
#define RDSPARSER_GROUP0_H
#include "rdsparser_private.h"

bool rdsparser_group0_get_ta(const rdsparser_data_t data);
bool rdsparser_group0_get_ms(const rdsparser_data_t data);
uint8_t rdsparser_group0_get_af1(const rdsparser_data_t data);
uint8_t rdsparser_group0_get_af2(const rdsparser_data_t data);
uint8_t rdsparser_group0_get_ps_pos(const rdsparser_data_t data);

#endif