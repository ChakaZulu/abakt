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

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

struct TLocaleIdLang {
    char* locId;
    char* language;
};

#pragma warn -8017 // Suppress warning "Redefinition of '_' is not identical".
#define _(txt) txt
static const TLocaleIdLang LocaleIdLangList[] =
{
    { "en", _("English") },
    { "da", _("Danish") },
    { "nl", _("Dutch") },
    { "eo", _("Esperanto") },
    { "fr", _("French") },
    { "de", _("German") },
    { "hu", _("Hungarian") },
    { "it", _("Italian") },
    { "pl", _("Polish") },
    { "pt", _("Portuguese") },
    { "ru", _("Russian") },
    { "es", _("Spanish") },
    { "sv", _("Swedish") },
    { "",   "Unknown" }
};
#undef _(txt) txt
#pragma warn .8017 // Restore warning "Redefinition of '_' is not identical".
static const int TLocaleIdLangListSize = sizeof(LocaleIdLangList)/sizeof(LocaleIdLangList[0]);
//---------------------------------------------------------------------------

String LocaleIdToLanguage(String locId)
{
    String language = "";
    for (int i = 0; i < TLocaleIdLangListSize; i++)
    {
        if (locId == String(LocaleIdLangList[i].locId))
        {
            language = "[" + String(LocaleIdLangList[i].locId)
                + "] " + String(_(LocaleIdLangList[i].language));
            break;
        }
    }
    if (language.IsEmpty())
        language = locId;
    return language;
}
//---------------------------------------------------------------------------

String LanguageToLocaleId(String language)
{
    String locId = "";
    for (int i = 0; i < TLocaleIdLangListSize; i++)
    {
        if (language == String(LocaleIdLangList[i].language))
        {
            locId = String(LocaleIdLangList[i].locId);
            break;
        }
    }
    return locId;
}
//---------------------------------------------------------------------------

