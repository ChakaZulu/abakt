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

#include "AbaktGlobals.h"
#include "eccMisc.h"
#include "eccString.h"
#include "eccFileVersionInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/* TODO 1 : == Update RELEASE_STR (and Build number) with every release. */
// NOTE:    For the -RC# and stable releases ABT_TEST should be 0.
#define RELEASE_STR ""   // "-alpha1", "-beta2", "-RC3" or "" (==stable release).
//---------------------------------------------------------------------------
TAbaktGlobals* PGlobals;        // Object is created in Abakt.cpp
//---------------------------------------------------------------------------

TAbaktGlobals::TAbaktGlobals()
{
	// Settings modifiable by Preferences Window:

	// - OnStartup:
	InitialProfileType		= 0;
	CustomInitialProfile 	= "";
	CreateFileAssociations	= true;
	// - Application:
	UserDataDir				= getDefaultUserDataDir();
	ShowDescriptionBoxes 	= true;
    LocaleId                = "en";
	// - Action:
	DefaultDestDir			= "";
	_tempDir     			= "";
	ExePath7Zip				= "";
    OEMlistfile7Zip         = true;
	ActFile_ConfirmDelete 	= true;
	// - Logging:
	LogBuildingFileLists	= false;
	LogFullAction			= false;
	AutoSaveActionLog		= false;
	ActionLogDir			= getDefaultUserDataDir() + "Log\\";
    SaveAppStatus           = true; 
    AppStatusMaxSize        = 2048; // Default (kB units).
	// - Advanced:
	RemoveNonExistingSourceDirs = true;
	UseNewUIOnBFD			= true;
//removed 095b1:    IncludeEmptyDirs		= false;
    CreateTempZipCopy       = true;
    UseZipBuilderAddSafe    = true; 
    TreatWarningsAsErrors   = false;

	// Commandline parameters:
	cmdLine_ParametersPresent = false;
	cmdLine_AutoExecute		= false;
	cmdLine_Minimize		= false;
	cmdLine_AutoExit		= false;
	cmdLine_OVRDestFormatText = false;
	cmdLine_CustomDestFormatText = "";

    // == Set default location of ini-file, can be overridden with cmdline option -I:
    // .. Since 0.9.2-beta5 (b.311): First try to find Abakt.ini in Abakt.exe's directory,
    // .. if it is not found there, use the directory provided by getDefaultUserDataDir().
    // .. [Using Abakt.ini in Abakt.exe's directory as the preferred ini-file was suggested by Stefan in nov-2005].
    if (FileExists(ecc::AppPath() + "Abakt.ini"))
       	_iniFilePath 	= ecc::AppPath() + "Abakt.ini";
    else
       	_iniFilePath 	= getDefaultUserDataDir() + "Abakt.ini";

	// Directories assumed to be available while application runs:
	_helpFile   	    = ecc::AppPath() + "Help\\Abakt.chm"; // Default helpfile (English).

	// Other:
	_retrieveAppVersion();
}
//---------------------------------------------------------------------------

String TAbaktGlobals::getDefaultUserDataDir()
{
	return ecc::GetSystemFolder(ecc::sfAppData) + "Abakt\\";
}
//---------------------------------------------------------------------------

void TAbaktGlobals::setIniFilePath(String iniFilePath)
{
    if (!iniFilePath.Pos("\\"))
        iniFilePath = IncludeTrailingPathDelimiter(GetCurrentDir()) + iniFilePath;
    _iniFilePath = iniFilePath;
}
//---------------------------------------------------------------------------

String TAbaktGlobals::getIniFilePath()
{
    return _iniFilePath;
}
//---------------------------------------------------------------------------

bool TAbaktGlobals::_retrieveAppVersion()
{
	ecc::TFileVersionInfo fvi(Application->ExeName);
	if (!fvi.Failed())
	{
		AppVersionStr	= fvi.GetVersion();
		AppBuildStr  	= fvi.GetBuild();
		return true;
	}
	else
	{
		AppVersionStr	= fvi.GetErrorMsg();
		AppBuildStr  	= "";
		return false;
	}
}
//---------------------------------------------------------------------------

String TAbaktGlobals::humanVersionStr()
{
	return AppVersionStr
		+ RELEASE_STR
		#ifdef ABT_TEST
    		+ " (" + _("Build") + " " + AppBuildStr + ")"
		#endif

		#ifdef _DEBUG
			+ " +DEBUG"
		#endif

		#ifdef __CODEGUARD__
			+ " +CODEGUARD"
		#endif
	;
}
//---------------------------------------------------------------------------

String TAbaktGlobals::versionBuildStr()
{
	return AppVersionStr + "." + AppBuildStr;
}
//---------------------------------------------------------------------------

void TAbaktGlobals::setTempDir(String dir)
{
	_tempDir = IncludeTrailingPathDelimiter(dir);
}
//---------------------------------------------------------------------------

String TAbaktGlobals::getTempDir()
{
	if (_tempDir.IsEmpty())
	{
		_tempDir = ecc::GetSystemFolder(ecc::sfTemp);
	}

	if (!DirectoryExists(_tempDir))
	{
		if (!ForceDirectories(_tempDir))
		{
			ShowMessage(__FUNC__ "\n" + _("Could not create Temporary Directory:")
                + "\n" + ecc::QuoteStr(_tempDir) );
			_tempDir = "";
		}
	}

	return _tempDir;
}
//---------------------------------------------------------------------------

