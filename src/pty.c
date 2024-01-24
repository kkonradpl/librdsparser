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

static const char *rdsparser_pty_rds_long[] =
{
    "None",
    "News",
    "Current Affairs",
    "Information",
    "Sport",
    "Education",
    "Drama",
    "Cultures",
    "Science",
    "Varied Speech",
    "Pop Music",
    "Rock Music",
    "Easy Listening",
    "Light Classics M",
    "Serious Classics",
    "Other Music",
    "Weather & Metr",
    "Finance",
    "Children's Progs",
    "Social Affairs",
    "Religion",
    "Phone In",
    "Travel & Touring",
    "Leisure & Hobby",
    "Jazz Music",
    "Country Music",
    "National Music",
    "Oldies Music",
    "Folk Music",
    "Documentary",
    "Alarm Test",
    "Alarm - Alarm !"
};

static const char *rdsparser_pty_rbds_long[] =
{
    "None",
    "News",
    "Information",
    "Sport",
    "Talk",
    "Rock",
    "Classic Rock",
    "Adult Hits",
    "Soft Rock",
    "Top 40",
    "Country",
    "Oldies",
    "Soft",
    "Nostalgia",
    "Jazz",
    "Classical",
    "Rhythm and Blues",
    "Soft R & B",
    "Foreign Language",
    "Religious Music",
    "Religious Talk",
    "Personality",
    "Public",
    "College",
    "Hablar Espanol",
    "Musica Espanol",
    "Hip hop",
    "Not assigned",
    "Not assigned",
    "Weather",
    "Emergency Test",
    "ALERT! ALERT!"
};

static const char *rdsparser_pty_rds_short[] =
{
    "None",
    "News",
    "Affairs",
    "Info",
    "Sport",
    "Educate",
    "Drama",
    "Culture",
    "Science",
    "Varied",
    "Pop M",
    "Rock M",
    "Easy M",
    "Light M",
    "Classics",
    "Other M",
    "Weather",
    "Finance",
    "Children",
    "Social",
    "Religion",
    "Phone In",
    "Travel",
    "Leisure",
    "Jazz",
    "Country",
    "Nation M",
    "Oldies",
    "Folk M",
    "Document",
    "TEST",
    "Alarm !"
};

static const char *rdsparser_pty_rbds_short[] =
{
    "None",
    "News",
    "Inform",
    "Sports",
    "Talk",
    "Rock",
    "Cls Rock",
    "Adlt Hit",
    "Soft Rck",
    "Top 40",
    "Country",
    "Oldies",
    "Soft",
    "Nostalga",
    "Jazz",
    "Classicl",
    "R & B",
    "Soft R&B",
    "Language",
    "Rel Musc",
    "Rel Talk",
    "Persnlty",
    "Public",
    "College",
    "Habl Esp",
    "Musc Esp",
    "Hip hop",
    "N/A",
    "N/A",
    "Weather",
    "Test",
    "ALERT !"
};


const char*
rdsparser_pty_lookup(rdsparser_pty_t   pty,
                     const char      **lut)
{
    if (pty >= 0 &&
        pty < 32)
    {
        return lut[pty];
    }

    static const char unknown[] = "Unknown";
    return unknown;
}

const char*
rdsparser_pty_lookup_short(rdsparser_pty_t pty,
                           bool            rbds)
{
    const char **lut = rbds ? rdsparser_pty_rbds_short : rdsparser_pty_rds_short;
    return rdsparser_pty_lookup(pty, lut);
}

const char*
rdsparser_pty_lookup_long(rdsparser_pty_t pty,
                          bool            rbds)
{
    const char **lut = rbds ? rdsparser_pty_rbds_long : rdsparser_pty_rds_long;
    return rdsparser_pty_lookup(pty, lut);
}
