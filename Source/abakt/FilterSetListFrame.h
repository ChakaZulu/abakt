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
#ifndef FilterSetListFrameH
#define FilterSetListFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ToolWin.hpp>
#include "AbaktFilterSet.h"
//---------------------------------------------------------------------------
/*  This class maintains a visual display of a FilterSetList.
    All modifications are directly performed on the FilterSetList (_filterSetList).

    Be sure to call setFilterSetList() to inform this class
    of the FilterSetList it should use. */
//---------------------------------------------------------------------------
class TFrameFilterSetList : public TFrame
{
__published:	// IDE-managed Components
    TToolBar *tbar;
    TToolButton *tb_Add;
    TToolButton *tb_Remove;
    TToolButton *tb_Edit;
    TToolButton *tb_Duplicate;
    TToolButton *ToolButton4;
    TToolButton *tb_CheckAll;
    TToolButton *tb_CheckNone;
    TToolButton *ToolButton7;
    TToolButton *tb_MoveUp;
    TToolButton *tb_MoveDown;
    TToolButton *ToolButton12;
    TToolButton *tb_Clear;
    TListView *lv;
    void __fastcall tb_AddClick(TObject *Sender);
    void __fastcall tb_RemoveClick(TObject *Sender);
    void __fastcall tb_EditClick(TObject *Sender);
    void __fastcall tb_DuplicateClick(TObject *Sender);
    void __fastcall tb_CheckAllClick(TObject *Sender);
    void __fastcall tb_CheckNoneClick(TObject *Sender);
    void __fastcall tb_MoveUpClick(TObject *Sender);
    void __fastcall tb_MoveDownClick(TObject *Sender);
    void __fastcall tb_ClearClick(TObject *Sender);
    void __fastcall lvKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall lvMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);

private:	// User declarations

    typedef void CALLBACK_OnModified(void);
    /*  Defines a callback-function to be called when the FilterSetList has changed
        or when an item in the FilterSetList has changed. */

    CALLBACK_OnModified* _onModified;
    /*  Pointer to an onModified callback function, provided by the owener of this class.
        The pointer can be NULL. */

    std::vector<TAbaktFilterSet>* _filterSetList;

    void _swapFilterSetProperty_enabled();
    /*  Sets property 'enabled' of the selected FilterSet to !enabled.
        Fires an onChange event. */

public:		// User declarations

    __fastcall TFrameFilterSetList(TComponent* Owner);

    void setFilterSetList(std::vector<TAbaktFilterSet> *list);
    /*  Sets _filterSetList to [list],
        and populates the ListView with the items in [list].
        Does not fire an onChange event. */

    void setOnModifiedCallback(CALLBACK_OnModified* onModifiedCallback)
    {   _onModified = onModifiedCallback; }

    void redraw();
    /*  Redraws the ListView with the items in _filterSetList.
        Does not cause an onChange event. */

    void enableToolBar(bool enable) //092b8
    {
        tbar->Enabled = enable;
        for (int i = 0; i < tbar->ButtonCount; i++)
            tbar->Buttons[i]->Enabled = enable;
    }
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameFilterSetList *FrameFilterSetList;
//---------------------------------------------------------------------------
#endif
