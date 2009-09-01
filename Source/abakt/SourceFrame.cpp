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

#include <Masks.hpp>
#include "SourceFrame.h"
#include "AbaktFilter.h"
#include "Main.h"
#include "Gui_X_Profile.h"
#include "DataModule.h"
#include "AbaktGlobals.h"
#include "AbaktProfile.h"
#include "eccBrowseForDirectory.h"
#include "eccString.h"
#include "eccMisc.h"
#include "eccFile.h"
#include "FileProperties.h"
#include "GlobalFuncs.h"
#include "FilterSetEditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameSource *FrameSource;
//---------------------------------------------------------------------------

__fastcall TFrameSource::TFrameSource(TComponent* Owner)
	: TFrame(Owner)
{
    lbl_FilesSummary->Caption = "";

	_allowCollapseExpand = false;
	latestAddedSourceDir = NULL;	// NULL is Desktop.

    _columnToSort       = 0;    // 0 = Name
    _columnSortOrder    = 1;    // 1 = Ascending
    lv_Files->Columns->Items[0]->ImageIndex = 3;
    _filterIconToSort   = -1;

    // Copy pmi_Exclude submenu to other menu's:
    _addFilterSetSubMenuToPopupMenu(pmi_Include);
    _addFilterSetSubMenuToPopupMenu(pmi_NoCompression);
}
//---------------------------------------------------------------------------

void TFrameSource::_addFilterSetSubMenuToPopupMenu(TMenuItem *pmi)
{
	TMenuItem* mi;
	mi = new TMenuItem(pmi);
	mi->Caption = pmi_ThisFile->Caption;
	mi->OnClick = pmi_ThisFile->OnClick;
	pmi->Add(mi);

	mi = new TMenuItem(pmi);
	mi->Caption = pmi_FiletypeAllDirs->Caption;
	mi->OnClick = pmi_FiletypeAllDirs->OnClick;
	pmi->Add(mi);

	mi = new TMenuItem(pmi);
	mi->Caption = pmi_FiletypeThisDir->Caption;
	mi->OnClick = pmi_FiletypeThisDir->OnClick;
	pmi->Add(mi);

	mi = new TMenuItem(pmi);
	mi->Caption = pmi_FiletypeDirAndSubdirs->Caption;
	mi->OnClick = pmi_FiletypeDirAndSubdirs->OnClick;
	pmi->Add(mi);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tb_DirectoryAddClick(TObject *Sender)
{
	ecc::TBrowseForDirectory bd(PGlobals->UseNewUIOnBFD);
	bd.Handle		= Handle;
	bd.Directory	= latestAddedSourceDir;
	bd.Title		= _("Add Source Directory");
	if (!bd.Execute()) return;

	String srcDir = bd.Directory;
	if (!rootNodeDirExists(srcDir))
	{
		f_Main->status( ASPF(_("Adding Source Directory %s..."), ecc::QuoteStr(srcDir)) );
		TTreeNode* node = addSourceRootDir(srcDir);
        if (node)
        	f_Main->status(_("Ok."), -1);
        else
            f_Main->status(_("Failed."), -1);
		GuiToProfile.setModified(true);
		tv_Dirs->Selected = node;
		tv_Dirs->SetFocus();
	}
    /*  If srcDir exists a message could be displayed,
        but this is not really necessary.
    else
        ShowMessage( ASPF(_("Source Directory already exists: %s"), srcDir) );
    */

	latestAddedSourceDir = srcDir;
}
//---------------------------------------------------------------------------

TTreeNode* TFrameSource::addSourceRootDir(String dir)
{
	dir = IncludeTrailingPathDelimiter(dir);

	tv_Dirs->Items->BeginUpdate();
	TTreeNode* node = tv_Dirs->Items->Add(NULL, dir);
    node->StateIndex = nsChecked;

	_doExpand(node, false);

	tv_Dirs->AlphaSort();
	tv_Dirs->Items->EndUpdate();

	return node;
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tb_DirectoryRemoveClick(TObject *Sender)
{
	TTreeNode* node = tv_Dirs->Selected;
	if (!node) return;

	TTreeNode* rootnode;
	while (node != NULL)
	{
		rootnode = node;
		node = node->Parent;
	}
	if (!rootnode) return;

	if ( MessageDlg(_("Remove Root Source Directory from Profile?") + "\n\n"
            + ecc::QuoteStr(rootnode->Text)
			, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo
			, 0
            ) == mrNo)
        return;

	tv_Dirs->Items->Delete(rootnode);
	GuiToProfile.sourceDirLists();
	_redrawFileList();
	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tb_DirectoriesMoreFunctionsClick(
      TObject *Sender)
{
    tb_DirectoriesMoreFunctions->CheckMenuDropdown();
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tv_DirsKeyDown(TObject *Sender,
	  WORD &Key, TShiftState Shift)
{
	TTreeNode* node = tv_Dirs->Selected;
	switch (Key)
	{
		case VK_SPACE:
			// Do nothing if a control-char is pressed.
			// The actions will take care of Ctrl+Space and Shift+Ctrl+Space.
			if (!Shift.Empty()) break;
			_swapStateIndex(tv_Dirs->Selected);
			break;

		case VK_INSERT:
            tb_DirectoryAdd->Click();
			break;

		case VK_DELETE:
            tb_DirectoryRemove->Click();
			break;

		case VK_RIGHT:
			if (node->Expanded) break;
			_doExpand(node, false);
			Key = NULL; // Prevent changing to node->FirstChild.
			break;

		case VK_LEFT:
			if (!node->Expanded) break;
			_doCollapse(node, false);
			Key = NULL;	// Prevent changing to node->FirstChild.
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tv_DirsMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	// Determine node that is clicked upon:
	TTreeNode* node = tv_Dirs->GetNodeAt(X, Y);
	if (!node) return; // Could happen during slow operations like ExpandAllChildren().

	if (Button == mbRight)
	{
		// Make sure that popup actions act on the proper node:
		tv_Dirs->Selected = node;
	}

	THitTests hitTest = tv_Dirs->GetHitTestInfoAt(X,Y);

	if ((Button == mbLeft) && Shift.Contains(ssDouble))
	{
		if (!hitTest.Contains(htOnLabel)) return;
		if (node->Expanded)
		{
			_doCollapse(node, false);
			_redrawFileList();
		}
		else
		{
			_doExpand(node, false);
		}
		return;
	}

	if (Button == mbLeft)
	{
		if (hitTest.Contains(htOnButton)) // htOnButton is the '+' or '-' icon.
		{
			if (node->Expanded)
			{
				_doCollapse(node, false);
				_redrawFileList();
			}
			else
				_doExpand(node, false);
			// Prevent SwapStateIndex: (Or is this actually achieved by 'else', see below?)
			return;
		}
		else if (hitTest.Contains(htOnStateIcon)) // htOnStateIcon:	the state icon or bitmap associated with an item.
		{
			if ( (!_allowCollapseExpand) // Only swap state when not collapsing or expanding:
				&& (node == tv_Dirs->Selected) ) // Only swap state when the node clicked upon is the selected node.
			{
				_swapStateIndex(node);
				if (Shift.Contains(ssCtrl))
				{
					if (Shift.Contains(ssShift))
						act_UnSelectAllChildren->Execute();
					else
						act_SelectAllChildren->Execute();
				}
			}
		}
	} // (Button == mbLeft)
}
//---------------------------------------------------------------------------

void TFrameSource::_swapStateIndex(TTreeNode* node)
{
	if (!node) return;

	tv_Dirs->Items->BeginUpdate();

	switch (node->StateIndex)
	{
		case nsChecked:
			node->StateIndex = nsUnChecked;
			break;

		case nsGrayChecked:
			node->StateIndex = nsGrayUnChecked;
			break;

		case nsUnChecked:
			node->StateIndex = nsChecked;
			break;

		case nsGrayUnChecked:
			node->StateIndex = nsGrayChecked;
			break;
	}

	validateState(node);
	validateStateAllParents(node);

	tv_Dirs->Items->EndUpdate();

	GuiToProfile.sourceDirLists(); // Profile.excludeDirList has changed, _redrawFileList() needs this list.
	_redrawFileList();
	GuiToProfile.setModified(true);
	return;
}
//---------------------------------------------------------------------------

void TFrameSource::expandAllRootNodes(bool recurse)
{
    TTreeNode* node = tv_Dirs->Items->GetFirstNode();
    while (node)
    {
        _doExpand(node, recurse);
        node = node->getNextSibling();
    }
}
//---------------------------------------------------------------------------

void TFrameSource::validateState(TTreeNode* node)
{
	if (!node) return;

	if (!node->HasChildren)
	{
		// UnGray possible end-node, And validate all parents' state.
		_doUnGrayNode(node);
		validateStateAllParents(node);
		return;
	}

	// Validate state:
	if (_childrenHaveEqualStates(node))
	{
		_doUnGrayNode(node);
		if (node->StateIndex == node->getFirstChild()->StateIndex)
			_doUnGrayNode(node);
		else
			_doGrayNode(node);
	}
	else
	{
		_doGrayNode(node);
	}
}
//---------------------------------------------------------------------------

void TFrameSource::validateStateAllParents(TTreeNode *node)
{
	if (!node) return;

	node = node->Parent;
	while (node)
	{
		validateState(node);
		node = node->Parent;
	}
}
//---------------------------------------------------------------------------

void TFrameSource::validateStateAllChildren(TTreeNode *node)
{
	if (!node) return;

	node = node->getFirstChild();
	while(node)
	{
		validateState(node);
		validateStateAllChildren(node); // Recurse.
		node = node->getNextSibling();
	}
}
//---------------------------------------------------------------------------

bool TFrameSource::_childrenHaveEqualStates(TTreeNode* node)
{
	if (!node) return true;

	if (!node->HasChildren) return true;
	node = node->getFirstChild();
	int state = node->StateIndex; // StateIndex of first child.

	while(node)
	{
		if (node->StateIndex != state)
			return false;
		node = node->getNextSibling();
	}
	return true;
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tv_DirsChange(TObject *Sender,
	  TTreeNode *Node)
{
	/*  Remove Node if directory Node->Text does not exist
        and PGlobals->RemoveNonExistingSourceDirs is true. */

	if ( !DirectoryExists(_selectedSourceDirPath()) )
	{
		if (PGlobals->RemoveNonExistingSourceDirs)
		{
			f_Main->statusNotif(
                ASPF(_("Removing non-existing directory from Profile: %s..."),
                    ecc::QuoteStr(Node->Text)) );
			Node->Delete();
            _redrawFileList();  // 092b5: Fixes bug reported by Michael on 8-nov-2005.
			f_Main->statusNotif(_("Ok."), -1);
		}
 		else
		{
			f_Main->statusNotif(
                ASPF(_("Directory does not exist: %s."),
    				ecc::QuoteStr(Node->Text)) );
			f_Main->statusNotif(_("Ok."), -1);
			return;
		}
	}

	_redrawFileList();
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::act_RefreshFileListExecute(TObject *Sender)
{
	_redrawFileList();
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::act_SelectAllChildrenExecute(TObject *Sender)
{
	if (!tv_Dirs->Focused()) return;	// Ab.0002

	TTreeNode* node = tv_Dirs->Selected;
	if (!node) return;

	node->StateIndex = nsChecked;
	setStateAllChildren(node, nsChecked);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::act_UnSelectAllChildrenExecute(TObject *Sender)
{
	if (!tv_Dirs->Focused()) return;	// Ab.0002

	TTreeNode* node = tv_Dirs->Selected;
	if (!node) return;

	node->StateIndex = nsUnChecked;
	setStateAllChildren(node, nsUnChecked);
}
//---------------------------------------------------------------------------

void TFrameSource::setStateAllChildren(TTreeNode* node, TNodeStateIndex state)
{
	if (!node) return;

	tv_Dirs->Items->BeginUpdate();
	_setStateAllChildren2(node->getFirstChild(), state);
	validateState(node);
	validateStateAllParents(node);
	tv_Dirs->Items->EndUpdate();

	GuiToProfile.setModified(true);
	GuiToProfile.sourceDirLists();
	_redrawFileList();
}
//---------------------------------------------------------------------------

void TFrameSource::_setStateAllChildren2(TTreeNode* node, TNodeStateIndex state)
{
	while (node != NULL)
	{
		node->StateIndex = state;
		_setStateAllChildren2(node->getFirstChild(), state);
		node = node->getNextSibling();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::act_CollapseAllChildrenExecute(TObject *Sender)
{
	TTreeNode* node = tv_Dirs->Selected;
	if (!node) return;

	_doCollapse(node, true);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::act_ExpandAllChildrenExecute(TObject *Sender)
{
	TTreeNode* node = tv_Dirs->Selected;
	if (!node) return;

	_doExpand(node, true);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::act_ScanTreeBelowDirExecute(TObject *Sender)
{
	TTreeNode* node = tv_Dirs->Selected;
	if (!node) return;

	ecc::HourGlass(true);
	tv_Dirs->Items->BeginUpdate();
	f_Main->status( ASPF(_("Scanning directory tree: %s..."),
		ecc::QuoteStr(getSourceDirPathFromNode(node)) ) );

	addSubDirs(node, getSourceDirPathFromNode(node), false, true);
	validateStateAllChildren(node);
	GuiToProfile.sourceDirLists();

	f_Main->status(_("Ok."), -1);
	tv_Dirs->Items->EndUpdate();
	ecc::HourGlass(false);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::act_RefreshDirListExecute(TObject *Sender)
{
	GuiToProfile.sourceDirLists();
	ProfileToGui.sourceDirLists();

	tv_Dirs->Selected = NULL;
	tv_Dirs->Selected = tv_Dirs->Items->GetFirstNode();

	// Set focus to the treeview, only if Source page is visible:
	if (f_Main->PageControl->ActivePage == f_Main->tab_Source) // Ab.0006
	{
		tv_Dirs->SetFocus();
	}
}
//---------------------------------------------------------------------------

void TFrameSource::_doGrayNode(TTreeNode* node)
{
	if (!node) return;

	if (node->StateIndex == nsChecked)
		node->StateIndex = nsGrayChecked;
	else if (node->StateIndex == nsUnChecked)
        node->StateIndex = nsGrayUnChecked;
}
//---------------------------------------------------------------------------

void TFrameSource::_doUnGrayNode(TTreeNode* node)
{
	if (!node) return;

	if (node->StateIndex == nsGrayChecked)
		node->StateIndex = nsChecked;
	else if (node->StateIndex == nsGrayUnChecked)
        node->StateIndex = nsUnChecked;
}
//---------------------------------------------------------------------------

bool TFrameSource::isGrayNode(TTreeNode* node)
{
	if (!node) return false;

	return (node->StateIndex == nsGrayChecked)
			|| (node->StateIndex == nsGrayUnChecked)
			    ? true : false;
}
//---------------------------------------------------------------------------

bool TFrameSource::isCheckedNode(TTreeNode* node)
{
	return (node->StateIndex == nsChecked)
			|| (node->StateIndex == nsGrayChecked)
			? true : false;
}
//---------------------------------------------------------------------------

void TFrameSource::addSubDirs(TTreeNode* rootnode, String rootdir, bool done, bool all)
{
	rootdir = IncludeTrailingPathDelimiter(rootdir);

	tv_Dirs->Items->BeginUpdate();
	ecc::HourGlass(true);

	TStringList* list = new TStringList();
    try // 093b1: Big try/__finally block
    {
        try // 093b1: Big try/catch block
        {
            ecc::MakeRelativeDirList(rootdir, list);
            TTreeNode* node;
            bool exists;

            list->Sorted = true;
            for (int i = 0; i < list->Count; i++)
            {
                exists = false;
                node = rootnode->getFirstChild();
                while (node)
                {
                    if (node->Text.AnsiCompareIC(list->Strings[i]) == 0)
                    {
                        exists = true;
                        break;
                    }
                    node = node->getNextSibling();
                }
                if (!exists)
                {
                    node = tv_Dirs->Items->AddChild(rootnode, list->Strings[i]);
                    // Inherit parent's state:
                    node->StateIndex = isCheckedNode(node->Parent) ? nsChecked : nsUnChecked;
                    validateState(node);
                    validateStateAllParents(node);
                }
                if (all)
                    addSubDirs(node, getSourceDirPathFromNode(node), true, true); // Dive until the end.
                else if (!done)
                    addSubDirs(node, getSourceDirPathFromNode(node), true); // Dive one level deeper.
            }
        }
        catch(Exception &ex)
        {
            f_Main->status("Exception in " __FUNC__ " (" + IntToStr(__LINE__) + "): " + ex.Message);
        }
    }
    __finally
    {
        delete list;
    }

	tv_Dirs->Items->EndUpdate();
	ecc::HourGlass(false);
}
//---------------------------------------------------------------------------

bool TFrameSource::_newFilterSetDialog(const bool showEditor,
        const String name, const String mask, const TAbaktFilterSetAction fsAction)
{
    TAbaktFilterSet fs;
    fs.name     = name;
    fs.action   = fsAction;
    fs.path     = mask;
    f_FilterSetEditor->filterSet = fs;
    if (showEditor)
        if (f_FilterSetEditor->ShowModal() != mrOk)
            return false;

    fs = f_FilterSetEditor->filterSet;
    AddFilterSetToList_SimilarAware(fs, &PProfile->filterSetList);

    // Update Gui:
    f_Main->fr_Filters->fr_FilterSetList->redraw();
	_redrawFileList();
    GuiToProfile.setModified(true);

    return true;
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tb_FilterSetFunctionsClick(TObject *Sender)
{
    tb_FilterSetFunctions->CheckMenuDropdown();
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pmi_NewFilterSetBasedOnFileClick(TObject *Sender)
{
    TAbaktFilterSet fs;

    if (lv_Files->Selected)
    {
        fs.path = IncludeTrailingPathDelimiter(_selectedSourceDirPath())
                + lv_Files->Selected->Caption;
        fs.name = ASPF(_("(New FilterSet based on file %s)"), ecc::QuoteStr(fs.path));

        TFileProperties fp;
        if (!fp.retrieve(fs.path))
        {
            ShowMessage(__FUNC__ ": " + _("Could not retrieve file properties."));
            return;
        }

            fs.size = "==" + IntToStr(fp.getSize());
            fs.date = "==" + abtDateToStr(fp.getDate());
            fs.age  = "==" + IntToStr(fp.getAge());
            String fileAttrs = fp.getAttrsStr();
            for (int i = 1; i <= fileAttrs.Length(); i++)
            {
                if (!fs.attributes.IsEmpty()) fs.attributes += + ";";
                fs.attributes += "==" + String(fileAttrs[i]);
            }
    }

    // Show FilterSet Editor:
    f_FilterSetEditor->filterSet = fs;
    if (f_FilterSetEditor->ShowModal() != mrOk)
        return;

    fs = f_FilterSetEditor->filterSet;
    AddFilterSetToList_SimilarAware(fs, &PProfile->filterSetList);

    // Update Gui:
    f_Main->fr_Filters->fr_FilterSetList->redraw();
	_redrawFileList();
    GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pmi_EditActiveFilterSetClick(TObject *Sender)
{
    String fPath = (lv_Files->Selected) ? lv_Files->Selected->Caption : String("");
    if (fPath.IsEmpty()) return;

    fPath = IncludeTrailingPathDelimiter(_selectedSourceDirPath()) + fPath;

    TAbaktFilter filt;
    filt.testFile(fPath);
    int fsIdx = filt.getFilterSetIndex();
    if (fsIdx == -1) return;

    f_Main->fr_Filters->fr_FilterSetList->lv->ItemIndex = fsIdx;
    f_Main->fr_Filters->fr_FilterSetList->tb_Edit->Click();

    _redrawFileList();
}
//---------------------------------------------------------------------------

String TFrameSource::_selectedSourceDirPath()
{
	TTreeNode* node = tv_Dirs->Selected;
	return getSourceDirPathFromNode(node);
}
//---------------------------------------------------------------------------

String TFrameSource::getSourceDirPathFromNode(TTreeNode* node)
{
	if (!node)	 // Required!
		return "";

	String dir = node->Text;
	while (node->Parent)
	{
		node = node->Parent;
		dir = IncludeTrailingPathDelimiter(node->Text) + dir;
	}
	return dir; // Should IncludeTrailingPathDelimiter.
}
//---------------------------------------------------------------------------

bool TFrameSource::rootNodeDirExists(String dir)
{
	dir = IncludeTrailingPathDelimiter(dir);
	TTreeNode* node = tv_Dirs->Items->GetFirstNode();
	while (node != NULL)
	{
		if (node->Text == dir) return true;
		node = node->getNextSibling();
	}
	return false;
}
//---------------------------------------------------------------------------

void TFrameSource::_redrawFileList()
{
	int selItem = lv_Files->ItemIndex;

	String dir = _selectedSourceDirPath();
	if (dir.IsEmpty())
	{
		lv_Files->Items->Clear();
		return;
	}
	dir = IncludeTrailingPathDelimiter(dir);

	// Populate lv_Files with files in selected dir:
	ecc::HourGlass(true);
	lv_Files->Items->Clear();
	Application->ProcessMessages();
	lv_Files->Items->BeginUpdate();

    TListItem* litem;
    String fname;
    TAbaktFilter filt;
	TStringList* flist = new TStringList();
    try // 093b1: Big try/__finally block
    {
        try // 093b1: Big try/catch block
        {
            int included_filecount  = 0;
            float included_filesize = 0;
            lbl_FilesSummary->Caption = "...";
            ecc::MakeFileList(dir, flist, false);
            for (int i = 0; i < flist->Count; i++)
            {
                fname = flist->Strings[i];
                litem = lv_Files->Items->Add();

                // Filter Icon:
                if (filt.testFile(dir + fname))
                {
                    pmi_NotCompressed->Visible	= f_Main->fr_ActionType->rb_Zip->Checked;   // NoCompress only works with ZipForge
                    if (f_Main->fr_ActionType->rb_Zip->Checked)
                        litem->ImageIndex = filt.getFileNotCompressed() ? ICON_NOCOMPRESS : ICON_INCLUDE;
                    else
                        litem->ImageIndex = ICON_INCLUDE;
                    ++included_filecount;
                    included_filesize += filt.getFileProps().getSize();
                }
                else
                {
                    litem->ImageIndex = ICON_EXCLUDE;
                }

                // Name:
                litem->Caption = fname;
                // Size:
                litem->SubItems->Add( ecc::FloatToHumanFileSize(filt.getFileProps().getSize()) );
                // Attributes:
                litem->SubItems->Add( filt.getFileProps().getAttrsStr() );
                // Date (Last Modification):
                litem->SubItems->Add( filt.getFileProps().getDate().DateTimeString() );
                // Age:
                if (filt.getFileProps().getAge() != -1)
                    litem->SubItems->Add( IntToStr(filt.getFileProps().getAge()) );
                else    // Age is unknown:
                    litem->SubItems->Add("?");
                // Filter Info:
                litem->SubItems->Add( filt.getInfoStr() );
            }

            // Display included files summary:
            lbl_FilesSummary->Caption =
                ASPF(_("Included Files: %d/%d. Size: %s (%s Bytes)"),
                    included_filecount,
                    flist->Count,
                    ecc::FloatToHumanFileSize(included_filesize),
                    FloatToStr(included_filesize) );
        }
        catch(Exception &ex)
        {
            f_Main->status("Exception in " __FUNC__ " (" + IntToStr(__LINE__) + "): " + ex.Message);
        }
    }
    __finally
    {
    	delete flist;
    }

    lv_Files->CustomSort(NULL, 0);  // Always do sort on column.
    if (_filterIconToSort >= 0)
    {   // If chosen, perform a filter-button sort after column-sort:
        lv_Files->CustomSort(NULL, 100 + _filterIconToSort);
    }

	lv_Files->Items->EndUpdate();
	ecc::HourGlass(false);

	_makeFileVisible(selItem);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tv_DirsCollapsing(TObject *Sender,
	  TTreeNode *Node, bool &AllowCollapse)
{
    /*	Allow Collapse/Expand only when _allowCollapseExpand is true
        to prevent this bug:
        - Mouse events occur AFTER OnCollapsing, OnCollapse, OnExpanding and OnExpand.
        A mouse event will be triggered for the node the mouse is upon when
        the treeview's display may have changed.
        This can cause unwanted calls to SwapStateIndex() in OnMouseDown. */

	AllowCollapse = _allowCollapseExpand;
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tv_DirsExpanding(TObject *Sender,
	  TTreeNode *Node, bool &AllowExpansion)
{
    /*	Allow Collapse/Expand only when _allowCollapseExpand is true
        to prevent this bug:
        - Mouse events occur AFTER OnCollapsing, OnCollapse, OnExpanding and OnExpand.
        A mouse event will be triggered for the node the mouse is upon when
        the treeview's display may have changed.
        This can cause unwanted calls to SwapStateIndex() in OnMouseDown. */

	AllowExpansion = _allowCollapseExpand;
}
//---------------------------------------------------------------------------

void TFrameSource::_doExpand(TTreeNode* node, bool recurse)
{
	if (!node) return;

	ecc::HourGlass(true);
	tv_Dirs->Items->BeginUpdate();
	_allowCollapseExpand = true;

    if (recurse)
    {
        f_Main->status(ASPF(_("Finding all subdirectories of '%s'..."),
            ecc::QuoteStr(getSourceDirPathFromNode(node))) );
        addSubDirs(node, getSourceDirPathFromNode(node), false, true);
        GuiToProfile.sourceDirLists();
        f_Main->status(_("Ok."), -1);
    }
    else
    {
        addSubDirs(node, getSourceDirPathFromNode(node));
        GuiToProfile.sourceDirLists();
    }

    if (recurse) f_Main->status(_("Now fully expanding..."));
    node->Expand(recurse);
    if (recurse) f_Main->status(_("Ok."), -1);

	_allowCollapseExpand = false;
	tv_Dirs->Items->EndUpdate();
	ecc::HourGlass(false);
}
//---------------------------------------------------------------------------

void TFrameSource::_doCollapse(TTreeNode* node, bool recurse)
{
	if (!node) return;

	ecc::HourGlass(true);
	tv_Dirs->Items->BeginUpdate();
	_allowCollapseExpand = true;

	node->Collapse(recurse);

	_allowCollapseExpand = false;
	tv_Dirs->Items->EndUpdate();
	ecc::HourGlass(false);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tv_DirsKeyPress(TObject *Sender,
	  char &Key)
{
	if (Key == VK_SPACE) Key = NULL; // Prevents beep.
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pop_DirsPopup(TObject *Sender)
{
    tb_DirectoriesMoreFunctions->Down = false;   // Required (cosmetic).
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pop_FilterSetPopup(TObject *Sender)
{
    tb_FilterSetFunctions->Down = false;   // Required (cosmetic).
}
//---------------------------------------------------------------------------

void TFrameSource::_makeFileVisible(int selItem)
{
	if (selItem < 0) selItem = 0;//return;
	if (selItem >= lv_Files->Items->Count) return;

	lv_Files->ItemIndex = selItem;
	lv_Files->Selected->MakeVisible(false);
	lv_Files->ItemFocused = lv_Files->Selected;
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::lv_FilesColumnClick(TObject *Sender,
      TListColumn *Column)
{
    // Reset icon:
    if (_columnToSort < lv_Files->Columns->Count)
        lv_Files->Columns->Items[_columnToSort]->ImageIndex = -1;

    if (_columnToSort == Column->Index)
        _columnSortOrder = -1 * _columnSortOrder;
    _columnToSort = Column->Index;

    // Set new icon:
    Column->ImageIndex = (_columnSortOrder > 0) ? xfICON_UP : xfICON_DOWN;

    // Reset the tb_FileSort button:
    _filterIconToSort = -1;
    tb_FileSort->ImageIndex = _filterIconToSort;

    // Sort:
    lv_Files->CustomSort(NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pop_FileSortPopup(TObject *Sender)
{
    tb_FileSort->Down = false;   // Required (cosmetic).
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tb_FileSortClick(TObject *Sender)
{
    tb_FileSort->CheckMenuDropdown();
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::tb_FileSortItemClick(TObject *Sender)
{
    // Sort:
    _filterIconToSort = ((TMenuItem*)Sender)->ImageIndex;
    tb_FileSort->ImageIndex = _filterIconToSort;
    lv_Files->CustomSort(NULL, 100 + _filterIconToSort);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::lv_FilesCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
    Compare = 0;    // Default, do nothing.

    if (Data < 100) // Do column sort:
    {
        int idx;
        String baseDir;

        switch (_columnToSort)
        {
            case 0:   // Name (caption)
                Compare = _columnSortOrder * CompareText(Item1->Caption, Item2->Caption);
                break;

            case 1:   // Size (col 0)
                baseDir = IncludeTrailingPathDelimiter(_selectedSourceDirPath());
                Compare = _columnSortOrder *
                    (ecc::GetFileSize(baseDir + Item1->Caption) - ecc::GetFileSize(baseDir + Item2->Caption));
                break;

            case 2:   // Attr (col 1)
            case 5:   // FilterInfo (col 4)
                idx = _columnToSort - 1;
                Compare = _columnSortOrder *
                    CompareText(Item1->SubItems->Strings[idx], Item2->SubItems->Strings[idx]);
                break;

            case 4:   // Age (col 3) (since 0.9.2-beta3)
            case 3:   // Date Modified (col 2)
                baseDir = IncludeTrailingPathDelimiter(_selectedSourceDirPath());
                Compare = _columnSortOrder *
                    (FileAge(baseDir + Item1->Caption) - FileAge(baseDir + Item2->Caption));
                break;
        }
    }
    else    // Do filter-icon sort:
    {
        int it1 = Item1->ImageIndex, it2 = Item2->ImageIndex;

        switch (Data)
        {
            case 100: // ex,in,no  0,1,2
                Compare = 1 * (it1 - it2);
                break;

            case 101: // in,no,ex  1,2,0
                if ((it1 == 1) && (it2 == 2))
                {   it1 = 2; it2 = 1; }
                else if ((it1 == 2) && (it2 == 1))
                {   it1 = 1; it2 = 2; }
                Compare = -1 * (it1 - it2);
                break;

            case 102: // no,in,ex  2,1,0
                Compare = -1 * (it1 - it2);
                break;
        }
    }
}
//---------------------------------------------------------------------------

String TFrameSource::_buildPathMask(const TAbaktPathMaskType pmType)
{
	String fdir = IncludeTrailingPathDelimiter(_selectedSourceDirPath());
	// If no file is selected use all files (i.e. "*"):
	String fname = (lv_Files->Selected) ? lv_Files->Selected->Caption : String("*");

    fdir = ecc::ReplaceStr(fdir, "[", "[[]");   //095rc2: Prevent incorrect mask when path has a '['.
    fname = ecc::ReplaceStr(fname, "[", "[[]"); //095rc2: Prevent incorrect mask when path has a '['.

	switch (pmType)
	{
		case pmtFile:
			return fdir + "|" + fname;

		case pmtFiletype:
			return "*|*" + ExtractFileExt(fname);

		case pmtFiletypeThisDir:
			return fdir + "|*" + ExtractFileExt(fname);

		case pmtFiletypeThisDirAndBelow:
			return fdir + "*|*" + ExtractFileExt(fname);

        case pmtAllFilesThisDir:
			return fdir + "|*";

        #ifdef ABT_TEST
            default:
                ShowMessage(__FUNC__ ": Oops!");
        #endif

	}

	return "";  // Default.
}
//---------------------------------------------------------------------------

bool TFrameSource::_doPmiFilterSetFunction(TObject *Sender,
        const TAbaktPathMaskType pmtFile)
{
    TMenuItem* parentMenuItem = dynamic_cast<TMenuItem*>(Sender)->Parent;
    if (parentMenuItem == NULL)    // Should never occur!
    {
        #ifdef ABT_TEST
            ShowMessage(__FUNC__ ": parentMenuItem == NULL");
        #endif
        return false;
    }

    TAbaktFilterSetAction fsAction;
    String fsActionStr;

    // Find out which parentMenu to use (exclude/include):
    if (parentMenuItem == pmi_Exclude)
    {
        fsAction    = fsaExclude;
        fsActionStr = _("Exclude");
    }
    else if (parentMenuItem == pmi_Include)
    {
        fsAction    = fsaInclude;
        fsActionStr = _("Include (Always)");
    }
    else if (parentMenuItem == pmi_NoCompression)
    {
        fsAction    = fsaNoCompression;
        fsActionStr = _("No Compression");
    }
    else return false;   // Should never occur!

    // Show FilterSet dialog:
    String mask = _buildPathMask(pmtFile);
    _newFilterSetDialog(pmi_AlwaysShowEditor->Checked,
            fsActionStr + " " + ecc::QuoteStr(mask), mask,
            fsAction);

    return true;
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pmi_ThisFileClick(TObject *Sender)
{
    _doPmiFilterSetFunction(Sender, pmtFile);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pmi_FiletypeAllDirsClick(TObject *Sender)
{
    _doPmiFilterSetFunction(Sender, pmtFiletype);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pmi_FiletypeThisDirClick(TObject *Sender)
{
    _doPmiFilterSetFunction(Sender, pmtFiletypeThisDir);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pmi_FiletypeDirAndSubdirsClick(
      TObject *Sender)
{
    _doPmiFilterSetFunction(Sender, pmtFiletypeThisDirAndBelow);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pmi_AllFilesThisDirClick(TObject *Sender)
{
    _doPmiFilterSetFunction(Sender, pmtAllFilesThisDir);
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::pmi_AlwaysShowEditorClick(TObject *Sender)
{
    pmi_AlwaysShowEditor->Checked = !pmi_AlwaysShowEditor->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFrameSource::lv_FilesDblClick(TObject *Sender)
{
    pmi_EditActiveFilterSetClick(NULL);
}
//---------------------------------------------------------------------------

