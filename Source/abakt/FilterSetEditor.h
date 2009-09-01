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
#ifndef FilterSetEditorH
#define FilterSetEditorH
//---------------------------------------------------------------------------
#include "AbaktFilterSet.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class Tf_FilterSetEditor : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnl_Properties;
    TEdit *ed_Name;
    TLabel *lbl_Name;
    TCheckBox *cb_Enabled;
    TLabel *lbl_Action;
    TGroupBox *gbx_Conditions;
    TPanel *pnl_Bottom;
    TButton *bt_Ok;
    TButton *bt_Cancel;
    TEdit *ed_PathMask;
    TLabel *lbl_PathMask;
    TEdit *ed_Size;
    TLabel *lbl_Size;
    TLabel *lbl_Age;
    TEdit *ed_Age;
    TLabel *lbl_Date;
    TEdit *ed_Date;
    TLabel *lbl_Attributes;
    TLabel *Label2;
    TButton *bt_Reset;
    TEdit *ed_Attributes;
    TButton *bt_Help;
    TComboBoxEx *cbex_Action;
    void __fastcall bt_OkClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall bt_CancelClick(TObject *Sender);
    void __fastcall bt_ResetClick(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall bt_HelpClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);

private:	// User declarations

    void _copyFilterSetToGui();

    void _copyGuiToFilterSet();

    bool _validateFilterSet();

public:		// User declarations
    __fastcall Tf_FilterSetEditor(TComponent* Owner);

    TAbaktFilterSet filterSet;
    /*  Both the initial TAbaktFilterSet and the resulting TAbaktFilterSet. */
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_FilterSetEditor *f_FilterSetEditor;
//---------------------------------------------------------------------------
#endif
