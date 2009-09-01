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
#ifndef ActionH
#define ActionH
//---------------------------------------------------------------------------
#include "eccTimeLapse.h"
#include "eccString.h"
#include "AbaktFileList.h"
#include "ZipBuilder.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
/*  Usage:
    {
    	f_Action->ActionStyle = baSingle;   // Setting ActionStyle is mandatory.
	    f_Action->ShowModal();
    }
*/
//---------------------------------------------------------------------------

typedef void CALLBACK_BackupListCurrentFile(const String fpath, const bool file_passed);
//---------------------------------------------------------------------------

typedef enum
{
	asUndefined = 0,
	asSingle,	// Single Profile action.
	asGroup,	// Multiple Profile actions
	asViewLog
} TActionStyle;
//---------------------------------------------------------------------------

class Tf_Action : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TButton *bt_Close;
	TGroupBox *gbx_Log;
	TListBox *lb_Log;
	TGroupBox *gbx_Progress;
	TButton *bt_Abort;
	TPanel *Panel2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TSaveDialog *sdlg_Log;
    TButton *bt_SaveLog;
	TPanel *pnl_GroupProgress;
	TLabel *Label5;
	TProgressBar *pb_Group;
	TLabel *lbl_GroupPerc;
	TPanel *pnl_ProfileProgress;
	TLabel *Label4;
	TLabel *lbl_ProfilePerc;
	TProgressBar *pb_Profile;
	TEdit *ed_ProfilePath;
	TEdit *ed_Destination;
	TEdit *ed_Group;
	TPanel *Panel3;
	TLabel *lbl_File;
	TLabel *lbl_FilePerc;
	TProgressBar *pb_File;
	TLabel *lbl_ProgressInfo;
	TPanel *pnl_ElapsedTime;
	TTimer *tmr_Action;
	TPanel *pnl_GroupElapsedTime;
	TPanel *pnl_GroupCount;
    TZipBuilder *ZipBuilder;
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall lb_LogDrawItem(TWinControl *Control, int Index,
		  TRect &Rect, TOwnerDrawState State);
	void __fastcall tmr_ActionTimer(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall bt_SaveLogClick(TObject *Sender);
    void __fastcall bt_CloseClick(TObject *Sender);
    void __fastcall bt_AbortClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ZipBuilderProgressDetails(TObject *Sender,
          TProgressDetails *Details);
    void __fastcall ZipBuilderMessage(TObject *Sender, int ErrCode,
          AnsiString Message);
    void __fastcall ZipBuilderZipDialog(TObject *Sender,
          const AnsiString Title, AnsiString &msg, int &Context,
          TMsgDlgButtons Btns);
    void __fastcall ZipBuilderSetAddName(TObject *Sender,
          AnsiString &FileName, const AnsiString ExtName, bool &IsChanged);
    void __fastcall ZipBuilderGetNextDisk(TObject *Sender, int DiskSeqNo,
          int DiskTotal, AnsiString Drive, bool &Cancel);
    void __fastcall ZipBuilderStatusDisk(TObject *Sender, int PreviousDisk,
          AnsiString PreviousFile, TZipDiskStatus Status,
          TZipDiskAction &Action);

public:		// User declarations
	__fastcall Tf_Action(TComponent* Owner);

    void abort()            { _aborted = true; }
    /*  Abort from backup as soon as possible. */

	bool aborted()          { return _aborted; }
	bool failed()           { return _error; }

  	ecc::TTimeLapse timeLapse;

	TActionStyle actionStyle; // Set this before calling f_Backup->ShowModal().

    void doLog(const ecc::TLBMessageStyle smt, const String msg,
            const int lines_before = 0, const int lines_after = 0);
	void log(const String msg, const int lines_before = 0, const int lines_after = 0);
	void logB(const String msg, const int lines_before = 0, const int lines_after = 0);
	void logNotif(const String msg, const int lines_before = 0, const int lines_after = 0);
	void logNotifB(const String msg, const int lines_before = 0, const int lines_after = 0);
	void logWarn(const String msg, const int lines_before = 0, const int lines_after = 0);
	void logError(const String msg, const int lines_before = 0, const int lines_after = 0);

	TAbaktFileList* abtFileList;
	/* 	Helper-class used to build the lists of files for the current Action. */

	TStringList* zipFileList;
    /*  At the beginning of an Action: contains every file that is selected.
        At a later stage the in-archive duplicates are removed from this list
        (and added to abtFileList->errorItemList).
        Finally, if there were errors while archiving the remaining files they will
        be added to abtFileList->errorItemList, but not removed from zipFileList.
        >>  The files that were actually zipped can be found in zipFileList,
            but the files in abtFileList->errorItemList should be removed first. */

	void buildFileList();
	/* 	Sets _error and _aborted.
		An empty list is regarded as an error. */

	static void OnCurrentFile(TAbaktFilter* filt);
	/* 	Callback function, to be attached to TAbaktFileList.OnCurrentFile.
		[filt] points to the file currently being processed. */

    String getDestName();
    /*  Returns Destination's (formatted) name, without extension or path information. */

    String getDestDir();
    /*  Returns Destination's (formatted) destination directory. */

	bool testCreateDestDir(String destDir);
	/*	Checks for and possibly creates the Destination 'root' Directory.
		Returns false if the dir does not exist or could not be created. */

    void updateAppTitlePerc();    // added 092b8.
    /*  Adds Profile's and Group's (if appropriate) progress-percentage strings to Application->Title. */

private:

   	String _ORG_Caption;

	bool _aborted;
	bool _error;        // There was a fatal error, the operation failed.
	bool _busy;         // If true: form cannot close.

	// Used by Group:
	int _profileNr;     // 09b27: was double, now int
	int _profileCount;  // 09b27: was double, now int

	ecc::TTimeLapse _groupTimeLapse;

	bool _destDriveIsRemovable;

	bool _skipFileLogging;
	/*	If true: Log will not be saved to logfile for the current action only. */

	void _start();
	/*	Called when form opens. */

	void _initAction();
	/*	Perform some initializations for all actiontypes. */

	void _doAction();
	/*	Perform action according to Profile's actiontype.
		Also called by DoGroupAction(). */

	void _endAction();
	/*	Perform some finalializations for all actiontypes. */

// === <ZIP Methods/Properties>:

    String _actZipOperationStr;
    /*  Value is set in event ZipBuilderMessage(). */

    String _actZipCurrentFileName;
    String _actZipBuilderMessage;

	int _actZipSplitVolumeCount;
    /*  Amount of split zip archive files. */

    __int64 _actZipTotalBytesActedOn;

	String _tempCopyDestFile;
	/*	Used by azmReplace in DoZipBackupAccordingToMethod(). */

	void _doActionZip();

	bool _initActionZip(String& destZip);
    /*  Returns false if the Action could not start because of errors. */

	void _doZipBackupAccordingToMethod();

    bool _actTestingZipArchive;
    /*  Is true when testing a newly created Zip Archive in _testZipArchive(). */

    void _testZipArchive(const String fileName);
    /*  Tests Zip Archive [fileName] for errors.
        Sets _actTestingZipArchive as long as it is busy. */

	bool _zipBuilderFileAddError;
    /*  Is true when a file could not be added to the archive. */

    void _setActZipComment();
    /*  Set Zip Archive Comment (ZB says: do this after setting ZipFileName). */

    void _validateZipFileList();
    /*  Test for duplicate entries of filenames that could appear _inside_ the archive.
        The original filenames of any found duplicate entry will be removed
        from zipFileList and will not be included in the archive.
        A warning will be shown in the log window if duplicates are found. */

    String _insideArchiveFileName(String file);
    /*  (Helper function)
        Returns a string where root-drive(+path) information is stripped
        from [file], according to the StorePath settings (full/relative/none).
        The returned string is the filename as it will appear _inside_ the archive. */

	void _setArchiveZipSplitOptions();

    void _addEmptyDirsToZipFileList();

    int _zipCurrentFileNumber;
    int _zipTotalFileNumber;
    /*  These numbers are used when adding empty directories to a zip archive. */

	void _endActionZip();
    /*  Displays details of finished Zip Action. */

// === </ZIP Methods/Properties>

	void _abortedBackup_DeleteFiles();
    /*  Delete backup files when Action was aborted - including the .z## files. */

	void* _action__Object;
	/*	Contains the current Action___ object. Used to send an Abort message. */

	void _doActionSevenZip();
	/*	Sets _error if an error occured. */

	void _doActionFileOperation();
	/*	Sets _error if an error occured. */

	void _doGroupAction();
	/*	Perform a series of Actions. */

	void _updateElapsedTimeControls();

	void _clearArchiveAttrs();
	void _removeEmptyDirs();

	void _applyGroupCustomSettingsToProfile();
	/*	Copies settings overriden by Group to current Profile. */

	void _doHistoryCheck(String final_dest_file);
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_Action *f_Action;
//---------------------------------------------------------------------------
#endif

