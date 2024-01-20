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
#include <librdsparser.h>

static const char *rdsparser_country_name_lut[] =
{
    "Germany",
    "Algeria",
    "Andorra",
    "Israel",
    "Italy",
    "Belgium",
    "Russia",
    "Palestine",
    "Albania",
    "Austria",
    "Hungary",
    "Malta",
    "Egypt",
    "Greece",
    "Cyprus",
    "San Marino",
    "Switzerland",
    "Jordan",
    "Finland",
    "Luxembourg",
    "Bulgaria",
    "Denmark",
    "Gibraltar",
    "Iraq",
    "United Kingdom",
    "Libya",
    "Romania",
    "France",
    "Morocco",
    "Czechia",
    "Poland",
    "Vatican",
    "Slovakia",
    "Syria",
    "Tunisia",
    "Lichtenstein",
    "Iceland",
    "Monaco",
    "Lithuania",
    "Serbia",
    "Spain",
    "Norway",
    "Montenegro",
    "Ireland",
    "Turkey",
    "Tajikistan",
    "Netherlands",
    "Latvia",
    "Lebanon",
    "Azerbaijan",
    "Croatia",
    "Kazakhstan",
    "Sweden",
    "Belarus",
    "Moldova",
    "Estonia",
    "Macedonia",
    "Ukraine",
    "Kosovo",
    "Portugal",
    "Slovenia",
    "Armenia",
    "Uzbekistan",
    "Georgia",
    "Turkmenistan",
    "Bosnia",
    "Kyrgyzstan"
};

static const char *rdsparser_country_iso_lut[] =
{
    "DE",
    "DZ",
    "AD",
    "IL",
    "IT",
    "BE",
    "RU",
    "PS",
    "AL",
    "AT",
    "HU",
    "MT",
    "EG",
    "GR",
    "CY",
    "SM",
    "CH",
    "JO",
    "FI",
    "LU",
    "BG",
    "DK",
    "GI",
    "IQ",
    "GB",
    "LY",
    "RO",
    "FR",
    "MA",
    "CZ",
    "PL",
    "VA",
    "SK",
    "SY",
    "TN",
    "LI",
    "IS",
    "MC",
    "LT",
    "RS",
    "ES",
    "NO",
    "ME",
    "IE",
    "TR",
    "TJ",
    "NL",
    "LV",
    "LB",
    "AZ",
    "HR",
    "KZ",
    "SE",
    "BY",
    "MD",
    "EE",
    "MK",
    "UA",
    "XK",
    "PT",
    "SI",
    "AM",
    "UZ",
    "GE",
    "TM",
    "BA",
    "KG"
};

const char*
rdsparser_country_lookup(rdsparser_country_t   country,
                         const char          **lut,
                         const char           *unknown)
{
    if (country < 0 ||
        country >= RDSPARSER_COUNTRY_COUNT)
    {
        return unknown;
    }

    return lut[country];
}

const char*
rdsparser_country_lookup_iso(rdsparser_country_t country)
{
    static const char *unknown = "??";
    return rdsparser_country_lookup(country, rdsparser_country_iso_lut, unknown);
}

const char*
rdsparser_country_lookup_name(rdsparser_country_t country)
{
    static const char *unknown = "Unknown";
    return rdsparser_country_lookup(country, rdsparser_country_name_lut, unknown);
}
