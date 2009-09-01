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

#include "AbaktGroup.h"
#include "Group.h"
#include "GlobalFuncs.h"
#include "AbaktProfile.h"
#include "XML_IO.h"
#include "eccString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TAbaktGroup* PGroup;	// Object is created in Abakt.cpp
//---------------------------------------------------------------------------
static const char* ABT_GROUP_XML_ROOT_ELM = "abakt-group";
static TAbtELMid __ELMid;
static TAbtELMid __ELMid2;	// Level 2 elements.
//---------------------------------------------------------------------------

TAbaktGroup::TAbaktGroup()
{
	reset();
}
//---------------------------------------------------------------------------

void TAbaktGroup::reset()
{
	modified	= false;
	name        = _("Untitled");
	fileName	= "";

    profileList.clear();
    customFilterSetList.clear();
    Config.reset();
}
//---------------------------------------------------------------------------

bool TAbaktGroup::write(String fPath)
{
    fileName = fPath;
	std::ofstream ofs(fPath.c_str());
	if (!ofs) return false;

	xout::XmlHeader(ofs);

	xout::rootElement(ofs, ABT_GROUP_XML_ROOT_ELM, name);

	// Profiles:
	ofs << std::endl
		<< "<profiles>" << std::endl;
		for (unsigned int i=0; i < profileList.size(); i++)
		{
			ofs << "\t"
				<< "<profile"
				<< " filename='" << ecc::StrToXmlUtf8Str(profileList[i].str).c_str() << "'"
    			<< " selected='" << ecc::BoolToStr(profileList[i].checked).c_str() << "'"
				<< "/>" << std::endl;
		}
		ofs << "</profiles>" << std::endl;

	ofs << std::endl;
	ofs << "<" << ELM_FileFilters << ">" << std::endl;
	for (unsigned int i = 0; i < customFilterSetList.size(); i++)
	{
		ofs << "\t"
			<< FilterSetToXmlStr(customFilterSetList[i]).c_str()
            << std::endl;
	}
	ofs << "</" << ELM_FileFilters << ">" << std::endl;

	ofs	<< std::endl;
	xout::StartElm(ofs, 0, ELM_ActionTypes);
	xout::configItemList(ofs, 1, Config.Actions.File->items, true);
	xout::configItemList(ofs, 1, Config.Actions.Zip->items, true);
	xout::configItemList(ofs, 1, Config.Actions.SevenZip->items, true);
	xout::EndElm(ofs, 0, ELM_ActionTypes);

	ofs	<< std::endl;
	xout::configItemList(ofs, 0, Config.Action->items, true);

	ofs	<< std::endl;
	xout::configItemList(ofs, 0, Config.Destination->items, true);

	ofs	<< std::endl;
	xout::configItemList(ofs, 0, Config.History->items, true);

	ofs	<< std::endl;
	xout::configItemList(ofs, 0, Config.FileList->items, true);

	// End:
	ofs << std::endl;
	xout::EndElm(ofs, 0, ABT_GROUP_XML_ROOT_ELM);

	return true;
}
//---------------------------------------------------------------------------

bool TAbaktGroup::read(const String fPath)
{
	// Clear/Reset things:
	reset();
	modified = false;
	fileName = fPath;

    _reachedEOF = false;
	__ELMid = ELMid_none;
	__ELMid2 = ELMid_none;

	// 092b7:
	if (fPath.IsEmpty())
    {
		ShowMessage(__FUNC__ ": [fPath] is empty!");
        return false;
    }

    // Read/Parse [fPath]:
    bool readOk = XmlIO_ParseFile(fPath, _xml_read_start, _xml_read_end);

    if (!_reachedEOF)
    {
		ShowMessage(__FUNC__ ": Parsing XML failed, closing root element not detected!");
        readOk = false;
    }

    // === 0.9.2-beta3: Convert/Import old FileFilterProperties Size, DateModified,
    //                  Age, Attributes to new FilterSets.
    if (readOk)
    {
        if (Build < 309)    // 309 = 0.9.2-beta3
            __importProfilePreviousToBuild309();
    }

    return readOk;
}
//---------------------------------------------------------------------------

void TAbaktGroup::__importProfilePreviousToBuild309()
{
    // This function is similar to the same-named function in the AbaktProfile unit.
    // The only difference is that only ONE filterSetList is used (customFilterSetList).

    abakt::TConfig::TFilePropertiesFilter* fpf = &PGroup->Config.FilePropertiesFilter;
    TAbaktFilterSet fs;

    // === Size - Import from pre 0.9.2-beta3 Profiles, convert to new FilterSet:
    fs.reset();
    if (fpf->Size->iSmaller->get() != 0)
    {
        fs.size     = "<" + IntToStr(fpf->Size->iSmaller->get());
    }
    if (!fs.size.IsEmpty())
    {
        fs.action   = fsaExclude;
        fs.enabled  = fpf->Size->iSmaller->override;
        fs.name     = "Imported Size < Filter: " + ecc::QuoteStr(fs.size);
        customFilterSetList.push_back(fs);
    }

    fs.reset();
    if (fpf->Size->iGreater->get() != 0)
    {
        fs.size     = fs.size + ">" + IntToStr(fpf->Size->iGreater->get());
    }
    if (!fs.size.IsEmpty())
    {
        fs.action   = fsaExclude;
        fs.enabled  = fpf->Size->iGreater->override;
        fs.name     = "Imported Size > Filter: " + ecc::QuoteStr(fs.size);
        customFilterSetList.push_back(fs);
    }

    // === Date - Import from pre 0.9.2-beta3 Profiles, convert to new FilterSet:
    fs.reset();
    fs.action   = fsaExclude;
    fs.enabled  = (fpf->DateModified->tDate1->override
                    && fpf->DateModified->bEnabled->override && fpf->DateModified->bEnabled->value);
    fs.date      = "<" + fpf->DateModified->tDate1->value;
    fs.name     = "Imported Date1 Filter: " + ecc::QuoteStr(fs.date);;
    customFilterSetList.push_back(fs);

    fs.reset();
    fs.action   = fsaExclude;
    fs.enabled  = (fpf->DateModified->tDate2->override
                    && fpf->DateModified->bEnabled->override && fpf->DateModified->bEnabled->value);
    fs.date      = ">" + fpf->DateModified->tDate2->value;
    fs.name     = "Imported Date2 Filter: " + ecc::QuoteStr(fs.date);;
    customFilterSetList.push_back(fs);

    // === Age - Import from pre 0.9.2-beta3 Profiles, convert to new FilterSet:
    // (Only include files with age between Age1 and Age2,
    // in other words: exclude files outside Age1 and Age2.)
    fs.reset();
    fs.action   = fsaExclude;
    fs.enabled  = (fpf->Age->iAge1->override
                    && fpf->Age->bEnabled->override && fpf->Age->bEnabled->value);
    fs.age      = "<" + IntToStr(fpf->Age->iAge1->get());
    fs.name     = "Imported Age1 Filter: " + ecc::QuoteStr(fs.age);
    customFilterSetList.push_back(fs);

    fs.reset();
    fs.action   = fsaExclude;
    fs.enabled  = (fpf->Age->iAge2->override
                    && fpf->Age->bEnabled->override && fpf->Age->bEnabled->value);
    fs.age      = "<" + IntToStr(fpf->Age->iAge2->get());
    fs.name     = "Imported Age2 Filter: " + ecc::QuoteStr(fs.age);
    customFilterSetList.push_back(fs);

/*     if (fpf->Age->bEnabled->value == true)
    {
        fs.action   = fsaExclude;
        fs.enabled  = true;
        fs.age      = "<" + IntToStr(fpf->Age->iAge1->get())
                        + "; >" + IntToStr(fpf->Age->iAge2->get());
        fs.name     = "Imported Age Filter: " + ecc::QuoteStr(fs.age);
        customFilterSetList.push_back(fs);
    } */

    // === Attributes - Import from pre 0.9.2-beta3 Profiles, convert to new FilterSet:
    fs.reset();
    // import OLD attribute H setting:
    if (fpf->Attributes->eMethodH->getIndex() == fpaExclude)
        fs.attributes = "==H";
    else if (fpf->Attributes->eMethodH->getIndex() == fpaOnlyInclude)
        fs.attributes = "!=H";
    fs.action   = fsaExclude;
    fs.enabled  = fpf->Attributes->eMethodH->override;
    fs.name     = "Imported H Filter: " + ecc::QuoteStr(fs.attributes);
    customFilterSetList.push_back(fs);

    // import OLD attribute R setting:
    fs.reset();
    if (fpf->Attributes->eMethodR->getIndex() == fpaExclude)
        fs.attributes = "==R";
    else if (fpf->Attributes->eMethodR->getIndex() == fpaOnlyInclude)
        fs.attributes = "!=R";
    fs.action   = fsaExclude;
    fs.enabled  = fpf->Attributes->eMethodR->override;
    fs.name     = "Imported R Filter: " + ecc::QuoteStr(fs.attributes);
    customFilterSetList.push_back(fs);

    // import OLD attribute S setting:
    if (fpf->Attributes->eMethodS->getIndex() == fpaExclude)
        fs.attributes = "==S";
    else if (fpf->Attributes->eMethodS->getIndex() == fpaOnlyInclude)
        fs.attributes = "!=S";
    fs.action   = fsaExclude;
    fs.enabled  = fpf->Attributes->eMethodS->override;
    fs.name     = "Imported S Filter: " + ecc::QuoteStr(fs.attributes);
    customFilterSetList.push_back(fs);

/*    if (!is.IsEmpty())
        fs.attributes = "==" + is + "; ";
    if (!isnot.IsEmpty())
        fs.attributes += "!=" + isnot + ";";
    fs.attributes = fs.attributes.Trim();

    // Add imported FilterSet (if iany) to exclude list:
     if (!fs.attributes.IsEmpty())
    {
        fs.action   = fsaExclude;
        fs.enabled  = true;
        fs.name     = "Imported Attribute Filter: " + ecc::QuoteStr(fs.attributes);
        customFilterSetList.push_back(fs);
    } */
}
//---------------------------------------------------------------------------

void TAbaktGroup::_xml_read_start(void *data, const char *el, const char **attr)
{
	String elm = String(el);
	String atr, val;

	// <abakt-group>:
	if (elm == ABT_GROUP_XML_ROOT_ELM)
	{
		PGroup->name		= xin::AttrValStr("name", attr);
		PGroup->Version		= xin::AttrValStr("version", attr);
		PGroup->Build		= xin::AttrValInt("build", attr);
		return;
	}

	// <profiles>
	if (elm == ELM_profiles)
	{
		__ELMid = ELMid_profiles;
		return;
	}
	// <profile>
	if (__ELMid == ELMid_profiles)
        if (elm == "profile")
        {
            bool sel = false;
            String fname;
            for (int i = 0; attr[i]; i += 2)
            {
                atr = String(attr[i]);
                val = String(attr[i + 1]);
                if (atr == "selected")
                    sel = ecc::StrToBool(val);
                if (atr == "filename")
                    fname = Utf8ToAnsi(val);
            }
            // Both sel and fname should have a value at this point:
            ecc::TCheckStringListItem item;
            item.checked = sel;
            item.str = fname;
            PGroup->profileList.push_back(item);
            return;
        }

    // <FileFilters>: added in 0.9.2-beta3
	if (elm == ELM_FileFilters)
	{
		__ELMid = ELMid_FileFilters;
		return;
	}
	if (__ELMid == ELMid_FileFilters)
	{
		xin::readSect_FileFilters_Filter_intoFSList(elm, attr, PGroup->customFilterSetList);
		return;
	}

    // <FilePropertiesFilter>: (used to import pre-0.9.2-beta3 groups)
	if (elm == ELM_FilePropertiesFilter)
	{
		__ELMid = ELMid_FilePropertiesFilter;
		return;
	}
	if (__ELMid == ELMid_FilePropertiesFilter)
	{
		PGroup->_readSect_FilePropertiesFilter(elm, attr);
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
		PGroup->_readSect_ActionTypes(elm, attr);
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
		PGroup->_readSect_Action(elm, attr);
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
		PGroup->_readSect_Destination(elm, attr);
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
		PGroup->_readSect_History(elm, attr);
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
		PGroup->_readSect_FileList(elm, attr);
		return;
	}
}
//---------------------------------------------------------------------------

void TAbaktGroup::_xml_read_end(void *data, const char *el)
{
	String elm = String(el);

	if (elm == ELM_profiles)
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_FileFilters)
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_FilePropertiesFilter)
    // -- Required as long as importing pre 0.9.2-beta3.309 groups is supported.
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

	// Detect closing root element </abakt-group>:
	if (elm == ABT_GROUP_XML_ROOT_ELM)
    {
        PGroup->_reachedEOF = true;
    }
}
//---------------------------------------------------------------------------

void TAbaktGroup::_readSect_FilePropertiesFilter(String elm, const char **attr)
{
	// <Attributes>:
	if (elm == ELM_Fpf_Attributes)
	{
		__ELMid2 = ELMid_Fpf_Attributes;
		return;
	}
	if (__ELMid2 == ELMid_Fpf_Attributes)
	{
		xin::itemAttrsToList(attr, Config.FilePropertiesFilter.Attributes->items);
	}

	// <Size>:
	if (elm == ELM_Fpf_Size)
	{
		__ELMid2 = ELMid_Fpf_Size;
		return;
	}
	if (__ELMid2 == ELMid_Fpf_Size)
	{
		xin::itemAttrsToList(attr, Config.FilePropertiesFilter.Size->items);
	}

	// <DateModified>:
	if (elm == ELM_Fpf_DateModified)
	{
		__ELMid2 = ELMid_Fpf_DateModified;
		return;
	}
	if (__ELMid2 == ELMid_Fpf_DateModified)
	{
		xin::itemAttrsToList(attr, Config.FilePropertiesFilter.DateModified->items);
	}

	// <Age> (250+):
	if (elm == ELM_Fpf_Age)
	{
		__ELMid2 = ELMid_Fpf_Age;
		return;
	}
	if (__ELMid2 == ELMid_Fpf_Age)
	{
		xin::itemAttrsToList(attr, Config.FilePropertiesFilter.Age->items);
	}
}
//---------------------------------------------------------------------------

void TAbaktGroup::_readSect_Action(String elm, const char **attr)
{
	xin::itemAttrsToList(attr, Config.Action->items);
}
//---------------------------------------------------------------------------

void TAbaktGroup::_readSect_ActionTypes(String elm, const char **attr)
{
	// <Zip>:
	if (elm == ELM_Act_Zip)
	{
		__ELMid2 = ELMid_Act_Zip;
		return;
	}
	if (__ELMid2 == ELMid_Act_Zip)
	{
		xin::itemAttrsToList(attr, Config.Actions.Zip->items);
	}

	// <SevenZip>:
	if (elm == ELM_Act_7Zip)
	{
		__ELMid2 = ELMid_Act_7Zip;
		return;
	}
	if (__ELMid2 == ELMid_Act_7Zip)
	{
		xin::itemAttrsToList(attr, Config.Actions.SevenZip->items);
	}

	// <File>:
	if (elm == ELM_Act_File)
	{
		__ELMid2 = ELMid_Act_File;
		return;
	}
	if (__ELMid2 == ELMid_Act_File)
	{
		xin::itemAttrsToList(attr, Config.Actions.File->items);
	}
}
//---------------------------------------------------------------------------

void TAbaktGroup::_readSect_Destination(String elm, const char **attr)
{
   	xin::itemAttrsToList(attr, Config.Destination->items);
}
//---------------------------------------------------------------------------

void TAbaktGroup::_readSect_History(String elm, const char **attr)
{
	xin::itemAttrsToList(attr, Config.History->items);
}
//---------------------------------------------------------------------------

void TAbaktGroup::_readSect_FileList(String elm, const char **attr)
{
	xin::itemAttrsToList(attr, Config.FileList->items);
}
//---------------------------------------------------------------------------

