#include <wx/wx.h>
#include <wx/log.h>
#include <string>

#include "RenumKicadPCB_Base.h"
#include "RenumPrototypes.h"
#include "RenumKicadPCBClasses.h"
#include "RenumLiterals.h"
#include "RenumExternals.h"

RenumKicadPCB::RenumKicadPCB( wxWindow* parent, wxWindowID id,
             const wxString& title, const wxPoint& pos, const wxSize& size, long style )
                : RenumKicadPCB_Base( parent, id, title, pos, size, style )
{
}

//
// Set the menu to all of the paramters
//
void RenumKicadPCB::ProjectFileSel( wxFileDirPickerEvent& event )
{
    ( void ) event;
    G_ProjectFileName = m_ProjectFile->GetPath();
    LoadPCBFile(G_PCBInputFile);
}



void RenumKicadPCB::OnRenumberClick( wxCommandEvent& event )
{
(void) event;

    GetParameters();                                    //Set the program settings
    SetParameters();
    WriteParameterFile();                           //Write them to disk
    if( G_PCBInputFile.buffer.empty())               //If nothing already loaded
    if( 0 != LoadPCBFile( G_PCBInputFile ))
            wxLogError("Please select a KiCad project file (*.pro) with a PCB and Schematics");        //wxLogFatalError
    G_LogFile.clear();                              //Clear the log file
    RenumKiCadPCB();                                //Do the renumbering
    if( G_WriteLogFile ) WriteLogFile();            //Write out the log file
    m_MessageWindow->AppendText( "Done\n\n");
}

void RenumKicadPCB::OnCancel( wxCommandEvent& event  )
{
    (void) event;
    exit(0);
}

void RenumKicadPCB::OKDone( wxCommandEvent& event )
{
    (void) event;

    GetParameters();
    WriteParameterFile(  );
    exit(0);
}

void RenumKicadPCB::AccessText( wxCommandEvent& event )
{
    (void) event;
    G_TextDialog = new TextFunctions( NULL, wxID_ANY, wxT("PCB Text Field Manipulation"),
                                                         wxDefaultPosition, wxSize(500,600), wxDEFAULT_DIALOG_STYLE );
    G_TextDialog->Show( true );

}


void RenumKicadPCB::GetParameters( void )
{
        G_ProjectFileName = m_ProjectFile->GetPath();
        G_TopSortCode = m_TopSortDir->GetSelection();
        G_BottomSortCode = m_BottomSortDir->GetSelection();

        G_TopStartRefDes = atoi( m_TopRefDesStart->GetValue().c_str());
        G_BottomStartRefDes = atoi( m_BottomRefDesStart->GetValue().c_str());

        G_RemoveTopPrefix = m_RemoveTopPrefix->GetValue( );
        G_RemoveBottomPrefix = m_RemoveBottomPrefix->GetValue( );
        G_TopPrefix = m_TopPrefix->GetValue( );
        G_BotPrefix = m_BottomPrefix->GetValue( );

        G_WriteChangeFile = m_WriteChangeFile->GetValue( );
        G_WriteLogFile  = m_WriteLogFile->GetValue( );
        G_SortOnModules = m_SortOnModules->GetValue( );
        if(!m_SortGrid->GetValue( ).ToDouble( &G_SortGrid )) G_SortGrid = DEFAULT_GRID;
}


void    RenumKicadPCB::SetParameters( void )
{
        G_RemoveTopPrefix &= ( 0 != G_TopPrefix.size());    //Don't remove if nothing to remove
        G_RemoveBottomPrefix &= ( 0 != G_BotPrefix.size());
        m_ProjectFile->SetPath( G_ProjectFileName );
        m_TopSortDir->SetSelection( G_TopSortCode );
        m_BottomSortDir->SetSelection( G_BottomSortCode );
        m_TopRefDesStart->ChangeValue( std::to_string( G_TopStartRefDes ));
        m_TopPrefix->ChangeValue( G_TopPrefix );
        m_RemoveTopPrefix->SetValue( 0 == G_RemoveTopPrefix ? false : true );
        m_BottomRefDesStart->ChangeValue( std::to_string( G_BottomStartRefDes ));
        m_BottomPrefix->ChangeValue( G_BotPrefix );
        m_RemoveBottomPrefix->SetValue( 0 == G_RemoveBottomPrefix ? false : true );
        m_SortGrid->ChangeValue( std::to_string( G_SortGrid ));
        m_WriteChangeFile->SetValue( 0 == G_WriteChangeFile ? false : true );
        m_WriteLogFile->SetValue( 0 == G_WriteLogFile ? false : true );
        m_SortOnModules->SetValue( 0 == G_SortOnModules ? false : true );
}

void    RenumKicadPCB::SetStyle( const char* Colour )
{       m_MessageWindow->SetDefaultStyle(wxColour( Colour ));   }

void    RenumKicadPCB::ShowMessage( const char *message )
{       m_MessageWindow->AppendText( message ); }

void    RenumKicadPCB::ShowMessage( const wxString &message )
{       m_MessageWindow->AppendText( message ); }

void    RenumKicadPCB::FatalError( const wxString &message )
{       wxLogFatalError( message ); }

void    RenumKicadPCB::ShowError( const wxString &message )
{       wxLogError( message ); }

void    RenumKicadPCB::SayAbout( wxCommandEvent& event )
{
    (void) event;
    G_AboutDialog = new AboutDialog( NULL, wxID_ANY, wxT("About RenumKiCadPCB"),
                                                         wxDefaultPosition, wxSize(400,600), wxDEFAULT_DIALOG_STYLE );
    G_AboutDialog->ShowModal();
    delete( G_AboutDialog );
}

void    RenumKicadPCB::SayReadMe(wxCommandEvent& event)
{
    (void)event;
    G_ReadMeDialog = new ReadMeDialog(NULL, wxID_ANY, wxT("About RenumKiCadPCB"),
        wxDefaultPosition, wxSize(1000, 600), wxDEFAULT_DIALOG_STYLE);
    G_ReadMeDialog->ShowModal();
    delete(G_ReadMeDialog);
}

//
// AboutDialog methods
//
void    ReadMeDialog::CloseReadMe(wxCommandEvent& event)
{
    (void)event;
    G_ReadMeDialog->Destroy();
}

ReadMeDialog::ReadMeDialog(wxWindow* parent, wxWindowID id,
    const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : ReadMeDialog_Base(parent, id, title, pos, size, style)
{
    ReadMeBox->AppendText(
"This program automatically renumbers a KiCad PCB according to various rules then back-\
annotates the renumbering scheme into the schematic hierarchy. This makes finding components\
on the PCB very easy. I recommend renumbering early in the layout process because the\
copper pour problem(see below) only shows up after updating. Also, RenumKiCadPCB\
will spot errors early. I have stress tested with \
https://github.com/OLIMEX/DIY-LAPTOP/tree/master/HARDWARE/A64-TERES/TERES-PCB1-A64-MAIN_Rev_C\
which is the most complex KiCad project I have come across. The Rev C files themselves have \
issues such as ERC and DRC errors, an excess #PWR flag, and there are blank reference \
designations on the PCB.\n\n\
Due to a bug in KiCad PCBNew([Bug 1609401] Re: PCBnew fails to properly import netlist after \
changing annotation with pours see https ://bugs.launchpad.net/kicad/+bug/1609401). If \
RenumKiCadPCB is run on a project with copper pours and a schematic netlist is created \
and imported into PCBNew errors can occur in the netlist import and subsequent DRC. This\
happens even without using RenumKiCadPCB: sometimes after changing reference designations\
on PCB and eeSchema if you then generate a netlist and re-import it into PCBNew there may \
be netlist errors and DRC errors if the board has copper pours.\n\n\
The fix is simple. Type 'B'and the pours will regenerate except where there are errors where\
they appear hatched. Note the rats nest netlist name in the hatched error zone. Edit the zone \
(hit 'E' near a zone edge) and change the pour net to the rats net netlist name. (the rats nest\
name is usually near the top of the selection window). Type 'B' again, and the pour will fill. \
Run DRC and the errors are gone.\n");
}

//
// AboutDialog methods
//
void    AboutDialog::CloseAbout( wxCommandEvent& event )
{
    (void) event;
    G_AboutDialog->Destroy();
}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id,
                          const wxString& title, const wxPoint& pos, const wxSize& size, long style )
                             : AboutDialog_Base( parent, id, title, pos, size, style )
{
    AboutBox->AppendText(

    "RenumKiCadPCB V0.352\nReleased under GPL 3.0 or later. See source for details.\
    \n\n**** No warranty : use at your own risk ****\
    \n\nWritten by Brian Piccioni. \
    \nEmail brian@documenteddesigns.com with bug reports or feature requests\
    \n\
    \nCompiled: " );

    AboutBox->AppendText( __DATE__ );
    AboutBox->AppendText( " " );
    AboutBox->AppendText( __TIME__ );
}


TextFunctions:: TextFunctions( wxWindow* parent, wxWindowID id,
                               const wxString& title, const wxPoint& pos, const wxSize& size, long style )
                                    : TextFunctions_Base( parent, id, title, pos, size, style )
{
    GetParameters( );

    if( G_PCBInputFile.buffer.empty())               //If nothing already loaded
    if( 0 != LoadPCBFile( G_PCBInputFile ))
            wxLogError("Please select a KiCad project file (*.pro) with a PCB and Schematics");        //wxLogFatalError

    G_TmpPCBBuffer = G_PCBInputFile.buffer;
    ScanPCBTextFields( );

int numfields = G_AllTextFields.size();
std::string choice;

    for(  int i = 0; i < numfields; i++ )
    {
        choice = G_AllTextFields[ i ].name + " on " + G_AllTextFields[ i ].layer;
        m_TextFieldChoice->Append( choice.c_str() );
        m_OverlayReferenceChoice->Append( choice.c_str() );
    }
    m_TextFieldChoice->SetSelection( 0 );
    m_OverlayReferenceChoice->SetSelection( 0 );
    m_SetTextAngle->SetValue( 0 == G_SetTextAngle ? false : true );
}

void TextFunctions::UpdateTextFieldCLick( wxCommandEvent& event )
{
    (void) event;
std::string subject;
std::string message;

    message.clear();
    GetParameters( );               //Find out what is to be done
    subject = G_AllTextFields[ G_TextFieldChoice ].name + " on " + G_AllTextFields[ G_TextFieldChoice ].layer;

    if( G_CenterOnModule )
        message = "Trying to center all " + subject + " on respective modules.\n";

    if( G_SetTextAngle )
        message = "Setting all " + subject + " to " + FloatFormat( G_TextAngle, 2 ) + " degrees.\n";

    if( G_OverlayReference )
        message = "Overlaying all " + subject + " with " + G_AllTextFields[ G_OverlayReferenceChoice ].name
                + " on " + G_AllTextFields[ G_OverlayReferenceChoice ].layer + '\n';

    m_TextMessageWindow->AppendText( message );
    UpdatePCBText( );               //And do it
}

void TextFunctions::SaveChangesClick( wxCommandEvent& event )
{
    (void) event;
    G_PCBInputFile.buffer = G_TmpPCBBuffer;
    SaveKiCadFile( G_PCBInputFile );
    m_TextMessageWindow->AppendText("Changes saved.\n");

}


void TextFunctions::SelectTextAngle(wxCommandEvent& event)
{
    (void)event;
    G_SetTextAngle = true;
    m_SetTextAngle->SetValue(true);
}

void TextFunctions::SelectOverlay(wxCommandEvent& event)
{
    (void)event;
    G_SetTextAngle = true;
    m_OverlayReference->SetValue(true);
}

void TextFunctions::ExitTextFieldClick( wxCommandEvent& event )
{
    (void) event;
    SetParameters( );
    delete( G_TextDialog );
}

void TextFunctions::GetParameters( void )
{
    G_TextFieldChoice = m_TextFieldChoice->GetSelection();
    G_OverlayReferenceChoice = m_OverlayReferenceChoice->GetSelection();
    G_OverlayReference = m_OverlayReference->GetValue();
    G_SetTextAngle = m_SetTextAngle->GetValue( );
    G_CenterOnModule = m_CenterOnModule->GetValue();
    if(!m_TextAngle->GetValue( ).ToDouble( &G_TextAngle )) G_TextAngle = 0;
}

void    TextFunctions::SetParameters( void )
{
    m_TextFieldChoice->SetSelection( G_TextFieldChoice );
    m_OverlayReferenceChoice->SetSelection( G_OverlayReferenceChoice );
    m_TextAngle->ChangeValue( FloatFormat( G_TextAngle, 2 ));
    m_SetTextAngle->SetValue( 0 == G_SetTextAngle ? false : true );
    m_OverlayReference->SetValue( 0 == m_OverlayReference ? false : true );
    m_CenterOnModule->SetValue( 0 == m_CenterOnModule ? false : true );

}


ContinueAbort::ContinueAbort( wxWindow* parent, wxWindowID id,
                          const wxString& title, const wxPoint& pos, const wxSize& size, long style )
                             : ContinueAbort_Base( parent, id, title, pos, size, style )
{ }

void ContinueAbort::ErrorContinue( wxCommandEvent& event )
{
    G_Errcount = 0;
    this->Destroy();
    ( void ) event;
}

void ContinueAbort::ErrorAbort( wxCommandEvent& event )
{
    ( void ) event;
    this->Destroy();
}

Abort::Abort( wxWindow* parent, wxWindowID id,
                          const wxString& title, const wxPoint& pos, const wxSize& size, long style )
                             : Abort_Base( parent, id, title, pos, size, style )
{ }

void Abort::ErrorContinue( wxCommandEvent& event )
{
    this->Destroy();
    ( void ) event;
}

//
//
// Use these to make errors and messages a bit easier
//
void    ShowError( const char *message, int arg1 = 0 )
{   G_RenumMenu->ShowError( wxString::Format( message, arg1 )); }

void    FatalError( const char *message )
{   G_RenumMenu->ShowMessage( message ); }

void    FatalError( const char *message, std::string& arg1 )
{   G_RenumMenu->FatalError( wxString::Format( message, arg1.c_str() )); }

void    ShowMessage( const char *message )
{   G_RenumMenu->ShowMessage( message ); }

void    ShowMessage( const char *message, int arg1  )
{   G_RenumMenu->ShowMessage( wxString::Format( message, arg1 )); }

void    ShowMessage( const char *message, std::string& arg1  )
{   G_RenumMenu->ShowMessage( wxString::Format( message, arg1.c_str() )); }

void    ShowMessage( const char *message, std::string& arg1, std::string& arg2 )
{   G_RenumMenu->ShowMessage( wxString::Format( message, arg1.c_str(), arg2.c_str() )); }

void    ShowWarning( const char *message, int arg1  )
{
    G_RenumMenu->SetStyle( "RED");
    G_RenumMenu->ShowMessage( wxString::Format( message, arg1 ));
    G_RenumMenu->SetStyle( "BLACK");
}

void    ShowWarning( const char *message, std::string& arg1  )
{
    G_RenumMenu->SetStyle( "RED");
    G_RenumMenu->ShowMessage( wxString::Format( message, arg1.c_str() ));
    G_RenumMenu->SetStyle( "BLACK");
}
void    ShowWarning( const char *message, std::string& arg1, std::string& arg2  )
{
    G_RenumMenu->SetStyle( "RED");
    G_RenumMenu->ShowMessage( wxString::Format( message, arg1.c_str(), arg2.c_str() ));
    G_RenumMenu->SetStyle( "BLACK");
}
//
