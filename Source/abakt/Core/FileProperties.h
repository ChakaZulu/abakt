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
#ifndef FilePropertiesH
#define FilePropertiesH
//---------------------------------------------------------------------------

class TFileProperties {

public:

	TFileProperties();
    /*  Default constructor.
        A TFileProperties object should not be used instantly,
            you should call '[TFileProperties].retrieve(fileName)' first.
        Or (better), create an instance of this class
            using the constructor TFileProperties(String fileName),
            and then call failed() or isInitialized() to find out
            if retrieving the file's properties succeeded. */

	TFileProperties(String fileName);
    /*  See default constructor. */

/*     void __testPFINDFIRSTFILEW(); */
    /*  Tests if FindFirstFileW exists on system.
        Should be called by every constructor. */

	bool retrieve(String fileName);
    /*  If this function returns false you should not trust the values
        returned by the get<file-property>() functions. */

	bool failed()
    {   return _error; }

  	String getErrorStr()
    {   return _errorStr; }

	bool isInitialized()
    {   return _initialized; }

    bool getAttrState(const char& attr);
    /*  Returns true if [attr] is in {A,C,H,R,S,T,D}. Case-sensitive. */

    String getAttrsStr();
    /*  Does not return 'D' if file is a directory. Use isDir() instead. */

	bool isDir()                { return _attrs.Directory; }
    /*  Returns 'true' if file is a directory, false otherwise. */

	__int64 getSize();
    /*  Returns the filesize (in Bytes).
        Returns -1 if _initialized is false. */

	TDateTime getDate();
    /*  Returns file's LastWriteTime (Date Modified) as a date (without time information).
        Returns NULL if _initialized is false. */

    TDateTime getDateTime();
    /*  Returns file's LastWriteTime (Date Modified) including time information.
        Returns NULL if _initialized is false. */

    int getAge();
    /*  Returns the file's age (in Days).
        See _fetchDateTime() and _lastWriteTime.
        Returns -1 if _initialized is false. */

private:

	bool _initialized;
    /*  Is false if class's properties contain no valid data. */

	bool _error;

	String _errorStr;

    struct TFileAttributes {
        bool Archive    : 1;
        bool Compressed : 1;    // Not used in Abakt.
        bool Hidden     : 1;
        bool ReadOnly   : 1;
        bool System     : 1;
        bool Temporary  : 1;    // Not used in Abakt.
        bool Directory  : 1;
    } _attrs;

	__int64 _size;              // File size (bytes).

	TDateTime _lastWriteTime;	// File modification date.

    int _age;                   // File age (days).

	void _fetchAttributes(const WIN32_FIND_DATA &fi);

	void _fetchSize(const WIN32_FIND_DATA &fi);

	void _fetchDateTime(const WIN32_FIND_DATA &fi);
    /*  Sets _lastWriteTime and calculates _age. */
};
//---------------------------------------------------------------------------
#endif
