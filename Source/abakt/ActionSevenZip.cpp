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

#include "ActionSevenZip.h"
#include "Action.h"
#include "Main.h"
#include "AbaktProfile.h"
#include "AbaktGlobals.h"
#include "GlobalFuncs.h"
#include "eccString.h"
#include "eccFile.h"
#include "eccMisc.h"
#include "eccFileVersionInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

static HANDLE hChildProcess;        // 7z(a).exe console process.
static int __compressingFileCount;  // The nr of the file that 7z is compressing.
DWORD WINAPI _readOutputThread(LPVOID hPipeRead);   // Thread that captures output.
static Tf_Action* FORMACTION = NULL;                // Pointer to f_Action, used in thread.
static bool __threadIsRunning;
//---------------------------------------------------------------------------

String TActionSevenZip::_build7zCommandLine(const bool maskPassword)
{
    // Shortcut for readability:
    abakt::TConfigSet_Act7Zip* pc7z = PProfile->config.Actions.SevenZip;

    // command:
    String command = pc7z->tCommand->value;

    // archive type (-t):
    String type = "-t"
        //+ Act7ZipArchiveTypeToStr((TAct7ZipArchiveType)pc7z->eArchiveType->getIndex());
        + pc7z->tArchiveType->value;

    // destination archive path = destdir+name+ext:
    String destName = abtFormatText(PProfile->config.Destination->tNameTemplate->value);
    String destExt = PProfile->config.Destination->bAppendExtension->value
        ? String("." + pc7z->tArchiveType->value)
        : String("");
    String destPath = _getDestDir() + destName + destExt;
    _destFileName = destPath;

    // password (-p):
    String password;
    if (!pc7z->tPassword->value.IsEmpty())
    {
        if (maskPassword) password = password.StringOfChar('*', pc7z->tPassword->value.Length());
        else password = pc7z->tPassword->value;
        password = "-p\"" + password + "\""; // 092b8: added quotes
    }

    // working dir (-w):
    String workDir = PGlobals->getTempDir().IsEmpty()
        ? String("") : String("-w" + ecc::QuoteStr(PGlobals->getTempDir()) );

    // => Complete command line:
    String line = pc7z->tCommandLineTemplate->value;
    line = ecc::ReplaceStr(line, "$$", "$");
    line = ecc::ReplaceStr(line, "$C", command);
    line = ecc::ReplaceStr(line, "$T", type);
    line = ecc::ReplaceStr(line, "$N", ecc::QuoteStr(destPath));
    line = ecc::ReplaceStr(line, "$P", password);
    line = ecc::ReplaceStr(line, "$W", workDir);
    line = ecc::ReplaceStr(line, "$L", "@" + ecc::QuoteStr(_fileListFile)); // @listfile
    return line;
}
//---------------------------------------------------------------------------


//===========================================================================
//  TActionSevenZip
//===========================================================================

//---------------------------------------------------------------------------

bool TActionSevenZip::init()
{
	_error          = false;
	_aborted        = false;
	_inited         = false;
    __compressingFileCount = 0;
	_destFileName   = "";
    _fileListFile = _getDestDir() + f_Action->getDestName() + "~.txt";

    // == Test of 7-Zip executable exists:
	if (!FileExists(PGlobals->ExePath7Zip))
	{
		f_Action->logError(_("Could not find 7-Zip executable.")
            + " " + ASPF(_("File does not exist: %s"), ecc::AposStr(PGlobals->ExePath7Zip)) );
		_error = true;
        return false;
	}

	// == Display Destination Drive Information:
    String destDir = f_Action->getDestDir();
    if (!f_Action->testCreateDestDir(destDir))
    {
        // If dest-dir does not exist and/or will not be auto-created, set _error to true:
        _error = true;
        return false;
    }
    // Added 092rc1:
 	f_Action->log( ASPF(_("Destination Drive: %s"), ecc::QuoteStr(ExtractFileDrive(destDir))) );

	bool destDriveIsRemovable = ecc::DriveIsRemovable(destDir);
    f_Action->log( ASPF("- " + _("Removable: %s"),
            destDriveIsRemovable ? String(_("No")) : String(_("Yes")) ) );

    unsigned int dtype = ::GetDriveType( ExtractFileDrive(destDir).c_str() );
    if (dtype == DRIVE_CDROM)
        f_Action->log( "- " + _("Type: CD/DVD") );

	String diskFreeSpaceStr;
	__int64 diskFreeSpace;
	ecc::GetDriveFreeSpace(destDir, diskFreeSpaceStr, diskFreeSpace);
    f_Action->log( ASPF("- " + _("Free Space: %s (%s Bytes)"),
            ecc::FloatToHumanFileSize(diskFreeSpace), diskFreeSpaceStr) );

    // == Display Temporary Directory and it's free space:
    // Added 092rc1:
	ecc::GetDriveFreeSpace(PGlobals->getTempDir(), diskFreeSpaceStr, diskFreeSpace);
 	f_Action->log( ASPF(_("Temporary Directory: %s"),
            ecc::QuoteStr(PGlobals->getTempDir()) ) );
 	f_Action->log( ASPF("- " + _("Free Space: %s (%s Bytes)"),
            ecc::FloatToHumanFileSize(diskFreeSpace), diskFreeSpaceStr) );


 	if (!_error)
	{
        _commandLine    = _build7zCommandLine(false);
        _commandLineGui = _build7zCommandLine(true);
        //  Note: _destFileName is set in function _build7zCommandLine() and
        //  its value will be used throughout the 7-Zip action.
		if (_destFileName.IsEmpty())
		{
			f_Action->logError(_("Destination archive has no name."));
			_error = true;
		}
	}

	if (!_error)
	{
        // == Display password usage:
        f_Action->log(ASPF(_("Using Password: %s"),
                PProfile->config.Actions.SevenZip->tPassword->value.IsEmpty()
                    ? String(_("No")) : String(_("Yes")) ) );
	}

 	if (!_error)
	{
        f_Action->log(_("Calling 7-Zip with command line:"));
        f_Action->log( _commandLineGui );
    }

	_inited = true;
	return !(_error || _aborted);
}
//---------------------------------------------------------------------------

bool TActionSevenZip::execute()
{
	if (!_inited) return false;
	if (_error || _aborted) return false;

	f_Action->logB(_("Note:"), 1);
	f_Action->log(_("7-Zip requires the source files to be on one drive."));
	f_Action->log(" " + _("The 7-Zip executable will be called for every drive."), -1);

	String drive, prevDrive, file;

	// Call 7z.exe for every drive:

	std::ifstream ifs(f_Action->abtFileList->incFileName.c_str());
	if (!ifs)
	{
		f_Action->logError(ASPF(_("Could not open file: %s"),
                ecc::QuoteStr(f_Action->abtFileList->incFileName)) );
		_error = true;
		return false;
	}

	drive = prevDrive = "";

	_deleteFileListFile();
	int filesLeft = 0;
	char line[2048];

	while (ifs.getline(line, 2048))
	{
		// Test for reading more than 2048 chars or another error:
		if (ifs.fail())
		{
			f_Action->logError(__FUNC__ ": [Error while reading stream]");
			_error = true;
			break;
		}
		file = String(line);

        /* TODO 3 : drive should become (optionable) StorePath (like Zip) */
		drive = IncludeTrailingPathDelimiter(ExtractFileDrive(file));

        /* 093b1 (b.324): use OEM or UTF-8 to encode filenames in 7z listfile: */
        if (PGlobals->OEMlistfile7Zip)
            CharToOem(file.c_str(), file.c_str());
        else
            file = AnsiToUtf8(file);    // UTF-8 is default encoding in releases NEWER than 7-Zip 4.32.

		file = file.Delete(1, drive.Length());
		if (drive != prevDrive)
		{
			if (filesLeft)
				_call7ZipExe(prevDrive);    // Tests and sets _error and _aborted.
			filesLeft = 0;
			if (_error || _aborted)
				break;
			prevDrive = drive;
		}
		abtAppendStrToFile(ecc::QuoteStr(file), _fileListFile, false);
		++filesLeft;
	}
	// Do the remains:
	if (filesLeft)
		_call7ZipExe(prevDrive);    // Tests and sets _error and _aborted.
	_inited = false;    // Reset.

    // Display 7-Zip archive details:
    if ((!_aborted) && (!_error))
    {
        TFileProperties fp(_destFileName);
        if (!fp.failed())
        {
		    f_Action->logB(_("7-Zip Archive Details:"), 1);
            f_Action->log(ASPF(_("Compressed Size: %s (%s Bytes)"),
                    ecc::FloatToHumanFileSize(fp.getSize()), IntToStr(fp.getSize())) );
        }
    }

	if (_error || _aborted)
		_deleteFileListFile();

	return !(_error || _aborted);
}
//---------------------------------------------------------------------------

//===========================================================================
//  Spawned console process and redirection functions
//===========================================================================

//---------------------------------------------------------------------------

/*  See "How To Spawn Console Processes with Redirected Standard Handles"
    on http://support.microsoft.com/default.aspx?scid=kb;en-us;190351
    for more information. */

void TActionSevenZip::_call7ZipExe(String drive)
{
    FORMACTION = f_Action;

	if (_error || _aborted) return;

	// Requires _destFileName(?) and _fileListFile.
 	if (!FileExists(_fileListFile))
	{
		_error = true;
		f_Action->logError(__FUNC__ ": "
            + ASPF(_("Could not find temporary filelist: %s"),
                ecc::QuoteStr(_fileListFile)) );
		return;
	}

    f_Action->logB( ASPF(_("Calling 7-Zip executable for drive: %s"), ecc::QuoteStr(drive)), 1);
    f_Action->log( ASPF(_("Using temporary filelist (%s): %s"),
        ( PGlobals->OEMlistfile7Zip ? String("OEM") : String("UTF-8") ), ecc::QuoteStr(_fileListFile)) );
    hChildProcess = NULL;

    HANDLE hOutputReadTmp, hOutputRead, hOutputWrite;
    HANDLE hErrorWrite;
    SECURITY_ATTRIBUTES sa;

    // Set up the security attributes struct.
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    // Create the child output pipe.
    if (!CreatePipe(&hOutputReadTmp, &hOutputWrite, &sa, 0))
        _handleError(__FUNC__ " [CreatePipe1]");

    // Create a duplicate of the output write handle for the std error
    // write handle. This is necessary in case the child application
    // closes one of its std output handles.
    if (!DuplicateHandle(GetCurrentProcess(), hOutputWrite,
            GetCurrentProcess(), &hErrorWrite, 0,
            TRUE, DUPLICATE_SAME_ACCESS)
        ) _handleError(__FUNC__ " [DuplicateHandle0]");

    // Create new output read handle and the input write handles. Set
    // the Properties to FALSE. Otherwise, the child inherits the
    // properties and, as a result, non-closeable handles to the pipes
    // are created.
    if (!DuplicateHandle(GetCurrentProcess(), hOutputReadTmp,
            GetCurrentProcess(),
            &hOutputRead, // Address of new handle.
            0, FALSE, // Make it uninheritable.
            DUPLICATE_SAME_ACCESS)
        ) _handleError(__FUNC__ " [DuplicateHandle1]");

    // Close inheritable copies of the handles you do not want to be inherited:
    if (!CloseHandle(hOutputReadTmp)) _handleError(__FUNC__ " [CloseHandle1]");

	// Start 7-Zip console process:
    _prepAndLaunchRedirectedChild(drive, hOutputWrite, hErrorWrite);

    // Close pipe handles (do not continue to modify the parent).
    // You need to make sure that no handles to the write end of the
    // output pipe are maintained in this process or else the pipe will
    // not close when the child process exits and the ReadFile will hang.
    if (!CloseHandle(hOutputWrite)) _handleError(__FUNC__ " [CloseHandle3]");
    if (!CloseHandle(hErrorWrite)) _handleError(__FUNC__ " [CloseHandle5]");

    // Launch the thread that reads the child's output:
    HANDLE hThread;
    DWORD ThreadId;
    __threadIsRunning = true;
    hThread = CreateThread(NULL, 0, _readOutputThread, (LPVOID)hOutputRead, 0, &ThreadId);
    if (hThread == NULL)
    {
        __threadIsRunning = false;
        _handleError(__FUNC__ " [Could not create thread]");
        _error = true;
    }

    while (__threadIsRunning)
    {
        if (_aborted)
        {
            f_Action->logWarn(_("Terminating 7-Zip process..."), 1);
            Application->ProcessMessages();
            if (TerminateProcess(hChildProcess, 0) != 0)
                f_Action->logWarn(_("Ok."), -1, 1);
            else
            {
                f_Action->logWarn(_("Failed.") + " "
                    + abtGetLastErrorStr(), -1, 1);
            }
            break;
        }

        // Prevent Abakt from using all the CPU cycles by calling Sleep():
        Sleep(2); // Sleep for 0.002 sec, this will give the 7z.exe process more CPU cycles.
        // Notify user that Abakt is busy:
        f_Action->lbl_ProgressInfo->Caption =
            ASPF(_("File %d/%d [%s]"),
                __compressingFileCount, FORMACTION->abtFileList->incFilesTotalCount,
                f_Action->timeLapse.AsStr() );
        Application->ProcessMessages();
    }

    if (!CloseHandle(hOutputRead))
        _handleError(__FUNC__ " [CloseHandle7]");

    // Tell the thread to exit and wait for thread to die:
    if (WaitForSingleObject(hThread, 1000) == WAIT_FAILED)
        _handleError(__FUNC__ " [WaitForSingleObject]");

    _deleteFileListFile();

	f_Action->logNotif( ASPF(_("Elapsed Time: %s"), TranslateHMS(f_Action->timeLapse.AsString())) );
}
//---------------------------------------------------------------------------

void TActionSevenZip::_prepAndLaunchRedirectedChild(String drive,
        HANDLE hChildStdOut, HANDLE hChildStdErr)
{
	f_Action->log(_("COMMAND:"));
	f_Action->log( ecc::QuoteStr(PGlobals->ExePath7Zip) + " " + _commandLineGui );
	String cmdLine = ecc::QuoteStr(PGlobals->ExePath7Zip) + " " + _commandLine;

    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE; //SW_SHOW;
    si.hStdOutput = hChildStdOut;
    si.hStdError  = hChildStdErr;

	// Create the child process:
	BOOL bFuncRetn = CreateProcess(
        NULL,
        cmdLine.c_str(),    // command line
        NULL,          	    // process security attributes
        NULL,          	    // primary thread security attributes
        TRUE,         	    // fInheritHandles: handles are inherited
        CREATE_NEW_CONSOLE, // creation flags
        NULL,          	    // use parent's environment
        drive.c_str(),	    // lpCurrentDirectory - Required for 7z(a).exe.
        &si,	            // STARTUPINFO pointer
        &pi	  	            // receives PROCESS_INFORMATION
		);

	if (bFuncRetn == NULL)
	{
		f_Action->logError(__FUNC__ ": CreateProcess() failed: " + abtGetLastErrorStr());
	}

    // Set global child process handle to cause threads to exit.
    hChildProcess = pi.hProcess;

    // Close any unnecessary handles.
    if (!CloseHandle(pi.hThread)) _handleError(__FUNC__ " [CloseHandle9]");
}
//---------------------------------------------------------------------------

DWORD WINAPI _readOutputThread(LPVOID hPipeRead)
// Monitors handle for input. Exits when child exits or pipe breaks.
{
    String str, substr;
    int bsCount = 0;

    CHAR lpBuffer[128];
    DWORD nBytesRead;

    FORMACTION->logB("< --- " + _("Reading Output") + " - " + _("Please wait") + " --- >", 1);
    Application->ProcessMessages();

    while (1)
    {
//      if (_aborted) break;
        if ( !ReadFile(hPipeRead, lpBuffer, sizeof(lpBuffer), &nBytesRead, NULL) )
        {
            if (GetLastError() == ERROR_BROKEN_PIPE)
                break; // pipe done - normal exit path.
            else
            {
                FORMACTION->log(__FUNC__ ": Something bad happened, please report.");
                Application->ProcessMessages();
                break;
            }
        }

        str = str + AnsiString(lpBuffer, nBytesRead);
        str = ecc::ReplaceStr(str, "\r", "");

        // Eat "\b" and the character before "\b":
        int pos = str.Pos("\b");
        bool percCharFound = false;
        String perc = "";
        while (pos && (pos > 1))
        {
            ++bsCount;
            perc = str.SubString(pos - 1, 1) + perc; // Append char in reverse.
            if (bsCount == 4)
            {
                if (perc.Pos("%"))  // If the line is too long perc does not contain a '%'.
                {
                    percCharFound = true;
                    FORMACTION->lbl_ProfilePerc->Caption = perc.Trim();
                    FORMACTION->pb_Profile->Position = StrToIntDef(ecc::ReplaceStr(perc.Trim(), "%", ""), 0);
                    FORMACTION->updateAppTitlePerc();   //092b8
                    Application->ProcessMessages();
                }
                perc = "";
                bsCount = 0;
            }
            str.Delete(pos - 1, 2);
            pos = str.Pos("\b");
        }

        // Split lines:
        pos = str.Pos("\n");
        while (pos)
        {
            substr = str.SubString(1, pos - 1);
            bool suppressLog = false;       //093b1

            // If string starts with "Compressing" calculate and display progress percentage:
            if (substr.Pos("Compressing") == 1)
            {
                if (!PGlobals->LogFullAction)
                    suppressLog = true;     //093b1: Do not log succesful "Compressing..." messages.
                ++__compressingFileCount;
                float perc = ((float)__compressingFileCount / (float)FORMACTION->abtFileList->incFilesTotalCount) * 100;
                if (!percCharFound) { // Since 7z 4.19b the '%' character is not present in the captured output.
                    FORMACTION->lbl_ProfilePerc->Caption = IntToStr((int)perc) + "%";
                    FORMACTION->pb_Profile->Position = (int)perc;
                    FORMACTION->updateAppTitlePerc();   //092b8
                }
            }
            if (!suppressLog)                       //093b1
                FORMACTION->log(substr);            //093b1

            Application->ProcessMessages();
            str.Delete(1, pos);
            pos = str.Pos("\n");
        }
    }

    FORMACTION->logB("< --- " + _("Finished Reading Output") + " --- >", 1);
    Application->ProcessMessages();
    __threadIsRunning = false;
    return 1;
}
//---------------------------------------------------------------------------

void TActionSevenZip::_handleError(String msg)
// Displays [msg], the error number and corresponding message.
{
    DWORD errCode = GetLastError();
    f_Action->logError(ASPF("%s SYS_ERR[%d]: %s",
        msg, errCode, abtGetLastErrorStr(errCode)) );
    _error = true;
}
//---------------------------------------------------------------------------

void TActionSevenZip::_deleteFileListFile()
{
	if (!FileExists(_fileListFile)) return;

	if (!DeleteFile(_fileListFile))
		f_Action->logError( ASPF(_("Could not delete temporary filelist file: %s"),
             ecc::QuoteStr(_fileListFile)) );
}
//---------------------------------------------------------------------------

String TActionSevenZip::_getDestDir()
{
    // If a quote (') is found in destdir, it should be formatted:
    if (PProfile->config.Destination->pDir->value.Pos("'"))
        return abtFormatText(PProfile->config.Destination->pDir->value);
    else
        return PProfile->config.Destination->pDir->value;
}
//---------------------------------------------------------------------------

