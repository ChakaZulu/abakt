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

#include "ActionFileOperation.h"
#include "Action.h"
#include "AbaktProfile.h"
#include "AbaktGlobals.h"
#include "eccFile.h"
#include "GlobalFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

/*	CopyProgressRoutine Callback Declaration:
	Note: LARGE_INTEGER is an __int64 type. */
static DWORD __stdcall CopyProgress_Callback(
		LARGE_INTEGER TotalFileSize,
		LARGE_INTEGER TotalBytesTransferred,
		LARGE_INTEGER StreamSize,
		LARGE_INTEGER StreamBytesTransferred,
		DWORD dwStreamNumber,
		DWORD dwCallbackReason,
		HANDLE hSourceFile,
		HANDLE hDestinationFile,
		LPVOID lpData);
//---------------------------------------------------------------------------

/*	CopyProgressRoutine Callback Definition: */
static DWORD __stdcall CopyProgress_Callback(
		LARGE_INTEGER TotalFileSize,
		LARGE_INTEGER TotalBytesTransferred,
		LARGE_INTEGER StreamSize,
		LARGE_INTEGER StreamBytesTransferred,
		DWORD dwStreamNumber,
		DWORD dwCallbackReason,
		HANDLE hSourceFile,
		HANDLE hDestinationFile,
		LPVOID lpData)
{
	long double part  	= (long double)TotalBytesTransferred.QuadPart;
	long double total	= (long double)TotalFileSize.QuadPart;
	long double perc	= 0;

	if (total)	// Prevent Division by Zero.
		perc = (part / total) * 100;

	f_Action->pb_File->Position = (int)perc;
	f_Action->lbl_FilePerc->Caption	= IntToStr((int)perc) + "%";

	Application->ProcessMessages();
	TActionFileOperation* afo = (TActionFileOperation*)lpData;

	return (afo->aborted() ? PROGRESS_CANCEL : PROGRESS_CONTINUE);
};
//===========================================================================

//---------------------------------------------------------------------------

TActionFileOperation::TActionFileOperation()
{
	_inited	= false;

	/*	== Detect if CopyFileExA() exists in kernel32.dll:
		If _pCopyFileEx is NULL, the function CopyFileExA() does not exist,
        and FileCopy() should be called.
		Example: Search for getdiskfreespaceex on http://msdn.microsoft.com  */
	_pCopyFileEx = (PCOPYFILEEX) GetProcAddress(
		GetModuleHandle("kernel32.dll"), "CopyFileExA" );
	if (_pCopyFileEx)
	{	/*	Test if CopyFileEx functions properly. For some weird reason this function
			exists on some systems but fails to work.
			Will now perform a test-copy to see if it works. */
		_pCopyFileEx(	// returns ZERO (false) on failure.
			PGlobals->getIniFilePath().c_str(), 	// pointer to name of an existing file
			PGlobals->getIniFilePath().c_str(),		// pointer to filename to copy to
			CopyProgress_Callback,      // pointer to the callback function
			this, 				        // to be passed to the callback function
			&_copyFileExCancel,         // flag that can be used to cancel the operation
			COPY_FILE_FAIL_IF_EXISTS	//DWORD dwCopyFlags	// flags that specify how the file is copied
		    );
		if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
		{
			f_Action->logNotif("Ok: CopyFileExA is available on this system,"
				" but does not function properly. Falling back to simple FileCopy.");
			_pCopyFileEx = NULL;
		}
	}

	/*  Detect if MoveFileWithProgressA exists in kernel32.dll: */
	_pMoveFileWithProgress = (PMOVEFILEWITHPROGRESS) GetProcAddress(
		GetModuleHandle("kernel32.dll"), "MoveFileWithProgressA" );
}
//---------------------------------------------------------------------------

bool TActionFileOperation::init()
{
	_inited		= false;
	_destinationDir = "";
	_aborted	= false;
	_copyFileExCancel = FALSE;
	_error		= false;
	_fopFlags	= FOF_MULTIDESTFILES | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;

	switch ( (TActFileMethod)PProfile->config.Actions.File->eMethod->getIndex() )
	{
		case afmCopy:
			_fopFunc = FO_COPY;
			_operationStr = _("Copying Files:");
			if (!_initCopyMove())
				_error = true;
			break;

		case afmMove:
			_fopFunc = FO_MOVE;
			_operationStr = _("Moving Files:");
			if (!_initCopyMove())
				_error = true;
			break;

		case afmDelete:
			_fopFunc = FO_DELETE;
			_operationStr = _("Files.");
			if (PProfile->config.Actions.File->bUseRecycleBin->value)
			{
				_fopFlags |= FOF_ALLOWUNDO;
				_operationStr += " " + _("Using Recycle Bin if possible.");
			}
			_log( ASPF(_("Method: Delete %s"), _operationStr) );
			_operationStr = ASPF(_("Deleting %s"), _operationStr);
			if (!_confirmDelete())
				_aborted = true;
			break;

		default:
			_log(ecc::smtAppError, __FUNC__ " : Method is undefined.");
			_error = true;
	}

	_inited	= true; // Allow execute().
	return !(_error || _aborted);
}
//---------------------------------------------------------------------------

bool TActionFileOperation::_initCopyMove()
{
	_destinationDir = PProfile->config.Destination->pDir->value;
    if (_destinationDir.IsEmpty()) //092RC2
    {
    	_log(ecc::smtError, _("Destination Directory can not be empty."));
        return false;
    }

	_destinationDir = _destinationDir + f_Action->getDestName();

	// Destination directory has been created (if necessary) in f_Action.
	_destinationDir = IncludeTrailingPathDelimiter(_destinationDir);
	_log( ASPF(_("Destination Directory: %s"), ecc::QuoteStr(_destinationDir)) );

    // Path Information:
    _log(_("Path Information") + ": "
        + _(PProfile->config.Actions.File->ePathInformation->selected()) );

	// Overwrite:
	if (PProfile->config.Actions.File->bOverwrite->value)
	{
		_log(_("Existing files will be overwritten."));
		_fopFlags &= ~FOF_RENAMEONCOLLISION;
	}

	return true;
}
//---------------------------------------------------------------------------
#include "process.h" 
bool TActionFileOperation::execute()
{
	if (!_inited) return false;
	if (_error || _aborted) return false;

	f_Action->lbl_ProgressInfo->Visible = true;
    try
    {
/* removed 095b1:
         if (PProfile->config.Actions.File->bUseSHFileOperation->value)
            _doOperation_SHFileOperation();
        else */
        {
            _doOperation_FileByFile();
            _doOperation_EmptyDirs();
        }
    }
    catch (Exception &ex)
	{
		_log(ecc::smtAppError, "Exception in " __FUNC__ ": " + ex.Message);
    }
	f_Action->lbl_ProgressInfo->Visible = false;

	_inited = false;	// Reset.
	return !(_error || _aborted);
}
//---------------------------------------------------------------------------

void TActionFileOperation::abort()
{
	if (_error) return;
	_aborted = true;
	_copyFileExCancel = TRUE;
}
//---------------------------------------------------------------------------

bool TActionFileOperation::_confirmDelete()
{
	if (!PGlobals->ActFile_ConfirmDelete)
		return true;

	if (PGlobals->cmdLine_AutoExecute)
	{
		_log(_("Confirming Action Type Delete is disabled when performing an AutoExecute backup using the command line."));
		return true;
	}

	f_Action->timeLapse.Pause();
	int val = MessageDlg(
		_("You chose to Delete files.") + "\n\n"
        + _("Are you sure?"),
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);
	f_Action->timeLapse.Resume();
	if (val != mrYes)
		return false;
	return true;
}
//---------------------------------------------------------------------------

//removed 095b1:
/* void TActionFileOperation::_doOperation_SHFileOperation()
{
	if (_error || _aborted)
		return;

	_log(ecc::smtDefB, _operationStr, 1);
	_log(ecc::smtNotif, _("Using Windows function SHFileOperation."));

	std::ifstream ifs(f_Action->abtFileList->incFileName.c_str());
	if (!ifs)
	{
		_log(ecc::smtError,
            ASPF(_("Could not open file: %s"), ecc::QuoteStr(f_Action->abtFileList->incFileName)) );
		_error = true;
		return;
	}

	//	Determine which files to Copy/Move/Delete:
	String src_fpath, src_relative_fpath, dest_fpath;
	String src_list, dest_list;
	String log;
	unsigned int lineCount = 0;
	float perc;
	char line[2048];

	while (ifs.getline(line, 2048))
	{
		// Test for reading more than 2048 chars or another error:
		if (ifs.fail())
		{
			f_Action->logError(__FUNC__ " : Error while reading stream. Please report.");
			_error = true;
			break;
		}

		// Display progress:
		++lineCount;
		perc = ((float)lineCount / (float)f_Action->abtFileList->incFilesTotalCount) * 100;
		f_Action->lbl_ProfilePerc->Caption	= IntToStr((int)perc) + "%";
		f_Action->pb_Profile->Position 		= (int)perc;
        f_Action->updateAppTitlePerc(); //092b8
		Application->ProcessMessages();

		//	Determine relative destination path by stripping drive-part from [src_fpath].
		src_fpath = String(line);
		src_relative_fpath = src_fpath.SubString( ExtractFileDrive(src_fpath).Length() + 1, src_fpath.Length() );
		dest_fpath = ExcludeTrailingBackslash(_destinationDir) + src_relative_fpath;

		log = ASPF(_("Performing SHFileOperation (%d/%d) on %s"), //+ " to " + ecc::QuoteStr(dest_fpath);
            lineCount, f_Action->abtFileList->incFilesTotalCount,
            ecc::QuoteStr(src_fpath) );
		f_Action->lbl_ProgressInfo->Caption = log;
		src_list += src_fpath + '\0';
		dest_list += dest_fpath + '\0';
	}
	ecc::AddDoubleZeroToString(src_list);
	ecc::AddDoubleZeroToString(dest_list);

	//	Do SHFileOperation:
	SHFILEOPSTRUCT fop;
	//  See: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/shellcc/platform/shell/reference/structures/shfileopstruct.asp
	ZeroMemory(&fop, sizeof(fop));
	fop.hwnd	= 0;
	fop.wFunc 	= _fopFunc;
	fop.pFrom 	= src_list.c_str();
	fop.pTo		= dest_list.c_str(); // Ignored when FO_DELETE.
	fop.fFlags	= _fopFlags;
	//	BOOL fAnyOperationsAborted receives true if operation was cancelled. //
	fop.lpszProgressTitle	= "Abakt - Action File Operation in progress"; // Requires flag FOF_SIMPLEPROGRESS.

	if (SHFileOperation(&fop))	// SHFileOperation() returns true on failure.
	{
		if (fop.fAnyOperationsAborted)
		{
			_log(ecc::smtDefB, _("Aborted."));
			_aborted = true;
		}
		else
		{
			_log(ecc::smtError, _("Failed."));
			_error = true;
		}
	}
	else
	{
		_log(ecc::smtDefB, _("Ok."));
	}
} */
//---------------------------------------------------------------------------

void TActionFileOperation::_doOperation_FileByFile()
{
	_log(ecc::smtDefB, _operationStr, 1);
	_log(ecc::smtNotif, _("Performing File-By-File Operation."));

	std::ifstream ifs(f_Action->abtFileList->incFileName.c_str());
	if (!ifs)
	{
		_log(ecc::smtError,
            ASPF(_("Could not open file: %s"), ecc::QuoteStr(f_Action->abtFileList->incFileName)) );
		_error = true;
		return;
	}

	TActFileMethod method = (TActFileMethod)PProfile->config.Actions.File->eMethod->getIndex();
	if ( (method == afmCopy) && (_pCopyFileEx == NULL) )
		_log(ecc::smtNotif, "Using simple CopyFile without progress indicators (Windows 95/98/ME).");
	if ( (method == afmMove) && (_pMoveFileWithProgress == NULL) )
		_log(ecc::smtNotif, "Using simple MoveFile without progress indicators (Windows 95/98/ME/NT).");

	DWORD lastError = NULL;
	String srcFileName, src_relative_fpath, destFileName;
	String logMsg;
	unsigned int lineCount = 0;
	unsigned int successFileCount = 0;
	float perc;
	char line[2048];

	f_Action->lbl_ProgressInfo->Visible = true;

    // == For every source file ... :
	while (ifs.getline(line, 2048))
	{
		// Test for reading more than 2048 chars or another error:
		if (ifs.fail())
		{
			f_Action->logError(__FUNC__ " : Error while reading stream. Please report.");
			_error = true;
			break;
		}

		if (_aborted)
		{
			_log(ecc::smtDefB, _("Aborted."));
			break;
		}

		// Display progress:
		++lineCount;
		perc = ((float)lineCount / (float)f_Action->abtFileList->incFilesTotalCount) * 100;
		f_Action->lbl_ProfilePerc->Caption	= IntToStr((int)perc) + "%";
		f_Action->pb_Profile->Position 		= (int)perc;
		Application->ProcessMessages();

        // === Adjust destFileName to PProfile->config.Actions.File->ePathInformation:

		//	Determine relative destination path by stripping drive-part from [srcFileName].
		srcFileName = String(line);
		src_relative_fpath = srcFileName.SubString( ExtractFileDrive(srcFileName).Length() + 1, srcFileName.Length() );

        bool rootSrcDirFound;
        String rootSrcDir;
        int azp = (TActZipStorePath)PProfile->config.Actions.File->ePathInformation->getIndex();
        switch (azp)
        {
            case azpNone:
                destFileName = IncludeTrailingBackslash(_destinationDir) + ExtractFileName(srcFileName);
                break;

            case azpRelative:
                rootSrcDirFound = false;
                for (unsigned int i = 0; i < PProfile->sourceRootDirList.size(); i++)
                {
                    rootSrcDir = PProfile->sourceRootDirList[i];
                    if (srcFileName.Pos(rootSrcDir) == 1)
                    {
                        rootSrcDirFound = true;
                        break;
                    }
                }
                #ifdef ABT_TEST
                    if (!rootSrcDirFound) // Should never happen!
                    {
                        f_Action->logError(__FUNC__": ROOT NOT FOUND!? Please report.");
                    }
                #endif
                destFileName = ExcludeTrailingBackslash(_destinationDir)
                    + srcFileName.SubString(rootSrcDir.Length(), srcFileName.Length()); // Strip drive+rootdir
                break;

            default:
            case azpFull:
                destFileName = ExcludeTrailingBackslash(_destinationDir) + src_relative_fpath;
        }

        // Perform chosen File Operation:
    	switch (method)
		{
			case afmCopy:
				logMsg = ASPF(_("Copying [%d/%d]: %s --> %s"),
                    lineCount, f_Action->abtFileList->incFilesTotalCount,
                    ecc::QuoteStr(srcFileName), ecc::QuoteStr(destFileName) );
				f_Action->lbl_ProgressInfo->Caption = logMsg;
				if (PGlobals->LogFullAction) _log(logMsg + "...");
				lastError = _doCopyFile(srcFileName, destFileName);
				break;

			case afmMove:
				logMsg = ASPF(_("Moving [%d/%d]: %s --> %s"),
                    lineCount, f_Action->abtFileList->incFilesTotalCount,
                    ecc::QuoteStr(srcFileName), ecc::QuoteStr(destFileName) );
				f_Action->lbl_ProgressInfo->Caption = logMsg;
				if (PGlobals->LogFullAction) _log(logMsg + "...");
				if (FileExists(destFileName) && (PProfile->config.Actions.File->bOverwrite->value == false))
				{
					if (!PGlobals->LogFullAction) _log(logMsg + "...");
					_log(ecc::smtNotif, _("Nothing done.") + _("Destination file exists."), -1);
					continue;
				}
				else
					lastError = _doMoveFile(srcFileName, destFileName);
				break;

			case afmDelete:
				logMsg = ASPF(_("Deleting [%d/%d]: %s"),
                    lineCount, f_Action->abtFileList->incFilesTotalCount,
                    ecc::QuoteStr(srcFileName) );
				f_Action->lbl_ProgressInfo->Caption = logMsg;
				if (PGlobals->LogFullAction) _log(logMsg + "...");
				lastError = _doDeleteFile(srcFileName);
				break;
		}

		if (lastError == ERROR_SUCCESS)
		{
			if (PGlobals->LogFullAction) _log(_("Ok."), -1);
			++successFileCount;
		}
		else
		{
            f_Action->abtFileList->errorItemList->Append(srcFileName);
			if (!PGlobals->LogFullAction) _log(logMsg + "...");
            _log(ecc::smtError, _("Failed.") + ASPF(" [SYS_ERR %d]: %s",
                lastError, abtGetLastErrorStr(lastError)), -1);
			if ( (lastError == ERROR_ALREADY_EXISTS) || (lastError == ERROR_ACCESS_DENIED) )
			{
            	_log(ecc::smtError, _("Is destination file Read-Only/Hidden?"));
                _log(ecc::smtError, _("Is source or destination file in use by another application?"));
            }
		}
	}

	// End of File-By-File Operation:
	_log(ecc::smtNotif, ASPF(_("Operation performed successfully on %d/%d files."),
        successFileCount, f_Action->abtFileList->incFilesTotalCount ));
}
//---------------------------------------------------------------------------

/* TODO 1 : 095b1: TEST: Deal with copy/move(/delete?) empty directories: */

void TActionFileOperation::_doOperation_EmptyDirs()
{
    _log(ecc::smtDefaultB, _("Empty Directories:"), 1);
    if (!PProfile->config.Action->bIncludeEmptySourceDirectories->value)
    {
        _log(_("Disabled."));
        return;
    }

    if (f_Action->abtFileList->emptyDirList->Count == 0)
    {
        _log(_("None found."));
        return;
    }

    int azp = (TActZipStorePath)PProfile->config.Actions.File->ePathInformation->getIndex();

    if (azp == azpNone)
    {
        _log(_("Disabled if 'Path Information' is 'None'."));
        return;
    }

    bool rootSrcDirFound;
    String rootSrcDir;
    String emptyDir;
    TActFileMethod method = (TActFileMethod)PProfile->config.Actions.File->eMethod->getIndex();

    for (int i = 0; i < f_Action->abtFileList->emptyDirList->Count; i++)
    {
        String src_emptyDir = ExcludeTrailingBackslash(f_Action->abtFileList->emptyDirList->Strings[i]);

        // Strip filedrive (either 'C\' or '\\<servername>\<sharename>') from src_EmptyDir:
        emptyDir = src_emptyDir.SubString(ExtractFileDrive(src_emptyDir).Length() + 1, src_emptyDir.Length() );

        switch (azp)
        {
            case azpNone:
                // Should never happen!
                // - Empty Directories can not be used if 'Path Information' is 'None'.
                return;

            case azpRelative:
                rootSrcDirFound = false;
                for (unsigned int i = 0; i < PProfile->sourceRootDirList.size(); i++)
                {
                    rootSrcDir = PProfile->sourceRootDirList[i];
                    if (src_emptyDir.Pos(rootSrcDir) == 1)
                    {
                        rootSrcDirFound = true;
                        break;
                    }
                }
                #ifdef ABT_TEST
                    if (!rootSrcDirFound) // Should never happen!
                    {
                        f_Action->logError(__FUNC__": ROOT NOT FOUND!? Please report.");
                    }
                #endif
                emptyDir = ExcludeTrailingBackslash(_destinationDir)
                    + src_emptyDir.SubString(rootSrcDir.Length(), src_emptyDir.Length()); // Strip drive+rootdir
                break;

            default:
            case azpFull:
                emptyDir = ExcludeTrailingBackslash(_destinationDir) + emptyDir;
                break;
        }

        // == Actually copy/move/delete the empty directories:
        /* TODO 1 : if (method == afmMove) then delete the original empty dir.
            Maybe use Tf_Action::_removeEmptyDirs() to do this? */

        // Perform chosen Empty Directory Operation:
    	switch (method)
		{
			case afmCopy:
			case afmMove:
                _log(ASPF(_("Creating Empty Directory [%d/%d]: '%s'..."),
                    i + 1, f_Action->abtFileList->emptyDirList->Count, emptyDir));
                if (DirectoryExists(emptyDir))
                    _log(_("Exists."), -1);
                else
                {
                    if (ForceDirectories(emptyDir))
                        _log(_("Ok."), -1);
                    else
                        _log(ecc::smtError, _("Failed."), -1);
                }
                break;

			case afmDelete:
                _log(ASPF(_("Deleting Empty Directory [%d/%d]: '%s'..."),
                    i + 1, f_Action->abtFileList->emptyDirList->Count, src_emptyDir));
                // Windows function RemoveDirectory() deletes empty directories only:
                if (RemoveDirectory(src_emptyDir.c_str()) != 0)
                    _log(_("Ok."), -1);
                else
                    _log(ecc::smtError, _("Failed."), -1);
                break;

            default:
                break;
        }
    }
}
//---------------------------------------------------------------------------

DWORD TActionFileOperation::_doCopyFile(String src, String dest)
{
/* 	Note: CopyFileEx and CopyProgressRoutine do not work on Win95/98/Me.
	If CopyFileExA does not exists CopyFile is used (provides no progress callback). */

	if (!_testCreateDir(ExtractFilePath(dest)))
		return -1;

    // Should ReadOnly destination files be overwritten? (271+)
    bool disabledReadOnly = false;
    int attr;
    if ((PProfile->config.Actions.File->bOverwriteReadOnly->value)
        && FileExists(dest))
    {
        // Test if destination file is ReadOnly:
        attr = FileGetAttr(dest);
        if (attr & faReadOnly)
        {
            // Temporarily remove R attribute:
            if (FileSetAttr(dest, attr & !faReadOnly) != 0)
                _log("[" __FUNC__ + _("Could not clear R attribute") + "]");
            else
                disabledReadOnly = true;
        }
    }

	bool success;
	if (_pCopyFileEx)
	{	// -- Use CopyFileEx:
		success = _pCopyFileEx(	// returns ZERO (false) on failure.
			src.c_str(), 		// pointer to name of an existing file
			dest.c_str(),		// pointer to filename to copy to
			CopyProgress_Callback, // pointer to the callback function
			this, 				// to be passed to the callback function
			&_copyFileExCancel,	// flag that can be used to cancel the operation
			(PProfile->config.Actions.File->bOverwrite->value ? NULL : COPY_FILE_FAIL_IF_EXISTS)	//DWORD dwCopyFlags	// flags that specify how the file is copied
		);
	}
	else
	{	// -- Use CopyFile:
		success = CopyFile(		// returns ZERO (false) on failure.
			src.c_str(), 		// pointer to name of an existing file
			dest.c_str(),		// pointer to filename to copy to
			!PProfile->config.Actions.File->bOverwrite->value	// bFailIfExists, flag for operation if file exists
		);
	}

    if (disabledReadOnly)
    {
        // Restore ReadOnly attribute of destination file:
        if (FileSetAttr(dest, attr | faReadOnly) != 0)
            _log("[" __FUNC__ + _("Could not re-set R attribute") + "]");
    }

	if (success)
		return ERROR_SUCCESS; // All is well.
	else
		return GetLastError();
}
//---------------------------------------------------------------------------

DWORD TActionFileOperation::_doMoveFile(String src, String dest)
{
/* 	Using MoveFileWithProgress can only be used with WinXP or Win2000Pro.
	On other systems MoveFile is used. */

	if (!_testCreateDir(ExtractFilePath(dest)))
		return -1;

	bool success;
	if (_pMoveFileWithProgress)
	{
		// == MoveFileWithProgress ==
		success = _pMoveFileWithProgress(
			src.c_str(),			//LPCTSTR lpExistingFileName,
			dest.c_str(),			//LPCTSTR lpNewFileName,
			CopyProgress_Callback, 	//LPPROGRESS_ROUTINE lpProgressRoutine,
			this,					//LPVOID lpData,
			(PProfile->config.Actions.File->bOverwrite->value ? MOVEFILE_REPLACE_EXISTING : 0)
				+ MOVEFILE_COPY_ALLOWED + MOVEFILE_WRITE_THROUGH //DWORD dwFlags
			);
	}
	else
	{
		// == MoveFile ==
		/* 	If [dest] exists MoveFile will fail,
			so, if (overwrite==true), then [dest] should be deleted before moving. */
		bool doMove = true;
		if (FileExists(dest) && (PProfile->config.Actions.File->bOverwrite->value == true))
		{
			if (!DeleteFile(dest.c_str())) // Call WinAPI's DeleteFile() to generate a LastError code.
			{
				_log(ecc::smtError,
                    ASPF(_("Could not delete existing destination file: "), ecc::QuoteStr(dest)), -1);
				doMove = false;
				success = false;
			}
		}
		if (doMove)
		{
			success = MoveFile(
				src.c_str(),	//LPCTSTR lpExistingFileName,	// address of name of the existing file
				dest.c_str()	//LPCTSTR lpNewFileName,	// address of new name for the file
				);
		}
	}

	if (success)
		return ERROR_SUCCESS; // All is well.
	else
		return GetLastError();
}
//---------------------------------------------------------------------------

DWORD TActionFileOperation::_doDeleteFile(String src)
{
	bool success;
	if (PProfile->config.Actions.File->bUseRecycleBin->value)
		success =  ecc::FileToRecycleBin(src);
	else
		success = DeleteFile(src);

	if (success)
		return ERROR_SUCCESS; // All is well.
	else
		return -1;
}
//---------------------------------------------------------------------------

bool TActionFileOperation::_testCreateDir(String dir)
{
	if (!DirectoryExists(dir))
	{
		if (!ForceDirectories(dir))
		{
			_log(ecc::smtError, ASPF(_("Could not create directory: %s"), ecc::QuoteStr(dir)));
			return false;
		}
	}
	return true;
}
//---------------------------------------------------------------------------

void TActionFileOperation::_log(const String msg, int lines_before, int lines_after)
{
	f_Action->log(msg, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void TActionFileOperation::_log(const ecc::TLBMessageStyle style, const String msg,
		int lines_before, int lines_after)
{
	f_Action->doLog(style, msg, lines_before, lines_after);
}
//---------------------------------------------------------------------------

