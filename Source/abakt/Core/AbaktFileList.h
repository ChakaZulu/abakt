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
#ifndef AbaktFileListH
#define AbaktFileListH
//---------------------------------------------------------------------------
#include "AbaktFilter.h"
//---------------------------------------------------------------------------

class TAbaktFileList {

public:

	typedef void CALLBACK_OnCurrentFile(TAbaktFilter* filterItem);
	CALLBACK_OnCurrentFile* OnCurrentFile;
	/*  This callback will be called for every file, and for every directory
        that could not be opened.
        The calling function should always call fileHasError(). If there was
        an error [filterItem] is NULL, and getErrorString provides more information.

        How to use the callback function:
		Add the following declaration to the class that instantiates TAbaktFileList:
			static void OnCurrentFile(TAbaktFilter* filterItem); */

	TAbaktFileList()
	{
		errorItemList = new TStringList();
		emptyDirList = new TStringList();

		_building = false;
		OnCurrentFile = NULL;

		incFileName = "";
		excFileName = "";

		_scanFullTree = false; // Do it here, so it can be modified before calling a Build...() function.
		_reset();
	}

	~TAbaktFileList()
	{
        delete emptyDirList;
		delete errorItemList;
	}

	TStringList* errorItemList;
	/*  List of files and directories that had errors during building.
        f_Action also uses this list to store filenames that could not be added to a zip archive. */

    TStringList* emptyDirList;
    /*  This list contains every empty directory encountered during building. */

	unsigned int incFilesTotalCount;
	__int64 incFilesTotalSize;
	unsigned int excFilesTotalCount;
	__int64 excFilesTotalSize;

	String incFileName;
    /*  FileName of file that contains files to include. */

	String excFileName;
    /*  FileName of file that contains files to exclude. */

	bool buildAll();
	/*	Builds lists for all Source Root Directories.
        Calls OnCurrentFile for every tested file.
		The in-/excluded filelists are saved to disk (incFileName and excFileName).
		Returns false if (_fatalError == true) or (_aborted == true),
        call getErrorString() to get more information. */

	bool buildDir(String dir, bool recurse);
	/*	See _doBuild(). */

	bool isBuilding()
	{	return _building; }

	void abort()
	{	_aborted = true; }
	/*	Aborts the building process.
		Building has stopped when _building == false. */

	bool aborted()
	{	return _aborted; }

    bool fileHasError()
    {   return _fileError; }
    /*  Call this function in your OnCurrentFile callback to test if there
        is an error with the current file. */

	String getErrorString()
	{	return _errorString; }
    /*  Call this function in your OnCurrentFile callback to get more
        information on the error with the current file. */

	TAbaktFilter* getFilterItem()
	{	return &_filterItem; }
	/*	Returns a pointer to the TAbaktFilter object.
		Use this in OnCurrentFile to get file information. */

	String getCurrentSourceRootDir()
	{	return _currentSourceRootDir; }

	bool setScanFullTree(bool full);
	/*	Should the full tree be scanned?
		If false, only the selected Source Directories will be scanned.
		Does nothing when building is in progress (_building == true).
		Returns the value _scanFullTree is set to. */

	bool getScanFullTree()
	{	return _scanFullTree; }

    void cleanUp();
    /*  Deletes files incFileName and excFileName.
        Clears errorItemList and emptyDirList. */

private:
//	void* FOwner;
	/*	Pointer to the class that created this TAbaktFileList object.
		You can use this pointer to gain access to the methods/properties of the owner.
		This pointer is passed as parameter [owner] of OnCurrentFile(). */

	bool _building;
	/*	Is true when Build() or BuildAll() is busy. */

	bool _aborted;
	/*	When set to true using abort() it may take some time
		before _building is false. */

	bool _fatalError;
	/*	If true a fatal error occurred during building.
		More information can be obtained by calling getErrorString(). */

	bool _recurse;

	bool _scanFullTree;
	/*	If set, the excluded directory trees will be scanned too. */

	bool _fileError;
	String _errorString;
	String _currentSourceRootDir;
	TAbaktFilter _filterItem;

	void _reset();
	/*	Resets all variables to their initial values. */

	void _doBuild(String dir, std::ofstream& ofs_inc, std::ofstream& ofs_exc);
	/*	Builds list for [dir].
		Recursive if _recurse is true.
		For every file that is tested OnCurrentFile is called (if set). */

	void _appendFPathToFile(String fpath, std::ofstream& ofs);
	void _appendFPathToIncFile(String fpath);
	void _appendFPathToExcFile(String fpath);

	bool _deleteOldListFile(String fpath);
	/*	Returns true if [fpath] does not exist or was safely deleted.
		Returns false otherwise. */
};
//---------------------------------------------------------------------------
#endif
