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

#include <librdsparser.h>
#include <stdint.h>
#include "ecc.h"

static const rdsparser_country_t rdsparser_ecc_eba_lut[][15] =
{
    {
        RDSPARSER_COUNTRY_GERMANY,          // 1
        RDSPARSER_COUNTRY_ALGERIA,          // 2
        RDSPARSER_COUNTRY_ANDORRA,          // 3
        RDSPARSER_COUNTRY_ISRAEL,           // 4
        RDSPARSER_COUNTRY_ITALY,            // 5
        RDSPARSER_COUNTRY_BELGIUM,          // 6
        RDSPARSER_COUNTRY_RUSSIA,           // 7
        RDSPARSER_COUNTRY_PALESTINE,        // 8
        RDSPARSER_COUNTRY_ALBANIA,          // 9
        RDSPARSER_COUNTRY_AUSTRIA,          // A
        RDSPARSER_COUNTRY_HUNGARY,          // B
        RDSPARSER_COUNTRY_MALTA,            // C
        RDSPARSER_COUNTRY_GERMANY,          // D
        RDSPARSER_COUNTRY_UNKNOWN,          // E ?? Canaries in TEF6686_ESP
        RDSPARSER_COUNTRY_EGYPT             // F
    },
    {
        RDSPARSER_COUNTRY_GREECE,           // 1
        RDSPARSER_COUNTRY_CYPRUS,           // 2
        RDSPARSER_COUNTRY_SAN_MARINO,       // 3
        RDSPARSER_COUNTRY_SWITZERLAND,      // 4
        RDSPARSER_COUNTRY_JORDAN,           // 5
        RDSPARSER_COUNTRY_FINLAND,          // 6
        RDSPARSER_COUNTRY_LUXEMBURG,        // 7
        RDSPARSER_COUNTRY_BULGARIA,         // 8
        RDSPARSER_COUNTRY_DENMARK,          // 9
        RDSPARSER_COUNTRY_GIBRALTAR,        // A
        RDSPARSER_COUNTRY_IRAQ,             // B
        RDSPARSER_COUNTRY_UNITED_KINGDOM,   // C
        RDSPARSER_COUNTRY_LIBYA,            // D
        RDSPARSER_COUNTRY_ROMANIA,          // E
        RDSPARSER_COUNTRY_FRANCE            // F
    },
    {
        RDSPARSER_COUNTRY_MOROCCO,          // 1
        RDSPARSER_COUNTRY_CZECHIA,          // 2
        RDSPARSER_COUNTRY_POLAND,           // 3
        RDSPARSER_COUNTRY_VATICAN,          // 4
        RDSPARSER_COUNTRY_SLOVAKIA,         // 5
        RDSPARSER_COUNTRY_SYRIA,            // 6
        RDSPARSER_COUNTRY_TUNISIA,          // 7
        RDSPARSER_COUNTRY_UNKNOWN,          // 8 ?? Madeira in TEF6686_ESP
        RDSPARSER_COUNTRY_LICHTENSTEIN,     // 9
        RDSPARSER_COUNTRY_ICELAND,          // A
        RDSPARSER_COUNTRY_MONACO,           // B
        RDSPARSER_COUNTRY_LITHUANIA,        // C
        RDSPARSER_COUNTRY_SERBIA,           // D
        RDSPARSER_COUNTRY_SPAIN,            // E
        RDSPARSER_COUNTRY_NORWAY            // F
    },
    {
        RDSPARSER_COUNTRY_MONTENEGRO,       // 1
        RDSPARSER_COUNTRY_IRELAND,          // 2
        RDSPARSER_COUNTRY_TURKEY,           // 3
        RDSPARSER_COUNTRY_UNKNOWN,          // 4
        RDSPARSER_COUNTRY_TAJIKISTAN,       // 5
        RDSPARSER_COUNTRY_UNKNOWN,          // 6
        RDSPARSER_COUNTRY_UNKNOWN,          // 7
        RDSPARSER_COUNTRY_NETHERLANDS,      // 8
        RDSPARSER_COUNTRY_LATVIA,           // 9
        RDSPARSER_COUNTRY_LEBANON,          // A
        RDSPARSER_COUNTRY_AZERBAIJAN,       // B
        RDSPARSER_COUNTRY_CROATIA,          // C
        RDSPARSER_COUNTRY_KAZAKHSTAN,       // D
        RDSPARSER_COUNTRY_SWEDEN,           // E
        RDSPARSER_COUNTRY_BELARUS           // F
    },
    {
        RDSPARSER_COUNTRY_MOLDOVA,          // 1
        RDSPARSER_COUNTRY_ESTONIA,          // 2
        RDSPARSER_COUNTRY_MACEDONIA,        // 3
        RDSPARSER_COUNTRY_UNKNOWN,          // 4
        RDSPARSER_COUNTRY_UNKNOWN,          // 5
        RDSPARSER_COUNTRY_UKRAINE,          // 6
        RDSPARSER_COUNTRY_KOSOVO,           // 7
        RDSPARSER_COUNTRY_PORTUGAL,         // 8
        RDSPARSER_COUNTRY_SLOVENIA,         // 9
        RDSPARSER_COUNTRY_ARMENIA,          // A
        RDSPARSER_COUNTRY_UZBEKISTAN,       // B
        RDSPARSER_COUNTRY_GEORGIA,          // C
        RDSPARSER_COUNTRY_UNKNOWN,          // D
        RDSPARSER_COUNTRY_TURKMENISTAN,     // E
        RDSPARSER_COUNTRY_BOSNIA            // F
    },
    {
        RDSPARSER_COUNTRY_UNKNOWN,          // 1
        RDSPARSER_COUNTRY_UNKNOWN,          // 2
        RDSPARSER_COUNTRY_KYRGYZSTAN,       // 3
        RDSPARSER_COUNTRY_UNKNOWN,          // 4
        RDSPARSER_COUNTRY_UNKNOWN,          // 5
        RDSPARSER_COUNTRY_UNKNOWN,          // 6
        RDSPARSER_COUNTRY_UNKNOWN,          // 7
        RDSPARSER_COUNTRY_UNKNOWN,          // 8
        RDSPARSER_COUNTRY_UNKNOWN,          // 9
        RDSPARSER_COUNTRY_UNKNOWN,          // A
        RDSPARSER_COUNTRY_UNKNOWN,          // B
        RDSPARSER_COUNTRY_UNKNOWN,          // C
        RDSPARSER_COUNTRY_UNKNOWN,          // D
        RDSPARSER_COUNTRY_UNKNOWN,          // E
        RDSPARSER_COUNTRY_UNKNOWN           // F
    },
};

rdsparser_country_t
rdsparser_ecc_lookup(int32_t pi,
                     int16_t ecc)
{
    const uint8_t pi_country = (pi >> 12) & 0xF;
    
    if (pi != RDSPARSER_PI_UNKNOWN &&
        pi_country != 0)
    {
        const uint8_t pi_id = pi_country - 1;

        if (ecc >= 0xE0 &&
            ecc <= 0xE5)
        {
            const uint8_t ecc_id = ecc - 0xE0;
            return rdsparser_ecc_eba_lut[ecc_id][pi_id];
        }
    }

    return RDSPARSER_COUNTRY_UNKNOWN;
}
