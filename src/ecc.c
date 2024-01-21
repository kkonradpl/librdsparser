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

/* ECC LUTs based on IEC 62106-4:2018 */

static const rdsparser_country_t rdsparser_ecc_a0_a6_lut[][15] =
{
    {   /* A0 */
        /* 1 */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* 2 */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* 3 */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* 4 */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* 5 */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* 6 */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* 7 */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* 8 */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* 9 */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* A */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* B */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* C */ RDSPARSER_COUNTRY_UNKNOWN,
        /* D */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR,
        /* E */ RDSPARSER_COUNTRY_USA_OR_VI_OR_PR
    },
    {   /* A1 */
        /* 1 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 2 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 3 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 6 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 7 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 8 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 9 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* A */ RDSPARSER_COUNTRY_UNKNOWN,
        /* B */ RDSPARSER_COUNTRY_CANADA,
        /* C */ RDSPARSER_COUNTRY_CANADA,
        /* D */ RDSPARSER_COUNTRY_CANADA,
        /* E */ RDSPARSER_COUNTRY_CANADA,
        /* F */ RDSPARSER_COUNTRY_GREENLAND
    },
    {   /* A2 */
        /* 1 */ RDSPARSER_COUNTRY_ANGUILLA,
        /* 2 */ RDSPARSER_COUNTRY_ANTIGUA_AND_BARBUDA,
        /* 3 */ RDSPARSER_COUNTRY_BRAZIL_OR_EQUATOR,
        /* 4 */ RDSPARSER_COUNTRY_FALKLAND_ISLANDS,
        /* 5 */ RDSPARSER_COUNTRY_BARBADOS,
        /* 6 */ RDSPARSER_COUNTRY_BELIZE,
        /* 7 */ RDSPARSER_COUNTRY_CAYMAN_ISLANDS,
        /* 8 */ RDSPARSER_COUNTRY_COSTA_RICA,
        /* 9 */ RDSPARSER_COUNTRY_CUBA,
        /* A */ RDSPARSER_COUNTRY_ARGENTINA,
        /* B */ RDSPARSER_COUNTRY_BRAZIL,
        /* C */ RDSPARSER_COUNTRY_BRAZIL_OR_BERMUDA,
        /* D */ RDSPARSER_COUNTRY_BRAZIL_OR_NETHERLANDS_ANTILLES, 
        /* E */ RDSPARSER_COUNTRY_GUADELOUPE,
        /* F */ RDSPARSER_COUNTRY_BAHAMAS
    },
    {   /* A3 */
        /* 1 */ RDSPARSER_COUNTRY_BOLIVIA,
        /* 2 */ RDSPARSER_COUNTRY_COLOMBIA,
        /* 3 */ RDSPARSER_COUNTRY_JAMAICA,
        /* 4 */ RDSPARSER_COUNTRY_MARTINIQUE,
        /* 5 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 6 */ RDSPARSER_COUNTRY_PARAGUAY,
        /* 7 */ RDSPARSER_COUNTRY_NICARAGUA,
        /* 8 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 9 */ RDSPARSER_COUNTRY_PANAMA,
        /* A */ RDSPARSER_COUNTRY_DOMINICA,
        /* B */ RDSPARSER_COUNTRY_DOMINICAN_REPUBLIC,
        /* C */ RDSPARSER_COUNTRY_CHILE,
        /* D */ RDSPARSER_COUNTRY_GRENADA,
        /* E */ RDSPARSER_COUNTRY_TURKS_AND_CAICOS_ISLANDS,
        /* F */ RDSPARSER_COUNTRY_GUYANA
    },
    {   /* A4 */
        /* 1 */ RDSPARSER_COUNTRY_GUATEMALA,
        /* 2 */ RDSPARSER_COUNTRY_HONDURAS,
        /* 3 */ RDSPARSER_COUNTRY_ARUBA,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_MONTSERRAT,
        /* 6 */ RDSPARSER_COUNTRY_TRINIDAD_AND_TOBAGO,
        /* 7 */ RDSPARSER_COUNTRY_PERU,
        /* 8 */ RDSPARSER_COUNTRY_SURINAME,
        /* 9 */ RDSPARSER_COUNTRY_URUGUAY,
        /* A */ RDSPARSER_COUNTRY_ST_KITTS,
        /* B */ RDSPARSER_COUNTRY_ST_LUCIA,
        /* C */ RDSPARSER_COUNTRY_EL_SALVADOR,
        /* D */ RDSPARSER_COUNTRY_HAITI,
        /* E */ RDSPARSER_COUNTRY_VENEZUELA,
        /* F */ RDSPARSER_COUNTRY_VIRGIN_ISLANDS
    },
    {   /* A5 */
        /* 1 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 2 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 3 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 6 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 7 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 8 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 9 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* A */ RDSPARSER_COUNTRY_UNKNOWN,
        /* B */ RDSPARSER_COUNTRY_MEXICO,
        /* C */ RDSPARSER_COUNTRY_ST_VINCENT,
        /* D */ RDSPARSER_COUNTRY_MEXICO,
        /* E */ RDSPARSER_COUNTRY_MEXICO,
        /* F */ RDSPARSER_COUNTRY_MEXICO
    },
    {   /* A6 */
        /* 1 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 2 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 3 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 6 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 7 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 8 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 9 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* A */ RDSPARSER_COUNTRY_UNKNOWN,
        /* B */ RDSPARSER_COUNTRY_UNKNOWN,
        /* C */ RDSPARSER_COUNTRY_UNKNOWN,
        /* D */ RDSPARSER_COUNTRY_UNKNOWN,
        /* E */ RDSPARSER_COUNTRY_UNKNOWN,
        /* F */ RDSPARSER_COUNTRY_ST_PIERRE_AND_MIQUELON
    }
};

static const rdsparser_country_t rdsparser_ecc_d0_d4_lut[][15] =
{
    {   /* D0 */
        /* 1 */ RDSPARSER_COUNTRY_CAMEROON,
        /* 2 */ RDSPARSER_COUNTRY_CENTRAL_AFRICAN_REPUBLIC,
        /* 3 */ RDSPARSER_COUNTRY_DJIBOUTIA,
        /* 4 */ RDSPARSER_COUNTRY_MADAGASCAR,
        /* 5 */ RDSPARSER_COUNTRY_MALI,
        /* 6 */ RDSPARSER_COUNTRY_ANGOLA,
        /* 7 */ RDSPARSER_COUNTRY_EQUATORIAL_GUINEA,
        /* 8 */ RDSPARSER_COUNTRY_GABON,
        /* 9 */ RDSPARSER_COUNTRY_GUINEA,
        /* A */ RDSPARSER_COUNTRY_SOUTH_AFRICA,
        /* B */ RDSPARSER_COUNTRY_BURKINA_FASO,
        /* C */ RDSPARSER_COUNTRY_REPUBLIC_OF_CONGO,
        /* D */ RDSPARSER_COUNTRY_TOGO,
        /* E */ RDSPARSER_COUNTRY_BENIN,
        /* F */ RDSPARSER_COUNTRY_MALAWI
    },
    {   /* D1 */
        /* 1 */ RDSPARSER_COUNTRY_NAMIBIA,
        /* 2 */ RDSPARSER_COUNTRY_LIBERIA,
        /* 3 */ RDSPARSER_COUNTRY_GHANA,
        /* 4 */ RDSPARSER_COUNTRY_MAURITANIA,
        /* 5 */ RDSPARSER_COUNTRY_SAO_TOME_AND_PRINCIPE,
        /* 6 */ RDSPARSER_COUNTRY_CAPE_VERDE,
        /* 7 */ RDSPARSER_COUNTRY_SENEGAL,
        /* 8 */ RDSPARSER_COUNTRY_GAMBIA,
        /* 9 */ RDSPARSER_COUNTRY_BURUNDI,
        /* A */ RDSPARSER_COUNTRY_ASCENSION_ISLAND,
        /* B */ RDSPARSER_COUNTRY_BOTSWANA,
        /* C */ RDSPARSER_COUNTRY_COMOROS,
        /* D */ RDSPARSER_COUNTRY_TANZANIA,
        /* E */ RDSPARSER_COUNTRY_ETHIOPIA,
        /* F */ RDSPARSER_COUNTRY_NIGERIA
    },
    {   /* D2 */
        /* 1 */ RDSPARSER_COUNTRY_SIERRA_LEONE,
        /* 2 */ RDSPARSER_COUNTRY_ZIMBABWE,
        /* 3 */ RDSPARSER_COUNTRY_MOZAMBIQUE,
        /* 4 */ RDSPARSER_COUNTRY_UGANDA,
        /* 5 */ RDSPARSER_COUNTRY_SWAZILAND,
        /* 6 */ RDSPARSER_COUNTRY_KENYA,
        /* 7 */ RDSPARSER_COUNTRY_SOMALIA,
        /* 8 */ RDSPARSER_COUNTRY_NIGER,
        /* 9 */ RDSPARSER_COUNTRY_CHAD,
        /* A */ RDSPARSER_COUNTRY_GUINEA_BISSAU,
        /* B */ RDSPARSER_COUNTRY_DR_CONGO,
        /* C */ RDSPARSER_COUNTRY_COTE_DIVOIRE,
        /* D */ RDSPARSER_COUNTRY_UNKNOWN,
        /* E */ RDSPARSER_COUNTRY_ZAMBIA,
        /* F */ RDSPARSER_COUNTRY_ERITREA
    },
    {   /* D3 */
        /* 1 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 2 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 3 */ RDSPARSER_COUNTRY_WESTERN_SAHARA,
        /* 4 */ RDSPARSER_COUNTRY_CABINDA,
        /* 5 */ RDSPARSER_COUNTRY_RWANDA,
        /* 6 */ RDSPARSER_COUNTRY_LESOTHO,
        /* 7 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 8 */ RDSPARSER_COUNTRY_SEYCHELLES,
        /* 9 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* A */ RDSPARSER_COUNTRY_MAURITIUS,
        /* B */ RDSPARSER_COUNTRY_UNKNOWN,
        /* C */ RDSPARSER_COUNTRY_SUDAN,
        /* D */ RDSPARSER_COUNTRY_UNKNOWN,
        /* E */ RDSPARSER_COUNTRY_UNKNOWN
    },
    {   /* D4 */
        /* 1 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 2 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 3 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 6 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 7 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 8 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 9 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* A */ RDSPARSER_COUNTRY_SOUTH_SUDAN,
        /* B */ RDSPARSER_COUNTRY_UNKNOWN,
        /* C */ RDSPARSER_COUNTRY_UNKNOWN,
        /* D */ RDSPARSER_COUNTRY_UNKNOWN,
        /* E */ RDSPARSER_COUNTRY_UNKNOWN
    }
};

static const rdsparser_country_t rdsparser_ecc_e0_e5_lut[][15] =
{
    {   /* E0 */
        /* 1 */ RDSPARSER_COUNTRY_GERMANY,
        /* 2 */ RDSPARSER_COUNTRY_ALGERIA,
        /* 3 */ RDSPARSER_COUNTRY_ANDORRA,
        /* 4 */ RDSPARSER_COUNTRY_ISRAEL,
        /* 5 */ RDSPARSER_COUNTRY_ITALY,
        /* 6 */ RDSPARSER_COUNTRY_BELGIUM,
        /* 7 */ RDSPARSER_COUNTRY_RUSSIA,
        /* 8 */ RDSPARSER_COUNTRY_PALESTINE,
        /* 9 */ RDSPARSER_COUNTRY_ALBANIA,
        /* A */ RDSPARSER_COUNTRY_AUSTRIA,
        /* B */ RDSPARSER_COUNTRY_HUNGARY,
        /* C */ RDSPARSER_COUNTRY_MALTA,
        /* D */ RDSPARSER_COUNTRY_GERMANY,
        /* E */ RDSPARSER_COUNTRY_UNKNOWN,
        /* F */ RDSPARSER_COUNTRY_EGYPT,
    },
    {   /* E1 */
        /* 1 */ RDSPARSER_COUNTRY_GREECE,
        /* 2 */ RDSPARSER_COUNTRY_CYPRUS,
        /* 3 */ RDSPARSER_COUNTRY_SAN_MARINO,
        /* 4 */ RDSPARSER_COUNTRY_SWITZERLAND,
        /* 5 */ RDSPARSER_COUNTRY_JORDAN,
        /* 6 */ RDSPARSER_COUNTRY_FINLAND,
        /* 7 */ RDSPARSER_COUNTRY_LUXEMBOURG,
        /* 8 */ RDSPARSER_COUNTRY_BULGARIA,
        /* 9 */ RDSPARSER_COUNTRY_DENMARK,
        /* A */ RDSPARSER_COUNTRY_GIBRALTAR,
        /* B */ RDSPARSER_COUNTRY_IRAQ,
        /* C */ RDSPARSER_COUNTRY_UNITED_KINGDOM,
        /* D */ RDSPARSER_COUNTRY_LIBYA,
        /* E */ RDSPARSER_COUNTRY_ROMANIA,
        /* F */ RDSPARSER_COUNTRY_FRANCE,
    },
    {   /* E2 */
        /* 1 */ RDSPARSER_COUNTRY_MOROCCO,
        /* 2 */ RDSPARSER_COUNTRY_CZECHIA,
        /* 3 */ RDSPARSER_COUNTRY_POLAND,
        /* 4 */ RDSPARSER_COUNTRY_VATICAN,
        /* 5 */ RDSPARSER_COUNTRY_SLOVAKIA,
        /* 6 */ RDSPARSER_COUNTRY_SYRIA,
        /* 7 */ RDSPARSER_COUNTRY_TUNISIA,
        /* 8 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 9 */ RDSPARSER_COUNTRY_LIECHTENSTEIN,
        /* A */ RDSPARSER_COUNTRY_ICELAND,
        /* B */ RDSPARSER_COUNTRY_MONACO,
        /* C */ RDSPARSER_COUNTRY_LITHUANIA,
        /* D */ RDSPARSER_COUNTRY_SERBIA,
        /* E */ RDSPARSER_COUNTRY_SPAIN,
        /* F */ RDSPARSER_COUNTRY_NORWAY,
    },
    {   /* E3 */
        /* 1 */ RDSPARSER_COUNTRY_MONTENEGRO,
        /* 2 */ RDSPARSER_COUNTRY_IRELAND,
        /* 3 */ RDSPARSER_COUNTRY_TURKEY,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_TAJIKISTAN,
        /* 6 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 7 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 8 */ RDSPARSER_COUNTRY_NETHERLANDS,
        /* 9 */ RDSPARSER_COUNTRY_LATVIA,
        /* A */ RDSPARSER_COUNTRY_LEBANON,
        /* B */ RDSPARSER_COUNTRY_AZERBAIJAN,
        /* C */ RDSPARSER_COUNTRY_CROATIA,
        /* D */ RDSPARSER_COUNTRY_KAZAKHSTAN,
        /* E */ RDSPARSER_COUNTRY_SWEDEN,
        /* F */ RDSPARSER_COUNTRY_BELARUS,
    },
    {   /* E4 */
        /* 1 */ RDSPARSER_COUNTRY_MOLDOVA,
        /* 2 */ RDSPARSER_COUNTRY_ESTONIA,
        /* 3 */ RDSPARSER_COUNTRY_MACEDONIA,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 6 */ RDSPARSER_COUNTRY_UKRAINE,
        /* 7 */ RDSPARSER_COUNTRY_KOSOVO,
        /* 8 */ RDSPARSER_COUNTRY_PORTUGAL,
        /* 9 */ RDSPARSER_COUNTRY_SLOVENIA,
        /* A */ RDSPARSER_COUNTRY_ARMENIA,
        /* B */ RDSPARSER_COUNTRY_UZBEKISTAN,
        /* C */ RDSPARSER_COUNTRY_GEORGIA,
        /* D */ RDSPARSER_COUNTRY_UNKNOWN,
        /* E */ RDSPARSER_COUNTRY_TURKMENISTAN,
        /* F */ RDSPARSER_COUNTRY_BOSNIA_HERZEGOVINA,
    },
    {   /* E5 */
        /* 1 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 2 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 3 */ RDSPARSER_COUNTRY_KYRGYZSTAN,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 6 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 7 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 8 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 9 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* A */ RDSPARSER_COUNTRY_UNKNOWN,
        /* B */ RDSPARSER_COUNTRY_UNKNOWN,
        /* C */ RDSPARSER_COUNTRY_UNKNOWN,
        /* D */ RDSPARSER_COUNTRY_UNKNOWN,
        /* E */ RDSPARSER_COUNTRY_UNKNOWN,
    }
};

static const rdsparser_country_t rdsparser_ecc_f0_f4_lut[][15] =
{
    {   /* F0 */
        /* 1 */ RDSPARSER_COUNTRY_AUSTRALIA_CAPITAL_TERRITORY,
        /* 2 */ RDSPARSER_COUNTRY_AUSTRALIA_NEW_SOUTH_WALES,
        /* 3 */ RDSPARSER_COUNTRY_AUSTRALIA_VICTORIA,
        /* 4 */ RDSPARSER_COUNTRY_AUSTRALIA_QUEENSLAND,
        /* 5 */ RDSPARSER_COUNTRY_AUSTRALIA_SOUTH_AUSTRALIA,
        /* 6 */ RDSPARSER_COUNTRY_AUSTRALIA_WESTERN_AUSTRALIA,
        /* 7 */ RDSPARSER_COUNTRY_AUSTRALIA_TASMANIA,
        /* 8 */ RDSPARSER_COUNTRY_AUSTRALIA_NORTHERN_TERRITORY,
        /* 9 */ RDSPARSER_COUNTRY_SAUDI_ARABIA,
        /* A */ RDSPARSER_COUNTRY_AFGHANISTAN,
        /* B */ RDSPARSER_COUNTRY_MYANMAR,
        /* C */ RDSPARSER_COUNTRY_CHINA,
        /* D */ RDSPARSER_COUNTRY_NORTH_KOREA,
        /* E */ RDSPARSER_COUNTRY_BAHREIN,
        /* F */ RDSPARSER_COUNTRY_MALAYSIA
    },
    {   /* F1 */
        /* 1 */ RDSPARSER_COUNTRY_KIRIBATI,
        /* 2 */ RDSPARSER_COUNTRY_BHUTAN,
        /* 3 */ RDSPARSER_COUNTRY_BANGLADESH,
        /* 4 */ RDSPARSER_COUNTRY_PAKISTAN,
        /* 5 */ RDSPARSER_COUNTRY_FIJI,
        /* 6 */ RDSPARSER_COUNTRY_OMAN,
        /* 7 */ RDSPARSER_COUNTRY_NAURU,
        /* 8 */ RDSPARSER_COUNTRY_IRAN,
        /* 9 */ RDSPARSER_COUNTRY_NEW_ZEALAND,
        /* A */ RDSPARSER_COUNTRY_SOLOMON_ISLANDS,
        /* B */ RDSPARSER_COUNTRY_BRUNEI_DARUSSALAM,
        /* C */ RDSPARSER_COUNTRY_SRI_LANKA,
        /* D */ RDSPARSER_COUNTRY_TAIWAN,
        /* E */ RDSPARSER_COUNTRY_SOUTH_KOREA,
        /* F */ RDSPARSER_COUNTRY_HONG_KONG
    },
    {   /* F2 */
        /* 1 */ RDSPARSER_COUNTRY_KUWAIT,
        /* 2 */ RDSPARSER_COUNTRY_QATAR,
        /* 3 */ RDSPARSER_COUNTRY_CAMBODIA,
        /* 4 */ RDSPARSER_COUNTRY_SAMOA,
        /* 5 */ RDSPARSER_COUNTRY_INDIA,
        /* 6 */ RDSPARSER_COUNTRY_MACAO,
        /* 7 */ RDSPARSER_COUNTRY_VIETNAM,
        /* 8 */ RDSPARSER_COUNTRY_PHILIPPINES,
        /* 9 */ RDSPARSER_COUNTRY_JAPAN,
        /* A */ RDSPARSER_COUNTRY_SINGAPORE,
        /* B */ RDSPARSER_COUNTRY_MALDIVES,
        /* C */ RDSPARSER_COUNTRY_INDONESIA,
        /* D */ RDSPARSER_COUNTRY_UNITED_ARAB_EMIRATES,
        /* E */ RDSPARSER_COUNTRY_NEPAL,
        /* F */ RDSPARSER_COUNTRY_VANUATU
    },
    {   /* F3 */
        /* 1 */ RDSPARSER_COUNTRY_LAOS,
        /* 2 */ RDSPARSER_COUNTRY_THAILAND,
        /* 3 */ RDSPARSER_COUNTRY_TONGA,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 6 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 7 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 8 */ RDSPARSER_COUNTRY_CHINA,
        /* 9 */ RDSPARSER_COUNTRY_PAPUA_NEW_GUINEA,
        /* A */ RDSPARSER_COUNTRY_UNKNOWN,
        /* B */ RDSPARSER_COUNTRY_YEMEN,
        /* C */ RDSPARSER_COUNTRY_UNKNOWN,
        /* D */ RDSPARSER_COUNTRY_UNKNOWN,
        /* E */ RDSPARSER_COUNTRY_MICRONESIA,
        /* F */ RDSPARSER_COUNTRY_MONGOLIA
    },
    {   /* F4 */
        /* 1 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 2 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 3 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 4 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 5 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 6 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 7 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 8 */ RDSPARSER_COUNTRY_UNKNOWN,
        /* 9 */ RDSPARSER_COUNTRY_CHINA,
        /* A */ RDSPARSER_COUNTRY_UNKNOWN,
        /* B */ RDSPARSER_COUNTRY_MARSHALL_ISLANDS,
        /* C */ RDSPARSER_COUNTRY_UNKNOWN,
        /* D */ RDSPARSER_COUNTRY_UNKNOWN,
        /* E */ RDSPARSER_COUNTRY_UNKNOWN,

    }
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

        if (ecc >= 0xA0 &&
            ecc <= 0xA6)
        {
            const uint8_t ecc_id = ecc - 0xA0;
            return rdsparser_ecc_a0_a6_lut[ecc_id][pi_id];
        }

        if (ecc >= 0xD0 &&
            ecc <= 0xD4)
        {
            const uint8_t ecc_id = ecc - 0xD0;
            return rdsparser_ecc_d0_d4_lut[ecc_id][pi_id];
        }

        if (ecc >= 0xE0 &&
            ecc <= 0xE5)
        {
            const uint8_t ecc_id = ecc - 0xE0;
            return rdsparser_ecc_e0_e5_lut[ecc_id][pi_id];
        }

        if (ecc >= 0xF0 &&
            ecc <= 0xF4)
        {
            const uint8_t ecc_id = ecc - 0xF0;
            return rdsparser_ecc_f0_f4_lut[ecc_id][pi_id];
        }
    }

    return RDSPARSER_COUNTRY_UNKNOWN;
}
