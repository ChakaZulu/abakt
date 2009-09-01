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

#include "FinalizeFrame.h"
#include "AbaktProfile.h"
#include "Gui_X_Profile.h"
#include "Main.h"
#include "AbaktHistory.h"
#include "GlobalFuncs.h"
#include "History.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameFinalize *FrameFinalize;
//---------------------------------------------------------------------------

__fastcall TFrameFinalize::TFrameFinalize(TComponent* Owner)
	: TFrame(Owner)
{
	ScrollBox_Finalize->BorderStyle   = bsNone;

    // Copy [ConfigSet]->info strings to GUI:
    // (The info strings have already been translated in AbaktConfigSets.cpp.)
    lbl_FileList_Enabled_Info->Caption  = PProfile->config.FileList->bEnabled->info;
    lbl_TestArchiveInfo->Caption        = PProfile->config.Action->bTestArchive->info;
	lbl_RemoveEmptyDirsInfo->Caption    = PProfile->config.Action->bRemoveEmptyDirs->info;
}
//---------------------------------------------------------------------------

void __fastcall TFrameFinalize::SetProfileModified(TObject *Sender)
{
	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

//===========================================================================
// History:
//===========================================================================

//---------------------------------------------------------------------------

void __fastcall TFrameFinalize::bt_HistoryClick(TObject *Sender)
{
    GuiToProfile.all();
    f_History->ShowModal();
	ProfileToGui.all();
}
//---------------------------------------------------------------------------

