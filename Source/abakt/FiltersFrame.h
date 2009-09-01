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
#ifndef FiltersFrameH
#define FiltersFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include "AbaktClasses.h"
#include "AbaktFilterSet.h"
#include <ToolWin.hpp>
#include "FilterSetListFrame.h"
#include <vector>
//---------------------------------------------------------------------------

class TFrameFilters : public TFrame
{
__published:	// IDE-managed Components
	TPanel *pnl_Descr_Filters;
	TLabel *Label4;
    TGroupBox *gbx_FilterSetList;
    TFrameFilterSetList *fr_FilterSetList;

private:	// User declarations

    static void OnFilterSetListModified(void);
	/* 	Callback function to pass to TFrameFilterSetListView::setOnModifiedCallback(). */

//	void _disableComplementaryMask(String mask, TCheckListBox* clb);

public:		// User declarations
	__fastcall TFrameFilters(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameFilters *FrameFilters;
//---------------------------------------------------------------------------
#endif

