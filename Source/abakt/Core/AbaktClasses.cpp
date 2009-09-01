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
#pragma hdrstop

#include "AbaktClasses.h"

//---------------------------------------------------------------------------
/*  Do NOT #include "TRANSLATE.h"!
    All strings in the objects in this file should be English. Translation
    is done by the GUI. The reason that the strings are surrounded by _()
    is that dxgettext should be able to find them (see below). */
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

/*  How to make the String arrays translatable:

    === Make sure dxgettext finds the strings to translate by surrounding
    every individual string with _(). The #define and #undef directives
    are necessary to prevent a bug on non-English systems: the strings
    are translated in the system's language, not in the selected language.
    NOT: --- Because of the redefinition of _(txt) warning #8017 is suppressed
        in this file: #pragma warn -8017 ?????

    #define _(txt) txt
    static const String test[] =
    {
        _("string0"),
        _("string1")
    };
    #undef _(txt) txt

    === Show the literal, untranslated string:
    ShowMessage(test[0]);       // This produces "string0".

    === Show the translation of the string:
    ShowMessage(_(test[1]));    // This produces whatever translation you
                                // have provided for "string1" in the .po file.
*/
//---------------------------------------------------------------------------

//===========================================================================
//  Make sure (dx)gettext finds the following strings:

#define _(txt) txt  // See above.

// Used in Group window (and possible elsewhere):
static const String DUMMY_translate_true    = _("true");
static const String DUMMY_translate_false   = _("false");

// Used in calls to VCL's MessageDlg (and possible elsewhere) (See TMsgDlgButtons() Help):
// (Reported by Yenidai on 2005-Feb-08)
static const String DUMMY_translate_Cancel  = _("Cancel");  // mbCancel
static const String DUMMY_translate_Confirm = _("Confirm"); // mbConfirm
static const String DUMMY_translate__Yes    = _("&Yes");    // mbYes
static const String DUMMY_translate__No     = _("&No");     // mbNo
static const String DUMMY_translate__Retry  = _("&Retry");  // mbRetry
static const String DUMMY_translate__Ignore = _("&Ignore"); // mbIgnore

// Used in dropdown menu's:
// (Reported by Yenidai on 2005-Dec-14)
/* TODO 3 : TRANSLATE: Ctrl+Space (Can it be done?) */
//static const String DUMMY_translate_Space   = _("Space");

#undef _(txt) txt

//===========================================================================

//===========================================================================
//	TAttributeFilterMethod
//===========================================================================

#define _(txt) txt  // See above.
static const String TAttributeFilterMethodList[] =
{
	_("Ignore"),
	_("Exclude"),
	_("OnlyInclude")
};  // Should correspond to associated enum members in header file.
#undef _(txt) txt

static const int TAttributeFilterMethodListSize
    = sizeof(TAttributeFilterMethodList)/sizeof(TAttributeFilterMethodList[0]);
//---------------------------------------------------------------------------

int AttributeFilterMethodStrListSize()
{
	return TAttributeFilterMethodListSize;
}
//---------------------------------------------------------------------------

String AttributeFilterMethodToStr(TAttributeFilterMethod idx)
{
	if (idx >= TAttributeFilterMethodListSize)
		idx = fpaIgnore; // Default

	return TAttributeFilterMethodList[idx];
}
//---------------------------------------------------------------------------

TAttributeFilterMethod StrToAttributeFilterMethod(String str)
{
	for (int i = 0; i < TAttributeFilterMethodListSize; i++)
	{
		if (str == TAttributeFilterMethodList[i])
			return (TAttributeFilterMethod)i;
	}
	return fpaIgnore;	// Default
}
//---------------------------------------------------------------------------

//===========================================================================
//	TActionType
//===========================================================================

#define _(txt) txt  // See above.
static const String TActionTypeStrList[] =
{
	_("None"),
	_("Zip"),
	_("SevenZip"),
	_("File")
};  // Should correspond to associated enum members in header file.
#undef _(txt) txt

static const int TActionTypeStrListSize
    = sizeof(TActionTypeStrList)/sizeof(TActionTypeStrList[0]);
//---------------------------------------------------------------------------

int ActionTypeStrListSize()
{
	return TActionTypeStrListSize;
}
//---------------------------------------------------------------------------

String ActionTypeToStr(TActionType idx)
{
	if (idx >= TActionTypeStrListSize)
		idx = actZip; // Default

	return TActionTypeStrList[idx];
}
//---------------------------------------------------------------------------

TActionType StrToActionType(String str)
{
	for (int i = 0; i < TActionTypeStrListSize; i++)
	{
		if (str == TActionTypeStrList[i])
			return (TActionType)i;
	}
	return actZip;	// Default
}
//---------------------------------------------------------------------------

//===========================================================================
//	TArchiveMode
//===========================================================================

#define _(txt) txt  // See above.
static const String TArchiveModeStrList[] =
{
	_("Default"),		// Default ;)
	_("Full"),
	_("Differential"),
	_("Incremental"),
	_("Inverse")
};  // Should correspond to associated enum members in header file.
#undef _(txt) txt

static const int TArchiveModeStrListSize
    = sizeof(TArchiveModeStrList)/sizeof(TArchiveModeStrList[0]);
//---------------------------------------------------------------------------

int ArchiveModeStrListSize()
{
	return TArchiveModeStrListSize;
}
//---------------------------------------------------------------------------

String ArchiveModeToStr(TArchiveMode idx)
{
	if (idx >= TArchiveModeStrListSize)
		idx = amDefault; // Default
	if (idx < 0) return "";

	return TArchiveModeStrList[idx];
}
//---------------------------------------------------------------------------

TArchiveMode StrToArchiveMode(String str)
{
	for (int i = 0; i < TArchiveModeStrListSize; i++)
	{
		if (str == TArchiveModeStrList[i])
			return (TArchiveMode)i;
	}
	return amDefault;	// Default
}
//---------------------------------------------------------------------------

//===========================================================================
//	TActZipMethod (ActionType Zip Method)
//===========================================================================

#define _(txt) txt  // See above.
static const String TActZipMethodStrList[] =
{
	_("New"),
	_("Replace"),
	_("Update"),
	_("Freshen")
};  // Should correspond to associated enum members in header file.
#undef _(txt) txt

static const int TActZipMethodStrListSize
    = sizeof(TActZipMethodStrList)/sizeof(TActZipMethodStrList[0]);
//---------------------------------------------------------------------------

int ActZipMethodStrListSize()
{
	return TActZipMethodStrListSize;
}
//---------------------------------------------------------------------------

String ActZipMethodToStr(TActZipMethod idx)
{
	if (idx >= TActZipMethodStrListSize)
		idx = azmNew; // Default

	return TActZipMethodStrList[idx];
}
//---------------------------------------------------------------------------

TActZipMethod StrToActZipMethod(String str)
{
	for (int i = 0; i < TActZipMethodStrListSize; i++)
	{
		if (str == TActZipMethodStrList[i])
			return (TActZipMethod)i;
	}
	return azmNew;	// Default
}
//---------------------------------------------------------------------------

//===========================================================================
//	TActZipStorePath (ActionType Zip StorePath)
//===========================================================================

#define _(txt) txt  // See above.
static const String TActZipStorePathStrList[] =
{
	_("Full"),
	_("Relative"),
	_("None")
};  // Should correspond to associated enum members in header file.
#undef _(txt) txt

static const int TActZipStorePathStrListSize
    = sizeof(TActZipStorePathStrList)/sizeof(TActZipStorePathStrList[0]);
//---------------------------------------------------------------------------

int ActZipStorePathStrListSize()
{
	return TActZipStorePathStrListSize;
}
//---------------------------------------------------------------------------

String ActZipStorePathToStr(TActZipStorePath idx)
{
	if (idx >= TActZipStorePathStrListSize) return "";
	if (idx < 0)                            return "";
	return TActZipStorePathStrList[idx];
}
//---------------------------------------------------------------------------

TActZipStorePath StrToActZipStorePath(String str)
{
	for (int i = 0; i < TActZipStorePathStrListSize; i++)
	{
		if (str == TActZipStorePathStrList[i])
			return (TActZipStorePath)i;
	}
	return azpFull;	// Default
}
//---------------------------------------------------------------------------

//===========================================================================
//	TActZip64Mode (ActionType Zip Zip64Mode)
//===========================================================================

#define _(txt) txt  // See above.
static const String TActZip64ModeStrList[] =
{
	_("Always"),
	_("Automatic"),
	_("Disabled")
};  // Should correspond to associated enum members in header file.
#undef _(txt) txt

static const int TActZip64ModeStrListSize
    = sizeof(TActZip64ModeStrList)/sizeof(TActZip64ModeStrList[0]);
//---------------------------------------------------------------------------

int ActZip64ModeStrListSize()
{
	return TActZip64ModeStrListSize;
}
//---------------------------------------------------------------------------

String ActZip64ModeToStr(TActZip64Mode idx)
{
	if (idx >= TActZip64ModeStrListSize) return "";
	if (idx < 0)                         return "";
	return TActZip64ModeStrList[idx];
}
//---------------------------------------------------------------------------

TActZip64Mode StrToActZip64Mode(String str)
{
	for (int i = 0; i < TActZip64ModeStrListSize; i++)
	{
		if (str == TActZip64ModeStrList[i])
			return (TActZip64Mode)i;
	}
	return az64Auto;	// Default
}
//---------------------------------------------------------------------------

//===========================================================================
//	TActZipSplitPreset (ActionType Zip Method)
//===========================================================================

#define _(txt) txt  // See above.
static const String TActZipSplitPresetStrList[] =
{
	_("Custom..."),
	_("1.44 MB (3.5\" floppy disk)"),
	_("94.6 MB (100 MB IOMEGA disk)"),
	_("200 MB (MO disk)"),
	_("250 MB (MO disk)"),
	_("600 MB (MO disk)"),
	_("650 MB (74 min. CD)"),
	_("700 MB (80 min. CD)"),
	_("750 MB (750 MB IOMEGA disk)")

};  // Should correspond to associated enum members in header file.
#undef _(txt) txt

static const int TActZipSplitPresetStrListSize
    = sizeof(TActZipSplitPresetStrList)/sizeof(TActZipSplitPresetStrList[0]);
//---------------------------------------------------------------------------

int ActZipSplitPresetStrListSize()
{
    return TActZipSplitPresetStrListSize;
}
//---------------------------------------------------------------------------

String ActZipSplitPresetToStr(TActZipSplitPreset idx)
{
	if (idx >= TActZipSplitPresetStrListSize)
        return "";
	if (idx < 0)
        return "";
	return TActZipSplitPresetStrList[idx];
}
//---------------------------------------------------------------------------

TActZipSplitPreset StrToActZipSplitPreset(String str)
{
	for (int i = 0; i < TActZipStorePathStrListSize; i++)
	{
		if (str == TActZipSplitPresetStrList[i])
			return (TActZipSplitPreset)i;
	}
	return azspCustom;  // Default
}
//---------------------------------------------------------------------------

//===========================================================================
//	TActFileMethod (ActionType File Method)
//===========================================================================

#define _(txt) txt  // See above.
static const String TActFileMethodStrList[] =
{
	_("Copy"),
	_("Move"),
	_("Delete")
};  // Should correspond to associated enum members in header file.
#undef _(txt) txt

static const int TActFileMethodStrListSize
    = sizeof(TActFileMethodStrList)/sizeof(TActFileMethodStrList[0]);
//---------------------------------------------------------------------------

int ActFileMethodStrListSize()
{
	return TActFileMethodStrListSize;
}
//---------------------------------------------------------------------------

String ActFileMethodToStr(TActFileMethod idx)
{
	if (idx >= TActFileMethodStrListSize)
		idx = afmCopy; // Default

	return TActFileMethodStrList[idx];
}
//---------------------------------------------------------------------------

TActFileMethod StrToActFileMethod(String str)
{
	for (int i = 0; i < TActFileMethodStrListSize; i++)
	{
		if (str == TActFileMethodStrList[i])
			return (TActFileMethod)i;
	}
	return afmCopy;	// Default
}
//---------------------------------------------------------------------------

//===========================================================================
//	TAct7ZipArchiveType (ActionType SevenZip ArchiveType)
//===========================================================================

/* TODO 4 : Combine both String[] arrays below? */

static const String TAct7ZipArchiveTypeStrList[] =
// these are also the values passed to 7z.exe using switch -t
{
	"7z",
	"zip",
//	"gzip",
//	"bzip2",
	"tar"
};

static const int TAct7ZipArchiveTypeStrListSize
    = sizeof(TAct7ZipArchiveTypeStrList)/sizeof(TAct7ZipArchiveTypeStrList[0]);
//---------------------------------------------------------------------------

static const String TAct7ZipArchiveTypeExtList[] =
{
	"7z",
	"zip",
//	"gz",
//	"bz2",
	"tar"
};

static const int TAct7ZipArchiveTypeExtListSize
    = sizeof(TAct7ZipArchiveTypeExtList)/sizeof(TAct7ZipArchiveTypeExtList[0]);
//---------------------------------------------------------------------------

int Act7ZipArchiveTypeStrListSize()
{
	return TAct7ZipArchiveTypeStrListSize;
}
//---------------------------------------------------------------------------

TAct7ZipArchiveType StrToAct7ZipArchiveType(String str)
{
	for (int i = 0; i < TAct7ZipArchiveTypeStrListSize; i++)
	{
		if (str == TAct7ZipArchiveTypeStrList[i])
			return (TAct7ZipArchiveType)i;
	}
	return astZip;	// Default
}
//---------------------------------------------------------------------------

