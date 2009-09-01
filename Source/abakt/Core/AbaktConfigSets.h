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
#ifndef AbaktConfigSetsH
#define AbaktConfigSetsH
//---------------------------------------------------------------------------
#include "AbaktConfigItems.h"
//---------------------------------------------------------------------------

//===========================================================================
namespace abakt {
//===========================================================================

//===========================================================================
/* 	abakt::TConfigSet
	An abstract base class that encapsulates
    an abakt::TItemList and its abakt::TItem's. */
//===========================================================================

class TConfigSet
{
	public:
		TConfigSet();
		/*  Creates [items].
			The derived class's constructor can be used to create
            abakt::TItem objects and (optionally) call setDefaultValues(). */

		virtual ~TConfigSet();
		/* 	Destroys [items] and its members. */

		AnsiString name;
        /*  Internal English name of the TConfigSet. */

		AnsiString caption;
        /*  Caption (name) that will be used in the GUI (translatable). */

		abakt::TItemList* items;
        /*  Contains the list of all items in this ConfigSet. */

		virtual void setDefaultValues(void) = 0;
		/*	Derived classes MUST override this function. */

		void setOverrides(bool val);
        /*  Sets the override value for all items to [val]. */
};

//===========================================================================
//	TConfigSet's FilePropertiesFilter classes: TConfigSet_Fpf...
//===========================================================================

class TConfigSet_FpfAttrs : public TConfigSet
{
	public:
		TConfigSet_FpfAttrs();
		void setDefaultValues(void);
		// Items:
		abakt::TEnum *eMethodH;
		abakt::TEnum *eMethodR;
		abakt::TEnum *eMethodS;
};

class TConfigSet_FpfSize : public TConfigSet
{
	public:
		TConfigSet_FpfSize();
		void setDefaultValues(void);
		// Items:
        abakt::TUIntRange *iSmaller;
        abakt::TUIntRange *iGreater;
};

class TConfigSet_FpfDateModified : public TConfigSet
{
	public:
		TConfigSet_FpfDateModified();
		void setDefaultValues(void);
		// Items:
		abakt::TBool *bEnabled;
		abakt::TText *tDate1;  // 'yyyy-mm-dd'.
		abakt::TText *tDate2;  // 'yyyy-mm-dd'.
};

class TConfigSet_FpfAge : public TConfigSet
{
	public:
		TConfigSet_FpfAge();
		void setDefaultValues(void);
		// Items:
		abakt::TBool *bEnabled;     // If true, only files between Age1 and Age2 will be included.
		abakt::TUIntRange *iAge1;   // 0-32767 (TUpDown limit).
		abakt::TUIntRange *iAge2;   // 0-32767 (TUpDown limit).
};

//===========================================================================
//	TConfigSet's Action classes: TConfigSet_Act...
//===========================================================================

//---------------------------------------------------------------------------
//	TConfigSet_ActZip
//---------------------------------------------------------------------------

class TConfigSet_ActZip : public TConfigSet
{
	public:
		TConfigSet_ActZip();
		void setDefaultValues(void);
		// Items:
		abakt::TEnum* eMethod;
		abakt::TEnum* eStorePath;
		abakt::TUIntRange* iCompression;
//		abakt::TEnum* eZip64Mode;
		abakt::TText* tPassword;
		abakt::TBool* bSplit;
		abakt::TEnum* eSplitPreset;
		abakt::TUIntRange* iSplitCustomSize;
		abakt::TBool* bAddComment;
		abakt::TText* tCommentTemplate;
};

//---------------------------------------------------------------------------
//	TConfigSet_Act7Zip
//---------------------------------------------------------------------------

class TConfigSet_Act7Zip : public TConfigSet
{
	public:
		TConfigSet_Act7Zip();
		void setDefaultValues(void);
		// Items:
		abakt::TText* tCommandLineTemplate;
		abakt::TText* tCommand;
//		abakt::TEnum* eArchiveType; /* TODO 1 : Make this a TText object (?) */
		abakt::TText* tArchiveType; /* 092b6: TEnum -> TText. */
		abakt::TText* tPassword;
};

//---------------------------------------------------------------------------
//	TConfigSet_ActFile
//---------------------------------------------------------------------------

class TConfigSet_ActFile : public TConfigSet
{
	public:
		TConfigSet_ActFile();
		void setDefaultValues(void);
		// Items:
		abakt::TEnum* eMethod;
		abakt::TBool* bOverwrite;
        abakt::TBool* bOverwriteReadOnly;
		abakt::TBool* bUseRecycleBin;
//removed 095b1:		abakt::TBool* bUseSHFileOperation;
		abakt::TEnum* ePathInformation;
};

//---------------------------------------------------------------------------
//	TConfigSet_Action
//---------------------------------------------------------------------------

class TConfigSet_Action : public TConfigSet
{
	public:
		TConfigSet_Action();
    	void setDefaultValues(void);
		// Items:
		abakt::TEnum* eType;
		abakt::TEnum* eArchiveMode;
		abakt::TBool* bRemoveEmptyDirs;     // Finalize option
		abakt::TBool* bTestArchive;         // Finalize option
		abakt::TBool* bIncludeEmptySourceDirectories; // added 095b1
};

//---------------------------------------------------------------------------
//	TConfigSet_Destination
//---------------------------------------------------------------------------

class TConfigSet_Destination : public TConfigSet
{
	public:
		TConfigSet_Destination();
		void setDefaultValues(void);
		// Items:
		abakt::TPath* pDir;
		abakt::TBool* bAutoCreateDir;
		abakt::TText* tNameTemplate;
		abakt::TBool* bAppendExtension;
};

//---------------------------------------------------------------------------
//	TConfigSet_History
//---------------------------------------------------------------------------

class TConfigSet_History : public TConfigSet
{
	public:
		TConfigSet_History();
		void setDefaultValues(void);
		// Items:
		abakt::TBool* bMaintain;
		abakt::TUIntRange* iMaxDays;
		abakt::TUIntRange* iMinBackups;
		abakt::TUIntRange* iMaxBackups;
		abakt::TBool* bUseRecycleBin;
};

//---------------------------------------------------------------------------
//	TConfigSet_FileList
//---------------------------------------------------------------------------

class TConfigSet_FileList : public TConfigSet
{
	public:
		TConfigSet_FileList();
		void setDefaultValues(void);
		// Items:
		abakt::TBool* bEnabled;
};

//===========================================================================
//	TConfig - Encapsulates all of the above.
//  This is the declaration of the TConfig object that TProfile and TGroup use.
//===========================================================================

class TConfig
{
	public:
		TConfig();
		~TConfig();

        void reset();    // Set all ConfigSet's items to their default values.

		typedef struct {
			TConfigSet_FpfAttrs *Attributes;
			TConfigSet_FpfSize *Size;
			TConfigSet_FpfDateModified *DateModified;
            TConfigSet_FpfAge *Age;
		} TFilePropertiesFilter;
        TFilePropertiesFilter FilePropertiesFilter;

		typedef struct {
			TConfigSet_ActZip *Zip;
			TConfigSet_Act7Zip *SevenZip;
			TConfigSet_ActFile *File;
		} TActions;
        TActions Actions;

		TConfigSet_Action *Action;
		TConfigSet_Destination *Destination;
		TConfigSet_History *History;
		TConfigSet_FileList *FileList;
};

//===========================================================================
//	Some helpful functions:
//===========================================================================

String ItemValueToStr(TItem* item);
/*  - Returns item.value as a string.
    - The caller is responsible for translating the returned string.
      Translating is only safe for TBool and TEnum items!
    - Returns "?" if [item] is unknown. This should never happen. */

void CopyConfigSetItemsOnOverride(TConfigSet* source, TConfigSet* dest);
/* 	Copies the values of the source items to dest when item.override==true. */

void CopyAbaktConfigOnCustomOverride(abakt::TConfig& cfgSource, abakt::TConfig& cfgDest);
/* 	Copies abakt::TConfig from cfgSource to cfgDest
    for every item that has item.override==true. */

//===========================================================================
};	// namespace abakt
//===========================================================================

//---------------------------------------------------------------------------
#endif
