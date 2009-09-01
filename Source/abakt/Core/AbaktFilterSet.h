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
#ifndef AbaktFilterSetH
#define AbaktFilterSetH
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------
/*  The class TAbaktFilterSet simply stores information.
    It does not perform any match (test) operations, see AbaktFilter.cpp for that.

    This file contains some global usage functions for FilterSet
    and FilterSetList (std::vector<TAbaktFilterSet> *list) operations.
*/
//---------------------------------------------------------------------------

typedef enum {
    fsaExclude = 0,
    fsaInclude,
    fsaNoCompression
} TAbaktFilterSetAction;
//---------------------------------------------------------------------------

class TAbaktFilterSet {

public:

    TAbaktFilterSet();
    /*  Calls reset(). */

    // === Properties:
    String name;                    // A user defined descriptive name for this FilterSet.
    TAbaktFilterSetAction action;   // Either 'fsaExclude', 'fsaInclude' or 'fsaNoCompress'.
    bool enabled;                   // Whether this FilterSet is active or not.

    // Conditions (made up of expressions):
    String path;    // This is a mask (Abakt-style). If empty, every file matches (same as '*').
    String size;
    String date;
    String age;
    String attributes;


    // === Methods:

    void reset();
    /*  Clears all properties.
        Except for 'name', which will be set to "(New FilterSet)" (translatable). */


    // === Operators:

    // operator '=' (assignment):
//    TAbaktFilterSet& operator = (const TAbaktFilterSet& fs);

};
//---------------------------------------------------------------------------

// == Globally available FilterSet/List manipulation functions ==

/* TODO 2 : Make global FilterSet functions methods of TAbaktFilterSet?
    Or create a new TAbaktFilterSetList class, based on std::vector? */


String FilterSetToXmlStr(const TAbaktFilterSet& fs);
/*  Converts [fs] to an xml element string used in Abakt's data files.
    The resulting string will be formatted like this:
        "<Filter name='myDescriptiveName' enabled='true' action='exclude'
            path='' size='' date='' age='' attributes=''/>". */

String FilterSetActionToStr(const TAbaktFilterSetAction fsa);
/*  Converts [fsa] to a string that is used in the xml files.
    Default return value is "exclude". */

TAbaktFilterSetAction StrToFilterSetAction(const String str);
/*  Converts a string to a TFilterSetAction value.
    Default return value is fsaExclude. */

bool CompareFilterSetConditions(const TAbaktFilterSet& fs1,
        const TAbaktFilterSet& fs2);
/*  Returns true if all conditions of [fs1] and [fs2] are equal,
    returns false otherwise.
    Note: the properties name, action and enabled are not compared. */

TAbaktFilterSet* SimilarFilterSetExistsInList(const TAbaktFilterSet& fs,
        std::vector<TAbaktFilterSet> *fsList);
/*  Returns a pointer to the item in [fsList] that has conditions equal to [fs].
    Returns NULL if no 'existing' item is found.
    Note: Uses CompareFilterSetConditions() to compare items. */

void MergeFilterSetLists(std::vector<TAbaktFilterSet> *sourceList,
        std::vector<TAbaktFilterSet> *destList);
/*  Appends all 'non-existing' items in [sourceList] to [destList].
    Whenever an item in [sourceList] is equal to an item in [destList] ('exists'),
    the properties name, action and enable of [sourceList]'s item are used to
    modify the 'existing' item in [destList].
    Note: Uses FilterSetExistsInList() to compare items. */

void AddFilterSetToList_SimilarAware(const TAbaktFilterSet& fs,
        std::vector<TAbaktFilterSet> *fsList);
/*  Adds [fs] to [fsList], unless a FilterSet 'similar' to [fs] is present in [fsList].
    If a FilterSet similar to [fs] is found in [fsList],
    the 'properties' (name/action/enabled) of the 'existing' FilterSet are replaced
    with the properties of [fs].
    == In other words ==
    This function tests if a similar FilterSet exists:
    -   If not, the FilterSet [fs] will be added to the [fsList].
    -   If so, the existing FilterSet properties (name/action/enabled) in [fsList]
        will be modified with [fs]'s properties.
        The existing conditions will be left untouched. */

//---------------------------------------------------------------------------
#endif
