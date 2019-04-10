///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "RenumKicadPCB_Base.h"

///////////////////////////////////////////////////////////////////////////

RenumKicadPCB_Base::RenumKicadPCB_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerProjectFilename;
	bSizerProjectFilename = new wxBoxSizer( wxVERTICAL );

	wxStaticText* staticProjectFileName;
	staticProjectFileName = new wxStaticText( this, wxID_ANY, _("Project file:"), wxDefaultPosition, wxDefaultSize, 0 );
	staticProjectFileName->Wrap( -1 );
	bSizerProjectFilename->Add( staticProjectFileName, 0, wxLEFT|wxRIGHT, 5 );

	m_ProjectFile = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a KiCad project file (*.pro)"), _("*.pro"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	m_ProjectFile->SetToolTip( _("Select a valid KiCad project with PCB and root schematic with the name.\nNOTE: The PCB or SCH files associated with the project must not be open in KiCad!\n") );

	bSizerProjectFilename->Add( m_ProjectFile, 0, wxALL|wxEXPAND, 5 );


	bMainSizer->Add( bSizerProjectFilename, 0, wxEXPAND|wxTOP, 10 );

	wxGridBagSizer* gbSizer15;
	gbSizer15 = new wxGridBagSizer( 0, 0 );
	gbSizer15->SetFlexibleDirection( wxBOTH );
	gbSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_TopSortDirText = new wxStaticText( this, wxID_ANY, _("Top Sort Direction"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TopSortDirText->Wrap( -1 );
	m_TopSortDirText->SetToolTip( _("Set the direction of sort: Top to bottom, left to regith is as you read a book") );

	gbSizer15->Add( m_TopSortDirText, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	wxString m_TopSortDirChoices[] = { _("Top to bottom, left to right"), _("Top to bottom, right to left"), _("Bottom to top, left to right"), _("Bottom to top, right to left"), _("Left to right, top to bottom"), _("Left to right, bottom to top"), _("Right to left, top to bottom"), _("Right to left, bottom to top") };
	int m_TopSortDirNChoices = sizeof( m_TopSortDirChoices ) / sizeof( wxString );
	m_TopSortDir = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_TopSortDirNChoices, m_TopSortDirChoices, 0 );
	m_TopSortDir->SetSelection( 0 );
	m_TopSortDir->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	m_TopSortDir->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );

	gbSizer15->Add( m_TopSortDir, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	wxString m_BottomSortDirChoices[] = { _("Top to bottom, left to right"), _("Top to bottom, right to left"), _("Bottom to top, left to right"), _("Bottom to top, right to left"), _("Left to right, top to bottom"), _("Left to right, bottom to top"), _("Right to left, top to bottom"), _("Right to left, bottom to top") };
	int m_BottomSortDirNChoices = sizeof( m_BottomSortDirChoices ) / sizeof( wxString );
	m_BottomSortDir = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_BottomSortDirNChoices, m_BottomSortDirChoices, 0 );
	m_BottomSortDir->SetSelection( 1 );
	m_BottomSortDir->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	m_BottomSortDir->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );

	gbSizer15->Add( m_BottomSortDir, wxGBPosition( 0, 9 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_BottomSortDirText = new wxStaticText( this, wxID_ANY, _("Bottom Sort Direction"), wxDefaultPosition, wxDefaultSize, 0 );
	m_BottomSortDirText->Wrap( -1 );
	m_BottomSortDirText->SetToolTip( _("Set sort direction if looking at the solder side of the board") );

	gbSizer15->Add( m_BottomSortDirText, wxGBPosition( 0, 7 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_staticText31111 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31111->Wrap( -1 );
	gbSizer15->Add( m_staticText31111, wxGBPosition( 0, 11 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_TopRefDesStartText = new wxStaticText( this, wxID_ANY, _("Top Ref Des Start"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TopRefDesStartText->Wrap( -1 );
	m_TopRefDesStartText->SetToolTip( _("Enter a number from 1-65,536") );

	gbSizer15->Add( m_TopRefDesStartText, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_TopRefDesStart = new wxTextCtrl( this, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer15->Add( m_TopRefDesStart, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_TopPrefixText = new wxStaticText( this, wxID_ANY, _("Top Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TopPrefixText->Wrap( -1 );
	m_TopPrefixText->SetToolTip( _("Optional prefix for component side reference designations (i.e. F_)") );

	gbSizer15->Add( m_TopPrefixText, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_TopPrefixText1 = new wxStaticText( this, wxID_ANY, _("Sort Grid"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TopPrefixText1->Wrap( -1 );
	m_TopPrefixText1->SetToolTip( _("Sets the \"rounding grid\" for sort: usually smaller for less dense boards (try 1.0 to start)") );

	gbSizer15->Add( m_TopPrefixText1, wxGBPosition( 5, 1 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_BottomPrefixText = new wxStaticText( this, wxID_ANY, _("Bottom Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
	m_BottomPrefixText->Wrap( -1 );
	m_BottomPrefixText->SetToolTip( _("Optional prefix for solder side reference designations (i.e. B_)") );

	gbSizer15->Add( m_BottomPrefixText, wxGBPosition( 2, 7 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_TopPrefix = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer15->Add( m_TopPrefix, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_SortGrid = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer15->Add( m_SortGrid, wxGBPosition( 5, 2 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_SortOnModules = new wxRadioButton( this, wxID_ANY, _("Sort on Module Coordinates"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SortOnModules->SetValue( true );
	m_SortOnModules->SetToolTip( _("Sort on the origin of the module") );

	gbSizer15->Add( m_SortOnModules, wxGBPosition( 5, 7 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_SortOnRefDes = new wxRadioButton( this, wxID_ANY, _("Sort on Ref Des Coordinates"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SortOnRefDes->SetToolTip( _("Sort on the origin of the reference designation of the module") );

	gbSizer15->Add( m_SortOnRefDes, wxGBPosition( 5, 9 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_BottomPrefix = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer15->Add( m_BottomPrefix, wxGBPosition( 2, 9 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_BottomRefDesStartText = new wxStaticText( this, wxID_ANY, _("Bottom Ref Des Start\n(Blank continues from top)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_BottomRefDesStartText->Wrap( -1 );
	m_BottomRefDesStartText->SetToolTip( _("Leave blank or zero, or enter a number greater than the highest reference designation on the component side. If there R100 is the highest component on the top side, this number must be 101 or more.\n") );

	gbSizer15->Add( m_BottomRefDesStartText, wxGBPosition( 1, 7 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_BottomRefDesStart = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer15->Add( m_BottomRefDesStart, wxGBPosition( 1, 9 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_RemoveTopPrefix = new wxCheckBox( this, wxID_ANY, _("Remove Top Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RemoveTopPrefix->SetToolTip( _("If checked will remove the component side prefix") );

	gbSizer15->Add( m_RemoveTopPrefix, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_RemoveBottomPrefix = new wxCheckBox( this, wxID_ANY, _("Remove Bottom Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RemoveBottomPrefix->SetToolTip( _("If checked will remove the bottom side prefix") );

	gbSizer15->Add( m_RemoveBottomPrefix, wxGBPosition( 3, 7 ), wxGBSpan( 1, 1 ), wxALL, 1 );

	m_WriteChangeFile = new wxCheckBox( this, wxID_ANY, _("Write change file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_WriteChangeFile->SetToolTip( _("Write a \"Was/Is\" file with format Was->Is") );

	gbSizer15->Add( m_WriteChangeFile, wxGBPosition( 6, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_WriteLogFile = new wxCheckBox( this, wxID_ANY, _("Generate log file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_WriteLogFile->SetToolTip( _("Generate a log file (useful for debugging)") );

	gbSizer15->Add( m_WriteLogFile, wxGBPosition( 6, 9 ), wxGBSpan( 1, 1 ), wxALL, 5 );


	bMainSizer->Add( gbSizer15, 0, 0, 1 );

	wxBoxSizer* bLowerSizer;
	bLowerSizer = new wxBoxSizer( wxVERTICAL );

	bLowerSizer->SetMinSize( wxSize( 660,250 ) );
	m_MessageWindow = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_RICH|wxTE_RICH2 );
	bLowerSizer->Add( m_MessageWindow, 1, wxALIGN_TOP|wxALL|wxEXPAND, 5 );


	bMainSizer->Add( bLowerSizer, 1, wxEXPAND, 1 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	wxGridBagSizer* gbSizer3;
	gbSizer3 = new wxGridBagSizer( 0, 0 );
	gbSizer3->SetFlexibleDirection( wxBOTH );
	gbSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_RenumberButton = new wxButton( this, wxID_ANY, _("Renumber"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RenumberButton->SetToolTip( _("Renumber the board and back-annotate the schematics") );

	gbSizer3->Add( m_RenumberButton, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_SaveandExitButton1 = new wxButton( this, wxID_ANY, _("Save Parameters and Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SaveandExitButton1->SetToolTip( _("Save these parameters and exit") );

	gbSizer3->Add( m_SaveandExitButton1, wxGBPosition( 0, 24 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_AccessTextFunctions = new wxButton( this, wxID_ANY, _("Access Text Functions"), wxDefaultPosition, wxDefaultSize, 0 );
	m_AccessTextFunctions->SetToolTip( _("Open the Text Function dialog") );

	gbSizer3->Add( m_AccessTextFunctions, wxGBPosition( 0, 10 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_ExitButton = new wxButton( this, wxID_ANY, _("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ExitButton->SetToolTip( _("Exit without saving the parameters") );

	gbSizer3->Add( m_ExitButton, wxGBPosition( 0, 32 ), wxGBSpan( 1, 1 ), wxALL, 5 );


	bSizer9->Add( gbSizer3, 1, wxEXPAND, 5 );


	bMainSizer->Add( bSizer9, 0, wxEXPAND|wxLEFT, 5 );


	this->SetSizer( bMainSizer );
	this->Layout();
	m_menubar = new wxMenuBar( 0 );
	RenumMenuAbout = new wxMenu();
	wxMenuItem* SayAbout;
	SayAbout = new wxMenuItem( RenumMenuAbout, wxID_ANY, wxString( _("&About") ) , wxEmptyString, wxITEM_NORMAL );
	RenumMenuAbout->Append( SayAbout );

	m_menubar->Append( RenumMenuAbout, _("&About") );

	m_ReadMe = new wxMenu();
	wxMenuItem* SayReadMe;
	SayReadMe = new wxMenuItem( m_ReadMe, wxID_ANY, wxString( _("&Read") ) , wxEmptyString, wxITEM_NORMAL );
	m_ReadMe->Append( SayReadMe );

	m_menubar->Append( m_ReadMe, _("&Read Me") );

	this->SetMenuBar( m_menubar );


	this->Centre( wxBOTH );

	// Connect Events
	m_ProjectFile->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( RenumKicadPCB_Base::ProjectFileSel ), NULL, this );
	m_RenumberButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RenumKicadPCB_Base::OnRenumberClick ), NULL, this );
	m_SaveandExitButton1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RenumKicadPCB_Base::OKDone ), NULL, this );
	m_AccessTextFunctions->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RenumKicadPCB_Base::AccessText ), NULL, this );
	m_ExitButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RenumKicadPCB_Base::OnCancel ), NULL, this );
	RenumMenuAbout->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( RenumKicadPCB_Base::SayAbout ), this, SayAbout->GetId());
	m_ReadMe->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( RenumKicadPCB_Base::SayReadMe ), this, SayReadMe->GetId());
}

RenumKicadPCB_Base::~RenumKicadPCB_Base()
{
	// Disconnect Events
	m_ProjectFile->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( RenumKicadPCB_Base::ProjectFileSel ), NULL, this );
	m_RenumberButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RenumKicadPCB_Base::OnRenumberClick ), NULL, this );
	m_SaveandExitButton1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RenumKicadPCB_Base::OKDone ), NULL, this );
	m_AccessTextFunctions->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RenumKicadPCB_Base::AccessText ), NULL, this );
	m_ExitButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RenumKicadPCB_Base::OnCancel ), NULL, this );

}

ReadMeDialog_Base::ReadMeDialog_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	ReadMeBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer6->Add( ReadMeBox, 1, wxALL|wxEXPAND, 5 );

	CloseReadMeDialog = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( CloseReadMeDialog, 0, wxALIGN_CENTER|wxALL, 5 );


	this->SetSizer( bSizer6 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	CloseReadMeDialog->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ReadMeDialog_Base::CloseReadMe ), NULL, this );
}

ReadMeDialog_Base::~ReadMeDialog_Base()
{
	// Disconnect Events
	CloseReadMeDialog->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ReadMeDialog_Base::CloseReadMe ), NULL, this );

}

AboutDialog_Base::AboutDialog_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	AboutBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer6->Add( AboutBox, 1, wxALL|wxEXPAND, 5 );

	CloseAboutDialog = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( CloseAboutDialog, 0, wxALIGN_CENTER|wxALL, 5 );


	this->SetSizer( bSizer6 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	CloseAboutDialog->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialog_Base::CloseAbout ), NULL, this );
}

AboutDialog_Base::~AboutDialog_Base()
{
	// Disconnect Events
	CloseAboutDialog->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialog_Base::CloseAbout ), NULL, this );

}

ContinueAbort_Base::ContinueAbort_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	wxBoxSizer* bSizer99;
	bSizer99 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* sbErrorAbort;
	sbErrorAbort = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Errors found: continue?") ), wxHORIZONTAL );

	m_Continue = new wxButton( sbErrorAbort->GetStaticBox(), wxID_ANY, _("Write Files Anyway"), wxDefaultPosition, wxDefaultSize, 0 );
	sbErrorAbort->Add( m_Continue, 0, wxALIGN_LEFT|wxALL, 5 );

	m_Abort = new wxButton( sbErrorAbort->GetStaticBox(), wxID_ANY, _("DIscard Changes"), wxDefaultPosition, wxDefaultSize, 0 );
	sbErrorAbort->Add( m_Abort, 0, wxALIGN_RIGHT|wxALL, 5 );


	bSizer99->Add( sbErrorAbort, 1, wxALIGN_CENTER|wxEXPAND, 5 );


	this->SetSizer( bSizer99 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_Continue->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ContinueAbort_Base::ErrorContinue ), NULL, this );
	m_Abort->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ContinueAbort_Base::ErrorAbort ), NULL, this );
}

ContinueAbort_Base::~ContinueAbort_Base()
{
	// Disconnect Events
	m_Continue->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ContinueAbort_Base::ErrorContinue ), NULL, this );
	m_Abort->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ContinueAbort_Base::ErrorAbort ), NULL, this );

}

Abort_Base::Abort_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	wxBoxSizer* bSizer99;
	bSizer99 = new wxBoxSizer( wxHORIZONTAL );

	m_Continue = new wxButton( this, wxID_ANY, _("OK Try Again"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer99->Add( m_Continue, 0, wxALIGN_CENTER|wxALL, 5 );


	this->SetSizer( bSizer99 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_Continue->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Abort_Base::ErrorContinue ), NULL, this );
}

Abort_Base::~Abort_Base()
{
	// Disconnect Events
	m_Continue->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Abort_Base::ErrorContinue ), NULL, this );

}

TextFunctions_Base::TextFunctions_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	wxGridBagSizer* gbSizer3;
	gbSizer3 = new wxGridBagSizer( 0, 0 );
	gbSizer3->SetFlexibleDirection( wxBOTH );
	gbSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_TextFieldtoModify = new wxStaticText( this, wxID_ANY, _("Text Field to Modify"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TextFieldtoModify->Wrap( -1 );
	m_TextFieldtoModify->SetToolTip( _("This field on this layer will be acted on") );

	gbSizer3->Add( m_TextFieldtoModify, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxArrayString m_TextFieldChoiceChoices;
	m_TextFieldChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_TextFieldChoiceChoices, 0 );
	m_TextFieldChoice->SetSelection( 0 );
	gbSizer3->Add( m_TextFieldChoice, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_SetTextAngle = new wxRadioButton( this, wxID_ANY, _("Set Text Angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SetTextAngle->SetToolTip( _("Set all Text Field selected above to this angle (0 is horizontal)") );

	gbSizer3->Add( m_SetTextAngle, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_TextAngle = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer3->Add( m_TextAngle, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_OverlayReference = new wxRadioButton( this, wxID_ANY, _("Overlay With This Field"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OverlayReference->SetToolTip( _("Overlay Text Field to Modify with the one selected here") );

	gbSizer3->Add( m_OverlayReference, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_CenterOnModule = new wxRadioButton( this, wxID_ANY, _("Center On Module"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CenterOnModule->SetToolTip( _("Try and center the Text Field to Modfy on the module") );

	gbSizer3->Add( m_CenterOnModule, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxArrayString m_OverlayReferenceChoiceChoices;
	m_OverlayReferenceChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_OverlayReferenceChoiceChoices, 0 );
	m_OverlayReferenceChoice->SetSelection( 0 );
	gbSizer3->Add( m_OverlayReferenceChoice, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );


	bSizer8->Add( gbSizer3, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxHORIZONTAL );

	wxGridBagSizer* gbSizer31;
	gbSizer31 = new wxGridBagSizer( 0, 0 );
	gbSizer31->SetFlexibleDirection( wxBOTH );
	gbSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_UpdateTextButton = new wxButton( this, wxID_ANY, _("Update Text Fields"), wxDefaultPosition, wxDefaultSize, 0 );
	m_UpdateTextButton->SetToolTip( _("Update Text Field to Modify as indicated (cannot be undone)") );

	gbSizer31->Add( m_UpdateTextButton, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 2 );

	m_SaveTextButton = new wxButton( this, wxID_ANY, _("Save Changes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SaveTextButton->SetToolTip( _("Save the changes I made to the PCB and write them out (last chance!)") );

	gbSizer31->Add( m_SaveTextButton, wxGBPosition( 0, 7 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 2 );

	m_ExitTextButton = new wxButton( this, wxID_ANY, _("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ExitTextButton->SetToolTip( _("Exit without updating the PCB") );

	gbSizer31->Add( m_ExitTextButton, wxGBPosition( 0, 13 ), wxGBSpan( 1, 1 ), wxALL, 2 );


	bSizer91->Add( gbSizer31, 1, wxEXPAND, 5 );


	bSizer8->Add( bSizer91, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_TextMessageWindow = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_RICH|wxTE_RICH2 );
	bSizer9->Add( m_TextMessageWindow, 10, wxALIGN_TOP|wxALL|wxEXPAND, 5 );


	bSizer8->Add( bSizer9, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer8 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_TextAngle->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TextFunctions_Base::SelectTextAngle ), NULL, this );
	m_OverlayReferenceChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TextFunctions_Base::SelectOverlay ), NULL, this );
	m_UpdateTextButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextFunctions_Base::UpdateTextFieldCLick ), NULL, this );
	m_SaveTextButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextFunctions_Base::SaveChangesClick ), NULL, this );
	m_ExitTextButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextFunctions_Base::ExitTextFieldClick ), NULL, this );
}

TextFunctions_Base::~TextFunctions_Base()
{
	// Disconnect Events
	m_TextAngle->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TextFunctions_Base::SelectTextAngle ), NULL, this );
	m_OverlayReferenceChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TextFunctions_Base::SelectOverlay ), NULL, this );
	m_UpdateTextButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextFunctions_Base::UpdateTextFieldCLick ), NULL, this );
	m_SaveTextButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextFunctions_Base::SaveChangesClick ), NULL, this );
	m_ExitTextButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextFunctions_Base::ExitTextFieldClick ), NULL, this );

}
