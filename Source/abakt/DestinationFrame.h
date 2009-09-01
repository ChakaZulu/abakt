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
#ifndef DestinationFrameH
#define DestinationFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFrameDestination : public TFrame
{
__published:	// IDE-managed Components
	TScrollBox *ScrollBox_Dest;
	TGroupBox *gbx_Name;
	TLabel *Label29;
	TLabel *Label32;
	TLabel *Label34;
    TEdit *ed_NameTemplate;
    TEdit *ed_FormattedNamePreview;
	TPanel *pnl_Descr_Destination;
	TLabel *Label15;
	TLabel *lbl_AppendExtension;
	TCheckBox *cb_AppendExtension;
	TGroupBox *gbx_Directory;
	TButton *bt_BrowseDir;
    TEdit *ed_Directory;
	TCheckBox *cb_AutoCreateDir;
    TComboBox *cmbx_NameTemplateExamples;
	void __fastcall bt_BrowseDirClick(TObject *Sender);
	void __fastcall SetProfileModified(TObject *Sender);
    void __fastcall cmbx_NameTemplateExamplesSelect(TObject *Sender);
    void __fastcall ed_NameTemplateChange(TObject *Sender);
    void __fastcall cb_AppendExtensionClick(TObject *Sender);
    void __fastcall ed_DirectoryChange(TObject *Sender);

private:	// User declarations

    String _formattedDestDir(String destDir);
    /*  Returns Destination's (formatted) destination directory. */

public:		// User declarations
	__fastcall TFrameDestination(TComponent* Owner);

//	void formattedDirPreview();
	void formattedNamePreview();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameDestination *FrameDestination;
//---------------------------------------------------------------------------
#endif
