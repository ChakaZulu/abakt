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

#include "CommandLine.h"
#include "Action.h"
#include "Main.h"
#include "Group.h"
#include "AbaktGlobals.h"
#include "AbaktProfile.h"
#include "Gui_X_Profile.h"
#include "ExitCodes.h"
#include "GlobalFuncs.h"
#include "TemplateEditor.h"
//---------------------------------------------------------------------------
/* Command line option -P (priority level) needs "process.h" */
#include "process.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

/*  The function declarations below (and their definitions, farther below)
    are used for parsing the command line options. */

    // Declarations:
    void __doProfileAction(const String file); 	// .abp file
    void __doGroupAction(const String file);		// .abg file
    void __cmdLineLog(const ecc::TLBMessageStyle smt, const String msg);
    void __cmdLineLog(const String msg);

//---------------------------------------------------------------------------
static bool __createLog;	    // -l==true, -L==false
static String __templateFile;	// -T "FILE"

// -P=# variables:
static int __priorityWish;
static DWORD __priorityClass;
String priorityClassString;
unsigned __processId;
HANDLE __processHandle;

//---------------------------------------------------------------------------

bool ParseCommandLine()
{
	// For how-to on redirecting GUI output to console see:
	// http://dslweb.nwnexus.com/~ast/dload/guicon.htm

	PGlobals->cmdLine_AutoExit = false;
	PGlobals->cmdLine_Minimize = false;
	PGlobals->cmdLine_AutoExecute = false;
	PGlobals->cmdLine_OVRDestFormatText = false;
	PGlobals->cmdLine_CustomDestFormatText = "";
	__createLog = PGlobals->AutoSaveActionLog;
    __templateFile = "";

	if (_argc == 1)
    {
         __cmdLineLog(_("No command line arguments used. See the Manual for an overview."));
        return false; // No cmdline args.
    }

	PGlobals->cmdLine_ParametersPresent = true;

	String file;
	String arg;
	for (int i = 1; i < _argc; i++)
	{
		arg = String(_argv[i]);
		if (arg[1] != '-')
		{
			file = arg;
			break; // File MUST be the last arg.
		}
		if (arg.Length() < 2) continue;
		switch (arg[2])
		{
			case 'b':
				PGlobals->cmdLine_AutoExecute = true;
				break;

			case 'm':
				PGlobals->cmdLine_Minimize = true;
				break;

			case 'o':
				PGlobals->cmdLine_OVRDestFormatText = true;
				if (++i >= _argc) break;    // Get next arg if it exists.
				PGlobals->cmdLine_CustomDestFormatText = String(_argv[i]);
				break;

			case 'x':
				PGlobals->cmdLine_AutoExit = true;
				break;

			case 'l':
				__createLog = true;
				break;
			case 'L':
				__createLog = false;
				break;

            /* 095b2: Added command line option -P=#: Set application priority Level. */
            case 'P':   // -P=PRIORITY, where PRIORITY is a number from 0-3 (default=1).
                __priorityWish = StrToIntDef(String(arg[4]), 1); // Default value is '1'.
                __processId = getpid();    // Get process ID.
                __processHandle = OpenProcess(
                        PROCESS_ALL_ACCESS, //DWORD dwDesiredAccess,	// access flag
                        FALSE,              //BOOL bInheritHandle,	// handle inheritance flag
                        __processId         //DWORD dwProcessId 	// process identifier
                       );
                if (__processHandle == NULL)
                {
                    __cmdLineLog(__FUNC__ ": Could not obtain __processHandle");
                    continue;
                }
                switch (__priorityWish)
                {
                    case 0: __priorityClass = IDLE_PRIORITY_CLASS;
                        priorityClassString = _("Idle");
                        break;
                    case 2: __priorityClass = HIGH_PRIORITY_CLASS;
                        priorityClassString = _("High");
                        break;
                    case 3: __priorityClass = REALTIME_PRIORITY_CLASS;
                        priorityClassString = _("RealTime");
                        break;
                    default:
                    __priorityWish = 1; // Only values from 0-3 are allowed.
                    case 1: __priorityClass = NORMAL_PRIORITY_CLASS;
                        priorityClassString = _("Normal");
                }
                if (SetPriorityClass(
                        __processHandle,    // HANDLE hProcess:	handle to the process
                        __priorityClass     // DWORD dwPriorityClass: priority class value
                    ) != 0)
                    __cmdLineLog(ASPF(_("Application priority level has been set to: %s (%s)."),
                            IntToStr(__priorityWish), priorityClassString) );
                break;

            case 'I':   // -I "FILE", where FILE is a custom Abakt.ini file.
                // Step-over custom Abakt's .ini file location.
                // Has already been dealt with in Abakt.cpp.
				++i;
                break;

            case 'T':   // -T "FILE", where FILE is an Abakt Template (*.abt) file.
				if (++i >= _argc) break;    // Get next arg if it exists.
				__templateFile = String(_argv[i]);
                break;
		}
	}

	if (file.IsEmpty())
	{
		__cmdLineLog(_("No filename provided") + "..." + _("Ok."));
    	PGlobals->cmdLine_ParametersPresent = false; // Cmdline finished: Reset.
		return false;
	}

	// If [file] has no path information, try to find it in default Profiles/Groups dirs:
	if ( (file.Pos(":") == 0) || (file.Pos("\\") == 0) )
	{
		if (ExtractFileExt(file).LowerCase() == ".abp")
			file = PGlobals->getProfilesDir() + file;
		if (ExtractFileExt(file).LowerCase() == ".abg")
			file = PGlobals->getGroupsDir() + file;
	}

	if (!FileExists(file))
	{
		__cmdLineLog(ecc::smtError, _("Error") + " - " + ASPF(_("Could not open file: %s"), ecc::QuoteStr(file)) );
    	// 095b2: ?Required?: PGlobals->cmdLine_ParametersPresent = false; // Cmdline finished: Reset.
		return false;
	}

	if (ExtractFileExt(file).LowerCase() == ".abp")
		__doProfileAction(file);
	if (ExtractFileExt(file).LowerCase() == ".abg")
		__doGroupAction(file);
	PGlobals->cmdLine_ParametersPresent = false; // Cmdline finished: Reset.
	return true;
}
//---------------------------------------------------------------------------

//===========================================================================

//---------------------------------------------------------------------------

void __doProfileAction(const String file)
/*	Function is declared local. */
{
	if (PGlobals->cmdLine_AutoExecute)
	{
    	PGlobals->AutoSaveActionLog = __createLog;
    }
	__cmdLineLog(ASPF(_("Using Profile %s"), ecc::QuoteStr(file)) );
	f_Main->openProfileInGui(file);

    // Override Profile's settings with settings found in Template (if used):
   	if (!__templateFile.IsEmpty())
	{
        String file = __templateFile;
        // Add extension .abt if not already available:
        if (ExtractFileExt(file).LowerCase() != ".abt")
            file = file + ".abt";
        // Add path to .abt-file (defaults to Data/Templates) if not already available:
        if ( (file.Pos(":") == 0) || (file.Pos("\\") == 0) )
            file = PGlobals->getTemplatesDir() + file;
        // Display what will be done:
 		__cmdLineLog(ecc::smtNotif, ASPF(_("[-T] Adjusting Profile using Template %s."),
                ecc::QuoteStr(file)) );

        // Actually perform the override:
        f_TemplateEditor->openTemplate(file);
        abakt::TConfig* tmplConfig = f_TemplateEditor->getConfig();
        abakt::CopyAbaktConfigOnCustomOverride(*tmplConfig, PProfile->config);
        // Copy template's FilterSets to the Profile's list:
        MergeFilterSetLists(f_TemplateEditor->getFilterSetList(), &PProfile->filterSetList);

        ProfileToGui.all();
        // 092b9: Mark PProfile as modified only if not auto-executed & auto-closed:
        if ( PGlobals->cmdLine_AutoExecute && PGlobals->cmdLine_AutoExit)
            GuiToProfile.setModified(false);
        else
            GuiToProfile.setModified(true);
    }

	if (PGlobals->cmdLine_OVRDestFormatText)
	{
		if (PProfile->config.Destination->tNameTemplate->value != PGlobals->cmdLine_CustomDestFormatText)
		{
			__cmdLineLog(ecc::smtNotif, ASPF(_("[-o] Adjusting Destination's Name with: %s"),
                PGlobals->cmdLine_CustomDestFormatText) );
			PProfile->config.Destination->tNameTemplate->value = PGlobals->cmdLine_CustomDestFormatText;
			if (!PGlobals->cmdLine_AutoExit)
				GuiToProfile.setModified(true);
		}
		if (!PGlobals->cmdLine_AutoExit)
		{
            // Update GUI (and set PProfile->Modified):
			ProfileToGui.all();
		}
	}

	if (PGlobals->cmdLine_AutoExecute)
	{
		__cmdLineLog(_("[-b] Executing Action"));

		// == Do Action:
		f_Main->Enabled = false;	// Prevent shifting focus to f_Main.
		f_Action->actionStyle = asSingle;
		f_Action->Show();           // Starts the action, returns when action has finished.

        // == Determine and log ExitCode:
        // Make it possible to read %ERRORLEVEL% from .BAT scripts:
        AbaktExitCode = f_Action->failed() ? abakt::EXIT_ERROR : abakt::EXIT_OK;
        // If there were errors with some files
        // set abakt::EXIT_WARNING, or abakt::EXIT_ERROR:
        if (f_Action->abtFileList->errorItemList->Count > 0)
        {
            AbaktExitCode |= abakt::EXIT_WARNING;          // Always.
            if (PGlobals->TreatWarningsAsErrors)
                AbaktExitCode |= abakt::EXIT_ERROR;        // When TreatWarningsAsErrors is 'true'.
        }
        f_Action->logNotif( ASPF(_("ExitCode = %i"), AbaktExitCode) );
        __cmdLineLog( ASPF(_("ExitCode = %i"), AbaktExitCode) );

        // == Close f_Action:
		f_Action->Close();

        // == End:
		f_Main->Enabled = true;
		if (PGlobals->cmdLine_AutoExit)
			f_Main->Close();
		else
			f_Main->SetFocus();	// Required.
	}
}
//---------------------------------------------------------------------------

void __doGroupAction(const String file)
/*	Function is declared local. */
{
	if (PGlobals->cmdLine_AutoExecute)
	{
		__cmdLineLog(ASPF(_("Executing Group %s"),
            ecc::QuoteStr(file)));
		PGlobals->AutoSaveActionLog = __createLog;
		if (!f_Group->openGroup(file))
		{
			__cmdLineLog(ASPF(_("Could not open Group %s"),
                ecc::QuoteStr(file) + "."));
			return;
		}

		if (PGlobals->cmdLine_OVRDestFormatText)
		{
			__cmdLineLog(_("[-o] Adjusting Destination's FormatText can only be used when opening single Profiles."));
		}

		// Do Group Action:
		f_Main->Enabled = false;	// Prevent shifting focus to f_Main.
		f_Action->actionStyle = asGroup;
        f_Action->Show();
        f_Action->Close();
		f_Main->Enabled = true;

		if (PGlobals->cmdLine_AutoExit)
			f_Main->Close();
		else
		{
			//Application->Restore();
			f_Main->SetFocus();	// Seems necessary...
		}
	}
	else
	{
		__cmdLineLog(ASPF(_("Using Group %s"),
            ecc::QuoteStr(file) ));
		f_Group->openGroup(file);
		f_Group->ShowModal();
	}
}
//---------------------------------------------------------------------------

void __cmdLineLog(const ecc::TLBMessageStyle smt, const String msg)
{
    f_Main->doLog(_("Command Line") + ": " + msg, smt);
}
//---------------------------------------------------------------------------

void __cmdLineLog(const String msg)
{
    __cmdLineLog(ecc::smtDefault, msg);
}
//---------------------------------------------------------------------------

