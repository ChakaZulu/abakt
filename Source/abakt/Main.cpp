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

#include "Main.h"
#include "About.h"
#include "Group.h"
#include "Action.h"
#include "RestoreBackup.h"
#include "Preferences.h"
#include "Gui_X_Profile.h"
#include "Gui_X_Globals.h"
#include "FilterPreview.h"
#include "CommandLine.h"
#include "DataModule.h"
#include "AbaktProfile.h"
#include "AbaktGlobals.h"
#include "eccFormMemory.h"
#include "eccFile.h"
#include "eccMisc.h"
#include "eccString.h"
#include "GlobalFuncs.h"
#include "TemplateEditor.h"
#include "FilterSetEditor.h"
#include "History.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SourceFrame"
#pragma link "FiltersFrame"
#pragma link "ActionTypeFrame"
#pragma link "ActionTypeSettingsFrame"
#pragma link "DestinationFrame"
#pragma link "FinalizeFrame"
#pragma resource "*.dfm"
Tf_Main *f_Main;
//---------------------------------------------------------------------------
static const String INI_SECT_APPLICATION = "Application";
static const String INI_SECT_HELP		 = "Help";
static const String INI_SECT_OTHER		 = "Other";
static const String INI_SECT_fr_Filters_FilterSetListView_Columns = "fr_Filters.FilterSetListView.Columns";
//---------------------------------------------------------------------------

__fastcall Tf_Main::Tf_Main(TComponent* Owner)
	: TForm(Owner)
{
	pgc_Action->ActivePage  = tab_ActionType;
    PageControl->TabHeight  = 1;
    PageControl->TabWidth   = 1;

	// Read Settings from .ini file:
	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	mem.ReadProps(this);

    // 095rc2: Minimize window if command line option -m is used:
    // ??? (Disregarding the -x option, which was required to make -m work in previous releases.) 
//if (PGlobals->cmdLine_Minimize)
//     WindowState = wsMinimized;

	// Always Show StatusBar (could be disabled in builds < 250)
	gbx_Status->Visible = true;
	Splitter->Visible 	= true;

	// Create TRecentFiles object:
	_recentFiles = new TRecentFiles(mi_RecentFiles, OnSelectRecentFile);

	// Create file associations and parse commandline parameters:
	Application->OnActivate = ApplicationActivate;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
    #endif
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::FormDestroy(TObject *Sender)
{
	// Destroy TRecentFiles object:
	delete _recentFiles;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::FormClose(TObject *Sender, TCloseAction &Action)
{
	try
	{
		_saveSettings();
	}
	catch(...)
	{
        // 092b7: Replaced ShowMessage() with statusError().
        statusError( ASPF(_("Failed to save your settings to:\n%s."),
                ecc::QuoteStr(PGlobals->getIniFilePath())) );
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::FormShow(TObject *Sender)
{
	static bool been_here = false;
	if (been_here) return;	// Needed when cmdline param -m (Minimize) is used.
	been_here = true;
    /*	This function is called when Visible is set to true.
	    Should only be called once, on application startup. */

	_ORGCaption = "Abakt " + PGlobals->humanVersionStr();
	status(_ORGCaption + ".");

    pnl_ExecuteButton->Width = pnl_ExecuteButton->Width + 2;  // 092b9: Force panel to redraw.

	PageControl->ActivePage = tab_Source;

	// Read settings from .ini file:
	_loadSettings();

	// Do this:
	PProfile->reset();
	PProfile->name = _("Untitled");
	PGlobals->DefaultDestDir = f_Prefs->ed_DefDestDir->Text;
	GlobalsToGui.all();

	// Display Profile's default values:
	ProfileToGui.all();
	GuiToProfile.setModified(false);

	// == Icons (Do it here to prevent BCB from including the bitmap twice in the executable):

    // Set the imagelist to use for the Main Menu:
    MainMenu->Images    = DM->imgs_Main;

    act_ExecuteProfile->ImageIndex  = ICON_EXECUTE; // Makes icon appear in the main menu.
    // -- File menu:
    act_FileNew->ImageIndex      = ICON_FILE_NEW;
    mi_FileNew->ImageIndex       = act_FileNew->ImageIndex;
    act_FileOpen->ImageIndex     = ICON_FILE_OPEN;
    mi_FileOpen->ImageIndex      = act_FileOpen->ImageIndex;
    act_FileSave->ImageIndex     = ICON_FILE_SAVE;
    mi_FileSave->ImageIndex      = act_FileSave->ImageIndex;
    // Attach _recentFiles menu to tb_FileOpen's DropdownMenu:
    tb_FileOpen->DropdownMenu    = _recentFiles->getPopupMenu();
    // -- View menu:
    act_EditSource->ImageIndex      = ICON_SOURCE;
    act_EditFilters->ImageIndex     = ICON_FILTERS;
    act_EditAction->ImageIndex      = ICON_ACTION;
    act_EditFinalize->ImageIndex    = ICON_FINALIZE;

    // -- Tools menu:
    act_Group->ImageIndex = ICON_GROUP;

    // == Fix them nasty BCB TFrame bugs on startup... ==
    // -- Shortcut for readability & convenience:
    TFrameActionTypeSettings* frACTSETS = f_Main->fr_ActionTypeSettings;

    frACTSETS->ed_7z_Preview->Width     = frACTSETS->gbx_7z_CommandLine->Width - frACTSETS->ed_7z_Preview->Left - 8;
    frACTSETS->bt_7z_Default->Left      = frACTSETS->gbx_7z_CommandLine->Width - frACTSETS->bt_7z_Default->Width - 8;
    frACTSETS->ed_7z_Template->Width    = frACTSETS->bt_7z_Default->Left - 8 - frACTSETS->ed_7z_Template->Left;
    frACTSETS->bt_Zip_CommentPreview->Left = frACTSETS->pnl_Zip_Comment->Width - frACTSETS->bt_Zip_CommentPreview->Width - 8;
    frACTSETS->ed_Zip_CommentTemplate->Width = frACTSETS->bt_Zip_CommentPreview->Left - 8 - frACTSETS->ed_Zip_CommentTemplate->Left;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_ExitExecute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (!PProfile->modified)
	{
		CanClose = true;
		return;
	}

	// Profile was modified:
	int val = _modifiedProfileDlg();

	switch (val)
	{
		case mrCancel:
			CanClose = false;
			break;

		case mrNo: // Ab.0164
			CanClose = true;
			break;

		default:
		case mrYes:
			if (PProfile->fileName.IsEmpty())
			{
				if (!_saveProfileDialog())
					CanClose = false;
			}
			else
			{
                GuiToProfile.all(); //093b1, reported by AH (28-apr-2006)
				if (PProfile->write(PProfile->fileName))
					CanClose = true;
				else
				{
					if (!_saveProfileDialog())
						CanClose = false;
				}
			}
			break;
	}
}
//---------------------------------------------------------------------------

int Tf_Main::_modifiedProfileDlg()
{
	return MessageDlg(
		_("Save modified Profile?"),
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_FileSaveExecute(TObject *Sender)
{
	if (PProfile->fileName.IsEmpty())
	{
		sdlg_Profile->FileName = ""; // Fixes a bug reported by FPSt on 26-aug-2004.
		act_FileSaveAs->Execute();
		return;
	}
	saveProfile(PProfile->fileName);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_FileSaveAsExecute(TObject *Sender)
{
	if (!sdlg_Profile->Execute())
		return;
	saveProfile(sdlg_Profile->FileName);
}
//--------------------------------------------------------------------------

void Tf_Main::saveProfile(String fileName)
{
	setCaption(ecc::ExtractFileNameNoExt(fileName));
	sdlg_Profile->FileName = ecc::ExtractFileNameNoExt(fileName);

	ecc::HourGlass(true);
	status( ASPF(_("Saving Profile %s..."), ecc::QuoteStr(fileName)) );

	GuiToProfile.all();
	PProfile->fileName = fileName;
	bool success = true;
	if (PProfile->write(fileName))
	{
		GuiToProfile.setModified(false);
		_latestOpenedProfile = fileName;
		_recentFiles->add(fileName);
		status(_("Ok."), -1);
	}
	else
	{
		GuiToProfile.setModified(true);
		status(_("Failed."), -1);
		ShowMessage( ASPF(_("Could not write to file %s"), ecc::QuoteStr(fileName)) +
            + "\n\n" + ASPF(_("Please try '%s'."), _("Save Profile As...")) );
		success = false;
	}

	if (success)
	{
		sdlg_Profile->InitialDir = ExtractFilePath(sdlg_Profile->FileName);
		odlg_Profile->InitialDir = ExtractFilePath(sdlg_Profile->FileName);
		fr_Destination->formattedNamePreview();
	}

	ecc::HourGlass(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_FileOpenExecute(TObject *Sender)
{
	if (PProfile->modified)
	{
		int val = _modifiedProfileDlg();
		if (val	== mrYes)
		{
			if (!act_FileSave->Execute()) return;
		}
	}
	if (!odlg_Profile->Execute()) return;

	String fname = odlg_Profile->FileName;
	odlg_Profile->InitialDir = ExtractFilePath(odlg_Profile->FileName);
	sdlg_Profile->InitialDir = ExtractFilePath(odlg_Profile->FileName);

	openProfileInGui(fname);
	_latestOpenedProfile = fname;
	_recentFiles->add(fname);
}
//---------------------------------------------------------------------------

bool Tf_Main::openProfile(String fileName)
{
	bool open_ok = true;
	status( ASPF(_("Opening Profile %s..."), ecc::QuoteStr(fileName)), 1);
	PageControl->ActivePage = tab_Source;

	if (PProfile->read(fileName))
	{
        if (PProfile->buildStr != PGlobals->AppBuildStr)
            status(ASPF(_("(Created with Abakt %s.%s)..."),
                PProfile->versionStr.c_str(), PProfile->buildStr.c_str()), -1);
		status(_("Ok."), -1);
        if (StrToInt(PProfile->buildStr) < 309) // 309 = 0.9.2-beta3 (FilterSets)
        {
    		doLog(ASPF(_("Profile has been upgraded to Abakt %s.%s."),
                PGlobals->AppVersionStr.c_str(), PGlobals->AppBuildStr.c_str()),
                    ecc::smtNotificationB);
    		doLog(_("Please check all settings, then save again using 'Save Profile As...'."),
                    ecc::smtNotificationB);
        }
	}
	else
	{
		statusError( _("Failed.") + " " + ASPF(_("Error: %s"), PProfile->getErrorStr()), -1);
		open_ok = false;
	}

	return open_ok;
}
//---------------------------------------------------------------------------

bool Tf_Main::openProfileInGui(String fname)
{
	if (!openProfile(fname)) return false;

    sdlg_Profile->FileName = fname;

    _recentFiles->add(fname); // Move file to top of list.

    ProfileToGui.all(); // Also verifies the SourceDir treeview node's states.

    setCaption(PProfile->name);
    fr_Destination->cmbx_NameTemplateExamples->ItemIndex = 0; // Set to "Choose...".
    GuiToProfile.setModified(false);

    f_FilterPreview->clearLists();

    if (fr_Source->tv_Dirs->Items->Count)
    {
        fr_Source->expandAllRootNodes(false);
        /* 	fr_Source->_redrawFileList() is called automatically by setting tv_SourceDirs->Selected to NULL
            and setting tv_SourceDirs->Selected to its first node.
            This is because the tv_SourceDirs::OnChange event is triggered. */
        fr_Source->tv_Dirs->Selected = NULL;
        fr_Source->tv_Dirs->Selected = fr_Source->tv_Dirs->Items->GetFirstNode();
        // Set focus to the treeview:
        ActiveControl = fr_Source->tv_Dirs;
    }

    _displayActiveActionTypeSettingTab();

	return true;
}
//---------------------------------------------------------------------------

void Tf_Main::setCaption(String name)
{
	Caption = name
        + " - " + _ORGCaption;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_ExecuteProfileExecute(TObject *Sender)
{
	status( ASPF(_("Executing Action. Using Profile %s..."),
        ecc::QuoteStr(PProfile->name)) );

	GuiToProfile.all();
	f_Action->actionStyle = asSingle;
	f_Action->ShowModal();  // Execute action.

    if (f_Action->failed())
        statusError(_("Failed."), -1);
    else
    {
        status(_("Ok."), -1);
        if (f_Action->abtFileList->errorItemList->Count > 0)
            statusWarn(" " + _("There were warnings."), -1);
    }

	// Refresh filelist (A-bits may have changed):
	fr_Source->act_RefreshFileListExecute(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_FileNewExecute(TObject *Sender)
{
	if (PProfile->modified)
	{
		int val = _modifiedProfileDlg();
		if (val	== mrYes)
		{
			if (!_saveProfileDialog()) return;
		}
		if (val	== mrCancel) return;
	}

	status(_("New Profile") + "...", 1);
	PProfile->reset();
    // 267+ Using default.abp as new profile (always stored in DefaultUserDataDir):
    String defaultAbp = IncludeTrailingBackslash(PGlobals->getDefaultUserDataDir())
            + "default.abp";
    if (FileExists(defaultAbp))
    {
        if (PProfile->read(defaultAbp))
        {
            status(ASPF(_("(Using %s)..."), ecc::QuoteStr(defaultAbp)), -1);
        }
    }
    PProfile->fileName  = "";
    PProfile->name      = _("Untitled");
	f_FilterPreview->clearLists();
	ProfileToGui.all();
	_displayActiveActionTypeSettingTab();
	fr_Destination->cmbx_NameTemplateExamples->ItemIndex = 0; // Set to "Choose...".
	GuiToProfile.setModified(false);
	fr_Source->lv_Files->Clear();
	status(_("Ok."), -1);
}
//---------------------------------------------------------------------------

bool Tf_Main::_saveProfileDialog()
{
	if (!sdlg_Profile->Execute()) return false;
	saveProfile(sdlg_Profile->FileName);
	return true;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_FileCloseExecute(TObject *Sender)
{
	act_FileNew->Execute();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_GroupExecute(TObject *Sender)
{
	if (PProfile->modified)
	{
		int val = MessageDlg(
			_("Current Profile has been modified.") + "\n"
			 + _("Changes made to this Profile will be lost if Group Actions are performed.")
			+ "\n\n" + _("Save Profile?"),
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

		if (val	== mrYes)
		{
			if (!act_FileSave->Execute()) return;
		}
		if (val	== mrCancel) return;
	}

	f_Group->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::mu_ViewClick(TObject *Sender)
{
	_showViewSourceMenuItems( (PageControl->ActivePage == tab_Source) );
}
//---------------------------------------------------------------------------

void Tf_Main::_showViewSourceMenuItems(bool show)
{
	mi_Source_RefreshDirectoryList->Visible = show;
	mi_Source_RefreshFileList->Visible = show;
	mi_Source_ScanTreeBelowDir->Visible = show;

	mi_Source_RefreshDirectoryList->Enabled = show;
	mi_Source_RefreshFileList->Enabled = show;
	mi_Source_ScanTreeBelowDir->Enabled = show;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_TemplateEditorExecute(TObject *Sender)
{
    f_TemplateEditor->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_RestoreZipArchiveExecute(TObject *Sender)
{
    f_RestoreBackup->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_EditHistoryExecute(TObject *Sender)
{
    GuiToProfile.all();
    f_History->ShowModal();
	ProfileToGui.all();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_PreferencesExecute(TObject *Sender)
{
	f_Prefs->ud_MaxRecentFiles->Position = _recentFiles->getMaxVisibleItemsCount();

	f_Prefs->ShowModal();

	// Update default directories:
	if (f_Prefs->defaultDirsModified)
		_setInitialDirsToDefault();

	// Update _recentFiles' properties:
	_recentFiles->setMaxVisibleItemsCount(f_Prefs->ud_MaxRecentFiles->Position);
	_recentFiles->setShowPath(f_Prefs->cb_RecentFilesShowPath->Checked);
	f_Group->recentFiles->setMaxVisibleItemsCount(f_Prefs->ud_MaxRecentFiles->Position);
	f_Group->recentFiles->setShowPath(f_Prefs->cb_RecentFilesShowPath->Checked);

	// Show the DescriptionBoxes?:
	_showDescriptionBoxes(PGlobals->ShowDescriptionBoxes);

	fr_Source->act_RefreshFileList->Execute();
}
//---------------------------------------------------------------------------

void Tf_Main::_showDescriptionBoxes(bool show)
{
    act_ToggleDescriptionBoxes->Checked         = show;

    // Shortcuts for readability & convenience:
    TFrameActionTypeSettings* frACTSETS = f_Main->fr_ActionTypeSettings;
    TFrameActionType* frACTTYPE         = f_Main->fr_ActionType;

	fr_Source->pnl_Descr_Source->Visible    = show;

	fr_Filters->pnl_Descr_Filters->Visible  = show;

	frACTTYPE->pnl_Descr_ActionType->Visible = show;

	frACTSETS->pnl_Descr_ActSets->Visible	= show;
	frACTSETS->pnl_Descr_actZip->Visible	= show;
	frACTSETS->pnl_Descr_act7Zip->Visible	= show;
	frACTSETS->pnl_Descr_actFile->Visible	= show;

	fr_Destination->pnl_Descr_Destination->Visible = show;

	fr_Finalize->pnl_Descr_Finalize->Visible    = show;

	f_FilterPreview->pnl_Descr->Visible     = show;

    f_Group->pnl_Descr_Profiles->Visible	        = show;
    f_Group->pnl_Descr_CustomConfiguration->Visible = show;
    f_Group->pnl_Descr_CustomFilterSets->Visible	= show;

    f_TemplateEditor->pnl_Descr_CustomConfiguration->Visible = show;
}
//---------------------------------------------------------------------------

void Tf_Main::_saveSettings()
{
	if (PGlobals->cmdLine_AutoExecute)
		return;	// Don't save settings.

	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	TIniFile* ini = mem.GetIniFile();

	mem.SetComponent(f_Main);
	mem.WriteProps("Top;Left;Width;Height");
	if (WindowState != wsMinimized) mem.WriteProps("WindowState");
	mem.WriteProps(gbx_Status, "Height");

	mem.SetComponent(fr_Source);
	mem.WriteProps(fr_Source->gbx_Dirs, "Width");
	mem.WriteProps(fr_Source->pmi_AlwaysShowEditor, "Checked");

   	// Section [Help]:
	ini->WriteString(INI_SECT_HELP, "HelpFile", PGlobals->getHelpFile());

	// Section [Other]:
	ini->WriteString(INI_SECT_OTHER, "LatestAddedSourceDir", fr_Source->latestAddedSourceDir);
	ini->WriteString(INI_SECT_OTHER, "LatestOpenedProfile", _latestOpenedProfile);

	// Save frSRC->lv_Files->Column sizes:
	for (int i = 0; i < fr_Source->lv_Files->Columns->Count; i++)
	{
    	ini->WriteInteger(fr_Source->Name + ".lv_Files.Columns",
            "Col" + IntToStr(i) + ".Width",
            fr_Source->lv_Files->Columns->Items[i]->Width);
    }

	// Save fr_Filters->fr_FilterSetList->lv->Column sizes:
	for (int i = 0; i < fr_Filters->fr_FilterSetList->lv->Columns->Count; i++)
	{
    	ini->WriteInteger(INI_SECT_fr_Filters_FilterSetListView_Columns,
            "Col" + IntToStr(i) + ".Width",
            fr_Filters->fr_FilterSetList->lv->Columns->Items[i]->Width);
    }

	// Save RecentFiles information:
	_recentFiles->toIniFile(PGlobals->getIniFilePath(), INI_SECT_RECENT_PROFILES);

	// Save Application information (231+):
	ini->WriteString(INI_SECT_APPLICATION, "Version", PGlobals->AppVersionStr);
	ini->WriteString(INI_SECT_APPLICATION, "Build", PGlobals->AppBuildStr);

	// Save f_Action settings:
	mem.SetComponent(f_Action);
	mem.WriteProps("Top;Left;Width;Height");
	if (f_Action->WindowState != wsMinimized) mem.WriteProps("WindowState");

	// Save f_FilterSetEditor settings:
	mem.SetComponent(f_FilterSetEditor);
	mem.WriteProps("Top;Left;Width;Height");
}
//---------------------------------------------------------------------------

void Tf_Main::_setInitialDirsToDefault()
{
	sdlg_Status->InitialDir		    	= PGlobals->ActionLogDir;
	odlg_Profile->InitialDir 		    = PGlobals->getProfilesDir();
	sdlg_Profile->InitialDir 	    	= PGlobals->getProfilesDir();
	f_Group->odlg_Group->InitialDir     = PGlobals->getGroupsDir();
	f_Group->sdlg_Group->InitialDir     = PGlobals->getGroupsDir();
	f_Group->odlg_Profiles->InitialDir 	= PGlobals->getProfilesDir();
    f_TemplateEditor->odlg_Template->InitialDir 	= PGlobals->getTemplatesDir();
    f_TemplateEditor->sdlg_Template->InitialDir 	= PGlobals->getTemplatesDir();
	if (fr_Destination->ed_Directory->Text.IsEmpty())
		fr_Destination->ed_Directory->Text = PGlobals->DefaultDestDir;
}
//---------------------------------------------------------------------------

void Tf_Main::_loadSettings()
{
	statusNotif( ASPF(_("Reading preferences from %s..."),
            ecc::QuoteStr(PGlobals->getIniFilePath()) ) );
	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	// Restore some form and frame settings:
	mem.ReadProps(fr_Source);
	mem.ReadProps(f_Action);
	mem.ReadProps(f_FilterSetEditor);

	_setInitialDirsToDefault();

	TIniFile* ini = mem.GetIniFile();

   	// Read Section [Help]:
	PGlobals->setHelpFile( ini->ReadString(INI_SECT_HELP, "HelpFile", PGlobals->getHelpFile()) );

	// Read Section [Other]:
	fr_Source->latestAddedSourceDir = ini->ReadString(INI_SECT_OTHER, "LatestAddedSourceDir", "");
	_latestOpenedProfile = ini->ReadString(INI_SECT_OTHER, "LatestOpenedProfile", "");

	// Read section [fr_Source.lv_Files.Columns]:
	String sect = fr_Source->Name + ".lv_Files.Columns";
	String ident;
	int cols = 0;
	while (1)
	{
		ident = "Col" + IntToStr(cols) + ".Width";
		if (!ini->ValueExists(sect, ident)) break;
		fr_Source->lv_Files->Columns->Items[cols]->Width = ini->ReadInteger(sect, ident, 50);
		++cols;
	}
    //  Adjust column widths for .ini's older than 0.9.2-beta3 (b309)
    if (cols == 5) // 0.9.2-beta3: added a new column (Age):
    {   // Set width of column FilterInfo to width of new column Age:
        fr_Source->lv_Files->Columns->Items[5]->Width = fr_Source->lv_Files->Columns->Items[4]->Width;
        fr_Source->lv_Files->Columns->Items[4]->Width = 40; // New Age column.
    }

	// Read column sizes:
	sect = INI_SECT_fr_Filters_FilterSetListView_Columns;
	cols = 0;
	while (1)
	{
		ident = "Col" + IntToStr(cols) + ".Width";
		if (!ini->ValueExists(sect, ident)) break;
		fr_Filters->fr_FilterSetList->lv->Columns->Items[cols]->Width = ini->ReadInteger(sect, ident, 50);
		++cols;
	}

	// Show the DescriptionBoxes?
	_showDescriptionBoxes(PGlobals->ShowDescriptionBoxes);

	// Read _recentFiles data:
	_recentFiles->fromIniFile(PGlobals->getIniFilePath(), INI_SECT_RECENT_PROFILES);
	f_Prefs->ud_MaxRecentFiles->Position = _recentFiles->getMaxVisibleItemsCount();
	f_Prefs->cb_RecentFilesShowPath->Checked = _recentFiles->getShowPath();

	status(_("Ok."), -1);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_ViewActionLogExecute(TObject *Sender)
{
	f_Action->actionStyle = asViewLog;
	f_Action->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_FilterPreviewExecute(TObject *Sender)
{
	f_FilterPreview->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::ApplicationActivate(TObject *Sender)
/*  This function is called AFTER ::Show(). */
{

	static bool been_here = false;
	if (been_here) return;
	been_here = true;

	if (!ParseCommandLine())
	{
		// Do this when NO commandline parameters are used:
		_doCreateFileAssociations();

        // 092rc3: _doCreateFileAssociations() will be performed only once, so disable it now:
        f_Prefs->cb_CreateFileAssociations->Checked = false;
        f_Prefs->saveToIniFile();

		Application->ProcessMessages();	// Let Abakt show itself.
		_doInitialProfile();
	}
}
//---------------------------------------------------------------------------

void Tf_Main::_doCreateFileAssociations()
{
	if (!f_Prefs->cb_CreateFileAssociations->Checked)
		return;

	ecc::HourGlass(true);

	ecc::CreateFileAssociation(".abp", _("Abakt Profile"), "Abakt", Application->ExeName, true);
	ecc::CreateFileAssociation(".abg", _("Abakt Group"),   "Abakt", Application->ExeName, true);
	ecc::CreateFileAssociation(".abh", _("Abakt History"), "Abakt", Application->ExeName, false);
	ecc::CreateFileAssociation(".abt", _("Abakt Template"),"Abakt", Application->ExeName, false);

	ecc::HourGlass(false);
}
//---------------------------------------------------------------------------

void Tf_Main::_doInitialProfile()
{
	// Start with favorite Profile:
	switch (PGlobals->InitialProfileType)
	{
		default:
		case 0:     // New Profile (default).
            act_FileNew->Execute();
			break;

		case 1:     // Previously opened/saved Profile.
			if (FileExists(_latestOpenedProfile))
				openProfileInGui(_latestOpenedProfile);
            else act_FileNew->Execute();
			break;

		case 2:     // Custom Profile.
			if (FileExists(PGlobals->CustomInitialProfile))
				openProfileInGui(PGlobals->CustomInitialProfile);
            else act_FileNew->Execute();
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_StatusSaveAsExecute(TObject *Sender)
{
	if (!sdlg_Status->Execute())
		return;
	lb_Status->Items->SaveToFile(sdlg_Status->FileName);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::SplitterCanResize(TObject *Sender, int &NewSize,
	  bool &Accept)
{
	if ((Height - NewSize) < 380)
		Accept = false;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::FormCanResize(TObject *Sender, int &NewWidth,
	  int &NewHeight, bool &Resize)
{
	if (NewHeight < Height)
	{
		if (NewHeight < Constraints->MinHeight)
			gbx_Status->Height = gbx_Status->Height - (Height - NewHeight);
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_EditSourceExecute(TObject *Sender)
{
	// Refresh Filelist:
	fr_Source->act_RefreshFileList->Execute();
	PageControl->ActivePage = tab_Source;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_EditFiltersExecute(TObject *Sender)
{
	PageControl->ActivePage = tab_Filters;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_EditActionExecute(TObject *Sender)
{
	PageControl->ActivePage = tab_Action;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_EditActionTypeExecute(TObject *Sender)
{
	pgc_Action->ActivePage  = tab_ActionType;
	PageControl->ActivePage = tab_Action;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_EditActionTypeSettingsExecute(TObject *Sender)
{
	pgc_Action->ActivePage  = tab_ActionTypeSettings;
	PageControl->ActivePage = tab_Action;
	_displayActiveActionTypeSettingTab();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_EditDestinationExecute(TObject *Sender)
{
	pgc_Action->ActivePage  = tab_Destination;
	PageControl->ActivePage = tab_Action;
    fr_Destination->cmbx_NameTemplateExamples->ItemIndex = 0;
	fr_Destination->formattedNamePreview();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_EditFinalizeExecute(TObject *Sender)
{
	PageControl->ActivePage = tab_Finalize;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::lb_StatusDrawItem(TWinControl *Control, int Index,
        TRect &Rect, TOwnerDrawState State)
{
	ecc::LBMessage_DrawItem(Control, Index, Rect, State);
}
//---------------------------------------------------------------------------

void Tf_Main::doLog(String msg, const ecc::TLBMessageStyle smt,
        const int lines_before, const int lines_after)
{
	// Log Status to ListBox:
	try
	{
    	ecc::LBMessage(lb_Status, msg, smt, lines_before, lines_after);
	}
	catch (Exception &ex)
	{
   		lb_Status->Clear();
		lb_Status->Items->Append(__FUNC__ ": Cleared Status panel: maximum size has been reached.");
	}

	// Log Status Messages to file (since 092b9):

    String appStatusFile = PGlobals->getDefaultUserDataDir() + "Abakt.log";
	try
	{
		if (PGlobals->SaveAppStatus)
		{
			abtAppendStrToFile(msg, appStatusFile, true, true);

            // Check size of AppStatus log file:
            TFileProperties fp(appStatusFile);
            if (!fp.failed())
            {
                int fileSize = (int)fp.getSize();    // Actual file size (in Bytes).
                int maxFileSize = 1024 * PGlobals->AppStatusMaxSize; // Max file size (in Bytes).
                // Truncate AppStatus log file if its size has grown beyond max size:
                if (fileSize >= (maxFileSize / 2))
                {
                    // Delete previous old logfile:
                    abtAppendStrToFile(ASPF("Renaming '%s' to '%s.old'.", appStatusFile, appStatusFile),
                        appStatusFile, true, true);
                    DeleteFile(appStatusFile + ".old");
                    // Rename logfile to logfile.old:
                    RenameFile(appStatusFile, appStatusFile + ".old");
                }
            }
		}
	}
	catch (Exception &ex)
	{
        statusError(__FUNC__ ": Could not save Status messages to: " + appStatusFile);
        statusError("- Disabled logging Status messages to file.");
        PGlobals->SaveAppStatus = false;
	}

	Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void Tf_Main::status(const String msg, int lines_before, int lines_after)
{
	doLog(msg, ecc::smtDefault, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void Tf_Main::statusNotif(const String msg, int lines_before, int lines_after)
{
	doLog(msg, ecc::smtNotification, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void Tf_Main::statusWarn(const String msg, int lines_before, int lines_after)
{
	doLog(msg, ecc::smtWarning, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void Tf_Main::statusError(const String msg, int lines_before, int lines_after)
{
	doLog(msg, ecc::smtError, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_ClearStatusExecute(TObject *Sender)
{
	lb_Status->Clear();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::mi_Source_RefreshDirectoryListClick(TObject *Sender)
{
	fr_Source->act_RefreshDirListExecute(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::mi_Source_RefreshFileListClick(TObject *Sender)
{
	fr_Source->act_RefreshFileListExecute(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::mi_Source_ScanTreeBelowDirClick(TObject *Sender)
{
	fr_Source->act_ScanTreeBelowDirExecute(Sender);
}
//---------------------------------------------------------------------------

void Tf_Main::OnSelectRecentFile(String fpath)
// This function is declared as static (use f_Main when appropriate).
{
	if (PProfile->modified)
	{
		int val = f_Main->_modifiedProfileDlg();
		if (val	== mrYes)
		{
			if (!f_Main->act_FileSave->Execute())
				return;
		}
	}
	f_Main->openProfileInGui(fpath);
	f_Main->odlg_Profile->InitialDir = ExtractFilePath(f_Main->odlg_Profile->FileName);
	f_Main->sdlg_Profile->InitialDir = ExtractFilePath(f_Main->odlg_Profile->FileName);
	f_Main->_latestOpenedProfile = fpath;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::mi_RecentFilesClick(TObject *Sender)
{
	_recentFiles->popupAt(Left + 20, Top + 50);
}
//---------------------------------------------------------------------------

void Tf_Main::_displayActiveActionTypeSettingTab()
{
    // Shortcut for readability & convenience:
    TFrameActionTypeSettings* frACTSETS = f_Main->fr_ActionTypeSettings;

    // Set tab's images to nill:
	frACTSETS->tab_Zip->ImageIndex	= -1;
	frACTSETS->tab_7Zip->ImageIndex = -1;
	frACTSETS->tab_File->ImageIndex = -1;

	GuiToProfile.actionType();

	TActionType at = (TActionType)PProfile->config.Action->eType->getIndex();
	switch (at)
	{
		case actZip:
			frACTSETS->pgc_ActionTypes->ActivePage = frACTSETS->tab_Zip;
			frACTSETS->tab_Zip->ImageIndex = ICON_CHECKED_NOBOX;
			break;

		case act7Zip:
			frACTSETS->pgc_ActionTypes->ActivePage = frACTSETS->tab_7Zip;
            frACTSETS->update7ZipPreview();
			frACTSETS->tab_7Zip->ImageIndex = ICON_CHECKED_NOBOX;
			break;

		case actFile:
			frACTSETS->pgc_ActionTypes->ActivePage = frACTSETS->tab_File;
			frACTSETS->tab_File->ImageIndex = ICON_CHECKED_NOBOX;
			break;
	}
}
//-------------------------------------- -------------------------------------

void __fastcall Tf_Main::tab_SourceShow(TObject *Sender)
{
    tb_Source->Down = true;
    // Adjust hotkey availability for objects that are (not) in focus:
    fr_Source->tbr_Directories->Enabled = true;
    fr_Source->tbr_FilterSets->Enabled  = true;
    fr_Source->tbr_FilterIcons->Enabled = true;
    fr_Source->ActionList->State        = asNormal;
    fr_Filters->fr_FilterSetList->enableToolBar(false);

    fr_Source->act_RefreshFileList->Execute();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::tab_FiltersShow(TObject *Sender)
{
    tb_Filters->Down = true;
    // Adjust hotkey availability for objects that are (not) in focus:
    fr_Source->tbr_Directories->Enabled = false;
    fr_Source->tbr_FilterSets->Enabled  = false;
    fr_Source->tbr_FilterIcons->Enabled = false;
    fr_Source->ActionList->State        = asSuspended;
    fr_Filters->fr_FilterSetList->enableToolBar(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::tab_ActionShow(TObject *Sender)
{
    tb_Action->Down = true;
    // Adjust hotkey availability for objects that are (not) in focus:
    fr_Source->tbr_Directories->Enabled = false;
    fr_Source->tbr_FilterSets->Enabled  = false;
    fr_Source->tbr_FilterIcons->Enabled = false;
    fr_Source->ActionList->State        = asSuspended;
    fr_Filters->fr_FilterSetList->enableToolBar(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::tab_FinalizeShow(TObject *Sender)
{
    tb_Finalize->Down = true;
    // Adjust hotkey availability for objects that are (not) in focus:
    fr_Source->tbr_Directories->Enabled = false;
    fr_Source->tbr_FilterSets->Enabled  = false;
    fr_Source->tbr_FilterIcons->Enabled = false;
    fr_Source->ActionList->State        = asSuspended;
    fr_Filters->fr_FilterSetList->enableToolBar(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::tab_ActionTypeSettingsShow(TObject *Sender)
{
	_displayActiveActionTypeSettingTab();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::tab_DestinationShow(TObject *Sender)
{
	// Adjust extension of Destination's fileName according to selected ActionType:
	if (!fr_Destination->ed_Directory->Text.IsEmpty())
	{
    	fr_Destination->ed_Directory->Text
            = IncludeTrailingPathDelimiter(fr_Destination->ed_Directory->Text);
    }
	fr_Destination->formattedNamePreview();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_ManualExecute(TObject *Sender)
{
	String url = PGlobals->getHelpFile();
    if (!FileExists(url))
    {
        ShowMessage(ASPF(_("File does not exist: %s"), ecc::QuoteStr(url)) );
        return;
    }
    HINSTANCE ret = ::ShellExecute(Handle, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	if ( (int)ret <= 32)    // The return value is an error value if <= 32 (see help).
        ShowMessage(ASPF(_("Could not open file: %s"), ecc::QuoteStr(url)) );
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::mi_SelectManualClick(TObject *Sender)
{
    odlg_Manual->InitialDir = ecc::AppPath() + "Help";
	if (!odlg_Manual->Execute()) return;

    PGlobals->setHelpFile(odlg_Manual->FileName);
	odlg_Profile->InitialDir = ExtractFilePath(odlg_Profile->FileName);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_ToggleDescriptionBoxesExecute(TObject *Sender)
{
    act_ToggleDescriptionBoxes->Checked         = !act_ToggleDescriptionBoxes->Checked;
    f_Prefs->cb_ShowDescriptionBoxes->Checked   = act_ToggleDescriptionBoxes->Checked;
    PGlobals->ShowDescriptionBoxes              = f_Prefs->cb_ShowDescriptionBoxes->Checked;

    _showDescriptionBoxes(PGlobals->ShowDescriptionBoxes);

    GuiToGlobals.all();
    f_Prefs->saveToIniFile();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::mi_AbaktWebsiteClick(TObject *Sender)
{
    f_About->pnl_InternetClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Main::act_AboutExecute(TObject *Sender)
{
	f_About->ShowModal();
}
//---------------------------------------------------------------------------

