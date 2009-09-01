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

USEFORM("Main.cpp", f_Main);
USEFORM("About.cpp", f_About);
USEFORM("Action.cpp", f_Action);
USEFORM("DataModule.cpp", DM); /* TDataModule: File Type */
USEFORM("SourceFrame.cpp", FrameSource); /* TFrame: File Type */
USEFORM("FiltersFrame.cpp", FrameFilters); /* TFrame: File Type */
USEFORM("ActionTypeFrame.cpp", FrameActionType); /* TFrame: File Type */
USEFORM("ActionTypeSettingsFrame.cpp", FrameActionTypeSettings); /* TFrame: File Type */
USEFORM("DestinationFrame.cpp", FrameDestination); /* TFrame: File Type */
USEFORM("FinalizeFrame.cpp", FrameFinalize); /* TFrame: File Type */
USEFORM("Group.cpp", f_Group);
USEFORM("ConfigEditFrame.cpp", FrameConfigEdit); /* TFrame: File Type */
USEFORM("FilterPreview.cpp", f_FilterPreview);
USEFORM("Preferences.cpp", f_Prefs);
USEFORM("FileSizeEditor.cpp", f_FileSizeEditor);
USEFORM("TemplateEditor.cpp", f_TemplateEditor);
USEFORM("RestoreBackup.cpp", f_RestoreBackup);
USEFORM("FilterSetEditor.cpp", f_FilterSetEditor);
USEFORM("CheckListBoxFrame.cpp", FrameCheckListBox); /* TFrame: File Type */
USEFORM("FilterSetListFrame.cpp", FrameFilterSetList); /* TFrame: File Type */
USEFORM("History.cpp", f_History);
//---------------------------------------------------------------------------
#include "AbaktGlobals.h"
#include "AbaktProfile.h"
#include "AbaktGroup.h"
#include "ExitCodes.h"
#include <Inifiles.hpp>
#include "eccMisc.h"
//---------------------------------------------------------------------------

#ifdef ABT_TRANSLATE
void SelectLanguage()
{
    String loc;

    TIniFile *ini = new TIniFile(PGlobals->getIniFilePath());
    loc = ini->ReadString("Translations", "LocaleId", "en"); // [loc] may have been set by Setup (267+).
    delete ini;

    UseLanguage(loc);   // See gnugettext.pas
    PGlobals->LocaleId = loc;
}
#endif
//---------------------------------------------------------------------------

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Set classes to be ignored by gnugettext:
    #ifdef ABT_TRANSLATE
        TP_GlobalIgnoreClass(__classid(TFont));
    #endif

    /*  System's language can be obtained by calling GetCurrentLanguage() HERE: */
    // ShowMessage("System's language=" + GetCurrentLanguage());

	// == Create global classes, made accessible by their pointers:
	PGlobals	= new TAbaktGlobals();	// Pointer is declared in "AbaktGobals.h".
    #ifdef ABT_TEST
        if (PGlobals == NULL)
            ShowMessage("PGlobals == NULL");
    #endif

    // Do some command line preparations:
	String arg;
	for (int i = 1; i < _argc; i++)
	{
		arg = String(_argv[i]);
		switch (arg[2])
		{
			case 'b':   // Do it in Abakt.cpp to prevent flickering of main window on startup.
				PGlobals->cmdLine_AutoExecute = true;
				break;

			case 'm':   // Do it in Abakt.cpp to prevent flickering of main window on startup.
				PGlobals->cmdLine_Minimize = true;
				break;

			case 'I':
				if (++i >= _argc)
                {
                    AbaktExitCode = abakt::EXIT_ERROR;
                    break;
                }
                arg = String(_argv[i]);
				PGlobals->setIniFilePath(arg);
				break;
		}
	}

    // Detect and set language:
    #ifdef ABT_TRANSLATE
        SelectLanguage();
    #endif
	PProfile	= new TAbaktProfile();  // Pointer is declared in "AbaktProfile.h".
	PGroup		= new TAbaktGroup();	// Pointer is declared in "AbaktGroup.h".

    AbaktExitCode = abakt::EXIT_OK;

	if (!PGlobals || !PProfile || !PGroup)
	{
		ShowMessage( __FUNC__ "\n:Could not create instances of required global objects!");
        AbaktExitCode = abakt::EXIT_ERROR;
		return AbaktExitCode;
	}


	// Do minimize in Abakt.cpp to prevent flickering of main window on startup:
	//095rc2: Moved (similar) code to Tf_Main::Tf_Main(). Reported by TH, 9-nov-2006.
    if (PGlobals->cmdLine_AutoExecute)
	{
		if (PGlobals->cmdLine_Minimize)
            Application->Minimize();
	}

	// Start application:
	try
	{
        Application->Initialize();
        Application->Title = "Abakt";
        Application->CreateForm(__classid(Tf_Main), &f_Main);
        Application->CreateForm(__classid(Tf_About), &f_About);
        Application->CreateForm(__classid(Tf_Group), &f_Group);
        Application->CreateForm(__classid(Tf_Action), &f_Action);
        Application->CreateForm(__classid(TDM), &DM);
        Application->CreateForm(__classid(Tf_Prefs), &f_Prefs);
        Application->CreateForm(__classid(Tf_FilterPreview), &f_FilterPreview);
        Application->CreateForm(__classid(Tf_FileSizeEditor), &f_FileSizeEditor);
        Application->CreateForm(__classid(Tf_TemplateEditor), &f_TemplateEditor);
        Application->CreateForm(__classid(Tf_RestoreBackup), &f_RestoreBackup);
        Application->CreateForm(__classid(Tf_FilterSetEditor), &f_FilterSetEditor);
        Application->CreateForm(__classid(Tf_History), &f_History);
        Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
        AbaktExitCode = abakt::EXIT_ERROR;
	}

    // 095rc2: Attempt to fix Abakt hanging when started with no user logged in:
    if (PGlobals->cmdLine_AutoExecute)
    {
        if (PGlobals->cmdLine_Minimize)
        {
            Application->Restore();
        }
    }

	//	Destroy global objects:
	delete PGroup;
	delete PProfile;
	delete PGlobals;

	// End:
    return AbaktExitCode;
}
//---------------------------------------------------------------------------

