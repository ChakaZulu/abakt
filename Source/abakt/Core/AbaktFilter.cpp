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

#include "AbaktFilter.h"
#include "AbaktProfile.h"
#include "GlobalFuncs.h"
#include <Masks.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/*  Notes:
    #include <Masks.hpp>		required by: MatchesMask()
*/
//---------------------------------------------------------------------------

int TAbaktFilter::testFile(const String fPath)
{
	// Busy:
	_fileTested = false;

	// Test if file passes the filters:
    _filePassed = _doTestFile(fPath);

	// Finished:
	_fileTested = true;
	return _filePassed;
}
//---------------------------------------------------------------------------

int TAbaktFilter::_doTestFile(const String fPath)
{
	_fileName		= fPath;
	_fileNoCompress	= false;
	_dirNotSelected	= false;
	_infoStr   		= "";
    _filterSetIndex = -1;

    // Always (!) retrieve the file's properties, even if dir is not selected.
    // This information is used in the Source panel to display file information.
    _fileProps.retrieve(_fileName);

	// == Error checks:
	if (_fileProps.failed())
	{
		_infoStr = _("Error reading file properties.") + " " + _fileProps.getErrorStr();
		return -1;  // error
	}
    #ifdef ABT_TEST
        if (!_fileProps.isInitialized())
        {
            _infoStr = "ERIK: Error! TFileProperties not initialized.";
            return -1; // error
        };
    #endif

	if (!_dirIsSelected(ExtractFilePath(_fileName)))
	{
		_infoStr = _("Directory Not Selected");
		_dirNotSelected = true;
		return 0;   // excluded
	}

    // == Deal with ArchiveMode:
    TArchiveMode am = (TArchiveMode)PProfile->config.Action->eArchiveMode->getIndex();
    switch (am)
    {
	    case amDifferential:
            if (!_fileProps.getAttrState('A'))
            {
                _infoStr = ArchiveModeToStr(am) + ": " + _("Use A-files only.");
                return 0;
            }
            break;

	    case amIncremental:
            if (!_fileProps.getAttrState('A'))
            {
        		_infoStr = ArchiveModeToStr(am) + ": " + _("Use A-files only.");
                return 0;
            }
            break;

    	case amInverse:
            if (_fileProps.getAttrState('A'))
            {
        		_infoStr = ArchiveModeToStr(am) + ": " + _("Use non A-files only.");
                return 0;
            }
            break;
    }


//  === NEW FILTER CODE (SINCE 0.9.2-BETA3 [FILTERSET]) ===
/*  Tests if file matches an include filterset (Always Include).
    If it doesn't, tests if it should be excluded. */

    int result;

    result = _doTestFile_filterSetList(_fileName, _fileProps, fsaInclude);
    if (result == -1) return -1;    // error
    if (result == 1)
    {
        // 092RC2: File is included, maybe it should be NotCompressed too?
        result = _doTestFile_filterSetList(_fileName, _fileProps, fsaNoCompression);
        if (result == -1) return -1;    // error
        else if (result == 1) _fileNoCompress = true;

        // Anyway, the file is included:
        return 1;      // included (always)
    }

    result = _doTestFile_filterSetList(_fileName, _fileProps, fsaExclude);
    if (result == -1) return -1;    // error
    if (result == 1) return 0;      // excluded

    // 092RC2: File is not excluded, maybe it should be NotCompressed?
    result = _doTestFile_filterSetList(_fileName, _fileProps, fsaNoCompression);
    if (result == -1) return -1;    // error
    else if (result == 1) _fileNoCompress = true;

    return 1;   // included (file does not match any of the exclude filterSets).
}
//---------------------------------------------------------------------------

bool TAbaktFilter::_dirIsSelected(String dir)
{
/*	Abakt assumes:
    For each directory that isn't new, exactly one of the four conditions must be true.
    For a new dir, one of the first two could still be true. */

	if (PProfile->dirIsPartOfExcludeTree(dir))
		return false;
	if (PProfile->dirIsPartOfIncludeTree(dir))
		return true;
	if (PProfile->dirInExcludeDirList(dir))
		return false;
	if (PProfile->dirInIncludeDirList(dir))
		return true;

/*	New directory:
    Recurse to find nearest parent that isn't new.
    All parents of a dir can't be new, so it should terminate, but... */
/*  093b1: ...unfortunately it does not terminate if a Profile is opened with missing directories.
    Below is a fix that prevents a neverending recursive loop when dir is something like "C:\".
    (Reported by JB, april 2006.) */

    int oldLen = dir.Length();
    dir = _extractDirsParentPath(dir);
    int newLen = dir.Length();
    if (oldLen == newLen) return false;

	return _dirIsSelected(dir);
}
//---------------------------------------------------------------------------

String TAbaktFilter::_extractDirsParentPath(String dir)
{
	int p = dir.Length();

    // Determine length of drive portion, i.e. "C:\" or "\\<servername>\<sharename>\".
    int driveLen = String(ExtractFileDrive(dir)).Length() + 1; // '+ 1' is used for the backslash.

	while (p > driveLen)
	{
		if (dir[--p] == '\\')
			break;
	}
	return dir.SetLength(p);
}
//---------------------------------------------------------------------------

int TAbaktFilter::_doTestFile_filterSetList(
        const String fPath, TFileProperties& fProps, const TAbaktFilterSetAction fsAction)
{
    // Shortcut for readability:
    std::vector<TAbaktFilterSet>& fsList = PProfile->filterSetList;

    int result;
    for (unsigned int i = 0; i < fsList.size(); i++)
    {
        if (fsList[i].enabled == false)     // Only test enabled FilterSets.
            continue;

        if (fsList[i].action != fsAction)   // Only test FilterSets with action of type fsAction.
            continue;

        result = 0;
        //  When result == -1: Condition has a Syntax Error, this is a fatal error.
        //  The user should have been notified by the FilterSet Editor which calls validateAllConditions().

        // -- Path Mask:
        if (!fsList[i].path.IsEmpty())
        {
            result = _matchConditionPath(fsList[i].path, fPath);
            if (result == 0) continue;
            if (result == -1) return -1;
        }

        // -- Size:
        if (!fsList[i].size.IsEmpty())
        {
            result = _matchConditionInt(fsList[i].size, fProps.getSize());
            if (result == 0) continue;
            if (result == -1) return -1;
        }

        // -- Date:
        if (!fsList[i].date.IsEmpty())
        {
            result = _matchConditionDate(fsList[i].date, fProps.getDate());
            if (result == 0) continue;
            if (result == -1) return -1;
        }

        // -- Age:
        if (!fsList[i].age.IsEmpty())
        {
            result = _matchConditionInt(fsList[i].age, fProps.getAge());
            if (result == 0) continue;
            if (result == -1) return -1;
        }

        // -- Attributes:
        if (!fsList[i].attributes.IsEmpty())
        {
            result = _matchConditionAttributes(fsList[i].attributes, fProps.getAttrsStr());
            if (result == 0) continue;
            if (result == -1) return -1;
        }

        //  Finally:
        if (result == 1)
        {
            _infoStr = fsList[i].name;
            _filterSetIndex = i;
            return 1;   // All of this filterSet's conditions matched.
        }
    }

    // None of the filterSets matched:
    _infoStr = "";
    return 0;
}
//---------------------------------------------------------------------------

int TAbaktFilter::_matchConditionPath(String mask, String fPath)
{
    // An empty mask means 'any file' (mask is treated as if it is "*"):
    if (mask.IsEmpty()) return 1;

    // Test for illegal characters:
	if (mask.LastDelimiter("/\"<>"))
    {
        _infoStr = _("Syntax error:") + " " + _("characters / \" < > are not allowed");
        return -1;
    }

    // Do match for every entry in path (entries are separated by a ';'):

    if (mask[mask.Length()] != ';')
        mask += ";";    // Append trailing ';' if it is missing.
    TStringList* maskList = new TStringList();
    int maskResult = 0;
    try
    {
        String entry;
        while (mask.Length())
        {
            int pos = mask.Pos(";");
            if (pos == -1)
                pos = mask.Length();   // Last entry in the list.
            entry = mask.SubString(1, pos - 1);
            int entryResult = _matchConditionPathEntry(entry, fPath);
            if (entryResult == -1)
            {
                maskResult == -1;
                break;  // Fatal error, stop now.
            }
            // Mask entries are logically OR'ed:
            maskResult = maskResult | entryResult;  // Should result in 0 or 1.
            mask.Delete(1, pos);    // Remove tested entry from mask.
        }
    }
    catch (Exception &ex)
    {
        _infoStr = "Error in " __FUNC__;
        return -1;  // Error.
    }
    delete maskList;
    return maskResult;
}
//---------------------------------------------------------------------------

int TAbaktFilter::_matchConditionPathEntry(String mask, String fPath)
{
    // Match mask against a "base dir" (ie. the '|' character is used):
    if (mask.Pos("|"))
    {
        String maskBaseDir = mask.SubString(1, mask.Pos("|") - 1);
        mask = mask.Delete(1, maskBaseDir.Length() + 1); // Note that mask.Delete() modifies string mask.
        // maskBaseDir may contain wildcards:
        if (maskBaseDir.LastDelimiter("*?["))
        {
            if (!MatchesMask(ExtractFilePath(fPath), maskBaseDir)) return 0;  // No match.
            if (MatchesMask(ExtractFileName(fPath), mask)) return 1; // fPath matches mask.
            return 0;  // No match.
        }
        if (mask.Pos("\\")) // Mask contains path information.
        {
            // Match mask against subdirs of maskBaseDir too (case-insensitive):
            if (maskBaseDir.LowerCase() != fPath.SubString(1, maskBaseDir.Length()).LowerCase())
                return 0;  // No match.
            // Match mask against post-'|'-part of fPath:
            // Note that fPath.Delete() modifies string fPath.
            if (MatchesMask( fPath.Delete(1, maskBaseDir.Length()), mask))
                return 1; // fPath matches mask.
        }
        else
        {
            // Match mask against maskBaseDir only (case-insensitive):
            if (maskBaseDir.LowerCase() != ExtractFilePath(fPath).LowerCase())
                return 0;  // No match.
            if (MatchesMask(ExtractFileName(fPath), mask))
                return 1; // fPath matches mask.
        }
    }
    else
    {
        if (mask.Pos("\\"))
        {
            // Match against full path:
            if (MatchesMask(fPath, mask)) return 1; // fPath matches mask.
        }
        else
        {
            // Match against filename.ext only:
            if (MatchesMask(ExtractFileName(fPath), mask)) return 1; // fPath matches mask.
        }
    }

	return 0;  // No match.
}
//---------------------------------------------------------------------------

int TAbaktFilter::_matchConditionInt(String condition, const __int64 value)
{
    // Note: [condition] must not be empty, test before calling this function.

    // Procedure:
    // 1. Split condition into expressions (seperated by ';').
    // 2. Test expression(s).
    // 3. If a expression is true, return 1 (expressions are logically OR'ed).

    while (!condition.IsEmpty())
    {
        // Extract first/next expression from [condition]:
        int expressionLen = condition.Pos(";") - 1;
        if (expressionLen == -1) expressionLen = condition.Length();
        String expression = condition.SubString(1, expressionLen);
        expression = expression.Trim();

        // Split expression into operator [expOpStr] (< <= = != >= >) and value [expValStr]:
        String expOpStr, expValStr;   // operator and value as strings.
        if (!_splitExpression(expression, expOpStr, expValStr))
            return -1;  // Syntax error.

        // Convert expValStr to (__int64 expVal):
        __int64 expVal = FileSizeStrToIntDef(expValStr, -1);
        if (expVal == -1)
        {
            _infoStr = ASPF(_("Syntax error:") + " " + _("value '%s' is not a valid number"), expValStr);
            return -1;
        }

        // Test expression (result is either 0 or 1, or -1 if the operator is not recognised):
        int result = -1;
        if (expOpStr == "<")
            result = (value < expVal);
        else if (expOpStr == "<=")
            result = (value <= expVal);
        else if (expOpStr == "==")
            result = (value == expVal);
        else if (expOpStr == "!=")
            result = (value != expVal);
        else if (expOpStr == ">=")
            result = (value >= expVal);
        else if (expOpStr == ">")
            result = (value > expVal);

        if (result == -1)
        {
            _infoStr = ASPF(_("Syntax error:") + " " + _("operator '%s' not recognised"), expOpStr);
            return -1;
        }
        if (result == 1) return 1;  // Expressions are logically OR'ed.

        // Next expression:
        condition = condition.Delete(1, expressionLen + 1);
    }

    return 0;
}
//---------------------------------------------------------------------------

int TAbaktFilter::_matchConditionDate(String condition, const TDateTime fDate)
{
    // Note: [condition] must not be empty, test before calling this function.

    // Procedure:
    // 1. Split condition into expressions (seperated by ';').
    // 2. Test expression(s).
    // 3. If a expression is true, return 1 (expressions are logically OR'ed).

    while (!condition.IsEmpty())
    {
        // Extract first/next expression from [condition]:
        int expressionLen = condition.Pos(";") - 1;
        if (expressionLen == -1) expressionLen = condition.Length();
        String expression = condition.SubString(1, expressionLen);
        expression = expression.Trim();

        // Split expression into operator [expOpStr] (< <= = != >= >) and value [expValStr]:
        String expOpStr, expValStr;   // operator and value as strings.
        if (!_splitExpression(expression, expOpStr, expValStr))
            return -1;  // Syntax error.

        // Convert expValStr to TDateTime expVal:
        TDateTime expVal;
        try
        {
             expVal= abtStrToDate(expValStr);
        }
        catch (...)
        {
            _infoStr = ASPF(_("Syntax error:") + " " + _("value '%s' is not a valid date"), expValStr);
            return -1;
        }

        // Test expression (result is either 0 or 1, or -1 if the operator is not recognised):
        int result = -1;
        if (expOpStr == "<")
            result = (fDate < expVal);
        else if (expOpStr == "<=")
            result = (fDate <= expVal);
        else if (expOpStr == "==")
            result = (fDate == expVal);
        else if (expOpStr == "!=")
            result = (fDate != expVal);
        else if (expOpStr == ">=")
            result = (fDate >= expVal);
        else if (expOpStr == ">")
            result = (fDate > expVal);

        if (result == -1)
        {
            _infoStr = ASPF(_("Syntax error:") + " " + _("operator '%s' not recognised"), expOpStr);
            return -1;
        }
        if (result == 1) return 1;  // Expressions are logically OR'ed.

        // Next expression:
        condition = condition.Delete(1, expressionLen + 1);
    }

    return 0;
}
//---------------------------------------------------------------------------

int TAbaktFilter::_matchConditionAttributes(String condition, const String fAttrs)
{
    // Note: [condition] must not be empty, test before calling this function.

    // Procedure:
    // 1. Split condition into expressions (seperated by ';').
    // 2. Test expression(s).
    // 3. If a expression is true, return 1 (expressions are logically OR'ed).

    while (!condition.IsEmpty())
    {
        // Extract first/next expression from [condition]:
        int expressionLen = condition.Pos(";") - 1;
        if (expressionLen == -1) expressionLen = condition.Length();
        String expression = condition.SubString(1, expressionLen);
        expression = expression.Trim();

        // Split expression into operator [expOpStr] and value [expValStr]:
        String expOpStr, expValStr;   // operator and value as strings.
        if (!_splitExpression(expression, expOpStr, expValStr))
            return -1;  // Syntax error.

        // For attributes only these operators are allowed: (= !=) .
        // Test expression (result is either 0 or 1, or -1 if the operator is not recognised):

        int result = -1;
        if (expOpStr == "==")
        {
            result = 1;
            // For every attribute in expValStr test if it is set:
            for (int i = 1; i <= expValStr.Length(); i++)
                if (fAttrs.Pos(expValStr) == 0)
                {
                    result = 0;
                    break;
                }
        }
        else if (expOpStr == "!=")
        {
            result = 1;
            // For every attribute in expValStr test if it is NOT set:
            for (int i = 1; i <= expValStr.Length(); i++)
                if (fAttrs.Pos(expValStr) != 0)
                {
                    result = 0;
                    break;
                }
        }

        if (result == -1)
        {
            _infoStr = ASPF(_("Syntax error:") + " " + _("operator '%s' not recognised"), expOpStr);
            return -1;
        }
        if (result == 1) return 1;  // Expressions are logically OR'ed.

        // Next expression:
        condition = condition.Delete(1, expressionLen + 1);
    }

    return 0;
}
//---------------------------------------------------------------------------

bool TAbaktFilter::_splitExpression(String expression, String &expOpStr, String &expValStr)
{
    if (expression.Length() < 2)    // At least two characters are required: '=2', '!=', etc.
    {
        _infoStr = ASPF(_("Syntax error:") + " " + _("expression '%s' too short"), expression);
        return false;
    }

    // Valid operators are: < <= == != >= >
    switch (expression[1])
    {
        case '<':   // < or <=
            expOpStr = "<";
            break;

        case '=':   // ==
            expOpStr = "=";
            break;

        case '!':   // !=
            expOpStr = "!";
            break;

        case '>':   // > or >=
            expOpStr = ">";
            break;

        default:
            expOpStr = "";  // Not good.
    }

    if (expOpStr.IsEmpty())
    {
        _infoStr = ASPF(_("Syntax error:") + " " + _("no operator in expression '%s'"), expression);
        return false;
    }
    // Deals with second character of operater (=) used in expressions: <=, ==, !=, >=
    if (expression[2] == '=')
        expOpStr += "=";

    // Now that we have an operator, the rest of the expression must be the value:
    expValStr = expression.SubString(expOpStr.Length() + 1, expression.Length());
    expValStr = expValStr.Trim();
    if (expValStr.IsEmpty())
    {
        _infoStr = ASPF(_("Syntax error:") + " " + _("no value in expression '%s'"), expression);
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------

String TAbaktFilter::validateAllConditions(TAbaktFilterSet& fs)
{
    // The file to use for validating:
    String fPath = Application->ExeName;
    String invalidMsg;
    int result;

    TFileProperties fProps(fPath);
    if (fProps.failed())
    {
        invalidMsg = __FUNC__ ": Could not read file properties. " + fProps.getErrorStr();
        return invalidMsg;
    }

    result = _matchConditionPath(fs.path, fPath);
    if (result == -1) invalidMsg = invalidMsg +
        _("Condition") + " " + _("Path") + ": " + _infoStr;

    result = _matchConditionInt(fs.size, fProps.getSize());
    if (result == -1) invalidMsg = invalidMsg +
        _("Condition") + " " + _("Size") + "': " + _infoStr;

    result = _matchConditionDate(fs.date, fProps.getDate());
    if (result == -1) invalidMsg = invalidMsg +
    _("Condition") + " " + _("Date") + "': " + _infoStr;

    result = _matchConditionInt(fs.age, fProps.getAge());
    if (result == -1) invalidMsg = invalidMsg +
        _("Condition") + " " + _("Age") + "': " + _infoStr;

    result = _matchConditionAttributes(fs.attributes, fProps.getAttrsStr());
    if (result == -1) invalidMsg = invalidMsg +
        _("Condition") + " " + _("Attributes") + ": " + _infoStr;

    return invalidMsg;
}
//---------------------------------------------------------------------------

