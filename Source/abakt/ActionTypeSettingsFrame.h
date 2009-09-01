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
#ifndef ActionTypeSettingsFrameH
#define ActionTypeSettingsFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFrameActionTypeSettings : public TFrame
{
__published:	// IDE-managed Components
	TTabSheet *tab_Zip;
	TTabSheet *tab_7Zip;
	TTabSheet *tab_File;
	TPageControl *pgc_ActionTypes;
	TPanel *pnl_Descr_actFile;
	TPanel *pnl_Descr_ActSets;
	TLabel *Label14;
	TScrollBox *ScrollBox_ActZip;
    TGroupBox *gbx_Zip_Method;
	TLabel *Label10;
	TLabel *Label9;
	TLabel *Label3;
	TLabel *Label4;
    TRadioButton *rb_Zip_MethodReplace;
    TRadioButton *rb_Zip_MethodUpdate;
    TRadioButton *rb_Zip_MethodNew;
    TRadioButton *rb_Zip_MethodFreshen;
	TPanel *pnl_Split;
	TLabel *Label11;
	TLabel *Label12;
    TCheckBox *cb_Zip_Split;
    TComboBox *cmbx_Zip_SplitVolumeSizePreset;
    TPanel *pnl_Zip_Comment;
    TCheckBox *cb_Zip_AddComment;
	TPanel *pnl_Descr_actZip;
	TLabel *Label15;
	TPanel *Panel4;
    TLabel *lbl_FileInfo_Overwrite;
    TLabel *lbl_FileInfo_UseRecycleBin;
    TCheckBox *cb_File_Overwrite;
    TCheckBox *cb_File_UseRecycleBin;
    TEdit *ed_Zip_SplitCustomSize;
    TLabel *lbl_ZipInfo_SplitCustomSize;
    TButton *bt_actZip_SplitCustomSize;
    TScrollBox *ScrollBox_7Zip;
    TGroupBox *gbx_7z_CommandLine;
    TLabel *Label22;
    TLabel *Label29;
    TEdit *ed_7z_Template;
    TEdit *ed_7z_Preview;
    TGroupBox *gbx_7z_Variables;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label5;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label30;
    TLabel *Label31;
    TComboBox *cmbx_7z_ArchiveType;
    TEdit *ed_7z_Password;
    TEdit *ed_7z_Command;
    TLabel *lbl_7zInfo_Command;
    TLabel *Label32;
    TLabel *Label34;
    TPanel *pnl_Descr_act7Zip;
    TLabel *Label24;
    TPanel *Panel2;
    TLabel *Label26;
    TEdit *ed_Zip_Password;
    TPanel *pnl_Compression;
    TLabel *Label1;
    TLabel *lbl_ZipInfo_Compression;
    TEdit *ed_Zip_Compression;
    TUpDown *ud_Zip_Compression;
    TLabel *Label2;
    TButton *bt_7z_Default;
    TPanel *pnl_StorePath;
    TLabel *lbl_ZipInfo_StorePath;
    TLabel *Label8;
    TComboBox *cmbx_Zip_StorePath;
    TLabel *lbl_7zInfo_ArchiveType;
    TEdit *ed_Zip_CommentTemplate;
    TLabel *Label6;
    TButton *bt_Zip_CommentPreview;
    TCheckBox *cb_File_OverwriteReadOnly;
    TLabel *lbl_7zInfo_Template;
    TLabel *lbl_File_PathInformation;
    TComboBox *cmbx_File_PathInformation;
    TLabel *lbl_FileInfo_PathInformation;
	void __fastcall SetProfileModified(TObject *Sender);
	void __fastcall ed_Zip_SplitCustomSizeKeyPress(TObject *Sender, char &Key);
	void __fastcall ud_Zip_CompressionChangingEx(TObject *Sender,
          bool &AllowChange, short NewValue, TUpDownDirection Direction);
    void __fastcall pgc_ActionTypesChange(TObject *Sender);
    void __fastcall bt_actZip_SplitCustomSizeClick(TObject *Sender);
    void __fastcall ed_Zip_SplitCustomSizeDblClick(TObject *Sender);
    void __fastcall ed_Zip_SplitCustomSizeChange(TObject *Sender);
    void __fastcall bt_7z_DefaultClick(TObject *Sender);
    void __fastcall ed_Zip_CompressionChange(TObject *Sender);
    void __fastcall ed_Zip_CompressionKeyPress(TObject *Sender, char &Key);
    void __fastcall bt_Zip_CommentPreviewClick(TObject *Sender);
    void __fastcall ed_7z_TemplateChange(TObject *Sender);
    void __fastcall cmbx_7z_ArchiveTypeChange(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TFrameActionTypeSettings(TComponent* Owner);

    void update7ZipPreview();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameActionTypeSettings *FrameActionTypeSettings;
//---------------------------------------------------------------------------
#endif
