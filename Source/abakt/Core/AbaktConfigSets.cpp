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

#include "AbaktConfigSets.h"
#include "AbaktClasses.h"
#include "eccString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

/*  Important notes:
    Every TConfigItem's caption and info property may be translated by
    using _().
    All TEnum._items members should be English. Translation is done by the GUI.
*/

//===========================================================================
namespace abakt {
//===========================================================================

//===========================================================================
//	TAbaktConfigItems
//===========================================================================

TConfigSet::TConfigSet()
{
	items   = new abakt::TItemList();
	name    = "";
	caption = "";
}
//---------------------------------------------------------------------------

TConfigSet::~TConfigSet()
{
	items->destroy();
	delete items;
}
//---------------------------------------------------------------------------

void TConfigSet::setOverrides(bool val)
{
	for (unsigned int i = 0; i < items->size(); i++)
	{
        items->at(i)->override = val;
    }
}
//---------------------------------------------------------------------------

//===========================================================================
//	TConfigSet_Fpf... (FilePropertiesFilter)
//===========================================================================

//---------------------------------------------------------------------------
//	TConfigSet_FpfAttrs
//---------------------------------------------------------------------------

TConfigSet_FpfAttrs::TConfigSet_FpfAttrs()
{
    name            = "FilePropertiesFilter";       // TConfigSet's internal English caption.
    caption         = _("File Properties Filter");  // TConfigSet's GUI caption.
   	items->name 	= "Attributes";                 // TItemList's internal English caption.
    items->caption	= _("Attributes");              // TItemList's GUI caption.
    items->owner    = this;                         // TItemList's owner: a TConfigSet.

    eMethodH = items->createEnum("H");
    eMethodH->caption   = _("H Filter Method");
 	for (int i = 0; i < AttributeFilterMethodStrListSize(); i++)
		eMethodH->add(AttributeFilterMethodToStr((TAttributeFilterMethod)i));
    eMethodH->info      = _("H = Hidden");

    eMethodR = items->createEnum("R");
    eMethodR->caption   = _("R Filter Method");
 	for (int i = 0; i < AttributeFilterMethodStrListSize(); i++)
		eMethodR->add(AttributeFilterMethodToStr((TAttributeFilterMethod)i));
    eMethodR->info      = _("R = Read Only");

    eMethodS = items->createEnum("S");
    eMethodS->caption = _("S Filter Method");
 	for (int i = 0; i < AttributeFilterMethodStrListSize(); i++)
		eMethodS->add(AttributeFilterMethodToStr((TAttributeFilterMethod)i));
    eMethodS->info      = _("S = System");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_FpfAttrs::setDefaultValues(void)
{
    eMethodH->setIndex((int)fpaIgnore);
    eMethodR->setIndex((int)fpaIgnore);
    eMethodS->setIndex((int)fpaIgnore);

	setOverrides(false);
}

//---------------------------------------------------------------------------
//	TConfigSet_FpfSize
//---------------------------------------------------------------------------

TConfigSet_FpfSize::TConfigSet_FpfSize()
{
    name            = "FilePropertiesFilter";       // TConfigSet's internal English caption.
    caption         = _("File Properties Filter");  // TConfigSet's GUI caption.
   	items->name 	= "Size";                       // TItemList's internal English caption.
    items->caption	= _("Size");                    // TItemList's GUI caption.
    items->owner    = this;                         // TItemList's owner: a TConfigSet.

	iSmaller = items->createUIntRange("Smaller", 0, 0, 0x7fffffff);
	iSmaller->caption   = _("Exclude files smaller than N Bytes");
	iSmaller->info      = _("Ignored if 0.");

	iGreater = items->createUIntRange("Greater", 0, 0, 0x7fffffff);
	iGreater->caption   = _("Exclude files greater than N Bytes");
	iGreater->info      = _("Ignored if 0.");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_FpfSize::setDefaultValues(void)
{
    iSmaller->set(0);
    iGreater->set(0);

	setOverrides(false);
}

//---------------------------------------------------------------------------
//	TConfigSet_FpfDateModified
//---------------------------------------------------------------------------

TConfigSet_FpfDateModified::TConfigSet_FpfDateModified()
{
    name            = "FilePropertiesFilter";       // TConfigSet's internal English caption.
    caption         = _("File Properties Filter");  // TConfigSet's GUI caption.
   	items->name 	= "DateModified";               // TItemList's internal English caption.
    items->caption	= _("Modified");                // TItemList's GUI caption.
    items->owner    = this;                         // TItemList's owner: a TConfigSet.

    // Items:
    bEnabled = items->createBool("Enabled");
    bEnabled->caption   = _("Enabled");
    bEnabled->info      = _("Exclude files modified between Date1 and Date2");

    tDate1 = items->createText("Date1");
    tDate1->caption = _("File modification date 1");
    tDate1->info    = _("Format: 'yyyy-mm-dd'.");

    tDate2 = items->createText("Date2");
    tDate2->caption = _("File modification date 2");
    tDate2->info    = _("Format: 'yyyy-mm-dd'.");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_FpfDateModified::setDefaultValues(void)
{
    bEnabled->value = false;
    tDate1->value = "2004-01-01";
    tDate2->value = "2005-01-01";

	setOverrides(false);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	TConfigSet_FpfDateModified
//---------------------------------------------------------------------------

TConfigSet_FpfAge::TConfigSet_FpfAge()
{
    name            = "FilePropertiesFilter";       // TConfigSet's internal English caption.
    caption         = _("File Properties Filter");  // TConfigSet's GUI caption.
   	items->name 	= "Age";                        // TItemList's internal English caption.
    items->caption	= _("Age");                     // TItemList's GUI caption.
    items->owner    = this;                         // TItemList's owner: a TConfigSet.

    // Items:
    bEnabled = items->createBool("Enabled");
    bEnabled->caption = _("Enabled");
    bEnabled->info = _("Only Include files with an age between Age1 and Age2 days.");

    iAge1 = items->createUIntRange("Age1", 0, 0, 0x7FFF);
    iAge1->caption = _("File age 1");
    iAge1->info    = _("Should be less than or equal to Age2.");
    iAge2 = items->createUIntRange("Age2", 0, 0, 0x7FFF);
    iAge2->caption = _("File age 2");
    iAge2->info    = _("Should be greater than or equal to Age2.");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_FpfAge::setDefaultValues(void)
{
    bEnabled->value = false;
    iAge1->set(0);
    iAge2->set(0);

	setOverrides(false);
}
//---------------------------------------------------------------------------

//===========================================================================
//	TConfigSet_ActZip
//===========================================================================

TConfigSet_ActZip::TConfigSet_ActZip()
{
    name            = "ActionTypes";    // TConfigSet's internal English caption.
    caption         = _("Action Type"); // TConfigSet's GUI caption.
   	items->name 	= "Zip";            // TItemList's internal English caption.
    items->caption	= _("Zip");         // TItemList's GUI caption.
    items->owner    = this;             // TItemList's owner: a TConfigSet.

	eMethod = items->createEnum("Method");
	eMethod->caption = _("Archive Method");
	eMethod->info =
        _("Choosing Update or Freshen possibly has no effect on a dynamic Archive Filename (date/time).");
	for (int i = 0; i < ActZipMethodStrListSize(); i++)
		eMethod->add(ActZipMethodToStr((TActZipMethod)i));

	eStorePath = items->createEnum("StorePath");
	eStorePath->caption = _("Path Information");
	eStorePath->info    = _("The path information to store for every file.");
	for (int i = 0; i < ActZipStorePathStrListSize(); i++)
		eStorePath->add(ActZipStorePathToStr((TActZipStorePath)i));

	iCompression = items->createUIntRange("Compression", 6, 0, 9);
	iCompression->caption   = _("Compression Level");
	iCompression->info      = _("0 (None) - 6 (Normal) - 9 (Max.)");

/*	eZip64Mode = items->createEnum("Zip64Mode");
	eZip64Mode->caption = _("Zip64 Mode");
	eZip64Mode->info    = _("Zip64 allows Zip archives to be greater than 4 GB.");
	for (int i = 0; i < ActZip64ModeStrListSize(); i++)
		eZip64Mode->add(ActZip64ModeToStr((TActZip64Mode)i)); */

	tPassword = items->createText("Password", true);
	tPassword->caption  = _("Password");
	tPassword->info     = _("Spaces are allowed.");

	bSplit = items->createBool("Split");
	bSplit->caption = _("Split Archive");
	bSplit->info    = ""; // 092b10 _("It is not possible to Update or Freshen a Split Archive.");

	eSplitPreset = items->createEnum("SplitPreset");
	eSplitPreset->caption   = _("Split - Volume Size Preset");
	eSplitPreset->info      = _("Choose a predefined setting or define a custom value.");
	for (int i = 0; i < ActZipSplitPresetStrListSize(); i++)
		eSplitPreset->add(ActZipSplitPresetToStr((TActZipSplitPreset)i));

	iSplitCustomSize = items->createUIntRange("SplitCustomSize", 0x10000, 0x10000, 0x7FFFFFFF);
	iSplitCustomSize->caption   = _("Split - Custom Volume Size");
	iSplitCustomSize->info      = _("Range: 64 kB - 2048 MB");

	bAddComment = items->createBool("AddComment");
	bAddComment->caption    = _("Add Comment");
    bAddComment->info       = _("Does not apply to split Zip archives.");

    tCommentTemplate = items->createText("CommentTemplate");
	tCommentTemplate->caption   = _("Comment Template");
	tCommentTemplate->info      = _("See the manual for more information.");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_ActZip::setDefaultValues(void)
{
	eMethod->setIndex((int)azmNew);
	eStorePath->setIndex((int)azpFull);
	iCompression->set(6);
//    eZip64Mode->setIndex((int)az64Disabled);
	tPassword->value = "";
	bSplit->value = false;
	eSplitPreset->setIndex((int)azspCustom);
	iSplitCustomSize->set( iSplitCustomSize->getMin() );
	bAddComment->value = false;
    tCommentTemplate->value = _("'Abakt $V ['dd'-'mm'-'yyyy'@'hh.nn']'");
	setOverrides(false);
}
//---------------------------------------------------------------------------

//===========================================================================
//	TConfigSet_Act7Zip
//===========================================================================

TConfigSet_Act7Zip::TConfigSet_Act7Zip()
{
    name            = "ActionTypes";    // TConfigSet's internal English caption.
    caption         = _("Action Type"); // TConfigSet's GUI caption.
   	items->name 	= "SevenZip";       // TItemList's internal English caption.
    items->caption	= _("7-Zip");       // TItemList's GUI caption.
    items->owner    = this;             // TItemList's owner: a TConfigSet.

    tCommandLineTemplate = items->createText("CommandLineTemplate");
	tCommandLineTemplate->caption = _("Command Line Template");
	tCommandLineTemplate->info    = _("Refer to Abakt's and 7-Zip's manuals for more information.");
        //_("Possible variables: $C=Command, $N=Name, $T=ArchiveType, $P=Password, $W=WorkingDir, $L=ListFile.\nRefer to the 7-Zip manual for help on the Command Line syntax.");

   	tCommand = items->createText("Command");
	tCommand->caption   = _("Command");
	tCommand->info      = _("a=Add; u=Update");

    tArchiveType = items->createText("ArchiveType");
	tArchiveType->caption   = _("Archive Type");
	tArchiveType->info      = _("The type of the archive to create ('7z', 'zip' etc.), also sets the extension (do not enter leading dot '.').");
/*092b6
 	eArchiveType = items->createEnum("ArchiveType");
	eArchiveType->caption = _("Archive Type");
 	for (int i = 0; i < Act7ZipArchiveTypeStrListSize(); i++)
		eArchiveType->add(Act7ZipArchiveTypeToStr((TAct7ZipArchiveType)i)); */

	tPassword = items->createText("Password", true);
	tPassword->caption = _("Password");
	tPassword->info    = _("Spaces are not allowed.");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_Act7Zip::setDefaultValues(void)
{
    tCommandLineTemplate->value = "$C $T $N -mx=5 $P $W $L";
    tCommand->value     = "a";
    tArchiveType->value = "7z";
//	eArchiveType->setIndex((int)ast7Zip);
	tPassword->value	= "";

	setOverrides(false);
}
//---------------------------------------------------------------------------

//===========================================================================
//	TConfigSet_ActFile
//===========================================================================

TConfigSet_ActFile::TConfigSet_ActFile()
{
    name            = "ActionTypes";        // TConfigSet's internal English caption.
    caption         = _("Action Type");     // TConfigSet's GUI caption.
   	items->name 	= "File";               // TItemList's internal English caption.
    items->caption	= _("File Operation");  // TItemList's GUI caption.
    items->owner    = this;                 // TItemList's owner: a TConfigSet.

	eMethod = items->createEnum("Method");
	eMethod->caption = _("Method");
	for (int i = 0; i < ActFileMethodStrListSize(); i++)
		eMethod->add(ActFileMethodToStr((TActFileMethod)i));

	bOverwrite = items->createBool("Overwrite");
	bOverwrite->caption = _("Overwrite Existing Files");
	bOverwrite->info = _("Applies to Methods 'Copy/Move'."); //"\nIf unchecked, files will be named 'Copy (#) of ...'.";

	bOverwriteReadOnly = items->createBool("OverwriteReadOnly");
	bOverwriteReadOnly->caption = _("Also Overwrite Existing Read Only Files");
	//bOverwriteReadOnly->info = ASPF(_("Requires %s."), bOverwrite->info);

	bUseRecycleBin = items->createBool("UseRecycleBin");
	bUseRecycleBin->caption = _("Use Recycle Bin");
	bUseRecycleBin->info = _("Applies to Method 'Delete'.");

//removed 095b1:
/* 	bUseSHFileOperation = items->createBool("UseSHFileOperation");
	bUseSHFileOperation->caption = _("Use Windows function SHFileOperation");
	bUseSHFileOperation->info = _("Applies to Methods 'Copy/Move/Delete'.") + "\n"
        + _("Provides less feedback, but is sometimes faster. Do not use with many files (thousands)."); */

	ePathInformation = items->createEnum("PathInformation");
	ePathInformation->caption = _("Path Information");
	ePathInformation->info    = _("The destination path information used when copying/moving a file.");
	for (int i = 0; i < ActZipStorePathStrListSize(); i++)
		ePathInformation->add(ActZipStorePathToStr((TActZipStorePath)i));

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_ActFile::setDefaultValues(void)
{
	eMethod->setIndex((int)afmCopy);
	bOverwrite->value		    = true;
    bOverwriteReadOnly->value	= false;
	bUseRecycleBin->value	    = true;
//removed 095b1:	bUseSHFileOperation->value  = false;
    ePathInformation->setIndex((int)azpRelative);   //095b1: Default is azpRelative instead of azpFull. 

	setOverrides(false);
}
//---------------------------------------------------------------------------

//===========================================================================
//	TConfigSet_Action
//===========================================================================

TConfigSet_Action::TConfigSet_Action()
{
    name            = ""; //NOT_USED    // TConfigSet's internal English caption.
    caption         = _("Action");      // TConfigSet's GUI caption.
   	items->name 	= "Action";         // TItemList's internal English caption.
    items->caption	= _("Action");      // TItemList's GUI caption.
    items->owner    = this;             // TItemList's owner: a TConfigSet.

	eType = items->createEnum("ActionType");
	eType->caption = _("Action Type");
	for (int i = 0; i < ActionTypeStrListSize(); i++)
		eType->add(ActionTypeToStr((TActionType)i));

	eArchiveMode = items->createEnum("ArchiveMode");
	eArchiveMode->caption = _("Archive Mode");
    eArchiveMode->info    = _("The Archive attribute will not be modified when using Action Types 'Move' or 'Delete'.");
	for (int i = 0; i < ArchiveModeStrListSize(); i++)
		eArchiveMode->add(ArchiveModeToStr((TArchiveMode)i));

	bRemoveEmptyDirs = items->createBool("RemoveEmptyDirs");
	bRemoveEmptyDirs->caption = _("Remove Emptied Source Directories");
	bRemoveEmptyDirs->info    = _("Remove Source directories that were emptied as a result of File Operation actions.");

    bTestArchive = items->createBool("TestArchive");
	bTestArchive->caption = _("Test Archive After Action");
	bTestArchive->info    = _("Applies to Zip Archives only.");

    bIncludeEmptySourceDirectories = items->createBool("IncludeEmptySourceDirectories");
    bIncludeEmptySourceDirectories->caption = _("Include Empty Source Directories");
    bIncludeEmptySourceDirectories->info = _("Perform the chosen Action on Empty Source Directories as well as on files.");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_Action::setDefaultValues(void)
{
	eType->setIndex( (int)actZip );
	eArchiveMode->setIndex( (int)amDefault );
	bRemoveEmptyDirs->value = false;
    bTestArchive->value = false;
    bIncludeEmptySourceDirectories->value = true;

	setOverrides(false);
}
//---------------------------------------------------------------------------

//===========================================================================
//	TConfigSet_Destination
//===========================================================================

TConfigSet_Destination::TConfigSet_Destination()
{
    name            = ""; //NOT_USED    // TConfigSet's internal English caption.
    caption         = _("Action");      // TConfigSet's GUI caption.
   	items->name 	= "Destination";    // TItemList's internal English caption.
    items->caption	= _("Destination"); // TItemList's GUI caption.
    items->owner    = this;             // TItemList's owner: a TConfigSet.

	pDir = items->createPath("Directory");
	pDir->caption = _("Destination Directory");

	bAutoCreateDir = items->createBool("AutoCreateDirectory");
	bAutoCreateDir->caption = _("Automatically Create Directory");
	bAutoCreateDir->info    = _("If enabled, the directory will be created on Action execution.");

    tNameTemplate = items->createText("NameTemplate");
    tNameTemplate->caption  = _("Name Template");
    tNameTemplate->info     = _("See the manual for more information.");

	bAppendExtension = items->createBool("AppendExtension");
	bAppendExtension->caption   = _("Append Extension");
	bAppendExtension->info      = _("If disabled, the default extension ('.zip', '.7z', etc.) will not be appended.");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_Destination::setDefaultValues(void)
{
	pDir->value 				= "";
	bAutoCreateDir->value		= true;
    tNameTemplate->value		= _("'$N'");
	bAppendExtension->value		= true;

	setOverrides(false);
}
//---------------------------------------------------------------------------

//===========================================================================
//	TConfigSet_History
//===========================================================================

TConfigSet_History::TConfigSet_History()
{
    name            = ""; //NOT_USED    // TConfigSet's internal English caption.
    caption         = _("Finalize");    // TConfigSet's GUI caption.
   	items->name 	= "History";        // TItemList's internal English caption.
    items->caption	= _("History");     // TItemList's GUI caption.
    items->owner    = this;             // TItemList's owner: a TConfigSet.

	bMaintain = items->createBool("Maintain");
	bMaintain->caption      = _("Maintain a list of successful backups");
	bMaintain->info         = _("If disabled, the History will not be maintained, and no '*.abh' files will be created or updated.");

	iMaxDays = items->createUIntRange("MaximumDays", 0, 0, 0x7FFF);
	iMaxDays->caption       = _("Remove backups older than N days");
	iMaxDays->info          = _("Ignored if 0.");

	iMinBackups = items->createUIntRange("MinimumBackups", 0, 0, 0x7FFF);
	iMinBackups->caption    = _("Always keep a minimum of N backups");
	iMinBackups->info       = _("Ignored if 0.");

	iMaxBackups = items->createUIntRange("MaximumBackups", 0, 0, 0x7FFF);
	iMaxBackups->caption    = _("Keep a maximum of N backups");
	iMaxBackups->info       = _("Ignored if 0.");

	bUseRecycleBin = items->createBool("UseRecycleBin");
	bUseRecycleBin->caption = _("Use Recycle Bin");
	bUseRecycleBin->caption = _("Move deleted backups to Recycle Bin if possible");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_History::setDefaultValues(void)
{
	bMaintain->value = false;       // Default is 'false' since 0.9.3-RC2.
	iMaxDays->set(0);
	iMinBackups->set(0);
	iMaxBackups->set(0);
	bUseRecycleBin->value = false;  // Default is 'false' since 0.9.3-RC2.

	setOverrides(false);
}
//---------------------------------------------------------------------------

//===========================================================================
//	TConfigSet_FileList
//===========================================================================

TConfigSet_FileList::TConfigSet_FileList()
{
    name            = ""; //NOT_USED    // TConfigSet's internal English caption.
    caption         = _("Finalize");    // TConfigSet's GUI caption.
   	items->name 	= "FileList";       // TItemList's internal English caption.
    items->caption	= _("FileList");    // TItemList's GUI caption.
    items->owner    = this;             // TItemList's owner: a TConfigSet.

	bEnabled = items->createBool("Enabled");
	bEnabled->caption  = _("Add FileList to Destination");
	bEnabled->info     = _("If enabled, a text file containing the files successfully operated on will be written to the destination directory.");

	setDefaultValues();
}
//---------------------------------------------------------------------------

void TConfigSet_FileList::setDefaultValues(void)
{
	bEnabled->value = false;

	setOverrides(false);
}
//---------------------------------------------------------------------------

//===========================================================================
//	TConfig
//===========================================================================

TConfig::TConfig()
{
    FilePropertiesFilter.Attributes   = new TConfigSet_FpfAttrs();
    FilePropertiesFilter.Size         = new TConfigSet_FpfSize();
    FilePropertiesFilter.DateModified = new TConfigSet_FpfDateModified();
    FilePropertiesFilter.Age          = new TConfigSet_FpfAge();

	Actions.Zip			= new TConfigSet_ActZip();
	Actions.SevenZip	= new TConfigSet_Act7Zip();
	Actions.File  		= new TConfigSet_ActFile();

	Action		    = new TConfigSet_Action();
	Destination	    = new TConfigSet_Destination();
	History		    = new TConfigSet_History();
	FileList	    = new TConfigSet_FileList();
}
//---------------------------------------------------------------------------

TConfig::~TConfig()
{
    delete FilePropertiesFilter.Attributes;
    delete FilePropertiesFilter.Size;
    delete FilePropertiesFilter.DateModified;
    delete FilePropertiesFilter.Age;

	delete Actions.Zip;
	delete Actions.SevenZip;
	delete Actions.File;

	delete Action;
	delete Destination;
	delete History;
    delete FileList;
}
//---------------------------------------------------------------------------

void TConfig::reset()
{
    FilePropertiesFilter.Attributes->setDefaultValues();
    FilePropertiesFilter.Size->setDefaultValues();
    FilePropertiesFilter.DateModified->setDefaultValues();
    FilePropertiesFilter.Age->setDefaultValues();

	Actions.Zip->setDefaultValues();
	Actions.SevenZip->setDefaultValues();
	Actions.File->setDefaultValues();

	Action->setDefaultValues();
	Destination->setDefaultValues();
	History->setDefaultValues();
    FileList->setDefaultValues();
}
//---------------------------------------------------------------------------


//===========================================================================
//	Functions:
//===========================================================================

String ItemValueToStr(TItem* item)
{
    String str;
	switch (item->type())
	{
		case abakt::dtBool:
			return ecc::BoolToStr( ((abakt::TBool*)item)->value );

		case abakt::dtUIntRange:
			return IntToStr( ((abakt::TUIntRange*)item)->get() );

		case abakt::dtText:
            // 092b8: added code to mask text if appropriate.
            str = ((abakt::TText*)item)->value;
            if (((abakt::TText*)item)->masked)
                return str.StringOfChar('*', str.Length());
			else
                return str;

		case abakt::dtPath:
			return ((abakt::TPath*)item)->value;

		case abakt::dtEnum:
			return ((abakt::TEnum*)item)->selected();

        default:
            #ifdef ABT_TEST
            	ShowMessage(__FUNC__ ": Unknown item->type()");
            #endif
        	return "?";
	}
}
//---------------------------------------------------------------------------

void CopyConfigSetItemsOnOverride(TConfigSet* source, TConfigSet* dest)
{
	TItem *srcItem, *destItem;

	for (unsigned int i = 0; i < source->items->size(); i++)
	{
		srcItem = source->items->at(i);
		destItem = dest->items->at(i);

		#ifdef ABT_TEST
            if ( (!srcItem) || (!destItem) )
            {
                ShowMessage(__FUNC__ ": srcItem and/or destItem is NULL");
                continue; // Error!
            }
            if (srcItem->name != destItem->name)
            {
                ShowMessage(__FUNC__ ": srcItem->name != destItem->name");
                continue; // Error!
            }

            if (srcItem->type() != destItem->type())
            {
                ShowMessage(__FUNC__ ": srcItem->type() != destItem->type()");
                continue; // Error!
            }
		#endif

		// Copy overridden items:
		if (!srcItem->override) continue;
		switch (srcItem->type())
		{
			case dtBool:
				((TBool*)destItem)->value = ((TBool*)srcItem)->value;
				break;

			case abakt::dtUIntRange:
				((TUIntRange*)destItem)->set( ((TUIntRange*)srcItem)->get() );
				break;

			case abakt::dtText:
				((TText*)destItem)->value = ((TText*)srcItem)->value;
				break;

			case abakt::dtPath:
				((TPath*)destItem)->value = ((TPath*)srcItem)->value;
				break;

			case abakt::dtEnum:
				((TEnum*)destItem)->setIndex( ((TEnum*)srcItem)->getIndex() );
				break;

			default:
				ShowMessage(__FUNC__ ": Unknown srcItem->type()");
				return;
		}
	}
}
//---------------------------------------------------------------------------

void CopyAbaktConfigOnCustomOverride(abakt::TConfig& cfgSource, abakt::TConfig& cfgDest)
/* == TODO 1 : ALWAYS: PProfile->Config has changed? Change this too! */
{
	CopyConfigSetItemsOnOverride(cfgSource.Action,          cfgDest.Action);
	CopyConfigSetItemsOnOverride(cfgSource.Actions.File,    cfgDest.Actions.File);
	CopyConfigSetItemsOnOverride(cfgSource.Actions.Zip,     cfgDest.Actions.Zip);
	CopyConfigSetItemsOnOverride(cfgSource.Actions.SevenZip,    cfgDest.Actions.SevenZip);

	CopyConfigSetItemsOnOverride(cfgSource.Destination,     cfgDest.Destination);
	CopyConfigSetItemsOnOverride(cfgSource.History,         cfgDest.History);
	CopyConfigSetItemsOnOverride(cfgSource.FileList,        cfgDest.FileList);
}

//===========================================================================
};	// namespace abakt
//===========================================================================

