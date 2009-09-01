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
#ifndef TemplateEditorH
#define TemplateEditorH
//---------------------------------------------------------------------------
#include "ConfigEditFrame.h"
#include "eccCheckStringList.h"
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
#include "FilterSetListFrame.h"
//---------------------------------------------------------------------------

/*  Note on expat's XML_Char:
    Since Abakt saves xml data using UTF8, expat is expected to have been compiled
    with XML_Char defined as char. */

//---------------------------------------------------------------------------

class Tf_TemplateEditor : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu;
    TMenuItem *mu_File;
    TMenuItem *mu_Tools;
    TMenuItem *mi_FileOpen;
    TMenuItem *mi_FileNew;
    TMenuItem *mi_FileSave;
    TMenuItem *mi_FileSaveAs;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *mi_FileClose;
    TMenuItem *mi_ToolsPopulateWithCurrentProfile;
    TMenuItem *mi_ToolsApplyToCurrentProfile;
    TOpenDialog *odlg_Template;
    TSaveDialog *sdlg_Template;
    TPageControl *PageControl;
    TTabSheet *tab_CustomConfiguration;
    TPanel *pnl_Descr_CustomConfiguration;
    TLabel *Label15;
    TMenuItem *N3;
    TMenuItem *mi_RecentFiles;
    TTabSheet *tab_CustomFilterSets;
    TPanel *pnl_Toolbars;
    TPanel *Panel3;
    TPanel *pnl_FileToolBar;
    TToolBar *tbar_File;
    TToolButton *tb_FileNew;
    TToolButton *tb_FileOpen;
    TToolButton *tb_FileSave;
    TToolButton *ToolButton1;
    TPanel *pnl_TabsToolbar;
    TToolBar *tbar_ProfileTabs;
    TToolButton *tb_ViewCustomConfiguration;
    TToolButton *tb_ViewCustomFilterSets;
    TToolButton *ToolButton2;
    TMenuItem *mi_ViewPageCustomFilterSets;
    TMenuItem *mu_View;
    TMenuItem *mi_ViewPageCustomConfiguration;
    TActionList *ActionList;
    TAction *act_ViewPageCustomFilterSets;
    TAction *act_ViewPageCustomConfiguration;
    TFrameConfigEdit *fr_CustomConfigEdit;
    TFrameFilterSetList *fr_CustomFilterSetList;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall mi_ToolsPopulateWithCurrentProfileClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall mi_FileNewClick(TObject *Sender);
    void __fastcall mi_FileSaveClick(TObject *Sender);
    void __fastcall mi_FileOpenClick(TObject *Sender);
    void __fastcall mi_CloseClick(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall mi_FileSaveAsClick(TObject *Sender);
    void __fastcall mi_ToolsApplyToCurrentProfileClick(TObject *Sender);
    void __fastcall mi_RecentFilesClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall act_ViewPageCustomFilterSetsExecute(TObject *Sender);
    void __fastcall tab_CustomFilterSetsShow(TObject *Sender);
    void __fastcall tab_CustomConfigurationShow(TObject *Sender);
    void __fastcall act_ViewPageCustomConfigurationExecute(
          TObject *Sender);

private:	// User declarations

   	String _ORG_Caption;

   	String _fileName;
    String _templateName;   // Is same as _fileName without path+ext information.
    /*  If empty, template is assumed to be a new, untitled, template.  */

   	String _version;        // Abakt's version
    int _build;             // Abakt's build number.

    bool _modified;          // Is true when template has been modified.
    void _setModified(bool modified);
    /*  Sets _modified to [modified] and updates this form's Caption. */

	static void OnConfigModified(void);
	/* 	Callback function for TFrameConfigEdit::OnModified. */

	TRecentFiles* _recentFiles;
	static void OnSelectRecentFile(String fpath);
	/* 	Callback function for TRecentFiles.OnSelect. */

    int _modifiedTemplateDlg();
    /*  Notifies user that the Template has been changed
        and asks if the modified Template should be saved.
        Returns mrYes, mrNo or mrCancel. */

	// Filter Mask Lists:
    /* TODO 2 : To be replaced with Group-FilterSets: */
/*  To be replaced with Group-FilterSets:
    std::vector<ecc::TCheckStringListItem> _excludeMaskList;
	std::vector<ecc::TCheckStringListItem> _includeMaskList;
	std::vector<ecc::TCheckStringListItem> _noCompressMaskList; */

	abakt::TConfig _config;
    /*  The template's Configuration settings. */

    std::vector<TAbaktFilterSet> _filterSetList;
    /*  The template's FilterSet list. */

    bool _writeTemplate(String fPath);
    /*  Writes Abakt template to [fPath].
        Returns false on failure. */

    bool _readTemplate(String fPath);
    /*  Reads template from fPath.
        Returns false on failure. */


	// == XML Read (parse) members:

    bool _reachedEOF;
    /*  Used to detect if </abakt-profile> has been reached. If not, there was
        probably an error while parsing the XML file. */

	static void _xml_read_start(void *data, const char *el, const char **attr);
	static void _xml_read_end(void *data, const char *el);

	void _readSect_Action(String elm, const char **attr);
	void _readSect_ActionTypes(String elm, const char **attr);
	void _readSect_Destination(String elm, const char **attr);
	void _readSect_History(String elm, const char **attr);
	void _readSect_FileList(String elm, const char **attr);

public:		// User declarations
    __fastcall Tf_TemplateEditor(TComponent* Owner);

    String getFileName()            { return _fileName; }
    String getTemplateName()        { return _templateName; }

    void openTemplate(String fpath);
    /*  Reads the Configuration settings in [fpath] to _config.
        This function is 'public' because Core/CommandLine.cpp calls it. */

    abakt::TConfig* getConfig()
    {   return &_config; }

    std::vector<TAbaktFilterSet> * getFilterSetList()
    {   return &_filterSetList; }

};
//---------------------------------------------------------------------------
extern PACKAGE Tf_TemplateEditor *f_TemplateEditor;
//---------------------------------------------------------------------------
#endif
