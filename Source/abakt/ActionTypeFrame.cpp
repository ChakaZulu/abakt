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
#pragma hdrstop

#include "ActionTypeFrame.h"
#include "Gui_X_Profile.h"
#include "AbaktProfile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameActionType *FrameActionType;
//---------------------------------------------------------------------------
__fastcall TFrameActionType::TFrameActionType(TComponent* Owner)
	: TFrame(Owner)
{
	ScrollBox_ActionType->BorderStyle = bsNone;
	rb_Zip->Checked = true;

    lbl_ArchiveModeInfo->Caption = PProfile->config.Action->eArchiveMode->info;
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionType::SetModified(TObject *Sender)
{
	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrameActionType::ModeModified(TObject *Sender)
{
	// Update Profile, then GUI:
	GuiToProfile.archiveMode();
	ProfileToGui.archiveMode();

	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------


