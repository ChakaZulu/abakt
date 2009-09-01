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
#ifndef AbaktProfileH
#define AbaktProfileH
//---------------------------------------------------------------------------
#include "AbaktClasses.h"
#include "AbaktConfigSets.h"
#include "AbaktFilterSet.h"
#include <fstream>
#include "eccCheckStringList.h"
//---------------------------------------------------------------------------
/*  Note on expat's XML_Char:
    Since Abakt saves xml data using UTF8, expat is expected to have been compiled
    with XML_Char defined as char. */
//---------------------------------------------------------------------------

class TAbaktProfile {

public:
	TAbaktProfile();

	String name;        // Descriptive name of the profile (this is actually the filename minus path and extension information).
	String versionStr;  // Version of profile on disk.
	String buildStr;    // Build of profile on disk.
	String fileName;    // Is empty for a new Profile (Name appears as "Untitled").

	bool modified;
    /*  Whenever a modification has been made to an unsaved Profile [modified] will be true. */

 	String getErrorStr() const    { return _errorStr; }
    /*  Returns a string that contains information on the error. */

	std::vector<String> sourceRootDirList;
	/*	List of all source's root directories.
		Every entry contains a trailing backslash. */

	std::vector<String> excludeDirList;
	/*	List of single directories to exclude.
		Every entry contains a trailing backslash. */

	std::vector<String> includeDirList;
	/*	List of single directories to include.
		Every entry contains a trailing backslash. */

	std::vector<String> excludeTreeList;
	/*	List of directory-trees to exclude (tree = this directory and its sub-directories).
		Every entry contains a trailing backslash. */

	std::vector<String> includeTreeList;
	/*	List of directory-trees to include (tree = this directory and its sub-directories).
		Every entry contains a trailing backslash. */

    // == NEW (0.9.2-beta3): Replaces ___???MaskLists and Config.FilePropertiesFilter:
    std::vector<TAbaktFilterSet> filterSetList;

	abakt::TConfig config;

	void reset();
	/* 	Resets Profile properties to default values.
		You should call ProfileToGui.All() after calling this function.
		This function is also called by read(). */

	bool write(String fPath);
    /*  Writes a Profile (.abp file) to file [fPath].
        Returns 'false' on failure. */

	bool read(const String fPath);
    /*  Reads a Profile (.abp file) from file [fPath].
        Returns 'false' on failure. */


	// == Global-usage functions:

	bool dirInExcludeDirList(String dir); // not: const String dir for backwards compat.
	/*	[dir] is assumed to include trailing backslash. */

	bool dirInIncludeDirList(String dir); // not: const String dir for backwards compat.
	/*	[dir] is assumed to include trailing backslash. */

    bool dirInExcludeTreeList(const String dir);
	/*	[dir] is assumed to include trailing backslash. */

	bool dirIsPartOfExcludeTree(const String dir);

	bool dirIsPartOfIncludeTree(const String dir);

private:

    String _errorStr;

    void __importProfilePreviousToBuild309();
    /*  This function converts pre-0.9.2-beta3 filter settings to new FilterSets.
        (___MaskLists and Config.FilePropertiesFilter) */

    // 0.9.2-beta3: __???MaskLists are used to import old Profiles and should not be used for other purposes:
	std::vector<ecc::TCheckStringListItem> __ExcludeMaskList;
	std::vector<ecc::TCheckStringListItem> __IncludeMaskList;
	std::vector<ecc::TCheckStringListItem> __NoCompressMaskList;


	// == XML Read(parse) members:

    bool _reachedEOF;
    /*  Used to detect if </abakt-profile> has been reached. If not, there was
        probably an error while parsing the XML file. */

	static void _xml_read_start(void *data, const char *el, const char **attr);
	/*	Parse xml start-elements, such as: "<element attr='val'>". */

	static void _xml_read_end(void *data, const char *el);
	/*	Parse xml end-elements, such as "</element>". */


	// == XML Read (parse) and Write members:

	void _writeSect_source(std::ofstream& ofs);
	void _readSect_source(String elm, const char **attr);
	void _readSect_source_root(const char **attr);
	void _readSect_source_root_exclude(const char **attr);
	void _readSect_source_root_include(const char **attr);

    void _writeSect_FileFilters(std::ofstream& ofs);        // new in 0.9.2-beta3

	void _readSect_filters(String elm, const char **attr);  // required to import pre 0.9.2-beta3.309 profiles.
	void _readSect_filters_masklist(const char **attr,      // required to import pre 0.9.2-beta3.309 profiles.
            std::vector<ecc::TCheckStringListItem>& list, TAbaktFilterSetAction fsAction);
    void _readSect_FilePropertiesFilter(String elm, const char **attr); // required to import pre 0.9.2-beta3.309 profiles.

	void _readSect_ActionTypes(String elm, const char **attr);
	void _readSect_Action(String elm, const char **attr);
	void _readSect_Destination(String elm, const char **attr);
	void _readSect_History(String elm, const char **attr);
	void _readSect_FileList(String elm, const char **attr);

    // == Helper functions:

	bool _strInCheckStringListNoCase(String str, std::vector<ecc::TCheckStringListItem>& list);
    /*  Returns true if [str] is present in [list], case insensitive. */
};
//---------------------------------------------------------------------------
extern TAbaktProfile* PProfile;	// Defined in AbakProfile.cpp. Created in Abakt.cpp.
//---------------------------------------------------------------------------
#endif

