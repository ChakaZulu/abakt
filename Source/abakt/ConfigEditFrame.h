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
#ifndef ConfigEditFrameH
#define ConfigEditFrameH
//---------------------------------------------------------------------------
#include "AbaktConfigItems.h"
#include "AbaktConfigSets.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFrameConfigEdit : public TFrame
{
__published:	// IDE-managed Components
    TGroupBox *gbx_ProfileProperty;
    TLabel *lbl_ItemCaption;
    TPanel *Panel5;
    TPanel *pnl_ItemValue;
    TScrollBox *sbox_ObjectInfo;
    TLabel *lbl_ObjectInfo;
    TPanel *Panel2;
    TLabel *lbl_ItemListCaption;
    TEdit *ed_InternalNameValue;
    TPanel *Panel1;
    TToolBar *tbar;
    TToolButton *tb_CheckAll;
    TToolButton *tb_CheckNone;
    TListView *lv;
    void __fastcall lvCustomDrawItem(TCustomListView *Sender,
          TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
    void __fastcall lvKeyPress(TObject *Sender, char &Key);
    void __fastcall lvMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall lvSelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
    void __fastcall tb_CheckAllClick(TObject *Sender);
    void __fastcall tb_CheckNoneClick(TObject *Sender);

private:	// User declarations

    void _makeList(abakt::TConfigSet* cfset);

    void _setModified();
    /*  Calls the OnModified callback. */

	void _clearItemControls();
	void _displayControl(abakt::TItem* item);
    void _displayInternalNameValue(abakt::TItem* item);

	void _displayBoolControl(abakt::TBool* item);
	void _displayUIntRangeControl(abakt::TUIntRange* item);
	void _displayTextControl(abakt::TText* item);
	void _displayPathControl(abakt::TPath* item);
	void _displayEnumControl(abakt::TEnum* item);

public:		// User declarations
    __fastcall TFrameConfigEdit(TComponent* Owner);

    void makeConfigList(abakt::TConfig& config);

	typedef void CALLBACK_OnModified(void);
	CALLBACK_OnModified* OnModified;

    void enableToolBar(bool enable) //092b8
    {
        tbar->Enabled = enable;
        for (int i = 0; i < tbar->ButtonCount; i++)
            tbar->Buttons[i]->Enabled = enable;
    }

	void __fastcall OnChangeBoolControl(TObject *Sender);
	void __fastcall OnChangeUIntRangeControl(TObject *Sender);
    void __fastcall SizeEditButtonClick(TObject *Sender);
	void __fastcall OnChangeTextControl(TObject *Sender);
	void __fastcall OnChangePathControl(TObject *Sender);
	void __fastcall PathBrowseButtonClick(TObject *Sender);
	void __fastcall OnChangeEnumControl(TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameConfigEdit *FrameConfigEdit;
//---------------------------------------------------------------------------
#endif
