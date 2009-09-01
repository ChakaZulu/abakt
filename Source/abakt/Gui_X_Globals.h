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
#ifndef Gui_X_GlobalsH
#define Gui_X_GlobalsH
//---------------------------------------------------------------------------

//===========================================================================
// GuiToGlobals
//===========================================================================

class TGuiToGlobals {

public:
	void all();
	/*	Copies all GUI prefs/f_Prefs to AbaktGlobals. */

};
//---------------------------------------------------------------------------
extern TGuiToGlobals GuiToGlobals;
//---------------------------------------------------------------------------

//===========================================================================
// GlobalsToGui
//===========================================================================

class TGlobalsToGui {

public:
	void all();
	/*	Copies all global settings to Gui/f_Prefs. */

};
//---------------------------------------------------------------------------
extern TGlobalsToGui GlobalsToGui;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
