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

#include "Gui_X_Profile.h"
#include "Main.h"
#include "AbaktProfile.h"
#include "AbaktGlobals.h"
#include "eccString.h"
#include "eccMisc.h"
#include "eccFile.h"
#include "GlobalFuncs.h"
#include "History.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

// Create global instances:
TGuiToProfile	GuiToProfile;   // Declared as 'extern' in header file.
TProfileToGui	ProfileToGui;   // Declared as 'extern' in header file.
//---------------------------------------------------------------------------

//===========================================================================
// GuiToProfile
//===========================================================================

void TGuiToProfile::all()
{
	PProfile->versionStr    = PGlobals->AppVersionStr;
	PProfile->buildStr   	= PGlobals->AppBuildStr;

	// == Action(Type):
	actionType();	// Includes methods for actFile.
	_actionTypeSettings();
	archiveMode();  // Includes FilePropsFilter.A_AttrFilterMethod and FClearArchiveAttr.
	PProfile->config.Action->bIncludeEmptySourceDirectories->value
        = f_Main->fr_ActionType->cb_IncludeEmptySourceDirectories->Checked;

	// == Destination:
    _destination();

	// == Finalize - History window:
	PProfile->config.History->bMaintain->value			= f_History->cb_MaintainHistory->Checked;
	PProfile->config.History->iMaxDays->set( f_History->ud_MaxDays->Position );
	PProfile->config.History->iMinBackups->set( f_History->ud_MinBackups->Position );
	PProfile->config.History->iMaxBackups->set( f_History->ud_MaxBackups->Position );
	PProfile->config.History->bUseRecycleBin->value	   	= f_History->cb_UseRecycleBin->Checked;
	// == Finalize - FileList:
	PProfile->config.FileList->bEnabled->value          = f_Main->fr_Finalize->cb_FileList_Enabled->Checked;
	// == Finalize - others... (Action's Finalize settings):
	PProfile->config.Action->bRemoveEmptyDirs->value    = f_Main->fr_Finalize->cb_RemoveEmptyDirs->Checked;
	PProfile->config.Action->bTestArchive->value        = f_Main->fr_Finalize->cb_TestArchive->Checked;

	sourceDirLists();
}
//---------------------------------------------------------------------------

void TGuiToProfile::_destination()
{
	if (!f_Main->fr_Destination->ed_Directory->Text.IsEmpty())
		f_Main->fr_Destination->ed_Directory->Text
            = IncludeTrailingPathDelimiter(f_Main->fr_Destination->ed_Directory->Text);
	PProfile->config.Destination->pDir->value			   	= f_Main->fr_Destination->ed_Directory->Text.c_str();
	PProfile->config.Destination->bAutoCreateDir->value		= f_Main->fr_Destination->cb_AutoCreateDir->Checked;
	PProfile->config.Destination->tNameTemplate->value	   	= f_Main->fr_Destination->ed_NameTemplate->Text.c_str();
	PProfile->config.Destination->bAppendExtension->value 	= f_Main->fr_Destination->cb_AppendExtension->Checked;
}
//---------------------------------------------------------------------------

void TGuiToProfile::actionType()
{
    // Shortcut for readability & convenience:
    TFrameActionType* frACTTYPE         = f_Main->fr_ActionType;

	// Archive:
	if (frACTTYPE->rb_Zip->Checked)
	{
		PProfile->config.Action->eType->setIndex((int)actZip);
	}
	else if (frACTTYPE->rb_7Zip->Checked)
	{
		PProfile->config.Action->eType->setIndex((int)act7Zip);
	}
	// File Operation:
	else if (frACTTYPE->rb_File_Copy->Checked)
	{
		PProfile->config.Action->eType->setIndex((int)actFile);
		PProfile->config.Actions.File->eMethod->setIndex((int)afmCopy);
	}
	else if (frACTTYPE->rb_File_Move->Checked)
	{
		PProfile->config.Action->eType->setIndex((int)actFile);
		PProfile->config.Actions.File->eMethod->setIndex((int)afmMove);
	}
	else if (frACTTYPE->rb_File_Delete->Checked)
	{
		PProfile->config.Action->eType->setIndex((int)actFile);
		PProfile->config.Actions.File->eMethod->setIndex((int)afmDelete);
	}
	// Undefined. Use default:
	else
	{
		PProfile->config.Action->eType->setIndex((int)actZip);
	}
}
//---------------------------------------------------------------------------

void TGuiToProfile::_actionTypeSettings()
{
    // Shortcut for readability & convenience:
    TFrameActionTypeSettings* frACTSETS = f_Main->fr_ActionTypeSettings;

    // Shortcut for readability:
    abakt::TConfig::TActions* act = &PProfile->config.Actions;
    #ifdef ABT_TEST
        if (act == NULL) ShowMessage(__FUNC__ ": [act] can not be NULL");
    #endif

	// Zip:
	/*	The cmbx_Zip_... objects get their values from act->Zip,
		so it is safe to use setIndex(). */
	act->Zip->eMethod->setIndex(        _actZipMethod() );
	act->Zip->eStorePath->setIndex(     (TActZipStorePath)frACTSETS->cmbx_Zip_StorePath->ItemIndex );
	act->Zip->iCompression->set(        frACTSETS->ud_Zip_Compression->Position );
	act->Zip->tPassword->value	        = frACTSETS->ed_Zip_Password->Text;
	act->Zip->bSplit->value		        = frACTSETS->cb_Zip_Split->Checked;
	act->Zip->eSplitPreset->setIndex(   frACTSETS->cmbx_Zip_SplitVolumeSizePreset->ItemIndex );
	act->Zip->iSplitCustomSize->set(
        StrToIntDef(frACTSETS->ed_Zip_SplitCustomSize->Text, act->Zip->iSplitCustomSize->getMin()) );
	act->Zip->bAddComment->value        = frACTSETS->cb_Zip_AddComment->Checked;
	act->Zip->tCommentTemplate->value   = frACTSETS->ed_Zip_CommentTemplate->Text;

	// 7-Zip:
	act->SevenZip->tCommandLineTemplate->value  = frACTSETS->ed_7z_Template->Text;
	act->SevenZip->tCommand->value              = frACTSETS->ed_7z_Command->Text;
	act->SevenZip->tArchiveType->value          = frACTSETS->cmbx_7z_ArchiveType->Text;
	act->SevenZip->tPassword->value             = frACTSETS->ed_7z_Password->Text;

	// File:
	act->File->bOverwrite->value            = frACTSETS->cb_File_Overwrite->Checked;
	act->File->bOverwriteReadOnly->value    = frACTSETS->cb_File_OverwriteReadOnly->Checked;
	act->File->bUseRecycleBin->value        = frACTSETS->cb_File_UseRecycleBin->Checked;
//removed 095b1:	act->File->bUseSHFileOperation->value   = frACTSETS->cb_File_UseSHFileOperation->Checked;
	act->File->ePathInformation->setIndex(  (TActZipStorePath)frACTSETS->cmbx_File_PathInformation->ItemIndex );
}
//---------------------------------------------------------------------------

void TGuiToProfile::archiveMode()
{
    // Shortcut for readability & convenience:
    TFrameActionType* frACTTYPE         = f_Main->fr_ActionType;

	if (frACTTYPE->rb_AMode_Full->Checked)
		PProfile->config.Action->eArchiveMode->setIndex((int)amFull);
	else if (frACTTYPE->rb_AMode_Differential->Checked)
		PProfile->config.Action->eArchiveMode->setIndex((int)amDifferential);
	else if (frACTTYPE->rb_AMode_Incremental->Checked)
		PProfile->config.Action->eArchiveMode->setIndex((int)amIncremental);
	else if (frACTTYPE->rb_AMode_Inverse->Checked)
		PProfile->config.Action->eArchiveMode->setIndex((int)amInverse);
	// Undefined, choose default:
	else
		PProfile->config.Action->eArchiveMode->setIndex((int)amDefault);
}
//---------------------------------------------------------------------------

TActZipMethod TGuiToProfile::_actZipMethod()
{
    // Shortcut for readability & convenience:
    TFrameActionTypeSettings* frACTSETS = f_Main->fr_ActionTypeSettings;

	if (frACTSETS->rb_Zip_MethodNew->Checked)
		return azmNew;
	if (frACTSETS->rb_Zip_MethodReplace->Checked)
		return azmReplace;
	if (frACTSETS->rb_Zip_MethodUpdate->Checked)
		return azmUpdate;
	if (frACTSETS->rb_Zip_MethodFreshen->Checked)
		return azmFreshen;
	return azmNew;	// Default.
}
//---------------------------------------------------------------------------

void TGuiToProfile::_sourceRootDirList()
{
	PProfile->sourceRootDirList.clear();

	TTreeNode* node = f_Main->fr_Source->tv_Dirs->Items->GetFirstNode();
	while (node)
	{
		PProfile->sourceRootDirList.push_back(IncludeTrailingPathDelimiter(node->Text));
		node = node->getNextSibling();
	}
}
//---------------------------------------------------------------------------

void TGuiToProfile::sourceDirLists()
{
	_sourceRootDirList();
	PProfile->excludeDirList.clear();
	PProfile->includeDirList.clear();
	PProfile->excludeTreeList.clear();
	PProfile->includeTreeList.clear();
	String rootdir;

	TTreeNode* node = f_Main->fr_Source->tv_Dirs->Items->GetFirstNode();
	while (node)
	{
		_addSourceDirToList(node, rootdir, false);
		_sourceDirLists2(node->getFirstChild(),
            IncludeTrailingPathDelimiter(rootdir + node->Text));
		node = node->getNextSibling();
	}
}
//---------------------------------------------------------------------------

void TGuiToProfile::_sourceDirLists2(TTreeNode* node, String dir)
{
	// rootdir is assumed to IncludeTrailingPathDelimiter
	while (node)
	{
		_addSourceDirToList(node, dir);
		_sourceDirLists2( node->getFirstChild(),	dir + node->Text + "\\" ); // Recurse.
		node = node->GetNextChild(node);
	}
}
//---------------------------------------------------------------------------

void TGuiToProfile::_addSourceDirToList(TTreeNode* node, String dir, const bool include_backslash)
{
	// Root source dirs do not have a backslash.
	String bslash = (include_backslash) ? "\\" : "";
	String node_text = node->Text;

	switch (node->StateIndex)
	{
		case nsGrayUnChecked:
			PProfile->excludeDirList.push_back(dir + node_text + bslash);
			break;

		case nsGrayChecked:
			PProfile->includeDirList.push_back(dir + node_text + bslash);
			break;

		case nsUnChecked:
			if (!PProfile->dirIsPartOfExcludeTree(dir))
				PProfile->excludeTreeList.push_back(dir + node_text + bslash);
			break;

		case nsChecked:
			if (!PProfile->dirIsPartOfIncludeTree(dir))
				PProfile->includeTreeList.push_back(dir + node_text + bslash);
			break;
	}
}
//---------------------------------------------------------------------------

void TGuiToProfile::setModified(const bool modified)
{
	PProfile->modified = modified;
	f_Main->act_FileSave->Enabled = modified;

	if (modified)
		f_Main->setCaption(PProfile->name + "*");
	else
		f_Main->setCaption(PProfile->name);
}
//---------------------------------------------------------------------------

//===========================================================================
// ProfileToGui
//===========================================================================

void TProfileToGui::all()
{
    // Shortcut for readability & convenience:
    TFrameActionTypeSettings* frACTSETS = f_Main->fr_ActionTypeSettings;

	// == Action (ActionType, ArchiveMode and a few Finalize settings):
	_actionType();	// Includes methods for actFile.
	archiveMode();
	f_Main->fr_ActionType->cb_IncludeEmptySourceDirectories->Checked
        = PProfile->config.Action->bIncludeEmptySourceDirectories->value;

	// == Zip:

    // Shortcut for readability:
    abakt::TConfig::TActions* act = &PProfile->config.Actions;
    #ifdef ABT_TEST
        if (act == NULL) ShowMessage(__FUNC__ ": [act] can not be NULL");
    #endif

	// The cmbx_Zip_... objects get their values from act->Zip, so it is safe to use getIndex().
	_actZipMethod((TActZipMethod)act->Zip->eMethod->getIndex());
	frACTSETS->cmbx_Zip_StorePath->ItemIndex		= (int)act->Zip->eStorePath->getIndex();
	frACTSETS->ud_Zip_Compression->Position	= act->Zip->iCompression->get();
//	frACTSETS->cmbx_Zip_64Mode->ItemIndex		= (int)act->Zip->eZip64Mode->getIndex();
	frACTSETS->ed_Zip_Password->Text		   	= act->Zip->tPassword->value;
	frACTSETS->cb_Zip_Split->Checked	 	  	= act->Zip->bSplit->value;
	frACTSETS->cmbx_Zip_SplitVolumeSizePreset->ItemIndex = act->Zip->eSplitPreset->getIndex();
	frACTSETS->ed_Zip_SplitCustomSize->Text		= IntToStr(act->Zip->iSplitCustomSize->get());
	frACTSETS->cb_Zip_AddComment->Checked       = act->Zip->bAddComment->value;
	frACTSETS->ed_Zip_CommentTemplate->Text     = act->Zip->tCommentTemplate->value;

	// == 7-Zip:
	frACTSETS->ed_7z_Template->Text = act->SevenZip->tCommandLineTemplate->value;
	frACTSETS->ed_7z_Command->Text  = act->SevenZip->tCommand->value;
//	frACTSETS->cmbx_7z_ArchiveType->ItemIndex	= act->SevenZip->eArchiveType->getIndex();
	frACTSETS->cmbx_7z_ArchiveType->Text = act->SevenZip->tArchiveType->value;
	frACTSETS->ed_7z_Password->Text = act->SevenZip->tPassword->value;

	// == File:
	frACTSETS->cb_File_Overwrite->Checked  		    = act->File->bOverwrite->value;
	frACTSETS->cb_File_OverwriteReadOnly->Checked   = act->File->bOverwriteReadOnly->value;
	frACTSETS->cb_File_UseRecycleBin->Checked 	    = act->File->bUseRecycleBin->value;
//removed 095b1:	frACTSETS->cb_File_UseSHFileOperation->Checked  = act->File->bUseSHFileOperation->value;
    frACTSETS->cmbx_File_PathInformation->ItemIndex = (int)act->File->ePathInformation->getIndex();

	// == Destination
	f_Main->fr_Destination->ed_Directory->Text     = PProfile->config.Destination->pDir->value;
	f_Main->fr_Destination->cb_AutoCreateDir->Checked   = PProfile->config.Destination->bAutoCreateDir->value;
	f_Main->fr_Destination->ed_NameTemplate->Text       = PProfile->config.Destination->tNameTemplate->value;
	f_Main->fr_Destination->cb_AppendExtension->Checked = PProfile->config.Destination->bAppendExtension->value;

	// == Finalize - History:
    String finHist = ASPF(_("Enabled: %s"),
        PProfile->config.History->bMaintain->value ? String(_("Yes")).c_str() :  String(_("No")).c_str() );
	f_Main->fr_Finalize->lbl_HistoryEnabled->Caption    = finHist;
	// ... History window:
	f_History->cb_MaintainHistory->Checked  = PProfile->config.History->bMaintain->value;
	f_History->ud_MaxDays->Position		    = PProfile->config.History->iMaxDays->get();
	f_History->ud_MinBackups->Position		= PProfile->config.History->iMinBackups->get();
	f_History->ud_MaxBackups->Position		= PProfile->config.History->iMaxBackups->get();
	f_History->cb_UseRecycleBin->Checked    = PProfile->config.History->bUseRecycleBin->value;
	// == Finalize - FileList:
	f_Main->fr_Finalize->cb_FileList_Enabled->Checked   = PProfile->config.FileList->bEnabled->value;
	// == Finalize - others... (Action's Finalize settings):
	f_Main->fr_Finalize->cb_RemoveEmptyDirs->Checked    = PProfile->config.Action->bRemoveEmptyDirs->value;
	f_Main->fr_Finalize->cb_TestArchive->Checked        = PProfile->config.Action->bTestArchive->value;

	sourceDirLists();
	filters();
}
//---------------------------------------------------------------------------

void TProfileToGui::_actionType()
{
    // Shortcut for readability & convenience:
    TFrameActionType* frACTTYPE         = f_Main->fr_ActionType;

    TActionType act = (TActionType)PProfile->config.Action->eType->getIndex();
	switch (act)
	{
		default:
		case actZip:
			frACTTYPE->rb_Zip->Checked = true;
			break;

		case act7Zip:
			frACTTYPE->rb_7Zip->Checked = true;
			break;

		case actFile:
			switch ((TActFileMethod)PProfile->config.Actions.File->eMethod->getIndex())
			{
				default:
				case afmCopy:
					frACTTYPE->rb_File_Copy->Checked = true;
					break;
				case afmMove:
					frACTTYPE->rb_File_Move->Checked = true;
					break;
				case afmDelete:
					frACTTYPE->rb_File_Delete->Checked = true;
					break;
			}
			break;
	}
}
//---------------------------------------------------------------------------

void TProfileToGui::archiveMode()
{
    // Shortcut for readability & convenience:
    TFrameActionType* frACTTYPE         = f_Main->fr_ActionType;

	switch( (TArchiveMode)PProfile->config.Action->eArchiveMode->getIndex() )
	{
		default:
		case amDefault:
			frACTTYPE->rb_AMode_Default->Checked = true;
			break;

		case amFull:
			frACTTYPE->rb_AMode_Full->Checked = true;
			break;

		case amDifferential:
			frACTTYPE->rb_AMode_Differential->Checked = true;
			break;

		case amIncremental:
			frACTTYPE->rb_AMode_Incremental->Checked = true;
			break;

		case amInverse:
			frACTTYPE->rb_AMode_Inverse->Checked = true;
			break;
	}
}
//---------------------------------------------------------------------------

void TProfileToGui::_actZipMethod(TActZipMethod method)
{
    // Shortcut for readability & convenience:
    TFrameActionTypeSettings* frACTSETS = f_Main->fr_ActionTypeSettings;

	switch (method)
	{
		default:
		case azmNew:
			frACTSETS->rb_Zip_MethodNew->Checked = true;
			break;

		case azmReplace:
			frACTSETS->rb_Zip_MethodReplace->Checked = true;
			break;

		case azmUpdate:
			frACTSETS->rb_Zip_MethodUpdate->Checked = true;
			break;

		case azmFreshen:
			frACTSETS->rb_Zip_MethodFreshen->Checked = true;
			break;
	}
}
//---------------------------------------------------------------------------

void TProfileToGui::sourceDirLists()
{
	String dir;
	TTreeNode* node;

	f_Main->fr_Source->tv_Dirs->Items->Clear();
	ecc::HourGlass(true);
	f_Main->fr_Source->tv_Dirs->Items->BeginUpdate();

	for (unsigned int i = 0; i < PProfile->sourceRootDirList.size(); i++)
	{
		dir = PProfile->sourceRootDirList[i];
		if (!DirectoryExists(dir))
		{
			f_Main->statusWarn(ASPF(_("Source Root Directory does not exist: %s."),
                ecc::QuoteStr(dir)) );
			continue;
		}

		// Create new rootnode:
		if (f_Main->fr_Source->rootNodeDirExists(dir)) continue;	// Fixes bug: re-adding when cmdline -m.

		node = f_Main->fr_Source->tv_Dirs->Items->Add(NULL, dir);
		node->StateIndex = nsChecked; // Default.

		_listToTreeView(PProfile->excludeDirList, nsGrayUnChecked, node, dir);
		_listToTreeView(PProfile->includeDirList, nsGrayChecked, node, dir);
		_listToTreeView(PProfile->excludeTreeList, nsUnChecked, node, dir);
		_listToTreeView(PProfile->includeTreeList, nsChecked, node, dir);
		f_Main->fr_Source->addSubDirs(node, dir);
		/* 	Just to be sure:
			Correct the states of ALL nodes below this rootnode.
			Fixes the states if a dir was stored in the profile but does not exist.
			And helps updating an older Profile. */
		f_Main->fr_Source->validateStateAllChildren(node);
	}

	f_Main->fr_Source->tv_Dirs->AlphaSort();
	f_Main->fr_Source->expandAllRootNodes(false);
	f_Main->fr_Source->tv_Dirs->Items->EndUpdate();
	ecc::HourGlass(false);
}
//---------------------------------------------------------------------------

void TProfileToGui::_listToTreeView(std::vector<String> &list, const TNodeStateIndex state,
		TTreeNode* rootnode, String rootdir)
{
	rootdir = IncludeTrailingPathDelimiter(rootdir);
	TTreeNode *node, *nod;

	_removeNonExistentDirsFromList(list);

	int pos;
	bool node_exists;
	String dir, subdir;
	for (unsigned int i = 0; i < list.size(); i++)
	{
		dir = list[i];

		// Ab.0075 Change charactercase for dir into 'real' case on disk:
        // Do not call ActualPathName() if dir is a drive (like 'C:\'):
        if (dir != IncludeTrailingPathDelimiter(ExtractFileDrive(dir)))
    		dir = IncludeTrailingPathDelimiter(ecc::ActualPathName(dir));

		// Skip directories that are not below current Root Source Dir:
		if (dir.LowerCase().Pos(rootdir.LowerCase()) != 1)
			continue;

		// Strip rootdir from dir:
		dir = dir.Delete(1, rootdir.Length());

		// Add unselected subdir:
		node = rootnode;
		while (dir.Length())
		{
			pos			= dir.Pos("\\");
			subdir		= dir.SubString(1, pos-1);
			node_exists	= false;
			nod			= node->getFirstChild();
			while (nod)
			{
				if (nod->Text.AnsiCompareIC(subdir) == 0)
				{
					node_exists = true;
					break;
				}
				nod = nod->getNextSibling();
			}

/* 092RC2: Incorrect code, replaced by code below this comment block (SE 27-mar-2006)
            if (!node_exists)
			{
				if (DirectoryExists(list[i]))
				{
					node = f_Main->fr_Source->tv_Dirs->Items->AddChild(node, subdir);
					node->StateIndex = state;
				}
			}
			dir = dir.Delete(1, pos);
			if (nod) node = nod;
		}
		node->StateIndex = state; */

            if (!node_exists)
            {
                if (DirectoryExists(list[i]))
                {
                    node = f_Main->fr_Source->tv_Dirs->Items->AddChild(node, subdir);
                    node->StateIndex = state;
                }
                else
                {
                    node = NULL;
                }
            }
            else
            {
                node = nod;
            }
            if (!node) break;
            dir = dir.Delete(1, pos);
        }
        if (node) node->StateIndex = state;
	}
}
//---------------------------------------------------------------------------

void TProfileToGui::_removeNonExistentDirsFromList(std::vector<String> &list)
{
	unsigned int i = 0;
	while (i < list.size())
	{
		if (!DirectoryExists(list[i]))
		{
			f_Main->statusNotif( ASPF(_("Directory does not exist: %s"),
                ecc::QuoteStr(list[i])) + " - ");
                ++i;
 			if (PGlobals->RemoveNonExistingSourceDirs)
			{
				std::vector<String>::iterator pos = &list[i];
				list.erase(pos);
				f_Main->statusNotif( _("Removed from Profile."), -1);
			}
			else
			{
				f_Main->statusNotif( _("Ok."), -1);
				++i;
			}
		}
		else ++i;
	}
}
//---------------------------------------------------------------------------

void TProfileToGui::filters()
{
    // NEW (since 0.9.2-beta3):
    f_Main->fr_Filters->fr_FilterSetList->setFilterSetList(&PProfile->filterSetList);
}
//---------------------------------------------------------------------------

