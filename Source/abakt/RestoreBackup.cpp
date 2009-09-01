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

#include "RestoreBackup.h"
#include "AbaktGlobals.h"
#include "AbaktHistory.h"
#include "eccFormMemory.h"
#include "eccBrowseForDirectory.h"
#include "eccString.h"
#include "eccMisc.h"
#include "eccFile.h"
#include "GlobalFuncs.h"
#include "FileProperties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ZipBuilder"
#pragma resource "*.dfm"
Tf_RestoreBackup *f_RestoreBackup;
//---------------------------------------------------------------------------

/* === Notes:
0.9.2-RC2:
    - Set ZipBuilder.ExtrOptions[ExtrDirNames] to true. Required to extract empty directories.
*/
//---------------------------------------------------------------------------

__fastcall Tf_RestoreBackup::Tf_RestoreBackup(TComponent* Owner)
	: TForm(Owner)
{
	// Set initial values:
	cb_Overwrite->Checked = false;
    lbl_ArchiveDetails->Caption = "";

    // Read saved GUI properties:
	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	mem.ReadProps(this);

	// == Read other prefs from inifile:
	TIniFile* ini = mem.GetIniFile();

	// -- Read lv_Files->Column sizes:
	String sect = Name + ".lv_Files.Columns";
	String ident;
	int cols = 0;
	while (1)
	{
		ident = "Col" + IntToStr(cols) + ".Width";
		if (!ini->ValueExists(sect, ident)) break;
		lv_Files->Columns->Items[cols]->Width = ini->ReadInteger(sect, ident, 50);
		++cols;
	}

    // Other:
	if (odlg_Backup->InitialDir.IsEmpty())
        odlg_Backup->InitialDir = PGlobals->DefaultDestDir;
	if (sdlg_Log->InitialDir.IsEmpty())
    	sdlg_Log->InitialDir	= PGlobals->ActionLogDir;

    // Make sure the dll's in Abakt's installation directory are used:
    ZipBuilder->DLLDirectory    = ExtractFilePath(Application->ExeName);

    // Required:
	_aborted    = false;
	_busy	    = false;
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
    #endif
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::FormShow(TObject *Sender)
{
    lv_Files->Clear();

	gbx_Progress->Visible = false;

	if (ed_DestDir->Text.IsEmpty())
		odlg_Backup->InitialDir = PGlobals->DefaultDestDir;

    // Retrieve latest archive filename from History and display its contents:
    _addArchiveListToComboBox();
    _listFilesInArchive(cmbx_ZipArchiveFilename->Text);
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::FormClose(TObject *Sender,
	  TCloseAction &Action)
{
	try
	{
		ecc::TFormMemory mem(PGlobals->getIniFilePath());
		TIniFile* ini = mem.GetIniFile();
		ini->EraseSection(Name); // Remove old settings.
		ini->UpdateFile();	// Flush file to disk.

		mem.SetComponent(this);
		mem.WriteProps("Top;Left;Width;Height");
		mem.WriteProps("WindowState");
		mem.WriteProps(pnl_Bottom, "Height");
		mem.WriteProps(ed_DestDir, "Text");
        mem.WriteProps(odlg_Backup, "InitialDir");
        mem.WriteProps(sdlg_Log, "InitialDir");

		// Save lv_Files->Column sizes:
		for (int i = 0; i < lv_Files->Columns->Count; i++)
			ini->WriteInteger(Name + ".lv_Files.Columns", "Col" + IntToStr(i) + ".Width", lv_Files->Columns->Items[i]->Width);
	}
	catch(...)
	{
		ShowMessage( __FUNC__ ":\n\n" + ASPF(_("Failed to save your settings to:\n%s."),
                ecc::QuoteStr(PGlobals->getIniFilePath())) );
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::act_CloseExecute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::FormCloseQuery(TObject *Sender,
	  bool &CanClose)
{
	CanClose = !_busy;
}
//---------------------------------------------------------------------------

void Tf_RestoreBackup::_doLog(const String msg, const ecc::TLBMessageStyle style,
        const int lines_before, const int lines_after) const
{
	ecc::LBMessage(lb_Log, msg, style, lines_before, lines_after);
	Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void Tf_RestoreBackup::log(const String msg, const int lines_before, const int lines_after) const
{
	_doLog(msg, ecc::smtDefault, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::lb_LogDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{
	ecc::LBMessage_DrawItem(Control, Index, Rect, State);
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::act_SaveLogExecute(TObject *Sender)
{
	if (sdlg_Log->Execute())
	{
		lb_Log->Items->SaveToFile(sdlg_Log->FileName);
		sdlg_Log->InitialDir = ExtractFileDir(sdlg_Log->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::act_AbortExecute(TObject *Sender)
{
	if ( MessageDlg(_("Abort Restore Operation?"), mtConfirmation,
			TMsgDlgButtons() << mbYes << mbNo, 0)
		!= mrYes) return;

	_aborted = true;
    /* TODO 2 : Restore: markup log messages */
	log(">> Restore Backup aborted!", 1, 1);
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::bt_ZipArchiveClick(TObject *Sender)
{
	if (!odlg_Backup->Execute()) return;
	String fname = odlg_Backup->FileName;
	cmbx_ZipArchiveFilename->Text = fname;
	odlg_Backup->InitialDir = ExtractFileDir(fname);
    _listFilesInArchive(fname);
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::bt_ListFilesClick(TObject *Sender)
{
	String fname = cmbx_ZipArchiveFilename->Text;
	odlg_Backup->InitialDir = ExtractFileDir(fname);
    _listFilesInArchive(fname);
}
//---------------------------------------------------------------------------

void Tf_RestoreBackup::_listFilesInArchive(String fileName)
{
//092b7    ZipBuilder->Unload_Unz_Dll();   // Necessary?
    lb_Log->Clear();    // Always.
    if (fileName.IsEmpty()) return;

    _displayArchiveDetails(fileName);

	unsigned int mod_datetime;
	TListItem* litem;
	log( ASPF(_("Opening Zip Archive: %s..."), ecc::QuoteStr(fileName)) );
    _driveIsRemovable = ecc::DriveIsRemovable(fileName); // Needed by ZipBuilder->OnGetNextDisk.
	ecc::HourGlass(true);
	lv_Files->Clear();
	lv_Files->Items->BeginUpdate();
    _busy           = true;
	_aborted        = false;
    ZipDirEntry* entry;
	try
	{
        ZipBuilder->ZipFileName = fileName;
/* TODO 2 : (ZipBuilder->IsSpanned) */
/*         if (ZipBuilder->IsSpanned)
        {
            // Required SpanOptions for extracting multi-archive zip from fixed disk:
            ZipBuilder->SpanOptions = ZipBuilder->SpanOptions << spNoVolumeName << spCompatName;
            log(__FUNC__ ": " + _("Detected Spanned/Split Zip Archive."));
        } */
        for (int i = 0; i < ZipBuilder->Count; i++)
		{
            entry = ZipBuilder->DirEntry[i];
			litem = lv_Files->Items->Add();
			// Name:
			litem->Caption = ExtractFileName(entry->FileName);
            if (litem->Caption.IsEmpty())
                litem->Caption = "<" + _("Directory") + ">";
			// Path:
			litem->SubItems->Add(ExtractFilePath(entry->FileName));
			// Size:
			litem->SubItems->Add(ecc::FloatToHumanFileSize(entry->UncompressedSize));
			// Attributes:
			litem->SubItems->Add(abtAttrToStr(entry->ExtFileAttrib));
			// Modified:
			litem->SubItems->Add(FileDateToDateTime(entry->DateTime).DateTimeString());
			// Compression rate:
            float ratio = 0;
            if ((float)entry->UncompressedSize != 0)   // Prevent divide-by-zero.
                ratio = (float)entry->CompressedSize / (float)entry->UncompressedSize;
			litem->SubItems->Add( FloatToStrF(ratio * 100, ffFixed, 3, 1) );
			// Compressed file size:
			litem->SubItems->Add(ecc::FloatToHumanFileSize(entry->CompressedSize));

			Application->ProcessMessages();
		}
	}
	catch(Exception &ex)
	{
		_doLog("Exception in " __FUNC__ ": " + ex.Message, ecc::smtAppError);
	}

	lv_Files->SelectAll();
	lv_Files->Items->EndUpdate();
	ecc::HourGlass(false);
    _busy           = false;

    if (ZipBuilder->Count)
	{
        log(_("Ok."), -1);
	    log( ASPF(_("Files in Zip Archive: %d"), ZipBuilder->Count) );
    }
    else
        log(_("Failed."), -1);
}
//---------------------------------------------------------------------------

void Tf_RestoreBackup::_displayArchiveDetails(String fileName)
{
    lbl_ArchiveDetails->Caption = "";

    TFileProperties fp(fileName);
    if (fp.failed()) return;

    lbl_ArchiveDetails->Caption =
        ASPF(_("Date Created: %s - Size: %s (%s B)"),
            FormatDateTime("d/mmm/yyyy hh:nn", fp.getDateTime()),
            ecc::FloatToHumanFileSize(fp.getSize()), IntToStr(fp.getSize()) );
}
//---------------------------------------------------------------------------

void Tf_RestoreBackup::_addArchiveListToComboBox()
{
    cmbx_ZipArchiveFilename->Clear();

	TAbaktHistory hist(*PProfile);
    if (hist.getSize() == 0) return;
    // Add archives to combobox (in reverse order):
	for (unsigned int i = hist.getSize() - 1; i != 0; i--)
	{
        cmbx_ZipArchiveFilename->Items->Append(hist.getFileName(i));
	}
    if (cmbx_ZipArchiveFilename->Items->Count > 0)
        cmbx_ZipArchiveFilename->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::bt_DestDirClick(TObject *Sender)
{
	using namespace ecc;

	TBrowseForDirectory bd(PGlobals->UseNewUIOnBFD);
	bd.Handle		= Handle;
	bd.Directory	= ed_DestDir->Text;
	bd.Title 		= ecc::ReplaceStr(bt_DestDir->Caption, "&", "");

	if (bd.Execute())
	{
		String dest_dir = IncludeTrailingPathDelimiter(bd.Directory);
		ed_DestDir->Text = dest_dir;
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::act_RestoreFilesExecute(TObject *Sender)
{
	String fname = cmbx_ZipArchiveFilename->Text;
	if (!FileExists(fname))
	{
		ShowMessage( ASPF(_("Zip Archive does not exist:\n\n%s"),
            ecc::QuoteStr(fname)) );
		return;
	}

	String dest_dir = ed_DestDir->Text;
	if (!DirectoryExists(dest_dir))
	{
		ShowMessage( ASPF(_("Destination Directory does not exist:\n\n%s"),
            ecc::QuoteStr(dest_dir)) );
		return;
	}

	// Prepare:
	pb_Profile->Position    = 0;
	lbl_ProfilePerc->Caption = "---";
	gbx_Progress->Visible   = true;
	_enableControls(false);
	ActiveControl           = bt_Abort;
	lb_Log->Clear();
	ecc::HourGlass(true);
	_busy           = true;
	_aborted        = false;
	Application->ProcessMessages();

	// Extract:
	TStringList* flist = new TStringList(); // List of selected files.
	try
	{
		// Build list of selected files:
		for (int i = 0; i < lv_Files->Items->Count; i++)
		{
			if (!lv_Files->Items->Item[i]->Selected) continue;
            if (lv_Files->Items->Item[i]->SubItems->Strings[2].Pos("D")) // 2 is the Attr column.
            {
                flist->Append(
                    lv_Files->Items->Item[i]->SubItems->Strings[0] );   // 0 is the Path column.
            }
            else
            {
                flist->Append(
				    lv_Files->Items->Item[i]->SubItems->Strings[0]      // 0 is the Path column.
				    + lv_Files->Items->Item[i]->Caption );
            }
		}

		// Extract selected files:
		ZipBuilder->ZipFileName = fname;
		ZipBuilder->Password = ed_Password->Text;
		ZipBuilder->ExtrBaseDir = dest_dir;
/* TODO 2 : HERE: if (ZipBuilder->IsSpanned) */
if (ZipBuilder->IsSpanned)
{
    log(_("Detected Split/Spanned Zip Archive."));
    if (!_driveIsRemovable)
    {
        // Required SpanOptions for extracting multi-archive zip from fixed disk:
        ZipBuilder->SpanOptions = ZipBuilder->SpanOptions << spNoVolumeName << spCompatName;
    }
}
else
{
    // Required SpanOptions for extracting non-split archives,
    // and multi-archive zips on removable disks:
    ZipBuilder->SpanOptions = ZipBuilder->SpanOptions >> spNoVolumeName >> spCompatName;
}
        if (cb_Overwrite->Checked)
            ZipBuilder->ExtrOptions = ZipBuilder->ExtrOptions << ExtrOverWrite;
        else
            ZipBuilder->ExtrOptions = ZipBuilder->ExtrOptions >> ExtrOverWrite;
		ZipBuilder->FSpecArgs->Assign(flist);
		ZipBuilder->Extract();
        /* TODO 2 : Extract (empty) directories: */
        /* if (dlist->Count) log(">> Extract (empty) directories:");
        for (int i = 0; i < dlist->Count; i++)
		    ZipForge->ExtractFiles(dlist->Strings[i], faDirectory); */
	}
	catch(Exception &ex)
	{
		_doLog("Exception in " __FUNC__ ": " + ex.Message, ecc::smtAppError);
	}
	delete flist;

	// End:
	_doLog(_("Zip Archive Details:"), ecc::smtDefaultB, 1);
	if (!_aborted)
	{
		log(ASPF(_("Files/Directories in Zip Archive: %d"), ZipBuilder->Count) );
    	log(ASPF(_("Files/Directories Acted On: %d"), ZipBuilder->SuccessCnt) );
	}

    /* TODO 2 : Restore: markup log messages */
	if (_aborted)
		log("[ --- " + _("Restore Operation Aborted") + " --- ]", 1, 1);
	else
		log("[ --- " + _("Restore Operation Finished") + " --- ]", 1, 1);

	// After:
	_busy = false;
	ecc::HourGlass(false);
	gbx_Progress->Visible	= false;
	_enableControls(true);
	ActiveControl 			= bt_Close;
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::act_SelectAllFilesExecute(
	  TObject *Sender)
{
	lv_Files->SelectAll();
}
//---------------------------------------------------------------------------

void Tf_RestoreBackup::_enableControls(bool enable)
{
	bt_DestDir->Enabled 		= enable;
	bt_ZipArchive->Enabled	    = enable;
    bt_ListFiles->Enabled	    = enable;
	cb_Overwrite->Enabled		= enable;
	act_RestoreFiles->Enabled	= enable;
	pnl_Top->Enabled			= enable;
	act_Abort->Enabled 			= !enable;
	act_SaveLog->Enabled 		= enable;
	act_Close->Enabled 			= enable;
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::ZipBuilderProgressDetails(
      TObject *Sender, TProgressDetails *Details)
{
    // === Display status:

    String msg;

    __int64 totalSize = Details->TotalSize;

    switch (Details->Order)
    {
        case TotalFiles2Process:    // *Details: TotalCount
            break;

        case TotalSize2Process:     // *Details: TotalSize
            // Occurs after TotalFiles2Process, so display both values here:
            log(ASPF(_("Files/Directories to Process: %d. Total Size: %s (%s B)"), Details->TotalCount,
                    ecc::FloatToHumanFileSize(totalSize), IntToStr(totalSize) ));
            break;

        case NewFile:               // *Details: ItemName, ItemPosition, ItemNumber
            pb_File->Position = 0;
            msg = ASPF(_("Extracting File [%d/%d]: %s --> %s..."),
                        Details->ItemNumber, Details->TotalCount,
                        ecc::QuoteStr(_stripDestDir(Details->ItemName)),
                        ecc::QuoteStr(Details->ItemName) );
			lbl_ProgressInfo->Caption = msg;
			if (PGlobals->LogFullAction)
                log(msg);
            break;

        case ProgressUpdate:        // *Details: TotalPerCent, ItemPerCent, TotalPosition, ItemPosition.
            pb_File->Position       = Details->ItemPerCent;
            lbl_FilePerc->Caption   = IntToStr(Details->ItemPerCent) + "%";
            pb_Profile->Position        = Details->TotalPerCent;
            lbl_ProfilePerc->Caption    = IntToStr(Details->TotalPerCent) + "%";
            break;

        case EndOfBatch:
            //NOT NEEDED: _doLog("### " __FUNC__ " [EndOfBatch]", ecc::smtNotif);
            // Reset the progress bars:
            pb_File->Position       = 0;
            pb_Profile->Position    = 0;
            break;

        case NewExtra:
            //NOT NEEDED: _doLog("### " __FUNC__ " [NewExtra]", ecc::smtNotif);
            break;

        case ExtraUpdate:           // *Details: ItemPosition (ItemPerCent)
            // ??? Occurs when copying compressed files to disk when spanning.
            // ??? - In that case Details->ItemPerCent displays the perc of files copied.
            // ??? - Display of "Copying %s..." occurs in ZipBuilderMessage().
            pb_Profile->Position = Details->ItemPerCent;
            lbl_ProfilePerc->Caption   = IntToStr(Details->ItemPerCent) + "%";
            break;

        #ifdef ABT_TEST
        default:
            log(__FUNC__ ": Details->Order unknown!");
        #endif
    }
}
//---------------------------------------------------------------------------

String Tf_RestoreBackup::_stripDestDir(String fileName)
{
    if (fileName.Pos(ed_DestDir->Text) != 1)
        return fileName;

    return fileName.Delete(1, ed_DestDir->Text.Length());
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::ZipBuilderMessage(TObject *Sender,
      int ErrCode, AnsiString Message)
{
	if (_aborted)
    {
        _doLog(__FUNC__ " _aborted: " + ASPF("UNZMSG [%d]: %s", ErrCode, Message),
            ecc::smtNotif);
        ZipBuilder->Cancel = true;  /* TODO 3 : Do ZipBuilder->Cancel = true; again? */
        return;
    }

    Message = Message.Trim(); // Sometimes Message contains leading spaces.

    // Report Failure:
    if (ErrCode
        && !(Message.Pos("Skipped") == 1)) // File was skipped because it already exists and overwrite == false.
    {
        _doLog(__FUNC__ + ASPF(": ErrCode=0x%X (%d), Message=\"%s\"", ErrCode, ErrCode, Message),
            ecc::smtError);
    }

    // Skip "trace:" messages. Occurs when Verbose==true:
    if (ZipBuilder->Verbose)
        if (Message.Pos("trace:") == 1) return;

    // Skip "Trace Msg" messages. Occurs when Verbose==true and ExtrOptions contains ExtrTest,
    // ie. when 'testing' an archive:
    if (ZipBuilder->Verbose)
        if (Message.Pos("Trace") == 1) return;

    // === Parse Message (the text-message sent by UNZDLL):

    bool parsed = false;

    // -- Parse "Deleted pre-existing file C:\RestoreDir\ProfileName\someFile.ext":
    if (Message.Pos("Deleted") == 1)
    {
        log(_("[Overwrite]..."), -1);
        parsed = true;
    }
    // -- Parse "Unzipped file C:\RestoreDir\ProfileName\someFile.ext of size 3094":
    //    And display 'Ok.':
    else if (Message.Pos("Unzipped") == 1)
    {
        log(_("Ok."), -1);
        parsed = true;
    }
    // -- Parse "Skipped C:\RestoreDir\ProfileName\someFile.ext 2": (Note the " 2" at the end!)
    else if (Message.Pos("Skipped") == 1)
    {
        String file = ecc::QuoteStr(Message.SubString(9, Message.Length() -9-1 ));
        // 9: position after "Skipped ". 1: " 2" at the end of Message.
        String msg = ASPF(_("Skipped Existing File %s...Ok."), file);
        log(msg);
        lbl_ProgressInfo->Caption = msg;
        parsed = true;
    }
    // -- Parse "creating: [empty directory]" (added in 092RC2):
    else if (Message.Pos("creating:") == 1)
    {
        String dir = ecc::QuoteStr(Message.SubString(11, Message.Length() -11 ));
        // 11: position after "creating: ".
        log(ASPF(_("Creating Empty Directory %s...Ok."), dir));
        parsed = true;
    }
    // -- Parse "Copying: [file]".  Occurs before Extracting. (added in 095b2):
    else if (Message.Pos("Copying:") == 1)
    {
        String dir = ecc::QuoteStr(Message.SubString(10, Message.Length() -10 ));
        // 10: position after "Copying: ".
        lbl_ProgressInfo->Caption = ASPF(_("Copying file %s...Ok."), dir);
        Application->ProcessMessages();
        parsed = true;
    }
    // -- Do not display these messages:
    else if (Message.Pos("Loaded") == 1)    // Parse "Loaded P:\Abakt\UNZDLL.DLL".
        parsed = true;
    else if (Message.Pos("UNZDLL") == 1)    // Parse "UNZDLL expects to test XX files". XX=64 or less.
        parsed = true;
    else if (Message.Pos("Unloaded") == 1)  // Parse "Unloaded P:\Abakt\UNZDLL.DLL".
        parsed = true;
    else if (Message.Pos("Files acted on") == 1) // Parse "Files acted on = 179".
        parsed = true;
    // added in 092RC2:
    else if (Message.Pos("Read local filename") == 1) // Parse "Read local filename Abakt\Source\abakt\Abakt.cpp".
        parsed = true;
    // added in 092RC2:
    else if (Message.Pos("Time Hour") == 1) // Parse "Time Hour 10, min 36 [etc.]".
        parsed = true;
    // added in 092RC2:
    else if (Message.Pos("Using DelZip.dll") == 1) // Parse "Using DelZip.dll version [etc.]".
        parsed = true;
    // added in 092RC2:
    else if (Message.Pos("DLL expects") == 1) // Parse "DLL expects [etc.]".
        parsed = true;

    // === Display Message if it was not parsed by Abakt:
    if (!parsed)
        log(ASPF(__FUNC__ ": UNZMSG (unparsed) [%d]: %s", ErrCode, Message));
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::ZipBuilderGetNextDisk(TObject *Sender,
      int DiskSeqNo, int DiskTotal, AnsiString Drive, bool &Cancel)
{
	if (_aborted)
    {
        Cancel = true;
        #ifdef ABT_TEST
            log(__FUNC__ ": _aborted: Cancel=true");
        #endif
        return;
    }

    log(ASPF(__FUNC__": DiskSeqNo=%i, DiskTotal=%i, Drive=%s, _driveIsRemovable=%d",
            DiskSeqNo, DiskTotal, Drive, _driveIsRemovable) );

    if ((DiskSeqNo == 0) && _driveIsRemovable)
    {
        log(__FUNC__": DriveIsRemovable() && DiskSeqNo == 0: What to do now? -> PLEASE INSERT FIRST DISK??");
        Cancel = true;
        _aborted = true;
        return;
    }

	// If drive is removable ask for disk:
	if (_driveIsRemovable)
	{
		int result = MessageDlg(
			ASPF(_("Please insert disk %d of %d in drive %s."),
                DiskSeqNo, DiskTotal, ecc::QuoteStr(Drive + ":") )
			, mtConfirmation
			, TMsgDlgButtons() << mbOK << mbAbort
			, 0);
		if (result == mrAbort)
		{
			Cancel = true;
			_aborted = true;
			//not needed, ZB aborts immediately: log(_("Please wait until ZipBuilder allows the action to be aborted..."));
		}
	}
/* 	_actZipSplitVolumeCount = DiskSeqNo;
	if (!(_aborted || _error))
	{
		logNotif( ASPF(_("Saving Zip Archive part %d to drive %s."),
            DiskSeqNo, ecc::QuoteStr(Drive)) );
	} */
}
//---------------------------------------------------------------------------

void __fastcall Tf_RestoreBackup::cmbx_ZipArchiveFilenameChange(
      TObject *Sender)
{
    if (cmbx_ZipArchiveFilename->ItemIndex == -1) return;
    String fileName = cmbx_ZipArchiveFilename->Items->Strings[cmbx_ZipArchiveFilename->ItemIndex];
    _listFilesInArchive(fileName);
}
//---------------------------------------------------------------------------

