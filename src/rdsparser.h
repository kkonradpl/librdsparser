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

#ifndef RDSPARSER_RDSPARSER_H
#define RDSPARSER_RDSPARSER_H

void rdsparser_set_pi(rdsparser_t *rds, rdsparser_pi_t pi);
void rdsparser_set_pty(rdsparser_t *rds, rdsparser_pty_t pty);
void rdsparser_set_tp(rdsparser_t *rds, rdsparser_tp_t tp);
void rdsparser_set_ta(rdsparser_t *rds, rdsparser_ta_t ta);
void rdsparser_set_ms(rdsparser_t *rds, rdsparser_ms_t ms);
void rdsparser_set_ecc(rdsparser_t *rds, rdsparser_ecc_t ecc);
void rdsparser_set_af(rdsparser_t *rds, uint8_t new_af);

#endif