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
#ifndef PreferencesH
#define PreferencesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class Tf_Prefs : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl;
	TTabSheet *tab_Startup;
	TTabSheet *tab_Application;
	TOpenDialog *odlg_7ZipExe;
    TCheckBox *cb_CreateFileAssociations;
	TRadioGroup *rb_InitialProfileType;
	TTabSheet *tab_Logging;
	TTabSheet *tab_Backup;
	TButton *bt_SelectCustomInitialProfile;
	TEdit *ed_CustomInitialProfile;
	TOpenDialog *odlg_InitialProfile;
	TLabel *Label4;
	TCheckBox *cb_ActFile_ConfirmDelete;
	TLabel *Label7;
	TCheckBox *cb_ShowDescriptionBoxes;
	TPanel *Panel8;
	TLabel *Label13;
	TTabSheet *tab_Advanced;
	TCheckBox *cb_RemoveNonExistingSourceDirs;
    TCheckBox *cb_UseNewUIOnBFD;
    TGroupBox *gbx_Language;
    TComboBox *cmbx_Languages;
    TGroupBox *GroupBox1;
    TEdit *ed_PrefsFile;
    TGroupBox *gbx_RecentFiles;
    TCheckBox *cb_RecentFilesShowPath;
    TEdit *ed_MaxRecentFiles;
    TUpDown *ud_MaxRecentFiles;
    TLabel *Label2;
    TCheckBox *cb_CreateTempZipCopy;
    TCheckBox *cb_TreatWarningsAsErrors;
    TLabel *Label9;
    TLabel *Label14;
    TPanel *pnl_Bottom;
    TButton *bt_Help;
    TButton *bt_Ok;
    TButton *bt_Cancel;
    TLabel *Label5;
    TGroupBox *gbx_Logging_Action;
    TLabel *Label3;
    TLabel *Label10;
    TCheckBox *cb_LogBuildingFileLists;
    TCheckBox *cb_LogFullAction;
    TButton *bt_ActionLogDir;
    TEdit *ed_ActionLogDir;
    TCheckBox *cb_AutoSaveActionLog;
    TGroupBox *gbx_Logging_Application;
    TCheckBox *cb_SaveAppStatus;
    TLabel *Label8;
    TEdit *ed_AppStatusMaxSize;
    TUpDown *sped_MaxStatusLogSize;
    TCheckBox *cb_UseZipBuilderAddSafe;
    TLabel *Label1;
    TGroupBox *gbx_DefaultDestDir;
    TEdit *ed_DefDestDir;
    TButton *bt_DefDestDir;
    TGroupBox *GroupBox2;
    TEdit *ed_DefDataDir;
    TButton *bt_DefDataDir;
    TGroupBox *gbx_TemporaryDirectory;
    TEdit *ed_TempDir;
    TButton *bt_TempDir;
    TButton *bt_DefaultTempDir;
    TGroupBox *gbx_7ZipExe;
    TButton *bt_7ZipExe;
    TEdit *ed_ExePath7Zip;
    TLabel *lbl_Get7Zip;
    TCheckBox *cb_OEMlistfile7Zip;
	void __fastcall bt_OkClick(TObject *Sender);
	void __fastcall bt_DefDestDirClick(TObject *Sender);
	void __fastcall bt_TempDirClick(TObject *Sender);
	void __fastcall bt_DefaultTempDirClick(TObject *Sender);
	void __fastcall bt_ActionLogDirClick(TObject *Sender);
	void __fastcall lbl_Get7ZipClick(TObject *Sender);
	void __fastcall bt_7ZipExeClick(TObject *Sender);
	void __fastcall bt_DefDataDirClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall bt_CancelClick(TObject *Sender);
	void __fastcall bt_SelectCustomInitialProfileClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall bt_HelpClick(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormDestroy(TObject *Sender);

private:	// User declarations

    TStringList* _availableLocales;

public:		// User declarations
	__fastcall Tf_Prefs(TComponent* Owner);

	void saveToIniFile();

	bool defaultDirsModified;   // Let f_Main know dirs have changed.
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_Prefs *f_Prefs;
//---------------------------------------------------------------------------
#endif
