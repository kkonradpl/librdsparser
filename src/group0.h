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

#ifndef LIBRDS_GROUP0_H
#define LIBRDS_GROUP0_H
#include "librds_private.h"

bool librds_group0_get_ta(const librds_data_t data);
bool librds_group0_get_ms(const librds_data_t data);
uint8_t librds_group0_get_af1(const librds_data_t data);
uint8_t librds_group0_get_af2(const librds_data_t data);
uint8_t librds_group0_get_ps_pos(const librds_data_t data);

#endif