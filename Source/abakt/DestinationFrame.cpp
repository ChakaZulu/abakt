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

#include "DestinationFrame.h"
#include "AbaktProfile.h"
#include "AbaktGlobals.h"
#include "GlobalFuncs.h"
#include "Gui_X_Profile.h"
#include "eccBrowseForDirectory.h"
#include "eccFile.h"
#include "AbaktHistory.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameDestination *FrameDestination;

//---------------------------------------------------------------------------
#ifdef ABT_TRANSLATE
    #pragma warn -8017  // Suppress warning.
    #define _(txt) txt  // See AbaktClasses.cpp for an explanation.
#endif
static const String NameTemplateExamples[] =
/*  These strings correspond to cmbx_FormatTextExamples's Items. */
{
    "",                                     // Choose...
    "'$N'",                                 // <Name>
	_("'$N-MyText'"),
	"'$N-BAK-['yyyy'-'mm'-'dd']'",          // <Name>-BAK-<Date>
	"'$N-['yyyy'-'mm'-'dd']-$M'",           // <Name>-<Date>-<ArchiveMode>
	"'$N-['yyyy'-'mm'-'dd'@'hh.nn']'",      // <Name>-<Date@Time>
    _("'Backup of Profile $N made on 'dddd', 'dd'-'mmm'-'yyyy")
};
#ifdef ABT_TRANSLATE
    #undef _(txt) txt
    #pragma warn .8017  // Restore warning value.
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

__fastcall TFrameDestination::TFrameDestination(TComponent* Owner)
	: TFrame(Owner)
{
	ScrollBox_Dest->BorderStyle = bsNone;

//disabled in 0.9.2-beta3	lbl_AutoCreateDir->Caption	 = PProfile->Config.Destination->bAutoCreateDir->info;
	lbl_AppendExtension->Caption = PProfile->config.Destination->bAppendExtension->info;

//    formattedDirPreview();
	formattedNamePreview();
}
//---------------------------------------------------------------------------

void __fastcall TFrameDestination::bt_BrowseDirClick(TObject *Sender)
{
	ecc::TBrowseForDirectory bd(PGlobals->UseNewUIOnBFD);
	bd.Handle 		= Handle;
	bd.Directory	= _formattedDestDir(ed_Directory->Text);
	bd.Title 		= _("Choose Destination Directory:");
	if (bd.Execute())
	{
		ed_Directory->Text = IncludeTrailingPathDelimiter(bd.Directory);
		GuiToProfile.setModified(true);
	}
}
//---------------------------------------------------------------------------

String TFrameDestination::_formattedDestDir(String destDir)
{
    // If a quote (') is found in destdir, it should be formatted:
    if (destDir.Pos("'"))
        return abtFormatText(destDir);
    else
        return destDir;
}
//---------------------------------------------------------------------------

void __fastcall TFrameDestination::SetProfileModified(TObject *Sender)
{
	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrameDestination::ed_DirectoryChange(TObject *Sender)
{
//    formattedDirPreview();
    GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

/* TODO 3 : implement formattedDirPreview(): */
/* void TFrameDestination::formattedDirPreview()
{
    // Only format text when a quote (') is found, otherwise treat it as a normal directory string.
     if (ed_DestinationDir->Text.Pos("'"))
        ed_FormattedDirPreview->Text = abtFormatText(ed_DestinationDir->Text);
} */
//---------------------------------------------------------------------------

void TFrameDestination::formattedNamePreview()
{
	String prev = ed_NameTemplate->Text;

    prev = abtFormatText(prev);

	// Make sure PProfile is up to date:
	GuiToProfile.actionType();

	TActionType act = (TActionType)PProfile->config.Action->eType->getIndex();
	switch (act)
	{
		case actZip:
			if (!cb_AppendExtension->Checked)
				break;
			else
				prev += ".zip";
			break;

		case act7Zip:
			if (!cb_AppendExtension->Checked)
				break;
			else
				//prev += "." + Act7ZipArchiveTypeToExt((TAct7ZipArchiveType)f_Main->fr_ActionTypeSettings->cmbx_7z_ArchiveType->ItemIndex );
				prev += "." + f_Main->fr_ActionTypeSettings->cmbx_7z_ArchiveType->Text;
			break;

		case actFile:
            if (!prev.IsEmpty())
    			prev += "\\";
			break;

		default:
			break;
	}
	ed_FormattedNamePreview->Text = prev;
}
//---------------------------------------------------------------------------

void __fastcall TFrameDestination::cmbx_NameTemplateExamplesSelect(
      TObject *Sender)
{
    String newFormatText = NameTemplateExamples[cmbx_NameTemplateExamples->ItemIndex];
    if (newFormatText.IsEmpty()) return;

    ed_NameTemplate->Text = _( newFormatText );
   	formattedNamePreview();
    GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrameDestination::ed_NameTemplateChange(TObject *Sender)
{
   	formattedNamePreview();
    GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrameDestination::cb_AppendExtensionClick(TObject *Sender)
{
   	formattedNamePreview();
    GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

