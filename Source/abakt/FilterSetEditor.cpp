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

#include "FilterSetEditor.h"
#include "AbaktGlobals.h"
#include "AbaktFilter.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tf_FilterSetEditor *f_FilterSetEditor;
//---------------------------------------------------------------------------

__fastcall Tf_FilterSetEditor::Tf_FilterSetEditor(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterSetEditor::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
        // TComboBoxEx and gnugettext don't work together very well:
        TP_GlobalIgnoreClass(__classid(TComboBoxEx));
    #endif

    // Prepare cbex_Action (add text, icon nr's, etc.):
    try
    {
        cbex_Action->ItemsEx->Clear();
        cbex_Action->Images = DM->imgs_Filters;
        String TR_str;

        /*  TComboExItem* __fastcall AddItem(
                constAnsiString Caption,
                const int ImageIndex, const int SelectedImageIndex, const int OverlayImageIndex,
                const int Indent, void *Data); */

        TR_str = _("Exclude");  // First, prepare a translated string, then add it to cbex_Action:
        cbex_Action->ItemsEx->AddItem(TR_str,
            0, 0, -1,
            0, NULL);

        TR_str = _("Include (Always)");   // First, prepare a translated string, then add it to cbex_Action:
        cbex_Action->ItemsEx->AddItem(TR_str,
            1, 1, -1,
            0, NULL);

        TR_str = _("No Compression");     // First, prepare a translated string, then add it to cbex_Action:
        cbex_Action->ItemsEx->AddItem(TR_str,
            2, 2, -1,
            0, NULL);
    }
    catch (...)
    {
        ShowMessage(__FUNC__ ":Oops! Please report.");
    }
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterSetEditor::FormShow(TObject *Sender)
{
    _copyFilterSetToGui();
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterSetEditor::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
    if (ModalResult == mrOk)
        if (!_validateFilterSet())
            CanClose = false;
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterSetEditor::bt_ResetClick(TObject *Sender)
{
    String rememberFilterSetName = filterSet.name;
    filterSet.reset();
    filterSet.name = rememberFilterSetName;
    _copyFilterSetToGui();
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterSetEditor::bt_HelpClick(TObject *Sender)
{
    String url = "mk:@MSITStore:" + PGlobals->getHelpFile() + "::/Edit_Filters.html";
    ShellExecute(Handle, "open", "hh.exe", url.c_str(), NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterSetEditor::bt_OkClick(TObject *Sender)
{
    /*  ModalResult for this button is set to mrOk. */

    _copyGuiToFilterSet();
    // Form will only close if the FilterSet conditions are valid, see FormCloseQuery.
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterSetEditor::bt_CancelClick(TObject *Sender)
{
    /*  ModalResult for this button is set to mrCancel. */

    // The function that called this->ShowModal() should ignore new settings.
}
//---------------------------------------------------------------------------

bool Tf_FilterSetEditor::_validateFilterSet()
{
    _copyGuiToFilterSet();
    TAbaktFilter abf;
    String validateMessage = abf.validateAllConditions(filterSet);

    if (validateMessage.IsEmpty())
    {
        // If validateMessage is empty, all conditions are valid:
        return true;
    }
    else
    {
        // One or more conditions are not valid, validateMessage contains more information:
        ShowMessage(validateMessage);
        return false;
    }
}
//---------------------------------------------------------------------------

void Tf_FilterSetEditor::_copyFilterSetToGui()
{
    ed_Name->Text           = filterSet.name;
    cbex_Action->ItemIndex  = (int)filterSet.action;
    cb_Enabled->Checked     = filterSet.enabled;

    ed_PathMask->Text   = filterSet.path;
    ed_Size->Text       = filterSet.size;
    ed_Date->Text       = filterSet.date;
    ed_Age->Text        = filterSet.age;
    ed_Attributes->Text = filterSet.attributes;
}
//---------------------------------------------------------------------------

void Tf_FilterSetEditor::_copyGuiToFilterSet()
{
    filterSet.name     = ed_Name->Text;
    filterSet.action   = (TAbaktFilterSetAction)cbex_Action->ItemIndex;
    filterSet.enabled  = cb_Enabled->Checked;

    filterSet.path         = ed_PathMask->Text;
    filterSet.size         = ed_Size->Text;
    filterSet.date         = ed_Date->Text;
    filterSet.age          = ed_Age->Text;
    filterSet.attributes   = ed_Attributes->Text;
}
//---------------------------------------------------------------------------

void __fastcall Tf_FilterSetEditor::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Key == VK_F1)
        bt_Help->Click();
}
//---------------------------------------------------------------------------

