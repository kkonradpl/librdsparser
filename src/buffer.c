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

#include <stdint.h>
#include <librdsparser_private.h>
#include "buffer.h"
#include "af.h"

#define RDSPARSER_BUFFER_UPDATE(buffer, name, value) \
    if (buffer->data_used.name == value || \
        (buffer->extended_check && buffer->data_temp.name != value)) \
    { \
        buffer->data_temp.name = value; \
        return false; \
    } \
    buffer->data_used.name = value; \
    return true;


static void
rdsparser_buffer_data_clear(rdsparser_buffer_data_t *data)
{
    data->pi = RDSPARSER_PI_UNKNOWN;
    data->pty = RDSPARSER_PTY_UNKNOWN;
    data->tp = RDSPARSER_TP_UNKNOWN;
    data->ta = RDSPARSER_TA_UNKNOWN;
    data->ms = RDSPARSER_MS_UNKNOWN;
    data->ecc = RDSPARSER_ECC_UNKNOWN;
    data->country = RDSPARSER_COUNTRY_UNKNOWN;
    rdsparser_af_clear(&data->af);
}

void
rdsparser_buffer_init(rdsparser_buffer_t *buffer)
{
    rdsparser_buffer_clear(buffer);
    buffer->extended_check = false;
}

void
rdsparser_buffer_clear(rdsparser_buffer_t *buffer)
{
    rdsparser_buffer_data_clear(&buffer->data_used);
    rdsparser_buffer_data_clear(&buffer->data_temp);
}

void
rdsparser_buffer_set_extended_check(rdsparser_buffer_t *buffer,
                                    bool                value)
{
    buffer->extended_check = value;
}

bool
rdsparser_buffer_get_extended_check(const rdsparser_buffer_t *buffer)
{
    return buffer->extended_check;
}

bool
rdsparser_buffer_update_pi(rdsparser_buffer_t *buffer,
                           rdsparser_pi_t      value)
{
    RDSPARSER_BUFFER_UPDATE(buffer, pi, value);
}

rdsparser_pi_t
rdsparser_buffer_get_pi(const rdsparser_buffer_t *buffer)
{
    return buffer->data_used.pi;
}

bool
rdsparser_buffer_update_pty(rdsparser_buffer_t *buffer,
                            rdsparser_pty_t     value)
{
    RDSPARSER_BUFFER_UPDATE(buffer, pty, value);
}

rdsparser_pty_t
rdsparser_buffer_get_pty(const rdsparser_buffer_t *buffer)
{
    return buffer->data_used.pty;
}

bool
rdsparser_buffer_update_tp(rdsparser_buffer_t *buffer,
                           rdsparser_tp_t      value)
{
    RDSPARSER_BUFFER_UPDATE(buffer, tp, value);
}

rdsparser_tp_t
rdsparser_buffer_get_tp(const rdsparser_buffer_t *buffer)
{
    return buffer->data_used.tp;
}

bool
rdsparser_buffer_update_ta(rdsparser_buffer_t *buffer,
                           rdsparser_ta_t      value)
{
    RDSPARSER_BUFFER_UPDATE(buffer, ta, value);
}

rdsparser_ta_t
rdsparser_buffer_get_ta(const rdsparser_buffer_t *buffer)
{
    return buffer->data_used.ta;
}

bool
rdsparser_buffer_update_ms(rdsparser_buffer_t *buffer,
                           rdsparser_ms_t      value)
{
    RDSPARSER_BUFFER_UPDATE(buffer, ms, value);
}

rdsparser_ms_t
rdsparser_buffer_get_ms(const rdsparser_buffer_t *buffer)
{
    return buffer->data_used.ms;
}

bool
rdsparser_buffer_update_ecc(rdsparser_buffer_t *buffer,
                            rdsparser_ecc_t     value)
{
    RDSPARSER_BUFFER_UPDATE(buffer, ecc, value);
}

rdsparser_ecc_t
rdsparser_buffer_get_ecc(const rdsparser_buffer_t *buffer)
{
    return buffer->data_used.ecc;
}

bool
rdsparser_buffer_update_country(rdsparser_buffer_t  *buffer,
                                rdsparser_country_t  value)
{
    RDSPARSER_BUFFER_UPDATE(buffer, country, value);
}

rdsparser_country_t
rdsparser_buffer_get_country(const rdsparser_buffer_t *buffer)
{
    return buffer->data_used.country;
}

bool
rdsparser_buffer_add_af(rdsparser_buffer_t *buffer,
                        uint8_t             value)
{
    if (!rdsparser_af_get(&buffer->data_used.af, value))
    {
        if (buffer->extended_check && 
            !rdsparser_af_get(&buffer->data_temp.af, value))
        {
            rdsparser_af_set(&buffer->data_temp.af, value);
            return false;
        }

        rdsparser_af_set(&buffer->data_used.af, value);
        return true;
    }
    
    return false;
}

const rdsparser_af_t*
rdsparser_buffer_get_af(const rdsparser_buffer_t *buffer)
{
    return &buffer->data_used.af;
}
