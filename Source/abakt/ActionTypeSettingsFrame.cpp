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

#include "ActionTypeSettingsFrame.h"
#include "AbaktProfile.h"
#include "AbaktGlobals.h"
#include "ActionSevenZip.h"
#include "Gui_X_Profile.h"
#include "DataModule.h"
#include "Main.h"
#include "FileSizeEditor.h"
#include "GlobalFuncs.h"
#include "eccString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameActionTypeSettings *FrameActionTypeSettings;
//---------------------------------------------------------------------------

__fastcall TFrameActionTypeSettings::TFrameActionTypeSettings(TComponent* Owner)
	: TFrame(Owner)
{
	ScrollBox_ActZip->BorderStyle   = bsNone;
	ScrollBox_7Zip->BorderStyle     = bsNone;

    // Copy [ConfigSet]->info strings to GUI:
    // (The info strings have already been translated in AbaktConfigSets.cpp.)
    abakt::TConfigSet_ActZip* pcZip = PProfile->config.Actions.Zip;
    lbl_ZipInfo_StorePath->Caption      = pcZip->eStorePath->info;
    lbl_ZipInfo_Compression->Caption    = pcZip->iCompression->info;
//    lbl_ZipInfo_SplitArchive->Caption   = pcZip->bSplit->info;
    lbl_ZipInfo_SplitCustomSize->Caption = pcZip->iSplitCustomSize->info;

    abakt::TConfigSet_Act7Zip* pc7z = PProfile->config.Actions.SevenZip;
    lbl_7zInfo_Template->Caption    = pc7z->tCommandLineTemplate->info;
    lbl_7zInfo_Command->Caption     = pc7z->tCommand->info;
    lbl_7zInfo_ArchiveType->Caption = pc7z->tArchiveType->info;

    abakt::TConfigSet_ActFile* pcFile = PProfile->config.Actions.File;
    lbl_FileInfo_Overwrite->Caption             = pcFile->bOverwrite->info;
    lbl_FileInfo_UseRecycleBin->Caption         = pcFile->bUseRecycleBin->info;
//removed 095b1:    lbl_FileInfo_UseSHFileOperation->Caption    = pcFile->bUseSHFileOperation->info;
    lbl_FileInfo_PathInformation->Caption       = pcFile->ePathInformation->info;

	// Populate cmbx_Zip_StorePath:
    cmbx_Zip_StorePath->Clear();
	for (unsigned int i = 0; i < pcZip->eStorePath->size(); i++)
		cmbx_Zip_StorePath->Items->Append( _(pcZip->eStorePath->at(i)) );

	// Populate cmbx_Zip_SplitVolumeSizePreset:
    cmbx_Zip_SplitVolumeSizePreset->Clear();
	for (unsigned int i = 0; i < pcZip->eSplitPreset->size(); i++)
    	cmbx_Zip_SplitVolumeSizePreset->Items->Append( _(pcZip->eSplitPreset->at(i)) );

	// Populate cmbx_7z_ArchiveType:
/* 092b6: the archivetype(==archive extension) can now be an arbitrary string.
    cmbx_7z_ArchiveType->Clear();
	for (unsigned int i = 0; i < pc7z->eArchiveType->size(); i++)
		cmbx_7z_ArchiveType->Items->Append( _(pc7z->eArchiveType->at(i)) ); */

	// Populate cmbx_File_PathInformation:
    cmbx_File_PathInformation->Clear();
	for (unsigned int i = 0; i < pcFile->ePathInformation->size(); i++)
		cmbx_File_PathInformation->Items->Append( _(pcFile->ePathInformation->at(i)) );
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::SetProfileModified(TObject *Sender)
{
	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::ed_Zip_SplitCustomSizeKeyPress(TObject *Sender,
	  char &Key)
{
    if (Key == VK_RETURN)
        ed_Zip_SplitCustomSizeDblClick(Sender);
    if (Key == VK_DELETE)
        return;         // Del is ok.
    if ((Key < '0') || (Key > '9'))
        Key = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::ed_Zip_SplitCustomSizeChange(
      TObject *Sender)
{
    TEdit* ed = (TEdit*)Sender;
    if (!ed) return;

    abakt::TUIntRange *item = PProfile->config.Actions.Zip->iSplitCustomSize;

    __int64 val = StrToIntDef(ed->Text, item->getMin());
    if (val < item->getMin()) val = item->getMin();
    if (val > item->getMax()) val = item->getMax();
    ed_Zip_SplitCustomSize->Text = IntToStr(val);
    SetProfileModified(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::ed_Zip_SplitCustomSizeDblClick(
      TObject *Sender)
{
    bt_actZip_SplitCustomSize->Click();
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::bt_actZip_SplitCustomSizeClick(
      TObject *Sender)
{
    abakt::TUIntRange *item = PProfile->config.Actions.Zip->iSplitCustomSize;
	f_FileSizeEditor->value = StrToIntDef(ed_Zip_SplitCustomSize->Text, item->getMin());
	if (f_FileSizeEditor->ShowModal() == mrOk)
	{
		__int64 val = f_FileSizeEditor->value;
		if (val < item->getMin()) val = item->getMin();
		if (val > item->getMax()) val = item->getMax();
		ed_Zip_SplitCustomSize->Text = IntToStr(val);
		SetProfileModified(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::pgc_ActionTypesChange(
      TObject *Sender)
{
	if (pgc_ActionTypes->ActivePage == tab_7Zip)
	{
        update7ZipPreview();
	}
}
//---------------------------------------------------------------------------

void TFrameActionTypeSettings::update7ZipPreview()
{
    // Shortcut for readability & convenience:
    TFrameActionTypeSettings* frACTSETS = f_Main->fr_ActionTypeSettings;

	f_Main->fr_Destination->formattedNamePreview();

    // 7z command:
    String command = frACTSETS->ed_7z_Command->Text;

    // 7z archive type (-t):
	String type = "-t"
        //+ Act7ZipArchiveTypeToStr((TAct7ZipArchiveType)frACTSETS->cmbx_7z_ArchiveType->ItemIndex);
        + cmbx_7z_ArchiveType->Text;

    // destination archive path = destdir+name+ext:
    String destName = abtFormatText(f_Main->fr_Destination->ed_NameTemplate->Text);
    String destExt = f_Main->fr_Destination->cb_AppendExtension->Checked
        //? String("." + Act7ZipArchiveTypeToExt((TAct7ZipArchiveType)frACTSETS->cmbx_7z_ArchiveType->ItemIndex))
        ? String("." + cmbx_7z_ArchiveType->Text)
        : String("");
    String destPath = f_Main->fr_Destination->ed_Directory->Text + destName + destExt;

    // password (-p):
    String password;
    bool maskPassword = true;
    if (!frACTSETS->ed_7z_Password->Text.IsEmpty())
    {
        if (maskPassword) password = password.StringOfChar('*', frACTSETS->ed_7z_Password->Text.Length());
        else password = frACTSETS->ed_7z_Password->Text;
        password = "-p\"" + password + "\""; // 092b8: added quotes
    }

    // working dir (-w):
    String workDir = PGlobals->getTempDir().IsEmpty()
        ? String("") : String("-w" + ecc::QuoteStr(PGlobals->getTempDir()) );

    // listfile (@):
    String listFile = f_Main->fr_Destination->ed_Directory->Text
        + destName + "~.txt";

    // => Complete command line:
    String line = frACTSETS->ed_7z_Template->Text;
    line = ecc::ReplaceStr(line, "$$", "$");
    line = ecc::ReplaceStr(line, "$C", command);
    line = ecc::ReplaceStr(line, "$T", type);
    line = ecc::ReplaceStr(line, "$N", ecc::QuoteStr(destPath));
    line = ecc::ReplaceStr(line, "$P", password);
    line = ecc::ReplaceStr(line, "$W", workDir);
    line = ecc::ReplaceStr(line, "$L", "@" + ecc::QuoteStr(listFile));
    ed_7z_Preview->Text = line;
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::bt_7z_DefaultClick(TObject *Sender)
{
    ed_7z_Template->Text = "$C $T $N -mx=5 $P $W $L";
    update7ZipPreview();
    SetProfileModified(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::ed_Zip_CompressionChange(
      TObject *Sender)
{
    if (ed_Zip_Compression->Modified)
        SetProfileModified(Sender);
    if (ed_Zip_Compression->Text.IsEmpty()) ed_Zip_Compression->Text = "0";
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::ed_Zip_CompressionKeyPress(
      TObject *Sender, char &Key)
{
    if (Key == VK_BACK)
        return; // Backspace is ok.
    if ((Key < '0') || (Key > '9'))
        Key = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::ud_Zip_CompressionChangingEx(
	  TObject *Sender, bool &AllowChange, short NewValue,
	  TUpDownDirection Direction)
{
	SetProfileModified(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::bt_Zip_CommentPreviewClick(
      TObject *Sender)
{
    ShowMessage( abtFormatText(ed_Zip_CommentTemplate->Text) );
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::ed_7z_TemplateChange(
      TObject *Sender)
{
    update7ZipPreview();
    SetProfileModified(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionTypeSettings::cmbx_7z_ArchiveTypeChange(
      TObject *Sender)
{
    update7ZipPreview();
    SetProfileModified(Sender);
}
//---------------------------------------------------------------------------

