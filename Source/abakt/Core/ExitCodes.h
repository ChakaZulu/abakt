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
#ifndef ExitCodesH
#define ExitCodesH
//---------------------------------------------------------------------------

//===========================================================================
namespace abakt {
//===========================================================================

typedef enum
{
	EXIT_OK         = 0x00,
	EXIT_ERROR      = 0x01,
	EXIT_WARNING    = 0x02
} TExitCode;

//===========================================================================
};	// namespace abakt
//===========================================================================

extern abakt::TExitCode AbaktExitCode; // Defined in the .cpp file.
/*  This is a globally modifyable value for the TExitCode Abakt will return
    when started with commandline parameters. */

//---------------------------------------------------------------------------
#endif