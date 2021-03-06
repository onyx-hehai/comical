/*
 * ComicBookRAR.h
 * Copyright (c) 2003-2011, James Athey
 */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   In addition, as a special exception, the author gives permission to   *
 *   link the code of his release of Comical with Rarlabs' "unrar"         *
 *   library (or with modified versions of it that use the same license    *
 *   as the "unrar" library), and distribute the linked executables. You   *
 *   must obey the GNU General Public License in all respects for all of   *
 *   the code used other than "unrar". If you modify this file, you may    *
 *   extend this exception to your version of the file, but you are not    *
 *   obligated to do so. If you do not wish to do so, delete this          *
 *   exception statement from your version.                                *
 *                                                                         *
 ***************************************************************************/

#ifndef _ComicBookRAR_h_
#define _ComicBookRAR_h_

#include "ComicBook.h"
#ifdef _WIN32
#include <windef.h>
#endif
#include "dll.hpp"
 
class ComicBookRAR : public ComicBook {

public:
	ComicBookRAR(wxString _filename, wxUint32 _cacheLen, COMICAL_ZOOM _zoom, long _zoomLevel, bool _fitOnlyOversize, COMICAL_MODE _mode, FREE_IMAGE_FILTER _filter, COMICAL_DIRECTION _direction, wxInt32 _scrollbarThickness);
	~ComicBookRAR() {};

protected:
	virtual wxInputStream * ExtractStream(wxUint32 pageindex);
	virtual wxInputStream * ExtractStream(wxString path);
	virtual bool TestPassword();

private:
	wxString OpenArchiveError(int Error);
	wxString ProcessFileError(int Error, wxString compressedFile);
	HANDLE openRar(RAROpenArchiveDataEx *flags, RARHeaderDataEx *header, wxUint8 mode);
	void closeRar(HANDLE rarFile, RAROpenArchiveDataEx *flags);
};

#endif
