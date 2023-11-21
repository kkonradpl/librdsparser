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

#ifndef LIBRDS_GROUP_H
#define LIBRDS_GROUP_H
#include "librds_private.h"

uint16_t librds_group_get_pi(const librds_data_t data);
uint8_t librds_group_get_group(const librds_data_t data);
librds_group_flag_t librds_group_get_flag(const librds_data_t data);
uint8_t librds_group_get_pty(const librds_data_t data);
uint8_t librds_group_get_tp(const librds_data_t data);

#endif