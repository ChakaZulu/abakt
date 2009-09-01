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
#ifndef RestoreBackupH
#define RestoreBackupH
//---------------------------------------------------------------------------
#include <ActnList.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include "eccString.h"
#include "ZipBuilder.h"
//---------------------------------------------------------------------------
class Tf_RestoreBackup : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnl_Top;
	TActionList *ActionList;
	TAction *act_Abort;
	TAction *act_Close;
	TAction *act_SaveLog;
	TSaveDialog *sdlg_Log;
	TAction *act_RestoreFiles;
	TOpenDialog *odlg_Backup;
	TCheckBox *cb_Overwrite;
	TGroupBox *gbx_ZipFiles;
	TListView *lv_Files;
	TSplitter *Splitter;
	TPopupMenu *pop_Files;
	TMenuItem *mi_SelectAll;
	TAction *act_SelectAllFiles;
	TButton *Button2;
    TButton *bt_ZipArchive;
	TPanel *pnl_Bottom;
    TGroupBox *gbx_Log;
	TListBox *lb_Log;
	TGroupBox *gbx_Progress;
	TButton *bt_Abort;
	TPanel *pnl_ProfileProgress;
	TLabel *Label4;
	TLabel *lbl_ProfilePerc;
	TProgressBar *pb_Profile;
	TPanel *Panel1;
	TButton *bt_Close;
	TButton *Button1;
	TEdit *ed_Password;
    TZipBuilder *ZipBuilder;
    TPanel *Panel2;
    TLabel *Label1;
    TLabel *lbl_FilePerc;
    TProgressBar *pb_File;
    TLabel *lbl_ProgressInfo;
    TButton *bt_ListFiles;
    TComboBox *cmbx_ZipArchiveFilename;
    TLabel *lbl_ArchiveDetails;
    TBevel *Bevel2;
    TLabel *lbl_Password;
    TGroupBox *gbx_DestDir;
    TEdit *ed_DestDir;
    TButton *bt_DestDir;
	void __fastcall act_CloseExecute(TObject *Sender);
	void __fastcall act_SaveLogExecute(TObject *Sender);
	void __fastcall act_AbortExecute(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall act_RestoreFilesExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall act_SelectAllFilesExecute(TObject *Sender);
    void __fastcall bt_ZipArchiveClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall bt_DestDirClick(TObject *Sender);
    void __fastcall ZipBuilderProgressDetails(TObject *Sender,
          TProgressDetails *Details);
    void __fastcall ZipBuilderMessage(TObject *Sender, int ErrCode,
          AnsiString Message);
    void __fastcall bt_ListFilesClick(TObject *Sender);
    void __fastcall lb_LogDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
    void __fastcall ZipBuilderGetNextDisk(TObject *Sender, int DiskSeqNo,
          int DiskTotal, AnsiString Drive, bool &Cancel);
    void __fastcall cmbx_ZipArchiveFilenameChange(TObject *Sender);

private:	// User declarations
	bool _aborted;
	bool _busy;

    bool _driveIsRemovable;
    /*  Required to be up-to-date when Extract starts. */

    void _enableControls(bool enable);

    void _listFilesInArchive(String fileName);
  	/*  Displays files in archive [fileName] */

    void _displayArchiveDetails(String fileName);

    void _addArchiveListToComboBox();

    String _stripDestDir(String fileName);
    /*  Returns fileName without preceding ed_DestDir->Text information. */

    void _doLog(const String msg, const ecc::TLBMessageStyle style,
		    const int lines_before = 0, const int lines_after = 0) const;
	void log(String msg, int lines_before = 0, int lines_after = 0) const;

//	String _toAttrStr(unsigned int attr);

public:		// User declarations
	__fastcall Tf_RestoreBackup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_RestoreBackup *f_RestoreBackup;
//---------------------------------------------------------------------------
#endif
