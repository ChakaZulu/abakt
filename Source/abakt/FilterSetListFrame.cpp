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

	You should have received a  copy of the GNU General Public License
	along with Abakt; if not, write to the Free Software Foundation, Inc.,
	59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

===========================================================================*/

//---------------------------------------------------------------------------
#include <vcl.h>
#include "TRANSLATE.h"
#pragma hdrstop

#include "FilterSetListFrame.h"
#include "DataModule.h"
#include "FilterSetEditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameFilterSetList *FrameFilterSetList;
//---------------------------------------------------------------------------
__fastcall TFrameFilterSetList::TFrameFilterSetList(TComponent* Owner)
    : TFrame(Owner)
{
    _filterSetList = NULL;
    _onModified = NULL;
}
//---------------------------------------------------------------------------

void TFrameFilterSetList::setFilterSetList(std::vector<TAbaktFilterSet> *list)
{
    _filterSetList = list;

    redraw();
}
//---------------------------------------------------------------------------

void TFrameFilterSetList::redraw()
{
    if (_filterSetList == NULL) return;

    lv->Items->BeginUpdate();

	lv->Clear();

    TListItem* litem;
	for (unsigned int i = 0; i < _filterSetList->size(); i++)
	{
        // Add a new item to the ListView:
    	litem = lv->Items->Add();

		// action (icon):
        if (_filterSetList->at(i).action == fsaInclude) litem->ImageIndex = ICON_INCLUDE;
        else if (_filterSetList->at(i).action == fsaNoCompression) litem->ImageIndex = ICON_NOCOMPRESS;
        else litem->ImageIndex = ICON_EXCLUDE;  // Default.
        // name:
        litem->Caption = _filterSetList->at(i).name;
		// enabled:
        litem->Checked = _filterSetList->at(i).enabled;
        // path (mask):
        litem->SubItems->Add( _filterSetList->at(i).path );
        // size:
        litem->SubItems->Add( _filterSetList->at(i).size );
        // date (modification):
        litem->SubItems->Add( _filterSetList->at(i).date );
        // age:
        litem->SubItems->Add( _filterSetList->at(i).age );
        // attributes:
        litem->SubItems->Add( _filterSetList->at(i).attributes );
	}

    lv->Items->EndUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::tb_AddClick(TObject *Sender)
{
    if (_filterSetList == NULL) return;

    f_FilterSetEditor->filterSet.reset();

    if (f_FilterSetEditor->ShowModal() != mrOk)
        return;

    // Add FilterSet (== insert at current position):
    int idx = lv->ItemIndex;
    if (idx < 0)
    {
        // No item is selected (or list is empty), append the new FilterSet to end of list:
        _filterSetList->push_back(f_FilterSetEditor->filterSet);
        idx = lv->Items->Count; // End of list.
    }
    else
    {
        // Insert new FilterSet in PProfile->filterSetList:
        TAbaktFilterSet* insertPos = &_filterSetList->at(idx);
        _filterSetList->insert(insertPos, f_FilterSetEditor->filterSet);
    }

    // Update GUI:
    redraw();

    // Set lv_FilterSets->ItemIndex to idx:
    if (idx < lv->Items->Count)
    {
        lv->ItemIndex = idx;
        lv->ItemFocused = lv->Items->Item[idx];
        lv->Items->Item[idx]->MakeVisible(false);
    }

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::tb_RemoveClick(TObject *Sender)
{
    if (_filterSetList == NULL) return;

    int idx = lv->ItemIndex;
    if (idx < 0) return;

    // Remove FilterSet:
    TAbaktFilterSet* removePos = &_filterSetList->at(idx);
    #ifdef ABT_TEST
        if (removePos == NULL)
        {
            ShowMessage(__FUNC__ ": (removePos == NULL)");
            return;
        }
    #endif
    _filterSetList->erase(removePos);

    // Update GUI:
    redraw();

    // Set focus to idx:
    if (idx >= lv->Items->Count) --idx;
    if (idx >= 0)
    {
        lv->ItemIndex = idx;
        lv->ItemFocused = lv->Items->Item[idx];
        lv->Items->Item[idx]->MakeVisible(false);
    }

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::tb_EditClick(TObject *Sender)
{
    if (_filterSetList == NULL) return;

	int idx = lv->ItemIndex;
	if (idx < 0) return;

    // Edit FilterSet:
    f_FilterSetEditor->filterSet = _filterSetList->at(idx);
    if (f_FilterSetEditor->ShowModal() != mrOk)
        return;
    _filterSetList->at(idx) = f_FilterSetEditor->filterSet;
    //(*fsList)[idx] = f_FilterSetEditor->filterSet;

    // Update GUI:
    redraw();

    // Set lv_FilterSets->ItemIndex to idx:
    lv->ItemIndex = idx;
    lv->ItemFocused = lv->Items->Item[idx];
    lv->Items->Item[idx]->MakeVisible(false);

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::tb_DuplicateClick(TObject *Sender)
{
    if (_filterSetList == NULL) return;

	int idx = lv->ItemIndex;
	if (idx < 0) return;

    // Duplicate FilterSet:
    f_FilterSetEditor->filterSet = _filterSetList->at(idx);
    f_FilterSetEditor->filterSet.name = _("Copy of") + " " + f_FilterSetEditor->filterSet.name;

    if (f_FilterSetEditor->ShowModal() != mrOk)
        return;

    // Insert duplicated (and possibly modified) FilterSet at current position [idx] +1:
    if ((unsigned)++idx < _filterSetList->size())
    {
        TAbaktFilterSet* insertPos = &_filterSetList->at(idx);
        _filterSetList->insert(insertPos, f_FilterSetEditor->filterSet);
    }
    else
    {
        _filterSetList->push_back(f_FilterSetEditor->filterSet);
    }

    // Update GUI:
    redraw();

    // Set lv_FilterSets->ItemIndex to idx of duplicated FilterSet:
    lv->ItemIndex = idx;
    lv->ItemFocused = lv->Items->Item[idx];
    lv->Items->Item[idx]->MakeVisible(false);

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::tb_CheckAllClick(TObject *Sender)
{
    if (_filterSetList == NULL) return;

	int idx = lv->ItemIndex; // Remember focussed filterSet.

    // Check all:
	for (unsigned int i = 0; i < _filterSetList->size(); i++)
        _filterSetList->at(i).enabled = true;

    // Update GUI:
    redraw();

    // Restore focussed filterSet (if any):
    if (idx != -1)
    {
        lv->ItemIndex = idx;
        lv->ItemFocused = lv->Items->Item[idx];
        lv->Items->Item[idx]->MakeVisible(false);
    }

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::tb_CheckNoneClick(TObject *Sender)
{
    if (_filterSetList == NULL) return;

	int idx = lv->ItemIndex; // Remember focussed filterSet.

    // Check all:
	for (unsigned int i = 0; i < _filterSetList->size(); i++)
        _filterSetList->at(i).enabled = false;

    // Update GUI:
    redraw();

    // Restore focussed filterSet (if any):
    if (idx != -1)
    {
        lv->ItemIndex = idx;
        lv->ItemFocused = lv->Items->Item[idx];
        lv->Items->Item[idx]->MakeVisible(false);
    }

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::tb_MoveUpClick(TObject *Sender)
{
    if (_filterSetList == NULL) return;

	int idx = lv->ItemIndex; // Remember focussed filterSet.

    // Move up:
    if (idx <= 0) return;
    TAbaktFilterSet fs1 = _filterSetList->at(idx);      // Selected FilterSet.
    TAbaktFilterSet fs2 = _filterSetList->at(idx - 1);  // Previous FilterSet.
    _filterSetList->at(idx) = fs2;
    _filterSetList->at(idx - 1) = fs1;

    // Update GUI:
    redraw();

    // Restore focussed filterSet:
    --idx;
    lv->ItemIndex = idx;
    lv->ItemFocused = lv->Items->Item[idx];
    lv->Items->Item[idx]->MakeVisible(false);

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::tb_MoveDownClick(TObject *Sender)
{
    if (_filterSetList == NULL) return;

	int idx = lv->ItemIndex;

    // Move down:
    if ((unsigned)idx >= _filterSetList->size() - 1) return;
    TAbaktFilterSet fs1 = _filterSetList->at(idx);      // Selected FilterSet.
    TAbaktFilterSet fs2 = _filterSetList->at(idx + 1);  // Next FilterSet.
    _filterSetList->at(idx) = fs2;
    _filterSetList->at(idx + 1) = fs1;

    // Update GUI:
    redraw();

    // Set lv_FilterSets->ItemIndex to idx:
    ++idx;
    lv->ItemIndex = idx;
    lv->ItemFocused = lv->Items->Item[idx];
    lv->Items->Item[idx]->MakeVisible(false);

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::tb_ClearClick(TObject *Sender)
{
    if (_filterSetList == NULL) return;

	if (MessageDlg( _("Remove all FilterSets?")
            + "\n\n" + _("This can not be undone."),
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0
		    ) != mrYes)
	    return;

    // Clear all:
    _filterSetList->clear();

    // Update GUI:
    redraw();

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::lvKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    switch (Key)
    {
        case VK_SPACE:
            _swapFilterSetProperty_enabled();   // Set FilterSet's enabled to !enabled.
            break;

        case VK_INSERT:
            tb_Add->Click();
            break;

        case VK_DELETE:
            tb_Remove->Click();
            break;

        case VK_F2:
            tb_Edit->Click();
            break;

        case 'A':
            if (Shift.Contains(ssCtrl | ssShift))
            {
                tb_CheckAll->Click();
                Key = 0;    // Required!
            }
            break;

        case 'N':
            if (Shift.Contains(ssCtrl | ssShift))
            {
                tb_CheckNone->Click();
                Key = 0;    // Required!
            }
            break;

        case VK_UP:
            if (Shift.Contains(ssCtrl))
            {
                tb_MoveUp->Click();
                Key = 0;    // Required!
            }
            break;

        case VK_DOWN:
            if (Shift.Contains(ssCtrl))
            {
                tb_MoveDown->Click();
                Key = 0;    // Required!
            }
            break;

        default:
            break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrameFilterSetList::lvMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	THitTests hitTest = lv->GetHitTestInfoAt(X, Y);
	if (Button == mbLeft)
	    if (hitTest.Contains(htOnStateIcon))    // htOnStateIcon: the state icon or bitmap associated with an item.
        {
            lv->ItemIndex = lv->GetItemAt(X, Y)->Index; // Make sure clicked item is selected.
            _swapFilterSetProperty_enabled();    // Set FilterSet's enabled to !enabled, and update Gui.
        }
}
//---------------------------------------------------------------------------

void TFrameFilterSetList::_swapFilterSetProperty_enabled()
{
    if (_filterSetList == NULL) return;

	int idx = lv->ItemIndex;
	if (idx < 0) return;

    // Edit FilterSet:
    _filterSetList->at(idx).enabled = !_filterSetList->at(idx).enabled;

    // Notify owner a modification was made:
    if (_onModified != NULL) _onModified();
}
//---------------------------------------------------------------------------

