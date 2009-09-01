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
#ifndef DataModuleH
#define DataModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ImgList.hpp>
#include <Controls.hpp>
//---------------------------------------------------------------------------

//	imgs_Main:
enum ICONS_MAIN
{
	ICON_EXECUTE = 0,
    ICON_GROUP,
	ICON_SOURCE,
    ICON_FILTERS,
    ICON_ACTION,
    ICON_FINALIZE,
    ICON_FILE_NEW,
    ICON_FILE_OPEN,
    ICON_FILE_SAVE
};

//	imgs_Buttons:
enum ICONS_BUTTONS
{
	ICON_ADD = 0,
    ICON_REMOVE,
    ICON_EDIT,
    ICON_CLEAR,
    ICON_CHECKALL,
    ICON_CHECKNONE,
    ICON_UP,
    ICON_DOWN,
    ICON_SORT,
    ICON_DUPLICATE
};

//	imgs_CheckBoxes:
enum ICONS_CHECKBOXES
{
	ICON_CHECKED_NOBOX = 0,
	ICON_CHECKED,
	ICON_UNCHECKED,
	ICON_GRAYCHECKED,
	ICON_GRAYUNCHECKED
};

//	imgs_Filters:
enum ICONS_FILTERS
{
	ICON_EXCLUDE = 0,
	ICON_INCLUDE,
	ICON_NOCOMPRESS,
	xfICON_UP, xfICON_DOWN // These icons are copied from imgs_Buttons to imgs_Filters in the .cpp file.
};

//---------------------------------------------------------------------------

class TDM : public TDataModule
{
__published:	// IDE-managed Components
	TImageList *imgs_Main;
    TImageList *imgs_Filters;
    TImageList *imgs_Buttons;
    TImageList *imgs_CheckBoxes;

private:	// User declarations

public:		// User declarations
	__fastcall TDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDM *DM;
//---------------------------------------------------------------------------
#endif
