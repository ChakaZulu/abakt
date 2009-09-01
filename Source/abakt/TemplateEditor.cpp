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

#include "TemplateEditor.h"
#include "AbaktProfile.h"
#include "AbaktGlobals.h"
#include "GlobalFuncs.h"
#include "Gui_X_Profile.h"
#include "eccFormMemory.h"
#include "eccString.h"
#include "eccFile.h"
#include "XML_IO.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ConfigEditFrame"
#pragma link "FilterSetListFrame"
#pragma resource "*.dfm"
Tf_TemplateEditor *f_TemplateEditor;
//---------------------------------------------------------------------------
static const char* ABT_TEMPLATE_XML_ROOT_ELM    = "abakt-template";
static const char* ABT_PROFILE_XML_ROOT_ELM     = "abakt-profile";  // See Core/AbaktProfile.cpp
static const char* ABT_GROUP_XML_ROOT_ELM       = "abakt-group";    // See Core/AbaktGroup.cpp
static TAbtELMid __ELMid;
static TAbtELMid __ELMid2;	// Level 2 elements.
//---------------------------------------------------------------------------

__fastcall Tf_TemplateEditor::Tf_TemplateEditor(TComponent* Owner)
    : TForm(Owner)
{
	PageControl->ActivePage = tab_CustomFilterSets;
    PageControl->TabHeight = 1;
    PageControl->TabWidth = 1;

    _modified = false;
    mi_FileSave->Enabled = _modified;
    odlg_Template->InitialDir = PGlobals->getTemplatesDir();

	// Restore form properties:
	ecc::TFormMemory mem(PGlobals->getIniFilePath());
	mem.ReadProps(this);
	TIniFile* ini = mem.GetIniFile();

	// Read section [<this.name>.fr_ConfigEdit.lv_config.Columns]:
	String sect = Name + ".lv_Config.Columns";
	String ident;
	int cols = 0;
	while (1)
	{
		ident = "Col" + IntToStr(cols) + ".Width";
		if (!ini->ValueExists(sect, ident)) break;
		fr_CustomConfigEdit->lv->Columns->Items[cols]->Width
            = ini->ReadInteger(sect, ident, 50);
		++cols;
	}

	// Create TRecentFiles object:
	_recentFiles = new TRecentFiles(mi_RecentFiles, OnSelectRecentFile);

    // Set fr_ConfigEdit callback that notifies f_TemplateEditor that a config setting has changed:
    fr_CustomConfigEdit->OnModified = OnConfigModified;

    // Prepare fr_FilterSetList:
    fr_CustomFilterSetList->setFilterSetList(&_filterSetList);
    fr_CustomFilterSetList->setOnModifiedCallback(OnConfigModified);
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
    #endif

	_ORG_Caption = Caption; // Set it here to make sure the title can be translated (092RC2).

    mi_FileNew->Click();    // Create a new empty Template on startup.
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::FormShow(TObject *Sender)
{
	static bool been_here = false;
	if (been_here) return;
	been_here = true;
    /*	This function is called when Visible is set to true.
	    Should only be called once. */

	_recentFiles->fromIniFile(PGlobals->getIniFilePath(), INI_SECT_RECENT_TEMPLATES);

	// == Icons (Do it here to prevent BCB from including the bitmap twice in the executable):
    // Set the imagelist to use for the Main Menu:
    MainMenu->Images    = DM->imgs_Main;
    // Attach RecentFiles menu to tb_FileOpen's DropdownMenu:
    tb_FileOpen->DropdownMenu = _recentFiles->getPopupMenu();
    // -- File menu:
    mi_FileNew->ImageIndex       = ICON_FILE_NEW;
    mi_FileOpen->ImageIndex      = ICON_FILE_OPEN;
    mi_FileSave->ImageIndex      = ICON_FILE_SAVE;
    // -- View menu:
    act_ViewPageCustomFilterSets->ImageIndex    = ICON_FILTERS;
    act_ViewPageCustomConfiguration->ImageIndex = ICON_ACTION;
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::FormDestroy(TObject *Sender)
{
	// Destroy TRecentFiles object:
	delete _recentFiles;
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	try
	{
		ecc::TFormMemory mem(PGlobals->getIniFilePath());
		mem.SetComponent(this);
		TIniFile* ini = mem.GetIniFile();

		mem.WriteProps("Top;Left;Width;Height");
		mem.WriteProps("WindowState");

		// Save lv_Config->Column sizes:
		for (int i = 0; i < fr_CustomConfigEdit->lv->Columns->Count; i++)
			ini->WriteInteger(Name + ".lv_Config.Columns",
			"Col" + IntToStr(i) + ".Width", fr_CustomConfigEdit->lv->Columns->Items[i]->Width);

        _recentFiles->toIniFile(PGlobals->getIniFilePath(), INI_SECT_RECENT_TEMPLATES);
	}
	catch(...)
	{
		ShowMessage( ASPF(_("Failed to save your settings to:\n%s."),
                ecc::QuoteStr(PGlobals->getIniFilePath())) );
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
    if (_modified)
	{
		int val = _modifiedTemplateDlg();
		if (val	== mrYes)
		    mi_FileSave->Click();
		if (val	== mrCancel)
            CanClose = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::mi_CloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

int Tf_TemplateEditor::_modifiedTemplateDlg()
{
    return MessageDlg(
			_("Template has been modified.")
			+ "\n\n" + _("Save Template?"),
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::OnSelectRecentFile(String fpath)
// This function is declared as static.
{
    f_TemplateEditor->openTemplate(fpath);
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::_setModified(bool modified)
{
    _modified = modified;
    mi_FileSave->Enabled = _modified;
    String name = f_TemplateEditor->_templateName;
    if (name.IsEmpty())
        name = _("Untitled");

    Caption = name + (_modified ? "*" : "") + " - " + _ORG_Caption;
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::OnConfigModified(void)
// This function is declared as static.
{
    f_TemplateEditor->_setModified(true);
}
//---------------------------------------------------------------------------

bool Tf_TemplateEditor::_writeTemplate(String fpath)
{
	std::ofstream ofs(fpath.c_str());
	if (!ofs) return false;

    _templateName = ecc::ExtractFileNameNoExt(fpath);
    _fileName = fpath;
    sdlg_Template->InitialDir = ExtractFilePath(fpath);

	xout::XmlHeader(ofs);

	xout::rootElement(ofs, ABT_TEMPLATE_XML_ROOT_ELM, _templateName);


	// Save config items; using attribute 'override' to mark a setting as selected:

    /* TODO 2 : Test if template saves the 092b3 filterSets properly. */
	ofs << std::endl;
	ofs << "<" << ELM_FileFilters << ">" << std::endl;
	for (unsigned int i = 0; i < _filterSetList.size(); i++)
	{
		ofs << "\t"
			<< FilterSetToXmlStr(_filterSetList[i]).c_str()
            << std::endl;
	}
	ofs << "</" << ELM_FileFilters << ">" << std::endl;


	ofs	<< std::endl;
	xout::StartElm(ofs, 0, ELM_ActionTypes);
	xout::configItemList(ofs, 1, _config.Actions.File->items, true);
	xout::configItemList(ofs, 1, _config.Actions.Zip->items, true);
	xout::configItemList(ofs, 1, _config.Actions.SevenZip->items, true);
	xout::EndElm(ofs, 0, ELM_ActionTypes);

	ofs	<< std::endl;
	xout::configItemList(ofs, 0, _config.Action->items, true);

	ofs	<< std::endl;
	xout::configItemList(ofs, 0, _config.Destination->items, true);

	ofs	<< std::endl;
	xout::configItemList(ofs, 0, _config.History->items, true);

   	ofs	<< std::endl;
	xout::configItemList(ofs, 0, _config.FileList->items, true);

	// End:
	ofs << std::endl;
	xout::EndElm(ofs, 0, ABT_TEMPLATE_XML_ROOT_ELM);

	return true;
}
//---------------------------------------------------------------------------

bool Tf_TemplateEditor::_readTemplate(String fpath)
{
	// Reset things:
	_fileName = fpath;
	_templateName = ""; // Will be set by _xml_read_start.
	_config.reset();
    _filterSetList.clear();

    _reachedEOF = false;
	__ELMid = ELMid_none;
	__ELMid2 = ELMid_none;
    bool readOk = XmlIO_ParseFile(fpath, _xml_read_start, _xml_read_end);

    if (!_reachedEOF)
    {
		ShowMessage(__FUNC__ ": Parsing XML failed, closing root element not detected!");
        readOk = false;
    }

    return readOk;
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::_xml_read_start(void *data, const char *el, const char **attr)
{
	String elm = String(el);
	String atr, val;

	// <abakt-template>:
	if (elm == ABT_TEMPLATE_XML_ROOT_ELM)
	{
		f_TemplateEditor->_templateName	= xin::AttrValStr("name", attr);
		f_TemplateEditor->_version		= xin::AttrValStr("version", attr);
		f_TemplateEditor->_build	    = xin::AttrValInt("build", attr);
		return;
	}

    // <FileFilters>: added in 0.9.2-beta3
	if (elm == ELM_FileFilters)
	{
		__ELMid = ELMid_FileFilters;
		return;
	}
	if (__ELMid == ELMid_FileFilters)
	{
		xin::readSect_FileFilters_Filter_intoFSList(elm, attr, f_TemplateEditor->_filterSetList);
		return;
	}

	// <ActionTypes>:
	if (elm == ELM_ActionTypes)
	{
		__ELMid = ELMid_ActionTypes;
		__ELMid2 = ELMid_none;
		return;
	}
	if (__ELMid == ELMid_ActionTypes)
	{
		f_TemplateEditor->_readSect_ActionTypes(elm, attr);
		return;
	}

	// <Action>:
	if (elm == ELM_Action)
	{
		__ELMid = ELMid_Action;
		return;
	}
	if (__ELMid == ELMid_Action)
	{
		f_TemplateEditor->_readSect_Action(elm, attr);
		return;
	}

	// <Destination>:
	if (elm == ELM_Destination)
	{
		__ELMid = ELMid_Destination;
		return;
	}
	if (__ELMid == ELMid_Destination)
	{
		f_TemplateEditor->_readSect_Destination(elm, attr);
		return;
	}

	// <History>:
	if (elm == ELM_History)
	{
		__ELMid = ELMid_History;
		return;
	}
	if (__ELMid == ELMid_History)
	{
		f_TemplateEditor->_readSect_History(elm, attr);
		return;
	}

	// <FileList>:
	if (elm == ELM_FileList)
	{
		__ELMid = ELMid_FileList;
		return;
	}
	if (__ELMid == ELMid_FileList)
	{
		f_TemplateEditor->_readSect_FileList(elm, attr);
		return;
	}
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::_xml_read_end(void *data, const char *el)
{
	String elm = String(el);

	if (elm == ELM_FileFilters)
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_FilePropertiesFilter)
    // -- Required as long as importing pre 0.9.2-beta3.309 groups is supported.
	{
		__ELMid = ELMid_none;
		__ELMid2 = ELMid_none;
		return;
	}

	if (elm == ELM_ActionTypes)
	{
		__ELMid = ELMid_none;
		__ELMid2 = ELMid_none;
		return;
	}

	if (elm == ELM_Action)
	{
		__ELMid = ELMid_none;
		return;
	}
	if (elm == ELM_Destination)
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_History)
	{
		__ELMid = ELMid_none;
		return;
	}

	if (elm == ELM_FileList)
	{
		__ELMid = ELMid_none;
		return;
	}

	// Detect closing root element </abakt-template>, </abakt-profile> or </abakt-group>:
	if (elm == ABT_TEMPLATE_XML_ROOT_ELM)
        f_TemplateEditor->_reachedEOF = true;
	else if (elm == ABT_PROFILE_XML_ROOT_ELM)
        f_TemplateEditor->_reachedEOF = true;
	else if (elm == ABT_GROUP_XML_ROOT_ELM)
        f_TemplateEditor->_reachedEOF = true;
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::_readSect_Action(String elm, const char **attr)
{
	xin::itemAttrsToList(attr, _config.Action->items);
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::_readSect_ActionTypes(String elm, const char **attr)
{
	// <Zip>:
	if (elm == ELM_Act_Zip)
	{
		__ELMid2 = ELMid_Act_Zip;
		return;
	}
	if (__ELMid2 == ELMid_Act_Zip)
	{
		xin::itemAttrsToList(attr, _config.Actions.Zip->items);
	}

	// <SevenZip>:
	if (elm == ELM_Act_7Zip)
	{
		__ELMid2 = ELMid_Act_7Zip;
		return;
	}
	if (__ELMid2 == ELMid_Act_7Zip)
	{
		xin::itemAttrsToList(attr, _config.Actions.SevenZip->items);
	}

	// <File>:
	if (elm == ELM_Act_File)
	{
		__ELMid2 = ELMid_Act_File;
		return;
	}
	if (__ELMid2 == ELMid_Act_File)
	{
		xin::itemAttrsToList(attr, _config.Actions.File->items);
	}
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::_readSect_Destination(String elm, const char **attr)
{
   	xin::itemAttrsToList(attr, _config.Destination->items);
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::_readSect_History(String elm, const char **attr)
{
	xin::itemAttrsToList(attr, _config.History->items);
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::_readSect_FileList(String elm, const char **attr)
{
	xin::itemAttrsToList(attr, _config.FileList->items);
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::mi_FileNewClick(TObject *Sender)
{
    if (_modified)
	{
		int val = _modifiedTemplateDlg();
		if (val	== mrYes)
		    mi_FileSave->Click();
		if (val	== mrCancel)
            return;
	}

    _config.reset();
    fr_CustomConfigEdit->makeConfigList(_config);
    _filterSetList.clear();
    _templateName = "";
    _setModified(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::mi_RecentFilesClick(TObject *Sender)
{
	_recentFiles->popupAt(Left + 20, Top + 50);
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::mi_FileOpenClick(TObject *Sender)
{
    if (_modified)
	{
		int val = _modifiedTemplateDlg();
		if (val	== mrYes)
		    mi_FileSave->Click();
		if (val	== mrCancel)
            return;
	}

    if (odlg_Template->Execute())
    {
        openTemplate(odlg_Template->FileName);
        _recentFiles->add(odlg_Template->FileName);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::mi_FileSaveClick(TObject *Sender)
{
    if (_fileName.IsEmpty())
    {
        mi_FileSaveAs->Click();
        return;
    }

    if (!_writeTemplate(_fileName))
    {
        ShowMessage(__FUNC__ ": Failed saving template to: " + _fileName);
        return;
    }
    _setModified(false);
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::mi_FileSaveAsClick(TObject *Sender)
{
    if (sdlg_Template->Execute())
    {
        if (!_writeTemplate(sdlg_Template->FileName))
        {
            ShowMessage(__FUNC__ ": Failed saving template to: " + sdlg_Template->FileName);
            return;
        }
        _setModified(false);
    }
}
//---------------------------------------------------------------------------

void Tf_TemplateEditor::openTemplate(String fpath)
{
    _config.reset();
    _filterSetList.clear();
    bool readOk = _readTemplate(fpath);
    if (!readOk)
    {
        ShowMessage(__FUNC__ ": could not open file " + fpath);
        return;
    }

    if (ExtractFileExt(_fileName) == ".abt")
        _modified = false;
    else
    {
        // File is imported from .abp or .abg, make sure file will not be overwritten accidentily:
        _modified = true;
        _fileName = "";
    }

    fr_CustomConfigEdit->makeConfigList(_config);
    fr_CustomFilterSetList->redraw();
    odlg_Template->InitialDir = ExtractFilePath(fpath);
    sdlg_Template->InitialDir = ExtractFilePath(fpath);
    _setModified(_modified);    // Update form's Caption.
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::mi_ToolsPopulateWithCurrentProfileClick(
      TObject *Sender)
{
    if (_modified)
	{
		int val = _modifiedTemplateDlg();
		if (val	== mrYes)
		    mi_FileSave->Click();
		if (val	== mrCancel)
            return;
	}

    /* TODO 2 : Should the existing 'override' values be kept? */
    fr_CustomConfigEdit->makeConfigList(PProfile->config);

    // Copy PProfile's filterSetList to this template's fsList:
    MergeFilterSetLists(&PProfile->filterSetList, &_filterSetList);
    fr_CustomFilterSetList->redraw();

    _setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::mi_ToolsApplyToCurrentProfileClick(
      TObject *Sender)
{
    if (MessageDlg(
			_("Replace the current Profile's Configuration settings with the values of the selected (checked) settings in this Template.")
            + "\n"
			+ _("Merge the current Profile's FilterSets with all FilterSets in this Template.")
			+ "\n\n"
            + _("Are you sure?"),
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0)
        == mrNo) return;

    abakt::CopyAbaktConfigOnCustomOverride(_config, PProfile->config);
    // Copy template's fsList to PProfile's fsList:
    MergeFilterSetLists(&_filterSetList, &PProfile->filterSetList);

    ProfileToGui.all();
	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::act_ViewPageCustomFilterSetsExecute(
      TObject *Sender)
{
    tb_ViewCustomFilterSets->Down = true;
	PageControl->ActivePage = tab_CustomFilterSets;
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::act_ViewPageCustomConfigurationExecute(
      TObject *Sender)
{
    tb_ViewCustomConfiguration->Down = true;
	PageControl->ActivePage = tab_CustomConfiguration;
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::tab_CustomFilterSetsShow(
      TObject *Sender)
{
    tb_ViewCustomFilterSets->Down = true;
    // Adjust hotkey availability for objects that are (not) in focus:
    fr_CustomFilterSetList->enableToolBar(true);
    fr_CustomConfigEdit->enableToolBar(false);  //092b8
}
//---------------------------------------------------------------------------

void __fastcall Tf_TemplateEditor::tab_CustomConfigurationShow(
      TObject *Sender)
{
    tb_ViewCustomConfiguration->Down = true;
    // Adjust hotkey availability for objects that are (not) in focus:
    fr_CustomFilterSetList->enableToolBar(false);
    fr_CustomConfigEdit->enableToolBar(true);  //092b8
}
//---------------------------------------------------------------------------

