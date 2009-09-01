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

#include "Action.h"
#include "Main.h"
#include "ActionFileOperation.h"
#include "ActionSevenZip.h"
#include "GlobalFuncs.h"
#include "AbaktGlobals.h"
#include "AbaktFilter.h"
#include "AbaktGroup.h"
#include "AbaktProfile.h"
#include "AbaktHistory.h"
#include "Gui_X_Profile.h"
#include "eccFile.h"
#include "eccMisc.h"
#include "eccString.h"
#include "ZBCtx.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ZipBuilder"
#pragma resource "*.dfm"
Tf_Action *f_Action;
//---------------------------------------------------------------------------

/* === Notes:
    "ZBCtx.h" is #included to make use of DHC_xxx codes in ZipBuilder's event OnZipDialog.
*/
//---------------------------------------------------------------------------

__fastcall Tf_Action::Tf_Action(TComponent* Owner)
	: TForm(Owner)
{
	ed_ProfilePath->Text    	= "";
	ed_Destination->Text        = "";
	ed_Group->Text 			    = "";
	pnl_GroupCount->Caption 	= "";
	lbl_ProgressInfo->Caption   = "";
	sdlg_Log->InitialDir	    = PGlobals->ActionLogDir;

    // Make ZipBuilder's version info globally available (used in f_About).
    PGlobals->ZipBuilderVersionStr = ZipBuilder->VersionInfo;

	_busy 	   	= false;
	actionStyle	= asUndefined;
	_skipFileLogging = false;   //093b1(JB1)

	abtFileList = new TAbaktFileList();     // Should be destroyed in ::FormDestroy().
	abtFileList->OnCurrentFile = OnCurrentFile;

	zipFileList = new TStringList();        // Should be destroyed in ::FormDestroy().
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TP_GlobalIgnoreClass(__classid(TZipBuilder));   // Prevents gnugettext from interfering with TZipBuilder.
        TranslateComponent(this);
    #endif

	_ORG_Caption = Caption; // Set it here to make sure the title can be translated (092RC2).

}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::FormShow(TObject *Sender)
{
	sdlg_Log->InitialDir = PGlobals->ActionLogDir;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::FormDestroy(TObject *Sender)
{
	delete abtFileList;
	delete zipFileList;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::FormClose(TObject *Sender, TCloseAction &Action)
{
	// TFormMemory settings are saved in f_Main.

    // Setting actionStyle before showing f_Action is mandatory:
	actionStyle	= asUndefined;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	CanClose = !_busy;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::bt_CloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::FormActivate(TObject *Sender)
{
    // Execute the Action:
	_start();

    // Reset actionStyle:
	actionStyle = asUndefined;
}
//---------------------------------------------------------------------------

String Tf_Action::getDestName()
{
    return abtFormatText(PProfile->config.Destination->tNameTemplate->value);
}
//---------------------------------------------------------------------------

String Tf_Action::getDestDir()
{
    // If a quote (') is found in destdir, it should be formatted:
/* TODO 3 : uncomment the following when destdir-formatting is enabled: */
/*  if (PProfile->config.Destination->pDir->value.Pos("'"))
        return abtFormatText(PProfile->config.Destination->pDir->value);
    else */
        return PProfile->config.Destination->pDir->value;
}
//---------------------------------------------------------------------------

bool Tf_Action::testCreateDestDir(String destDir)
{
	// Don't create dir when File Operation = Delete:
	TActionType act = (TActionType)PProfile->config.Action->eType->getIndex();
	TActFileMethod afm = (TActFileMethod)PProfile->config.Actions.File->eMethod->getIndex();
	if ((act == actFile) && (afm == afmDelete))
		return true;

	if (destDir.IsEmpty())
	{
		logError(_("Destination Directory can not be empty."));
		return false;
	}
	if (!DirectoryExists(destDir))
	{
		logNotif( ASPF(_("Destination Directory does not exist: %s"), ecc::QuoteStr(destDir)) );
		if (PProfile->config.Destination->bAutoCreateDir->value)
		{
			logNotif( ASPF(_("Creating Destination Directory %s..."), ecc::QuoteStr(destDir)) );
			if (!ForceDirectories(destDir))
			{
				logError( _("Failed.") , -1);
				return false;
			}
			else
				logNotif( _("Ok.") , -1);
		}
		else
		{
			logError("", -1);
			logError(_("Destination Directory will not be automatically created."), 0, 1);
			return false;
		}
	}
	return true;
}
//---------------------------------------------------------------------------

void Tf_Action::_start()
{
	// asViewLog:
	if (actionStyle == asViewLog)
	{
		gbx_Progress->Visible	= false;
		bt_Close->Enabled 		= true;
		ActiveControl			= bt_Close;
		return;
	}

	// asSingle, asGroup:
    log("Abakt " + PGlobals->humanVersionStr(), 0, 1);

	ecc::HourGlass(true);
	pnl_GroupCount->Caption	    = "";
	ed_Group->Text		    	= "";
	gbx_Progress->Visible   	= true;
	bt_Abort->Enabled 	    	= true;
	bt_Close->Enabled 	    	= false;
	bt_SaveLog->Enabled      	= false;
	ActiveControl 		    	= bt_Abort;
	pb_Group->Position 	    	= 0;
	_profileCount 		    	= 1;
	_profileNr 			    	= 1;
	lbl_GroupPerc->Caption  	= "";
	pnl_GroupElapsedTime->Caption = "-:--";
    Application->Title          = "Abakt";  // 092b8: Reset to default caption without percentage indicators.
	lb_Log->Clear();
	_tempCopyDestFile	    	= "";
    _actTestingZipArchive       = false;
	_aborted	= false;
	_error		= false;

	try
	{
		if (actionStyle == asSingle)
		{
			_doAction();
		}
		if (actionStyle == asGroup)
		{
			_doGroupAction();
		}
	}
	catch (Exception &ex)
	{
		doLog(ecc::smtAppError, "Exception in " __FUNC__ ": " + ex.Message);
		_endActionZip();
	}

	// End:
    Application->Title      = "Abakt";  // 092b8: Reset to default caption without percentage indicators.
	actionStyle 			= asUndefined; // Reset.
	gbx_Progress->Visible	= false;
	bt_Abort->Enabled 		= false;
	bt_Close->Enabled 		= true;
	bt_SaveLog->Enabled     = true;
	ActiveControl 			= bt_Close;
	ecc::HourGlass(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::tmr_ActionTimer(TObject *Sender)
{
	_updateElapsedTimeControls();
}
//---------------------------------------------------------------------------

void Tf_Action::_updateElapsedTimeControls()
{
	if (timeLapse.IsRunning())
		pnl_ElapsedTime->Caption = timeLapse.AsStr();
	if ( (actionStyle == asGroup) && _groupTimeLapse.IsRunning() )
		pnl_GroupElapsedTime->Caption = _groupTimeLapse.AsStr();
	Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::lb_LogDrawItem(TWinControl *Control, int Index,
	  TRect &Rect, TOwnerDrawState State)
{
	ecc::LBMessage_DrawItem(Control, Index, Rect, State);
}
//---------------------------------------------------------------------------

void Tf_Action::doLog(const ecc::TLBMessageStyle smt, const String msg,
		const int lines_before, const int lines_after)
{
	// Log to ListBox:
	try
	{
		ecc::LBMessage(lb_Log, msg, smt, lines_before, lines_after);
	}
	catch (Exception &ex)
	{
		lb_Log->Clear();
		doLog(ecc::smtAppError, "Exception in " __FUNC__ ": " + ex.Message);
		logError("Writing to the Log window failed. The Log window will be cleared.");
		logError("Log window has been cleared because its maximum size has been reached.");
		if (PGlobals->AutoSaveActionLog && !_skipFileLogging)
			logError("The Action Log file is ok.");
		logError("---");
	}

	// Log to file:
	try
	{
		if ( PGlobals->AutoSaveActionLog && !_skipFileLogging
            && !PProfile->name.IsEmpty() ) // This line added 093b1(JB1). Prevents creation of "\Log\.log" file.
		{
			abtAppendStrToFile(msg, PGlobals->ActionLogDir + PProfile->name + ".log", true);
		}
	}
	catch (Exception &ex)
	{
        _skipFileLogging = true;
		doLog(ecc::smtAppError, "Exception in " __FUNC__ ": " + ex.Message);
		logError("Logging to file failed and will be disabled for the current Action.");
	}

	Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void Tf_Action::log(const String msg, const int lines_before, const int lines_after)
{
	doLog(ecc::smtDefault, msg, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void Tf_Action::logB(const String msg, const int lines_before, const int lines_after)
{
	doLog(ecc::smtDefaultB, msg, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void Tf_Action::logNotif(const String msg, const int lines_before, const int lines_after)
{
	doLog(ecc::smtNotification, msg, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void Tf_Action::logNotifB(const String msg, const int lines_before, const int lines_after)
{
	doLog(ecc::smtNotificationB, msg, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void Tf_Action::logWarn(const String msg, const int lines_before, const int lines_after)
{
	doLog(ecc::smtWarning, msg, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void Tf_Action::logError(const String msg, const int lines_before, const int lines_after)
{
	doLog(ecc::smtError, msg, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void Tf_Action::_initAction()
{
	_busy = true;
	ed_ProfilePath->Text		= PProfile->fileName;
	pb_File->Position 			= 0;
	pb_Profile->Position		= 0;
	pnl_ElapsedTime->Caption 	= "0:00";
	lbl_FilePerc->Caption   	= "";
	lbl_ProfilePerc->Caption   	= "";

	Caption = _ORG_Caption;
	if (actionStyle == asGroup)
		Caption = Caption + " - [" + _("Group") + ": " + PGroup->name + "]";
	Caption = Caption + " - [" + _("Profile") + ": " + PProfile->name + "]";

	_skipFileLogging = false;
	if (!DirectoryExists(PGlobals->ActionLogDir))
	{
		logWarn( ASPF(_("Action Log Directory does not exist: %s"),
            ecc::QuoteStr(PGlobals->ActionLogDir)) );
		logWarn(_("Logging to file is disabled for the current Action."));
		_skipFileLogging = true;
	}
	else
	{	// Delete previous logfile:
		DeleteFile(PGlobals->ActionLogDir + PProfile->name + ".log");
	}

	timeLapse.Start();
	tmr_Action->Enabled = true;

    // ### Display summary of Profile's settings: ###

	logNotifB( ASPF(_("[ Starting Action %s ]"), ecc::QuoteStr(PProfile->name)), 0, 1);
	log( ASPF(_("Abakt Version: %s"), PGlobals->humanVersionStr()) );
	log( ASPF(_("Profile: %s"), ecc::QuoteStr(PProfile->fileName)) );

    // == Display Log File information:
    log( ASPF(_("Log File: %s"),
            PGlobals->AutoSaveActionLog
                ? String(_(ecc::QuoteStr(PGlobals->ActionLogDir + PProfile->name + ".log")))
                : String(_("Disabled"))
            ) );

    // == Display ArchiveMode information:
    log( _("Archive Mode") + ": "
        + _(PProfile->config.Action->eArchiveMode->selected()) );

    // == Display ActionType information:
	log( _("Action Type") + ": "
        + _(PProfile->config.Action->eType->selected()) );
    TActionType act = (TActionType)PProfile->config.Action->eType->getIndex();
    switch (act)
    {
        case actZip:
            log("- " + _("Method") + ": "
                + _(PProfile->config.Actions.Zip->eMethod->selected()) );
            log("- " + _("Path Information") + ": "
                + _(PProfile->config.Actions.Zip->eStorePath->selected()) );
            break;

        case actFile:
            log("- " + _("Method") + ": "
                + _(PProfile->config.Actions.File->eMethod->selected()) );
            break;

        default:
        case act7Zip:
            break;
    }
}
//---------------------------------------------------------------------------

void Tf_Action::_endAction()
{
	// Clean up abtFileList:
    abtFileList->cleanUp();

	timeLapse.Stop();
	_updateElapsedTimeControls();
	tmr_Action->Enabled = false;
	logB( ASPF(_("Elapsed Time (Profile): %s"), TranslateHMS(timeLapse.AsString())), 1);

	logB(" -- " + ASPF(_("Finished Action %s"), ecc::QuoteStr(PProfile->name) + " -- "), 1, 1);

	Application->ProcessMessages();
	_busy = false;
}
//---------------------------------------------------------------------------

void Tf_Action::_endActionZip()
{
    if (PProfile->config.Actions.Zip->bSplit->value && !_destDriveIsRemovable)
    {
        if (!(_aborted || _error))
            logNotif(_("ZipBuilder automatically renamed the last part of the Split Archive to '*.zip'."));
    }

    // == Test archive for errors:
    _testZipArchive(ZipBuilder->ZipFileName);

    if (!_aborted)
    {
        logB( _("Zip Archive Details:"), 1);
        log(ASPF(_("Files in Zip Archive: %d"), ZipBuilder->Count) );
        log(ASPF(_("Files Acted On: %d"), ZipBuilder->SuccessCnt) );
        __int64 zipFileSize = ZipBuilder->ZipFileSize;
        if (PProfile->config.Actions.Zip->bSplit->value)
        {
            log(ASPF(_("Split Zip Archive Parts: %d"), _actZipSplitVolumeCount));
            zipFileSize += ((_actZipSplitVolumeCount - 1) * PProfile->config.Actions.Zip->iSplitCustomSize->get());
        }
        if (_actZipTotalBytesActedOn > 0)
        {
            float perc = ( 100 - ((float)zipFileSize/(float)_actZipTotalBytesActedOn) * 100) + 0.5;
            log(ASPF(_("Compressed Size: %s (%s B) of %s (%s B) = %i%% of original size."),
                ecc::FloatToHumanFileSize(zipFileSize), IntToStr(zipFileSize),
                ecc::FloatToHumanFileSize(_actZipTotalBytesActedOn), IntToStr(_actZipTotalBytesActedOn),
                100 - (int)perc) );
        }
    }

	// == Delete the newly created backup if operation was aborted:
	if (_aborted)
	{
		_abortedBackup_DeleteFiles();
	}

    // == Display files or directories that had errors:
	if (abtFileList->errorItemList->Count)
	{
		logB(_("The following files or directories could not be archived:"), 1 );
		for (int i = 0; i < abtFileList->errorItemList->Count; i++)
		{
			logError(ecc::QuoteStr(abtFileList->errorItemList->Strings[i]) );
		}
	}

    // == Save FileList to Destination directory:
    if (!_aborted)
    {
        logB(_("FileList:"), 1);
        if (PProfile->config.FileList->bEnabled->value)
        {
            String filelist_fName = IncludeTrailingBackslash(getDestDir()) + getDestName() + ".txt";
            log(ASPF(_("Saving FileList to: %s"), ecc::QuoteStr(filelist_fName)) );
            zipFileList->SaveToFile(filelist_fName);
        }
        else
            log(_("Disabled") );
    }

    // == Cleanup:
    zipFileList->Clear(); // Clear no-longer used memory.
}
//---------------------------------------------------------------------------

bool Tf_Action::_initActionZip(String& destZip)
{
	_aborted   		        = false;
	_zipBuilderFileAddError	= false;

	// == Check Source Directories:
	if (PProfile->sourceRootDirList.size() == 0)
	{
		logError(_("No Source directories.") );
		return false;
	}

    // == Display compression level:
	log("- " + ASPF(_("Compression Level: %d"),
            PProfile->config.Actions.Zip->iCompression->get()) );

	// == Display password usage:
    log("- " + ASPF(_("Using Password: %s"),
        	PProfile->config.Actions.Zip->tPassword->value.IsEmpty()
                ? String(_("No")) : String(_("Yes")) ) );

	// == Display "Perform Safe ZipBuilder Operations":
    log("- " + _("Perform Safe ZipBuilder Operations") + ": "
        + (PGlobals->UseZipBuilderAddSafe ? String(_("Yes")) : String(_("No"))) );

	// == Display Destination Drive Information:
	String destDir = getDestDir();
	if (!testCreateDestDir(destDir))
	{
		// If dest-dir does not exist and/or will not be auto-created, set _error to true:
		_error = true;
        return false;
	}

 	log( ASPF(_("Destination Drive: %s"), ecc::QuoteStr(ExtractFileDrive(destDir))) );

	_destDriveIsRemovable = ecc::DriveIsRemovable(destDir);
    log( ASPF("- " + _("Removable: %s"),
            _destDriveIsRemovable ? String(_("Yes")) : String(_("No")) ) );

    unsigned int dtype = ::GetDriveType( ExtractFileDrive(destDir).c_str() );
    if (dtype == DRIVE_CDROM)
        log( "- " + _("Type: CD/DVD") );

	String diskFreeSpaceStr;
	__int64 diskFreeSpace;
	ecc::GetDriveFreeSpace(destDir, diskFreeSpaceStr, diskFreeSpace);
    log( ASPF("- " + _("Free Space: %s (%s Bytes)"),
            ecc::FloatToHumanFileSize(diskFreeSpace), diskFreeSpaceStr) );

    // == Display Temporary Directory and it's free space:
	ecc::GetDriveFreeSpace(PGlobals->getTempDir(), diskFreeSpaceStr, diskFreeSpace);
 	log( ASPF(_("Temporary Directory: %s"),
            ecc::QuoteStr(PGlobals->getTempDir()) ) );
 	log( ASPF("- " + _("Free Space: %s (%s Bytes)"),
            ecc::FloatToHumanFileSize(diskFreeSpace), diskFreeSpaceStr) );


	// == Set Split options:
	//	- Requires up-to-date _destDriveIsRemovable.
	_setArchiveZipSplitOptions();
	//	Check Split requirements:
	//	- Requires up-to-date _destDriveIsRemovable.
	//	- Requires split options to be set.
/* 092b10: Removed limitation 'can not update/freshen split archive':
	if (PProfile->config.Actions.Zip->bSplit->value)
	{
		if( (PProfile->config.Actions.Zip->eMethod->getIndex() == (int)azmUpdate)
			|| (PProfile->config.Actions.Zip->eMethod->getIndex() == (int)azmFreshen) )
		{
			logError(_("It is not possible to Update or Freshen a Split Zip Archive."));
			return false;
		}
    } */

	// == Build destination filename:
	if (getDestName().IsEmpty())
	{
		logError(_("Zip Archive's Destination is empty.") );
		return false;
	}
	destZip = destDir + getDestName();

	if (PProfile->config.Action->eType->getIndex() == (int)actZip)
	{
		String destZipExt = PProfile->config.Destination->bAppendExtension->value ? ".zip" : "";

		// If destZip exists, append " (++nr)":
		if ( FileExists(destZip + destZipExt)
			&& (PProfile->config.Actions.Zip->eMethod->getIndex() == (int)azmNew))
		{
			int nr = 2;
			String tmpDestZip = destZip + " (" + IntToStr(nr) + ")" + destZipExt;
			while (FileExists(tmpDestZip))
			{
				tmpDestZip = destZip + " (" + IntToStr(++nr) + ")" + destZipExt;
			}
			destZip = tmpDestZip;   // destZip now has an extension (if destZipExt is not empty).
            logNotif( ASPF(_("Destination Zip Archive exists. New name: %s"),
                    destZip + destZipExt) );
		}
		else
		{
			destZip = destZip + destZipExt;
		}
		ed_Destination->Text = destZip;
		log(_("Destination") + ": " + ecc::QuoteStr(destZip));

        // Warn when trying to Freshen a non-existing Zip Archive:
		if ( !FileExists(destZip)
            && (PProfile->config.Actions.Zip->eMethod->getIndex() == (int)azmFreshen) )
		{
			logWarn( ASPF(_("You chose to Freshen a non-existing Zip Archive: %s."),
                    ecc::QuoteStr(destZip)), 1);
			logWarn(_("A new Zip Archive will be created."), 0, 1);
			PProfile->config.Actions.Zip->eMethod->setIndex((int)azmNew);
		}
	}

	return true;    // == Ok.
}
//---------------------------------------------------------------------------

void Tf_Action::_doActionZip()
{
    // 092b7: Make sure the DLL in Abakt's installation directory is used:
    ZipBuilder->DLLDirectory    = ExtractFilePath(Application->ExeName);

	String destZip; // Receives the destination filename for the Zip Archive.
	if (!_initActionZip(destZip))
	{
		logError( _("Action failed."), 0, 1);
		_error = true;
		return;
	}

    #ifdef ABT_TEST
    if (ZipBuilder->Trace)
        logWarn("ZipBuilder->Trace == true!");
    #endif

	// Start Backup:
/* TODO 3 : OK? Allow errors in filelist? Should these become warnings? */
	buildFileList();	// Sets _error and _aborted.
//	if (_error || _aborted) return;
    if (abtFileList->incFilesTotalCount == 0) return;   // added 092b8
	if (_aborted) return;

	try
	{
        // Initialize some Abakt variables:
        _actTestingZipArchive   = false;

        // Initialize ZipBuilder:
        ZipBuilder->Cancel = false;     // 095b2: Required to undo a previous ZB->Cancel = true. 
//      ZipBuilder->Unattended  = true;
        //  Set to true to prevent popups from the DLL. NOTE: If false, split/span will fail!

        ZipBuilder->UseDirOnlyEntries = true;   // Required if empty directories should be included!

        ZipBuilder->Verbose     = true;
        // NOTE (092b6): If 'false' the compression ratio and the 'Ok.' messages
        // will not be displayed while logging an Action!

        ZipBuilder->AddOptions  = ZipBuilder->AddOptions << AddHiddenFiles; // Required: Abakt's filter decides if files with the H or S attribute will be included.
        ZipBuilder->AddOptions  = ZipBuilder->AddOptions << AddDirNames;     // Required!
        ZipBuilder->AddOptions  = ZipBuilder->AddOptions << AddSeparateDirs; // Required???.
        if (PGlobals->UseZipBuilderAddSafe)
            ZipBuilder->AddOptions  = ZipBuilder->AddOptions << AddSafe; // Use a temp file to perform changes.
        else
            ZipBuilder->AddOptions  = ZipBuilder->AddOptions >> AddSafe;

        // Zip Password:
		ZipBuilder->Password        = PProfile->config.Actions.Zip->tPassword->value;
        if (ZipBuilder->Password.IsEmpty())
            ZipBuilder->AddOptions  = ZipBuilder->AddOptions >> AddEncrypt;
        else
            ZipBuilder->AddOptions  = ZipBuilder->AddOptions << AddEncrypt;

        // 092rc2: Clear ZipBuilder AddStoreSuffixes:
        ZipBuilder->AddStoreSuffixes.Clear();

        ZipBuilder->ZipFileName         = destZip;
		ZipBuilder->AddCompLevel		= PProfile->config.Actions.Zip->iCompression->get();
        ZipBuilder->TempDir 			= PGlobals->getTempDir();
        ZipBuilder->RootDir             = "";

        // Start the Zip Action:
		_doZipBackupAccordingToMethod();
	}
	catch(Exception &ex)
	{
		doLog(ecc::smtAppError, "Exception in " __FUNC__ "[1]: " + ex.Message);
		_error = true;
		_endActionZip();
		return;
	}

	// End of action (verify(test), clean-up on error and display statistics):
	_endActionZip();
}
//---------------------------------------------------------------------------

void Tf_Action::_doZipBackupAccordingToMethod()
{
	if (abtFileList->incFilesTotalCount > 0xFFFF)
	{
		logError(_("You can not add more than 65535 files to a Zip archive."));
        _error = true;
        return;
	}

	// == Fill zipFileList (read filenames from disk):
    zipFileList->Clear();
	zipFileList->Capacity = abtFileList->incFilesTotalCount;
	std::ifstream ifs(abtFileList->incFileName.c_str());
	if (!ifs)
	{
		logError(ASPF(_("Could not open file: %s"), ecc::QuoteStr(abtFileList->incFileName)) );
		_error = true;
		return;
	}
	char line[2048];
	while (ifs.getline(line, 2048))
	{
        if (!ifs.fail())
            zipFileList->Append(String(line));
        else
            logError(__FUNC__ ": ifs.getline() FAILED!");
    }

    // == Test for (and remove) duplicate entries of _inside_ filenames from zipFileList:
    _validateZipFileList();

    // == Add the empty directories to zipFileList ('as if' they are files, which they are not):
    _addEmptyDirsToZipFileList();   // 092b10

	// == Copy zipFileList to ZipBuilder:
    ZipBuilder->FSpecArgs->Assign(zipFileList);
//092b6: Removed (because zipFileList is needed for Finalize::FileList) zipFileList->Clear(); // Clear no-longer-used memory.

	// == Create Zip Archive using ZipBuilder:
	String noExist = _("No existing Zip Archive found. Creating new archive.");
	_tempCopyDestFile = "";     // Used by azmReplace.
    int zipBuilderAddResult;    // Receives (fatal) error code from DLL.

	switch ((TActZipMethod)PProfile->config.Actions.Zip->eMethod->getIndex())
	{
		case azmNew:
			logB(_("Creating New Zip Archive:"), 1);
            _setActZipComment();
            ZipBuilder->AddOptions = ZipBuilder->AddOptions >> AddFreshen >> AddUpdate;
//USEFUL?   ZipBuilder->Active = true;
            zipBuilderAddResult = ZipBuilder->Add();
            if (zipBuilderAddResult != 0)
            {
                _error = true;
            }
			break;

		case azmReplace:
			logB(_("Replacing Existing Zip Archive:"), 1);
			if (!FileExists(ZipBuilder->ZipFileName))
                logNotif(noExist);
			else if (PGlobals->CreateTempZipCopy)
			{
                if (_destDriveIsRemovable)
                {
                    logNotif(_("Creating a temporary copy on a removable drive is always disabled."));
                }
                else
                {
                    // Create temporary copy of previous backup:
                    /* TODO 2 : .z## files should be temporarily copied too. */
                    _tempCopyDestFile = ZipBuilder->ZipFileName + ".~bak";
                    logNotif( ASPF(_("Creating temporary copy of existing Zip Archive: %s..."),
                        ecc::QuoteStr(_tempCopyDestFile)) );
                    if (MoveFile(ZipBuilder->ZipFileName.c_str(), _tempCopyDestFile.c_str()) == 0)
                    {
                        logError(_("Failed.") , -1);
                        logNotif(_("Adding files to existing Zip Archive instead."));
                        _tempCopyDestFile = "";
                    }
                    else
                    {
                        logNotif(_("Ok.") , -1);
                        log(_("Adding files. Please wait."));
                    }
                }
			}
            ZipBuilder->AddOptions = ZipBuilder->AddOptions >> AddFreshen >> AddUpdate;
            _setActZipComment();
//USEFUL?   ZipBuilder->Active = true;
            zipBuilderAddResult = ZipBuilder->Add();
            if (zipBuilderAddResult != 0)
            {
                _error = true;
            }
            // Delete or restore temporary copy of previous backup:
			if ( (PGlobals->CreateTempZipCopy) && (!_tempCopyDestFile.IsEmpty()) )
			{
				if (!(_error || _aborted))
				{
					logNotif( ASPF(_("Deleting temporary copy of Zip Archive: %s..."),
                        ecc::QuoteStr(_tempCopyDestFile)) );
					if (!DeleteFile(_tempCopyDestFile))
						logError(_("Failed.") , -1);
					else
					{
						logNotif(_("Ok.") , -1);
						_tempCopyDestFile = ""; // Prevent restore.
					}
				}
				// else: Temp copy will be restored. See _doActionZip().
			}
			break;

		case azmUpdate:
			logB(_("Updating Existing Zip Archive:"), 1);
			if (!FileExists(ZipBuilder->ZipFileName))
                logNotif(noExist);
            else    // Set ZipBuilder to AddUpdate:
                ZipBuilder->AddOptions = ZipBuilder->AddOptions >> AddFreshen << AddUpdate;
            _setActZipComment();
            zipBuilderAddResult = ZipBuilder->Add();
            if (zipBuilderAddResult != 0)
            {
                _error = true;
            }
			break;

		case azmFreshen:
			logB("Freshening Existing Zip Archive:", 1);
			if (!FileExists(ZipBuilder->ZipFileName))
                logNotif(noExist);
            else    // Set ZipBuilder to AddFreshen:
                ZipBuilder->AddOptions = ZipBuilder->AddOptions << AddFreshen >> AddUpdate;
            _setActZipComment();
            zipBuilderAddResult = ZipBuilder->Add();
            if (zipBuilderAddResult != 0)
            {
                _error = true;
            }
			break;

        #ifdef ABT_TEST
		default:
   			doLog(ecc::smtAppError, __FUNC__ ": TActZipMethod not recognized!");
			break;
        #endif
	}

    // Show zipBuilderAddResult code, unless it is recognized by Abakt:
    switch(zipBuilderAddResult)
    {
        case 0:     // Ok.
        case 11007: // DS_Canceled: "User cancelled operation".
            break;

        default:    // Log unparsed result codes:
            logError(ASPF(__FUNC__ ": zipBuilderAddResult=%d", zipBuilderAddResult));
    }
}
//---------------------------------------------------------------------------

void Tf_Action::_testZipArchive(const String fileName)
{
    if (_error || _aborted) return;

    logB( ASPF(_("Testing Zip Archive %s:"), ecc::QuoteStr(fileName)), 1);
    if (!PProfile->config.Action->bTestArchive->value)
    {
        log(_("Disabled."));
        return;
    }

    _actTestingZipArchive = true;
    try
    {
        ZipBuilder->ZipFileName = fileName;
        ZipBuilder->ExtrOptions.Clear();
        ZipBuilder->ExtrOptions << ExtrTest;
        ZipBuilder->FSpecArgs->Clear();
        ZipBuilder->FSpecArgs->Add("*");    // Test ALL files.
        ZipBuilder->Extract();              // Start the test.

        if (ZipBuilder->Count + ZipBuilder->DirOnlyCount == ZipBuilder->SuccessCnt)
            logNotif(ASPF(_("All %d files tested Ok."),
                    ZipBuilder->Count + ZipBuilder->DirOnlyCount) );
        else
            logError(ASPF(_("Error: %d files tested BAD, or skipped!"),
                    ZipBuilder->Count + ZipBuilder->DirOnlyCount - ZipBuilder->SuccessCnt) );
    }
    catch (Exception &ex)
    {
        doLog(ecc::smtAppError, "Exception in " __FUNC__ ": " + ex.Message);
    }
    _actTestingZipArchive = false;
}
//---------------------------------------------------------------------------

void Tf_Action::_addEmptyDirsToZipFileList()
{
    if (_error || _aborted) return;

    logB(_("Detecting Empty Directories:"), 1);
    if (!PProfile->config.Action->bIncludeEmptySourceDirectories->value)
    {
        log(_("Disabled."));
        return;
    }

    if ((TActZipStorePath)PProfile->config.Actions.Zip->eStorePath->getIndex() == azpNone)
    {
        logNotif(_("Disabled if 'Path Information' is 'None'."));
        return;
    }

    if (abtFileList->emptyDirList->Count == 0)
    {
        log(_("None found."));
        return;
    }

    log(ASPF(_("Empty directories to include: %d"), abtFileList->emptyDirList->Count));
    for (int i = 0; i < abtFileList->emptyDirList->Count; i++)
    {
        String emptyDir = ExcludeTrailingBackslash(abtFileList->emptyDirList->Strings[i]);
        zipFileList->Append(emptyDir);
    }
}
//---------------------------------------------------------------------------

void Tf_Action::_setArchiveZipSplitOptions()
{
/*  ZipBuilder->SpanOptions (See TZipBuilder Help):
        spNoVolumeName	If True then will use numbered files on removable drives
        spCompatName	If True then uses names of the form name.zip + name.z01 ...
        spWipeFiles     If True, removes the files from the removable drive before writing.
                        Warning!: Deletes all files and Directories
        spTryFormat     If True and spWipeFiles, try to Format removable disk.
                        Warning!: Wipes all files - non-recoverable */

    // === Prepare:
	ZipBuilder->MaxVolumeSize = 0;  // Set to 0 when not spanning (default).

    // === Is splitting/spanning enabled?
 	if (PProfile->config.Actions.Zip->bSplit->value)
	{
        // - Enable spanning (Use either AddDiskSpan or AddDiskSpanErase):
        // Does not work??? AddDiskSpanErase and << spWipeFiles << spTryFormat ???
		ZipBuilder->AddOptions = ZipBuilder->AddOptions << AddDiskSpan;
        ZipBuilder->SpanOptions = ZipBuilder->SpanOptions >> spWipeFiles >> spTryFormat;
        if (_destDriveIsRemovable)
            // Don't create numbered files on removable media:
            ZipBuilder->SpanOptions = ZipBuilder->SpanOptions >> spNoVolumeName >> spCompatName;
        else
            // Create numbered files on fixed disks:
            ZipBuilder->SpanOptions = ZipBuilder->SpanOptions << spNoVolumeName << spCompatName;
	}
    else
    {
        // - Disable spanning and return:
		ZipBuilder->AddOptions  = ZipBuilder->AddOptions >> AddDiskSpan >> AddDiskSpanErase;
        ZipBuilder->SpanOptions = ZipBuilder->SpanOptions >> spWipeFiles >> spTryFormat;
        ZipBuilder->SpanOptions = ZipBuilder->SpanOptions >> spNoVolumeName >> spCompatName;
        return;
    }

    // === Set the size of every part of the spanned archive (according to the presets):
    /* TODO 3 : Span: MaxVolumeSize could be set to 0 to autodetect the space on _removable_ drives. */
 	switch ( (TActZipSplitPreset)PProfile->config.Actions.Zip->eSplitPreset->getIndex() )
	{
		case azspCustom:	// Custom
            // Using user-defined custom value.
			break;
		case azsp1_44:	// 1.44 MB (3.5" floppy disk)
			PProfile->config.Actions.Zip->iSplitCustomSize->set( 1457664 );
			break;
		case azsp100:	// 100 MB (IOMEGA disk) = 94.6 MB, size suggested by Simon.
			PProfile->config.Actions.Zip->iSplitCustomSize->set( 99195289 ); // == 94.6 * 1024*1024;
			break;
		case azsp200:	// 200 MB (MO disk)
			PProfile->config.Actions.Zip->iSplitCustomSize->set( 200 * 1024*1024 );
			break;
		case azsp250:	// 250 MB (MO disk)
			PProfile->config.Actions.Zip->iSplitCustomSize->set( 250 * 1024*1024 );
			break;
		case azsp600:	// 600 Mb (MO disk)
			PProfile->config.Actions.Zip->iSplitCustomSize->set( 600 * 1024*1024 );
			break;
		case azsp650:	// 650 Mb (74 min. CD)
			PProfile->config.Actions.Zip->iSplitCustomSize->set( 650 * 1024*1024 );
			break;
		case azsp700:	// 700 Mb (80 min. CD)
			PProfile->config.Actions.Zip->iSplitCustomSize->set( 700 * 1024*1024 );
			break;
		case azsp750:	// 750 Mb (Zip drive) = 752648192 B, size suggested by Simon.
			PProfile->config.Actions.Zip->iSplitCustomSize->set( 752648192 );
			break;

		default:
            throw Exception(String(__FUNC__ ": Unknown eSplitPreset!"));
	}
    // Inform ZipBuilder of the maximum volume size to be used:
	ZipBuilder->MaxVolumeSize = PProfile->config.Actions.Zip->iSplitCustomSize->get();

    // === Log what will be done:
    log(_("Split Zip Archive: Enabled"));
    if (_destDriveIsRemovable)
    {
        log("- " + _("Type: Spanning (using removable disks)"));
        logNotif("   " + _("You should label every disk."));
    }
    else
    {
        log("- " + _("Type: Splitting (using a fixed drive)."));
        log("   " + _("Files will be named '*.z01', '*.z02', etc., and finally '*.zip'."));
    }
	log("- " + ASPF(_("Each part will have a maximum size of %s (%s Bytes)"),
            ecc::FloatToHumanFileSize(ZipBuilder->MaxVolumeSize), IntToStr(ZipBuilder->MaxVolumeSize) ) );
}
//---------------------------------------------------------------------------

void Tf_Action::_abortedBackup_DeleteFiles()
{
	if (FileExists(ZipBuilder->ZipFileName))
	{
        log( ASPF(_("Deleting unfinished Zip Archive %s..."),
            ecc::QuoteStr(ZipBuilder->ZipFileName)) );
		if (DeleteFile(ZipBuilder->ZipFileName))
            log(_("Ok."), -1);
		else
			logWarn(_("Failed."), -1);
	}

	if (PProfile->config.Actions.Zip->bSplit->value)
	{
		String split_fname;
		String nr;
		// Delete split-files .z01 through .z99:
		for (int i = 1; i <= _actZipSplitVolumeCount; i++)
		{
			nr = IntToStr(i);
			if (i < 10) nr = "0" + nr;  // Add leading zero to [nr].
			split_fname = ChangeFileExt(ZipBuilder->ZipFileName, ".z" + nr);
			if (FileExists(split_fname))
			{
                logNotif(ASPF(_("Deleting %s..."), ecc::QuoteStr(split_fname)) );
				if (DeleteFile(split_fname))
                    logNotif(_("Ok."), -1);
				else
        			logWarn(_("Failed."), -1);
			}
		}
	}
}
//---------------------------------------------------------------------------

void Tf_Action::_doGroupAction()
{
    String rememberCurrentProfile = PProfile->fileName;

	_aborted = false;	// Required! Fixes bug: If a Group Backup was aborted it was not possible to do another Group Backup.

	_groupTimeLapse.Start();
	tmr_Action->Enabled = true;
	_updateElapsedTimeControls();
	ed_Group->Text = PGroup->fileName;

/* TODO 1 : JB: added many try/catch blocks to find bug */

	_profileCount = 0;
    try {   //JB
        for (unsigned int i = 0; i < PGroup->profileList.size(); i++ )
        {
            if (PGroup->profileList[i].checked)
                ++_profileCount;
        }
    } catch(Exception &ex)
    {   f_Main->status("Exception in " __FUNC__ " (" + IntToStr(__LINE__) + "): " + ex.Message); }

    try {   //JB
        String profilePath;
        float groupProgress;
        for (unsigned int i = 0; i < PGroup->profileList.size(); i++ )
        {
            if (_aborted) return;
            if (PGroup->profileList[i].checked)
            {
                pnl_GroupCount->Caption	= IntToStr((int)_profileNr) + "/" + IntToStr((int)_profileCount);
                logWarn( "--- [ "
                    + ASPF(_("Profile: %s"), pnl_GroupCount->Caption)
                    + " ] ---", 0, 1 );

                profilePath = PGroup->profileList[i].str;
                if (FileExists(profilePath))
                {
                    try {   //JB
                        if (f_Main->openProfileInGui(profilePath))
                        //  Open in GUI because ProfileToGui/GuiToProfile is needed to fix some bugs.
                        //	Some additional information will be displayed on the main window Status panel.
                        {
                            f_Main->sdlg_Profile->FileName = ""; // Fixes a bug reported by FPSt on 30-aug-2004.

                            // Display Group progress:
                            groupProgress = ((float)(_profileNr - 1) / (float)_profileCount ) * 100;
                            lbl_GroupPerc->Caption	= IntToStr((int)groupProgress) + "%";
                            pb_Group->Position 		= (int)groupProgress;
                            updateAppTitlePerc();   //092b8
                            // Set custom settings:
                            _applyGroupCustomSettingsToProfile();
                            // Perform action:
                            _doAction();	// Perform this Profile's Action.
                        }
                        else
                        {
                            logError(__FUNC__": f_Main->OpenProfileInGui() failed! - Please Report.");
                        }
                    } catch(Exception &ex)
                    {   f_Main->status("Exception in " __FUNC__ " (" + IntToStr(__LINE__) + "): " + ex.Message); }
                    _error = false; // Reset _error value so the next Profile can be executed.
                }
                else
                {
                    // Profile does not exist:
                    logWarn( ASPF(_("File does not exist: %s"), ecc::QuoteStr(profilePath)), 1, 1);
                }
                ++_profileNr;
            }
        }
    } catch(Exception &ex)
    {   f_Main->status("Exception in " __FUNC__ " (" + IntToStr(__LINE__) + "): " + ex.Message); }


    // All of the Group's profiles have been executed:
	_groupTimeLapse.Stop();
	log( String().StringOfChar('=', 50), 0, 1);
	logNotifB( ASPF(_("Elapsed Time (Group): %s"), TranslateHMS(_groupTimeLapse.AsString())) );

	tmr_Action->Enabled = false;

    // Because the last opened profile in the group may be modified by custom settings,
    // re-open this profile (unless application was started with cmdline args):
    if (!PGlobals->cmdLine_ParametersPresent)
        f_Main->openProfileInGui(rememberCurrentProfile);
}
//---------------------------------------------------------------------------

void Tf_Action::_applyGroupCustomSettingsToProfile()
{
    // Copy abakt::TConfig settings for all abakt::TItem's with override==true:
    abakt::CopyAbaktConfigOnCustomOverride(PGroup->Config, PProfile->config);

    // Copy Group's custom FilterSets to the Profile's list:
    MergeFilterSetLists(&PGroup->customFilterSetList, &PProfile->filterSetList);
}
//---------------------------------------------------------------------------

void Tf_Action::_doAction()
{
	_initAction();

	TActionType act = (TActionType)PProfile->config.Action->eType->getIndex();
	if (!_error)
	{
		switch (act)
		{
			case actZip:
				_doActionZip();
				break;

			case act7Zip:
				_doActionSevenZip();
				break;

			case actFile:
				_doActionFileOperation(); // Sets _error and _aborted.
				break;

            #ifdef ABT_TEST
			default:
				doLog(ecc::smtAppError, __FUNC__" : ActionType undefined!");
            #endif
		}
	}

	// Perform post-Action operations:
	if (_aborted || _error)
	{
		logWarn(_("Action not completed") + ":", 1);
		if (_aborted) logWarn("- " + _("Action aborted by user."));
		if (_error) logError("- " + _("An error occured."));

		// Restore temp copy if backup failed/aborted and azmReplace was used:
		if ( (PGlobals->CreateTempZipCopy) && (!_tempCopyDestFile.IsEmpty()) )
		{
			DeleteFile(ZipBuilder->ZipFileName);

			logNotif( ASPF(_("Restoring temporary copy %s..."), ecc::QuoteStr(_tempCopyDestFile)) );
			if (MoveFile(_tempCopyDestFile.c_str(), ZipBuilder->ZipFileName.c_str()) == 0)
			{
				logError(_("Failed.") , -1);
				logWarn(_("You should manually restore the temporary copy."));
			}
			else
			{
				logNotif(_("Ok.") , -1);
				_tempCopyDestFile = ""; // Reset.
			}
		}
	}
	else if (abtFileList->incFilesTotalCount > 0)  // added 092b8. // '.. != 0' is '.. > 0' since 093-JB1.324
	{
/* TODO 1 : JB: added many try/catch blocks to find bug */
        try {   //JB
    		_clearArchiveAttrs();
        } catch(Exception &ex)
        {   f_Main->status("Exception in " __FUNC__ " (" + IntToStr(__LINE__) + "): " + ex.Message); }
        try {   //JB
    		_removeEmptyDirs();
        } catch(Exception &ex)
        {   f_Main->status("Exception in " __FUNC__ " (" + IntToStr(__LINE__) + "): " + ex.Message); }
        try {   //JB
            if ((act == actZip) || (act == act7Zip))
                _doHistoryCheck(ed_Destination->Text);
        } catch(Exception &ex)
        {   f_Main->status("Exception in " __FUNC__ " (" + IntToStr(__LINE__) + "): " + ex.Message); }
	}

	_endAction();
}
//---------------------------------------------------------------------------

void Tf_Action::_doActionSevenZip()
{
	TActionSevenZip a7z;
	_action__Object = &a7z;

	try
	{
		_error = !a7z.init();
        String destFileName = a7z.getDestination();
		ed_Destination->Text = destFileName;
		log( ASPF(_("Destination: %s"), ecc::QuoteStr(destFileName)) );
		if (!(_error || _aborted))
		{
        	buildFileList();	// Sets _error and _aborted.
            if (abtFileList->incFilesTotalCount == 0) return;   // added 092b8
        }
		if (!(_error || _aborted))
			_error = !a7z.execute();
		if (!(_error || _aborted))
        {
            if (PProfile->config.Actions.SevenZip->tArchiveType->value.LowerCase() == "zip")
                _testZipArchive(destFileName);
        }
	}
	catch (Exception &ex)
	{
        doLog(ecc::smtAppError, "Exception in " __FUNC__ ": " + ex.Message);
	}

	_action__Object = NULL;	// Object will be deleted when function exits.
}
//---------------------------------------------------------------------------

void Tf_Action::_doActionFileOperation()
{
	TActionFileOperation afo;
	_action__Object = &afo;

	try
	{
		_error = !afo.init();
		ed_Destination->Text = afo.getDestinationDir();

		if (!(_error || _aborted))
		{
        	buildFileList();	// Sets _error and _aborted.
            if (abtFileList->incFilesTotalCount == 0) return;   // added 092b8
        }
		if (!(_error || _aborted))
		{
			_error = !afo.execute();
			_aborted = afo.aborted();
		}
	}
	catch (Exception &ex)
	{
        doLog(ecc::smtAppError, "Exception in " __FUNC__ ": " + ex.Message);
	}

	_action__Object = NULL;	// Object will be deleted when function exits.
}
//---------------------------------------------------------------------------

void Tf_Action::buildFileList()
{
	// Constructor has set: abtFileList->OnCurrentFile = OnCurrentFile;

	abtFileList->incFileName = PGlobals->getTempDir()
		+ getDestName() + "~IncFileList.txt";

	abtFileList->excFileName = PGlobals->getTempDir()
        + getDestName() + "~ExcFileList.txt";

    logB( _("Building File Lists") + ":", 1);
	log( ASPF(_("Saving List of Files to Include to %s."),
        ecc::QuoteStr(abtFileList->incFileName)) );
	log( ASPF(_("Saving List of Files to Exclude to %s."),
        ecc::QuoteStr(abtFileList->excFileName)) );

	bool success = abtFileList->buildAll();
    //  Note: [success] is false only if a fatal error occured.
	if (success)
	{
		// Are there any files to backup?
		if (abtFileList->incFilesTotalCount)
		{
			log( ASPF(_("Included Files: %d. Total Size: %s (%s Bytes)"),
                    abtFileList->incFilesTotalCount,
                    ecc::FloatToHumanFileSize(abtFileList->incFilesTotalSize),
                    IntToStr((__int64)abtFileList->incFilesTotalSize) ) );
		    log( ASPF(_("Excluded Files: %d. Total Size: %s (%s Bytes)"),
                    abtFileList->excFilesTotalCount,
                    ecc::FloatToHumanFileSize(abtFileList->excFilesTotalSize),
                    IntToStr((__int64)abtFileList->excFilesTotalSize) ) );
			log( "[ " + ASPF(_("Elapsed Time: %s"),
                    TranslateHMS(timeLapse.AsString())) + " ]" );

            if (!success)
            {
        		logError( _("Building File Lists") + " " + _("Failed.") );
		        logError(" " + abtFileList->getErrorString(), -1);
            }
		}
		else
		{
            // Following lines added in 092b6, suggested by Papp (23-nov-2005):
			logWarn(_("Nothing to do! No files selected."), 1);
            //abtFileList->abort();
			success = true;
		}
	}
 	else
	{
		logError( _("Building File Lists") + " " + _("Failed.") );
		logError(" " + abtFileList->getErrorString(), -1);
	}

	_aborted    = abtFileList->aborted();
	_error      = !success;

	lbl_ProgressInfo->Caption = "";
}
//---------------------------------------------------------------------------

void Tf_Action::OnCurrentFile(TAbaktFilter* filt)
// This function is declared as static.
{
	TAbaktFileList* flist = f_Action->abtFileList;

    if (flist->fileHasError())
    {
        // Display error and return:
        f_Action->logError( flist->getErrorString() );
        return;
    }

	f_Action->lbl_ProgressInfo->Caption =
        ASPF(_("Files: %d (%s) - File: %s"),
            flist->incFilesTotalCount,
            ecc::FloatToHumanFileSize(flist->incFilesTotalSize),
            ecc::QuoteStr(flist->getFilterItem()->getFileName())
        );

	f_Action->lbl_ProgressInfo->Repaint();

	if (!(flist->incFilesTotalCount % 25))
	{
		Application->ProcessMessages();
	}

	if (PGlobals->LogBuildingFileLists)
	{
        String info = " "; // Must contain at least one character.
		if (!filt->getInfoStr().IsEmpty())
			info = "[" + filt->getInfoStr() + "] ";
		if (filt->getFilePassed())
			f_Action->log( ASPF(_("+ Included %s: %s"),
                info, ecc::QuoteStr(filt->getFileName()) ));
		else
			f_Action->log( ASPF(_("- Excluded %s: %s"),
                info, ecc::QuoteStr(filt->getFileName()) ));
	}
}
//---------------------------------------------------------------------------

void Tf_Action::_clearArchiveAttrs()
{
	if (_aborted || _error)
		return;

	// Should the A-bit be cleared? (Depends on ArchiveMode).
	if (!(  (PProfile->config.Action->eArchiveMode->getIndex() == (int)amFull)
			|| (PProfile->config.Action->eArchiveMode->getIndex() == (int)amIncremental)
		)) return;

	logB(_("Clearing Archive Attributes..."), 1);

	String file;
	int attrs;
	bool error = false;

	std::ifstream ifs(f_Action->abtFileList->incFileName.c_str());
	if (!ifs)
	{
		logError( ASPF(_("Could not open file: %s"),
            ecc::QuoteStr(f_Action->abtFileList->incFileName)) );
		_error = true;
		return;
	}

	char line[2048];
	while (ifs.getline(line, 2048))
	{
		file = String(line);

		// Skip files that had an error during backup:
		if (abtFileList->errorItemList->IndexOf(file) != -1)
			continue;

		// Clear Archive attribute:
		attrs = FileGetAttr(file);
		if (attrs == -1)
		{
			error = true;
			logError( ASPF(_("Could not read attributes of file: %s"),
                ecc::QuoteStr(file)) );
			continue;
		}
		if (attrs & faArchive)
		{
			if (FileSetAttr(file, attrs & (~faArchive)) == -1)
			{
				error = true;
				logError( ASPF(_("Could not clear Archive attribute of file: %s"),
                    ecc::QuoteStr(file)) );
			}
		}
	}

	if (!error)
		log(_("Ok."));
}
//---------------------------------------------------------------------------

void Tf_Action::_doHistoryCheck(String final_dest_file)
{
	logB(_("History") + ":", 1);
	if (!PProfile->config.History->bMaintain->value)
	{
		log(_("Disabled") );
		return;
	}

	TAbaktHistory hist(*PProfile, lb_Log);
	if (!hist.execute(final_dest_file, (TArchiveMode)PProfile->config.Action->eArchiveMode->getIndex()) )
		log(_("Nothing done."));
}
//---------------------------------------------------------------------------

void Tf_Action::_removeEmptyDirs()
{
	if (_aborted || _error) return;
	if (!PProfile->config.Action->bRemoveEmptyDirs->value) return;

	bool doRemove = false;
	TActionType act = (TActionType)PProfile->config.Action->eType->getIndex();
	if (act == actFile)
	{
		TActFileMethod afm = (TActFileMethod)PProfile->config.Actions.File->eMethod->getIndex();
		if ( (afm == afmMove) || (afm == afmDelete) )
			doRemove = true;
	}

	if (!doRemove) return;

	logB(_("Removing Emptied Source Directories:"), 1);

	std::ifstream ifs(f_Action->abtFileList->incFileName.c_str());
	if (!ifs)
	{
		logB(_("Failed.") , -1);
		logError(ASPF(_("Could not open file: %s"),
            ecc::QuoteStr(f_Action->abtFileList->incFileName)) );
		return;
	}

	char line[2048];
	String dir;
	TStringList* dirList = new TStringList();
	dirList->Sorted = true; // Set Sort (requires using Append()), so the list can be iterated in reverse order later.
	try
	{
		// Read inc-filelist to build a list of directories.
		while (ifs.getline(line, 2048))
		{
			dir = ExtractFilePath( String(line) );
			if (dirList->IndexOf(dir) == -1)
				dirList->Append(dir);
		}
	}
	catch (...)
	{
		logB(_("Failed.") , -1);
		logError(__FUNC__ ": Could not build list of directories.");
		delete dirList;
		return;
	}

	bool dirsHaveBeenRemoved = false;
	// Iterate in reverse order so that empty dirs inside (other) empty dirs will be removed:
	for (int i = dirList->Count - 1; i >= 0 ; i--)
	{
		// Windows function RemoveDirectory() deletes empty directories only:
		if (RemoveDirectory(dirList->Strings[i].c_str()) != 0)
		{
			log( ASPF(_("Successfully deleted: %s"), ecc::QuoteStr(dirList->Strings[i])) );
			dirsHaveBeenRemoved = true;
		}
	}
	if (!dirsHaveBeenRemoved)
		logB(_("None found."), -1);
/* removed 095b1:
     	else
	    	logB(_("Ok.") ); */

	delete dirList;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::bt_SaveLogClick(TObject *Sender)
{
	if (sdlg_Log->Execute())
	{
		lb_Log->Items->SaveToFile(sdlg_Log->FileName);
		sdlg_Log->InitialDir = ExtractFileDir(sdlg_Log->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::bt_AbortClick(TObject *Sender)
{
	abtFileList->abort();

	timeLapse.Pause();
	_groupTimeLapse.Pause();

	TActionFileOperation* afo;
	TActionSevenZip* a7z;

	switch (PProfile->config.Action->eType->getIndex())
	{
		case actZip:
			if (MessageDlg( _("Abort Action?"), mtConfirmation,
					TMsgDlgButtons() << mbYes << mbNo, 0)
				!= mrYes) return;
			_aborted = true;
			abtFileList->abort();
			actionStyle = asUndefined; // Reset.
			logWarn("[ " + _("Aborting Action") + " - " + _("Please wait") + "... ]", 1, 1);
            ZipBuilder->Cancel = true;
			break;

		case act7Zip:
			if (MessageDlg( _("Abort Action?"), mtConfirmation,
					TMsgDlgButtons() << mbYes << mbNo, 0)
				!= mrYes) return;
            a7z = (TActionSevenZip*)_action__Object;
            #ifdef ABT_TEST
                if (!dynamic_cast<TActionSevenZip*>(a7z))
                {
                    doLog(ecc::smtAppError, __FUNC__ ": Could not cast to TActionSevenZip*");
                    return;
                }
            #endif
			a7z->abort();
            _aborted = true;
			break;

		case actFile:
			afo = (TActionFileOperation*)_action__Object;
            #ifdef ABT_TEST
                if (!dynamic_cast<TActionFileOperation*>(afo))
                {
                    doLog(ecc::smtAppError, __FUNC__ ": Could not cast to TActionFileOperation*");
                    return;
                }
            #endif
			afo->abort();
            _aborted = true;
			break;

		default:
			doLog(ecc::smtAppError, __FUNC__ ": TActionType unknown!", 1, 1);
	}

	timeLapse.Resume();
	_groupTimeLapse.Resume();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::ZipBuilderProgressDetails(TObject *Sender,
      TProgressDetails *Details)
{
	if (_aborted || _error)
    {
        // When a span action is aborted this event (OnProgressDetails) is fired for every file...
        lbl_ProgressInfo->Caption = ASPF(_("Aborting - (%d/%d) - Please wait..."),
                Details->ItemNumber, Details->TotalCount);
        Application->ProcessMessages();
        ZipBuilder->Cancel = true;  // Do this again?
        return;
    }

    // Store these numbers, used when adding empty directories (Tf_Action::ZipBuilderMessage):
    _zipCurrentFileNumber = Details->ItemNumber;
    _zipTotalFileNumber = Details->TotalCount;

    String msg;

    // Display file error:
    if (_zipBuilderFileAddError)
    {
        if (PGlobals->LogFullAction)
            logError(_("Failed."), -1);
        else
        {
            msg = _actZipOperationStr + " " +
                    ASPF(_("File [%d/%d]: %s..."),
                        Details->ItemNumber, Details->TotalCount,
                        ecc::QuoteStr(_actZipCurrentFileName).c_str() );
            logError(msg + _("Failed."));
        }
        _zipBuilderFileAddError = false;  // Reset.
        abtFileList->errorItemList->Append(_actZipCurrentFileName);
        if (PGlobals->TreatWarningsAsErrors)
        {    // Report that this error is fatal.
            logNotif(_("Treat Warnings As Errors") + " " + _("option is enabled: Action failed with Exit Code: ERROR.") );
            _error = true;
        }
    }

    if (!_actZipBuilderMessage.IsEmpty())
    {
        logError(_actZipBuilderMessage);
        _actZipBuilderMessage = "";    // Reset.
    }

    // === Display status:

    __int64 totalSize       = Details->TotalSize;
    __int64 totalPosition   = Details->TotalPosition;

    switch (Details->Order)
    {
        case TotalFiles2Process:    // *Details: TotalCount
            //log(ASPF("Files to Process: %d", Details->TotalCount));
            break;

        case TotalSize2Process:     // *Details: TotalSize
            // Occurs after TotalFiles2Process, so display both values here:
            log(ASPF(_("Files to Process: %d. Total Size: %s (%s B)."), Details->TotalCount,
                    ecc::FloatToHumanFileSize(totalSize), IntToStr(totalSize) ));
            break;

        case NewFile:               // *Details: ItemName, ItemPosition, ItemNumber
            pb_File->Position = 0;
            if (_actTestingZipArchive)
            {
                msg = ASPF(_("Testing File [%d/%d]: %s..."), Details->ItemNumber, Details->TotalCount,
                        ecc::QuoteStr(Details->ItemName) );
                if (PGlobals->LogFullAction)
                    log(msg);
                lbl_ProgressInfo->Caption = msg;
                break;
            }
            _actZipCurrentFileName = Details->ItemName;
            msg = _actZipOperationStr + " " +
                    ASPF(_("File [%d/%d]: %s --> %s..."),
                        Details->ItemNumber, Details->TotalCount,
                        ecc::QuoteStr(Details->ItemName),
                        ecc::QuoteStr(_insideArchiveFileName(Details->ItemName)) );
			lbl_ProgressInfo->Caption = msg;
			if (PGlobals->LogFullAction)
                log(msg);
            if (totalPosition > (__int64)0xFFFFFFFFi64)
            {
                logError(__FUNC__ + ASPF(": Zip Archive > 4GB! Details->TotalPosition=%s", IntToStr(totalPosition)),
                    0, 1);
            }
            break;

        case ProgressUpdate:        // *Details: TotalPerCent, ItemPerCent, TotalPosition, ItemPosition.
            pb_File->Position       = Details->ItemPerCent;
            lbl_FilePerc->Caption   = IntToStr(Details->ItemPerCent) + "%";
            pb_Profile->Position        = Details->TotalPerCent;
            lbl_ProfilePerc->Caption    = IntToStr(Details->TotalPerCent) + "%";
            updateAppTitlePerc();   //092b8
            break;

        case EndOfBatch:
            // Reset the progress bars:
            pb_File->Position       = 0;
            pb_Profile->Position    = 0;
            break;

        case NewExtra:
            break;

        case ExtraUpdate:           // *Details: ItemPosition (ItemPerCent)
            // Occurs when copying compressed files to disk when spanning.
            // - In that case Details->ItemPerCent displays the perc of files copied.
            // - Display of "Copying %s..." occurs in ZipBuilderMessage().
            if (PProfile->config.Actions.Zip->bSplit->value) //092b10
            {
                // Only display these values when splitting:
                pb_Profile->Position        = Details->ItemPerCent;
                lbl_ProfilePerc->Caption    = IntToStr(Details->ItemPerCent) + "%";
                updateAppTitlePerc();   //092b8
            }
            break;

        #ifdef ABT_TEST
        default:
            logError(__FUNC__ ": Details->Order unknown!");
        #endif
    }
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::ZipBuilderMessage(TObject *Sender, int ErrCode,
      AnsiString Message)
/*  Parse the zipdll messages, so they can be displayed in Abakt.
    ???Requires ZipBuilder->Verbose = true. */
{
    static bool FILESIZECHANGED = false;

	if (_aborted || _error)
    {
        logNotif(__FUNC__ " (_aborted || _error): " + ASPF("ZIPMSG [%d]: %s", ErrCode, Message));
        ZipBuilder->Cancel = true;  /* TODO 3 : Do ZipBuilder->Cancel = true; again? */
        return;
    }

    Message = Message.Trim(); // Sometimes Message contains leading spaces.

    // Capture known ErrCodes:
    if (ZipBuilder->TheErrorCode(ErrCode) == DS_Canceled) // ErrCode=11007 [DS_Canceled]: "User cancelled operation".
    {
        logError(__FUNC__ ": " + _("Action aborted by user."));
        return;
    }

    // Report Failure (unparsed ErrCode):
    if (ErrCode != 0 )
    {
        logError(__FUNC__ + ASPF(": ErrCode=0x%X (%d), Message=\"%s\"", ErrCode, ErrCode, Message));
    }

    // Do not display "trace:" messages (case-independent). Occurs when Verbose==true (?and also when Trace==true?):
    if (ZipBuilder->Verbose)
        if (Message.LowerCase().Pos("trace:") == 1) return;

    // Do not display "Trace Msg" messages. Occurs when Verbose==true and ExtrOptions contains ExtrTest,
    // ie. when 'testing' an archive:
    if (ZipBuilder->Verbose)
        if (Message.Pos("Trace Msg") == 1) return;

    // === Parse Message (the text-message sent by ZipBuilder's ZIPDLL):

    _actZipOperationStr = "";
    // If _actZipOperationStr is empty after parsing, Message will be displayed 'as-is'.

    if (Message.Pos("adding:") == 1)        // Requires Message to be trimmed (original string = "  adding:").
    {
        _actZipOperationStr = _("Adding");
        // 092b10: Detect empty dirs and display message:
        if (PProfile->config.Action->bIncludeEmptySourceDirectories->value)
        {
            if (Message[Message.Length()] == '\\') // if last character is '\'
            {
                pb_File->Position = 0;
                ++_zipCurrentFileNumber;
                String msg = _actZipOperationStr + " " +
                    ASPF(_("Empty Directory [%d/%d]: %s..."),
                        _zipCurrentFileNumber, _zipTotalFileNumber,
                        ecc::QuoteStr(Message.SubString(8, Message.Length())) );
                lbl_ProgressInfo->Caption = msg;
                if (PGlobals->LogFullAction)
                    log(msg);
            }
        }
    }

    // -- Parse "Warning: [...] file size changed while zipping: C:\sourcedir\sourcefile.ext":
    else if (Message.Pos("file size changed while zipping:") != 0) 
    {
        FILESIZECHANGED = true;
        _actZipOperationStr = "PARSED";  // Prevent displaying unparsed ZIPMSG.
    }
    // -- Display 'Ok.', and compression ratio '[X%]':
    else if (Message.Pos("deflated") == 1)
    {
        if (PGlobals->LogFullAction)
        {
            // Extract 'compressed to XX% of original size' from "deflated  in=5425,  out=1566,  XX%":
            String compressedSizeInfo = Message.SubString(Message.Length() - 2, 3).Trim();
            String sizeMsg = FILESIZECHANGED ? String("[" + _("File Size Changed While Zipping") + "]") : String(" ");
            ecc::TLBMessageStyle style = FILESIZECHANGED ? ecc::smtNotif : ecc::smtDefault;
            doLog(style, _("Ok.") + ASPF(" [%s] %s", compressedSizeInfo, sizeMsg), -1, 0);
            FILESIZECHANGED = false; // Reset (required).
        }
        _actZipOperationStr = "PARSED";  // Prevent displaying unparsed ZIPMSG.
    }
    else if (Message.Pos("stored") == 1)
    {
        if (PGlobals->LogFullAction)
            log(_("Ok.") + ASPF(" [%s]", String(_("Not Compressed"))), -1);
        _actZipOperationStr = "PARSED";  // Prevent displaying unparsed ZIPMSG.
    }

    else if (Message.Pos("updating:") == 1)
        _actZipOperationStr = _("Updating");

    else if (Message.Pos("keeping:") == 1)
        _actZipOperationStr = _("Keeping");

    else if (Message.Pos("up to date")) // (092b7)
    {
        // Test if "up to date" is at the end of the string:
        if ( Message.Length() - String("up to date").Length() == Message.Pos("up to date") - 1 )
            _actZipOperationStr = "PARSED";  // Prevent displaying unparsed ZIPMSG.
    }

    else if (Message.Pos("Copying:") == 1)
    {
        // Occurs when writing an (already compressed) file to a spanned archive.
        _actZipOperationStr = _("Copying");
        if (_actTestingZipArchive)
            _actZipOperationStr += " " + _("(for testing)");
        String insideFileName = Message.SubString(Message.Pos(":") + 2, Message.Length());
        String msg = _actZipOperationStr + " " + ASPF(_("File: %s"), insideFileName);
        log(msg);
        lbl_ProgressInfo->Caption = msg;
    }

    else if (Message.Pos("could not open for reading:") == 1)
    {
        _actZipBuilderMessage = ASPF("ZIPMSG [%d]: %s", ErrCode, Message);
        _zipBuilderFileAddError = true;
        _actZipOperationStr = "PARSED";  // Prevent displaying unparsed ZIPMSG.
    }

    else if (Message.Pos("Loaded") == 1)    // Parse "Loaded P:\Abakt\ZIPDLL.DLL".
    {
        _actZipOperationStr = "PARSED";     // Prevent displaying unparsed ZIPMSG.
    }
    else if (Message.Pos("Unloaded") == 1)  // Parse "Unloaded P:\Abakt\ZIPDLL.DLL".
    {
        _actZipOperationStr = "PARSED";     // Prevent displaying unparsed ZIPMSG.
    }
    else if (Message.Pos("Total Bytes") == 1) // Parse "Total Bytes=3416551, compr bytes=1454224 -> 57% savings".
    {
        int posEq = Message.Pos("=") + 1;
        int posComma = Message.Pos(",");
        _actZipTotalBytesActedOn = StrToIntDef(Message.SubString(posEq, posComma - posEq), -1);
        _actZipOperationStr = "PARSED";     // Prevent displaying unparsed ZIPMSG.
    }
    else if (Message.Pos("Files acted on") == 1) // Parse "Files acted on = 179".
    {
        _actZipOperationStr = "PARSED";     // Prevent displaying unparsed ZIPMSG.
        // This value is provided by reading ZipBuilder->SuccessCnt.
    }
    else if (Message.Pos("Unloaded") == 1)  // Parse "Unloaded P:\Abakt\ZIPDLL.DLL".
    {
        _actZipOperationStr = "PARSED";     // Prevent displaying unparsed ZIPMSG.
        // This value is provided by reading ZipBuilder->SuccessCnt.
    }

    else if (_actTestingZipArchive)
    {
        if (Message.Pos("Testing") == 1)  // Parse "Testing EDieSoft\XML\__create-ALL-projects.bat".
            _actZipOperationStr = "PARSED"; // Message is displayed in ZipBuilderProgressDetails().
        else if (Message.Pos("Tested") == 1)  // Parse "Tested   file EDieSoft\XML\__create-ALL-projects.bat of size 630".
        {
            _actZipOperationStr = "PARSED";
            if (PGlobals->LogFullAction)
                log(_("Ok."), -1);  // File tested ok.
        }
        else if (Message.Pos("Error") == 1)  // Parse "Error unzipping files".
        {
            _actZipOperationStr = "PARSED";
            logError(_("Failed."), -1);  // File tested with errors.
        }
        else if (Message.Pos("UNZDLL expects") == 1)  // Parse "UNZDLL expects to test 64 files".
            _actZipOperationStr = "PARSED";
        else if (Message.Pos("No Errors Found") == 1) // Parse "No Errors Found In 483 Tested Files of S:\Backups\Abakt\Abakt.zip".
            _actZipOperationStr = "PARSED";
        // added in 092b6:
        else if (Message.Pos("Read local filename") == 1) // Parse "Read local filename Abakt\Source\abakt\Abakt.cpp".
            _actZipOperationStr = "PARSED";
        // added in 092b7:
        else if (Message.Pos("Time Hour") == 1) // Parse "Time Hour 10, min 36 [etc.]".
            _actZipOperationStr = "PARSED";
        // added in 092b9:
        else if (Message.Pos("Using DelZip.dll") == 1) // Parse "Using DelZip.dll version [etc.]".
            _actZipOperationStr = "PARSED";
        // added in 092b9:
        else if (Message.Pos("DLL expects to test") == 1) // Parse "DLL expects to test [etc.]".
            _actZipOperationStr = "PARSED";
    }

    // === Display Message if it was not parsed by Abakt (ie. when _actZipOperationStr==""):
    if (_actZipOperationStr.IsEmpty())
    {
        logNotif( ASPF(_("ZipBuilder Message [Code=%d]: %s"), ErrCode, Message.c_str()) );
    }
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::ZipBuilderZipDialog(TObject *Sender,
      const AnsiString Title, AnsiString &msg, int &Context,
      TMsgDlgButtons Btns)
{
    int ctx = Context & 0xFFFF;     // Values are declared in ZBCtx.h (should be #included in this file).

    // Disable popup when setting ZipBuilder->Cancel to true:
    if (ctx == DHC_ExMessage)
    {
        // User aborted Action Zip:
        Context = mrOk; // Set Context to mrOk to make sure no popup is shown.
        return;
    }

    // Show everything else:
    logWarn( __FUNC__ ": " + ASPF("ZIPDLG [%s]: %s.", Title, msg) );
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::ZipBuilderSetAddName(TObject *Sender,
      AnsiString &FileName, const AnsiString ExtName, bool &IsChanged)
{
    /*  ExtName:    Source filename as stored on disk (includes disk-info).
        FileName:   Filename as it will be included in the ZIP archive. */

    IsChanged = false;

    int azp = (TActZipStorePath)PProfile->config.Actions.Zip->eStorePath->getIndex();
 	switch (azp)
	{
        #ifdef ABT_TEST
		default:
			logWarn(__FUNC__ ": TActZipStorePath unknown. Using azpFull.");
        #endif
		case azpFull:   // Do not change FileName.
			break;

		case azpRelative:
		case azpNone:
            FileName = _insideArchiveFileName(ExtName);
            IsChanged = true;
        break;
    }

    // 092RC2: Test and set NoCompression here. (Using undocumented option " |0", see mail RA 26-mar-2006)
    TAbaktFilter filt;
    if (filt.testFile(ExtName))
    {
        if (filt.getFileNotCompressed())
        {
            FileName = FileName + " |0";
            IsChanged = true;
        }
    }
}
//---------------------------------------------------------------------------

void Tf_Action::_setActZipComment()
{
 	if (!PProfile->config.Actions.Zip->bAddComment->value)
        return;

    String zipComment = abtFormatText(PProfile->config.Actions.Zip->tCommentTemplate->value);
    log( ASPF(_("Writing Zip Comment: %s..."), ecc::QuoteStr(zipComment)) );
    try
    {
        ZipBuilder->ZipComment = zipComment;
    }
    catch (...)
    {
        logError(_("Failed."), -1);
        doLog(ecc::smtAppError, __FUNC__ ": What happened here?");
        return;
    }
    log(_("Ok."), -1);
}
//---------------------------------------------------------------------------

void Tf_Action::_validateZipFileList()
{
    logB(_("Detecting Duplicate Filenames Inside Archive:"), 1);

    TStringList* insideArchiveFileNameList = new TStringList();
    insideArchiveFileNameList->Capacity = zipFileList->Count;
    bool duplicatesFound = false;

    ecc::TTimeLapse lapse;
    lapse.Start();

    try
    {
        // Build insideArchiveFileNameList:
        // This is a list that helps finding duplicate filenames that could appear in the archive.
        // Depends on the PathInformation setting.

        String file;
        for (int i = 0; i < zipFileList->Count; i++)
        {
            if (i % 50 == 0) // Update GUI every Nth file.
            {
                lbl_ProgressInfo->Caption = _("Detecting Duplicate Filenames Inside Archive:")
                    + " (fase 1/3) " + IntToStr(i) + "/" + IntToStr(zipFileList->Count);
                Application->ProcessMessages();
                if (_aborted) break;
            }

            file = _insideArchiveFileName(zipFileList->Strings[i]);
            file += "?" + zipFileList->Strings[i];
            insideArchiveFileNameList->Append(file);
        }

        // Sort items in insideArchiveFileNameList:

        lbl_ProgressInfo->Caption = _("Detecting Duplicate Filenames Inside Archive:")
            + " (fase 2/3) Sorting...";
        Application->ProcessMessages();

        insideArchiveFileNameList->CaseSensitive = false;
        insideArchiveFileNameList->Sort();

        // Find duplicates in insideArchiveFileNameList and rebuild zipFileList accordingly.

        int pos;
        String insidefile, outsidefile;
        zipFileList->Clear();
        for (int i = 0; i < insideArchiveFileNameList->Count; i++)
        {
            if (i % 50 == 0) // Update GUI every Nth file.
            {
                lbl_ProgressInfo->Caption = _("Detecting Duplicate Filenames Inside Archive:")
                    + " (fase 3/3) " + IntToStr(i) + "/" + IntToStr(insideArchiveFileNameList->Count);
                Application->ProcessMessages();
                if (_aborted) break;
            }

            pos = insideArchiveFileNameList->Strings[i].Pos("?");
            insidefile = insideArchiveFileNameList->Strings[i].SubString(1, pos - 1).LowerCase();
            outsidefile = insideArchiveFileNameList->Strings[i].SubString(pos + 1, 2000);

            if (i == 0)
            {
                zipFileList->Append(outsidefile);   // Always include first file in zip archive.
                insideArchiveFileNameList->Strings[i] = insidefile;
            }

            if (i > 0)  // Start testing from the second file on.
            {
                // Compare insidefile with previous insidefile:
                if (insidefile == insideArchiveFileNameList->Strings[i - 1].LowerCase())
                {
                    logWarn(ASPF(_("Filename %s exists! - File %s will not be included in archive"),
                        ecc::QuoteStr(insidefile), ecc::QuoteStr(outsidefile) ));
                    abtFileList->errorItemList->Append(outsidefile);
                    duplicatesFound = true;
                }
                else
                {
                    // Insidefile is not a duplicate, add outsidefile to zipFileList:
                    zipFileList->Append(outsidefile);
                }
                // Update insideArchiveFileNameList for comparison (few lines above):
                insideArchiveFileNameList->Strings[i] = insidefile;
            }
        }
    }
	catch (Exception &ex)
	{
		doLog(ecc::smtAppError, "Exception in " __FUNC__ " [1]: " + ex.Message);
        _error = true;
    }

    delete insideArchiveFileNameList;

    if (!duplicatesFound)
        log(_("None found."));
 	log( "[ " + ASPF(_("Elapsed Time: %s"), TranslateHMS(lapse.AsString())) + " ]" );
    lbl_ProgressInfo->Caption = _("Detecting Duplicate Filenames Inside Archive:") + " " + _("Done.");
    Application->ProcessMessages();
}
//---------------------------------------------------------------------------

String Tf_Action::_insideArchiveFileName(String file)
{
    String root;
    bool rootFound;

    int azp = (TActZipStorePath)PProfile->config.Actions.Zip->eStorePath->getIndex();
 	switch (azp)
	{
		case azpFull:
            root = ExtractFileDrive(file);
            file.Delete(1, root.Length());
			break;

		case azpRelative:
            rootFound = false;
            for (unsigned int i = 0; i < PProfile->sourceRootDirList.size(); i++)
            {
                root = PProfile->sourceRootDirList[i];
                if (file.Pos(root) == 1)
                {
                    rootFound = true;
                    break;
                }
            }
            if (rootFound)
                file.Delete(1, root.Length()); // Remove drive+rootdir
            else  // Should never happen!
                logError(__FUNC__": ROOT NOT FOUND!? Please Report.");
			break;

		case azpNone:
            file = ExtractFileName(file);
			break;

        default:  // Should never happen!
            logError(__FUNC__": [azp] NOT RECOGNIZED!? Please Report.");
	}
    return file;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::ZipBuilderStatusDisk(TObject *Sender,
      int PreviousDisk, AnsiString PreviousFile,
      TZipDiskStatus Status, TZipDiskAction &Action)
{
    /*  TZipDiskStatus is a set and can take one or more of the following values:
        zdsEmpty            The disk is empty and ready to use.
        zdsHasFiles         The disk is not empty and contains one or more file(s).
        zdsPreviousDisk     It’s possibly a previous disk from the backup set.
        zdsSameFileName     The disk contains a file with the same filename as the zip archive.
        zdsNotEnoughSpace   There is not enough space on the disk taken into
                            account KeepFreeOnDisk1, MaxVolumeSize and MinFreeVolumeSize. */

    /*  TZipDiskAction is an enum and should be set to one of the following values:
        zdaOk      Continue diskspanning (the default).
        zdaErase   Erase the disk first.
        zdaReject  Try again.
        zdaCancel  Cancel the diskspanning operation. */

	if (_aborted || _error)
    {
        Action = zdaCancel;
        return;
    }

    // === Deal with [PreviousDisk] and [PreviousFile]:
    // NOTE: PreviousDisk and PreviousFile are actually the NEXT diskNr and fileName!

    logNotif(ASPF(_("Copying Files to Zip Archive Part %d - (%s):"),
            PreviousDisk, ecc::QuoteStr(PreviousFile)) );
    _actZipSplitVolumeCount = PreviousDisk;

    // === Deal with [Status]:
/* TODO 2 : ?Deal with zdsPreviousDisk? */
/* TODO 2 : ?Deal with zdsSameFileName? */
/*     String status;
    if (Status.Contains(zdsEmpty))
        status += "+zdsEmpty";
    if (Status.Contains(zdsHasFiles))
        status += "+zdsHasFiles";
    if (Status.Contains(zdsNotEnoughSpace))
        status += "+zdsNotEnoughSpace";
    if (Status.Contains(zdsPreviousDisk))
        status += "+zdsPreviousDisk";
    if (Status.Contains(zdsSameFileName))
        status += "+zdsSameFileName";
    logWarn(__FUNC__": " + ASPF("Status=%s", status) ); */

    if (Status.Contains(zdsNotEnoughSpace) && !_destDriveIsRemovable)
    {
        logError(_("Destination disk does not have enough free space."));
        Action = zdaCancel;   // Cancel the diskspanning operation.
        _error = true;
        return;
    }

    if (_destDriveIsRemovable)
    {
        if (Status.Contains(zdsHasFiles))
        {
            String msg = _("Destination disk is not empty.");
            msg += "\n\n" + _("Use this disk anyway?")
                + "\n\n" + _("You can insert another disk now, then press Retry.");
            int result = MessageDlg(msg, mtConfirmation, TMsgDlgButtons() << mbYes << mbRetry << mbCancel, 0);
            if (result == mrYes)
            {
                Action = zdaOk;     // Continue diskspanning (the default).
            }
            else if (result == mrRetry)
            {
                log(__FUNC__ ": Action=zdaReject (Retry)");
                Action = zdaReject; // Try again.
            }
            else if (result == mrCancel)
            {
                log(__FUNC__ ": Action=zdaCancel");
                Action = zdaCancel; // Cancel the diskspanning operation.
                _aborted = true;
            }
        }
        else if (Status.Contains(zdsNotEnoughSpace))
        {
            String msg = _("Destination disk does not have enough free space.");
            msg += "\n\n" + _("Insert a (formatted) disk in the drive.");
            int result = MessageDlg(msg, mtConfirmation, TMsgDlgButtons() << mbOK << mbCancel, 0);
            if (result == mrOk)
            {
                Action = zdaReject; // Try again.
            }
            else if (result == mrCancel)
            {
                log(__FUNC__ ": CANCEL => zdaCancel");
                Action = zdaCancel; // Cancel the diskspanning operation.
                _aborted = true;
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall Tf_Action::ZipBuilderGetNextDisk(TObject *Sender,
      int DiskSeqNo, int DiskTotal, AnsiString Drive, bool &Cancel)
{
	if (_aborted || _error)
    {
        Cancel = true;
        #ifdef ABT_TEST
            logWarn(__FUNC__ ": Cancel = true");
        #endif
        return;
    }

    logWarn(ASPF(__FUNC__": DiskSeqNo=%i, DiskTotal=%i, Drive=%s",
            DiskSeqNo, DiskTotal, Drive) );

/* TODO 2 : ZB: Required? if (DiskSeqNo == 0) DiskSeqNo = 1; */
//    if (DiskSeqNo == 0) DiskSeqNo = 1;

	// If destination-drive is removable, ask for new disk:
	if (_destDriveIsRemovable)
	{
		int result = MessageDlg(
			ASPF(_("Insert a (formatted) disk in drive %s."),
			    ecc::QuoteStr(Drive + ":") )
            + "\n\n"
            + ASPF(_("Label this disk '%d'."), DiskSeqNo)
			, mtConfirmation
			, TMsgDlgButtons() << mbOK << mbAbort
			, 0);
		if (result == mrAbort)
		{
			Cancel = true;
			_aborted = true;
			//not needed, ZB aborts immediately: logNotif(_("Please wait until ZipBuilder allows the action to be aborted..."));
		}
	}

//DON'T! (see ZipBuilderStatusDisk):   _actZipSplitVolumeCount = DiskSeqNo;

	if (!(_aborted || _error))
	{
		logNotif( ASPF(_("Saving Zip Archive part %d to drive %s."),
            DiskSeqNo, ecc::QuoteStr(Drive)) );
	}
}
//---------------------------------------------------------------------------

void Tf_Action::updateAppTitlePerc()
{
    String title;// = "Abakt";
    if (!pnl_GroupCount->Caption.IsEmpty())
        title = "[" + pnl_GroupCount->Caption + "]";
    if (!lbl_ProfilePerc->Caption.IsEmpty())
        title = title + " "  + lbl_ProfilePerc->Caption;
    Application->Title = title + " Abakt";
    Application->ProcessMessages();
}
//---------------------------------------------------------------------------

