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
#ifndef AbaktGroupH
#define AbaktGroupH
//---------------------------------------------------------------------------
#include "eccCheckStringList.h"
#include "AbaktConfigSets.h"
#include "AbaktFilterSet.h"
//---------------------------------------------------------------------------

/*  Note on expat's XML_Char:
    Since Abakt saves xml data using UTF8, expat is expected to have been compiled
    with XML_Char defined as char. */

//---------------------------------------------------------------------------

class TAbaktGroup {

public:
	TAbaktGroup();

	void reset();

	bool write(String fPath);
    /*  Writes a Group (.abg file) to file [fPath].
        Returns 'false' on failure. */

	bool read(const String fpath);
    /*  Reads a Group (.abg file) from file [fPath].
        Returns 'false' on failure. */

	String fileName;

	bool modified;

	String	name;
	String	Version;
	int		Build;

	std::vector<ecc::TCheckStringListItem> profileList;

    void __importProfilePreviousToBuild309();
    /*  This function converts pre-0.9.2-beta3 filter settings to new FilterSets.
        (___MaskLists and Config.FilePropertiesFilter) */

    // == NEW (0.9.2-beta3): Replaces Config.FilePropertieFilter values:
    std::vector<TAbaktFilterSet> customFilterSetList;

	abakt::TConfig Config;

private:

	// == XML Read (parse) members:

    bool _reachedEOF;
    /*  Used to detect if </abakt-profile> has been reached. If not, there was
        probably an error while parsing the XML file. */

	static void _xml_read_start(void *data, const char *el, const char **attr);
	static void _xml_read_end(void *data, const char *el);

    void _readSect_FilePropertiesFilter(String elm, const char **attr);
	void _readSect_Action(String elm, const char **attr);
	void _readSect_ActionTypes(String elm, const char **attr);
	void _readSect_Destination(String elm, const char **attr);
	void _readSect_History(String elm, const char **attr);
	void _readSect_FileList(String elm, const char **attr);
};
//---------------------------------------------------------------------------
extern TAbaktGroup* PGroup;	// Defined in AbakGroup.cpp. Created in Abakt.cpp.
//---------------------------------------------------------------------------
#endif

