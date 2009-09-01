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
#pragma hdrstop

#include "FileProperties.h"
#include "DateUtils.hpp"
#include "eccFile.h"
#include "GlobalFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

#define _T(txt) txt

//---------------------------------------------------------------------------

TFileProperties::TFileProperties()
{
/*     __testPFINDFIRSTFILEW(); */

	_initialized    = false;
}
//---------------------------------------------------------------------------

TFileProperties::TFileProperties(String fname)
{
/*     __testPFINDFIRSTFILEW(); */

	retrieve(fname);
}
//---------------------------------------------------------------------------

/*	== 092b9: Detect if FindFirstFileW() exists in kernel32.dll:
    FindFirstFileW can be used with paths longer than MAX_PATH ("C:\<256 chars>").

    If _pFindFirstFileW is NULL, the function FindFirstFileW() does not exist,
    and FindFirstFile() should be called instead.

    More information:
    - http://msdn.microsoft.com/library/default.asp?url=/library/en-us/fileio/fs/findfirstfile.asp
    - http://msdn.microsoft.com/library/default.asp?url=/library/en-us/fileio/fs/naming_a_file.asp
*/

/* void TFileProperties::__testPFINDFIRSTFILEW()
{
    try
    {
        _pFindFirstFileW = (PFINDFIRSTFILEW) GetProcAddress(
            GetModuleHandle("kernel32.dll"), "FindFirstFileW" );
    }
    catch (...)
    {
        _pFindFirstFileW = NULL;
    }
} */
//---------------------------------------------------------------------------

bool TFileProperties::retrieve(const String fileName)
{
	_error = false;
    _errorStr = "";
	_initialized = false;

	// Get file handle:
    WIN32_FIND_DATA fi; // (Ab)Used as both WIN32_FIND_DATAA and WIN32_FIND_DATAW (equally defined) ...
	ZeroMemory(&fi, sizeof(fi));

	HANDLE fHandle = INVALID_HANDLE_VALUE; // Set to invalid at first.

/* TODO 2 : -- 092b9: _pFindFirstFileW - OK? */
/* TODO 2 : -- 092b10: Removed _pFindFirstFileW code - OK? */

    /*  Normal:             "\\?\D:\<path>".
        <server>\<share>:   "\\?\UNC\<server>\<share>". */

    // -- Try if FindFirstFile works with "\\?\" or "\\?\UNC\":
/*     if (_pFindFirstFileW)   // 092b9.
	{
        String UNCfName = fileName;
        if (UNCfName.Pos("\\\\") == 1)  // If fileName is already a path like \\server\share:
        {
            UNCfName.Delete(1, 2);                  // Strip "\\" from path.
            UNCfName = "\\\\?\\UNC\\" + UNCfName;   // Prepend path with "\\?\UNC\".
        }
        else
            UNCfName = "\\\\?\\" + fileName;    // Prepend path with "\\?\" for "X:\" paths (normal or mapped network drives).

        // Let's try if this works:
		fHandle = _pFindFirstFileW(UNCfName.c_str(), (PWIN32_FIND_DATAW)&fi);
	} */

    // -- If above (_pFindFirstFileW test) fails: fall back to normal FindFirstFile usage:
    if (fHandle == INVALID_HANDLE_VALUE)
	{
        // Use FindFirstFile without "\\?\" or "\\?\UNC\":
		fHandle = FindFirstFile(fileName.c_str(), &fi);
	}

    // -- Finally:
	if (fHandle == INVALID_HANDLE_VALUE)
	{
		_error = true;
        _errorStr = abtGetLastErrorStr();
		_initialized = false;
		return false;
	}

    // Fetch file properties:
    _fetchAttributes(fi);
    _fetchSize(fi);
    _fetchDateTime(fi);

	FindClose(fHandle);

	_initialized = true;
	return true;
}
//---------------------------------------------------------------------------

void TFileProperties::_fetchAttributes(const WIN32_FIND_DATA &fi)
{
	_attrs.Archive 	    = fi.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
	_attrs.Compressed	= fi.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED;
	_attrs.Hidden 	    = fi.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
	_attrs.ReadOnly	    = fi.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
	_attrs.System 	    = fi.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM;
	_attrs.Temporary 	= fi.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY;
	_attrs.Directory	= fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}
//---------------------------------------------------------------------------

void TFileProperties::_fetchSize(const WIN32_FIND_DATA &fi)
{
    //  The size of the file is equal to (nFileSizeHigh * MAXDWORD) + nFileSizeLow
    //  See Microsoft help on WIN32_FIND_DATA.

	// Helper union:
	union
	{
		struct
		{
			DWORD lo; // DWORD is assumed to be __int32
			DWORD hi;
		};
		__int64 size;
	} u_size;

	u_size.hi = fi.nFileSizeHigh;
	u_size.lo = fi.nFileSizeLow;

	_size = u_size.size;
}
//---------------------------------------------------------------------------

void TFileProperties::_fetchDateTime(const WIN32_FIND_DATA &fi)
{
    // Modification date:
	_lastWriteTime	= ecc::FILETIMEToTDateTime(fi.ftLastWriteTime);

    // Calculate age:
	double now = Now() + .5;
    double age = now - (double)_lastWriteTime;
    _age = (int)age;
}
//---------------------------------------------------------------------------

bool TFileProperties::getAttrState(const char& attr)
{
    if (!_initialized) return false; //throw std::runtime_error(ABT_ERR_LOC "Not initialized.");

    switch (attr)
    {
        case _T('A'):
            return _attrs.Archive;
        case _T('C'):
            return _attrs.Compressed;
        case _T('H'):
            return _attrs.Hidden;
        case _T('R'):
            return _attrs.ReadOnly;
        case _T('S'):
            return _attrs.System;
        case _T('T'):
            return _attrs.Temporary;
        case _T('D'):
            return _attrs.Directory;

        default:
            //throw std::runtime_error(ABT_ERR_LOC "Attribute not recognized.");
            return false;
    }
}
//---------------------------------------------------------------------------

String TFileProperties::getAttrsStr()
{
    if (!_initialized) return "";

	String str;
	str += _attrs.Archive		? _T("A") : _T("");
    // Not used in Abakt:	str += _attrs.Compressed	? _T("C") : _T("");
	str += _attrs.Hidden		? _T("H") : _T("");
	str += _attrs.ReadOnly  	? _T("R") : _T("");
	str += _attrs.System	 	? _T("S") : _T("");
    // Not used in Abakt:	str += _attrs.Temporary 	? _T("T") : _T("");
	return str;
}
//---------------------------------------------------------------------------

__int64 TFileProperties::getSize()
{
    if (!_initialized) return -1;

    return _size;
}
//---------------------------------------------------------------------------

TDateTime TFileProperties::getDate()
{
    if (!_initialized) return NULL;

    //	Set Time part to 0, to prevent failure when comparing (only the date is requested for):
	TDateTime fileDateModified = RecodeTime(_lastWriteTime, 0, 0, 0, 0);
    return fileDateModified;
}
//---------------------------------------------------------------------------

TDateTime TFileProperties::getDateTime()
{
    if (!_initialized) return NULL;

    return _lastWriteTime;
}
//---------------------------------------------------------------------------

int TFileProperties::getAge()
{
    if (!_initialized) return -1;

    return _age;
}
//---------------------------------------------------------------------------
