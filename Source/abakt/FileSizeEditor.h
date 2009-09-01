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
#ifndef FileSizeEditorH
#define FileSizeEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tf_FileSizeEditor : public TForm
{
__published:	// IDE-managed Components
	TEdit *ed_B;
	TEdit *ed_kB;
	TEdit *ed_MB;
	TLabel *lbl_B;
	TLabel *lbl_kB;
	TLabel *lbl_MB;
	TLabel *Label1;
    TPanel *Panel1;
    TButton *bt_Ok;
    TButton *bt_Cancel;
	void __fastcall ed_SizeKeyPress(TObject *Sender, char &Key);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ed_BKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall ed_kBKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall ed_MBKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall ed_BChange(TObject *Sender);
	void __fastcall ed_kBChange(TObject *Sender);
	void __fastcall ed_MBChange(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);

private:	// User declarations

    AnsiString _orgCaption;
    /*  Original TForm's caption. */ 

	bool _displaying;
	/*	If true, prevents nested calls to _displayValue()
		and prevents OnChange events. */

	void _displayValue();
	/*	Displays the text values for B, kB and MB.
		Does nothing when _displaying is true. */

	void _setValue(__int64 val);
	/*	Sets Value to [val] and calls _displayValue(). */

public:		// User declarations

	__fastcall Tf_FileSizeEditor(TComponent* Owner);

    void customTitle(AnsiString title)
    {   Caption = title; }
    /*  Displays custom form caption [title].
        Original caption is restored when form closes. */

	__int64 value;  // 'The' value (in bytes).
    /*  Set the value before showing this form.
        Read this value when the form has been closed. */
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_FileSizeEditor *f_FileSizeEditor;
//---------------------------------------------------------------------------
#endif
