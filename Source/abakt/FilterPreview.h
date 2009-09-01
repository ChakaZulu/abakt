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
#ifndef FilterPreviewH
#define FilterPreviewH
//---------------------------------------------------------------------------
#include "AbaktFileList.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

class Tf_FilterPreview : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TButton *bt_Close;
	TPopupMenu *pop_Files;
    TMenuItem *pmi_SelectAllFiles;
	TSaveDialog *sdlg_FileList;
    TMenuItem *pmi_ClearFileList;
	TPanel *pnl_Descr;
	TLabel *Label15;
    TGroupBox *gbx_FileLists;
    TPageControl *PageControl;
    TTabSheet *tab_IncludedFiles;
    TListView *lv_IncludeList;
    TTabSheet *tab_ExcludedFiles;
    TListView *lv_ExcludeList;
    TButton *bt_SaveActiveList;
    TGroupBox *gbx_BuildFileLists;
    TButton *bt_Build;
    TCheckBox *cb_ScanFullTree;
    TCheckBox *cb_StripDrive;
    TCheckBox *cb_AddQuotes;
    TButton *bt_Abort;
    TPanel *pnl_Included;
    TLabel *lbl_IncludedFilesInfo;
    TLabel *Label2;
    TPanel *pnl_Excluded;
    TLabel *Label4;
    TLabel *lbl_ExcludedFilesInfo;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall bt_AbortClick(TObject *Sender);
	void __fastcall pmi_SelectAllFilesClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall bt_SaveActiveListClick(TObject *Sender);
    void __fastcall bt_BuildClick(TObject *Sender);
    void __fastcall bt_CloseClick(TObject *Sender);
    void __fastcall pmi_ClearFileListClick(TObject *Sender);

private:	// User declarations

	bool _busy;
	bool _aborted;
	TAbaktFileList* _abtFileList;

	TListView* _getActiveListView();
	/*	Returns active TListView, or NULL on error. */

	static void OnCurrentFile(TAbaktFilter* filt);
	/*	Called by TAbaktFileList when its callback OnCurrentFile is set. */

public:		// User declarations
	__fastcall Tf_FilterPreview(TComponent* Owner);

	void clearLists();
    /*  Clears the listviews and the information labels. */
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_FilterPreview *f_FilterPreview;
//---------------------------------------------------------------------------
#endif
