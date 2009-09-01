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

#include "AbaktHistory.h"
#include <fstream>
#include "XML_IO.h"
#include "eccString.h"
#include "eccFile.h"
#include "eccMisc.h"
#include "FileProperties.h"
#include "AbaktGlobals.h"
#include "GlobalFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
static const char* ABT_HISTORY_XML_ROOT_ELM = "abakt-history";
static TAbtELMid ELMid;
static TAbaktHistory* __PHist = NULL;
//---------------------------------------------------------------------------
#include <algorithm>
bool __sortAgeReverse(const TAbaktHistoryItem& it1, const TAbaktHistoryItem& it2);
//---------------------------------------------------------------------------

TAbaktHistory::TAbaktHistory(TAbaktProfile& profile, TListBox* logListBox)
{
	__PHist = this; // This helps _xml_read_start() and _xml_read_end().
	_profile = &profile;
	_logListBox = logListBox;

    _latestArchive.fileName     = "";
    _latestArchive.age          = -1;
    _latestArchive.archiveMode  = (TArchiveMode)-1;

	if (_profile->fileName.IsEmpty())
	{
		// If empty: profile has never been saved (name == "Untitled").
		// Nothing will be written to the history-file.
		log(_("No History file. You have to save the Profile first."), ecc::smtWarning);
	}
	else
	{
		_fileName = ExtractFilePath(_profile->fileName) + _profile->name + ".abh";
		log( ASPF(_("Reading History file %s."), ecc::QuoteStr(_fileName.c_str())) );
	}

	_maxDays		= _profile->config.History->iMaxDays->get();
	_minBackups		= _profile->config.History->iMinBackups->get();
	_maxBackups		= _profile->config.History->iMaxBackups->get();
	_useRecycleBin	= _profile->config.History->bUseRecycleBin->value;

	_read();
}
//---------------------------------------------------------------------------

bool TAbaktHistory::write()
{
	if (_fileName.IsEmpty()) return false;

	std::ofstream ofs(_fileName.c_str());
	if (!ofs)
	{
		log(__FUNC__ " : Failed to open file: " + ecc::QuoteStr(_fileName) + ".", ecc::smtError);
		return false;
	}

	log(_("Writing updated History file..."));

	try
	{
		// Header:
		xout::XmlHeader(ofs);
		xout::rootElement(ofs, ABT_HISTORY_XML_ROOT_ELM,
                ecc::StrToXmlUtf8Str(_profile->name).c_str() );
		ofs << std::endl;

		// Associated profile:
		ofs	<< "<profile path='" << ecc::StrToXmlUtf8Str(_profile->fileName).c_str() << "'/>" << std::endl;
		ofs	<< std::endl;

        // Latest Archive:
		ofs	<< "<LatestArchive"
            << " path='" << ecc::StrToXmlUtf8Str(_latestArchive.fileName).c_str() << "'"
            << " archivemode='" << ArchiveModeToStr(_latestArchive.archiveMode).c_str() << "'"
            << "/>" << std::endl;
		ofs	<< std::endl;


/* TODO 3 : Hier beginnen met .abh-format aanpassen? */


		// List of files:
		xout::StartElm(ofs, 0, "filelist");
		for (unsigned int i = 0; i < _items.size(); i++)
		{
			ofs << "\t<file"
                << " path='" << ecc::StrToXmlUtf8Str(_items[i].fileName).c_str() << "'"
                << " archivemode='" << ArchiveModeToStr(_items[i].archiveMode).c_str() << "'"
				<< "/>" << std::endl;
		}
		xout::EndElm(ofs, 0, "filelist");
		ofs << std::endl;

		// Footer:
		xout::EndElm(ofs, 0, ABT_HISTORY_XML_ROOT_ELM);
	}
	catch (...)
	{
		log(_("Failed."), ecc::smtWarning, -1);
		return false;
	}
	log(_("Ok."), -1);
	return true;
}
//---------------------------------------------------------------------------

bool TAbaktHistory::_read()
{
	if (_fileName.IsEmpty()) return false;
	if (!FileExists(_fileName)) return false;

	std::ifstream ifs(_fileName.c_str());
	if (!ifs) return false;

	_items.clear();
	ELMid = ELMid_none;

	//	Read the history-file into _items:
	//	It is not not an error if the file does not exist (it wil be created by Write()).
	bool success = true;

	if (!FileExists(_fileName))
    {
		log( ASPF(_("History file does not exist: %s."), ecc::QuoteStr(_fileName.c_str())),
            ecc::smtError);
    }
	else
    {
		success = XmlIO_ParseFile(_fileName, _xml_read_start, _xml_read_end);
    }

	if (!success)
		log( ASPF(_("Error while parsing History file: %s."), ecc::QuoteStr(_fileName.c_str())),
            ecc::smtError);

	if (success)
		success = getAgeOfFiles();

	return success;
}
//---------------------------------------------------------------------------

void TAbaktHistory::_xml_read_start(void *data, const char *el, const char **attr)
{
	String elm = String(el);
	String atr, val;

    // List of files:
	if (elm == "filelist")
	{
		ELMid = ELMid_filelist;
		return;
	}

	if ( (ELMid == ELMid_filelist) && (elm == "file"))
	{
		TAbaktHistoryItem hItem;
		hItem.fileName = xin::AttrValStr("path", attr);
        String archiveMode = xin::AttrValStr("archivemode", attr);
        if (archiveMode.IsEmpty()) hItem.archiveMode = (TArchiveMode)-1; // Unknown (pre 305).
		else hItem.archiveMode = StrToArchiveMode(archiveMode);
		hItem.age = -1;
		if (__PHist->indexOf(hItem.fileName) == -1)
		{
			// If fileName does not exist, add it:
			__PHist->_items.push_back(hItem);
		}
		return;
	}

    // Latest Archive:
	if (elm == "LatestArchive")
	{
		ELMid = ELMid_none;
		__PHist->_latestArchive.fileName      = xin::AttrValStr("path", attr);
		__PHist->_latestArchive.archiveMode   = StrToArchiveMode(xin::AttrValStr("archivemode", attr));
		return;
	}
}
//---------------------------------------------------------------------------

void TAbaktHistory::_xml_read_end(void *data, const char *el)
{
	String elm = String(el);

	if (elm == "filelist")
	{
		ELMid = ELMid_none;
		return;
	}
}
//---------------------------------------------------------------------------

bool TAbaktHistory::getAgeOfFiles()
{
	bool success = true;
	TFileProperties fp;

	for (unsigned int i = 0; i < _items.size(); i++)
	{
		if (fp.retrieve(_items[i].fileName))
            _items[i].age = fp.getAge();
		else
			_items[i].age = -1;
	}

	sortByAge();

	return success;
}
//---------------------------------------------------------------------------

bool TAbaktHistory::sortByAge()
{
	bool success = true;

	try
	{
		std::sort(_items.begin(), _items.end(), __sortAgeReverse);
	}
	catch(...)
	{
		log(__FUNC__ " : Error while sorting.", ecc::smtError);
		success = false;
	}

	return success;
}
//---------------------------------------------------------------------------

bool __sortAgeReverse(const TAbaktHistoryItem& item1, const TAbaktHistoryItem& item2)
{
	return item1.age > item2.age;
}
//---------------------------------------------------------------------------

bool TAbaktHistory::update()
{
	bool success = getAgeOfFiles();

	if (success)
	{
		// Remove non-existent backup-files from _items:
		std::vector<TAbaktHistoryItem>::iterator pos;
		for (unsigned int i = 0; i < _items.size(); i++)
		{
			if (!FileExists(_items[i].fileName))
			{
				log( ASPF(_("Removing non-existent Backup path from History filelist: %s..."),
                        ecc::QuoteStr(_items[i].fileName)) );
				pos = &_items[i];
				_items.erase(pos);
				--i;	// Required!
				log(_("Ok."), -1);
			}
		}
	}

	return success;
}
//---------------------------------------------------------------------------

bool TAbaktHistory::add(const String fileName, const TArchiveMode archiveMode)
{
	if (_fileName.IsEmpty()) return false;

	if (ecc::DriveIsRemovable(fileName))
	{
		log(_("Files on removable disks are not recorded."), ecc::smtNotif);
		return true; // Not an error.
	}

	log(ASPF(_("Adding to History: %s..."), ecc::QuoteStr(fileName.c_str()) ) );
	try
	{
		if (indexOf(fileName) == -1)
		{
			TAbaktHistoryItem hItem;
			hItem.fileName      = fileName;
            hItem.archiveMode   = archiveMode;
			_items.push_back(hItem);
		}
	}
	catch (...)
	{
		log(_("Failed."), ecc::smtError, -1);
		return false;
	}
	log(_("Ok."), -1);
	return true;
}
//---------------------------------------------------------------------------

bool TAbaktHistory::execute()
{
	if (_fileName.IsEmpty()) return false;

	// Test MinBackups vs MaxBackups:
	if ( (_minBackups > 0) && (_maxBackups > 0) )
	{
		if (_minBackups > _maxBackups)
		{
			log( ASPF(_("MinBackups (%d) is greater than MaxBackups (%d)."),
                    _minBackups, _maxBackups),
                ecc::smtWarning);
			_maxBackups = _minBackups;
			log( ASPF(_("MaxBackups is set to %d."), _minBackups), ecc::smtNotif);
		}
	}

	if (!update()) return false;

	log( ASPF(_("Files listed in History: %d."), _items.size()) );

	unsigned int deleteCount = 0;	// Number of backups to delete.

	// Test _maxDays:
	if (_maxDays > 0)
	{
		for (unsigned int i = 0; i < _items.size(); i++)
		{
			if (_items[i].age == -1)
				log( ASPF(_("Could not determine age of %s."),
                        ecc::QuoteStr(_items[i].fileName).c_str()),
                    ecc::smtWarning);
			else
			{
				if ((unsigned int)_items[i].age > _maxDays)
					++deleteCount;
			}
		}
		if (deleteCount)
		{
			log( ASPF(_("Backups marked for removal by Age (Older than %d day(s)): %d."),
                    _maxDays, deleteCount),
                ecc::smtNotif);
		}
	}

	// Test _minBackups:
	if ( (_minBackups > 0) && (_items.size() > _minBackups) )
	{
		if ( (_items.size() - deleteCount) < _minBackups)
		{
			deleteCount = _items.size() - _minBackups;
			log( ASPF(_("Keeping a minimum of %d backups."), _minBackups),
                ecc::smtNotif);
		}
	}

	// Test _maxBackups:
	if ( (_maxBackups > 0) && (_items.size() > _maxBackups) )
	{
		if ( (_items.size() - deleteCount) > _maxBackups)
		{
			deleteCount = _items.size() - _maxBackups;
			log( ASPF(_("Keeping a maximum of %d backups."), _maxBackups),
                ecc::smtNotif);
		}
	}

	// Delete files:
	if (deleteCount <= 0)
	{
		log(_("No backups will be removed."), ecc::smtNotif);
	}
	else
	{
		log( ASPF(_("Removing %d backup(s):"), deleteCount), ecc::smtNotif);
		for (unsigned int i = 0; i < deleteCount; i++)
        {
            _removeBackup(_items[i].fileName);
        }
	}

	bool success = update();
	if (success) success = write();
	return success;
}
//---------------------------------------------------------------------------

void TAbaktHistory::_removeBackup(String fName)
{
    int filesRemoved = 0;

    if (_useRecycleBin)
    {
        log( ASPF(_("Moving to Recycle Bin: %s..."), ecc::QuoteStr(fName).c_str()) );
        if (ecc::FileToRecycleBin(fName)) ++filesRemoved;
    }
    else
    {
        log( ASPF(_("Permanently deleting %s..."), ecc::QuoteStr(fName).c_str()) );
        if (DeleteFile(fName)) ++filesRemoved;
    }

    if (!filesRemoved)
    {
        log(_("Failed."), ecc::smtWarning, -1);
        return;
    }

    // Remove split-files .Z01 - .Z99:
    String zName;
    String nr;
    int i;
    for (i = 1; i <= 99; i++)
    {
        nr = IntToStr(i);
        if (i < 10) nr = "0" + nr;
        zName = ChangeFileExt(fName, ".Z" + nr);
        if (FileExists(zName))
        {
            if (_useRecycleBin)
            {
                if (ecc::FileToRecycleBin(zName)) ++filesRemoved;
            }
            else
            {
                if (DeleteFile(zName)) ++filesRemoved;
            }
        }
        else break;
    }
    if (filesRemoved > 1)
        log( ASPF(_("and %d associated .Z## file(s)..."), filesRemoved-1), -1);
    if (filesRemoved != i)
        log(_("Could not remove all associated .Z## files."), ecc::smtWarning, -1);
    else
        log(_("Ok."), -1);
}
//---------------------------------------------------------------------------

bool TAbaktHistory::execute(const String fileName, const TArchiveMode archiveMode)
{
	bool success = add(fileName, archiveMode);
	if (success)
	{
        _latestArchive.fileName    = fileName;
        _latestArchive.archiveMode = archiveMode;
    	success = execute();
    }
	return success;
}
//---------------------------------------------------------------------------

int TAbaktHistory::indexOf(const String path) const
{
	for (unsigned int i = 0; i < _items.size(); i++)
	{
		if (path.AnsiCompareIC(_items[i].fileName) == 0)
            return i;
	}
	return -1;	// No match found.
}
//---------------------------------------------------------------------------

String TAbaktHistory::getFileName(const unsigned int index) const
{
	if (index >= _items.size()) return "";
	return _items[index].fileName;
}
//---------------------------------------------------------------------------

int TAbaktHistory::getAge(const unsigned int index) const
{
	if (index >= _items.size()) return -1;
	return _items[index].age;
}
//---------------------------------------------------------------------------

TArchiveMode TAbaktHistory::getArchiveMode(const unsigned int index) const
{
	if (index >= _items.size()) return (TArchiveMode)-1;
	return _items[index].archiveMode;
}
//---------------------------------------------------------------------------

void TAbaktHistory::log(const String msg,
    const int lines_before, const int lines_after) const
{
	if (!_logListBox) return;
	log(msg, ecc::smtDefault, lines_before, lines_after);
}
//---------------------------------------------------------------------------

void TAbaktHistory::log(const String msg, const ecc::TLBMessageStyle style,
		const int lines_before, const int lines_after) const
{
	if (!_logListBox) return;
	ecc::LBMessage(_logListBox, msg, style, lines_before, lines_after);

	// Log to file:
	if (PGlobals->AutoSaveActionLog)
	{
		abtAppendStrToFile(msg, PGlobals->ActionLogDir + PProfile->name + ".log", true);
	}
}
//---------------------------------------------------------------------------

