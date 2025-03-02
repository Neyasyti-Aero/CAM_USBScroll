///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __USBSCROLLGUIBASE_H__
#define __USBSCROLLGUIBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class ODrawingPanel;

#include <wx/scrolwin.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/slider.h>
#include <wx/filepicker.h>
#include <wx/radiobox.h>
#include <wx/spinctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class UsbScrollMainFrameBase
///////////////////////////////////////////////////////////////////////////////
class UsbScrollMainFrameBase : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			ID_MENU_FILE_SAVE_FRAME_AS = 1001,
			ID_MENU_CAMERA_STARTSTOP_STREAM,
			ID_MENU_CAMERA_SHOW_IMAGES,
			ID_MENU_CAMERA_SHOW_TEXT,
			ID_MENU_CAMERA_PROPERTIES,
			ID_MENU_SETTINGS_FLIP_H,
			ID_MENU_SETTINGS_FLIP_V,
			ID_MENU_SETTINGS_ENABLE_LOGGING,
			ID_MENU_SETTINGS_SHOW_LOG,
			ID_MENU_SETTINGS_UART_SET_LOG_FILE,
			ID_MENU_SETTINGS_UART_MODE_OFF,
			ID_MENU_SETTINGS_UART_MODE_LOGGING,
			ID_MENU_SETTINGS_UART_MODE_TEST,
			ID_MENU_SETTINGS_DISABLE_DECODING,
			ID_MENU_HELP_ABOUT
		};
		
		ODrawingPanel* m_poDrawingPanel;
		wxStatusBar* m_statusBar1;
		wxMenuBar* m_menubar1;
		wxMenu* m_poMenuFile;
		wxMenu* m_poMenuCamera;
		wxMenuItem* m_poMenuCameraStartStopStream;
		wxMenuItem* m_poMenuCameraProperties;
		wxMenu* m_poMenuSettings;
		wxMenu* m_poMenuSettingsUART;
		wxMenu* m_poMenuHelp;
		wxToolBar* m_poToolBar;
		wxToolBarToolBase* m_poToolCameraStartStopStream; 
		wxToolBarToolBase* m_poToolCameraShowImages; 
		wxToolBarToolBase* m_poToolCameraShowText; 
		wxToolBarToolBase* m_poToolSettingsShowLog; 
		wxToolBarToolBase* m_poToolSettingsFlipH; 
		wxToolBarToolBase* m_poToolSettingsFlipV; 
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) = 0;
		virtual void OnFileSaveFrameAs( wxCommandEvent& event ) = 0;
		virtual void OnCameraStartStopStream( wxCommandEvent& event ) = 0;
		virtual void OnCameraShowImages( wxCommandEvent& event ) = 0;
		virtual void OnCameraShowText( wxCommandEvent& event ) = 0;
		virtual void OnCameraProperties( wxCommandEvent& event ) = 0;
		virtual void OnSettingsFlipH( wxCommandEvent& event ) = 0;
		virtual void OnSettingsFlipV( wxCommandEvent& event ) = 0;
		virtual void OnSettingsEnableLogging( wxCommandEvent& event ) = 0;
		virtual void OnSettingsShowLog( wxCommandEvent& event ) = 0;
		virtual void OnSettingsUARTConfigureParams( wxCommandEvent& event ) = 0;
		virtual void OnUARTMode( wxCommandEvent& event ) = 0;
		virtual void OnSettingsDisableDecoding( wxCommandEvent& event ) = 0;
		virtual void OnMenuHelpAbout( wxCommandEvent& event ) = 0;
		
	
	public:
		
		UsbScrollMainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ES Experts USB Scroll"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 549,355 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
		
		~UsbScrollMainFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ODevicePropertiesFrmBase
///////////////////////////////////////////////////////////////////////////////
class ODevicePropertiesFrmBase : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			ID_SCOLLED_DEVICE_PROPERTIES_AREA = 1001,
			ID_MAIN_NOTEBOOK,
			ID_DEVICE_PANEL,
			ID_DEVICES_LIST_CH,
			ID_GET_DEVICES_LIST_BTN,
			ID_MODEL_TXT,
			ID_SERIAL_TXT,
			ID_SAVE_SETTINGS_BTN,
			ID_MIRRORIMAGE_CHB,
			ID_FLIPIMAGE_CHB,
			ID_FDGAIN_CHB,
			ID_FORMAT_CHS,
			ID_APPLY_SCAN_AREA_BTN,
			ID_MAX_IRIS_VALUE_TC,
			ID_MAX_IRIS_VALUE_SET_BTN,
			ID_DIRECT_SHUTTER_VALUE_TC,
			ID_DIRECT_SHUTTER_VALUE_SET_BTN,
			ID_TEMPERATURE_TC,
			ID_GET_TEMPERATURE_BTN,
			ID_ENABLE_DELAY_TIMING_CH,
			ID_PORT_TC,
			ID_PACKET_SIZE_TC,
			ID_APPLY_STREAM0_BTN,
			ID_SET_LUT_TABLE_BUTTON,
			ID_GET_LUT_TABLE_BUTTON
		};
		
		wxScrolledWindow* m_poScrolledArea;
		wxNotebook* m_poMainNotebook;
		wxPanel* m_poDevicePanel;
		wxChoice* m_poDevicesListCh;
		wxButton* m_poGetDevicesListBtn;
		wxTextCtrl* m_poModelTxt;
		wxTextCtrl* m_poSerialTxt;
		wxBoxSizer* m_poBitsAndSaveSizer;
		wxButton* m_poSaveSettingsBtn;
		wxStaticBoxSizer* m_poImageOptionsGB;
		wxCheckBox* m_poMirrorImageChB;
		wxCheckBox* m_poFlipImageChB;
		wxCheckBox* m_poFDGainChB;
		wxStaticBoxSizer* m_poModeGB;
		wxChoice* m_poFormatChs;
		wxButton* m_poApplyScanAreaBtn;
		wxFlexGridSizer* m_oScanAreaSizer;
		wxStaticBoxSizer* m_poTriggerModeSizer;
		wxCheckBox* m_poTriggerOnOffChb;
		wxCheckBox* m_poInversePolarityChb;
		wxRadioButton* m_poTriggerModeHardwareShutterRB;
		wxRadioButton* m_poTriggerModeHardwareSignalRB;
		wxRadioButton* m_poTriggerModeInternalSoftwareRB;
		wxButton* m_poGetFrameBtn;
		wxPanel* m_poSignal;
		wxBoxSizer* m_poSignalSizerTop;
		wxBoxSizer* m_poSignalSizer;
		wxTextCtrl* m_poMaxIrisValueTC;
		wxButton* m_poMaxIrisValueSetBtn;
		wxTextCtrl* m_poDirectShutterValueTC;
		wxButton* m_poDirectShutterValueSetBtn;
		wxPanel* m_poColor;
		wxBoxSizer* m_poColorSizer;
		wxPanel* m_poCompress;
		wxBoxSizer* m_poCompressSizer;
		wxPanel* m_poAdvanced;
		wxBoxSizer* m_poAdvancedSizer;
		wxTextCtrl* m_poTemperaturetTC;
		wxButton* m_poGetTemperatureBtn;
		wxStaticBoxSizer* m_poOutputControlsGB;
		wxRadioButton* m_poOut0R;
		wxRadioButton* m_poOut1R;
		wxRadioButton* m_poOut2R;
		wxRadioButton* m_poOut3R;
		wxStaticBoxSizer* m_poOutputTimesSizer;
		wxCheckBox* m_poEnableDelayTimingCh;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_poPortStream0TC;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_poPacketSizeStream0TC;
		wxButton* m_poApplyStream0Btn;
		wxButton* m_poSetLUTTableBtn;
		wxButton* m_poGetLUTTableBtn;
		wxStaticText* m_poLUTGetResultTxt;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFrameClose( wxCloseEvent& event ) = 0;
		virtual void OnDevicesListChoice( wxCommandEvent& event ) = 0;
		virtual void OnGetDevicesListButtonClick( wxCommandEvent& event ) = 0;
		virtual void OnSaveSettingsBtnClick( wxCommandEvent& event ) = 0;
		virtual void OnImageMirrorFlipFDGainChB( wxCommandEvent& event ) = 0;
		virtual void OnFormatSelectionChanged( wxCommandEvent& event ) = 0;
		virtual void OnApplyScanArea( wxCommandEvent& event ) = 0;
		virtual void OnTriggerOnOffChb( wxCommandEvent& event ) = 0;
		virtual void OnTriggerPolarityChb( wxCommandEvent& event ) = 0;
		virtual void OnTriggerModeRB( wxCommandEvent& event ) = 0;
		virtual void OnGetFrameBtn( wxCommandEvent& event ) = 0;
		virtual void OnSetMaxIrisValue( wxCommandEvent& event ) = 0;
		virtual void OnSetDirectShutterValue( wxCommandEvent& event ) = 0;
		virtual void OnGetTemperature( wxCommandEvent& event ) = 0;
		virtual void OnOutputRB( wxCommandEvent& event ) = 0;
		virtual void OnEnableDelayTimingCh( wxCommandEvent& event ) = 0;
		virtual void OnApplyStream0BtnClick( wxCommandEvent& event ) = 0;
		virtual void OnSetLUTTableBtn( wxCommandEvent& event ) = 0;
		virtual void OnGetLUTTableBtn( wxCommandEvent& event ) = 0;
		
	
	public:
		
		ODevicePropertiesFrmBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Device Prperties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 478,788 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		
		~ODevicePropertiesFrmBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class OFeatureControlBase
///////////////////////////////////////////////////////////////////////////////
class OFeatureControlBase : public wxPanel 
{
	private:
	
	protected:
		enum
		{
			ID_FEATURE_SLIDER = 1001,
			ID_FEATURE_VALUE
		};
		
		wxStaticBoxSizer* m_oFeatureName;
		wxStaticText* m_staticText5;
		wxSlider* m_poFeatureSlider;
		wxTextCtrl* m_poFeatureValue;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnScrollFeatureSlider( wxScrollEvent& event ) = 0;
		
	
	public:
		
		OFeatureControlBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 391,278 ), long style = wxTAB_TRAVERSAL ); 
		~OFeatureControlBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class OFrameInfoBase
///////////////////////////////////////////////////////////////////////////////
class OFrameInfoBase : public wxFrame 
{
	private:
	
	protected:
		wxTextCtrl* m_poTextCtrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFrameClose( wxCloseEvent& event ) = 0;
		
	
	public:
		
		OFrameInfoBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Frame Info"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 235,291 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		
		~OFrameInfoBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class OUARTParamsBase
///////////////////////////////////////////////////////////////////////////////
class OUARTParamsBase : public wxDialog 
{
	private:
	
	protected:
		enum
		{
			ID_SELF_TEST_MODE_RBG = 1001
		};
		
		wxStaticText* m_staticText7;
		wxFilePickerCtrl* m_poLogFilePath;
		wxRadioBox* m_poSelfTestModeRBG;
		wxStaticText* m_staticText8;
		wxSpinCtrl* m_poPollingInterval;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
	
	public:
		
		OUARTParamsBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("UART Params"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 570,231 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP ); 
		~OUARTParamsBase();
	
};

#endif //__USBSCROLLGUIBASE_H__
