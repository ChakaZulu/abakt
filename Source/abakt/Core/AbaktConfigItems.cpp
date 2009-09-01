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

#include "AbaktConfigItems.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//===========================================================================
namespace abakt {
//===========================================================================

//===========================================================================

const String TItemTypeStrList[] =
{
	"Undefined",	// Default.
	"Bool",
    "UIntRange",
	"Text",
	"Enum",
	"Path"
};
static const int TItemTypeStrListSize
    = sizeof(TItemTypeStrList)/sizeof(TItemTypeStrList[0]);

//---------------------------------------------------------------------------

const String ItemTypeToStr(const TItemType type)
{
	if (type >= TItemTypeStrListSize)
		return TItemTypeStrList[dtUndefined]; // Default

	return TItemTypeStrList[type];
}
//---------------------------------------------------------------------------

const TItemType StrToItemType(const String str)
{
	for (int i = 0; i < TItemTypeStrListSize; i++)
	{
		if (str == TItemTypeStrList[i])
			return (TItemType)i;
	}
	return dtUndefined;	// Default
}
//---------------------------------------------------------------------------

//===========================================================================

TItem::~TItem()
{
	_type	= dtUndefined;
	override = false;
}
//---------------------------------------------------------------------------

TItem::TItem()
{
	_type	= dtUndefined;
    owner   = NULL;
}
//---------------------------------------------------------------------------

TItemType TItem::type(void)
{
	return _type;
}
//---------------------------------------------------------------------------

//===========================================================================

TBool::TBool()
{
	_type	= dtBool;
	value	= false;
}
//---------------------------------------------------------------------------

//===========================================================================

TUIntRange::TUIntRange(const __int64 val, const __int64 min, const __int64 max)
{
	_type = dtUIntRange;
	_data = val;
	_min = min;
	_max = max;
}
//---------------------------------------------------------------------------

__int64 TUIntRange::get(void)
{
	return _data;
}
//---------------------------------------------------------------------------

__int64 TUIntRange::getMin(void)
{
	return _min;
}
//---------------------------------------------------------------------------

__int64 TUIntRange::getMax(void)
{
	return _max;
}
//---------------------------------------------------------------------------

__int64 TUIntRange::set(const __int64 val)
{
	if (val < _min) _data = _min;
	else if (val > _max) _data = _max;
	else _data = val;
	return _data;
}
//---------------------------------------------------------------------------

//===========================================================================

TText::TText()
{
	_type	= dtText;
    masked  = false;
}
//---------------------------------------------------------------------------

//===========================================================================

TPath::TPath()
{
	_type	= dtPath;
}
//---------------------------------------------------------------------------

//===========================================================================

TEnum::TEnum()
{
	_type	= dtEnum;
	_index	= -1;
	_items = new TStringList();
}
//---------------------------------------------------------------------------

TEnum::~TEnum()
{
	delete _items;
}
//---------------------------------------------------------------------------

bool TEnum::add(const AnsiString str)
{
	//if (!_items.add(str)) return false;
	_items->Append(str);

	if (_index == -1) _index = 0;	// Set default _index to first added item (=0);
	return true;
}
//---------------------------------------------------------------------------

int TEnum::getIndex(void)
{
	return _index;
}
//---------------------------------------------------------------------------

void TEnum::setIndex(const int i)
{
	if (i >= _items->Count) return; // _index = -1;
	else _index = i;
}
//---------------------------------------------------------------------------

const AnsiString TEnum::at(const int idx)
{
	if (idx >= _items->Count) return "";
	if (idx < 0) return "";
	return _items->Strings[idx];
}
//---------------------------------------------------------------------------

const AnsiString TEnum::selected(void)
{
	if (_index < 0) return "";
	return _items->Strings[_index];
}
//---------------------------------------------------------------------------

int TEnum::indexOf(const AnsiString str)
{
	return _items->IndexOf(str);
}
//---------------------------------------------------------------------------

unsigned int TEnum::size(void)
{
	return _items->Count;
}
//---------------------------------------------------------------------------

//===========================================================================

TItemList::TItemList()
{
	_size = 0;
    owner = NULL;
}
//---------------------------------------------------------------------------

void TItemList::destroy(void)
{
	for (unsigned int i = 0; i < _size; i++)
	{
		delete _items[i]; // Delete TItems's.
	}
	_size = 0;
}
//---------------------------------------------------------------------------

unsigned int TItemList::size(void) const
{
	return _size;
}
//---------------------------------------------------------------------------

TBool* TItemList::createBool(const AnsiString name)
{
	if (_size >= ABT_ITEMLIST_MAXSIZE) return NULL;
	TBool* item 		= new TBool();
	item->name 			= name;
	item->owner			= this;
	_items[_size++]		= item;
	return item;
}
//---------------------------------------------------------------------------

TUIntRange* TItemList::createUIntRange(const AnsiString name,
		const unsigned long val, const unsigned long min, const unsigned long max)
{
	if (_size >= ABT_ITEMLIST_MAXSIZE) return NULL;
	TUIntRange* item	= new TUIntRange(val, min, max);
	item->name			= name;
	item->owner			= this;
	_items[_size++]		= item;
	return item;
}
//---------------------------------------------------------------------------

TText* TItemList::createText(const AnsiString name, const bool masked)
{
	if (_size >= ABT_ITEMLIST_MAXSIZE) return NULL;
	TText* item 		= new TText();
	item->name 			= name;
	item->owner 		= this;
	_items[_size++] 	= item;
    item->masked        = masked;
	return item;
}
//---------------------------------------------------------------------------

TPath* TItemList::createPath(const AnsiString name)
{
	if (_size >= ABT_ITEMLIST_MAXSIZE) return NULL;
	TPath* item 		= new TPath();
	item->name 			= name;
	item->owner 		= this;
	_items[_size++] 	= item;
	return item;
}
//---------------------------------------------------------------------------

TEnum* TItemList::createEnum(const AnsiString name)
{
	if (_size >= ABT_ITEMLIST_MAXSIZE) return NULL;
	TEnum* item 		= new TEnum();
	item->name 			= name;
	item->owner 		= this;
	_items[_size++] 	= item;
	return item;
}
//---------------------------------------------------------------------------

bool TItemList::add(TItem* item)
{
	if (_size >= ABT_ITEMLIST_MAXSIZE) return false;
	for (unsigned int i = 0; i < _size; i++)
	{	// Exists?
		if (_items[i] == item) return false;
	}
	_items[_size++] = item;
	return true;
}
//---------------------------------------------------------------------------

TItem* TItemList::at(const unsigned int idx)
{
	if (idx >= _size) return NULL;
	return _items[idx];
}
//---------------------------------------------------------------------------

int TItemList::indexOfItem(const TItem *item)
{
	for (unsigned int i = 0; i < _size; i++)
	{
		if (_items[i] == item) return i;
	}
	return -1;
}
//---------------------------------------------------------------------------

int TItemList::indexOfName(const AnsiString& itemName)
{
	for (unsigned int i = 0; i < _size; i++)
	{
		if (_items[i]->name == itemName)
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------

//===========================================================================
};	// namespace abakt
//===========================================================================

