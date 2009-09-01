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

#include "FiltersFrame.h"
#include "Gui_X_Profile.h"
#include "AbaktProfile.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameFilters *FrameFilters;
//---------------------------------------------------------------------------

__fastcall TFrameFilters::TFrameFilters(TComponent* Owner)
	: TFrame(Owner)
{
    // Prepare fr_FilterSetList:
    fr_FilterSetList->setFilterSetList(&PProfile->filterSetList);
    // Set callback to make fr_FilterSetList notify Profile that the FilterSetList has changed:
    fr_FilterSetList->setOnModifiedCallback(OnFilterSetListModified);
}
//---------------------------------------------------------------------------

void TFrameFilters::OnFilterSetListModified(void)
{
	GuiToProfile.setModified(true);
}
//---------------------------------------------------------------------------

/* void TFrameFilters::_disableComplementaryMask(String mask, TCheckListBox* clb)
{
	TCheckListBox* compl_clb = NULL;

	if (clb == fr_Include->clb)
		compl_clb = fr_Exclude->clb;
	if (clb == fr_Exclude->clb)
		compl_clb = fr_Include->clb;

	if (compl_clb)
		UncheckMaskInFilter(mask, compl_clb);
} */
//---------------------------------------------------------------------------

