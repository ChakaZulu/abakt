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
#ifndef AbaktHistoryH
#define AbaktHistoryH
//---------------------------------------------------------------------------
#include "AbaktProfile.h"
#include "AbaktClasses.h"
#include "eccString.h"
//---------------------------------------------------------------------------

typedef struct {
	String fileName;
	int age;    // Age of fileName in days. If unknown [age] is -1.
    TArchiveMode archiveMode;   // Is -1 if unknown (pre 0.9.2-alpha6.305).
} TAbaktHistoryItem;
//---------------------------------------------------------------------------

class TAbaktHistory {

public:

	TAbaktHistory(TAbaktProfile& profile, TListBox* logListBox = NULL);
	/*	Informs this object about the [profile] to use, and sets _fileName.
		Use [logListBox] to display status messages, can be NULL.
		Calls _read() to read data stored in _fileName into _items. */

	bool getAgeOfFiles();
	/*	Sets the current age of every file in _items.
		If a file does not exist age is set to -1.
		Calls sortByAge().
		Returns false on failure.
		Note: Updated data is note saved. */

	bool sortByAge();
	/* 	Sorts _items by age, oldest files first.
		Returns false on failure.
		Note: Updated data is note saved. */

	bool update();
	/*  Calls getAgeOfFiles().
		Removes non-existent backup-files from _items.
		Returns false on failure.
		Note: Updated data is note saved. */

	bool add(const String fileName, const TArchiveMode archiveMode);
	/* 	Adds [fileName] (the filename of a backup) and [archiveMode] to _items.
		Returns false on failure.
		Note: Updated data is not saved. */

	bool execute();
	/*	Calls update().
		Tests if backup-files should be deleted and:
		- removes backup-files from disk accordingly,
		- removes succesfully deleted backup-filenames from the history list.
		Calls write() to write _items to _fileName.
		Returns false on failure. */

	bool execute(const String fileName, const TArchiveMode archiveMode);
	/* 	Calls add([fileName], [archiveMode]).
		Calls execute().
        Sets _latestArchive with [fileName] and [archiveMode].
		Returns false on failure. */

	bool write();
	/*	Writes _items and _latestArchive to _fileName.
		Returns false on failure. */

    TAbaktHistoryItem getLatestArchive()
    {   return _latestArchive; }

	int indexOf(const String fileName) const;
	/* 	Returns index of [fileName] in _items.
		Returns -1 if no match found. */

	unsigned int getSize() const
    {   return _items.size(); }

	String getFileName(const unsigned int index) const;
	/* 	Returns the fileName of item [index] in _items. */

	int getAge(const unsigned int index) const;
	/* 	Returns the age (in days) of item [index] in _items.
        Returns -1 if age is unknown (file does not exist). */

   	TArchiveMode getArchiveMode(const unsigned int index) const;
	/* 	Returns the TArchiveMode of item [index] in _items.
        Returns -1 if TArchiveMode is unknown (pre 305). */

	void log(const String msg,
            const int lines_before = 0, const int lines_after = 0) const;
    /*  Writes [msg] to _logListBox. */

	void log(const String msg, const ecc::TLBMessageStyle style,
			const int lines_before = 0, const int lines_after = 0) const;
    /*  Writes [msg] to _logListBox using [style]. */

private:

	TAbaktProfile* _profile;

	TListBox* _logListBox;
	/*	The TListBox used to display messages. Can be NULL. */

	String _fileName;
	/*	The XML-formatted history file (.abh) associated with _profile.
		If empty, nothing will be written to the history file. */

    TAbaktHistoryItem _latestArchive;
    /*  Contains the information of latest successful archive that was created.
        Values are set in function execute(fileName, archiveMode). */

	// -- Settings obtained from _profile by constructor:
	unsigned int _maxDays;
	unsigned int _minBackups;
	unsigned int _maxBackups;
	bool _useRecycleBin;

	std::vector<TAbaktHistoryItem> _items;

	bool _read();
	/*	Clears _items, then reads _fileName into _items.
        Reads _latestArchive.
		Calls getAgeOfFiles().
		Returns false on failure. */

	static void _xml_read_start(void *data, const char *el, const char **attr);
	/*	Parse xml start-elements, like "<element attr='val'>". */

	static void _xml_read_end(void *data, const char *el);
	/*	Parse xml end-elements, like "</element>". */

    void _removeBackup(String fileName);
    /*  Removes [fileName] and associated fileName.Z## files from disk.
        If _useRecycleBin is true, files will be moved to the Recycle Bin. */
};
//---------------------------------------------------------------------------
#endif
