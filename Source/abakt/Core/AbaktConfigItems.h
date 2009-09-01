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
#ifndef AbaktConfigItemsH
#define AbaktConfigItemsH
//---------------------------------------------------------------------------

//===========================================================================
namespace abakt {
//===========================================================================

class TConfigSet;  // Forward declaration, used in TItemList.owner.
//---------------------------------------------------------------------------

typedef enum
{
	dtUndefined, // Default.
	dtBool,
	dtUIntRange,
	dtText,
	dtEnum,
	dtPath
} TItemType;

const AnsiString ItemTypeToStr(const TItemType type);

const TItemType StrToItemType(const AnsiString str);
//===========================================================================

class TItemList;	// Forward declaration.

/* 	abakt::TItem is the base class for Abakt (plugin) data types. */

class TItem
{
	protected:
		TItemType _type;

	public:
		virtual ~TItem();	// A base class needs a virtual destructor.

		TItem();

		TItemType type(void);

		AnsiString name;   	// Internal English name of the item. Should be unique in every list.
		AnsiString caption; // Caption of the item, used in GUI.
		AnsiString info;   	// Information: 'What does this item do?', used in GUI.

		TItemList* owner;	// Pointer to the TItemList that owns this TItem.

		bool override;		// Used by AbaktGroup. Defaults to false.
};
//===========================================================================

class TBool : public TItem
{
	public:
		TBool();
		bool value;	// Defaults to false.
};
//===========================================================================

class TUIntRange : public TItem
{
    /*  Even though this class is meant to hold an unsigned integer,
        a signed __int64 is used (wxWidgets-Abakt uses wxLongLong, also signed).
        This is not likely to cause any problems, I hope... */

	private:
		__int64 _data;
		__int64 _min;
		__int64 _max;
	public:
		TUIntRange(const __int64 val, const __int64 min, const __int64 max);
		__int64 get(void);
		__int64 getMin(void);
		__int64 getMax(void);
		__int64 set(const __int64 val);
};
//===========================================================================

class TText : public TItem
{
	public:
		TText();
		AnsiString value;
        bool masked;    // Should value be displayed masked (for passwords).
};
//===========================================================================

class TPath : public TItem
{
	public:
		TPath();
		AnsiString value;
};
//===========================================================================

class TEnum : public TItem
{
	private:
		int _index;
		TStringList* _items;

	public:
		TEnum();
		~TEnum();

		bool add(const AnsiString str);

		int getIndex(void);

		void setIndex(const int idx);

		const AnsiString at(const int idx);

		const AnsiString selected(void);
        /*  Returns the string associated with _index. */

		int indexOf(const AnsiString str);

		unsigned int size(void);
};
//===========================================================================

static const unsigned int ABT_ITEMLIST_MAXSIZE = 64;

class TItemList {

	private:
		unsigned int _size;
		TItem* _items[ABT_ITEMLIST_MAXSIZE];

	public:
		TItemList();

		void destroy(void);
		/*	Delete TItem's create with create...() functions. */

		AnsiString name;   	// Internal English name of the list. Should be unique for every TConfigSet.
		AnsiString caption; // Caption of the list, used in GUI.
        TConfigSet* owner;  // Pointer to the TConfigSet that owns this TItemList.

		unsigned int size(void) const;

		TBool* createBool(const AnsiString name);
		TUIntRange* createUIntRange(const AnsiString name,
				const unsigned long val, const unsigned long min, const unsigned long max);
		TText* createText(const AnsiString name, const bool masked = false);
		TPath* createPath(const AnsiString name);
		TEnum* createEnum(const AnsiString name);
		/*  The create...() functions:
			- create a new TItem object and add its pointer to the list.
			- return NULL if the TItem object could not be created. */

		bool add(TItem* item);
		/*  Adds a pointer to an already existing TItem to the list.
			Returns false if the pointer could not be added. */

		TItem* at(const unsigned int idx);
		/* 	Returns NULL if idx >= _size. */

		int indexOfItem(const TItem *item);
		/*	Returns the index of the TItem object [item].
			Returns -1 if [item] is not in the list. */

		int indexOfName(const AnsiString& itemName);
		/*	Returns the index of the TItem object with name [itemName].
			Returns -1 if [itemName] could not be found. */
};
//===========================================================================


//===========================================================================
};	// namespace abakt
//===========================================================================

//---------------------------------------------------------------------------
#endif

