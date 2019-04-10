/*
 * RenumKicadPCBGUI.h
 *
 *  Created on: Mar. 29, 2019
 *      Author: BrianP
 */

#ifndef RENUMKICADPCBCLASSES_H_
#define RENUMKICADPCBCLASSES_H_

#include <wx/wx.h>
#include "RenumKicadPCB_Base.h"

class RenumKicadPCB: public RenumKicadPCB_Base
{
public:
    RenumKicadPCB( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title
                   = _("RenumKiCadPCB V4.0"), const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxSize( 850,615 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    void ShowMessage( const char *message );
    void ShowMessage( std::string &message );
    void ShowMessage( const wxString &message );
    void GetParameters( void );
    void SetParameters( void );
    void FatalError( const wxString &message );
    void ShowError( const wxString &message );
    void SetStyle( const char* Colour );

private:
    void ProjectFileSel( wxFileDirPickerEvent& event ) override;
    void OnRenumberClick( wxCommandEvent& event )override;
    void OnCancel( wxCommandEvent& event ) override;
    void OKDone( wxCommandEvent& event )override;
    void SayAbout( wxCommandEvent& event )override;
    void SayReadMe(wxCommandEvent& event)override;
    void AccessText( wxCommandEvent& event )override;

};



class AboutDialog: public AboutDialog_Base
{
public:
    AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 496,433 ), long style = wxDEFAULT_DIALOG_STYLE );

private:
    void CloseAbout( wxCommandEvent& event )override;
};


class ReadMeDialog : public ReadMeDialog_Base
{
public:
    ReadMeDialog(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Read Me"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(496, 433), long style = wxDEFAULT_DIALOG_STYLE);

private:
    void CloseReadMe(wxCommandEvent& event)override;
};


class ContinueAbort : public ContinueAbort_Base
{
public:
    ContinueAbort( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 362,127 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE );

private:
    void ErrorContinue( wxCommandEvent& event ) override;
    void ErrorAbort( wxCommandEvent& event ) override;
};


class Abort : public Abort_Base
{
public:
    Abort( wxWindow* parent, wxWindowID id = wxID_ANY,
           const wxString& title = _("Bottom Start Ref Des too Low"),
               const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 244,127 ), long style = wxCAPTION );

private:
    void ErrorContinue( wxCommandEvent& event ) override;
};

class TextFunctions : public TextFunctions_Base
{
public:
    TextFunctions( wxWindow* parent, wxWindowID id = wxID_ANY,
                   const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxSize( 501,562 ), long style = wxDEFAULT_DIALOG_STYLE );
    void GetParameters( void );
    void SetParameters( void );

private:
    void SelectOverlay(wxCommandEvent& event) override;
    void UpdateTextFieldCLick( wxCommandEvent& event ) override;
    void ExitTextFieldClick( wxCommandEvent& event ) override;
    void SaveChangesClick( wxCommandEvent& event ) override;
    void SelectTextAngle( wxCommandEvent& event ) override;
};


class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};



#endif /* RENUMKICADPCBCLASSES_H_ */
