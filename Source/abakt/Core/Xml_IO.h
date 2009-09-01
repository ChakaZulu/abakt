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
#ifndef Xml_IOH
#define Xml_IOH
//---------------------------------------------------------------------------
#include "expat.h"
#include "AbaktConfigItems.h"
#include <fstream>
#include "AbaktFilterSet.h"
#include <vector>
//---------------------------------------------------------------------------
/*  In this file most of the Xml IO (read/write & helper) functions are declared.
    For most functions, the namespaces 'xin' and 'xout' are required. */
//---------------------------------------------------------------------------

//===========================================================================

bool XmlIO_ParseFile(const String fPath,
		const XML_StartElementHandler& start, const XML_EndElementHandler& end);
/*	Opens and parses the XML file specified by fpath,
	using [start] and [end] as the element handlers.
	Returns false on error.	*/

//==========================================================================
namespace xout {
//===========================================================================

	void XmlHeader(std::ofstream &ofs);
    /*  Writes this standard XML header:
            "<?xml version='1.0' encoding='UTF-8'?>" */

	//---------------------------------------------------------------------------

	void rootElement(std::ofstream &ofs, const char* elmName, const String docName);
	/*	Writes (unclosed) XML root element.
        Includes attributes name (docName), version and build. */

	//---------------------------------------------------------------------------

	void configItemList(std::ofstream& ofs, int tabs, abakt::TItemList* itemlist, bool includeOverride = false);
	void _item_header(std::ofstream &ofs, int tabs, abakt::TItem* item, bool includeOverride = false);
	void itemBool(std::ofstream &ofs, int tabs, abakt::TBool* item, bool includeOverride = false);
	void itemUIntRange(std::ofstream &ofs, int tabs, abakt::TUIntRange* item, bool includeOverride = false);
	void itemText(std::ofstream &ofs, int tabs, abakt::TText* item, bool includeOverride = false);
	void itemPath(std::ofstream &ofs, int tabs, abakt::TPath* item, bool includeOverride = false);
	void itemEnum(std::ofstream &ofs, int tabs, abakt::TEnum* item, bool includeOverride = false);

	//---------------------------------------------------------------------------

	void Tabs(std::ofstream &ofs, int tabs);
	/*	Writes [tabs] tabs ('\t') to ofs. */

	void StartElm(std::ofstream &ofs, int tabs, const char* elm_name);
	/*	Writes to ofs: "<[elm_name]>". */

	void EndElm(std::ofstream &ofs, int tabs, const char* elm_name);
	/*	Writes to ofs: "</[elm_name]>". */

	void Elm(std::ofstream &ofs, int tabs, const char* elm_name,
			bool close = true);
	/*	Writes to ofs: "<[elm_name]/>".
		If [close] is false, the element is not closed. */

	void ElmBool(std::ofstream &ofs, int tabs, const char* elm_name, const bool val,
			bool close = true);
	/*	Writes to ofs: "<[elm_name] bool='[val]'/>".
		Where [val] is "true" or "false".
		If [close] is false, the element is not closed. */

	void ElmInt(std::ofstream &ofs, int tabs, const char* elm_name, const long val,
			bool close = true);
	/*	Writes to ofs: "<[elm_name] int='[val]'>".
		Where [val] is an IntToStr().c_str().
		If [close] is false, the element is not closed. */

	void ElmUInt(std::ofstream &ofs, int tabs, const char* elm_name, const unsigned long val,
			bool close = true);
	/*	Writes to ofs: "<[elm_name] uint='[val]'>".
		Where [val] is an IntToStr().c_str().
		If [close] is false, the element is not closed. */

	void ElmEnum(std::ofstream &ofs, int tabs, const char* elm_name, const int val,
			bool close = true);
	/*	Writes to ofs: "<[elm_name] enum='[val]'>".
		Same as WriteElmInt(). */

	void ElmEnumStr(std::ofstream &ofs, int tabs, const char* elm_name, const String val,
			bool close = true);
	/*	Writes to ofs: "<[elm_name] enumstr='[val]'>".
		Before writing, [val] is converted to an XML-UTF8 string.
		Same as WriteElmStr(). */

	void ElmStr(std::ofstream &ofs, int tabs, const char* elm_name, const String val,
			bool close = true);
	/*	Writes to ofs: "<[elm_name] string='[val]'/>".
		Before writing, [val] is converted to an XML-UTF8 string.
		If [close] is false, the element is not closed. */

	void Comment(std::ofstream &ofs, int tabs, const char* comment);
	/*	Writes a comment to ofs: "<!-- [comment] -->". */

//===========================================================================
} // namespace xout;    CLOSE
//===========================================================================

//===========================================================================
namespace xin {
//===========================================================================

    void readSect_FileFilters_Filter_intoFSList(String elm, const char **attr,
            std::vector<TAbaktFilterSet> &fsList);
    /*  Implemented in 0.9.2.beta3:
        Combines and replaces the _readSect_FileFilters_Filter() functions in Profile/Group/Template... */

	void itemAttrsToList(const char **attr, abakt::TItemList* itemlist);
	/*	Finds the appropriate abakt::TItem in [itemlist],
		and sets it value. */

	bool selected(const char **attr, const bool def = false);

	bool Bool(const char **attr, const bool def = false);

	long Int(const char **attr, const long def = 0);

	unsigned long UInt(const char **attr, const unsigned long def = 0);

	int Enum(const char **attr, const int def = 0);

	String EnumStr(const char **attr);

	String Str(const char **attr);
	/* 	Returns Utf8ToAnsi() value of attribute 'string'. */

	bool AttrValBool(const String attr_name, const char **attr, const bool def = false);
	/* 	Returns true if attr_name's value is "true".
		Returns [def] otherwise. */

	int AttrValInt(const String attr_name, const char **attr, const int def = 0);
	/* 	Returns StrToIntDef( [attr_name]'s value, [def]). */

	String AttrValStr(const String attr_name, const char **attr);
	/* 	Returns Utf8ToAnsi() value of attribute [attr_name]. */

//===========================================================================
} // namespace xin;     CLOSE
//===========================================================================

#endif
