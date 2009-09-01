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

#include "AbaktProfile.h"
#include "AbaktGlobals.h"
#include "GlobalFuncs.h"
#include "Xml_IO.h"
#include "eccFile.h"
#include "eccString.h"
#include "expat.h"
//---------------------------------------------------------------------------
TAbaktProfile* PProfile;	// Object is created in Abakt.cpp
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
static TAbtELMid __ELMid;
static TAbtELMid __ELMid2;	// Level 2 elements.
//---------------------------------------------------------------------------

TAbaktProfile::TAbaktProfile()
{
	reset();
}
//---------------------------------------------------------------------------

void TAbaktProfile::reset()
{
	modified 	= false;
	name 		= ""; // f_Main will set it to "Untitled" on Profile/New.
	fileName 	= "";
	versionStr	= PGlobals->AppVersionStr;
	buildStr    = PGlobals->AppBuildStr;

	// Set default FilePropertiesFilter Settings:
    // -- Required as long as importing pre 0.9.2-beta3.309 profiles is supported.
    config.FilePropertiesFilter.Attributes->setDefaultValues();
    config.FilePropertiesFilter.Size->setDefaultValues();
    config.FilePropertiesFilter.DateModified->setDefaultValues();
    config.FilePropertiesFilter.Age->setDefaultValues();

	// Default ActionType Settings:
	config.Actions.File->setDefaultValues();
	config.Actions.Zip->setDefaultValues();
	config.Actions.SevenZip->setDefaultValues();

	// Default Action Settings:
	config.Action->setDefaultValues();

	// Default Destination Settings:
	config.Destination->setDefaultValues();
	config.Destination->pDir->value = PGlobals->DefaultDestDir;

	// Default Finalize Settings:
	config.History->setDefaultValues();     // History
	config.FileList->setDefaultValues();    // FileList

	// Clear all lists:
	sourceRootDirList.clear();
	excludeDirList.clear();
	includeDirList.clear();
	excludeTreeList.clear();
	includeTreeList.clear();

    // Clear (old) mask lists:
    // Required as long as importing pre 0.9.2-beta3.309 profiles is supported.
	__ExcludeMaskList.clear();
	__IncludeMaskList.clear();
	__NoCompressMaskList.clear();
    // NEW (since 0.9.2-beta3):
    filterSetList.clear();
}
//---------------------------------------------------------------------------

bool TAbaktProfile::write(String fPath)
{
	fileName = fPath;

	std::ofstream ofs(fPath.c_str());
	if (!ofs) return false;

	name = ecc::ExtractFileNameNoExt(fPath);

	// Header:
	xout::XmlHeader(ofs);
	xout::rootElement(ofs, ELM_abakt_profile, name);

	// Save sections:
	_writeSect_source(ofs);
    _writeSect_FileFilters(ofs);    // Since 0.9.2-beta3.309

	// Save sections (build 225+):
	ofs	<< std::endl;
	xout::StartElm(ofs, 0, ELM_ActionTypes);
	xout::configItemList(ofs, 1, config.Actions.File->items, false);
	xout::configItemList(ofs, 1, config.Actions.Zip->items, false);
	xout::configItemList(ofs, 1, config.Actions.SevenZip->items, false);
	xout::EndElm(ofs, 0, ELM_ActionTypes);
	ofs	<< std::endl;
	xout::configItemList(ofs, 0, config.Action->items, false);
	ofs	<< std::endl;
	xout::configItemList(ofs, 0, config.Destination->items, false);
	ofs	<< std::endl;
	xout::configItemList(ofs, 0, config.History->items, false);
	ofs	<< std::endl;
	xout::configItemList(ofs, 0, config.FileList->items, false);

	// Footer:
	ofs << std::endl;
	xout::EndElm(ofs, 0, ELM_abakt_profile);

	modified = false;

	return true;
}
//---------------------------------------------------------------------------

bool TAbaktProfile::read(const String fPath)
{
	reset();
	fileName = fPath;

	// Parse xml file:
    _reachedEOF = false;
	__ELMid = ELMid_none;
	__ELMid2 = ELMid_none;
    _errorStr = "";

	if (fPath.IsEmpty())
    {
        _errorStr = __FUNC__ ": [fPath] is empty!";
        return false;
    }

    // Read/Parse [fPath]:
	bool readOk = XmlIO_ParseFile(fPath, _xml_read_start, _xml_read_end);

    if (!_reachedEOF)
    {
        _errorStr = __FUNC__ ": Parsing XML failed, closing root element not detected! Please report.";
        readOk = false;
    }

    // == Since 0.9.2-beta3.309:
    // Convert/Import old FileFilterProperties (Size, DateModified, Age, Attributes) to new FilterSets.
    if (readOk)
    {
        if (StrToInt(buildStr) < 309)    // 309 = 0.9.2-beta3
            __importProfilePreviousToBuild309();
    }

    // End:
	return readOk;
}
//---------------------------------------------------------------------------

void TAbaktProfile::__importProfilePreviousToBuild309() // 309 = 0.9.2-beta3
{
    abakt::TConfig::TFilePropertiesFilter* fpf = &PProfile->config.FilePropertiesFilter;
    TAbaktFilterSet fs;

    // Size - Import from pre 0.9.2-beta3 Profiles, convert to new FilterSet:
    fs.reset();
    if (fpf->Size->iSmaller->get() != 0)
    {
        fs.size     = "<" + IntToStr(fpf->Size->iSmaller->get());
    }
    if (fpf->Size->iGreater->get() != 0)
    {
        if (!fs.size.IsEmpty())
            fs.size = fs.size + "; ";
        fs.size     = fs.size + ">" + IntToStr(fpf->Size->iGreater->get());
    }
    if (!fs.size.IsEmpty())
    {
        fs.action   = fsaExclude;
        fs.enabled  = true;
        fs.name     = ASPF(_("Imported Size Filter: %s"), ecc::QuoteStr(fs.size).c_str());
        filterSetList.push_back(fs);
    }

    // Date - Import from pre 0.9.2-beta3 Profiles, convert to new FilterSet:
    fs.reset();
    if (fpf->DateModified->bEnabled->value == true)
    {
        fs.action   = fsaExclude;
        fs.enabled  = true;
        fs.date      = "<" + fpf->DateModified->tDate1->value
                        + "; >" + fpf->DateModified->tDate2->value;
        fs.name     = ASPF(_("Imported Date Filter: %s"), ecc::QuoteStr(fs.date).c_str());
        filterSetList.push_back(fs);
    }

    // Age - Import from pre 0.9.2-beta3 Profiles, convert to new FilterSet:
    // (Only include files with age between Age1 and Age2,
    // in other words: exclude files outside Age1 and Age2.)
    fs.reset();
    if (fpf->Age->bEnabled->value == true)
    {
        fs.action   = fsaExclude;
        fs.enabled  = true;
        fs.age      = "<" + IntToStr(fpf->Age->iAge1->get())
                        + "; >" + IntToStr(fpf->Age->iAge2->get());
        fs.name     = ASPF(_("Imported Age Filter: %s"), ecc::QuoteStr(fs.age).c_str());
        filterSetList.push_back(fs);
    }

    // Attributes - Import from pre 0.9.2-beta3 Profiles, convert to new FilterSet:
    fs.reset();
    String is = "", isnot = "";
    // import OLD attribute H setting:
    if (fpf->Attributes->eMethodH->getIndex() == fpaExclude)
        is += "H";
    else if (fpf->Attributes->eMethodH->getIndex() == fpaOnlyInclude)
        isnot += "H";
    // import OLD attribute R setting:
    if (fpf->Attributes->eMethodR->getIndex() == fpaExclude)
        is += "R";
    else if (fpf->Attributes->eMethodR->getIndex() == fpaOnlyInclude)
        isnot += "R";
    // import OLD attribute S setting:
    if (fpf->Attributes->eMethodS->getIndex() == fpaExclude)
        is += "S";
    else if (fpf->Attributes->eMethodS->getIndex() == fpaOnlyInclude)
        isnot += "S";

    if (!is.IsEmpty())
        fs.attributes = "==" + is + "; ";
    if (!isnot.IsEmpty())
        fs.attributes += "!=" + isnot + ";";
    fs.attributes = fs.attributes.Trim();

    // Add imported FilterSet (if any) to exclude list:
    if (!fs.attributes.IsEmpty())
    {
        fs.action   = fsaExclude;
        fs.enabled  = true;
        fs.name     = ASPF(_("Imported Attribute Filter: %s"), ecc::QuoteStr(fs.attributes).c_str());
        filterSetList.push_back(fs);
    }
}
//---------------------------------------------------------------------------

void TAbaktProfile::_xml_read_start(void *data, const char *el, const char **attr)
{
	String elm = String(el);
	String atr, val;

	// <abakt-profile>:
	if (elm == ELM_abakt_profile)
	{
		PProfile->name		= xin::AttrValStr("name", attr);
		PProfile->versionStr = xin::AttrValStr("version", attr);
		PProfile->buildStr  = xin::AttrValStr("build", attr);
		return;
	}

	// <source>:
	if (elm == ELM_source)
	{
		__ELMid = ELMid_source;
		return;
	}
	if (__ELMid == ELMid_source)
	{
		PProfile->_readSect_source(elm, attr);
		return;
	}

    // <FileFilters>: (Added in 0.9.2-beta3.309)
	if (elm == ELM_FileFilters)
	{
		__ELMid = ELMid_FileFilters;
		return;
	}
	if (__ELMid == ELMid_FileFilters)
	{
		xin::readSect_FileFilters_Filter_intoFSList(elm, attr, PProfile->filterSetList);
		return;
	}

	// <filters>:
    // -- Required as long as importing pre 0.9.2-beta3.309 profiles is supported.
	if (elm == ELM_filters)
	{
		__ELMid = ELMid_filters;
		return;
	}
	if (__ELMid == ELMid_filters)
	{
		PProfile->_readSect_filters(elm, attr);
		return;
	}

	// <FilePropertiesFilters>:
    // -- Required as long as importing pre 0.9.2-beta3.309 profiles is supported.
	if (elm == ELM_FilePropertiesFilter)
	{
		__ELMid = ELMid_FilePropertiesFilter;
		return;
	}
	if (__ELMid == ELMid_FilePropertiesFilter)
	{
		PProfile->_readSect_FilePropertiesFilter(elm, attr);
		return;
	}

	// <ActionTypes>:
	if (elm == ELM_ActionTypes)
	{
		__ELMid = ELMid_ActionTypes;
		__ELMid2 = ELMid_none;
		return;
	}
	if (__ELMid == ELMid_ActionTypes)
	{
		PProfile->_readSect_ActionTypes(elm, attr);
		return;
	}

	// <Action>:
	if (elm == ELM_Action)
	{
		__ELMid = ELMid_Action;
		return;
	}
	if (__ELMid == ELMid_Action)
	{
		PProfile->_readSect_Action(elm, attr);
		return;
	}

	// <Destination>:
	if (elm == ELM_Destination)
	{
		__ELMid = ELMid_Destination;
		return;
	}
	if (__ELMid == ELMid_Destination)
	{
		PProfile->_readSect_Destination(elm, attr);
		return;
	}

	// <History>:
	if (elm == ELM_History)
	{
		__ELMid = ELMid_History;
		return;
	}
	if (__ELMid == ELMid_History)
	{
		PProfile->_readSect_History(elm, attr);
		return;
	}

	// <FileList>:
	if (elm == ELM_FileList)
	{
		__ELMid = ELMid_FileList;
		return;
	}
	if (__ELMid == ELMid_FileList)
	{
		PProfile->_readSect_FileList(elm, attr);
		return;
	}
}
//---------------------------------------------------------------------------

void TAbaktProfile::_xml_read_end(void *data, const char *el)
{
	String elm = String(el);

	if (elm == ELM_source)
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_FileFilters)
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_filters)
    // -- Required as long as importing pre 0.9.2-beta3.309 profiles is supported.
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_FilePropertiesFilter)
    // -- Required as long as importing pre 0.9.2-beta3.309 profiles is supported.
	{
		__ELMid = ELMid_none;
		__ELMid2 = ELMid_none;
		return;
	}

	if (elm == ELM_ActionTypes)
	{
		__ELMid = ELMid_none;
		__ELMid2 = ELMid_none;
		return;
	}

	if (elm == ELM_Action)
	{
		__ELMid = ELMid_none;
		return;
	}
	if (elm == ELM_Destination)
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_History)
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_FileList)
	{
		__ELMid = ELMid_none;
		return;
	}

	// Detect closing root element </abakt-profile>:
	if (elm == ELM_abakt_profile)
    {
        PProfile->_reachedEOF = true;
    }
}
//---------------------------------------------------------------------------

//===========================================================================
//	<source>
//===========================================================================

void TAbaktProfile::_writeSect_source(std::ofstream& ofs)
{
	String rootdir;
	String dir;

	// <source>:
	ofs << std::endl
		<< "<" << ELM_source << ">" << std::endl;

	for (unsigned int i = 0; i < sourceRootDirList.size(); i++)
	{
		rootdir = sourceRootDirList[i];

		// <root>:
		ofs	<< "\t<" << ELM_src_root
			<< " dir='" << ecc::StrToXmlUtf8Str(rootdir).c_str() << "'";
		ofs	<< ">" << std::endl;

		// <exclude tree>:
		for (unsigned int j = 0; j < excludeTreeList.size(); j++)
		{
			dir = excludeTreeList[j];
			if (dir.Pos(rootdir) != 1) continue;
			// Strip source dir path:
			dir = dir.SubString(rootdir.Length() + 1, dir.Length());
			ofs << "\t\t<" << ELM_src_exclude
				<< " tree='" << ecc::StrToXmlUtf8Str(dir).c_str() << "'"
				<< "/>" << std::endl;
		}

		// <include tree>:
		for (unsigned int j = 0; j < includeTreeList.size(); j++)
		{
			dir = includeTreeList[j];
			if (dir.Pos(rootdir) != 1) continue;
			// Strip source dir path:
			dir = dir.SubString(rootdir.Length() + 1, dir.Length());
			ofs << "\t\t<" << ELM_src_include
				<< " tree='" << ecc::StrToXmlUtf8Str(dir).c_str() << "'"
				<< "/>" << std::endl;
		}

		// <exclude dir>:
		for (unsigned int j = 0; j < excludeDirList.size(); j++)
		{
			dir = excludeDirList[j];
			if (dir.Pos(rootdir) != 1) continue;
			// Strip source dir path:
			dir = dir.SubString(rootdir.Length() + 1, dir.Length());
			ofs << "\t\t<" << ELM_src_exclude
				<< " dir='" << ecc::StrToXmlUtf8Str(dir).c_str() << "'"
				<< "/>" << std::endl;
		}

		// <include>:
		for (unsigned int j = 0; j < includeDirList.size(); j++)
		{
			dir = includeDirList[j];
			if (dir.Pos(rootdir) != 1) continue;
			// Strip source dir path:
			dir = dir.SubString(rootdir.Length() + 1, dir.Length());
			ofs << "\t\t<" << ELM_src_include
				<< " dir='" << ecc::StrToXmlUtf8Str(dir).c_str() << "'"
				<< "/>" << std::endl;
		}

		// </root>:
		ofs	<< "\t</" << ELM_src_root << ">" << std::endl;
	}

	// </source>:
	ofs	<< "</" << ELM_source << ">" << std::endl;
}
//---------------------------------------------------------------------------

void TAbaktProfile::_readSect_source(String elm, const char **attr)
// Assumes __ELMid == ELMid_source.
{
	if (elm == ELM_src_exclude)
	{
		_readSect_source_root_exclude(attr);
		return;
	}

	if (elm == ELM_src_include)
	{
		_readSect_source_root_include(attr);
		return;
	}

	if (elm == ELM_src_root)
	{
		_readSect_source_root(attr);
		return;
	}
}
//---------------------------------------------------------------------------

void TAbaktProfile::_readSect_source_root(const char **attr)
{
	String atr, val;
	String item;

	for (int i = 0; attr[i]; i += 2)
	{
		atr = String(attr[i]);
		val = String(attr[i + 1]);
		if (atr == "dir")
		{
			val = IncludeTrailingPathDelimiter(val); // Backward compatibility with pre 0.8.
			item = Utf8ToAnsi(val);
		}
	}
	sourceRootDirList.push_back(item);
}
//---------------------------------------------------------------------------

void TAbaktProfile::_readSect_source_root_exclude(const char **attr)
{
	String atr, val;

	for (int i = 0; attr[i]; i += 2)
	{
		atr = String(attr[i]);
		val = String(attr[i + 1]);
		if (atr == "dir")
		{
			if (!val.Pos(":")) val = sourceRootDirList.back() + val;
			val = IncludeTrailingPathDelimiter(val); // Backward compatibility with pre 0.8.
			excludeDirList.push_back(Utf8ToAnsi(val));
		}
		if (atr == "tree")
		{
			if (!val.Pos(":")) val = sourceRootDirList.back() + val;
			excludeTreeList.push_back(Utf8ToAnsi(val));
		}
	}
}
//---------------------------------------------------------------------------

void TAbaktProfile::_readSect_source_root_include(const char **attr)
{
	String atr, val;

	for (int i = 0; attr[i]; i += 2)
	{
		atr = String(attr[i]);
		val = String(attr[i + 1]);
		if (atr == "dir")
		{
			if (!val.Pos(":")) val = sourceRootDirList.back() + val;
			val = IncludeTrailingPathDelimiter(val); // Backward compatibility with pre 0.8.
			includeDirList.push_back(Utf8ToAnsi(val));
		}
		if (atr == "tree")
		{
			if (!val.Pos(":")) val = sourceRootDirList.back() + val;
			includeTreeList.push_back(Utf8ToAnsi(val));
		}
	}
}
//---------------------------------------------------------------------------

//===========================================================================
//	<FileFilters>
//===========================================================================

void TAbaktProfile::_writeSect_FileFilters(std::ofstream& ofs)
{
	ofs << std::endl;
	ofs << "<" << ELM_FileFilters << ">" << std::endl;
	for (unsigned int i = 0; i < filterSetList.size(); i++)
	{
		ofs << "\t"
			<< FilterSetToXmlStr(filterSetList[i]).c_str()
            << std::endl;
	}
	ofs << "</" << ELM_FileFilters << ">" << std::endl;
}
//---------------------------------------------------------------------------

void TAbaktProfile::_readSect_filters(String elm, const char **attr)
// Assumes __ELMid == ELMid_filters.
{
	if (elm == ELM_filt_exclude)
	{
		_readSect_filters_masklist(attr, __ExcludeMaskList, fsaExclude);
		return;
	}
	if (elm == ELM_filt_include)
	{
		_readSect_filters_masklist(attr, __IncludeMaskList, fsaInclude);
		return;
	}
	if (elm == ELM_filt_no_compress)
	{
		_readSect_filters_masklist(attr, __NoCompressMaskList, fsaNoCompression);
		return;
	}
}
//---------------------------------------------------------------------------

void TAbaktProfile::_readSect_filters_masklist(const char **attr,
        std::vector<ecc::TCheckStringListItem> &list,
        TAbaktFilterSetAction fsAction)
{
	String atr, val, str;
	bool checked = false;

	for (int i = 0; attr[i]; i += 2)
	{
		atr = String(attr[i]);
		val = String(attr[i+1]);
		if (atr == "mask")
			str = Utf8ToAnsi(val);
		if (atr == "selected")
			checked = ecc::StrToBool(val);
	}

	// Assumes mask and/or sel are not to empty:
	if (!_strInCheckStringListNoCase(str, list))
	{
		ecc::TCheckStringListItem cs_item;
		cs_item.checked = checked;
		cs_item.str		= str;
		list.push_back(cs_item);

        // Convert pre-0.9.2-beta3 masklist items to new TAbaktFilterSet:
        TAbaktFilterSet fs;
        fs.name     = ASPF(_("Imported Path Mask: %s"), ecc::QuoteStr(str).c_str());
        fs.action   = fsAction;
        fs.enabled  = checked;
        fs.path     = str;   //ecc::QuoteStr(str);
        filterSetList.push_back(fs);
	}
}
//---------------------------------------------------------------------------

//===========================================================================
// (OBSOLETE since 0.9.2-beta3:)

// -- Required as long as importing pre 0.9.2-beta3.309 profiles is supported.

//	<FilePropertiesFilter>
//      <Attributes/>
//      <Size/>
//      <DateModified/>
//      <Age/>
//===========================================================================

void TAbaktProfile::_readSect_FilePropertiesFilter(String elm, const char **attr)
{
	// <Attributes>:
	if (elm == ELM_Fpf_Attributes)
	{
		__ELMid2 = ELMid_Fpf_Attributes;
		return;
	}
	if (__ELMid2 == ELMid_Fpf_Attributes)
	{
		xin::itemAttrsToList(attr, config.FilePropertiesFilter.Attributes->items);
	}

	// Size:
	if (elm == ELM_Fpf_Size)
	{
		__ELMid2 = ELMid_Fpf_Size;
		return;
	}
	if (__ELMid2 == ELMid_Fpf_Size)
	{
		xin::itemAttrsToList(attr, config.FilePropertiesFilter.Size->items);
	}

	// <DateModified>:
	if (elm == ELM_Fpf_DateModified)
	{
		__ELMid2 = ELMid_Fpf_DateModified;
		return;
	}
	if (__ELMid2 == ELMid_Fpf_DateModified)
	{
		xin::itemAttrsToList(attr, config.FilePropertiesFilter.DateModified->items);
	}

	// <Age>:
	if (elm == ELM_Fpf_Age)
	{
		__ELMid2 = ELMid_Fpf_Age;
		return;
	}
	if (__ELMid2 == ELMid_Fpf_Age)
	{
		xin::itemAttrsToList(attr, config.FilePropertiesFilter.Age->items);
	}
}
//---------------------------------------------------------------------------

//===========================================================================
//	<ActionTypes>
//      <File/>
//      <Zip/>
//      <SevenZip/>
//===========================================================================

void TAbaktProfile::_readSect_ActionTypes(String elm, const char **attr)
{
	// <Zip>:
	if (elm == ELM_Act_Zip)
	{
		__ELMid2 = ELMid_Act_Zip;
		return;
	}
	if (__ELMid2 == ELMid_Act_Zip)
	{
		xin::itemAttrsToList(attr, config.Actions.Zip->items);
	}

	// <SevenZip>:
	if (elm == ELM_Act_7Zip)
	{
		__ELMid2 = ELMid_Act_7Zip;
		return;
	}
	if (__ELMid2 == ELMid_Act_7Zip)
	{
		xin::itemAttrsToList(attr, config.Actions.SevenZip->items);
	}

	// <File>:
	if (elm == ELM_Act_File)
	{
		__ELMid2 = ELMid_Act_File;
		return;
	}
	if (__ELMid2 == ELMid_Act_File)
	{
		xin::itemAttrsToList(attr, config.Actions.File->items);
	}
}
//---------------------------------------------------------------------------

//===========================================================================
//	<Action>
//===========================================================================

void TAbaktProfile::_readSect_Action(String elm, const char **attr)
{
	xin::itemAttrsToList(attr, config.Action->items);
}
//---------------------------------------------------------------------------

//===========================================================================
//	<Destination>
//===========================================================================

void TAbaktProfile::_readSect_Destination(String elm, const char **attr)
{
   	xin::itemAttrsToList(attr, config.Destination->items);
}
//---------------------------------------------------------------------------

//===========================================================================
//	<History>
//===========================================================================

void TAbaktProfile::_readSect_History(String elm, const char **attr)
{
	xin::itemAttrsToList(attr, config.History->items);
}
//---------------------------------------------------------------------------

//===========================================================================
//	<FileList>
//===========================================================================

void TAbaktProfile::_readSect_FileList(String elm, const char **attr)
{
	xin::itemAttrsToList(attr, config.FileList->items);
}
//---------------------------------------------------------------------------

//===========================================================================
//	Global-usage functions
//===========================================================================

//---------------------------------------------------------------------------

bool TAbaktProfile::dirInExcludeDirList(const String dir)
{
	for (unsigned int i = 0; i < excludeDirList.size(); i++)
	{
		if (dir.AnsiCompareIC(excludeDirList[i]) == 0)
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------

bool TAbaktProfile::dirInIncludeDirList(const String dir)
{
	for (unsigned int i = 0; i < includeDirList.size(); i++)
	{
		if (dir.AnsiCompareIC(includeDirList[i]) == 0)
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------

bool TAbaktProfile::dirInExcludeTreeList(const String dir)
{
	for (unsigned int i = 0; i < excludeTreeList.size(); i++)
	{
		if (dir.AnsiCompareIC(excludeTreeList[i]) == 0)
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------

bool TAbaktProfile::dirIsPartOfExcludeTree(const String dir)
{
	for (unsigned int i = 0; i < excludeTreeList.size(); i++)
	{
		if (dir.LowerCase().Pos( excludeTreeList[i].LowerCase() ) == 1) // 1 = Position of first character.
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------

bool TAbaktProfile::dirIsPartOfIncludeTree(const String dir)
{
	for (unsigned int i = 0; i < includeTreeList.size(); i++)
	{
		if (dir.LowerCase().Pos( includeTreeList[i].LowerCase() ) == 1) // 1 = Position of first character.
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------

bool TAbaktProfile::_strInCheckStringListNoCase(String str, std::vector<ecc::TCheckStringListItem>& list)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (str.AnsiCompareIC(list[i].str) == 0)
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------

