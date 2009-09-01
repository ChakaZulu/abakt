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
#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include "FinalizeFrame.h"
#include "ActionTypeFrame.h"
#include "ActionTypeSettingsFrame.h"
#include "DestinationFrame.h"
#include "SourceFrame.h"
#include "FiltersFrame.h"
#include "eccString.h"
#include "RecentFiles.h"
#include <ActnList.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------

class Tf_Main : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TActionList *ActionList;
	TAction *act_Exit;
	TSaveDialog *sdlg_Profile;
    TMenuItem *mi_FileExit;
	TOpenDialog *odlg_Profile;
    TAction *act_FileSave;
    TMenuItem *mi_FileSave;
    TAction *act_FileOpen;
    TMenuItem *mi_FileOpen;
    TAction *act_ExecuteProfile;
	TMenuItem *N2;
	TMenuItem *mu_File;
	TMenuItem *mu_Help;
	TMenuItem *mi_About;
	TMenuItem *N3;
    TMenuItem *mi_FileNew;
    TAction *act_FileSaveAs;
    TMenuItem *mi_FileSaveAs;
    TAction *act_Group;
    TMenuItem *mi_ToolsExecuteProfile;
    TAction *act_FileClose;
    TMenuItem *mi_FileClose;
	TMenuItem *N6;
	TAction *act_ViewActionLog;
	TMenuItem *mu_Tools;
    TMenuItem *mi_ToolsGroup;
	TPageControl *PageControl;
	TTabSheet *tab_Source;
	TTabSheet *tab_Filters;
	TTabSheet *tab_Action;
	TMenuItem *mi_Help;
	TMenuItem *N5;
    TAction *act_Manual;
	TAction *act_About;
    TAction *act_RestoreZipArchive;
    TMenuItem *mi_ToolsRestoreZipArchive;
	TAction *act_Preferences;
	TMenuItem *mi_Preferences;
	TMenuItem *N9;
	TMenuItem *mi_RecentFiles;
	TMenuItem *mu_View;
	TMenuItem *mi_Source_RefreshDirectoryList;
	TMenuItem *N10;
	TAction *act_FilterPreview;
    TMenuItem *mi_ViewActionLog;
	TSplitter *Splitter;
	TSaveDialog *sdlg_Status;
	TGroupBox *gbx_Status;
	TListBox *lb_Status;
    TAction *act_EditSource;
    TAction *act_EditFilters;
    TMenuItem *mi_ToolsFilterPreview;
	TMenuItem *N13;
	TMenuItem *N12;
	TMenuItem *mu_Edit;
	TMenuItem *N8;
	TAction *act_ClearStatus;
	TMenuItem *actClearStatus1;
	TMenuItem *mi_Source_ScanTreeBelowDir;
	TMenuItem *N14;
    TMenuItem *mi_ViewFilters;
    TAction *act_StatusSaveAs;
    TMenuItem *mi_StatusSaveAs;
	TPopupMenu *pop_Status;
	TMenuItem *ClearStatus1;
	TMenuItem *SaveStatusAs2;
    TAction *act_EditActionTypeSettings;
    TAction *act_EditDestination;
    TMenuItem *mi_ViewActionTypeSettings;
    TMenuItem *mi_ViewDestination;
	TPageControl *pgc_Action;
	TTabSheet *tab_ActionType;
	TTabSheet *tab_ActionTypeSettings;
	TTabSheet *tab_Destination;
    TAction *act_EditActionType;
    TMenuItem *mi_ViewActionType;
	TMenuItem *mi_Source_RefreshFileList;
    TMenuItem *mi_ViewAction;
    TMenuItem *mi_ViewSource;
    TTabSheet *tab_Finalize;
    TAction *act_EditFinalize;
    TMenuItem *mi_ViewFinalize;
    TAction *act_TemplateEditor;
    TMenuItem *mi_ToolsTemplateEditor;
    TAction *act_EditAction;
    TPanel *pnl_Toolbars;
    TAction *act_FileNew;
    TMenuItem *N4;
    TAction *act_EditHistory;
    TMenuItem *ManageHistory1;
    TPanel *pnl_FileToolBar;
    TToolBar *tbar_File;
    TToolButton *tb_ProfileNew;
    TToolButton *tb_FileOpen;
    TToolButton *tb_ProfileSave;
    TPanel *pnl_TabsToolbar;
    TToolBar *tbar_ProfileTabs;
    TToolButton *tb_Source;
    TToolButton *tb_Filters;
    TToolButton *tb_Action;
    TToolButton *tb_Finalize;
    TToolButton *ToolButton1;
    TToolButton *ToolButton2;
    TAction *act_ToggleDescriptionBoxes;
    TMenuItem *mi_ShowDescriptionBoxes;
    TMenuItem *N1;
    TFrameFilters *fr_Filters;
    TFrameActionType *fr_ActionType;
    TPanel *pnl_ExecuteButton;
    TToolBar *tbar_ExecuteButton;
    TToolButton *tb_ExecuteProfile;
    TToolButton *ToolButton4;
    TFrameFinalize *fr_Finalize;
    TFrameSource *fr_Source;
    TFrameDestination *fr_Destination;
    TMenuItem *mi_SelectManual;
    TOpenDialog *odlg_Manual;
    TMenuItem *N7;
    TMenuItem *mi_AbaktWebsite;
    TFrameActionTypeSettings *fr_ActionTypeSettings;
	void __fastcall act_ExitExecute(TObject *Sender);
	void __fastcall act_FileSaveExecute(TObject *Sender);
	void __fastcall act_FileOpenExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall act_ExecuteProfileExecute(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall act_FileSaveAsExecute(TObject *Sender);
	void __fastcall act_FileNewExecute(TObject *Sender);
	void __fastcall act_GroupExecute(TObject *Sender);
	void __fastcall act_FileCloseExecute(TObject *Sender);
	void __fastcall act_ViewActionLogExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall act_AboutExecute(TObject *Sender);
	void __fastcall act_RestoreZipArchiveExecute(TObject *Sender);
	void __fastcall act_PreferencesExecute(TObject *Sender);
	void __fastcall act_FilterPreviewExecute(TObject *Sender);
	void __fastcall ApplicationActivate(TObject *Sender);
	void __fastcall SplitterCanResize(TObject *Sender, int &NewSize,
          bool &Accept);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth,
          int &NewHeight, bool &Resize);
	void __fastcall act_EditSourceExecute(TObject *Sender);
	void __fastcall act_EditFiltersExecute(TObject *Sender);
	void __fastcall lb_StatusDrawItem(TWinControl *Control, int Index,
		  TRect &Rect, TOwnerDrawState State);
	void __fastcall act_ClearStatusExecute(TObject *Sender);
	void __fastcall mi_Source_RefreshDirectoryListClick(TObject *Sender);
	void __fastcall mi_Source_RefreshFileListClick(TObject *Sender);
	void __fastcall mi_Source_ScanTreeBelowDirClick(TObject *Sender);
	void __fastcall act_ManualExecute(TObject *Sender);
	void __fastcall act_StatusSaveAsExecute(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall mi_RecentFilesClick(TObject *Sender);
	void __fastcall act_EditActionTypeSettingsExecute(TObject *Sender);
	void __fastcall act_EditDestinationExecute(TObject *Sender);
	void __fastcall tab_ActionTypeSettingsShow(TObject *Sender);
	void __fastcall act_EditActionTypeExecute(TObject *Sender);
	void __fastcall mu_ViewClick(TObject *Sender);
	void __fastcall tab_DestinationShow(TObject *Sender);
	void __fastcall act_EditFinalizeExecute(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall tab_FiltersShow(TObject *Sender);
    void __fastcall tab_SourceShow(TObject *Sender);
    void __fastcall tab_ActionShow(TObject *Sender);
    void __fastcall tab_FinalizeShow(TObject *Sender);
    void __fastcall act_TemplateEditorExecute(TObject *Sender);
    void __fastcall act_EditActionExecute(TObject *Sender);
    void __fastcall act_EditHistoryExecute(TObject *Sender);
    void __fastcall act_ToggleDescriptionBoxesExecute(TObject *Sender);
    void __fastcall mi_SelectManualClick(TObject *Sender);
    void __fastcall mi_AbaktWebsiteClick(TObject *Sender);

private:	// User declarations

	String _ORGCaption;

	String _latestOpenedProfile;

	int _modifiedProfileDlg();
    /*  Notifies user that the Profile has been changed
        and asks if the modified Profile should be saved.
        Returns mrYes, mrNo or mrCancel. */

	void _saveSettings();
	void _loadSettings();
	void _setInitialDirsToDefault();
	void _showViewSourceMenuItems(bool show);
	void _showDescriptionBoxes(bool show);

	bool _saveProfileDialog();

	void _doInitialProfile();

	void _doCreateFileAssociations();
    /*  If enabled in the Preferences this function restores the
        file associations for abp/abg/abf/(abt)/abh files. */

	TRecentFiles* _recentFiles;
	static void OnSelectRecentFile(String fpath);
	/* 	Callback function for TRecentFiles.OnSelect. */

	void _displayActiveActionTypeSettingTab();

public:		// User declarations

	__fastcall Tf_Main(TComponent* Owner);

	void setCaption(String name);

	bool openProfileInGui(String fname);
	/* 	Opens profile stored in [fname].
		Updates GUI controls (ProfileToGui) to reflect the settings in the profile.
		Notifies user if profile has been saved with an older Abakt.
		Returns false if profile could not be opened. */

	bool openProfile(String fileName);
	void saveProfile(String fileName);

    void doLog(const String msg, const ecc::TLBMessageStyle smt,
            const int lines_before = 0, const int lines_after = 0);
	void status(const String msg, int lines_before = 0, int lines_after = 0);
	void statusNotif(const String msg, int lines_before = 0, int lines_after = 0);
	void statusWarn(const String msg, int lines_before = 0, int lines_after = 0);
	void statusError(const String msg, int lines_before = 0, int lines_after = 0);

	//095rc2 removed: bool commandLineStartup;
	/*	Is true if application was started with command line parameters. */
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_Main *f_Main;
//---------------------------------------------------------------------------
#endif

