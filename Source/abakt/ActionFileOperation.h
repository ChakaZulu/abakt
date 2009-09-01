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
#ifndef ActionFileOperationH
#define ActionFileOperationH
//---------------------------------------------------------------------------
#include "eccString.h"
//---------------------------------------------------------------------------
/*
	Usage:
	This module depends on module f_Action,
	see Tf_Action::DoActionFileOperation() for an example.

	The idea is this:
	Call Init() to test if the requirements are met.
	Call f_Action->BuildFileList().
	Call Execute();
*/
//---------------------------------------------------------------------------
class TActionFileOperation {

public:

	TActionFileOperation();

	bool init();
	/* 	Checks requirements to execute action and sets variables.
		Sets _inited, _error and _aborted.
		Returns false if _error or _aborted are set. */

	String getDestinationDir()
	{
		if (!_inited) return "";
		else return _destinationDir;
	}

	bool execute();
	/* 	Performs Action according to the settings in PProfile.
		Returns false if _error or _aborted are set. */

	void abort();
	/*	Aborts the execution, unless _error is true. */

	bool aborted()
	{	return _aborted; }

	bool failed()
	{	return _error; }

private:

	typedef BOOL (WINAPI* PCOPYFILEEX)(LPCTSTR, LPCTSTR, LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD);
	PCOPYFILEEX _pCopyFileEx;
	/* 	_pCopyFileEx points to function CopyFileExA in kernel32.dll if it exists, else NULL. */

	typedef BOOL (WINAPI* PMOVEFILEWITHPROGRESS)(LPCTSTR, LPCTSTR, LPPROGRESS_ROUTINE, LPVOID, DWORD);
	PMOVEFILEWITHPROGRESS _pMoveFileWithProgress;
	/* 	_pMoveFileWithProgress points to function MoveFileWithProgressA in kernel32.dll if it exists, else NULL. */

	bool _inited;	// If false, Execute() is not allowed.
	bool _error;
	bool _aborted;
	int _copyFileExCancel;	// Set to true to cancel CopyFileEx() operation.
	FILEOP_FLAGS _fopFlags;
	UINT _fopFunc;
	String _operationStr;

	String _destinationDir;
	/* 	Contains Destination's baseDir + Destination's dirName,
		including trailing backslash. */

	bool _initCopyMove();
	/*  Displays and sets current Copy/Move settings and tests _destinationDir.
		Returns false if an error occurred. */

	bool _confirmDelete();
	/*  Displays a popup asking to proceed with Delete operation.
        Returns false if Delete operation should be cancelled. */

//removed 095b1:	void _doOperation_SHFileOperation();
	/*	Copy, Move or Delete files using the Windows built-in function SHFileOperation().
		Lacks callback/verification per file.
		Sets _error on error.
		Sets FAbort when Cancel button is pressed. */

	void _doOperation_FileByFile();
	/*	Copy, Move or Delete files.
		Has callback/verification per file.
		Sets _error and/or FAbort when applicable. */

    void _doOperation_EmptyDirs();
	/*	(095b1:) Copies, moves or delete empty Source directories. */

	DWORD _doCopyFile(String src, String dest);
	/*	Calls Windows function CopyFileEx().
		Returns false if an error occurred. */

	DWORD _doMoveFile(String src, String dest);
	/*	Calls Windows function MoveFileEx().
		Returns false if an error occurred. */

	DWORD _doDeleteFile(String src);
	/*	Calls Windows function DeleteFile.
		Returns false if an error occurred. */

    bool _testCreateDir(String dir);
	/*	Returns true if [dir] exists.
		If [dir] does not exist it is created.
		Returns false if [dir] could not be created. */

	void _log(const String msg,
			int lines_before = 0, int lines_after = 0);

	void _log(const ecc::TLBMessageStyle style, const String msg,
			int lines_before = 0, int lines_after = 0);
};
//---------------------------------------------------------------------------
#endif
