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

#include "About.h"
#include "AbaktGlobals.h"
#include "eccMisc.h"
#include "eccString.h"
#include "eccRelease.h"
#include "GlobalFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tf_About *f_About;
//---------------------------------------------------------------------------

__fastcall Tf_About::Tf_About(TComponent* Owner)
	: TForm(Owner)
{
	img_Logo->Picture->Assign(Application->Icon);

	lbl_Version->Caption = PGlobals->humanVersionStr();
    #ifndef ABT_TEST
        lbl_Version->Caption = lbl_Version->Caption + " (" + _("Build") + " " + PGlobals->AppBuildStr + ")";
    #endif
	lbl_Released->Caption = String(__DATE__);
	lbl_EccVersion->Caption = ecc::getReleaseVersionStr() + " - " + ecc::getReleaseDateStr();
	lbl_User->Caption = ecc::GetSystemUserName();

    #ifdef ABT_TRANSLATE
        //ed_Translator->Text = Utf8ToAnsi(AnsiString(GetTranslatorNameAndEmail())); 
        ed_Translator->Text = GetTranslatorNameAndEmail();  //092bRC2
    #else
        ed_Translator->Text = "";
    #endif
}
//---------------------------------------------------------------------------

void __fastcall Tf_About::FormCreate(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        TranslateComponent(this);
    #endif
}
//---------------------------------------------------------------------------

void __fastcall Tf_About::FormShow(TObject *Sender)
{
    #ifdef ABT_TRANSLATE
        String locId = GetCurrentLanguage();
        lbl_Language->Caption = _(LocaleIdToLanguage(locId));
    #else
        lbl_Language->Caption = "Default: English [en]";
    #endif

    lbl_ZipBuilder->Caption = ecc::ReplaceStr(lbl_ZipBuilder->Caption, "%s",
        PGlobals->ZipBuilderVersionStr);
}
//---------------------------------------------------------------------------

void __fastcall Tf_About::pnl_EmailClick(TObject *Sender)
{
    String url = "mailto:" + pnl_Email->Caption + "?subject=Abakt " + PGlobals->humanVersionStr();

    HINSTANCE ret = ::ShellExecute(Handle, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	if ( (int)ret <= 32)    // The return value is an error value if <= 32 (see help).
        ShowMessage(ASPF("Windows Error [%i]: Could not open %s", (int)ret, ecc::QuoteStr(url)) );
}
//---------------------------------------------------------------------------

void __fastcall Tf_About::pnl_InternetClick(TObject *Sender)
{
    String url = pnl_Internet->Caption;
	ShellExecute(Handle, "", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall Tf_About::pnl_LicenseClick(TObject *Sender)
{
    String url = "mk:@MSITStore:" + PGlobals->getHelpFile() + "::/Abakt_License.html";
	ShellExecute(Handle, "open", "hh.exe", url.c_str(), NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall Tf_About::pnl_MakeDonationClick(TObject *Sender)
{
    String url = pnl_Internet->Caption + "donate.html";
	ShellExecute(Handle, "", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall Tf_About::lbl_ZipBuilderLinkClick(TObject *Sender)
{
	String url = "http://www.drehoeksw.net/";
	ShellExecute(Handle, "", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

