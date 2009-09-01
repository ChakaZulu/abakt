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

#include "AbaktFilterSet.h"
#include "AbaktClasses.h"
#include "eccString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

TAbaktFilterSet::TAbaktFilterSet()
{
    reset();
}
//---------------------------------------------------------------------------

void TAbaktFilterSet::reset()
{
    name    = _("(New FilterSet)");
    action  = fsaExclude;
    enabled = true;

    path        = "";
    size        = "";
    date        = "";
    age         = "";
    attributes  = "";
}
//---------------------------------------------------------------------------

/* TAbaktFilterSet& TAbaktFilterSet::operator = (const TAbaktFilterSet& fs)
{
    if (this == &fs) return *this;

    this->name       = fs.name;
    this->action     = fs.action;
    this->enabled    = fs.enabled;
    this->path       = fs.path;
    this->size       = fs.size;
    this->date       = fs.date;
    this->age        = fs.age;
    this->attributes = fs.attributes;

    return *this;
}; */
//---------------------------------------------------------------------------

String FilterSetToXmlStr(const TAbaktFilterSet& fs)
{
    String xml = "<" + String(ELM_FF_Filter) +
        // Properties:
        " name='" + ecc::StrToXmlUtf8Str(fs.name) + "'"
        " enabled='" + ecc::BoolToStr(fs.enabled) + "'"
        " action='" + FilterSetActionToStr(fs.action) + "'"
        // Conditions:
        " path='" + ecc::StrToXmlUtf8Str(fs.path) + "'"
        " size='" + ecc::StrToXmlUtf8Str(fs.size) + "'"
        " date='" + ecc::StrToXmlUtf8Str(fs.date) + "'"
        " age='" + ecc::StrToXmlUtf8Str(fs.age) + "'"
        " attributes='" + ecc::StrToXmlUtf8Str(fs.attributes) + "'"
        "/>";

    return xml;
}
//---------------------------------------------------------------------------

String FilterSetActionToStr(const TAbaktFilterSetAction fsa)
{
    //  The returned strings should not be translated
    //  because they are used in English, XML-formatted Profiles.

    switch (fsa)
    {
        default:
        case fsaExclude:
            return "exclude";   // Default.

        case fsaInclude:
            return "include";

        case fsaNoCompression:
            return "noCompression";
    }
}
//---------------------------------------------------------------------------

TAbaktFilterSetAction StrToFilterSetAction(const String str)
{
    if (str == FilterSetActionToStr(fsaInclude))
        return fsaInclude;
    if (str == FilterSetActionToStr(fsaNoCompression))
        return fsaNoCompression;
    return fsaExclude;    // Default.
}
//---------------------------------------------------------------------------

bool CompareFilterSetConditions(const TAbaktFilterSet& fs1,
        const TAbaktFilterSet& fs2)
{
    if (fs1.path != fs2.path) return false;
    if (fs1.size != fs2.size) return false;
    if (fs1.date != fs2.date) return false;
    if (fs1.age != fs2.age) return false;
    if (fs1.attributes != fs2.attributes) return false;
    return true;
}
//---------------------------------------------------------------------------

TAbaktFilterSet* SimilarFilterSetExistsInList(const TAbaktFilterSet& fs,
        std::vector<TAbaktFilterSet> *fsList)
{
    for (unsigned int i = 0; i < fsList->size(); i++)
    {
        if (CompareFilterSetConditions(fs, fsList->at(i)))
            return &fsList->at(i);
    }
    return NULL;
}
//---------------------------------------------------------------------------

void MergeFilterSetLists(std::vector<TAbaktFilterSet> *sourceList,
        std::vector<TAbaktFilterSet> *destList)
{
    for (unsigned int i = 0; i < sourceList->size(); i++)
    {
        TAbaktFilterSet* fs = SimilarFilterSetExistsInList(sourceList->at(i), destList);
        if (fs == NULL)
        {
            destList->push_back(sourceList->at(i));
        }
        else // fs contains a pointer to an existing TAbaktFilterSet in destList.
        {
            fs->name    = sourceList->at(i).name;
            fs->action  = sourceList->at(i).action;
            fs->enabled = sourceList->at(i).enabled;
        }
    }
}
//---------------------------------------------------------------------------

void AddFilterSetToList_SimilarAware(const TAbaktFilterSet& fs,
        std::vector<TAbaktFilterSet> *fsList)
{
    TAbaktFilterSet* fsSimilar = SimilarFilterSetExistsInList(fs, fsList);
    if (fsSimilar == NULL)
    {
        fsList->push_back(fs);
    }
    else
    {   // Modify existing FilterSet's properties (and leave conditions untouched):
        fsSimilar->name    = fs.name;
        fsSimilar->action  = fs.action;
        fsSimilar->enabled = fs.enabled;
    }
}
//---------------------------------------------------------------------------

