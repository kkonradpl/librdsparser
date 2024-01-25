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

static const char *rdsparser_pty_rds_name[] =
{
    "No programme type or undefined",
    "News",
    "Current affairs",
    "Information",
    "Sport",
    "Education",
    "Drama",
    "Culture",
    "Science",
    "Varied",
    "Pop music",
    "Rock music",
    "Easy listening music",
    "Light classical",
    "Serious classical",
    "Other music",
    "Weather",
    "Finance",
    "Children's programmes",
    "Social affairs",
    "Religion",
    "Phone in",
    "Travel",
    "Leisure",
    "Jazz music",
    "Country music",
    "National music",
    "Oldies music",
    "Folk music",
    "Documentary",
    "Alarm test",
    "Alarm"
};

static const char *rdsparser_pty_rds_short[] =
{
    "",
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
    "Phone in",
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

static const char *rdsparser_pty_rds_long[] =
{
    "",
    "News",
    "Current affairs",
    "Information",
    "Sport",
    "Education",
    "Drama",
    "Cultures",
    "Science",
    "Varied speech",
    "Pop music",
    "Rock music",
    "Easy listening",
    "Light classics m",
    "Serious classics",
    "Other music",
    "Weather & metr",
    "Finance",
    "Children's progs",
    "Social affairs",
    "Religion",
    "Phone in",
    "Travel & touring",
    "Leisure & hobby",
    "Jazz music",
    "Country music",
    "National music",
    "Oldies music",
    "Folk music",
    "Documentary",
    "Alarm test",
    "Alarm - Alarm !"
};

static const char *rdsparser_pty_rbds_name[] =
{
    "No program type or undefined",
    "News",
    "Information",
    "Sports",
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
    "Soft Rhythm and Blues",
    "Foreign Language",
    "Religious Music",
    "Religious Talk",
    "Personality",
    "Public",
    "College",
    "Spanish Talk",
    "Spanish Music",
    "Hip-Hop",
    "Unassigned",
    "Unassigned",
    "Weather",
    "Emergency Test",
    "Emergency"
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
    "",
    "",
    "Weather",
    "Test",
    "ALERT !"
};

static const char *rdsparser_pty_rbds_long[] =
{
    "None",
    "News",
    "Information",
    "Sports",
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
    "",
    "",
    "Weather",
    "Emergency Test",
    "ALERT! ALERT!"
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
rdsparser_pty_lookup_name(rdsparser_pty_t pty,
                          bool            rbds)
{
    const char **lut = rbds ? rdsparser_pty_rbds_name : rdsparser_pty_rds_name;
    return rdsparser_pty_lookup(pty, lut);
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
