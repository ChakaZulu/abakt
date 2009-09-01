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
#ifndef TRANSLATEH
#define TRANSLATEH
//---------------------------------------------------------------------------
/*
	An attempt to make translations of Abakt.exe possible.

	Requirements:
    -   Set global conditional ABT_TRANSLATE before compilation:
        Project|Options|Directories/Conditionals. Without this an
        ANSI english version of Abakt.exe will be the result.
    -   Add 'Source\abakt\gnugettext.pas' to the project (Abakt.bpr).
        Compile the project: 'gnugettext.hpp' and 'gnugettext.obj' will be
        created. The file 'gnugettext.obj' is linked to Abake.exe by using
        '#pragma link "gnugettext"' (see below).
	-   This header file ('TRANSLATE.h') should be #included by every .cpp file
        that uses translations.
	-   The following code should be present in the OnCreate event for
        every TForm that will be translated:
            #ifdef ABT_TRANSLATE
                TranslateComponent(this);
            #endif
    -   *.mo files in "{exe_dir}/locale/{COUNTRY-CODE}/LC_MESSAGES".
	    The *.mo files are binary versions of *.po files.
    	Use poEdit to edit *.po files and create *.mo files,
    	see Resources below.

	Notes:
    -   Abakt needs to be restarted if you change the language.

    Resources:
    -   gnugettext.pas
    	See http://dybdahl.dk/dxgettext/
    	The home of gnugettext for Delphi/Kylix/C++Builder.
    -	poEdit
	    See http://poedit.sourceforge.net/

    Using ASPF("string: %s) in combination with _():
    -   You should convert the value used for %s to String().
        Example:    ASPF("translated string: %s", String(_("some text")) );
        Note the call of String(). Required because _() returns a WideString.
*/
//---------------------------------------------------------------------------

#ifdef ABT_TRANSLATE

    #include "gnugettext.hpp"
    /*  Required to enable compilation with translations enabled.
        Generate "gnugettext.hpp" by adding "gnugettext.pas" to the project,
        and (re)build Abakt. */

    #pragma link "..\\obj\\gnugettext.obj"
    /*  Links 'gnugettext.obj' to the executable (Abakt.exe).
        Required if gnugettext.pas is not a part of the project (which is useful
        because that speeds up compilation).
        Generate "gnugettext.obj" by adding "gnugettext.pas" to the project,
        then (re)build Abakt. */

//    #define _(txt) AnsiString(_(txt))
    /*  This macro is needed to prevent BCB compilation errors
        and to keep the _() translation facility intact.
        This is because in gnugettext.pas a function _() exists that
        returns a WideString. */

#else

    #define _(txt) AnsiString(txt)
    /*  Prevents compilation errors when ABT_TRANSLATE is off. */

#endif
//---------------------------------------------------------------------------

String LocaleIdToLanguage(String locId);
/*  Returns (untranslated) language string (ie. 'Dutch' when [locId]=='nl').
    If [locId] is not found it returns [locId].
    The calling function is responsible for translating the string returned. */

String LanguageToLocaleId(String language);
/*  Returns locale identifier (ie. 'nl' when [language]=='Dutch').
    If [language] is not found it returns an empty string.
    The calling function should pass an English string (ie. 'Dutch', not 'Nederlands'). */

//---------------------------------------------------------------------------
#endif TRANSLATEH

