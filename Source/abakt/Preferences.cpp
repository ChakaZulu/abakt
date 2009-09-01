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

#include "Preferences.h"
#include "Gui_X_Globals.h"
#include "AbaktGlobals.h"
#include "eccFormMemory.h"
#include "eccBrowseForDirectory.h"
#include "eccString.h"
#include "eccMisc.h"
#include "GlobalFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tf_Prefs *f_Prefs;
//---------------------------------------------------------------------------

__fastcall Tf_Prefs::Tf_Prefs(TComponent* Owner)
	: TForm(Owner)
{
    _availableLocales = new TStringList();

    gbx_Language->Visible =
    #ifdef ABT_TRANSLATE
        true;
    #else
        false;
    #endif

	PageControl->ActivePage	= tab_Application;
	ActiveControl = PageControl;

	ed_ActionLogDir->Text	= PGlobals->ActionLogDir;
	ed_DefDataDir->Text		= PGlobals->getDefaultUserDataDir();
	ed_DefDestDir->Text		= "";
	ed_TempDir->Text		= PGlobals->getTempDir();
	ed_ExePath7Zip->Text	= "";
	ed_CustomInitialProfile->Text = "";
	ed_PrefsFile->Text		= PGlobals->getIniFilePath();

    #ifdef ABT_TRANSLATE
        // Put the language codes from the 'locale' dir into a stringlist:
        DefaultInstance->GetListOfLanguages("default", _availableLocales);
        // Convert the locale id's to (translated) language names:
        cmbx_Languages->Items->Clear(); // redundant...
        for (int i = 0; i < _availableLocales->Count; i++)
        {
            cmbx_Languages->Items->Append(_(LocaleIdToLanguage(_availableLocales->Strings[i])));
        }
    #endif

	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	mem.ReadProps(this);

	// Copy the prefs to PGlobals:
	GuiToGlobals.all();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
    #endif
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::FormDestroy(TObject *Sender)
{
    delete _availableLocales;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::FormShow(TObject *Sender)
{
	defaultDirsModified = false;

    // Display selected language:
    cmbx_Languages->ItemIndex = _availableLocales->IndexOf(PGlobals->LocaleId);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_CancelClick(TObject *Sender)
{
	// Restore GUI from AbaktGlobals:
	GlobalsToGui.all();
}
//---------------------------------------------------------------------------

void Tf_Prefs::saveToIniFile()
{
	try
	{
		ecc::TFormMemory mem(PGlobals->getIniFilePath());

		mem.SetComponent(this);

		TIniFile* ini = mem.GetIniFile();
		ini->EraseSection(Name);    // Remove old settings.
		ini->UpdateFile();	        // Flush file to disk.

		mem.WriteProps("Top;Left;Width;Height");

		// - OnStartup:
		mem.WriteProps(rb_InitialProfileType, "ItemIndex");
		mem.WriteProps(ed_CustomInitialProfile, "Text");
		mem.WriteProps(cb_CreateFileAssociations, "Checked");
		// - Application:
		mem.WriteProps(ed_DefDataDir, "Text");
		mem.WriteProps(cb_ShowDescriptionBoxes, "Checked");
		// - Action:
		mem.WriteProps(ed_DefDestDir, "Text");
		mem.WriteProps(ed_TempDir, "Text");
		mem.WriteProps(ed_ExePath7Zip, "Text");
		mem.WriteProps(cb_OEMlistfile7Zip, "Checked");
		mem.WriteProps(cb_ActFile_ConfirmDelete, "Checked");
		// - Logging:
		mem.WriteProps(cb_LogBuildingFileLists, "Checked");
		mem.WriteProps(cb_LogFullAction, "Checked");
		mem.WriteProps(cb_AutoSaveActionLog, "Checked");
		mem.WriteProps(ed_ActionLogDir, "Text");
		mem.WriteProps(cb_SaveAppStatus, "Checked");
        mem.WriteProps(sped_MaxStatusLogSize, "Position");
		// - Advanced:
		mem.WriteProps(cb_RemoveNonExistingSourceDirs, "Checked");
		mem.WriteProps(cb_UseNewUIOnBFD, "Checked");
//removed 095b1:		mem.WriteProps(cb_IncludeEmptyDirs, "Checked");
        mem.WriteProps(cb_CreateTempZipCopy, "Checked");
        mem.WriteProps(cb_UseZipBuilderAddSafe, "Checked");
        mem.WriteProps(cb_TreatWarningsAsErrors, "Checked");

        // Language (only save when it has changed):
        #ifdef ABT_TRANSLATE
            if (PGlobals->LocaleId.IsEmpty())
            {
                PGlobals->LocaleId = "en";
            }
            if (cmbx_Languages->ItemIndex != -1)
            {
                String selectedLocId = _availableLocales->Strings[cmbx_Languages->ItemIndex];
                if (PGlobals->LocaleId != selectedLocId)
                {
                    ShowMessage( _("You need to restart Abakt for language changes to take effect.") );
                    ini->WriteString("Translations", "LocaleId", selectedLocId);
                }
            }
        #endif
	}
	catch(...)
	{
		ShowMessage( ASPF(_("Failed to save your settings to:\n%s."),
                ecc::QuoteStr(PGlobals->getIniFilePath())) );
	}

	// Copy prefs to AbaktGlobals:
	GuiToGlobals.all();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_OkClick(TObject *Sender)
{
	saveToIniFile();
	Close();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_DefDataDirClick(TObject *Sender)
{
	ecc::TBrowseForDirectory bd(PGlobals->UseNewUIOnBFD);
	bd.Handle		= Handle;
	bd.Directory	= ed_DefDataDir->Text;
	bd.Title		= _("Select Default Data Directory");

	if (bd.Execute())
	{
		PGlobals->UserDataDir = IncludeTrailingPathDelimiter(bd.Directory);

		ed_DefDataDir->Text = PGlobals->UserDataDir;
		ForceDirectories(PGlobals->getFiltersDir());
		ForceDirectories(PGlobals->getGroupsDir());
		ForceDirectories(PGlobals->getProfilesDir());
		ForceDirectories(PGlobals->getTemplatesDir());

		ShowMessage( _("The new default Data directories are:") + "\n\n"
			+ ecc::QuoteStr(PGlobals->getFiltersDir()) + "\n"
			+ ecc::QuoteStr(PGlobals->getGroupsDir()) + "\n"
			+ ecc::QuoteStr(PGlobals->getProfilesDir()) + "\n"
			+ ecc::QuoteStr(PGlobals->getTemplatesDir())// + "\n"
			);

		defaultDirsModified = true; // Let f_Main know dirs have changed.
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_ActionLogDirClick(TObject *Sender)
{
	ecc::TBrowseForDirectory bd(PGlobals->UseNewUIOnBFD);
	bd.Handle		= Handle;
	bd.Directory	= ed_ActionLogDir->Text;
	bd.Title		= _("Select") + " " + ecc::ReplaceStr(((TButton*)Sender)->Caption, "&", "");

	if (bd.Execute())
	{
		ed_ActionLogDir->Text = IncludeTrailingPathDelimiter(bd.Directory);
    	defaultDirsModified = true; // Let f_Main know dirs have changed.
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_DefDestDirClick(TObject *Sender)
{
	ecc::TBrowseForDirectory bd(PGlobals->UseNewUIOnBFD);
	bd.Handle		= Handle;
	bd.Directory	= ed_DefDestDir->Text;
	bd.Title		= _("Select") + " " + ecc::ReplaceStr(((TButton*)Sender)->Caption, "&", "");

	if (bd.Execute())
	{
		ed_DefDestDir->Text = IncludeTrailingPathDelimiter(bd.Directory);
		defaultDirsModified = true; // Let f_Main know dirs have changed.
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_TempDirClick(TObject *Sender)
{
	ecc::TBrowseForDirectory bd(PGlobals->UseNewUIOnBFD);
	bd.Handle		= Handle;
	bd.Directory	= ed_TempDir->Text;
	bd.Title		= _("Select") + " " + ecc::ReplaceStr(((TButton*)Sender)->Caption, "&", "");

	if (bd.Execute())
	{
		ed_TempDir->Text = bd.Directory;
		defaultDirsModified = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_DefaultTempDirClick(TObject *Sender)
{
	PGlobals->setTempDir( ecc::GetSystemFolder(ecc::sfTemp) );
	ed_TempDir->Text = PGlobals->getTempDir();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::lbl_Get7ZipClick(TObject *Sender)
{
    String url = "http://sourceforge.net/projects/sevenzip/";
	ShellExecute(Handle, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_7ZipExeClick(TObject *Sender)
{
	odlg_7ZipExe->InitialDir = ExtractFilePath(ed_ExePath7Zip->Text);
	if (!odlg_7ZipExe->Execute())
		return;

	ed_ExePath7Zip->Text = odlg_7ZipExe->FileName;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_SelectCustomInitialProfileClick(
	  TObject *Sender)
{
	if (!odlg_InitialProfile->Execute())
		return;

	ed_CustomInitialProfile->Text = odlg_InitialProfile->FileName;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::bt_HelpClick(TObject *Sender)
{
    String param = "mk:@MSITStore:" + PGlobals->getHelpFile() + "::/Preferences.html";
	ShellExecute(Handle, "open", "hh.exe", param.c_str(), NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Prefs::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Key == VK_F1)
        bt_Help->Click();
}
//---------------------------------------------------------------------------

