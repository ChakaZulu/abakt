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
#ifndef ActionTypeFrameH
#define ActionTypeFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrameActionType : public TFrame
{
__published:	// IDE-managed Components
    TScrollBox *ScrollBox_ActionType;
    TPanel *pnl_Descr_ActionType;
    TLabel *Label15;
    TGroupBox *gbx_ActionType;
    TLabel *Label1;
    TLabel *Label2;
    TRadioButton *rb_File_Copy;
    TRadioButton *rb_Zip;
    TRadioButton *rb_7Zip;
    TRadioButton *rb_File_Delete;
    TRadioButton *rb_File_Move;
    TGroupBox *gbx_ArchiveMode;
    TLabel *Label16;
    TLabel *Label29;
    TLabel *Label32;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *lbl_ArchiveModeInfo;
    TLabel *Label3;
    TRadioButton *rb_AMode_Default;
    TRadioButton *rb_AMode_Full;
    TRadioButton *rb_AMode_Differential;
    TRadioButton *rb_AMode_Incremental;
    TRadioButton *rb_AMode_Inverse;
    TPanel *Panel2;
    TCheckBox *cb_IncludeEmptySourceDirectories;
	void __fastcall SetModified(TObject *Sender);
	void __fastcall ModeModified(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TFrameActionType(TComponent* Owner);
	
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameActionType *FrameActionType;
//---------------------------------------------------------------------------
#endif
