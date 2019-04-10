///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/valgen.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/gbsizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/dialog.h>
#include <wx/statbox.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class RenumKicadPCB_Base
///////////////////////////////////////////////////////////////////////////////
class RenumKicadPCB_Base : public wxFrame
{
	private:

	protected:
		wxFilePickerCtrl* m_ProjectFile;
		wxStaticText* m_TopSortDirText;
		wxChoice* m_TopSortDir;
		wxChoice* m_BottomSortDir;
		wxStaticText* m_BottomSortDirText;
		wxStaticText* m_staticText31111;
		wxStaticText* m_TopRefDesStartText;
		wxTextCtrl* m_TopRefDesStart;
		wxStaticText* m_TopPrefixText;
		wxStaticText* m_TopPrefixText1;
		wxStaticText* m_BottomPrefixText;
		wxTextCtrl* m_TopPrefix;
		wxTextCtrl* m_SortGrid;
		wxRadioButton* m_SortOnModules;
		wxRadioButton* m_SortOnRefDes;
		wxTextCtrl* m_BottomPrefix;
		wxStaticText* m_BottomRefDesStartText;
		wxTextCtrl* m_BottomRefDesStart;
		wxCheckBox* m_RemoveTopPrefix;
		wxCheckBox* m_RemoveBottomPrefix;
		wxCheckBox* m_WriteChangeFile;
		wxCheckBox* m_WriteLogFile;
		wxTextCtrl* m_MessageWindow;
		wxButton* m_RenumberButton;
		wxButton* m_SaveandExitButton1;
		wxButton* m_AccessTextFunctions;
		wxButton* m_ExitButton;
		wxMenuBar* m_menubar;
		wxMenu* RenumMenuAbout;
		wxMenu* m_ReadMe;

		// Virtual event handlers, overide them in your derived class
		virtual void ProjectFileSel( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnRenumberClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OKDone( wxCommandEvent& event ) { event.Skip(); }
		virtual void AccessText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void SayAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void SayReadMe( wxCommandEvent& event ) { event.Skip(); }


	public:

		RenumKicadPCB_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("RenumKiCadPCB V4.0"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 920,810 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~RenumKicadPCB_Base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ReadMeDialog_Base
///////////////////////////////////////////////////////////////////////////////
class ReadMeDialog_Base : public wxDialog
{
	private:

	protected:
		wxTextCtrl* ReadMeBox;
		wxButton* CloseReadMeDialog;

		// Virtual event handlers, overide them in your derived class
		virtual void CloseReadMe( wxCommandEvent& event ) { event.Skip(); }


	public:

		ReadMeDialog_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Read me"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 496,433 ), long style = wxDEFAULT_DIALOG_STYLE );
		~ReadMeDialog_Base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialog_Base
///////////////////////////////////////////////////////////////////////////////
class AboutDialog_Base : public wxDialog
{
	private:

	protected:
		wxTextCtrl* AboutBox;
		wxButton* CloseAboutDialog;

		// Virtual event handlers, overide them in your derived class
		virtual void CloseAbout( wxCommandEvent& event ) { event.Skip(); }


	public:

		AboutDialog_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 496,433 ), long style = wxDEFAULT_DIALOG_STYLE );
		~AboutDialog_Base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ContinueAbort_Base
///////////////////////////////////////////////////////////////////////////////
class ContinueAbort_Base : public wxDialog
{
	private:

	protected:
		wxButton* m_Continue;
		wxButton* m_Abort;

		// Virtual event handlers, overide them in your derived class
		virtual void ErrorContinue( wxCommandEvent& event ) { event.Skip(); }
		virtual void ErrorAbort( wxCommandEvent& event ) { event.Skip(); }


	public:

		ContinueAbort_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 634,127 ), long style = wxCAPTION );
		~ContinueAbort_Base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class Abort_Base
///////////////////////////////////////////////////////////////////////////////
class Abort_Base : public wxDialog
{
	private:

	protected:
		wxButton* m_Continue;

		// Virtual event handlers, overide them in your derived class
		virtual void ErrorContinue( wxCommandEvent& event ) { event.Skip(); }


	public:

		Abort_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Bottom Start Ref Des too Low"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 244,127 ), long style = wxCAPTION );
		~Abort_Base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class TextFunctions_Base
///////////////////////////////////////////////////////////////////////////////
class TextFunctions_Base : public wxDialog
{
	private:

	protected:
		wxStaticText* m_TextFieldtoModify;
		wxChoice* m_TextFieldChoice;
		wxRadioButton* m_SetTextAngle;
		wxTextCtrl* m_TextAngle;
		wxRadioButton* m_OverlayReference;
		wxRadioButton* m_CenterOnModule;
		wxChoice* m_OverlayReferenceChoice;
		wxButton* m_UpdateTextButton;
		wxButton* m_SaveTextButton;
		wxButton* m_ExitTextButton;
		wxTextCtrl* m_TextMessageWindow;

		// Virtual event handlers, overide them in your derived class
		virtual void SelectTextAngle( wxCommandEvent& event ) { event.Skip(); }
		virtual void SelectOverlay( wxCommandEvent& event ) { event.Skip(); }
		virtual void UpdateTextFieldCLick( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveChangesClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void ExitTextFieldClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		TextFunctions_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 501,562 ), long style = wxDEFAULT_DIALOG_STYLE );
		~TextFunctions_Base();

};

