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
#ifndef HistoryH
#define HistoryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class Tf_History : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TButton *bt_Ok;
    TPageControl *PageControl;
    TTabSheet *tab_ArchiveCatalogue;
    TListBox *lb;
    TOpenDialog *odlg_HistoryFiles;
    TGroupBox *gbx_History;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *lbl_HistInfo_MinBackups;
    TLabel *lbl_HistInfo_MaxBackups;
    TLabel *lbl_HistInfo_MaxDays;
    TEdit *ed_MaxDays;
    TUpDown *ud_MaxDays;
    TEdit *ed_MinBackups;
    TUpDown *ud_MinBackups;
    TEdit *ed_MaxBackups;
    TUpDown *ud_MaxBackups;
    TCheckBox *cb_UseRecycleBin;
    TCheckBox *cb_MaintainHistory;
    TLabel *lbl_HistoryInfo;
    TPanel *pnl_ToolBar;
    TToolBar *tbar_ArchiveCatalogue;
    TToolButton *tb_ShowFileList;
    TToolButton *ToolButton3;
    TToolButton *tb_AddFiles;
    TToolButton *ToolButton5;
    TToolButton *tb_ApplySettings;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall tb_ShowFileListClick(TObject *Sender);
    void __fastcall bt_AddFilesClick(TObject *Sender);
    void __fastcall ed_History_KeyPress(TObject *Sender, char &Key);
    void __fastcall ed_MaxDaysChange(TObject *Sender);
    void __fastcall ed_MinBackupsChange(TObject *Sender);
    void __fastcall ed_MaxBackupsChange(TObject *Sender);
    void __fastcall cb_UseRecycleBinClick(TObject *Sender);
    void __fastcall tb_ApplySettingsClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall cb_MaintainHistoryClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
    __fastcall Tf_History(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_History *f_History;
//---------------------------------------------------------------------------
#endif
