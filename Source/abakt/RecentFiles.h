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
#ifndef RecentFilesH
#define RecentFilesH
//---------------------------------------------------------------------------
#include <Menus.hpp>
//---------------------------------------------------------------------------

/*	Usage:
	TRecentFiles* rf = new TRecentFiles();
	if (rf)
		rf->OnSelect = OnSelectRecentFile; // Declared as: void OnSelectRecentFile(String fpath)

	rf->add("C:\filename.ext");
	rf->popupAt(Form1->Left + 32, Form1->Top + 48);
	// Or:
	rf->toMenuItem(recent_files); // [recent_files] is a TMenuItem, most likely part of a TMenu.

	delete rf;
*/

//===========================================================================

class TRecentFiles {

private:

	void __fastcall OnRecentFileClick(TObject *Sender);
	/*	Internal callback, called when a menu item is selected. */

	typedef void CALLBACK_OnSelect(String fpath);
	CALLBACK_OnSelect* OnSelect;
	/*	External callback, can be set using the constructor. */

	int _maxVisibleItemsCount;
	int _maxItemsCount;
	TPopupMenu* _popMenu;
	TMenuItem* _defaultMenuItem;
	TStringList* _items;
	bool _showHotKeys;

	bool _showPath;
    /*  If true, the full paths of the recent files will be displayed. */

	void _itemsToMenu(TMenuItem* menu_item);
	/*	Populates menu_item with _maxVisibleItemsCount _items. */

	void _updateDefaultMenu();

public:

	TRecentFiles(TMenuItem* menu_item, CALLBACK_OnSelect* on_select);
	~TRecentFiles();

	void add(String item);
	/*	Adds [item] to the top of _items. The [item] is assumed to be a path to a file.
		If [item] exists, it is moved to the top (duplicates are not allowed).
		Updates _defaultMenuItem if it exists. */

	void clear();
	/*	Empties FItems, and sets _maxVisibleItemsCount and _maxItemsCount to their default values.
		Updates _defaultMenuItem if it exists. */

	bool isEmpty()
	{	return (_items->Count == 0); }

	int getCount()
	{	return _items->Count; }

	int setMaxVisibleItemsCount(int count);
	/*	Sets _maxVisibleItemsCount to [count], but will not exceed _maxItemsCount.
		Returns the value _maxVisibleItemsCount is set to.
		Updates _defaultMenuItem if it exists. */

	int getMaxVisibleItemsCount()
	{	return _maxVisibleItemsCount; }

	int getVisibleItemsCount();
	/*	Returns the actual amount of visible items.
		This value is equal to or less than _maxVisibleItemsCount. */

	void setShowPath(bool show);
    /*  Sets/unsets _showPath. */

 	bool getShowPath()
	{	return _showPath; }

	bool toIniFile(String ini_fpath, String section_name);
	/*	Saves list of files to section [section_name] of ini-file [ini_fpath].
		Saves the value of _maxVisibleItemsCount as "MaxVisibleItemsCount=[_maxVisibleItemsCount]".
		Saves the value of _showPath as "ShowPath=[_showPath]".
		The list will be stored as a name=value pair (ie. "Item.0=C:\filename.ext").
		Note: Replaces existing section.
		Returns false if an error occurred. */

	bool fromIniFile(String ini_fpath, String section_name);
	/*  Retrieves data from section [section_name] of ini-file [ini_fpath].
		Reads the values of _maxVisibleItemsCount and _showPath.
		Reads the name=value pairs for the items.
		Returns false if an error occurred.
		Updates _defaultMenuItem if it exists. */

	void popupAt(int X, int Y);
	/* 	Displays a maximum of _maxVisibleItemsCount items
		as a TPopupMenu at screen coordinates X and Y. */

	TPopupMenu* getPopupMenu();
	/*  Returns pointer to FPopMenu.
        Can be used to add the RecentFiles' TPopupMenu to an existing TMenuItem as a submenu. */
};
//---------------------------------------------------------------------------
#endif
