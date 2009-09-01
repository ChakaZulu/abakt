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
#ifndef FinalizeFrameH
#define FinalizeFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrameFinalize : public TFrame
{
__published:	// IDE-managed Components
    TScrollBox *ScrollBox_Finalize;
    TPanel *Panel1;
    TLabel *lbl_TestArchiveInfo;
    TCheckBox *cb_TestArchive;
    TPanel *Panel10;
    TLabel *lbl_RemoveEmptyDirsInfo;
    TCheckBox *cb_RemoveEmptyDirs;
    TGroupBox *gbx_History;
    TPanel *pnl_Descr_Finalize;
    TLabel *Label9;
    TButton *bt_History;
    TGroupBox *gbx_FileList;
    TCheckBox *cb_FileList_Enabled;
    TLabel *lbl_FileList_Enabled_Info;
    TLabel *lbl_HistoryEnabled;
	void __fastcall SetProfileModified(TObject *Sender);
    void __fastcall bt_HistoryClick(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TFrameFinalize(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameFinalize *FrameFinalize;
//---------------------------------------------------------------------------
#endif
