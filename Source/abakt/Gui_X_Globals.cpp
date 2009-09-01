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

#include "Gui_X_Globals.h"
#include "Preferences.h"
#include "AbaktGlobals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

// Create global instances:
TGuiToGlobals	GuiToGlobals;
TGlobalsToGui	GlobalsToGui;
//---------------------------------------------------------------------------

//===========================================================================
// GuiToGlobals
//===========================================================================

void TGuiToGlobals::all()
{
	// === Settings modifiable by Preferences Window:

	// - OnStartup:
	PGlobals->InitialProfileType  	= f_Prefs->rb_InitialProfileType->ItemIndex;
	PGlobals->CustomInitialProfile	= f_Prefs->ed_CustomInitialProfile->Text;
	PGlobals->CreateFileAssociations = f_Prefs->cb_CreateFileAssociations->Checked;
	// - Application:
	PGlobals->UserDataDir			= f_Prefs->ed_DefDataDir->Text;
	PGlobals->ShowDescriptionBoxes	= f_Prefs->cb_ShowDescriptionBoxes->Checked;
    // RecentFiles settings are maintained in f_Main.
    // b.267+: Do not modify, is set at app startup:    PGlobals->LocaleId = f_Prefs->cmbx_Language->Text;
	// - Action:
	PGlobals->DefaultDestDir		= f_Prefs->ed_DefDestDir->Text;
	PGlobals->setTempDir(f_Prefs->ed_TempDir->Text);
	PGlobals->ExePath7Zip			= f_Prefs->ed_ExePath7Zip->Text;
    PGlobals->OEMlistfile7Zip       = f_Prefs->cb_OEMlistfile7Zip->Checked;
	PGlobals->ActFile_ConfirmDelete = f_Prefs->cb_ActFile_ConfirmDelete->Checked;
	// - Logging:
	PGlobals->LogBuildingFileLists	= f_Prefs->cb_LogBuildingFileLists->Checked;
	PGlobals->LogFullAction			= f_Prefs->cb_LogFullAction->Checked;
	PGlobals->ActionLogDir			= f_Prefs->ed_ActionLogDir->Text;
	PGlobals->AutoSaveActionLog		= f_Prefs->cb_AutoSaveActionLog->Checked;
    PGlobals->SaveAppStatus         = f_Prefs->cb_SaveAppStatus->Checked;
    PGlobals->AppStatusMaxSize      = f_Prefs->sped_MaxStatusLogSize->Position;
	// - Advanced:
	PGlobals->RemoveNonExistingSourceDirs = f_Prefs->cb_RemoveNonExistingSourceDirs->Checked;
	PGlobals->UseNewUIOnBFD 		= f_Prefs->cb_UseNewUIOnBFD->Checked;
//removed 095b1:    PGlobals->IncludeEmptyDirs      = f_Prefs->cb_IncludeEmptyDirs->Checked;
    PGlobals->CreateTempZipCopy     = f_Prefs->cb_CreateTempZipCopy->Checked;
    PGlobals->UseZipBuilderAddSafe  = f_Prefs->cb_UseZipBuilderAddSafe->Checked;
    PGlobals->TreatWarningsAsErrors = f_Prefs->cb_TreatWarningsAsErrors->Checked;
}
//---------------------------------------------------------------------------

//===========================================================================
// GlobalsToGui
//===========================================================================

void TGlobalsToGui::all()
{
	// === Settings modifiable by Preferences Window:

	// - OnStartup:
	f_Prefs->rb_InitialProfileType->ItemIndex = PGlobals->InitialProfileType;
	f_Prefs->ed_CustomInitialProfile->Text	= PGlobals->CustomInitialProfile;
	f_Prefs->cb_CreateFileAssociations->Checked	= PGlobals->CreateFileAssociations;
	// - Application:
	f_Prefs->ed_DefDataDir->Text			= PGlobals->UserDataDir;
	f_Prefs->cb_ShowDescriptionBoxes->Checked = PGlobals->ShowDescriptionBoxes;
    // RecentFiles settings are maintained in f_Main.
    // b.267+: Do not change (f_Prefs does it):     f_Prefs->cmbx_Language->Text = PGlobals->LocaleId;
	// - Action:
	f_Prefs->ed_DefDestDir->Text			= PGlobals->DefaultDestDir;
	f_Prefs->ed_TempDir->Text				= PGlobals->getTempDir();
	f_Prefs->ed_ExePath7Zip->Text			= PGlobals->ExePath7Zip;
    f_Prefs->cb_OEMlistfile7Zip->Checked    = PGlobals->OEMlistfile7Zip;
	f_Prefs->cb_ActFile_ConfirmDelete->Checked = PGlobals->ActFile_ConfirmDelete;
	// - Logging:
	f_Prefs->cb_LogBuildingFileLists->Checked = PGlobals->LogBuildingFileLists;
	f_Prefs->cb_LogFullAction->Checked		= PGlobals->LogFullAction;
	f_Prefs->cb_AutoSaveActionLog->Checked	= PGlobals->AutoSaveActionLog;
	f_Prefs->ed_ActionLogDir->Text			= PGlobals->ActionLogDir;
    f_Prefs->cb_SaveAppStatus->Checked      = PGlobals->SaveAppStatus;
    f_Prefs->sped_MaxStatusLogSize->Position = PGlobals->AppStatusMaxSize;
	// - Advanced:
	f_Prefs->cb_RemoveNonExistingSourceDirs->Checked =	PGlobals->RemoveNonExistingSourceDirs;
	f_Prefs->cb_UseNewUIOnBFD->Checked		= PGlobals->UseNewUIOnBFD;
//removed 095b1:    f_Prefs->cb_IncludeEmptyDirs->Checked   = PGlobals->IncludeEmptyDirs;
    f_Prefs->cb_CreateTempZipCopy->Checked  = PGlobals->CreateTempZipCopy;
    f_Prefs->cb_UseZipBuilderAddSafe->Checked   = PGlobals->UseZipBuilderAddSafe;
    f_Prefs->cb_TreatWarningsAsErrors->Checked  = PGlobals->TreatWarningsAsErrors;
}
//---------------------------------------------------------------------------

