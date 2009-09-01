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
#ifndef Gui_X_ProfileH
#define Gui_X_ProfileH
//---------------------------------------------------------------------------
#include "eccCheckStringList.h"
#include "SourceFrame.h"
#include "AbaktClasses.h"
#include "AbaktFilterSet.h"
//---------------------------------------------------------------------------

//===========================================================================
// GuiToProfile
//===========================================================================

class TGuiToProfile {

public:
	void all();
	/*	Calls all memberfunctions. */

	void sourceDirLists();
	/* 	Builds a list of the in- and excluded) source dirs and trees. */

	void actionType();
	/*  Sets ActionType.
		Also sets methods for actFile. */

	void archiveMode();
	/*  Sets ArchiveMode.
		Also modifies FilePropsFilter.A_AttrFilterMethod. */

	void setModified(const bool modified);
	/*	Sets Profile.modified to [modified] and adds an '*' to f_Main->Caption. */

private:

    void _destination();
	/*  Sets Destination.<property> values. */

    TActZipMethod _actZipMethod();

	void _sourceRootDirList();
	/*	Build a list of all root-source dirs (whether selected or not). */

	void _sourceDirLists2(TTreeNode* node, String dir);
	/* 	Recursive helper-function for SourceDirLists().
		rootdir is assumed to include trailing backslash or colon. */

	void _addSourceDirToList(TTreeNode* node, String dir, const bool include_backslash = true);

	void _actionTypeSettings();
	/*  Sets ActionTypes.<type>.<property> values. */

};
//---------------------------------------------------------------------------
extern TGuiToProfile GuiToProfile;
//---------------------------------------------------------------------------

//===========================================================================
// ProfileToGui
//===========================================================================

class TProfileToGui {

public:
	void all();
	/*	Calls all memberfunctions. */

	void sourceDirLists();

	void archiveMode();

	void filters();
	/*  Copies PProfile->filterSetList to GUI. */

private:
	void _actionType();	// Includes methods for actFile.

	void _actZipMethod(TActZipMethod method);

	void _listToTreeView(std::vector<String> &list, const TNodeStateIndex state,
			TTreeNode* rootnode, String rootdir);

	void _removeNonExistentDirsFromList(std::vector<String> &list);
};
//---------------------------------------------------------------------------
extern TProfileToGui ProfileToGui;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
