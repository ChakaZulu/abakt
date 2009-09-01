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
#pragma hdrstop

#include "RecentFiles.h"
#include "eccString.h"
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

static const int MaxItemsCount_DEFAULT			= 128;
static const String MaxVisibleItemsCount_NAME	= "MaxVisibleItemsCount";
static const int MaxVisibleItemsCount_DEFAULT	= 20;
static const String ShowPath_NAME				= "ShowPath";
static const bool ShowPath_DEFAULT				= false;

//---------------------------------------------------------------------------

void __fastcall TRecentFiles::OnRecentFileClick(TObject *Sender)
{
	TMenuItem* mi = dynamic_cast<TMenuItem*>(Sender);
	if (!mi) return;

	//Call external callback:
	if (OnSelect)
	{
    	OnSelect(_items->Strings[mi->Tag]);
      	_itemsToMenu(_popMenu->Items); //092b8: Fixes weird behaviour when clicking a dropfown toolbutton.
    }
}
//---------------------------------------------------------------------------

TRecentFiles::TRecentFiles(TMenuItem* menu_item, CALLBACK_OnSelect* on_select)
{
	_defaultMenuItem = menu_item;
	OnSelect 		= on_select;
	_items	 		= new TStringList();
	_popMenu 		= new TPopupMenu(NULL);
	// Reset:
	_maxVisibleItemsCount = MaxVisibleItemsCount_DEFAULT;
	_maxItemsCount	= MaxItemsCount_DEFAULT;
	_showHotKeys	= true;
	_showPath		= ShowPath_DEFAULT;
	clear();
}
//---------------------------------------------------------------------------

TRecentFiles::~TRecentFiles()
{
	delete _items;
	delete _popMenu;
}
//---------------------------------------------------------------------------

void TRecentFiles::clear()
{
	_items->Clear();
	if (_defaultMenuItem)
		_defaultMenuItem->Clear();
	_popMenu->Items->Clear();
}
//---------------------------------------------------------------------------

void TRecentFiles::_itemsToMenu(TMenuItem* menu_item)
{
	menu_item->Clear();
	TMenuItem* mi;
	int i = 0;
	String hot;
	while (1)
	{
		if (i == _maxVisibleItemsCount) break;
		if (i == _items->Count) break;

		mi = new TMenuItem(menu_item);
		try
		{
			if (_showHotKeys)
			{
				if (i < 10) hot = IntToStr(i);
				else hot = String( (char)((int)'A' + (i - 10)) );
				hot = "&" + hot + " - ";
			}
			if (_showPath)
				mi->Caption = hot + _items->Strings[i];
			else
				mi->Caption = hot + ExtractFileName(_items->Strings[i]);
			mi->OnClick = OnRecentFileClick;
			mi->Tag		= i;	// Use Tag to refer to the actual item.
			menu_item->Add(mi);
		}
		catch (Exception &ex)
		{
			delete mi;
			#ifdef _DEBUG
				ShowMessage("Could not add a TMenuItem. Function: " __FUNC__);
			#endif
			break;
		}
		i++;
	}
}
//---------------------------------------------------------------------------

void TRecentFiles::_updateDefaultMenu()
{
	if (_defaultMenuItem)
		_itemsToMenu(_defaultMenuItem);
}
//---------------------------------------------------------------------------

void TRecentFiles::add(String item)
{
	if (item.IsEmpty()) return;
	int pos = _items->IndexOf(item);
	if (pos != -1) _items->Delete(pos);
	_items->Insert(0, item);

	// Check _maxItemsCount:
	while (_items->Count > _maxItemsCount)
		_items->Delete(_items->Count -1 );

	_updateDefaultMenu();
}
//---------------------------------------------------------------------------

int TRecentFiles::setMaxVisibleItemsCount(int count)
{
	// Check boundaries:
	if (count < 0) count = 0;
	if (count > _maxItemsCount) count = _maxItemsCount;

	_maxVisibleItemsCount = count;
	_updateDefaultMenu();
	return count;
}
//---------------------------------------------------------------------------

int TRecentFiles::getVisibleItemsCount()
{
	if (_items->Count < _maxVisibleItemsCount)
		return _items->Count;
	else
		return _maxVisibleItemsCount;
}
//---------------------------------------------------------------------------

void TRecentFiles::setShowPath(bool show)
{
	_showPath = show;
	_updateDefaultMenu();
}
//---------------------------------------------------------------------------

bool TRecentFiles::toIniFile(String ini_fpath, String section_name)
{
	bool error = false;
	TIniFile* ini = new TIniFile(ini_fpath);
	try
	{
		ini->EraseSection(section_name);
		ini->WriteInteger(section_name, MaxVisibleItemsCount_NAME, _maxVisibleItemsCount);
		ini->WriteBool(section_name, ShowPath_NAME, _showPath);
		for (int i=0; i < _items->Count; i++)
			ini->WriteString(section_name, "Item." + IntToStr(i), _items->Strings[i]);
	}
	catch(...)
	{
		error = true;
	}
	delete ini;
	return !error;
}
//---------------------------------------------------------------------------

bool TRecentFiles::fromIniFile(String ini_fpath, String section_name)
{
	bool error = false;
	TIniFile* ini = new TIniFile(ini_fpath);
	try
	{
		clear();

		_maxVisibleItemsCount = ini->ReadInteger(section_name, MaxVisibleItemsCount_NAME, MaxVisibleItemsCount_DEFAULT);
		_showPath = ini->ReadBool(section_name, ShowPath_NAME, ShowPath_DEFAULT);

		String item;
		int i = 0;
		while (1)
		{
			item = ini->ReadString(section_name, "Item." + IntToStr(i), "");
			if (item.IsEmpty()) break;
			if (_items->IndexOf(item) == -1)	// No duplicates
				_items->Append(item);
			++i;
		}
		_updateDefaultMenu();
	}
	catch(...)
	{
		error = true;
	}
	delete ini;
	return !error;
}
//---------------------------------------------------------------------------

void TRecentFiles::popupAt(int X, int Y)
{
	_popMenu->Items->Clear();
	_itemsToMenu(_popMenu->Items);
	_popMenu->Popup(X, Y);
}
//---------------------------------------------------------------------------

TPopupMenu* TRecentFiles::getPopupMenu()
{
	_itemsToMenu(_popMenu->Items);
    return _popMenu;
}
//---------------------------------------------------------------------------

