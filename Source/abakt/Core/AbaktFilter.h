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
#ifndef AbaktFilterH
#define AbaktFilterH
//---------------------------------------------------------------------------
#include "AbaktClasses.h"
#include "AbaktFilterSet.h"
#include "FileProperties.h"
//---------------------------------------------------------------------------

class TAbaktFilter {

public:

    int testFile(const String fPath);
    /* TODO 1 : testFile() should return an integer: 0, 1 , -1 ? */
	/*	This is THE function that tests if a file should be included or not.
		[fpath] is a complete path to a file (drive + directory + filename + extension).
		Requires the Profile's excludeDirList to be up-to-date.
        See _doTestFile() for a description of the return values. */

	bool getFileTested() const
	{	return _fileTested; }
    /*  Returns 'true' if a file has been tested,
        if the return value is 'false', the filter-engine is busy testing. */

 	bool getFilePassed() const
	{	return _filePassed; } 
    /*  Returns 'true' if a file passed the filter (included). */

	bool getFileNotCompressed() const
	{	return _fileNoCompress; }

	bool getDirNotSelected() const
	{	return _dirNotSelected; }
	/*	Returns true if the directory that contains _fileName
		is in Profile.ExcludeDirList. */

	String getInfoStr() const
	{	return _infoStr; }
	/*	Returns the (first) reason why a file is included or excluded,
        or a message describing an error if testFile() returns -1,
		or an empty string if the file is not influenced by any filter. */

    int getFilterSetIndex() const
    {   return _filterSetIndex; }
    /*  See description of _filterSetIndex. */

	String getFileName() const
	{	return _fileName; }

	TFileProperties getFileProps() const
	{	return _fileProps; }
    /*  The TFileProperties object that was used to test this file. */

    String validateAllConditions(TAbaktFilterSet& fs);
    /*  Returns an empty string if no syntax errors are found.
        Returns a string containing details on the syntax errors for every condition. */ 

private:

	String _fileName;
    /*  The file to test. */

	bool _fileTested;
	/* 	Is false if a file is still being tested. */

	bool _filePassed;
	/* 	Is false if a file has not passed the filter,
		or if the file is still being tested. */

	String _infoStr;
	/*	A string that describes why the file has (not) passed the filter.
		Empty if no filter describes why the file was *included*. */

	TFileProperties _fileProps;

	bool _fileNoCompress;

	bool _dirNotSelected;

    int _filterSetIndex;
    /*  The index of the first FilterSet that matches the file's properties.
        Is -1 if no FilterSet matches, when not initialized or when an error occurred. */

	int _doTestFile(const String fPath);
	/*	Does the actual test.
        Returns 1 if [fPath] is included,
        returns 0 if [fPath] is excluded,
        returns -1 when an error occurred (call getInfoStr() for more information). */

    int _doTestFile_filterSetList(
        const String fPath, TFileProperties& fProps, const TAbaktFilterSetAction fsAction);
    /*  Returns 1 if [fPath/fProps] matches an item (with action == fsAction) in PProfile's filterSetList,
        returns 0 if [fPath/fProps] doesn't match any item,
        returns -1 when a syntax error occurred (while parsing a FilterSet's conditions). */

	String _extractDirsParentPath(String dir);
	/*  Returns the full path of [dir]'s parent.
		Always including the drive-part ("C:\").
		[dir] is assumed to be a directory including the drive part (and no file-part!)
		The last character of [dir] is assumed to be a '\'. */

	bool _dirIsSelected(String dir);
	/* 	Returns true if [dir] is a directory that wll be included in the backup.
		[dir] is assumed to be a directory including the drive part (and no file-part!)
		The last character of [dir] is assumed to be a '\'. */

    int _matchConditionPath(String mask, String fPath);     // May contain several path masks (entries) separated by a ';'.
    int _matchConditionPathEntry(String mask, String fPath);
    int _matchConditionInt(String condition, const __int64 value);    // Used for Size and Age.
    int _matchConditionDate(String condition, const TDateTime fDate);
    int _matchConditionAttributes(String condition, const String fAttrs);
    /*  (Implemented in 0.9.2-beta3.)
        The _matchCondition...() functions:
        Return 1 if any of the expressions in [condition] is true.
        Return 0 if none of the expressions in [condition] are true.
        Return -1 if there is a syntax error in [condition], call getInfoStr() for more specific information. */

    bool _splitExpression(String expression, String &expOpStr, String &expValStr);
};
//---------------------------------------------------------------------------
#endif
