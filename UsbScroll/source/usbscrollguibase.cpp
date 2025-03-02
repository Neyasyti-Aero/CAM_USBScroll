///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "odrawingpanel.h"

#include "usbscrollguibase.h"

#include "../resources/flip_horz.png.h"
#include "../resources/flip_vert.png.h"
#include "../resources/image32x32.png.h"
#include "../resources/log.png.h"
#include "../resources/settings32x32.png.h"
#include "../resources/start32x32.png.h"
#include "../resources/text32x32.png.h"

///////////////////////////////////////////////////////////////////////////

UsbScrollMainFrameBase::UsbScrollMainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_poDrawingPanel = new ODrawingPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxFULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL );
	m_poDrawingPanel->SetScrollRate( 5, 5 );
	bSizer1->Add( m_poDrawingPanel, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menubar1 = new wxMenuBar( 0 );
	m_poMenuFile = new wxMenu();
	wxMenuItem* m_poMenuFileSaveFrameAs;
	m_poMenuFileSaveFrameAs = new wxMenuItem( m_poMenuFile, ID_MENU_FILE_SAVE_FRAME_AS, wxString( wxT("Save Frame As...") ) , wxEmptyString, wxITEM_NORMAL );
	m_poMenuFile->Append( m_poMenuFileSaveFrameAs );
	
	m_menubar1->Append( m_poMenuFile, wxT("File") ); 
	
	m_poMenuCamera = new wxMenu();
	m_poMenuCameraStartStopStream = new wxMenuItem( m_poMenuCamera, ID_MENU_CAMERA_STARTSTOP_STREAM, wxString( wxT("Start-Stop Stream") ) + wxT('\t') + wxT("CTRL+R"), wxT("Start/Stop video stream"), wxITEM_CHECK );
	m_poMenuCamera->Append( m_poMenuCameraStartStopStream );
	
	m_poMenuCamera->AppendSeparator();
	
	wxMenuItem* m_poMenuCameraShowImages;
	m_poMenuCameraShowImages = new wxMenuItem( m_poMenuCamera, ID_MENU_CAMERA_SHOW_IMAGES, wxString( wxT("Show Frames ") ) + wxT('\t') + wxT("CTRL+F"), wxT("Draw video frames"), wxITEM_CHECK );
	m_poMenuCamera->Append( m_poMenuCameraShowImages );
	
	wxMenuItem* m_poMenuCameraShowText;
	m_poMenuCameraShowText = new wxMenuItem( m_poMenuCamera, ID_MENU_CAMERA_SHOW_TEXT, wxString( wxT("Show Info") ) + wxT('\t') + wxT("CTRL+T"), wxT("Display panel with text information from frame"), wxITEM_CHECK );
	m_poMenuCamera->Append( m_poMenuCameraShowText );
	
	m_poMenuCamera->AppendSeparator();
	
	m_poMenuCameraProperties = new wxMenuItem( m_poMenuCamera, ID_MENU_CAMERA_PROPERTIES, wxString( wxT("Settings") ) + wxT('\t') + wxT("CTRL+S"), wxT("Set cameras settings"), wxITEM_CHECK );
	m_poMenuCamera->Append( m_poMenuCameraProperties );
	
	m_menubar1->Append( m_poMenuCamera, wxT("Camera") ); 
	
	m_poMenuSettings = new wxMenu();
	wxMenuItem* m_poMenuSettingsFlipH;
	m_poMenuSettingsFlipH = new wxMenuItem( m_poMenuSettings, ID_MENU_SETTINGS_FLIP_H, wxString( wxT("Flip Horizontally") ) , wxT("Flip image horizontally"), wxITEM_CHECK );
	m_poMenuSettings->Append( m_poMenuSettingsFlipH );
	
	wxMenuItem* m_poMenuSettingsFlipV;
	m_poMenuSettingsFlipV = new wxMenuItem( m_poMenuSettings, ID_MENU_SETTINGS_FLIP_V, wxString( wxT("Flip Vertically") ) , wxT("Flip image vertically"), wxITEM_CHECK );
	m_poMenuSettings->Append( m_poMenuSettingsFlipV );
	
	m_poMenuSettings->AppendSeparator();
	
	wxMenuItem* m_poMenuSettingsEnableLogging;
	m_poMenuSettingsEnableLogging = new wxMenuItem( m_poMenuSettings, ID_MENU_SETTINGS_ENABLE_LOGGING, wxString( wxT("Enable Logging") ) , wxT("Enables logging functionality"), wxITEM_CHECK );
	m_poMenuSettings->Append( m_poMenuSettingsEnableLogging );
	
	wxMenuItem* m_poMenuSettingsShowLog;
	m_poMenuSettingsShowLog = new wxMenuItem( m_poMenuSettings, ID_MENU_SETTINGS_SHOW_LOG, wxString( wxT("Show Log Window") ) , wxT("Shows Log window"), wxITEM_CHECK );
	m_poMenuSettings->Append( m_poMenuSettingsShowLog );
	
	m_poMenuSettings->AppendSeparator();
	
	m_poMenuSettingsUART = new wxMenu();
	wxMenuItem* m_poMenuSettingsUARTItem = new wxMenuItem( m_poMenuSettings, wxID_ANY, wxT("UART"), wxEmptyString, wxITEM_NORMAL, m_poMenuSettingsUART );
	wxMenuItem* m_poMenuSettingsUARTConfigureParams;
	m_poMenuSettingsUARTConfigureParams = new wxMenuItem( m_poMenuSettingsUART, ID_MENU_SETTINGS_UART_SET_LOG_FILE, wxString( wxT("Configure Parameters...") ) , wxEmptyString, wxITEM_NORMAL );
	m_poMenuSettingsUART->Append( m_poMenuSettingsUARTConfigureParams );
	
	m_poMenuSettingsUART->AppendSeparator();
	
	wxMenuItem* m_poMenuSettingsUARTModeNone;
	m_poMenuSettingsUARTModeNone = new wxMenuItem( m_poMenuSettingsUART, ID_MENU_SETTINGS_UART_MODE_OFF, wxString( wxT("Off") ) , wxEmptyString, wxITEM_CHECK );
	m_poMenuSettingsUART->Append( m_poMenuSettingsUARTModeNone );
	m_poMenuSettingsUARTModeNone->Check( true );
	
	wxMenuItem* m_poMenuSettingsUARTModeLogging;
	m_poMenuSettingsUARTModeLogging = new wxMenuItem( m_poMenuSettingsUART, ID_MENU_SETTINGS_UART_MODE_LOGGING, wxString( wxT("Logging") ) , wxEmptyString, wxITEM_CHECK );
	m_poMenuSettingsUART->Append( m_poMenuSettingsUARTModeLogging );
	
	wxMenuItem* m_poMenuSettingsUARTModeSelfTest;
	m_poMenuSettingsUARTModeSelfTest = new wxMenuItem( m_poMenuSettingsUART, ID_MENU_SETTINGS_UART_MODE_TEST, wxString( wxT("Self Test Channel Connection") ) , wxEmptyString, wxITEM_CHECK );
	m_poMenuSettingsUART->Append( m_poMenuSettingsUARTModeSelfTest );
	
	m_poMenuSettings->Append( m_poMenuSettingsUARTItem );
	
	m_poMenuSettings->AppendSeparator();
	
	wxMenuItem* m_poMenuSettingsDisableDecoding;
	m_poMenuSettingsDisableDecoding = new wxMenuItem( m_poMenuSettings, ID_MENU_SETTINGS_DISABLE_DECODING, wxString( wxT("Disable Decoding") ) , wxEmptyString, wxITEM_CHECK );
	m_poMenuSettings->Append( m_poMenuSettingsDisableDecoding );
	
	m_menubar1->Append( m_poMenuSettings, wxT("Settings") ); 
	
	m_poMenuHelp = new wxMenu();
	wxMenuItem* m_poMenuHelpAbout;
	m_poMenuHelpAbout = new wxMenuItem( m_poMenuHelp, ID_MENU_HELP_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_poMenuHelp->Append( m_poMenuHelpAbout );
	
	m_menubar1->Append( m_poMenuHelp, wxT("Help") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	m_poToolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_poToolCameraStartStopStream = m_poToolBar->AddTool( ID_MENU_CAMERA_STARTSTOP_STREAM, wxT("Start-Stop Stream"), start32x32_png_to_wx_bitmap(), wxNullBitmap, wxITEM_CHECK, wxT("Start-Stop Camera Stream"), wxT("Start-Stop Camera Stream"), NULL ); 
	
	m_poToolBar->AddSeparator(); 
	
	m_poToolCameraShowImages = m_poToolBar->AddTool( ID_MENU_CAMERA_SHOW_IMAGES, wxT("tool"), image32x32_png_to_wx_bitmap(), wxNullBitmap, wxITEM_CHECK, wxT("Draw video frames"), wxT("Draw video frames"), NULL ); 
	
	m_poToolCameraShowText = m_poToolBar->AddTool( ID_MENU_CAMERA_SHOW_TEXT, wxT("Show Info"), text32x32_png_to_wx_bitmap(), wxNullBitmap, wxITEM_CHECK, wxT("Display panel with text information from frame"), wxT("Display panel with text information from frame"), NULL ); 
	
	m_poToolSettingsShowLog = m_poToolBar->AddTool( ID_MENU_SETTINGS_SHOW_LOG, wxT("tool"), log_png_to_wx_bitmap(), wxNullBitmap, wxITEM_CHECK, wxT("Shows Log window"), wxT("Shows Log window"), NULL ); 
	
	m_poToolBar->AddSeparator(); 
	
	wxToolBarToolBase* m_poToolCameraProperties; 
	m_poToolCameraProperties = m_poToolBar->AddTool( ID_MENU_CAMERA_PROPERTIES, wxT("Properties"), settings32x32_png_to_wx_bitmap(), wxNullBitmap, wxITEM_CHECK, wxT("Set Camera Properties"), wxT("Set Camera Properties"), NULL ); 
	
	m_poToolBar->AddSeparator(); 
	
	m_poToolSettingsFlipH = m_poToolBar->AddTool( ID_MENU_SETTINGS_FLIP_H, wxT("Flip Horizontally"), flip_horz_png_to_wx_bitmap(), wxNullBitmap, wxITEM_CHECK, wxT("Flip image horizontally"), wxT("Flip image horizontally"), NULL ); 
	
	m_poToolSettingsFlipV = m_poToolBar->AddTool( ID_MENU_SETTINGS_FLIP_V, wxT("Flip Vertically"), flip_vert_png_to_wx_bitmap(), wxNullBitmap, wxITEM_CHECK, wxT("Flip image vertically"), wxT("Flip image vertically"), NULL ); 
	
	m_poToolBar->Realize(); 
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( UsbScrollMainFrameBase::OnClose ) );
	this->Connect( m_poMenuFileSaveFrameAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnFileSaveFrameAs ) );
	this->Connect( m_poMenuCameraStartStopStream->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnCameraStartStopStream ) );
	this->Connect( m_poMenuCameraShowImages->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnCameraShowImages ) );
	this->Connect( m_poMenuCameraShowText->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnCameraShowText ) );
	this->Connect( m_poMenuCameraProperties->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnCameraProperties ) );
	this->Connect( m_poMenuSettingsFlipH->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsFlipH ) );
	this->Connect( m_poMenuSettingsFlipV->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsFlipV ) );
	this->Connect( m_poMenuSettingsEnableLogging->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsEnableLogging ) );
	this->Connect( m_poMenuSettingsShowLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsShowLog ) );
	this->Connect( m_poMenuSettingsUARTConfigureParams->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsUARTConfigureParams ) );
	this->Connect( m_poMenuSettingsUARTModeNone->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnUARTMode ) );
	this->Connect( m_poMenuSettingsUARTModeLogging->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnUARTMode ) );
	this->Connect( m_poMenuSettingsUARTModeSelfTest->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnUARTMode ) );
	this->Connect( m_poMenuSettingsDisableDecoding->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsDisableDecoding ) );
	this->Connect( m_poMenuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnMenuHelpAbout ) );
}

UsbScrollMainFrameBase::~UsbScrollMainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( UsbScrollMainFrameBase::OnClose ) );
	this->Disconnect( ID_MENU_FILE_SAVE_FRAME_AS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnFileSaveFrameAs ) );
	this->Disconnect( ID_MENU_CAMERA_STARTSTOP_STREAM, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnCameraStartStopStream ) );
	this->Disconnect( ID_MENU_CAMERA_SHOW_IMAGES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnCameraShowImages ) );
	this->Disconnect( ID_MENU_CAMERA_SHOW_TEXT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnCameraShowText ) );
	this->Disconnect( ID_MENU_CAMERA_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnCameraProperties ) );
	this->Disconnect( ID_MENU_SETTINGS_FLIP_H, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsFlipH ) );
	this->Disconnect( ID_MENU_SETTINGS_FLIP_V, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsFlipV ) );
	this->Disconnect( ID_MENU_SETTINGS_ENABLE_LOGGING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsEnableLogging ) );
	this->Disconnect( ID_MENU_SETTINGS_SHOW_LOG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsShowLog ) );
	this->Disconnect( ID_MENU_SETTINGS_UART_SET_LOG_FILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsUARTConfigureParams ) );
	this->Disconnect( ID_MENU_SETTINGS_UART_MODE_OFF, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnUARTMode ) );
	this->Disconnect( ID_MENU_SETTINGS_UART_MODE_LOGGING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnUARTMode ) );
	this->Disconnect( ID_MENU_SETTINGS_UART_MODE_TEST, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnUARTMode ) );
	this->Disconnect( ID_MENU_SETTINGS_DISABLE_DECODING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnSettingsDisableDecoding ) );
	this->Disconnect( ID_MENU_HELP_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UsbScrollMainFrameBase::OnMenuHelpAbout ) );
	
}

ODevicePropertiesFrmBase::ODevicePropertiesFrmBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_poScrolledArea = new wxScrolledWindow( this, ID_SCOLLED_DEVICE_PROPERTIES_AREA, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_poScrolledArea->SetScrollRate( 5, 5 );
	wxBoxSizer* poScrolledAreaSizer;
	poScrolledAreaSizer = new wxBoxSizer( wxVERTICAL );
	
	m_poMainNotebook = new wxNotebook( m_poScrolledArea, ID_MAIN_NOTEBOOK, wxDefaultPosition, wxDefaultSize, 0 );
	m_poDevicePanel = new wxPanel( m_poMainNotebook, ID_DEVICE_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* devicePanelSizer;
	devicePanelSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* selectDeviceSzr;
	selectDeviceSzr = new wxStaticBoxSizer( new wxStaticBox( m_poDevicePanel, wxID_ANY, wxT("Select Device") ), wxVERTICAL );
	
	wxBoxSizer* deviceListSizer;
	deviceListSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_poDevicesListChChoices;
	m_poDevicesListCh = new wxChoice( selectDeviceSzr->GetStaticBox(), ID_DEVICES_LIST_CH, wxDefaultPosition, wxSize( 0,-1 ), m_poDevicesListChChoices, 0 );
	m_poDevicesListCh->SetSelection( 0 );
	m_poDevicesListCh->SetMinSize( wxSize( 100,-1 ) );
	
	deviceListSizer->Add( m_poDevicesListCh, 1, wxALL, 5 );
	
	m_poGetDevicesListBtn = new wxButton( selectDeviceSzr->GetStaticBox(), ID_GET_DEVICES_LIST_BTN, wxT("Get Devices List"), wxDefaultPosition, wxDefaultSize, 0 );
	deviceListSizer->Add( m_poGetDevicesListBtn, 0, wxALL, 5 );
	
	
	selectDeviceSzr->Add( deviceListSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* modelSizer;
	modelSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* modelText;
	modelText = new wxStaticText( selectDeviceSzr->GetStaticBox(), wxID_ANY, wxT("Model"), wxDefaultPosition, wxDefaultSize, 0 );
	modelText->Wrap( -1 );
	modelSizer->Add( modelText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_poModelTxt = new wxTextCtrl( selectDeviceSzr->GetStaticBox(), ID_MODEL_TXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	modelSizer->Add( m_poModelTxt, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxStaticText* serialText;
	serialText = new wxStaticText( selectDeviceSzr->GetStaticBox(), wxID_ANY, wxT("Serial"), wxDefaultPosition, wxDefaultSize, 0 );
	serialText->Wrap( -1 );
	modelSizer->Add( serialText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_poSerialTxt = new wxTextCtrl( selectDeviceSzr->GetStaticBox(), ID_SERIAL_TXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	modelSizer->Add( m_poSerialTxt, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	selectDeviceSzr->Add( modelSizer, 0, wxEXPAND, 5 );
	
	
	devicePanelSizer->Add( selectDeviceSzr, 0, wxEXPAND, 5 );
	
	m_poBitsAndSaveSizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_poBitsAndSaveSizer->Add( 20, 0, 0, 0, 5 );
	
	m_poSaveSettingsBtn = new wxButton( m_poDevicePanel, ID_SAVE_SETTINGS_BTN, wxT("Save Settings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_poBitsAndSaveSizer->Add( m_poSaveSettingsBtn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	devicePanelSizer->Add( m_poBitsAndSaveSizer, 0, wxEXPAND, 5 );
	
	m_poImageOptionsGB = new wxStaticBoxSizer( new wxStaticBox( m_poDevicePanel, wxID_ANY, wxT("Image Options") ), wxHORIZONTAL );
	
	
	m_poImageOptionsGB->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_poMirrorImageChB = new wxCheckBox( m_poImageOptionsGB->GetStaticBox(), ID_MIRRORIMAGE_CHB, wxT("Mirror"), wxDefaultPosition, wxDefaultSize, 0 );
	m_poImageOptionsGB->Add( m_poMirrorImageChB, 0, wxALL, 5 );
	
	
	m_poImageOptionsGB->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_poFlipImageChB = new wxCheckBox( m_poImageOptionsGB->GetStaticBox(), ID_FLIPIMAGE_CHB, wxT("Flip"), wxDefaultPosition, wxDefaultSize, 0 );
	m_poImageOptionsGB->Add( m_poFlipImageChB, 0, wxALL, 5 );
	
	
	m_poImageOptionsGB->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_poFDGainChB = new wxCheckBox( m_poImageOptionsGB->GetStaticBox(), ID_FDGAIN_CHB, wxT("FD Gain"), wxDefaultPosition, wxDefaultSize, 0 );
	m_poImageOptionsGB->Add( m_poFDGainChB, 0, wxALL, 5 );
	
	
	m_poImageOptionsGB->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	devicePanelSizer->Add( m_poImageOptionsGB, 0, wxEXPAND, 5 );
	
	m_poModeGB = new wxStaticBoxSizer( new wxStaticBox( m_poDevicePanel, wxID_ANY, wxT("Mode") ), wxVERTICAL );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_poFormatChsChoices;
	m_poFormatChs = new wxChoice( m_poModeGB->GetStaticBox(), ID_FORMAT_CHS, wxDefaultPosition, wxDefaultSize, m_poFormatChsChoices, 0 );
	m_poFormatChs->SetSelection( 0 );
	bSizer12->Add( m_poFormatChs, 1, wxALL, 5 );
	
	
	bSizer12->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_poApplyScanAreaBtn = new wxButton( m_poModeGB->GetStaticBox(), ID_APPLY_SCAN_AREA_BTN, wxT("Apply Scan Area"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_poApplyScanAreaBtn, 0, wxALL, 5 );
	
	
	m_poModeGB->Add( bSizer12, 0, wxEXPAND, 5 );
	
	m_oScanAreaSizer = new wxFlexGridSizer( 5, 2, 0, 0 );
	m_oScanAreaSizer->AddGrowableCol( 1 );
	m_oScanAreaSizer->SetFlexibleDirection( wxHORIZONTAL );
	m_oScanAreaSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_poModeGB->Add( m_oScanAreaSizer, 0, wxEXPAND, 5 );
	
	
	devicePanelSizer->Add( m_poModeGB, 0, wxEXPAND, 5 );
	
	m_poTriggerModeSizer = new wxStaticBoxSizer( new wxStaticBox( m_poDevicePanel, wxID_ANY, wxT("Trigger Mode") ), wxVERTICAL );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	m_poTriggerOnOffChb = new wxCheckBox( m_poTriggerModeSizer->GetStaticBox(), wxID_ANY, wxT("TRIGGER ON/OFF"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_poTriggerOnOffChb, 0, wxALL, 5 );
	
	
	bSizer16->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_poInversePolarityChb = new wxCheckBox( m_poTriggerModeSizer->GetStaticBox(), wxID_ANY, wxT("Inverse Polarity"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_poInversePolarityChb, 0, wxALL, 5 );
	
	
	m_poTriggerModeSizer->Add( bSizer16, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	m_poTriggerModeHardwareShutterRB = new wxRadioButton( m_poTriggerModeSizer->GetStaticBox(), wxID_ANY, wxT("Hardware input. Exposure = Shutter Reg"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer18->Add( m_poTriggerModeHardwareShutterRB, 0, wxALL, 5 );
	
	m_poTriggerModeHardwareSignalRB = new wxRadioButton( m_poTriggerModeSizer->GetStaticBox(), wxID_ANY, wxT("Hardware input. Exposure = Signal Active"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_poTriggerModeHardwareSignalRB, 0, wxALL, 5 );
	
	m_poTriggerModeInternalSoftwareRB = new wxRadioButton( m_poTriggerModeSizer->GetStaticBox(), wxID_ANY, wxT("Internal Soft. Exposure = Shutter Reg"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_poTriggerModeInternalSoftwareRB, 0, wxALL, 5 );
	
	
	bSizer17->Add( bSizer18, 0, wxEXPAND, 5 );
	
	
	bSizer17->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_poGetFrameBtn = new wxButton( m_poTriggerModeSizer->GetStaticBox(), wxID_ANY, wxT("Get Frame"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_poGetFrameBtn, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	
	m_poTriggerModeSizer->Add( bSizer17, 1, wxEXPAND, 5 );
	
	
	devicePanelSizer->Add( m_poTriggerModeSizer, 0, wxEXPAND, 5 );
	
	
	m_poDevicePanel->SetSizer( devicePanelSizer );
	m_poDevicePanel->Layout();
	devicePanelSizer->Fit( m_poDevicePanel );
	m_poMainNotebook->AddPage( m_poDevicePanel, wxT("Device"), true );
	m_poSignal = new wxPanel( m_poMainNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_poSignalSizerTop = new wxBoxSizer( wxVERTICAL );
	
	m_poSignalSizer = new wxBoxSizer( wxVERTICAL );
	
	
	m_poSignalSizerTop->Add( m_poSignalSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_poSignal, wxID_ANY, wxT("Max Iris Value") ), wxHORIZONTAL );
	
	m_poMaxIrisValueTC = new wxTextCtrl( sbSizer5->GetStaticBox(), ID_MAX_IRIS_VALUE_TC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer5->Add( m_poMaxIrisValueTC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_poMaxIrisValueSetBtn = new wxButton( sbSizer5->GetStaticBox(), ID_MAX_IRIS_VALUE_SET_BTN, wxT("Set Value"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer5->Add( m_poMaxIrisValueSetBtn, 0, wxALL, 5 );
	
	
	bSizer11->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_poSignal, wxID_ANY, wxT("Direct Shutter Values") ), wxHORIZONTAL );
	
	m_poDirectShutterValueTC = new wxTextCtrl( sbSizer6->GetStaticBox(), ID_DIRECT_SHUTTER_VALUE_TC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_poDirectShutterValueTC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_poDirectShutterValueSetBtn = new wxButton( sbSizer6->GetStaticBox(), ID_DIRECT_SHUTTER_VALUE_SET_BTN, wxT("Set Value"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_poDirectShutterValueSetBtn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer11->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	
	m_poSignalSizerTop->Add( bSizer11, 0, wxEXPAND, 5 );
	
	
	m_poSignal->SetSizer( m_poSignalSizerTop );
	m_poSignal->Layout();
	m_poSignalSizerTop->Fit( m_poSignal );
	m_poMainNotebook->AddPage( m_poSignal, wxT("Signal"), false );
	m_poColor = new wxPanel( m_poMainNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_poColorSizer = new wxBoxSizer( wxVERTICAL );
	
	
	m_poColor->SetSizer( m_poColorSizer );
	m_poColor->Layout();
	m_poColorSizer->Fit( m_poColor );
	m_poMainNotebook->AddPage( m_poColor, wxT("Color"), false );
	m_poCompress = new wxPanel( m_poMainNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_poCompressSizer = new wxBoxSizer( wxVERTICAL );
	
	
	m_poCompress->SetSizer( m_poCompressSizer );
	m_poCompress->Layout();
	m_poCompressSizer->Fit( m_poCompress );
	m_poMainNotebook->AddPage( m_poCompress, wxT("Compress"), false );
	m_poAdvanced = new wxPanel( m_poMainNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_poAdvancedSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_poAdvanced, wxID_ANY, wxT("Temperature") ), wxHORIZONTAL );
	
	m_poTemperaturetTC = new wxTextCtrl( sbSizer8->GetStaticBox(), ID_TEMPERATURE_TC, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	sbSizer8->Add( m_poTemperaturetTC, 0, wxALL, 5 );
	
	
	sbSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_poGetTemperatureBtn = new wxButton( sbSizer8->GetStaticBox(), ID_GET_TEMPERATURE_BTN, wxT("Get Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer8->Add( m_poGetTemperatureBtn, 0, wxALL, 5 );
	
	
	m_poAdvancedSizer->Add( sbSizer8, 0, wxEXPAND, 5 );
	
	m_poOutputControlsGB = new wxStaticBoxSizer( new wxStaticBox( m_poAdvanced, wxID_ANY, wxT("Output Controls") ), wxVERTICAL );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_poOut0R = new wxRadioButton( m_poOutputControlsGB->GetStaticBox(), wxID_ANY, wxT("OUT0"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer19->Add( m_poOut0R, 0, wxALL, 5 );
	
	
	bSizer19->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_poOut1R = new wxRadioButton( m_poOutputControlsGB->GetStaticBox(), wxID_ANY, wxT("OUT1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_poOut1R, 0, wxALL, 5 );
	
	
	bSizer19->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_poOut2R = new wxRadioButton( m_poOutputControlsGB->GetStaticBox(), wxID_ANY, wxT("OUT2"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_poOut2R, 0, wxALL, 5 );
	
	
	bSizer19->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_poOut3R = new wxRadioButton( m_poOutputControlsGB->GetStaticBox(), wxID_ANY, wxT("OUT3"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_poOut3R, 0, wxALL, 5 );
	
	
	m_poOutputControlsGB->Add( bSizer19, 0, wxEXPAND, 5 );
	
	m_poOutputTimesSizer = new wxStaticBoxSizer( new wxStaticBox( m_poOutputControlsGB->GetStaticBox(), wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_poEnableDelayTimingCh = new wxCheckBox( m_poOutputTimesSizer->GetStaticBox(), ID_ENABLE_DELAY_TIMING_CH, wxT("Enable delay timing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_poOutputTimesSizer->Add( m_poEnableDelayTimingCh, 0, wxALL, 5 );
	
	
	m_poOutputControlsGB->Add( m_poOutputTimesSizer, 1, wxEXPAND, 5 );
	
	
	m_poAdvancedSizer->Add( m_poOutputControlsGB, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_poAdvanced, wxID_ANY, wxT("Stream0 Params") ), wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( sbSizer4->GetStaticBox(), wxID_ANY, wxT("Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	sbSizer4->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_poPortStream0TC = new wxTextCtrl( sbSizer4->GetStaticBox(), ID_PORT_TC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_poPortStream0TC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	sbSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( sbSizer4->GetStaticBox(), wxID_ANY, wxT("Packet Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	sbSizer4->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_poPacketSizeStream0TC = new wxTextCtrl( sbSizer4->GetStaticBox(), ID_PACKET_SIZE_TC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_poPacketSizeStream0TC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_poApplyStream0Btn = new wxButton( sbSizer4->GetStaticBox(), ID_APPLY_STREAM0_BTN, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_poApplyStream0Btn, 0, wxALL, 5 );
	
	
	m_poAdvancedSizer->Add( sbSizer4, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_poSetLUTTableBtn = new wxButton( m_poAdvanced, ID_SET_LUT_TABLE_BUTTON, wxT("Set LUT Table"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_poSetLUTTableBtn, 0, wxALL, 5 );
	
	m_poGetLUTTableBtn = new wxButton( m_poAdvanced, ID_GET_LUT_TABLE_BUTTON, wxT("Get LUT Table"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_poGetLUTTableBtn, 0, wxALL, 5 );
	
	m_poLUTGetResultTxt = new wxStaticText( m_poAdvanced, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_poLUTGetResultTxt->Wrap( -1 );
	bSizer20->Add( m_poLUTGetResultTxt, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_poAdvancedSizer->Add( bSizer20, 0, wxEXPAND, 5 );
	
	
	m_poAdvanced->SetSizer( m_poAdvancedSizer );
	m_poAdvanced->Layout();
	m_poAdvancedSizer->Fit( m_poAdvanced );
	m_poMainNotebook->AddPage( m_poAdvanced, wxT("Advanced"), false );
	
	poScrolledAreaSizer->Add( m_poMainNotebook, 1, wxEXPAND | wxALL, 5 );
	
	
	m_poScrolledArea->SetSizer( poScrolledAreaSizer );
	m_poScrolledArea->Layout();
	poScrolledAreaSizer->Fit( m_poScrolledArea );
	mainSizer->Add( m_poScrolledArea, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODevicePropertiesFrmBase::OnFrameClose ) );
	m_poDevicesListCh->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnDevicesListChoice ), NULL, this );
	m_poGetDevicesListBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnGetDevicesListButtonClick ), NULL, this );
	m_poSaveSettingsBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnSaveSettingsBtnClick ), NULL, this );
	m_poMirrorImageChB->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnImageMirrorFlipFDGainChB ), NULL, this );
	m_poFlipImageChB->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnImageMirrorFlipFDGainChB ), NULL, this );
	m_poFDGainChB->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnImageMirrorFlipFDGainChB ), NULL, this );
	m_poFormatChs->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnFormatSelectionChanged ), NULL, this );
	m_poApplyScanAreaBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnApplyScanArea ), NULL, this );
	m_poTriggerOnOffChb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerOnOffChb ), NULL, this );
	m_poInversePolarityChb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerPolarityChb ), NULL, this );
	m_poTriggerModeHardwareShutterRB->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerModeRB ), NULL, this );
	m_poTriggerModeHardwareSignalRB->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerModeRB ), NULL, this );
	m_poTriggerModeInternalSoftwareRB->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerModeRB ), NULL, this );
	m_poGetFrameBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnGetFrameBtn ), NULL, this );
	m_poMaxIrisValueSetBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnSetMaxIrisValue ), NULL, this );
	m_poDirectShutterValueSetBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnSetDirectShutterValue ), NULL, this );
	m_poGetTemperatureBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnGetTemperature ), NULL, this );
	m_poOut0R->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnOutputRB ), NULL, this );
	m_poOut1R->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnOutputRB ), NULL, this );
	m_poOut2R->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnOutputRB ), NULL, this );
	m_poOut3R->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnOutputRB ), NULL, this );
	m_poEnableDelayTimingCh->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnEnableDelayTimingCh ), NULL, this );
	m_poApplyStream0Btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnApplyStream0BtnClick ), NULL, this );
	m_poSetLUTTableBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnSetLUTTableBtn ), NULL, this );
	m_poGetLUTTableBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnGetLUTTableBtn ), NULL, this );
}

ODevicePropertiesFrmBase::~ODevicePropertiesFrmBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODevicePropertiesFrmBase::OnFrameClose ) );
	m_poDevicesListCh->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnDevicesListChoice ), NULL, this );
	m_poGetDevicesListBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnGetDevicesListButtonClick ), NULL, this );
	m_poSaveSettingsBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnSaveSettingsBtnClick ), NULL, this );
	m_poMirrorImageChB->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnImageMirrorFlipFDGainChB ), NULL, this );
	m_poFlipImageChB->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnImageMirrorFlipFDGainChB ), NULL, this );
	m_poFDGainChB->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnImageMirrorFlipFDGainChB ), NULL, this );
	m_poFormatChs->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnFormatSelectionChanged ), NULL, this );
	m_poApplyScanAreaBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnApplyScanArea ), NULL, this );
	m_poTriggerOnOffChb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerOnOffChb ), NULL, this );
	m_poInversePolarityChb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerPolarityChb ), NULL, this );
	m_poTriggerModeHardwareShutterRB->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerModeRB ), NULL, this );
	m_poTriggerModeHardwareSignalRB->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerModeRB ), NULL, this );
	m_poTriggerModeInternalSoftwareRB->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnTriggerModeRB ), NULL, this );
	m_poGetFrameBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnGetFrameBtn ), NULL, this );
	m_poMaxIrisValueSetBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnSetMaxIrisValue ), NULL, this );
	m_poDirectShutterValueSetBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnSetDirectShutterValue ), NULL, this );
	m_poGetTemperatureBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnGetTemperature ), NULL, this );
	m_poOut0R->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnOutputRB ), NULL, this );
	m_poOut1R->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnOutputRB ), NULL, this );
	m_poOut2R->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnOutputRB ), NULL, this );
	m_poOut3R->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnOutputRB ), NULL, this );
	m_poEnableDelayTimingCh->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnEnableDelayTimingCh ), NULL, this );
	m_poApplyStream0Btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnApplyStream0BtnClick ), NULL, this );
	m_poSetLUTTableBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnSetLUTTableBtn ), NULL, this );
	m_poGetLUTTableBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODevicePropertiesFrmBase::OnGetLUTTableBtn ), NULL, this );
	
}

OFeatureControlBase::OFeatureControlBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	m_oFeatureName = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Feature") ), wxVERTICAL );
	
	m_staticText5 = new wxStaticText( m_oFeatureName->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	m_oFeatureName->Add( m_staticText5, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_poFeatureSlider = new wxSlider( m_oFeatureName->GetStaticBox(), ID_FEATURE_SLIDER, 50, 50, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer7->Add( m_poFeatureSlider, 1, wxALL, 5 );
	
	m_poFeatureValue = new wxTextCtrl( m_oFeatureName->GetStaticBox(), ID_FEATURE_VALUE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_poFeatureValue->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	bSizer7->Add( m_poFeatureValue, 0, wxALL, 5 );
	
	
	m_oFeatureName->Add( bSizer7, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_oFeatureName );
	this->Layout();
	
	// Connect Events
	m_poFeatureSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
}

OFeatureControlBase::~OFeatureControlBase()
{
	// Disconnect Events
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( OFeatureControlBase::OnScrollFeatureSlider ), NULL, this );
	
}

OFrameInfoBase::OFrameInfoBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_poTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer19->Add( m_poTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer19 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( OFrameInfoBase::OnFrameClose ) );
}

OFrameInfoBase::~OFrameInfoBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( OFrameInfoBase::OnFrameClose ) );
	
}

OUARTParamsBase::OUARTParamsBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer12;
	sbSizer12 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Logging") ), wxHORIZONTAL );
	
	m_staticText7 = new wxStaticText( sbSizer12->GetStaticBox(), wxID_ANY, wxT("Log File:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	sbSizer12->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_poLogFilePath = new wxFilePickerCtrl( sbSizer12->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_SMALL|wxFLP_USE_TEXTCTRL );
	sbSizer12->Add( m_poLogFilePath, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer25->Add( sbSizer12, 0, wxALL|wxEXPAND, 5 );
	
	wxString m_poSelfTestModeRBGChoices[] = { wxT("Shorting"), wxT("Crossover") };
	int m_poSelfTestModeRBGNChoices = sizeof( m_poSelfTestModeRBGChoices ) / sizeof( wxString );
	m_poSelfTestModeRBG = new wxRadioBox( this, ID_SELF_TEST_MODE_RBG, wxT("Self Test Channel Connection"), wxDefaultPosition, wxDefaultSize, m_poSelfTestModeRBGNChoices, m_poSelfTestModeRBGChoices, 2, wxRA_SPECIFY_COLS );
	m_poSelfTestModeRBG->SetSelection( 1 );
	bSizer25->Add( m_poSelfTestModeRBG, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Polling Interval, [ms]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer27->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_poPollingInterval = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000000, 100 );
	bSizer27->Add( m_poPollingInterval, 0, wxALL, 5 );
	
	
	bSizer25->Add( bSizer27, 0, wxEXPAND|wxALL, 5 );
	
	
	bSizer25->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	bSizer25->Add( m_sdbSizer1, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5 );
	
	
	this->SetSizer( bSizer25 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

OUARTParamsBase::~OUARTParamsBase()
{
}
