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

#include "FilterPreview.h"
#include "Gui_X_Profile.h"
#include "AbaktGlobals.h"
#include "eccFormMemory.h"
#include "eccFile.h"
#include "eccMisc.h"
#include "eccString.h"
#include "GlobalFuncs.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tf_FilterPreview *f_FilterPreview;
//---------------------------------------------------------------------------

__fastcall Tf_FilterPreview::Tf_FilterPreview(TComponent* Owner)
	: TForm(Owner)
{
	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	mem.ReadProps(this);

    TIniFile* ini = mem.GetIniFile();

	// Read section [f_FilterPreview.lv_IncludeList.Columns]
	String sect = Name + ".lv_IncludeList.Columns";
	String ident;
	int cols = 0;
	while (1)
	{
		ident = "Col" + IntToStr(cols) + ".Width";
		if (!ini->ValueExists(sect, ident)) break;
		lv_IncludeList->Columns->Items[cols]->Width = ini->ReadInteger(sect, ident, 50);
		++cols;
	}
	// Read section [f_FilterPreview.lv_ExcludeList.Columns]
	sect = Name + ".lv_ExcludeList.Columns";
	cols = 0;
	while (1)
	{
		ident = "Col" + IntToStr(cols) + ".Width";
		if (!ini->ValueExists(sect, ident)) break;
		lv_ExcludeList->Columns->Items[cols]->Width = ini->ReadInteger(sect, ident, 50);
		++cols;
	}

	_aborted = false;
	_busy = false;
	lbl_IncludedFilesInfo->Caption = "";
	lbl_ExcludedFilesInfo->Caption = "";
	PageControl->ActivePage = tab_IncludedFiles;

	_abtFileList = new TAbaktFileList();
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
    #endif
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::FormDestroy(TObject *Sender)
{
	delete _abtFileList;
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::FormActivate(TObject *Sender)
{
	ActiveControl = bt_Build;
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::FormCloseQuery(TObject *Sender,
	  bool &CanClose)
{
	CanClose = !_busy;
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::FormClose(TObject *Sender,
	  TCloseAction &Action)
{
	try
	{
		ecc::TFormMemory mem(PGlobals->getIniFilePath());
		mem.SetComponent(this);
		mem.WriteProps("Top;Left;Width;Height");
		mem.WriteProps("WindowState");
		mem.WriteProps(cb_StripDrive, "Checked");
		mem.WriteProps(cb_AddQuotes, "Checked");
		mem.WriteProps(cb_ScanFullTree, "Checked");
		mem.WriteProps(sdlg_FileList, "InitialDir");

        TIniFile* ini = mem.GetIniFile();
        // Save f_FilterPreview->lv_IncludeList->Column sizes:
        for (int i = 0; i < lv_IncludeList->Columns->Count; i++)
            ini->WriteInteger(Name + ".lv_IncludeList.Columns",
                "Col" + IntToStr(i) + ".Width",
                lv_IncludeList->Columns->Items[i]->Width);
        // Save f_FilterPreview->lv_ExcludeList->Column sizes:
        for (int i = 0; i < lv_ExcludeList->Columns->Count; i++)
            ini->WriteInteger(Name + ".lv_ExcludeList.Columns",
                "Col" + IntToStr(i) + ".Width",
                lv_ExcludeList->Columns->Items[i]->Width);
	}
	catch(...)
	{
		ShowMessage( ASPF(_("Failed to save your settings to:\n%s."),
                ecc::QuoteStr(PGlobals->getIniFilePath())) );
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::bt_CloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::bt_AbortClick(TObject *Sender)
{
	_abtFileList->abort();
	_aborted = true;
}
//---------------------------------------------------------------------------

TListView* Tf_FilterPreview::_getActiveListView()
{
	if (PageControl->ActivePage == tab_IncludedFiles)
		return lv_IncludeList;
	if (PageControl->ActivePage == tab_ExcludedFiles)
		return lv_ExcludeList;
	return NULL;
}
//---------------------------------------------------------------------------

void Tf_FilterPreview::clearLists()
{
	lv_IncludeList->Clear();
	lv_ExcludeList->Clear();
   	lbl_IncludedFilesInfo->Caption = "";
	lbl_ExcludedFilesInfo->Caption = "";
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::pmi_SelectAllFilesClick(
	  TObject *Sender)
{
	TListView* lv = _getActiveListView();
	if (!lv) return;

	lv->Items->BeginUpdate();
	lv->SelectAll();
	lv->Items->EndUpdate();
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::pmi_ClearFileListClick(TObject *Sender)
{
	TListView* lv = _getActiveListView();
	if (!lv) return;

	lv->Clear();
}
//---------------------------------------------------------------------------

void Tf_FilterPreview::OnCurrentFile(TAbaktFilter* filt)
/*  Function is declared as static. */
{
    if (f_FilterPreview->_abtFileList->fileHasError())
    {
        /* TODO 3 : Should user be notified of error? */
        return;
    }

	String fpath = filt->getFileName();
	bool fileIncluded = filt->getFilePassed();

	if (f_FilterPreview->cb_StripDrive->Checked)
	{
        int driveLen = String(ExtractFileDrive(fpath)).Length() + 1; // i.e. "C:\"
    	fpath = fpath.Delete(1, driveLen);
    }
	if (f_FilterPreview->cb_AddQuotes->Checked)
		fpath = ecc::QuoteStr(fpath);

    TFileProperties fprops = filt->getFileProps();

    TListView* listView;
    TLabel* lblInfo;
    unsigned int* fileCount;
    __int64* totalFileSize;

	if (fileIncluded)
    {
	    listView = f_FilterPreview->lv_IncludeList;
        lblInfo = f_FilterPreview->lbl_IncludedFilesInfo;
        fileCount = &f_FilterPreview->_abtFileList->incFilesTotalCount;
        totalFileSize = &f_FilterPreview->_abtFileList->incFilesTotalSize;
    }
    else
	{
        listView = f_FilterPreview->lv_ExcludeList;
        lblInfo = f_FilterPreview->lbl_ExcludedFilesInfo;
        fileCount = &f_FilterPreview->_abtFileList->excFilesTotalCount;
        totalFileSize = &f_FilterPreview->_abtFileList->excFilesTotalSize;
    }

    // 303: Test if files in excluded directories should be added to the exclude-list:
    bool addFile = true;
    if (!fileIncluded)
        if (filt->getDirNotSelected())
            if (!f_FilterPreview->_abtFileList->getScanFullTree())
                addFile = false;

    if (addFile)
    {
        try
        {
            TListItem* litem = listView->Items->Add();
            // Display Name:
            litem->Caption = fpath;
            // Display Size:
            litem->SubItems->Add( ecc::FloatToHumanFileSize(fprops.getSize()) );
            // Display Attributes:
            litem->SubItems->Add( fprops.getAttrsStr() );
            // Display Date (Last Modified):
            litem->SubItems->Add( fprops.getDate().DateTimeString() );
            // Display Filter Info:
            litem->SubItems->Add( filt->getInfoStr() );
        }
        catch (Exception &err)
        {
            ShowMessage( ASPF("Error in function %s: %s."
                    "\nTListView %s will be cleared."
                    "\nProbable cause: too many lines (>65535)",
                String(__FUNC__), ecc::QuoteStr(err.Message),
                listView->Name) );
            listView->Clear();
        }
    }

    // Show progress every Nth file:
    if (!(*fileCount % 50))
    {
        lblInfo->Caption = IntToStr(*fileCount)
            + " (" + ecc::FloatToHumanFileSize((float)*totalFileSize) + ")";
        Application->ProcessMessages();
    }
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::bt_SaveActiveListClick(TObject *Sender)
{
    TListView* lv = _getActiveListView();
    if (lv == NULL) return;
    if (lv->SelCount == 0) pmi_SelectAllFiles->Click();

	if (!sdlg_FileList->Execute()) return;
	sdlg_FileList->InitialDir = ExtractFilePath(sdlg_FileList->FileName);

    // Save every selected file entry:
    TStringList *fileList = new TStringList();
    bool error = false;
  	ecc::HourGlass(true);
    try
    {
        // Build list of files selected in the ListView:
        for (int i = 0; i < lv->Items->Count; i++)
        {
            if (lv->Items->Item[i]->Selected)
                fileList->Append( lv->Items->Item[i]->Caption );
        }
    }
    catch (...)
    {
        ShowMessage(__FUNC__ ": Oops!");
        error = true;
    }

    if (!error)
    {
        fileList->SaveToFile(sdlg_FileList->FileName);
    }
    delete fileList;
  	ecc::HourGlass(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterPreview::bt_BuildClick(TObject *Sender)
{
	// Disable Controls and initialize:
	bt_Build->Enabled			= false;
	bt_SaveActiveList->Enabled	= false;
	bt_Close->Enabled 			= false;
	bt_Abort->Enabled = true;

	_busy = true;
	_aborted = false;

	ecc::HourGlass(true);

	clearLists();
	lv_IncludeList->Items->BeginUpdate();
	lv_ExcludeList->Items->BeginUpdate();

	GuiToProfile.all();
	_abtFileList->setScanFullTree(cb_ScanFullTree->Checked);
    #ifdef ABT_TEST
        if (_abtFileList->getScanFullTree() != cb_ScanFullTree->Checked) // Should never happen.
            ShowMessage(__FUNC__ ": Could not set 'Scan Full Tree'.");
    #endif
	_abtFileList->OnCurrentFile = OnCurrentFile;

	// Build lists:
	if (_abtFileList->buildAll())
	{
		// Display filecounts:
		lbl_IncludedFilesInfo->Caption = ASPF(_("%d - Total Size: %s (%s Bytes)"),
            lv_IncludeList->Items->Count,
            ecc::FloatToHumanFileSize(_abtFileList->incFilesTotalSize),
            IntToStr((__int64)_abtFileList->incFilesTotalSize) );
        lbl_ExcludedFilesInfo->Caption = ASPF(_("%d - Total Size: %s (%s Bytes)"),
            lv_ExcludeList->Items->Count,
			ecc::FloatToHumanFileSize(_abtFileList->excFilesTotalSize),
			IntToStr((__int64)_abtFileList->excFilesTotalSize) );
	}
	else
	{
   		// Error in _abtFileList->BuildAll (abort is not an error):
		if (!_aborted)
	    	ShowMessage(__FUNC__ ": " + _abtFileList->getErrorString());
	}

    if (_aborted)
    {
        lbl_IncludedFilesInfo->Caption =
            lbl_IncludedFilesInfo->Caption + " [" + _("Operation Aborted") + "] ";
        lbl_ExcludedFilesInfo->Caption =
            lbl_ExcludedFilesInfo->Caption + " [" + _("Operation Aborted") + "] ";
    }

	// Enable Controls:
	lv_IncludeList->Items->EndUpdate();
	lv_ExcludeList->Items->EndUpdate();
	ecc::HourGlass(false);
	bt_Abort->Enabled = false;
	_busy = false;
	bt_SaveActiveList->Enabled		= true;
	bt_Close->Enabled 				= true;
	bt_Build->Enabled				= true;

	ActiveControl = bt_Close;
}
//---------------------------------------------------------------------------

