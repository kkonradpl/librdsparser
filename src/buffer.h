/*  SPDX-License-Identifier: LGPL-2.1-or-later
 *
 *  librdsparser – Radio Data System parser library
 *  Copyright (C) 2024  Konrad Kosmatka
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

#ifndef RDSPARSER_BUFFER_H
#define RDSPARSER_BUFFER_H
#include <librdsparser_private.h>

void rdsparser_buffer_init(rdsparser_buffer_t *buffer);
void rdsparser_buffer_clear(rdsparser_buffer_t *buffer);

void rdsparser_buffer_set_extended_check(rdsparser_buffer_t *buffer, bool value);
bool rdsparser_buffer_get_extended_check(const rdsparser_buffer_t *buffer);

bool rdsparser_buffer_update_pi(rdsparser_buffer_t *buffer, rdsparser_pi_t value);
rdsparser_pi_t rdsparser_buffer_get_pi(const rdsparser_buffer_t *buffer);

bool rdsparser_buffer_update_pty(rdsparser_buffer_t *buffer, rdsparser_pty_t value);
rdsparser_pty_t rdsparser_buffer_get_pty(const rdsparser_buffer_t *buffer);

bool rdsparser_buffer_update_tp(rdsparser_buffer_t *buffer, rdsparser_tp_t value);
rdsparser_tp_t rdsparser_buffer_get_tp(const rdsparser_buffer_t *buffer);

bool rdsparser_buffer_update_ta(rdsparser_buffer_t *buffer, rdsparser_ta_t value);
rdsparser_ta_t rdsparser_buffer_get_ta(const rdsparser_buffer_t *buffer);

bool rdsparser_buffer_update_ms(rdsparser_buffer_t *buffer, rdsparser_ms_t value);
rdsparser_ms_t rdsparser_buffer_get_ms(const rdsparser_buffer_t *buffer);

bool rdsparser_buffer_update_ecc(rdsparser_buffer_t *buffer, rdsparser_ecc_t value);
rdsparser_ecc_t rdsparser_buffer_get_ecc(const rdsparser_buffer_t *buffer);

bool rdsparser_buffer_update_country(rdsparser_buffer_t *buffer, rdsparser_country_t value);
rdsparser_country_t rdsparser_buffer_get_country(const rdsparser_buffer_t *buffer);

bool rdsparser_buffer_add_af(rdsparser_buffer_t *buffer, uint8_t value);
const rdsparser_af_t* rdsparser_buffer_get_af(const rdsparser_buffer_t *buffer);

#endif