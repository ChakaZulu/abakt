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

#include "AbaktFileList.h"
#include "AbaktProfile.h"
#include "eccString.h"
#include "GlobalFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

bool TAbaktFileList::buildAll()
{
	if (_building && !_fatalError) // If FError is true a previous Build...() has failed.
	{
		_fatalError = true;
		_errorString = __FUNC__ ": Building already in progress." ;
		return !_fatalError;
	}

	_reset();
	_building = true;

	// Clean-up old FileList files if they exist:
	_fatalError = !_deleteOldListFile(incFileName);
	if (!_fatalError)
		_fatalError = !_deleteOldListFile(excFileName);
	if (_fatalError)
		return !_fatalError;

	std::ofstream ofs_inc(incFileName.c_str()); // Can be 0 if IncFileName is empty.
	std::ofstream ofs_exc(excFileName.c_str()); // Can be 0 if ExcFileName is empty.

	for (unsigned int i = 0; i < PProfile->sourceRootDirList.size(); i++)
	{
		_currentSourceRootDir = IncludeTrailingPathDelimiter(PProfile->sourceRootDirList[i]);
		try
		{
			_doBuild(_currentSourceRootDir, ofs_inc, ofs_exc);
		}
		catch (Exception &ex)
		{
			_fatalError = true;
			_errorString = __FUNC__ ": Fatal Error: " + ex.Message;
		}
		if (_aborted) break;
		if (_fatalError) break;
	}

	_building = false;

	if (_fatalError || _aborted) return false;
	else return true;
}
//---------------------------------------------------------------------------

bool TAbaktFileList::buildDir(String dir, bool recurse)
{
	if (_building)
	{
		_errorString = __FUNC__ ": Building already in progress." ;
		_fatalError = true;
		return !_fatalError;
	}

	_reset();
	_building = true;

	std::ofstream ofs_inc_dummy;
	std::ofstream ofs_exc_dummy;

	try
	{
		_doBuild(dir, ofs_inc_dummy, ofs_exc_dummy);
	}
	catch (Exception &ex)
	{
		_fatalError = true;
		_errorString = __FUNC__ ": Fatal Error: " + ex.Message;
	}

	_building = false;
	return !_fatalError;
}
//---------------------------------------------------------------------------

void TAbaktFileList::_reset()
{
	_building = false;

	errorItemList->Clear();
    emptyDirList->Clear();
	incFilesTotalCount  = 0;
	incFilesTotalSize   = 0;
	excFilesTotalCount  = 0;
	excFilesTotalSize   = 0;

	_currentSourceRootDir = "";
	_errorString    = "";
	_recurse        = true;
	_aborted        = false;
	_fatalError     = false;
}
//---------------------------------------------------------------------------

bool TAbaktFileList::setScanFullTree(bool full)
{
	if (!_building) _scanFullTree = full;
	return _scanFullTree;
}
//---------------------------------------------------------------------------

void TAbaktFileList::cleanUp()
{
    errorItemList->Clear();
    emptyDirList->Clear();
    DeleteFile(incFileName);
	DeleteFile(excFileName);
}
//---------------------------------------------------------------------------

void TAbaktFileList::_doBuild(String dir, std::ofstream& ofs_inc, std::ofstream& ofs_exc)
{
	// Test if building should be aborted:
	if (_aborted) return;
	if (_fatalError) return;

   	_fileError = false;

	// Get the first item:
	dir = IncludeTrailingPathDelimiter(dir);

	HANDLE fhandle;
	WIN32_FIND_DATA filedata;
	String fitem;
	bool lastFile = false;

    int thisDirSubDirCount  = 0;  // Number of subdirs in [dir]. If 0 and (thisDirIncFileCount==0), directory is added to emptyDirList.
    int thisDirIncFileCount = 0;  // Number of Included files in [dir]. If 0, directory is added to emptyDirList.

	if (!_scanFullTree)
	{
		// Skip dir-trees that are excluded:
		if (PProfile->dirInExcludeTreeList(dir))
			return;
	}

	fhandle = ::FindFirstFile( String(dir + "*").c_str(), &filedata);
	if (fhandle == INVALID_HANDLE_VALUE)
	{
        DWORD lastError = GetLastError();
		_fileError = true;
        errorItemList->Append(dir);
        _errorString = ASPF(_("Error [%d] opening directory %s: %s"),
            lastError, ecc::QuoteStr(dir), abtGetLastErrorStr(lastError));
        // Call callback with empty filteritem to notify caller of error:
        if (OnCurrentFile) OnCurrentFile(NULL);
		return;
	}

	while (!lastFile)
	{
		if (_aborted)
		{
			FindClose(fhandle);
			return;
		}
		fitem = String(filedata.cFileName);

		// Test if the filedata-object is a directory or a file:
		if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
            // -- Object is a directory.
			Application->ProcessMessages(); // For lengthy operations.
			if ((fitem != ".") && (fitem != ".."))
			{
                ++thisDirSubDirCount;
				if (_recurse)
					_doBuild(dir + fitem, ofs_inc, ofs_exc);
			}
		}
		else
		{
            // -- Object is a file. Add it to Items.
			if (_filterItem.testFile(dir + fitem))
			{
                ++thisDirIncFileCount;
				++incFilesTotalCount;
				incFilesTotalSize += _filterItem.getFileProps().getSize();
				_appendFPathToFile(dir + fitem, ofs_inc);
			}
			else
			{
                // Only add files in _selected_ directories to list of excluded files:
                if (!PProfile->dirInExcludeDirList(dir))
                {
                    ++excFilesTotalCount;
                    excFilesTotalSize += _filterItem.getFileProps().getSize();
                    _appendFPathToFile(dir + fitem, ofs_exc);
                }
			}
			// Call callback if it exists:
			if (OnCurrentFile)
				OnCurrentFile(&_filterItem);
		}

		// Is this the last file in dir?
		if (!FindNextFile(fhandle, &filedata))
		{
            DWORD lastError = GetLastError();
			if (lastError == ERROR_NO_MORE_FILES)
				lastFile = true;
            else
            {
          		_fileError = true;
                String file = dir + String(filedata.cFileName);
        		_errorString = ASPF(_("Error [%d] in file %s: %s"),
                    lastError, ecc::QuoteStr(file), abtGetLastErrorStr(lastError));
                errorItemList->Append(file);
    			// Call callback with empty filteritem to notify caller of error:
	    		if (OnCurrentFile) OnCurrentFile(NULL);
            }
		}
	}

    // Remember empty directories (that passed the filter):
    // - f_Action will add these dirs to the backup if required.
    if (thisDirSubDirCount == 0)
    {
        if (thisDirIncFileCount == 0)
        {
            emptyDirList->Append(dir);
        }
    }

	FindClose(fhandle);
}
//---------------------------------------------------------------------------

void TAbaktFileList::_appendFPathToFile(String fpath, std::ofstream& ofs)
{
	if (!ofs) return;
	ofs << fpath.c_str() << std::endl;
}
//---------------------------------------------------------------------------

void TAbaktFileList::_appendFPathToIncFile(String fpath)
{
	if (incFileName.IsEmpty()) return;

	std::ofstream ofs(incFileName.c_str(), std::ios::app);
	if (!ofs)
	{
		_errorString = __FUNC__ " : Could not create std::ofstream " + ecc::QuoteStr(incFileName);
		_fatalError = true;
		return;
	}

	ofs << fpath.c_str() << std::endl;
}
//---------------------------------------------------------------------------

void TAbaktFileList::_appendFPathToExcFile(String fpath)
{
	if (excFileName.IsEmpty()) return;

	std::ofstream ofs(excFileName.c_str(), std::ios::app);
	if (!ofs)
	{
		_errorString = __FUNC__ " : Could not create std::ofstream " + ecc::QuoteStr(excFileName);
		_fatalError = true;
		return;
	}

	ofs << fpath.c_str() << std::endl;
}
//---------------------------------------------------------------------------

bool TAbaktFileList::_deleteOldListFile(String fpath)
{
	if (!FileExists(fpath)) return true;

	if (!DeleteFile(fpath))
	{
		_errorString = "Could not delete List File: " + ecc::QuoteStr(fpath) + ".";
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------

