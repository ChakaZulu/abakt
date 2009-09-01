/* ==========================================================================

	Abakt - A Backup Tool
	Copyright (C) 2003-2007 - Erik Dienske

	This file is part of Abakt.

	Abakt is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Abakt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Abakt; if not, write to the Free Software Foundation, Inc.,
	59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

===========================================================================*/

//---------------------------------------------------------------------------
#include <vcl.h>
#include "TRANSLATE.h"
#pragma hdrstop

#include "GlobalFuncs.h"
#include "AbaktProfile.h"
#include "AbaktClasses.h"
#include "eccFile.h"
#include "eccString.h"
#include "eccError.h"
#include "AbaktGlobals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

AnsiString ASPF(AnsiString format_str, ...)
{
    va_list argptr;
    va_start(argptr, format_str);   // Set argptr to the first of the variable arguments being passed.
    String msg;
    msg.vprintf(format_str.c_str(), argptr);
    va_end(argptr);                 // Required.
    return msg;
}
//---------------------------------------------------------------------------

String abtFormatText(String str)
{
	if (str.IsEmpty()) return str;

    str = ecc::ReplaceStr(str, "$N", PProfile->name);
    str = ecc::ReplaceStr(str, "$M",
        _(ArchiveModeToStr((TArchiveMode)PProfile->config.Action->eArchiveMode->getIndex())));
    str = ecc::ReplaceStr(str, "$V", PGlobals->versionBuildStr());
    str = ecc::ReplaceStr(str, "$$", "$");
	str = FormatDateTime(str, Now());
	return ecc::ReplaceIllegalFileNameChars(str);
}
//---------------------------------------------------------------------------

TDateTime abtStrToDate(String dateStr)
{
	unsigned short y, m, d;
	y = StrToInt(dateStr.SubString(1, 4));	// yyyy
	m = StrToInt(dateStr.SubString(6, 2));	// mm
	d = StrToInt(dateStr.SubString(9, 2));	// dd

	return EncodeDate(y, m, d);
}
//---------------------------------------------------------------------------

String abtDateToStr(TDateTime dt)
{
	return dt.FormatString("yyyy'-'mm'-'dd");
}
//---------------------------------------------------------------------------

TDateTime abtStrToTime(String time_str)
{
	unsigned short h, m;
	h = StrToInt(time_str.SubString(1, 2));
	m = StrToInt(time_str.SubString(4, 2));
	return EncodeTime(h, m, 0, 0);;
}
//---------------------------------------------------------------------------

String abtTimeToStr(TDateTime dt)
{
	return dt.FormatString("hh':'nn");
}
//---------------------------------------------------------------------------

time_t DateTime2time_t(const TDateTime dt)
{
    unsigned short dtYear, dtMonth, dtDay;
    dt.DecodeDate(&dtYear, &dtMonth, &dtDay);

    tm TM;
	TM.tm_year = (int)dtYear - 1900;
	TM.tm_mon  = (int)dtMonth - 1;
	TM.tm_mday = (int)dtDay;
    TM.tm_hour = 0;
    TM.tm_min  = 0;
    TM.tm_sec  = 1;
    TM.tm_isdst = -1;

    // Call mktime to fill in the weekday field of the structure:
    if (mktime(&TM) == -1)
        TM.tm_wday = 7;

	return mktime(&TM);
}
//---------------------------------------------------------------------------

TDateTime time_t2DateTime(const time_t timer)
{
    tm *TM = localtime(&timer);

	unsigned short dtYear  = TM->tm_year + 1900;
	unsigned short dtMonth = TM->tm_mon + 1;
	unsigned short dtDay   = TM->tm_mday;

    TDateTime dt;
    try
    {
        dt = EncodeDate(dtYear, dtMonth, dtDay);
    }
    catch (...)
    {
        dt = Now();
    }
	return dt;
}
//---------------------------------------------------------------------------

String abtGetLastErrorStr()
{
    return abtGetLastErrorStr(GetLastError());
}
//---------------------------------------------------------------------------

String abtGetLastErrorStr(DWORD lastErrorCode)
{
	String err = ecc::LastErrorCodeToStr(lastErrorCode);
	err = ecc::ReplaceStr(err, "\n", " ");
	err = ecc::ReplaceStr(err, "\r", " ");
	return err;
}
//---------------------------------------------------------------------------

String TranslateHMS(String str)
{
    str = ecc::ReplaceStr(str, "Hours", _("Hours"));
    str = ecc::ReplaceStr(str, "Minutes", _("Minutes"));
    str = ecc::ReplaceStr(str, "Seconds", _("Seconds"));
    return str;
}
//---------------------------------------------------------------------------

__int64 FileSizeStrToIntDef(const String str, const __int64 def)
{
    if (str.IsEmpty()) return def;

    __int64 multiplier = 0;
    switch (str[str.Length()])
    {
        case 'k': case 'K':
            multiplier = 1024;
        break;

        case 'm': case 'M':
            multiplier = 1024 * 1024;
        break;

        case 'g': case 'G':
            multiplier = 1024 * 1024 * 1024;
        break;
    }

    //092RC2: Replaced StrToIntDef() with StrToInt64Def() below:
    __int64 val;
    if (multiplier == 0)
        val = StrToInt64Def(str.SubString(1, str.Length() ), def);
    else
    {
        val = StrToInt64Def(str.SubString(1, str.Length() - 1), def);
        if (val != def)
            val = val * multiplier;
    }

    return val;
}
//---------------------------------------------------------------------------

String abtAttrToStr(const unsigned int attr)
{
	String str;
	if (attr & faArchive)	str += "A";
	if (attr & faHidden)	str += "H";
	if (attr & faReadOnly)	str += "R";
	if (attr & faSysFile)	str += "S";
	if (attr & faVolumeID)	str += "V";
	if (attr & faDirectory)	str += "D";
	return str;
}
//---------------------------------------------------------------------------

void abtAppendStrToFile(String str, String fpath, bool datetime, bool endline)
{
	if (fpath.IsEmpty()) return;

	std::ofstream log(fpath.c_str(), std::ios::out|std::ios::app); // Append, and create if necessary.
	if (!log) return;

	if (datetime)
		log	<< "[" << Now().DateTimeString().c_str() << "] ";
	log	<< str.c_str();
	if (endline)
		log << std::endl;
}
//---------------------------------------------------------------------------

