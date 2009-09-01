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
#ifndef GlobalFuncsH
#define GlobalFuncsH
//---------------------------------------------------------------------------
#include "time.h"
//---------------------------------------------------------------------------

AnsiString ASPF(AnsiString format, ...);
/*  Simulates AnsiString.sprintf().
    Example:
    {
        __int64 size = 213214046;
        _log( ASPF(_("Compressed Size: %s (%s Bytes) in %d file(s)"),
            ecc::FloatToHumanFileSize(size), IntToStr(size), 14) );
    }
    Note: sprintf can not handle types long or __int64. And a float is not
    precise enough. That's why IntToStr() is used.

    ***
    Using ASPF("string: %s) in combination with _():
    -   You should convert the value used for %s to String().
        Example:    ASPF("translated string: %s", String(_("some text")) );
        Note the call of String(). Required because _() returns a WideString.
*/

String abtFormatText(String str);
/*  Replaces "$N" with current Profile's Name, and "$$" with "$",
    and calls FormatDateTime() (see Borland Help). */

TDateTime abtStrToDate(String dateStr);
/* 	[dateStr] is assumed to be formatted as "yyyy-mm-dd".
    No checks are made! */

String abtDateToStr(TDateTime dt);

TDateTime abtStrToTime(String time_str);
/* 	[time_str] is assumed to be (2+1+2) == "HH:mm". */

String abtTimeToStr(TDateTime dt);

// 247+, DateTime info is stored in time_t. Conversion functions:
time_t DateTime2time_t(const TDateTime dt);
TDateTime time_t2DateTime(const time_t timer);

String abtGetLastErrorStr();
String abtGetLastErrorStr(DWORD lastErrorCode);
/*  Both functions above return a string (without newline characters) describing
    the value [lastErrorCode] returned by Windows function GetLastError(). */

String TranslateHMS(String str);
/*  Returns [str] with the words "Hours", "Minutes" and "Seconds"
    translated into the currently used language in the application. */

__int64 FileSizeStrToIntDef(const String str, const __int64 def);
/*  Converts a string to an __int64.
    Accepts one of these trailing characters: 'K', 'M' or 'G' (case-insensitive),
    which will multiply the value in [str] with 1024^X. */

String abtAttrToStr(const unsigned int attr);
/*  Converts (int) TZFArchiveItem.ExternalFileAttributes to an attributes string. */

void abtAppendStrToFile(const String str, const String fpath,
    	bool datetime, bool endline = true);
/*  Appends [str] to file [fpath].
	If [datetime], [str] is prefixed with a date/time string like "[14-04-2004 10:19:33]".
	If [endline], [str] is followed by std::endl.
	Useful for adding strings to a log file. */

//---------------------------------------------------------------------------
#endif

