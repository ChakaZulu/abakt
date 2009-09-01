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

#include "History.h"
#include "AbaktHistory.h"
#include "AbaktGlobals.h"
#include "GlobalFuncs.h"
#include "Gui_X_Profile.h"
#include "FileProperties.h"
#include "eccFile.h"
#include "eccFormMemory.h"
#include "eccMisc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tf_History *f_History;
//---------------------------------------------------------------------------
__fastcall Tf_History::Tf_History(TComponent* Owner)
    : TForm(Owner)
{
    PageControl->ActivePage = tab_ArchiveCatalogue;

    // Copy [ConfigSet]->info strings to GUI:
    // (The info strings have already been translated in AbaktConfigSets.cpp.)
//    lbl_HistoryMaintainInfo->Caption    = PProfile->config.History->bMaintain->caption;
    lbl_HistoryInfo->Caption            = PProfile->config.History->bMaintain->info;
	lbl_HistInfo_MaxDays->Caption       = PProfile->config.History->iMaxDays->info;
	lbl_HistInfo_MinBackups->Caption    = PProfile->config.History->iMinBackups->info;
	lbl_HistInfo_MaxBackups->Caption    = PProfile->config.History->iMaxBackups->info;
    cb_UseRecycleBin->Checked           = PProfile->config.History->bUseRecycleBin->value;

    // Read TForm settings from INI:
	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	mem.ReadProps(this);
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
    #endif
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    // Update Profile:
    GuiToProfile.all(); // (added in 0.9.4)

    // Write TForm settings to INI:
	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	mem.SetComponent(this);

    mem.WriteProps("Top;Left;Width;Height");
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::cb_MaintainHistoryClick(TObject *Sender)
{
//    PProfile->config.History->bMaintain->value = cb_MaintainHistory->Checked;
	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::tb_ShowFileListClick(TObject *Sender)
{
    GuiToProfile.all();
    lb->Clear();

	TAbaktHistory hist(*PProfile, lb);
	unsigned int size = hist.getSize();
	for (unsigned int i = 0; i < size; i++)
	{
		hist.log(ecc::QuoteStr(hist.getFileName(i)));
        if (!FileExists(hist.getFileName(i)))
			hist.log(". " + _("File not found."), -1);
		else
        {
			hist.log(". " + ASPF(_("Age: %d day(s). Archive Mode: %s."),
                hist.getAge(i), ArchiveModeToStr(hist.getArchiveMode(i))
                ), -1);
        }
	}
	hist.log( ASPF(_("Files listed in History: %d."), size) );
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::bt_AddFilesClick(TObject *Sender)
{
	odlg_HistoryFiles->InitialDir = PProfile->config.Destination->pDir->value;
	if (odlg_HistoryFiles->Execute())
	{
		lb->Items->Append("");
		// Add file(s) to history:
		TAbaktHistory hist(*PProfile, lb);
		for (int i = 0; i < odlg_HistoryFiles->Files->Count; i++)
		{
			hist.add(odlg_HistoryFiles->Files->Strings[i], (TArchiveMode)-1);
            // (TArchiveMode)-1: ArchiveMode is unknown for manually added files.
		}
		hist.write();
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::tb_ApplySettingsClick(TObject *Sender)
{
	lb->Items->Append("");

	GuiToProfile.all();
	TAbaktHistory hist(*PProfile, lb);
	hist.execute();
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::ed_History_KeyPress(TObject *Sender, char &Key)
{
    if ((Key == VK_DELETE) || (Key == VK_BACK))
    {   // Backspace and Delete are ok.
        GuiToProfile.setModified(true);
        return;
    }

    if ((Key < '0') || (Key > '9'))
        Key = NULL;
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::ed_MaxDaysChange(TObject *Sender)
{
    static String prevText = ed_MaxDays->Text;

    if (ed_MaxDays->Text.IsEmpty())
        ed_MaxDays->Text = "0";
    if (StrToInt(ed_MaxDays->Text) > ud_MaxDays->Max)
        ed_MaxDays->Text = IntToStr(ud_MaxDays->Max);

    // Prevent some weird automatic event.
    if (prevText != ed_MaxDays->Text)
        GuiToProfile.setModified(true);
    prevText = ed_MaxDays->Text;
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::ed_MinBackupsChange(TObject *Sender)
{
    static String prevText = ed_MinBackups->Text;

    if (ed_MinBackups->Text.IsEmpty())
        ed_MinBackups->Text = "0";
    if (StrToInt(ed_MinBackups->Text) > ud_MinBackups->Max)
        ed_MinBackups->Text = IntToStr(ud_MinBackups->Max);
    int newValue = StrToIntDef(ed_MinBackups->Text, ud_MinBackups->Position);
    if (newValue > ud_MaxBackups->Position)
        ud_MaxBackups->Position = newValue;

    // Prevent some weird automatic event.
    if (prevText != ed_MinBackups->Text)
        GuiToProfile.setModified(true);
    prevText = ed_MinBackups->Text;
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::ed_MaxBackupsChange(TObject *Sender)
{
    static String prevText = ed_MaxBackups->Text;

    if (ed_MaxBackups->Text.IsEmpty())
        ed_MaxBackups->Text = "0";
    if (StrToInt(ed_MaxBackups->Text) > ud_MaxBackups->Max)
        ed_MaxBackups->Text = IntToStr(ud_MaxBackups->Max);
    int newValue = StrToIntDef(ed_MaxBackups->Text, ud_MaxBackups->Position);
    if (newValue < ud_MinBackups->Position)
        ud_MinBackups->Position = newValue;

    // Prevent some weird automatic event.
    if (prevText != ed_MaxBackups->Text)
        GuiToProfile.setModified(true);
    prevText = ed_MaxBackups->Text;
}
//---------------------------------------------------------------------------

void __fastcall Tf_History::cb_UseRecycleBinClick(TObject *Sender)
{
	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

