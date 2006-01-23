/***************************************************************************
                              ComicalFrame.h
                             -------------------
    copyright            : (C) 2003-2006 by James Athey
    email                : jathey@comcast.net
 ***************************************************************************/

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

#ifndef _ComicalFrame_h
#define _ComicalFrame_h

#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/config.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

#include "ComicalBrowser.h"
#include "ComicalCanvas.h"
#include "ComicBook.h"

class ComicalFrame : public wxFrame
{
  public:

    ComicalFrame(const wxString& title,
              const wxPoint& pos,
              const wxSize& size,
              long style = wxDEFAULT_FRAME_STYLE);

    void OpenFile(wxString);
    void OpenDir(wxString);
    void OnOpen(wxCommandEvent& event);
    void OnOpenDir(wxCommandEvent& event);
    void OnFull(wxCommandEvent& event);

    wxToolBar *toolBarNav;
    wxMenuBar *menuBar;
    wxMenu *menuFile, *menuGo, *menuView, *menuHelp, *menuZoom, *menuMode, *menuFilter, *menuRotate, *menuRotateLeft, *menuRotateRight;
	wxStaticText *labelLeft, *labelRight;

  private:

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnGoTo(wxCommandEvent& event);
    void OnSingle(wxCommandEvent& event);
    void OnDouble(wxCommandEvent& event);
    void OnBuffer(wxCommandEvent& event);
	void OnZoomBox(wxCommandEvent& event);
	void OnBrowser(wxCommandEvent& event);
	void OnToolbar(wxCommandEvent& event);
	void OnPageError(wxCommandEvent& event);
	void OnHomepage(wxCommandEvent& event);
	void startBook();
	void setComicBook(ComicBook *newBook);
	void clearComicBook();
	
    wxConfig *config;
	wxBoxSizer *frameSizer, *bookPanelSizer, *toolbarSizer;	
	bool browserActive, toolbarActive;
    ComicalCanvas *theCanvas;
    ComicBook *theBook;
    ComicalBrowser *theBrowser;

    DECLARE_EVENT_TABLE()
};

enum
{
ID_S,
ID_M,
ID_OpenDir,
ID_Rotate,
ID_RotateLeft,
ID_RotateRight,
ID_Full,
ID_ZoomBox,
ID_Browser,
ID_Toolbar,
ID_Homepage,
//Zooms
ID_Unzoomed,
ID_3Q,
ID_Half,
ID_1Q,
ID_Fit,
ID_FitV,
ID_FitH,
//Modes
ID_Single,
ID_Double,
//Filters
ID_Box,
ID_Bicubic,
ID_Bilinear,
ID_BSpline,
ID_CatmullRom,
ID_Lanczos,
//Navigation
ID_First,
ID_Last,
ID_PrevTurn,
ID_NextTurn,
ID_PrevSlide,
ID_NextSlide,
ID_GoTo,
ID_Buffer,
//Rotation
ID_CW,
ID_CCW,
ID_North,
ID_East,
ID_South,
ID_West,
ID_CWL,
ID_CCWL,
ID_NorthLeft,
ID_EastLeft,
ID_SouthLeft,
ID_WestLeft
};

#endif
