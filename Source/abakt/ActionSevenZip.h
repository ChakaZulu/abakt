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
#ifndef ActionSevenZipH
#define ActionSevenZipH
//---------------------------------------------------------------------------
/*
	Usage: See f_Action::_doActionSevenZip().
*/
//---------------------------------------------------------------------------

class TActionSevenZip {

public:

	TActionSevenZip()
	{
		_inited = false;
	}

	bool init();
	/*  Initializes action. Should be called before calling execute().
        Returns false if requirements are not met. */

	bool execute();
	/* 	Performs Action according to the settings in PProfile.
		Returns false if _error or _aborted are set. */

    void abort()
    {   _aborted = true; }

	String getDestination()
	{
		if (!_inited) return "";
		else return _destFileName;
	}

private:
	bool _error;
	bool _aborted;
	bool _inited;

	String _fileListFile;
	/*	Path to the file that contains the temporary filelist. */

	String _destFileName;
	/*	Contains the path to the destination _without_ the extension.
		The extension is added by the 7-Zip executable. */

    String _commandLine;
    /*   Command line with which 7z.exe will be called. */

    String _commandLineGui;
    /*   Command line that will be used on-screen (passwork is masked). */

    String _build7zCommandLine(const bool maskPassword);
    /*  Returns the result of the CommandLineTemplate,
        based on the current Profile.Config (!) settings.
        - Requires PProfile.Config.Actions.SevenZip to be up-to-date.
        - Requires PProfile.Config.Destination(.AppendExtension) to be up-to-date.
        If [maskPassword] is true, the password characters will be replaced by '*'.
        Note: this function sets _destFileName, _commandLine and _commandLineGui:
        used throughout the action. */

	void _call7ZipExe(String drive);
	/* 	Calls 7-Zip executable for [drive].
		Adds the files in _fileListFile to _destFileName (without extension).
		Does nothing if _error or _aborted is set. */

    void _prepAndLaunchRedirectedChild(String drive,
            HANDLE hChildStdOut, HANDLE hChildStdErr);
    /*  Sets up STARTUPINFO structure, and launches redirected child. */

    void _handleError(String msg);
    /*  Displays [msg], the error number and corresponding message. */

	void _deleteFileListFile();
	/*	Deletes the temporary filelist (_fileListFile).
		Displays a warning if the could not be deleted (only if the file exists). */

    String _getDestDir();
    /*  Returns Destination's (formatted) destination directory. */

};
//---------------------------------------------------------------------------
#endif

