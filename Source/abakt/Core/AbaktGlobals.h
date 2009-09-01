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
#ifndef AbaktGlobalsH
#define AbaktGlobalsH
//---------------------------------------------------------------------------

static const String INI_SECT_RECENT_GROUPS		= "RecentGroups";
static const String INI_SECT_RECENT_PROFILES	= "RecentProfiles";
static const String INI_SECT_RECENT_TEMPLATES	= "RecentTemplates";
//---------------------------------------------------------------------------

class TAbaktGlobals {
/*	TAbaktGlobals is a class that stores some of Abakt's global preferences and settings.
	Mostly used by the Core modules.
	Should be initialized by calling GuiToGlobals.all() when Abakt starts. */

public:

	TAbaktGlobals();

	String AppVersionStr;
	String AppBuildStr;
	String ZipBuilderVersionStr; // Set in f_Action's constructor.

	String humanVersionStr();
	/*	Returns a version string including Build, TEST, DEBUG and CODEGUARD info. */

    String versionBuildStr();
	/*	Returns a version string formatted like "0.9.0.123". */

	// === Settings modifiable by Preferences Window:

	// - OnStartup:
	int InitialProfileType;
	String CustomInitialProfile;
	bool CreateFileAssociations;
	// - Application:
	String UserDataDir; 		// User's Abakt data directory.
	bool ShowDescriptionBoxes;
    String LocaleId;            // Should be set in Abakt.cpp, nowhere else!
    // RecentFiles settings are maintained in f_Main.
	// - Action:
	String DefaultDestDir;	    // Contains trailing backslash.
	String ExePath7Zip;
    bool OEMlistfile7Zip;       // added in 093b1
	bool ActFile_ConfirmDelete;
	// - Logging:
	bool LogBuildingFileLists;
	bool LogFullAction;         // "Detailed File Progress"
	bool AutoSaveActionLog;
	String ActionLogDir;
    bool SaveAppStatus;         // added in 092b9
    int AppStatusMaxSize;       // added in 092b9
	// - Advanced:
	bool RemoveNonExistingSourceDirs;   // .. from Profile when opening.
	bool UseNewUIOnBFD;
//removed 095b1:    bool IncludeEmptyDirs;      // ... in the backup.
    bool CreateTempZipCopy;     // Action ZipMethod 'Replace'.
    bool UseZipBuilderAddSafe;  // ZipBuilder.AddOptions << AddSafe;
    bool TreatWarningsAsErrors;
	// === Other:

	// Commandline parameters:
	bool cmdLine_ParametersPresent;		// Indicates Abakt has been started with cmdline params.
	bool cmdLine_AutoExecute;
	bool cmdLine_AutoExit;
	bool cmdLine_Minimize;
	bool cmdLine_OVRDestFormatText;
	String cmdLine_CustomDestFormatText;

	// Directories assumed to be available/unmodified while application runs:

	String getDefaultUserDataDir();

	void setIniFilePath(String iniFilePath);
	/*	Used in Abakt.cpp to override the location of Abakt.ini using
        command line parameter -I. */

	String getIniFilePath();
	/*	Returns path to current user's Abakt.ini file. */

	String getHelpFile()
    {   return _helpFile; }
	/*	Returns path to helpfile '[app_dir]\Help\Abakt.chm'. */

   	void setHelpFile(String helpFilePath)
    {   _helpFile = helpFilePath; }
	/*	Sets path to helpfile. Will be saved to ini-file in f_Main. */

	void setTempDir(String dir);
	String getTempDir();
	String getFiltersDir()		{ return UserDataDir + "Filters\\"; }
	String getGroupsDir() 		{ return UserDataDir + "Groups\\"; }
	String getProfilesDir()		{ return UserDataDir + "Profiles\\"; }
	String getTemplatesDir()	{ return UserDataDir + "Templates\\"; }

private:

	bool _retrieveAppVersion();
	/* 	Retrieves the application version information from disk (Abakt.exe)
		and stores it in AppVersion and AppBuild.
		Returns false if version could not be retrieved.
		In that case AppVersion contains the error message. */

	/*	The following paths/directories are assumed to be available while application runs: */
	String _iniFilePath;
	String _helpFile;
	String _tempDir;
};
//---------------------------------------------------------------------------
extern TAbaktGlobals* PGlobals;	// Defined in AbaktGlobals.cpp. Created in Abakt.cpp.
//---------------------------------------------------------------------------
#endif
