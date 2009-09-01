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
#ifndef SourceFrameH
#define SourceFrameH
//---------------------------------------------------------------------------
#include "AbaktFilterSet.h"
#include <ActnList.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------

/*	Mouse events occur AFTER OnCollapsing, OnCollapse, OnExpanding and OnExpand.
	A mouse event will be triggered for the node the mouse is upon when
	the treeview's display may have changed.

	This can cause unwanted calls to _swapStateIndex(). So all Collapse and
	Expand actions are done programmatically. */

//---------------------------------------------------------------------------

typedef enum
{
	/* Note that in the ImageList, index==0 is an unused but necessary value. */
	nsChecked = 1,
	nsUnChecked,
	nsGrayChecked,
	nsGrayUnChecked
} TNodeStateIndex;
//---------------------------------------------------------------------------

typedef enum
{
	pmtFile = 0,
	pmtFiletype,
	pmtFiletypeThisDir,
	pmtFiletypeThisDirAndBelow,
    pmtAllFilesThisDir
} TAbaktPathMaskType;
//---------------------------------------------------------------------------

class TFrameSource : public TFrame
{
__published:	// IDE-managed Components
	TActionList *ActionList;
	TAction *act_RefreshFileList;
	TGroupBox *gbx_Files;
	TGroupBox *gbx_Dirs;
    TTreeView *tv_Dirs;
	TListView *lv_Files;
	TPopupMenu *pop_Dirs;
	TAction *act_SelectAllChildren;
	TAction *act_UnSelectAllChildren;
	TMenuItem *mi_SelectAllChildren;
	TMenuItem *mi_UnSelectAllChildren;
	TAction *act_ExpandAllChildren;
	TAction *act_CollapseAllChildren;
	TMenuItem *N2;
	TMenuItem *mi_CollapseAllChildren;
	TMenuItem *mi_ExpandAllChildren;
	TAction *act_RefreshDirList;
    TPopupMenu *pop_FilterSet;
	TSplitter *Splitter;
	TAction *act_ScanTreeBelowDir;
	TMenuItem *N1;
	TMenuItem *mi_ScanTreeBelowDir;
	TPanel *pnl_Descr_Source;
	TLabel *Label15;
	TMenuItem *N3;
    TPanel *pnl_FilesSummary;
	TLabel *lbl_FilesSummary;
    TPanel *pnl_FileToolBars;
    TToolBar *tbr_FilterIcons;
    TToolBar *tbr_FilterSets;
    TToolButton *tb_FilterSetFunctions;
    TToolButton *ToolButton3;
    TToolBar *tbr_Directories;
    TToolButton *tb_DirectoryAdd;
    TToolButton *tb_DirectoryRemove;
    TToolButton *tb_DirectoriesMoreFunctions;
    TMenuItem *pmi_EditActiveFilterSet;
    TMenuItem *pmi_NewFilterSetBasedOnFile;
    TMenuItem *pmi_Exclude;
    TMenuItem *pmi_ThisFile;
    TMenuItem *pmi_FiletypeThisDir;
    TMenuItem *pmi_FiletypeDirAndSubdirs;
    TMenuItem *pmi_FiletypeAllDirs;
    TMenuItem *pmi_AllFilesThisDir;
    TMenuItem *pmi_Include;
    TMenuItem *pmi_AlwaysShowEditor;
    TMenuItem *N4;
    TToolButton *tb_FileSort;
    TPopupMenu *pop_FileSort;
    TMenuItem *pmi_None;
    TMenuItem *pmi_Excluded;
    TMenuItem *pmi_Included;
    TMenuItem *pmi_NotCompressed;
    TToolButton *ToolButton1;
    TMenuItem *pmi_NoCompression;
	void __fastcall tv_DirsKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift);
	void __fastcall tv_DirsMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall tv_DirsChange(TObject *Sender, TTreeNode *Node);
	void __fastcall act_RefreshFileListExecute(TObject *Sender);
	void __fastcall act_SelectAllChildrenExecute(TObject *Sender);
	void __fastcall act_UnSelectAllChildrenExecute(TObject *Sender);
	void __fastcall act_ExpandAllChildrenExecute(TObject *Sender);
	void __fastcall act_CollapseAllChildrenExecute(TObject *Sender);
	void __fastcall act_RefreshDirListExecute(TObject *Sender);
	void __fastcall tv_DirsCollapsing(TObject *Sender, TTreeNode *Node,
		  bool &AllowCollapse);
	void __fastcall tv_DirsExpanding(TObject *Sender, TTreeNode *Node,
		  bool &AllowExpansion);
	void __fastcall act_ScanTreeBelowDirExecute(TObject *Sender);
	void __fastcall tv_DirsKeyPress(TObject *Sender, char &Key);
	void __fastcall pop_FilterSetPopup(TObject *Sender);
    void __fastcall tb_DirectoryAddClick(TObject *Sender);
    void __fastcall tb_DirectoryRemoveClick(TObject *Sender);
    void __fastcall lv_FilesColumnClick(TObject *Sender,
          TListColumn *Column);
    void __fastcall lv_FilesCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
    void __fastcall tb_FileSortItemClick(TObject *Sender);
    void __fastcall tb_FilterSetFunctionsClick(TObject *Sender);
    void __fastcall pop_DirsPopup(TObject *Sender);
    void __fastcall tb_DirectoriesMoreFunctionsClick(TObject *Sender);
    void __fastcall pmi_EditActiveFilterSetClick(TObject *Sender);
    void __fastcall pmi_NewFilterSetBasedOnFileClick(TObject *Sender);
    void __fastcall pmi_ThisFileClick(TObject *Sender);
    void __fastcall pmi_FiletypeAllDirsClick(TObject *Sender);
    void __fastcall pmi_FiletypeThisDirClick(TObject *Sender);
    void __fastcall pmi_FiletypeDirAndSubdirsClick(TObject *Sender);
    void __fastcall pmi_AllFilesThisDirClick(TObject *Sender);
    void __fastcall pmi_AlwaysShowEditorClick(TObject *Sender);
    void __fastcall lv_FilesDblClick(TObject *Sender);
    void __fastcall tb_FileSortClick(TObject *Sender);
    void __fastcall pop_FileSortPopup(TObject *Sender);


public:		// User declarations

	__fastcall TFrameSource(TComponent* Owner);

	bool rootNodeDirExists(String dir);

	String latestAddedSourceDir;
	/*	Stores the last rootdir added to the tree using the 'Add' button. */

	void expandAllRootNodes(bool recurse);
	/*	Calls _doExpand() for every rootnode.*/

	void validateState(TTreeNode* node);
	/* 	Sets state of [node] to gray if [node] has children with
        states that differ from eachother. */

	void validateStateAllParents(TTreeNode *node);
	/*	Grays the state of all [node]'s parents if required. */

	void validateStateAllChildren(TTreeNode *node);
	/*	Corrects the states of all children below [node]. Recursive.
		Useful to correct the states if a directory is missing when a profile is opened. */

	String getSourceDirPathFromNode(TTreeNode* node);
	/*	Returns the full path to [node]'s directory. */

	void setStateAllChildren(TTreeNode* node, TNodeStateIndex state);
	/*	Sets StateIndex for all known children of [node].
		The as yet unknown children will inherit their parent's StateIndex when discovered. */

	bool isGrayNode(TTreeNode* node);
	/* 	Returns true if [node]'s StateIndex is nsGrayChecked or nsGrayUnChecked. */

	bool isCheckedNode(TTreeNode* node);
	/*	Returns true if [node]'s StateIndex is nsChecked or nsGrayUnChecked. */

	TTreeNode* addSourceRootDir(String dir);

	void addSubDirs(TTreeNode* rootnode, String rootdir,
            bool done = false, bool all = false);
	/*	[rootdir] is assumed to include drive-information.
		If [done] is false, addSubDirs will dive one directory deeper:
			A '+' sign will appear if [rootdir] has subdirectories.
		If [all] is true addSubDirs() will act recursively.
		Call GuiToProfile.SourceDirLists() after calling addSubDirs()
			to make Abakt's filter aware of the changes.
		#Note:
			You should probably call GuiToProfile.SourceDirLists()
			after addSubDirs() to update the Profile. */

private:	// User declarations

	bool _allowCollapseExpand;
	/*	If false the Collapse and Expand events are not allowed. */

	void _doCollapse(TTreeNode* node, bool recurse);
	/*  If [recurse] is true [node] and all its children will collapse.
		If [recurse] is false [node] will be collapse. */

	void _doExpand(TTreeNode* node, bool recurse);
	/*  If [recurse] is true:
			All existing subdirs will be added to the tree.
			[node] and all its children will be expanded.
		If [recurse] is false:
			Existing subdirs two levels deep will be added to the tree.
			[node] will be expanded.
		GuiToProfile.SourceDirLists() will be called after adding subdirs. */

    void _addFilterSetSubMenuToPopupMenu(TMenuItem *pmi);

    bool _doPmiFilterSetFunction(TObject *Sender, // 0.9.2-beta3.309
            const TAbaktPathMaskType pmtFile);
    /*  [Sender] is pop_File's selected (sub)menuitem.
        Returns false if [Sender] or its parent menuitem could not be determined. */

    String _buildPathMask(const TAbaktPathMaskType pmType);     // 0.9.2-beta3.309
    /*  Returns a pathmask string that suits [pmType].
        The string is based on the selected file's path and name. */

	bool _newFilterSetDialog(const bool showEditor,             // 0.9.2-beta3.309
            const String name, const String mask, const TAbaktFilterSetAction fsAction);
	/* 	Edit [mask], suggested by pop_Files, and add it to PProfile's filterSetList.
        Updates Gui.
		Returns false is [mask] is not accepted by user. */

	void _redrawFileList();
	/*  Updates the File listview according to current filter and sorting settings.
        Remembers and refocusses selected file (if any). */

	String _selectedSourceDirPath();
	/* 	Returns path of selected source dir, NOT always including TrailingBackslash.
		Or empty string if no dir is selected. */

	void _swapStateIndex(TTreeNode* node);
	/*	Swaps the state of node, validating the grayness of node.
		Updates node's parents grayness. */

	void _setStateAllChildren2(TTreeNode* node, TNodeStateIndex state);
	/*	Recursive helper function for SetStateAllChildren(). */

	bool _childrenHaveEqualStates(TTreeNode* node);
	/*	Returns true if all children have the same state. */

	void _doGrayNode(TTreeNode* node);
	void _doUnGrayNode(TTreeNode* node);

	void _makeFileVisible(int selItem);
    /*  Set focus to file [selItem] in listview, and make sure it is fully visible. */

    int _columnToSort;
    /*  The File listview item to sort on.
        Special: 100, 101, 102: Excluded, Included, NotCompressed. */

    int _columnSortOrder;
    /*  1=Ascending, -1=Descending */

    int _filterIconToSort;
    /*  -1=none, exc=0, inc=1, nocompr=2 */

};
//---------------------------------------------------------------------------
extern PACKAGE TFrameSource *FrameSource;
//---------------------------------------------------------------------------
#endif

