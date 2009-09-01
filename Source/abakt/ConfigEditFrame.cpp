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

#include "ConfigEditFrame.h"
#include "DataModule.h"
#include "FileSizeEditor.h"
#include "eccBrowseForDirectory.h"
#include "eccString.h"
#include "AbaktGlobals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameConfigEdit *FrameConfigEdit;
//---------------------------------------------------------------------------
static const int DEF_MARGIN_Y = 8;
static const int DEF_MARGIN_X = 8;
//---------------------------------------------------------------------------
__fastcall TFrameConfigEdit::TFrameConfigEdit(TComponent* Owner)
    : TFrame(Owner)
{
    OnModified = NULL;
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_setModified()
{
    if (OnModified)
        OnModified();
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::lvCustomDrawItem(
      TCustomListView *Sender, TListItem *Item, TCustomDrawState State,
      bool &DefaultDraw)
{
	abakt::TItem* item = (abakt::TItem*)Item->Data;
	if (item == NULL)
	{
        // -- TConfigSet.items.name
		DefaultDraw = false;
		TRect Rect = Item->DisplayRect(drBounds);
		Sender->Canvas->Brush->Color = (TColor)0xf0f0f0;
		Sender->Canvas->Font->Color = (TColor)0x202020;
		Sender->Canvas->Font->Style = Sender->Canvas->Font->Style << fsBold;
		Sender->Canvas->FillRect(Rect); // Clear the rectangle.
		if (Item->Selected)
		{
            // Draw frame:
			Sender->Canvas->Brush->Color = (TColor)0xC08080; // clNavy;
			Sender->Canvas->FrameRect(Rect); // Draw a frame.
		}
		// Display text:
		Sender->Canvas->TextOut(Rect.Left + 2, Rect.Top + 2, " " + Item->Caption);
	}
	else
	{
        // -- ConfigSet Name
		DefaultDraw = true;
		Item->StateIndex = item->override ? ICON_CHECKED : ICON_UNCHECKED;
		TRect Rect = Item->DisplayRect(drLabel);
		Sender->Canvas->Brush->Color = clWindow;
		Sender->Canvas->Font->Color = clBlack;
		//Sender->Canvas->Font->Color = clWindowText;  - ERIK: Why large font???
		Sender->Canvas->FillRect(Rect); // Clear the rectangle.
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::lvKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_SPACE)
	{
		TListItem* litem = lv->Selected;
		abakt::TItem* item = (abakt::TItem*)litem->Data;
		if (item)
		{
			item->override = !item->override;
			litem->StateIndex = item->override ? ICON_CHECKED : ICON_UNCHECKED;
			_setModified();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::lvMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	THitTests hit_test = lv->GetHitTestInfoAt(X,Y);
	if (hit_test.Contains(htOnStateIcon))
	{
		TListItem* litem = lv->GetItemAt(X, Y);
		lv->Selected = litem;
		lv->ItemFocused = litem;
		abakt::TItem* item = (abakt::TItem*)litem->Data;
		if (item)
		{
			item->override = !item->override;
			litem->StateIndex = item->override ? ICON_CHECKED : ICON_UNCHECKED;
			_setModified();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::lvSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
	if (!Selected) return;
	abakt::TItem* item = (abakt::TItem*)Item->Data;

	_displayControl(item);
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::tb_CheckAllClick(TObject *Sender)
{
    TListItem* litem;
    abakt::TItem* item;
    for (int i = 0; i < lv->Items->Count; i++)
    {
        litem = lv->Items->Item[i];
        if (!litem) continue;
		item = (abakt::TItem*)litem->Data;
        if (!item) continue;
        item->override      = true;
        litem->StateIndex   = ICON_CHECKED;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::tb_CheckNoneClick(TObject *Sender)
{
    TListItem* litem;
    abakt::TItem* item;
    for (int i = 0; i < lv->Items->Count; i++)
    {
        litem = lv->Items->Item[i];
        if (!litem) continue;
		item = (abakt::TItem*)litem->Data;
        if (!item) continue;
        item->override      = false;
        litem->StateIndex   = ICON_UNCHECKED;
    }
}
//---------------------------------------------------------------------------

//===========================================================================
//	Profile Config List & Edit functions
//===========================================================================

//---------------------------------------------------------------------------

void TFrameConfigEdit::makeConfigList(abakt::TConfig& config)
{
	lv->Clear();

    lv->Items->BeginUpdate();
	_makeList(config.Action);
	_makeList(config.Actions.Zip);
	_makeList(config.Actions.SevenZip);
	_makeList(config.Actions.File);
	_makeList(config.Destination);
	_makeList(config.History);
	_makeList(config.FileList);
    lv->Items->EndUpdate();

    // Set focus to item 1 (not 0, which is a header):
	lv->Selected	= lv->Items->Item[1];
	lv->ItemFocused	= lv->Items->Item[1];
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_makeList(abakt::TConfigSet* cfset)
{
    abakt::TItemList* itemlist = cfset->items;
	abakt::TItem* item;
	TListItem* litem;

	litem = lv->Items->Add();
	litem->Caption = cfset->caption + " :: " + itemlist->caption;
	litem->StateIndex = -1; // No icon for the header.

	for (unsigned int i = 0; i < itemlist->size(); i++)
	{
		item = itemlist->at(i);
		if (!item) continue;

		// Create list:
		litem = lv->Items->Add();
		litem->Data = (void*)item; // Abuse Data to store pointer to (descendant of) abakt::TObject.
		litem->StateIndex = item->override ? ICON_CHECKED : ICON_UNCHECKED;

		litem->Caption = _(item->caption); // TRANSLATED!

        switch (item->type())
        {
            // Translated value:
            case abakt::dtBool:
            case abakt::dtEnum:
                litem->SubItems->Add( _(abakt::ItemValueToStr(item)) ); // TRANSLATED!
                break;

            // Not translated value:
        	case abakt::dtUIntRange:
            case abakt::dtText:
            case abakt::dtPath:
                litem->SubItems->Add( abakt::ItemValueToStr(item) );
                break;

            default:
                litem->SubItems->Add( "Huh?: " __FUNC__ );
                break;
        }
        // ItemType is not translated:
		litem->SubItems->Add( abakt::ItemTypeToStr(item->type()) );
	}
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_clearItemControls()
{
	TComponent* comp;
	// Must do in reverse order:
	for (int i = pnl_ItemValue->ComponentCount - 1; i >= 0; i--)
	{
		comp = pnl_ItemValue->Components[i];
		pnl_ItemValue->RemoveComponent(comp);
		delete comp;
	}

	pnl_ItemValue->Height	= DEF_MARGIN_Y;
	pnl_ItemValue->Left		= DEF_MARGIN_X;
	pnl_ItemValue->Width	= pnl_ItemValue->Parent->Width - (2 * DEF_MARGIN_X);
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_displayControl(abakt::TItem* item)
{
	if (!item)
	{
        ed_InternalNameValue->Text  = "";
		lbl_ItemCaption->Caption    = "";
		lbl_ObjectInfo->Caption     = "";
		_clearItemControls();

		TListItem* litem = lv->Selected;
		if (litem)
            lbl_ItemCaption->Caption = "< " + litem->Caption + " >";
		return;
	}

	// Caption:
    #ifdef ABT_TEST
        if (item->owner == NULL) ShowMessage(__FUNC__": item->owner can not be NULL");
        if (item->owner->owner == NULL) ShowMessage(__FUNC__": item->owner->owner can not be NULL");
    #endif
	lbl_ItemCaption->Caption = item->caption;
    _displayInternalNameValue(item);

	// Info:
	lbl_ObjectInfo->Caption = item->info;

	// Data / Type:
	switch (item->type())
	{
		case abakt::dtBool:
			_displayBoolControl((abakt::TBool*)item);
			break;

		case abakt::dtUIntRange:
			_displayUIntRangeControl((abakt::TUIntRange*)item);
			break;

		case abakt::dtText:
			_displayTextControl((abakt::TText*)item);
			break;

		case abakt::dtPath:
			_displayPathControl((abakt::TPath*)item);
			break;

		case abakt::dtEnum:
			_displayEnumControl((abakt::TEnum*)item);
			break;

		default:
			ShowMessage( __FUNC__ " : Unknown object type!" );
			break;
	}
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_displayInternalNameValue(abakt::TItem* item)
{
    String configsetName = item->owner->owner->name;
    if (!configsetName.IsEmpty())
        configsetName = configsetName + ".";
    ed_InternalNameValue->Text = configsetName + item->owner->name + "." + item->name
        + "=" + ecc::QuoteStr(abakt::ItemValueToStr(item));
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_displayBoolControl(abakt::TBool* item)
{
	_clearItemControls();
	TPanel* pnl = pnl_ItemValue;

	TCheckBox* cb   = new TCheckBox(pnl);
	cb->Left        = DEF_MARGIN_X;
	cb->Top         = pnl->Height;
	cb->Width       = pnl->Width - cb->Left - DEF_MARGIN_X;
	cb->Anchors     = cb->Anchors << akLeft << akTop << akRight;
	cb->Caption     = item->caption;
	cb->Checked     = item->value;
	cb->Parent  	= pnl;	// Make control visible. Necessary!
	cb->OnClick     = OnChangeBoolControl;

	pnl->Height = pnl->Height + cb->Height + DEF_MARGIN_Y;
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::OnChangeBoolControl(TObject *Sender)
{
	int idx = lv->ItemIndex;
	if (idx == -1) return;
	TListItem* litem = lv->Items->Item[idx];
	abakt::TBool* item = (abakt::TBool*)litem->Data;
	if (!item) return;

	TCheckBox* cb = (TCheckBox*)Sender;
	item->value = cb->Checked;

	// Update listview:
	litem->SubItems->Strings[0] = _( abakt::ItemValueToStr((abakt::TItem*)item) ); // TRANSLATED!
    _displayInternalNameValue(item);
	_setModified();
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_displayUIntRangeControl(abakt::TUIntRange* item)
{
	_clearItemControls();
	TPanel* pnl = pnl_ItemValue;

	TEdit* ed	    = new TEdit(pnl);
	ed->Top	    	= pnl->Height;
	ed->Left	    = DEF_MARGIN_X;
	ed->Text    	= IntToStr(item->get());
	ed->Width	    = pnl->Width - ed->Left - DEF_MARGIN_X;
	ed->Anchors	    = ed->Anchors << akLeft << akTop << akRight;
	ed->OnChange    = OnChangeUIntRangeControl;
	ed->Parent	    = pnl;	// Make control visible. Necessary!

    if (item->getMax() > 32767)
    {   // Add a button to open the FileSize editor:
        TButton* bt     = new TButton(pnl);
        bt->Top 	    = pnl->Height - 2;
        bt->Left 	    = DEF_MARGIN_X;
        ed->Left 	    = bt->Left + bt->Width + DEF_MARGIN_X;
        bt->Caption     = _("Edit");
        bt->OnClick     = SizeEditButtonClick;
        bt->Parent 	    = pnl;	// Make control visible. Necessary!
	    bt->Tag 	    = (int)ed;	// Abuse Tag to store pointer to TEdit control.
    	ed->Width 	    = pnl->Width - ed->Left - DEF_MARGIN_X;
        bt->TabOrder    = 0;
    }
    else
    {
        TUpDown* ud     = new TUpDown(pnl);
        ud->Position    = item->get();
        ud->Min         = item->getMin();
        ud->Max         = item->getMax();
        ud->Thousands   = false;
        ud->Parent 	    = pnl;	// Make control visible. Necessary!
        ud->Associate   = ed;   // Do this after setting the parent.
        ud->AlignButton = udRight;
        ud->Left 	    = pnl->Width - ud->Width - DEF_MARGIN_X;
    	ed->Width 	    = ud->Left - ed->Left - DEF_MARGIN_X;
    }

	pnl->Height	= pnl->Height + ed->Height + DEF_MARGIN_Y;
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::SizeEditButtonClick(TObject *Sender)
{
	int idx = lv->ItemIndex;
	if (idx == -1) return;
	TListItem* litem = lv->Items->Item[idx];
	abakt::TUIntRange* item = (abakt::TUIntRange*)litem->Data;
	if (!item) return;

    TButton* bt = (TButton*)Sender;
    TEdit* ed = dynamic_cast<TEdit*>((TEdit*)bt->Tag);
	if (!ed) return; // !error

	f_FileSizeEditor->value = StrToIntDef(ed->Text, 0);
    f_FileSizeEditor->customTitle( item->caption );
	if (f_FileSizeEditor->ShowModal() == mrOk)
	{
		__int64 val = f_FileSizeEditor->value;
		if (val < item->getMin()) val = item->getMin();
		if (val > item->getMax()) val = item->getMax();
		ed->Text = IntToStr(val);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::OnChangeUIntRangeControl(TObject *Sender)
{
	int idx = lv->ItemIndex;
	if (idx == -1) return;
	TListItem* litem = lv->Items->Item[idx];
	abakt::TUIntRange* item = (abakt::TUIntRange*)litem->Data;
	if (!item) return;

	TEdit* ed = (TEdit*)Sender;
	unsigned long val = StrToIntDef(ed->Text, item->get());
	val = item->set(val);
	ed->Text = IntToStr(val);

	// Update listview:
	litem->SubItems->Strings[0] = abakt::ItemValueToStr((abakt::TItem*)item);
    _displayInternalNameValue(item);
	_setModified();
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_displayTextControl(abakt::TText* item)
{
	_clearItemControls();
	TPanel* pnl = pnl_ItemValue;

	TEdit* ed	    = new TEdit(pnl);
    if (item->masked) ed->PasswordChar = '*';
	ed->Top	    	= pnl->Height;
	ed->Left	    = DEF_MARGIN_X;
	ed->Text    	= item->value;
	ed->Width	    = pnl->Width - ed->Left - DEF_MARGIN_X;
	ed->Anchors	    = ed->Anchors << akLeft << akTop << akRight;
	ed->OnChange    = OnChangeTextControl;
	ed->Parent	    = pnl;	// Make control visible. Necessary!

	pnl->Height	= pnl->Height + ed->Height + DEF_MARGIN_Y;
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::OnChangeTextControl(TObject *Sender)
{
	int idx = lv->ItemIndex;
	if (idx == -1) return;
	TListItem* litem = lv->Items->Item[idx];
	abakt::TText* item = (abakt::TText*)litem->Data;
	if (!item) return;

	TEdit* ed = (TEdit*)Sender;
	item->value = ed->Text;

	// Update listview:
	litem->SubItems->Strings[0] = abakt::ItemValueToStr((abakt::TItem*)item);
    _displayInternalNameValue(item);
	_setModified();
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_displayPathControl(abakt::TPath* item)
{
	_clearItemControls();
	TPanel* pnl = pnl_ItemValue;

	TButton* bt     = new TButton(pnl);
	bt->Top 	    = pnl->Height - 2;
	bt->Left 	    = pnl->Width - bt->Width - DEF_MARGIN_X;
	bt->Caption     = _("Browse");
	bt->Anchors     = bt->Anchors >> akLeft << akTop << akRight;
	bt->OnClick     = PathBrowseButtonClick;
	bt->Parent 	    = pnl;	// Make control visible. Necessary!

	TEdit* ed	    = new TEdit(pnl);
	ed->Top		    = pnl->Height;
	ed->Left	    = DEF_MARGIN_X;
	ed->Text	    = item->value;
	ed->Width 	    = bt->Left - ed->Left - DEF_MARGIN_X;
	ed->Anchors	    = ed->Anchors << akLeft << akTop << akRight;
	ed->OnChange    = OnChangePathControl;
	ed->Parent	    = pnl;	// Make control visible. Necessary!

	bt->Tag 	    = (int)ed;	// Abuse Tag to store pointer to TEdit control.

	pnl->Height	= pnl->Height + ed->Height + DEF_MARGIN_Y;
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::PathBrowseButtonClick(TObject *Sender)
{
	int idx = lv->ItemIndex;
	if (idx == -1) return;
	TListItem* litem = lv->Items->Item[idx];
	abakt::TPath* item = (abakt::TPath*)litem->Data;
	if (!item) return;

	ecc::TBrowseForDirectory bfd(PGlobals->UseNewUIOnBFD);
	bfd.Handle 		= this->Handle;		// If not set Handle will be Application->Handle.
	bfd.Title 		= _("Choose directory:");
	bfd.Directory	= item->value; 		// Initial directory.
	if (bfd.Execute())
	{
		TButton* bt = (TButton*)Sender;
		TEdit* ed = dynamic_cast<TEdit*>((TEdit*)bt->Tag);
		if (ed)
			ed->Text = IncludeTrailingPathDelimiter(bfd.Directory); // ed will call BrowseDirChange.
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::OnChangePathControl(TObject *Sender)
{
	int idx = lv->ItemIndex;
	if (idx == -1) return;
	TListItem* litem = lv->Items->Item[idx];
	abakt::TPath* item = (abakt::TPath*)litem->Data;
	if (!item) return;

	TEdit* ed = (TEdit*)Sender;
	item->value = ed->Text;

	// Update listview:
	litem->SubItems->Strings[0] = abakt::ItemValueToStr((abakt::TItem*)item);
    _displayInternalNameValue(item);
	_setModified();
}
//---------------------------------------------------------------------------

void TFrameConfigEdit::_displayEnumControl(abakt::TEnum* item)
{
	_clearItemControls();
	TPanel* pnl = pnl_ItemValue;

	TComboBox* cmb = new TComboBox(pnl);
	cmb->Top 	    = pnl->Height;
	cmb->Left 	    = DEF_MARGIN_X;
	cmb->Width 	    = pnl->Width - cmb->Left - DEF_MARGIN_X;
	cmb->Anchors    = cmb->Anchors << akLeft << akTop << akRight;
	cmb->Style 	    = csDropDownList;
	cmb->Parent     = pnl;	// Make control visible. Necessary!
	for (unsigned int i = 0; i < item->size(); i++)
		cmb->Items->Append( _(item->at(i)) ); // translate!
	cmb->ItemIndex  = item->getIndex();
	cmb->OnChange   = OnChangeEnumControl;

	pnl->Height	= pnl->Height + cmb->Height + DEF_MARGIN_Y;
}
//---------------------------------------------------------------------------

void __fastcall TFrameConfigEdit::OnChangeEnumControl(TObject *Sender)
{
	int idx = lv->ItemIndex;
	if (idx == -1) return;
	TListItem* litem = lv->Items->Item[idx];
	abakt::TEnum* item = (abakt::TEnum*)litem->Data;
	if (!item) return;

	TComboBox* cmb = (TComboBox*)Sender;
	item->setIndex(cmb->ItemIndex);

	// Update listview:
	litem->SubItems->Strings[0] = _( abakt::ItemValueToStr((abakt::TItem*)item) ); // TRANSLATED!
    _displayInternalNameValue(item);
	_setModified();
}
//---------------------------------------------------------------------------

