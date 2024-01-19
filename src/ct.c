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
#include <stdbool.h>
#include "ct.h"

bool
rdsparser_ct_init(rdsparser_ct_t *ct,
                  uint32_t        mjd,
                  int8_t          hour,
                  int8_t          minute,
                  int8_t          offset)
{
    if (hour >= 24 ||
        minute >= 60)
    {
        /* Corrupted data, ignore */
        return false;
    }

    minute += (offset % 2) * 30;
    if (minute >= 60)
    {
        hour++;
        minute %= 60;
    }
    else if (minute < 0)
    {
        hour--;
        minute = 60 + minute;
    }

    hour += (offset / 2);
    if (hour >= 24)
    {
        mjd++;
        hour %= 24;
    }
    else if (hour < 0)
    {
        mjd--;
        hour = 24 + hour;
    }

    ct->year = ((mjd * 100 - 1507820) / 36525);
    uint32_t year_tmp = (ct->year * 36525) / 100;
    ct->month = ((mjd * 100 - 1495610) - year_tmp * 100) * 100 / 306001;
    uint32_t month_tmp = (ct->month * 306001) / 10000;
    ct->day = mjd - 14956 - year_tmp - month_tmp;

    uint8_t k = (ct->month == 14 || ct->month == 15) ? 1 : 0;
    ct->year = 1900 + ct->year + k;
    ct->month = ct->month - 1 - k * 12;
    
    ct->hour = hour;
    ct->minute = minute;
    ct->offset = offset;
    
    return true;
}

uint16_t
rdsparser_ct_get_year(const rdsparser_ct_t *ct)
{
    return ct->year;
}

uint8_t
rdsparser_ct_get_month(const rdsparser_ct_t *ct)
{
    return ct->month;
}

uint8_t
rdsparser_ct_get_day(const rdsparser_ct_t *ct)
{
    return ct->day;
}

uint8_t
rdsparser_ct_get_hour(const rdsparser_ct_t *ct)
{
    return ct->hour;
}

uint8_t
rdsparser_ct_get_minute(const rdsparser_ct_t *ct)
{
    return ct->minute;
}

int16_t
rdsparser_ct_get_offset(const rdsparser_ct_t *ct)
{
    return (int16_t)ct->offset * 30;
}
