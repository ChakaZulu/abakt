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

#include "FileSizeEditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tf_FileSizeEditor *f_FileSizeEditor;
//---------------------------------------------------------------------------
__fastcall Tf_FileSizeEditor::Tf_FileSizeEditor(TComponent* Owner)
	: TForm(Owner)
{
	_displaying = false;
    _orgCaption = Caption;
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
    #endif
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::FormShow(TObject *Sender)
{
	_displayValue();
    ActiveControl = ed_kB;
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ed_B->Enabled = true;
    Caption = _orgCaption;
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::ed_SizeKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_BACK) return;
	if ((Key < '0') || (Key > '9')) Key = NULL;
}
//---------------------------------------------------------------------------

void Tf_FileSizeEditor::_displayValue()
{
	if (_displaying) return;

	_displaying = true;
	if (value < 0) value = 0x7FFFFFFF;
	ed_B->Text = IntToStr(value);
	ed_kB->Text = FloatToStrF(value/1024, ffFixed, 7, 0);
	ed_MB->Text = FloatToStrF((value/1024)/1024, ffFixed, 7, 0);
	_displaying = false;
}
//---------------------------------------------------------------------------

void Tf_FileSizeEditor::_setValue(__int64 val)
{
	if (_displaying) return;
	value = val;
	_displayValue();
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::ed_BKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	if (Key == VK_UP)   _setValue(++value);
	if (Key == VK_DOWN) _setValue(--value);
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::ed_kBKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	if (Key == VK_UP)   _setValue(value + 1024);
	if (Key == VK_DOWN) _setValue(value - 1024);
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::ed_MBKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	if (Key == VK_UP)   _setValue(value + (1024*1024));
	if (Key == VK_DOWN) _setValue(value - (1024*1024));
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::ed_BChange(TObject *Sender)
{
	_setValue( StrToIntDef(ed_B->Text, 0) );
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::ed_kBChange(TObject *Sender)
{
	_setValue( 1024 * StrToIntDef(ed_kB->Text, 0) );
}
//---------------------------------------------------------------------------

void __fastcall Tf_FileSizeEditor::ed_MBChange(TObject *Sender)
{
	_setValue( (1024*1024) * StrToIntDef(ed_MB->Text, 0) );
}
//---------------------------------------------------------------------------

