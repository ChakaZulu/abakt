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
#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class Tf_About : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label11;
	TPanel *pnl_MakeDonation;
    TPanel *Panel2;
    TButton *bt_Close;
    TPanel *Panel3;
    TLabel *lbl_ZipBuilder;
    TPanel *lbl_ZipBuilderLink;
    TPanel *Panel4;
    TLabel *Label10;
    TLabel *lbl_User;
    TPanel *Panel5;
    TLabel *Label9;
    TLabel *Label12;
    TLabel *lbl_Language;
    TEdit *ed_Translator;
    TPanel *Panel6;
    TLabel *lbl_Released;
    TLabel *lbl_Version;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label8;
    TLabel *lbl_EccVersion;
    TPanel *Panel7;
    TLabel *lbl_Author;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label7;
    TPanel *pnl_Email;
    TPanel *pnl_Internet;
    TPanel *pnl_License;
    TPanel *Panel8;
    TLabel *lbl_AppName;
    TLabel *Label1;
    TImage *img_Logo;
	void __fastcall pnl_EmailClick(TObject *Sender);
	void __fastcall pnl_InternetClick(TObject *Sender);
	void __fastcall pnl_LicenseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall pnl_MakeDonationClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall lbl_ZipBuilderLinkClick(TObject *Sender);
	
private:	// User declarations

public:		// User declarations
	__fastcall Tf_About(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tf_About *f_About;
//---------------------------------------------------------------------------
#endif

