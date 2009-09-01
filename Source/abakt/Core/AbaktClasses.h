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
#ifndef AbaktClassesH
#define AbaktClassesH
//---------------------------------------------------------------------------
#include <system.hpp>	// TDateTime
#include "AbaktConfigItems.h"
//---------------------------------------------------------------------------

// Trick BCB6 into compiling abtString and _T():
// (abtString is used in abakt2)
#undef abtString
#define abtString char*
#define _T_not(txt) txt
// In abakt2 _T_not() should be _T(). Comment previous line to enable this.


//===========================================================================
/*	XML Element name definitions: */
//===========================================================================

/*  _T() is used to make the source compatible with abakt2 (work in progress).
    Don't confuse this with _(), used for translations. */

//	<abakt-profile>:
static const abtString ELM_abakt_profile(_T_not("abakt-profile"));

//	<source>:
static const abtString ELM_source(_T_not("source"));
	static const abtString ELM_src_root(    _T_not("root"));
	static const abtString ELM_src_exclude( _T_not("exclude"));
	static const abtString ELM_src_include( _T_not("include"));

//	<FileFilters>:  // Added in 0.9.2-beta3
static const abtString ELM_FileFilters(_T_not("FileFilters"));
	static const abtString ELM_FF_Filter(_T_not("Filter"));

//	<filters>:
static const abtString ELM_filters(_T_not("filters"));
	static const abtString ELM_filt_exclude(_T_not("exclude"));
	static const abtString ELM_filt_include(_T_not("include"));
	static const abtString ELM_filt_no_compress(_T_not("no_compress"));


//  === TConfigSets:

// -- <FilePropertiesFilter>: (247+)
static const abtString ELM_FilePropertiesFilter(_T_not("FilePropertiesFilter"));
	static const abtString ELM_Fpf_Attributes(      _T_not("Attributes"));
	static const abtString ELM_Fpf_Size(            _T_not("Size"));
	static const abtString ELM_Fpf_DateModified(    _T_not("DateModified"));
	static const abtString ELM_Fpf_Age(             _T_not("Age"));
// -- <ActionTypes>:
static const abtString ELM_ActionTypes(_T_not("ActionTypes"));
	static const abtString ELM_Act_Zip(     _T_not("Zip"));
	static const abtString ELM_Act_7Zip(    _T_not("SevenZip"));
	static const abtString ELM_Act_File(    _T_not("File"));
// -- <Action>:
static const abtString ELM_Action(_T_not("Action"));
// -- <Destination>:
static const abtString ELM_Destination(_T_not("Destination"));
// -- <History>:
static const abtString ELM_History(_T_not("History"));
// -- <FileList>:
static const abtString ELM_FileList(_T_not("FileList"));


//	=== abakt::TConfig <item>:

static const abtString ELM_item(_T_not("item"));
	static const abtString ATR_item_override(   _T_not("override"));
	static const abtString ATR_item_name(       _T_not("name"));
	static const abtString ATR_item_datatype(   _T_not("datatype"));
	static const abtString ATR_item_data(       _T_not("data"));


//	=== used by TAbaktGroup ---

static const abtString ELM_profiles(_T_not("profiles"));

//===========================================================================
/*	TAbtELMid
	Declaration of elements below root-element.
	Used while reading xml-profile: */
//===========================================================================

typedef enum
{
	ELMid_none = 0,
	ELMid_source,
	ELMid_filters,
    ELMid_FileFilters,
        ELMid_FF_Filter,
/* OLD */ ELMid_FilePropertiesFilter, // this, and sub-thisses are used to import pre 0.9.2-beta3 profiles. 
        ELMid_Fpf_Attributes,
        ELMid_Fpf_Size,
        ELMid_Fpf_DateModified,
        ELMid_Fpf_Age,
	ELMid_ActionTypes,
		ELMid_Act_Zip,
		ELMid_Act_7Zip,
		ELMid_Act_File,
	ELMid_Action,
	ELMid_Destination,
	ELMid_History,
	ELMid_FileList,
	ELMid_profiles, // Used by AbaktGroup.
	ELMid_filelist 	// Used by AbaktHistory.
} TAbtELMid;
//---------------------------------------------------------------------------

//===========================================================================
//	TAttributeFilterMethod
//  Used in TConfigSet_FpfAttrs
//===========================================================================

typedef enum {
	fpaIgnore   	= 0,	// Default
	fpaExclude	    = 1,    // Exclude files for which this attribute is set
	fpaOnlyInclude	= 2     // ONLY include files for which this attribute is set
} TAttributeFilterMethod;
// Should correspond to associated String[] members in cpp file.

int AttributeFilterMethodStrListSize();
String AttributeFilterMethodToStr(TAttributeFilterMethod idx);
TAttributeFilterMethod StrToAttributeFilterMethod(String str);
//---------------------------------------------------------------------------

//===========================================================================
//	TActionType
//===========================================================================

typedef enum {
	actNone,
	actZip,		// Default
	act7Zip,
	actFile
} TActionType;
// Should correspond to associated String[] members in cpp file.

int ActionTypeStrListSize();
String ActionTypeToStr(TActionType idx);
TActionType StrToActionType(String str);

//===========================================================================
//	TArchiveMode
//===========================================================================

typedef enum {
	amDefault,		// Default
	amFull,
	amDifferential,
	amIncremental,
	amInverse
} TArchiveMode;
// Should correspond to associated String[] members in cpp file.

int ArchiveModeStrListSize();
String ArchiveModeToStr(TArchiveMode idx);
TArchiveMode StrToArchiveMode(String str);

//===========================================================================
//	TActZipMethod (ActionType Zip Method)
//===========================================================================

typedef enum {
	azmNew,	 	// Default
	azmReplace,
	azmUpdate,
	azmFreshen
} TActZipMethod;
// Should correspond to associated String[] members in cpp file.

int ActZipMethodStrListSize();
String ActZipMethodToStr(TActZipMethod idx);
TActZipMethod StrToActZipMethod(String str);

//===========================================================================
//	TActZipStorePath (ActionType Zip StorePath)
//===========================================================================

typedef enum {
	azpFull,	 	// Default
	azpRelative,
	azpNone
} TActZipStorePath;
// Should correspond to associated String[] members in cpp file.

int ActZipStorePathStrListSize();
String ActZipStorePathToStr(TActZipStorePath idx);
TActZipStorePath StrToActZipStorePath(String str);

//===========================================================================
//	TActZip64Mode (ActionType Zip Zip64Mode)
//===========================================================================

/* TODO 1 : Remove TActZip64Mode code? */

typedef enum {
	az64Always,
	az64Auto,   
	az64Disabled
} TActZip64Mode;
// Should correspond to associated String[] members in cpp file.

int ActZip64ModeStrListSize();
String ActZip64ModeToStr(TActZip64Mode idx);
TActZip64Mode StrToActZip64Mode(String str);

//===========================================================================
//	TActZipSplitPreset (ActionType Zip Method)
//===========================================================================

typedef enum {
    azspCustom,
    azsp1_44,   // floppy
	azsp100,    // in MB...
	azsp200,
	azsp250,    // available since 251+
	azsp600,
	azsp650,
	azsp700,
	azsp750
} TActZipSplitPreset;
// Should correspond to associated String[] members in cpp file.

int ActZipSplitPresetStrListSize();
String ActZipSplitPresetToStr(TActZipSplitPreset idx);
TActZipSplitPreset StrToActZipSplitPreset(String str);

//===========================================================================
//	TActionType_Zip
//===========================================================================

typedef struct {
	TActZipMethod Method;
	int Compression;
	String Password;
	bool Split;
	int SplitVolumeSizePreset;
	unsigned long SplitCustomVolumeSizeB; // Unit is Bytes (minimum is 64 kB for ZipForge).
	bool AddVersionComment;
} TActionType_Zip;

//===========================================================================
//	TActFileMethod (ActionType File Method)
//===========================================================================

typedef enum {
	afmCopy,	// Default
	afmMove,
	afmDelete
} TActFileMethod;
// Should correspond to associated String[] members in cpp file.

int ActFileMethodStrListSize();
String ActFileMethodToStr(TActFileMethod idx);
TActFileMethod StrToActFileMethod(String str);

//===========================================================================
//	TAct7ZipArchiveType
//===========================================================================

typedef enum {
	ast7Zip,
	astZip,
//	astGzip,
//	astBzip2,
	astTar
} TAct7ZipArchiveType;

int Act7ZipArchiveTypeStrListSize();

TAct7ZipArchiveType StrToAct7ZipArchiveType(String str);
//===========================================================================

//---------------------------------------------------------------------------
#endif
