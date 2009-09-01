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
#ifndef GroupH
#define GroupH
//---------------------------------------------------------------------------
#include "ConfigEditFrame.h"
#include "RecentFiles.h"
#include "AbaktConfigItems.h"
#include <ActnList.hpp>
#include <CheckLst.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
#include "FilterSetListFrame.h"
#include <fstream>
//---------------------------------------------------------------------------
class Tf_Group : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TActionList *ActionList;
    TAction *act_FileSave;
    TAction *act_FileOpen;
    TAction *act_FileSaveAs;
    TAction *act_FileNew;
	TMenuItem *mu_File;
    TMenuItem *mi_FileNew;
    TMenuItem *mi_FileOpen;
    TMenuItem *mi_FileSave;
	TMenuItem *SaveProfileGroupAs1;
	TMenuItem *N1;
	TMenuItem *mi_Close;
	TOpenDialog *odlg_Group;
	TSaveDialog *sdlg_Group;
	TOpenDialog *odlg_Profiles;
	TAction *act_ExecuteGroup;
	TPageControl *PageControl;
	TTabSheet *tab_Profiles;
	TMenuItem *N3;
	TMenuItem *mi_StartGroupBackup;
	TMenuItem *mu_Tools;
    TAction *act_EditProfiles;
    TAction *act_EditCustomConfiguration;
	TMenuItem *mu_View;
	TMenuItem *N1Profiles1;
	TMenuItem *N3BackupType1;
	TAction *act_ViewActionLog;
	TMenuItem *BackupLog1;
	TMenuItem *N4;
	TMenuItem *mi_RecentFiles;
    TTabSheet *tab_CustomConfiguration;
    TPanel *pnl_Descr_CustomConfiguration;
    TLabel *Label2;
    TAction *act_EditCustomFilterSets;
    TMenuItem *CustomFilterSets1;
    TTabSheet *tab_CustomFilterSets;
    TPanel *pnl_Descr_CustomFilterSets;
    TLabel *Label3;
    TPanel *pnl_Descr_Profiles;
    TLabel *Label1;
    TToolBar *tbar_Profiles;
    TToolButton *tb_ProfileAdd;
    TToolButton *tb_ProfileRemove;
    TToolButton *tb_ProfileEdit;
    TToolButton *ToolButton4;
    TToolButton *tb_ProfileAll;
    TToolButton *tb_ProfileNone;
    TToolButton *ToolButton7;
    TToolButton *tb_ProfileUp;
    TToolButton *tb_ProfileDown;
    TToolButton *tb_ProfileSort;
    TToolButton *ToolButton12;
    TToolButton *tb_ProfileClear;
    TCheckListBox *clb_Profiles;
    TPanel *pnl_ToolBars;
    TPanel *Panel4;
    TToolBar *ToolBar1;
    TToolButton *tb_FileNew;
    TToolButton *tb_FileOpen;
    TToolButton *tb_FileSave;
    TToolButton *ToolButton5;
    TPanel *pnl_TabsToolbar;
    TToolBar *ToolBar2;
    TToolButton *tb_ViewProfiles;
    TToolButton *tb_ViewCustomFilterSets;
    TToolButton *tb_ViewCustomConfiguration;
    TFrameConfigEdit *fr_CustomConfigEdit;
    TToolButton *ToolButton1;
    TPanel *pnl_ExecuteButton;
    TToolBar *tbar_ExecuteButton;
    TToolButton *ToolButton2;
    TToolButton *ToolButton3;
    TFrameFilterSetList *fr_CustomFilterSetList;
    TMenuItem *mu_Edit;
    TAction *act_Manual;
    TMenuItem *mu_Help;
    TMenuItem *Manual1;
	void __fastcall mi_CloseClick(TObject *Sender);
	void __fastcall tb_ProfileAddClick(TObject *Sender);
	void __fastcall tb_ProfileRemoveClick(TObject *Sender);
	void __fastcall tb_ProfileAllClick(TObject *Sender);
	void __fastcall tb_ProfileNoneClick(TObject *Sender);
	void __fastcall act_FileSaveExecute(TObject *Sender);
	void __fastcall act_FileOpenExecute(TObject *Sender);
	void __fastcall act_FileNewExecute(TObject *Sender);
	void __fastcall act_FileSaveAsExecute(TObject *Sender);
	void __fastcall act_ExecuteGroupExecute(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SetGroupModified(TObject *Sender);
	void __fastcall act_EditProfilesExecute(TObject *Sender);
	void __fastcall act_EditCustomConfigurationExecute(TObject *Sender);
	void __fastcall act_ViewActionLogExecute(TObject *Sender);
	void __fastcall mi_RecentFilesClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall tb_ProfileUpClick(TObject *Sender);
    void __fastcall tb_ProfileDownClick(TObject *Sender);
    void __fastcall tb_ProfileClearClick(TObject *Sender);
    void __fastcall tb_ProfileEditClick(TObject *Sender);
    void __fastcall tb_ProfileSortClick(TObject *Sender);
    void __fastcall tab_CustomConfigurationShow(TObject *Sender);
    void __fastcall tab_ProfilesShow(TObject *Sender);
    void __fastcall act_EditCustomFilterSetsExecute(TObject *Sender);
    void __fastcall tab_CustomFilterSetsShow(TObject *Sender);
    void __fastcall clb_ProfilesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall act_ManualExecute(TObject *Sender);

private:	// User declarations

	String ORG_Caption;

	void _clearGroup();
	void _setGroupName(String name);

	bool _confirmOpenGroupInGUI();
	void _openGroupInGUI(String fpath);

	static void OnSelectRecentFile(String fpath);
	/* 	Callback function for TRecentFiles.OnSelect. */

	static void OnConfigModified(void);
	/* 	Callback function for TFrameConfigEdit::OnModified. */

    void _swapClbItems(TCheckListBox* clb, const int idx0, const int idx1);

    void _guiToGroup();
    void _groupToGui();

public:		// User declarations
	__fastcall Tf_Group(TComponent* Owner);

	TRecentFiles* recentFiles;

	bool openGroup(String fpath);

	void setModified(bool modified);
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_Group *f_Group;
//---------------------------------------------------------------------------
#endif

