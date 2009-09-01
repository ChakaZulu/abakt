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

#include "Group.h"
#include "Action.h"
#include "DataModule.h"
#include "AbaktGlobals.h"
#include "AbaktGroup.h"
#include "AbaktProfile.h"
#include "AbaktClasses.h"
#include "Xml_IO.h"
#include "eccFormMemory.h"
#include "eccFile.h"
#include "eccBrowseForDirectory.h"
#include "Main.h"
#include "GlobalFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ConfigEditFrame"
#pragma link "FilterSetListFrame"
#pragma resource "*.dfm"
Tf_Group *f_Group;
//---------------------------------------------------------------------------
static const String INI_SECT_Group_ConfigEditListView_Columns = "Group.ConfigEditListView.Columns";
static const String INI_SECT_Group_FilterSetListView_Columns = "Group.FilterSetListView.Columns";
//---------------------------------------------------------------------------

__fastcall Tf_Group::Tf_Group(TComponent* Owner)
	: TForm(Owner)
{
	ORG_Caption = Caption;
	PageControl->ActivePage = tab_Profiles;
    PageControl->TabHeight = 1;
    PageControl->TabWidth = 1;

	// Read form properties:
	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	mem.ReadProps(this);
	TIniFile* ini = mem.GetIniFile();

    // 092b7: Fix unpredictable EAccessViolation bug when opening an .ini file:
    ini->EraseSection("f_Group.lv_GroupItems.Columns");     // replaced with string that does not start with 'f_Group'.
    ini->EraseSection("f_Group.FilterSetListView.Columns"); // replaced with string that does not start with 'f_Group'.

	// === Read data from Abakt.ini (try-catch blocks since 092b7):
    String sect, ident;
    int cols;
	// Read section INI_SECT_Group_ConfigEditListView_Columns:
    try
    {
        sect = INI_SECT_Group_ConfigEditListView_Columns;
        cols = 0;
        while (1)
        {
            ident = "Col" + IntToStr(cols) + ".Width";
            if (!ini->ValueExists(sect, ident))
                break;
            fr_CustomConfigEdit->lv->Columns->Items[cols]->Width
                = ini->ReadInteger(sect, ident, 50);
            ++cols;
        }
    }
    catch (...)
    {
        #ifdef ABT_TEST
            ShowMessage(__FUNC__ " [1]: An error occurred! Please report.");
        #endif
    }

	// Read section INI_SECT_Group_FilterSetListView_Columns:
    try
    {
        sect = INI_SECT_Group_FilterSetListView_Columns;
        cols = 0;
        while (1)
        {
            ident = "Col" + IntToStr(cols) + ".Width";
            if (!ini->ValueExists(sect, ident))
                break;
            fr_CustomFilterSetList->lv->Columns->Items[cols]->Width = ini->ReadInteger(sect, ident, 50);
            ++cols;
        }
    }
    catch (...)
    {
        #ifdef ABT_TEST
            ShowMessage(__FUNC__ " [2]: An error occurred! Please report.");
        #endif
    }

	// Create TRecentFiles object:
	recentFiles = new TRecentFiles(mi_RecentFiles, OnSelectRecentFile);

    // Set callback to make fr_CustomConfigEdit notify f_Group that the config settings have changed:
    fr_CustomConfigEdit->OnModified = OnConfigModified;

	_groupToGui(); // Profile List
    fr_CustomConfigEdit->makeConfigList(PGroup->Config); //262+

    // Set callback to make fr_CustomFilterSetsListView notify f_Group that its _filterSetList has changed:
    fr_CustomFilterSetList->setOnModifiedCallback(OnConfigModified);

	setModified(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
    #endif
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::FormDestroy(TObject *Sender)
{
	// Destroy TRecentFiles object:
	delete recentFiles;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::FormClose(TObject *Sender,
	  TCloseAction &Action)
{
	try
	{
		ecc::TFormMemory mem(PGlobals->getIniFilePath());
		mem.SetComponent(this);
		TIniFile* ini = mem.GetIniFile();

		mem.WriteProps("Top;Left;Width;Height");
		mem.WriteProps("WindowState");

		// Save INI_SECT_Group_ConfigEditListView_Columns sizes:
		for (int i = 0; i < fr_CustomConfigEdit->lv->Columns->Count; i++)
			ini->WriteInteger(INI_SECT_Group_ConfigEditListView_Columns,
			"Col" + IntToStr(i) + ".Width", fr_CustomConfigEdit->lv->Columns->Items[i]->Width);

        // Save INI_SECT_Group_FilterSetListView_Columns sizes:
        for (int i = 0; i < fr_CustomFilterSetList->lv->Columns->Count; i++)
        {
            ini->WriteInteger(INI_SECT_Group_FilterSetListView_Columns,
                "Col" + IntToStr(i) + ".Width",
                fr_CustomFilterSetList->lv->Columns->Items[i]->Width);
        }

		recentFiles->toIniFile(PGlobals->getIniFilePath(), INI_SECT_RECENT_GROUPS);
	}
	catch(...)
	{
		ShowMessage( ASPF(_("Failed to save your settings to:\n%s."),
                ecc::QuoteStr(PGlobals->getIniFilePath())) );
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::FormShow(TObject *Sender)
{
	static bool been_here = false;
	if (been_here) return;
	been_here = true;
    /*	This function is called when Visible is set to true.
	    Should only be called once. */

	if (!PGlobals->cmdLine_ParametersPresent)
		_clearGroup();

    pnl_ExecuteButton->Width = pnl_ExecuteButton->Width + 2;  // 092b9: Force panel to redraw.

	recentFiles->fromIniFile(PGlobals->getIniFilePath(), INI_SECT_RECENT_GROUPS);

	// == Icons (Do it here to prevent BCB from including the bitmap twice in the executable):
    // -- File menu:
    act_FileNew->ImageIndex      = ICON_FILE_NEW;
    mi_FileNew->ImageIndex       = act_FileNew->ImageIndex;
    act_FileOpen->ImageIndex     = ICON_FILE_OPEN;
    mi_FileOpen->ImageIndex      = act_FileOpen->ImageIndex;
    act_FileSave->ImageIndex     = ICON_FILE_SAVE;
    mi_FileSave->ImageIndex      = act_FileSave->ImageIndex;
    // Attach recentFiles menu to tb_FileOpen's dropdownMenu:
    tb_FileOpen->DropdownMenu = recentFiles->getPopupMenu();
    // -- View menu:
    act_EditProfiles->ImageIndex            = ICON_SOURCE;
    act_EditCustomFilterSets->ImageIndex    = ICON_FILTERS;
    act_EditCustomConfiguration->ImageIndex = ICON_ACTION;
    // -- Tools menu:
    act_ExecuteGroup->ImageIndex = ICON_GROUP;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::FormCloseQuery(TObject *Sender,
	  bool &CanClose)
{
	if (PGroup->modified)
	{
		int val = MessageDlg( _("Group has been modified.")
            + "\n\n" + _("Save Group?"),
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
		if (val	== mrCancel) CanClose = false;
		if (val	== mrYes) act_FileSave->Execute();
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::mi_CloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tb_ProfileAddClick(TObject *Sender)
{
	if (odlg_Profiles->Execute())
	{
		for (int i = 0; i < odlg_Profiles->Files->Count; i++)
		{
			int idx = clb_Profiles->Items->Add(odlg_Profiles->Files->Strings[i]);
			clb_Profiles->Checked[idx] = true;
		}
		setModified(true);
		odlg_Profiles->InitialDir = ExtractFileDir(odlg_Profiles->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tb_ProfileRemoveClick(TObject *Sender)
{
	int idx = clb_Profiles->ItemIndex;
	if (idx < 0) return;
	clb_Profiles->Items->Delete(idx);
	setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tb_ProfileEditClick(TObject *Sender)
{
	int idx = clb_Profiles->ItemIndex;
	if (idx < 0) return;
    String profile = clb_Profiles->Items->Strings[idx];

    if ((PProfile->modified) && (PProfile->fileName != profile))
	{
    	int val = MessageDlg(
            ASPF(_("Active Profile %s has been modified."), ecc::QuoteStr(PProfile->name))
            + "\n\n" + _("Save Profile?"),
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
		if (val	== mrCancel)
            return;
		if (val	== mrYes)
            f_Main->act_FileSave->Execute();
    }

    if (PProfile->fileName == profile)
        Close();
    else
        if (f_Main->openProfileInGui(profile))
            Close();
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tb_ProfileAllClick(
	  TObject *Sender)
{
	for (int i = 0; i < clb_Profiles->Items->Count; i++)
		clb_Profiles->Checked[i] = true;
	setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tb_ProfileNoneClick(
	  TObject *Sender)
{
	for (int i = 0; i < clb_Profiles->Items->Count; i++)
		clb_Profiles->Checked[i] = false;
	setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_FileSaveExecute(
	  TObject *Sender)
{
	_guiToGroup();

	if (PGroup->fileName.IsEmpty())
	{
		act_FileSaveAs->Execute();
		return;
	}
	if (!PGroup->write(PGroup->fileName))
	{
		ShowMessage( ASPF(_("Could not write to file %s"), ecc::QuoteStr(PGroup->fileName)) );
		return;
	}
	_setGroupName(ecc::ExtractFileNameNoExt(PGroup->fileName));
	setModified(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_FileSaveAsExecute(
	  TObject *Sender)
{
	if (!sdlg_Group->Execute()) return;
	sdlg_Group->InitialDir = ExtractFileDir(sdlg_Group->FileName);
	odlg_Group->InitialDir = ExtractFileDir(sdlg_Group->FileName);

	PGroup->fileName = sdlg_Group->FileName;
	_guiToGroup();
	PGroup->name = ecc::ExtractFileNameNoExt(PGroup->fileName);
	_setGroupName(ecc::ExtractFileNameNoExt(PGroup->fileName));
	recentFiles->add(PGroup->fileName);
	if (!PGroup->write(PGroup->fileName))
	{
		ShowMessage( ASPF(_("Could not write to file %s"), ecc::QuoteStr(PGroup->fileName)) );
		return;
	}
	setModified(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_FileOpenExecute(
	  TObject *Sender)
{
	if (!_confirmOpenGroupInGUI()) return;
	if (!odlg_Group->Execute()) return;
	_openGroupInGUI(odlg_Group->FileName);
}
//---------------------------------------------------------------------------

bool Tf_Group::_confirmOpenGroupInGUI()
{
	if (PGroup->modified)
	{
		int val = MessageDlg( _("Group has been modified.")
            + "\n\n" + _("Save Group?"),
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
		if (val	== mrCancel) return false;
		if (val	== mrYes) act_FileSave->Execute();
	}
	return true;
}
//---------------------------------------------------------------------------

void Tf_Group::_openGroupInGUI(String fpath)
{
	odlg_Group->InitialDir = ExtractFileDir(fpath);
	sdlg_Group->InitialDir = ExtractFileDir(fpath);

	recentFiles->add(fpath);
	openGroup(fpath);
}
//---------------------------------------------------------------------------

bool Tf_Group::openGroup(String fpath)
{
	if (!PGroup->read(fpath))
        return false;

	_groupToGui(); // Profile List
    fr_CustomConfigEdit->makeConfigList(PGroup->Config); //262+

	_setGroupName(PGroup->name);
	setModified(false);
    PGroup->fileName = fpath;

    // Upgrade to 0.9.2-beta3.309:
 	if (PGroup->Build < 309)
	{
		if (!PGlobals->cmdLine_AutoExecute)
		{
    		ShowMessage(ASPF(_("Group has been upgraded to %s.%s. Check the settings and save again."),
                PGlobals->AppVersionStr.c_str(), PGlobals->AppBuildStr.c_str()));
			setModified(true);
		}
	}

	return true;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_FileNewExecute(
	  TObject *Sender)
{
	if (PGroup->modified)
	{
		int val = MessageDlg( _("Group has been modified.")
            + "\n\n" + _("Save Group?"),
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
		if (val	== mrYes)
		{
			if (!act_FileSave->Execute()) return;
		}
	}
	_clearGroup();
}
//---------------------------------------------------------------------------

void Tf_Group::_clearGroup()
{
	clb_Profiles->Clear();
	PGroup->fileName = "";

	PGroup->reset();

	_groupToGui(); // Profile List
    fr_CustomConfigEdit->makeConfigList(PGroup->Config); //262+

	setModified(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_ExecuteGroupExecute(
	  TObject *Sender)
{
	// Perform Group Backup:
	_guiToGroup();
	f_Action->actionStyle = asGroup;
	f_Action->ShowModal();
}
//---------------------------------------------------------------------------

void Tf_Group::setModified(bool modified)
{
	PGroup->modified = modified;
	act_FileSave->Enabled = modified;

	if (modified)
		_setGroupName(PGroup->name + "*");
	else
		_setGroupName(PGroup->name);
}
//---------------------------------------------------------------------------

void Tf_Group::_setGroupName(String name)
{
	Caption = name + " - " + _(ORG_Caption);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::SetGroupModified(TObject *Sender)
{
	setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_EditProfilesExecute(TObject *Sender)
{
    tb_ViewProfiles->Down = true;
	PageControl->ActivePage = tab_Profiles;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_EditCustomConfigurationExecute(
	  TObject *Sender)
{
    tb_ViewCustomConfiguration->Down = true;
	PageControl->ActivePage = tab_CustomConfiguration;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_EditCustomFilterSetsExecute(
      TObject *Sender)
{
    tb_ViewCustomFilterSets->Down = true;
	PageControl->ActivePage = tab_CustomFilterSets;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_ViewActionLogExecute(TObject *Sender)
{
	f_Action->actionStyle = asViewLog;
	f_Action->ShowModal();
}
//---------------------------------------------------------------------------

void Tf_Group::OnSelectRecentFile(String fpath)
// This function is declared as static.
{
	if (!f_Group->_confirmOpenGroupInGUI())
		return;
	f_Group->_openGroupInGUI(fpath);
}
//---------------------------------------------------------------------------

void Tf_Group::OnConfigModified(void)
// This function is declared as static.
{
	f_Group->setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::mi_RecentFilesClick(TObject *Sender)
{
	recentFiles->popupAt(Left + 20, Top + 50);
}
//---------------------------------------------------------------------------

void Tf_Group::_swapClbItems(TCheckListBox* clb, const int idx0, const int idx1)
{
    if (!clb) return;

    ecc::TCheckStringListItem item;
    item.str                    = clb->Items->Strings[idx0];
    item.checked                = clb->Checked[idx0];

    clb->Items->Strings[idx0]   = clb->Items->Strings[idx1];
    clb->Checked[idx0]          = clb->Checked[idx1];

    clb->Items->Strings[idx1]   = item.str;
    clb->Checked[idx1]          = item.checked;

    clb->ItemIndex              = idx1;
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tb_ProfileUpClick(TObject *Sender)
{
    TCheckListBox* clb = clb_Profiles;
    int idx = clb->ItemIndex;
    if (idx < 1) return;
    _swapClbItems(clb, idx, idx - 1);
	setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tb_ProfileDownClick(TObject *Sender)
{
    TCheckListBox* clb = clb_Profiles;
    int idx = clb->ItemIndex;
    if (idx >= clb->Count - 1) return;
    _swapClbItems(clb, idx, idx + 1);
	setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tb_ProfileSortClick(TObject *Sender)
{
    TCheckListBox* clb = clb_Profiles;
    clb->Sorted = true;
    clb->Sorted = false;
	setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tb_ProfileClearClick(TObject *Sender)
{
	if (MessageDlg( _("Clear list of Profiles?"),
            mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0 )
        != mrYes) return;

	clb_Profiles->Clear();
	setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tab_ProfilesShow(TObject *Sender)
{
    tb_ViewProfiles->Down = true;
    // Adjust hotkey availability for objects that are (not) in focus:
    tbar_Profiles->Enabled = true;
    fr_CustomFilterSetList->enableToolBar(false);
    fr_CustomConfigEdit->enableToolBar(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tab_CustomFilterSetsShow(TObject *Sender)
{
    tb_ViewCustomFilterSets->Down = true;
    // Adjust hotkey availability for objects that are (not) in focus:
    tbar_Profiles->Enabled = false;
    fr_CustomFilterSetList->enableToolBar(true);
    fr_CustomConfigEdit->enableToolBar(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::tab_CustomConfigurationShow(TObject *Sender)
{
    tb_ViewCustomConfiguration->Down = true;
    // Adjust hotkey availability for objects that are (not) in focus:
    tbar_Profiles->Enabled = false;
    fr_CustomFilterSetList->enableToolBar(false);
    fr_CustomConfigEdit->enableToolBar(true);
}
//---------------------------------------------------------------------------

void Tf_Group::_guiToGroup()
{
	PGroup->Version 	= PGlobals->AppVersionStr;
	PGroup->Build  		= StrToInt(PGlobals->AppBuildStr);

	// ProfileList:
	PGroup->profileList.clear();
	ecc::TCheckStringListItem cs_item;
	for (int i = 0; i < clb_Profiles->Items->Count; i++)
	{
		cs_item.checked = clb_Profiles->Checked[i];
		cs_item.str		= clb_Profiles->Items->Strings[i];
		PGroup->profileList.push_back(cs_item);
	}
}
//---------------------------------------------------------------------------

void Tf_Group::_groupToGui()
{
	// List of Profiles:
	clb_Profiles->Items->Clear();
	ecc::TCheckStringListItem cs_item;
	int item_idx;
	for (unsigned int i = 0; i < PGroup->profileList.size(); i++)
	{
		cs_item     = PGroup->profileList[i];
		item_idx    = f_Group->clb_Profiles->Items->Add(cs_item.str);
		clb_Profiles->Checked[item_idx] = cs_item.checked;
	}

	// List of custom FilterSets:
    fr_CustomFilterSetList->setFilterSetList(&PGroup->customFilterSetList);
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::clb_ProfilesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    switch (Key)
    {
        case VK_INSERT:
            tb_ProfileAddClick(Sender);
            break;

        case VK_DELETE:
            tb_ProfileRemoveClick(Sender);
            break;

        case VK_F2:
            tb_ProfileEditClick(NULL);
            break;

        case VK_UP:
            if (Shift.Contains(ssCtrl))
            {
                tb_ProfileUpClick(NULL);
                Key = 0;    // Required!
            }
            break;

        case VK_DOWN:
            if (Shift.Contains(ssCtrl))
            {
                tb_ProfileDownClick(NULL);
                Key = 0;    // Required!
            }
            break;

        default:
            break;
    }
}
//---------------------------------------------------------------------------

void __fastcall Tf_Group::act_ManualExecute(TObject *Sender)
{
    String param = "mk:@MSITStore:" + PGlobals->getHelpFile() + "::/Tools_Group.html";
	ShellExecute(Handle, "open", "hh.exe", param.c_str(), NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

