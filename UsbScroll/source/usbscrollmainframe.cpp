
#include "usbscrollmainframe.h"
#include "odrawingpanel.h"
#include "usbcamversion.h"
#include "ocameradatamanager.h"

#include "wx/msgdlg.h"
#include "wx/config.h"
#include "wx/aboutdlg.h"
#include "wx/filedlg.h"
#include "wx/display.h"

wxDEFINE_EVENT(STREAM_STOPPED_BY_REQUEST_EVENT, wxCommandEvent);

namespace
{
   constexpr char goShowDeviceProperties [] = "ShowDeviceProperties";
   constexpr char goShowImages []  = "ShowImages";
   constexpr char goShowText [] = "ShowText";
   constexpr char goShowLog [] = "ShowLog";
   constexpr char goEnableLogging [] = "EnableLogging";
   constexpr char goSettingsFlipH []  = "SettingsFlipH";
   constexpr char goSettingsFlipV []  = "SettingsFlipV";
   constexpr char goSettingDisableDecode []  = "DisableDecode";
   constexpr char goMainFramePositionX [] = "MainFramePositionX";
   constexpr char goMainFramePositionY [] = "MainFramePositionY";
   constexpr char goMainFrameSizeX [] = "MainFrameSizeX";
   constexpr char goMainFrameSizeY [] = "MainFrameSizeY";
   constexpr char goDeviceFramePositionX [] = "DeviceFramePositionX";
   constexpr char goDeviceFramePositionY [] = "DeviceFramePositionY";
   constexpr char goTextFramePositionX [] = "TextFramePositionX";
   constexpr char goTextFramePositionY [] = "TextFramePositionY";
   constexpr char goTextFrameSizeX [] = "TextFrameSizeX";
   constexpr char goTextFrameSizeY [] = "TextFrameSizeY";
   constexpr char goLogFramePositionX [] = "LogFramePositionX";
   constexpr char goLogFramePositionY [] = "LogFramePositionY";
   constexpr char goLogFrameSizeX [] = "LogFrameSizeX";
   constexpr char goLogFrameSizeY [] = "LogFrameSizeY";

   void ShowWindow(wxWindow& roWnd_p, bool bShow_p)
   {
      roWnd_p.Show(bShow_p);
      
      if (bShow_p && wxDisplay::GetFromWindow(&roWnd_p) == wxNOT_FOUND)
      {
         roWnd_p.CentreOnParent();
      }
   }
}

UsbScrollMainFrame::UsbScrollMainFrame(wxWindow* poParent_p)
   : UsbScrollMainFrameBase(poParent_p)
   , m_oDeviceProperties(this)
   , m_oFrameInfo(this)
   , m_oLog(this,"Cam Log Window")
{
   SetTitle(GetTitle() + " " + gVersionName);
   auto& oCM = CamM;

   oCM.AddNewFrameObserver(m_poDrawingPanel);
   oCM.AddNewFrameObserver(&m_oFrameInfo);
   oCM.SetStreamErrorObserver(this);

   Bind(STREAM_STOPPED_BY_REQUEST_EVENT, &UsbScrollMainFrame::OnStopStreamOnError, this, GetId());
   
   m_oDeviceProperties.SetCloseEventProcessor(std::bind(&UsbScrollMainFrame::OnCameraProperties, this, std::placeholders::_1));
   m_oFrameInfo.SetCloseEventProcessor(std::bind(&UsbScrollMainFrame::OnCameraShowText, this, std::placeholders::_1));
   m_oLog.SetCloseEventProcessor(std::bind(&UsbScrollMainFrame::OnSettingsShowLog, this, std::placeholders::_1));

   wxConfig oConfig("UsbScroll");
   wxCommandEvent oEvt;
   oEvt.SetId(ID_MENU_CAMERA_PROPERTIES);
   oEvt.SetInt(oConfig.ReadBool(goShowDeviceProperties, true));
   OnCameraProperties(oEvt);

   oEvt.SetId(ID_MENU_CAMERA_SHOW_IMAGES);
   oEvt.SetInt(oConfig.ReadBool(goShowImages, true));
   OnCameraShowImages(oEvt);

   oEvt.SetId(ID_MENU_CAMERA_SHOW_TEXT);
   oEvt.SetInt(oConfig.ReadBool(goShowText, false));
   OnCameraShowText(oEvt);

   oEvt.SetId(ID_MENU_SETTINGS_ENABLE_LOGGING);
   oEvt.SetInt(oConfig.ReadBool(goEnableLogging, true));
   OnSettingsEnableLogging(oEvt);

   oEvt.SetId(ID_MENU_SETTINGS_SHOW_LOG);
   oEvt.SetInt(oConfig.ReadBool(goShowLog, false));
   OnSettingsShowLog(oEvt);

   oEvt.SetId(ID_MENU_SETTINGS_FLIP_H);
   oEvt.SetInt(oConfig.ReadBool(goSettingsFlipH, oCM.GetFlipImageH()));
   OnSettingsFlipH(oEvt);

   oEvt.SetId(ID_MENU_SETTINGS_FLIP_V);
   oEvt.SetInt(oConfig.ReadBool(goSettingsFlipV, oCM.GetFlipImageV()));
   OnSettingsFlipV(oEvt);

   oEvt.SetId(ID_MENU_SETTINGS_DISABLE_DECODING);
   oEvt.SetInt(oConfig.ReadBool(goSettingDisableDecode, !oCM.GetDecode()));
   OnSettingsDisableDecoding(oEvt);

   SetPosition({
      static_cast<int>(oConfig.ReadLong(goMainFramePositionX, -1)),
      static_cast<int>(oConfig.ReadLong(goMainFramePositionY, -1))});

   wxSize oDS = GetSize();
   SetSize({
      static_cast<int>(oConfig.ReadLong(goMainFrameSizeX, oDS.x)),
      static_cast<int>(oConfig.ReadLong(goMainFrameSizeY, oDS.y))});

   m_oDeviceProperties.Move({
      static_cast<int>(oConfig.ReadLong(goDeviceFramePositionX, -1)),
      static_cast<int>(oConfig.ReadLong(goDeviceFramePositionY, -1))});

   m_oFrameInfo.Move({
      static_cast<int>(oConfig.ReadLong(goTextFramePositionX, -1)),
      static_cast<int>(oConfig.ReadLong(goTextFramePositionY, -1))});

   oDS = m_oFrameInfo.GetSize();
   m_oFrameInfo.SetSize({
      static_cast<int>(oConfig.ReadLong(goTextFrameSizeX, oDS.x)),
      static_cast<int>(oConfig.ReadLong(goTextFrameSizeY, oDS.y))});

   if (m_oLog.GetFrame() != nullptr)
   {
      m_oLog.GetFrame()->SetPosition({
         static_cast<int>(oConfig.ReadLong(goLogFramePositionX, -1)),
         static_cast<int>(oConfig.ReadLong(goLogFramePositionY, -1))});

      oDS = m_oLog.GetFrame()->GetSize();
      m_oLog.GetFrame()->SetSize({
         static_cast<int>(oConfig.ReadLong(goLogFrameSizeX, oDS.x)),
         static_cast<int>(oConfig.ReadLong(goLogFrameSizeY, oDS.y))});
   }

   m_oLog.AddMessage("Application run");

   m_oUARTModesManager.SetLogAbortedReporting([this](){UARTLogAborted();});
}

UsbScrollMainFrame::~UsbScrollMainFrame()
{
}

void UsbScrollMainFrame::OnFileSaveFrameAs(wxCommandEvent& roEvent_p)
{
   std::vector<std::tuple<wxString, wxString, wxBitmapType>> oTypes = {
      {wxT("PNG files (*.png)|*.png"), wxT("png"), wxBITMAP_TYPE_PNG},
      {wxT("BMP files (*.bmp)|*.bmp"), wxT("bmp"), wxBITMAP_TYPE_BMP},
      {wxT("GIF files (*.gif)|*.gif"), wxT("gif"), wxBITMAP_TYPE_GIF},
      {wxT("JPEG files (*.jpg)|*.jpg"), wxT("jpg"), wxBITMAP_TYPE_JPEG}
   };
   wxString oWildcard = std::get<0>(oTypes.front());
   std::for_each(std::next(oTypes.begin()), oTypes.end(), [&oWildcard](const auto& roItem_p)
      { oWildcard += wxT("|") + std::get<0>(roItem_p); });

   wxFileDialog oFD(this, wxT("Save Frame As"), wxT(""), wxT("frame"),
      oWildcard, wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
   
   if (oFD.ShowModal() == wxID_OK)
   {
      auto& roCamM = CamM;
      auto oLocker = roCamM.SharedLockFrameData();
      const auto& roFrameData = roCamM.GetFrameData();

      if (roFrameData.m_eState == INewFrameObserver::ETransferState::eNormal && roFrameData.GetFrameBuffer().IsOk())
      {
         wxImage oImg(roFrameData.GetFrameBuffer().ConvertToImage());
         oLocker.reset(nullptr);

         oImg.SaveFile(oFD.GetPath(), std::get<2>(oTypes[oFD.GetFilterIndex()]));
      }
   }      
}

void UsbScrollMainFrame::OnCameraStartStopStream( wxCommandEvent& roEvent_p )
{
   OCameraDataManager& oCM = OCameraDataManager::GetInstance();
   if (oCM.IsStreamWorked())
   {
      oCM.StopVideoStream();
   }
   else
   {
      oCM.StartVideoStream();
   }
   SetCheckForToolAndMenu(ID_MENU_CAMERA_STARTSTOP_STREAM, oCM.IsStreamWorked());
}

void UsbScrollMainFrame::OnSettingsFlipH( wxCommandEvent& roEvent_p )
{
   CamM.SetFlipImageH(roEvent_p.IsChecked());
   SetCheckForToolAndMenu(ID_MENU_SETTINGS_FLIP_H, roEvent_p.IsChecked());
}

void UsbScrollMainFrame::OnSettingsFlipV( wxCommandEvent& roEvent_p )
{
   CamM.SetFlipImageV(roEvent_p.IsChecked());
   SetCheckForToolAndMenu(ID_MENU_SETTINGS_FLIP_V, roEvent_p.IsChecked());
}

void UsbScrollMainFrame::OnCameraProperties( wxCommandEvent& roEvent_p )
{
   SetCheckForToolAndMenu(ID_MENU_CAMERA_PROPERTIES, roEvent_p.IsChecked());
   ShowWindow(m_oDeviceProperties, roEvent_p.IsChecked());
}

void UsbScrollMainFrame::OnSettingsDisableDecoding( wxCommandEvent& roEvent_p )
{
   bool bEnable = !roEvent_p.IsChecked();
   CamM.SetDecode(bEnable);
   GetMenuBar()->Check(ID_MENU_SETTINGS_DISABLE_DECODING, !bEnable);
   SetEnableForToolAndMenu(ID_MENU_SETTINGS_FLIP_H, bEnable);
   SetEnableForToolAndMenu(ID_MENU_SETTINGS_FLIP_V, bEnable);
}

void UsbScrollMainFrame::OnSettingsEnableLogging(wxCommandEvent& roEvent_p)
{
   if (roEvent_p.IsChecked())
   {   
      CamM.SetLogReceiver(m_oLog);
   }
   else
   {
      CamM.SetLogReceiver(ONoLog::Get());
   }
   GetMenuBar()->Check(ID_MENU_SETTINGS_ENABLE_LOGGING, roEvent_p.IsChecked());
}

void UsbScrollMainFrame::OnSettingsShowLog(wxCommandEvent& roEvent_p)
{
   m_oLog.Show(roEvent_p.IsChecked());
   SetCheckForToolAndMenu(ID_MENU_SETTINGS_SHOW_LOG, roEvent_p.IsChecked());
}

void UsbScrollMainFrame::UARTLogAborted()
{
   m_poMenuSettingsUART->Check(ID_MENU_SETTINGS_UART_MODE_OFF, true);
   m_poMenuSettingsUART->Check(ID_MENU_SETTINGS_UART_MODE_LOGGING, false);
   m_poMenuSettingsUART->Check(ID_MENU_SETTINGS_UART_MODE_TEST, false);
}

void UsbScrollMainFrame::OnSettingsUARTConfigureParams(wxCommandEvent& roEvent_p)
{
   m_oUARTModesManager.SelectParams(this);
}

void UsbScrollMainFrame::OnUARTMode(wxCommandEvent& roEvent_p)
{
   auto eMode = OUARTModesManager::EMode::eNone;
   switch (roEvent_p.GetId())
   {
   case ID_MENU_SETTINGS_UART_MODE_LOGGING:
      eMode = OUARTModesManager::EMode::eLogging;
      break;
   case ID_MENU_SETTINGS_UART_MODE_TEST:
      eMode = OUARTModesManager::EMode::eSelfTest;
      break;
   }
   m_poMenuSettingsUART->Check(ID_MENU_SETTINGS_UART_MODE_LOGGING, roEvent_p.GetId() == ID_MENU_SETTINGS_UART_MODE_LOGGING);
   m_poMenuSettingsUART->Check(ID_MENU_SETTINGS_UART_MODE_TEST, roEvent_p.GetId() == ID_MENU_SETTINGS_UART_MODE_TEST);
   m_poMenuSettingsUART->Check(ID_MENU_SETTINGS_UART_MODE_OFF, roEvent_p.GetId() == ID_MENU_SETTINGS_UART_MODE_OFF);
   //m_poMenuSettingsUART->UpdateUI();
   m_oUARTModesManager.SetMode(eMode);

   roEvent_p.Skip();
}

void UsbScrollMainFrame::OnMenuHelpAbout(wxCommandEvent& roEvent_p)
{
   wxAboutDialogInfo aboutInfo;

   aboutInfo.SetName("Usb Scroll");
   aboutInfo.SetVersion(gVersionName);
   aboutInfo.SetDescription(_("Usb camera interaction"));
   aboutInfo.SetCopyright("(C) 2022-2024");
   aboutInfo.SetWebSite("http://www.es-exp.ru/");

   wxAboutBox(aboutInfo, this);
}

void UsbScrollMainFrame::StopStreamOnRequest(ERquestType eType_p, std::string&& Message)
{
    wxCommandEvent* pEvent = new wxCommandEvent(STREAM_STOPPED_BY_REQUEST_EVENT, GetId());
    pEvent->SetEventObject(this);
    if (eType_p == ERquestType::eError)
    {
      if (Message.empty())
      {
         pEvent->SetString("Video Stream error. Stream has been stopped.");
      }
      else
      {
         pEvent->SetString(Message);       
      }
    }
    wxQueueEvent(this, pEvent);
}

void UsbScrollMainFrame::OnStopStreamOnError(wxCommandEvent& roEvent_p)
{
   if (!roEvent_p.GetString().empty())
   {
      wxMessageBox(roEvent_p.GetString());
   }
   SetCheckForToolAndMenu(ID_MENU_CAMERA_STARTSTOP_STREAM, false);
}

void UsbScrollMainFrame::OnCameraShowImages( wxCommandEvent& roEvent_p )
{
   SetCheckForToolAndMenu(ID_MENU_CAMERA_SHOW_IMAGES, roEvent_p.IsChecked());
   m_poDrawingPanel->SetDrawFrames(roEvent_p.IsChecked());
   CamM.SetFramesAreRequired(roEvent_p.IsChecked());
}

void UsbScrollMainFrame::OnCameraShowText( wxCommandEvent& roEvent_p )
{
   SetCheckForToolAndMenu(ID_MENU_CAMERA_SHOW_TEXT, roEvent_p.IsChecked());
   ShowWindow(m_oFrameInfo, roEvent_p.IsChecked());
}

void UsbScrollMainFrame::SetCheckForToolAndMenu(int iID_p, bool bChecked_p)
{
   GetMenuBar()->Check(iID_p, bChecked_p);
   GetToolBar()->ToggleTool(iID_p, bChecked_p);
}

void UsbScrollMainFrame::SetEnableForToolAndMenu(int iID_p, bool bChecked_p)
{
   GetMenuBar()->Enable(iID_p, bChecked_p);
   GetToolBar()->EnableTool(iID_p, bChecked_p);
}

void UsbScrollMainFrame::OnClose(wxCloseEvent& roEvent_p)
{
   CamM.StopVideoStream();

   wxConfig oConfig("UsbScroll");
   oConfig.Write(goShowDeviceProperties, GetMenuBar()->IsChecked(ID_MENU_CAMERA_PROPERTIES));
   oConfig.Write(goShowImages, GetMenuBar()->IsChecked(ID_MENU_CAMERA_SHOW_IMAGES));
   oConfig.Write(goShowText, GetMenuBar()->IsChecked(ID_MENU_CAMERA_SHOW_TEXT));
   oConfig.Write(goEnableLogging, GetMenuBar()->IsChecked(ID_MENU_SETTINGS_ENABLE_LOGGING));
   oConfig.Write(goShowLog, GetMenuBar()->IsChecked(ID_MENU_SETTINGS_SHOW_LOG));

   oConfig.Write(goSettingsFlipH, GetMenuBar()->IsChecked(ID_MENU_SETTINGS_FLIP_H));
   oConfig.Write(goSettingsFlipV, GetMenuBar()->IsChecked(ID_MENU_SETTINGS_FLIP_V));
   oConfig.Write(goSettingDisableDecode, GetMenuBar()->IsChecked(ID_MENU_SETTINGS_DISABLE_DECODING));

   auto oDP = GetPosition();
   oConfig.Write(goMainFramePositionX, oDP.x);
   oConfig.Write(goMainFramePositionY, oDP.y);

   auto oDS = GetSize();
   oConfig.Write(goMainFrameSizeX, oDS.x);
   oConfig.Write(goMainFrameSizeY, oDS.y);

   oDP = m_oDeviceProperties.GetScreenPosition();
   oConfig.Write(goDeviceFramePositionX, oDP.x);
   oConfig.Write(goDeviceFramePositionY, oDP.y);

   oDP = m_oFrameInfo.GetScreenPosition();
   oConfig.Write(goTextFramePositionX, oDP.x);
   oConfig.Write(goTextFramePositionY, oDP.y);

   oDS = m_oFrameInfo.GetSize();
   oConfig.Write(goTextFrameSizeX, oDS.x);
   oConfig.Write(goTextFrameSizeY, oDS.y);

   if (m_oLog.GetFrame() != nullptr)
   {
      oDP = m_oLog.GetFrame()->GetPosition();
      oConfig.Write(goLogFramePositionX, oDP.x);
      oConfig.Write(goLogFramePositionY, oDP.y);

      oDS = m_oLog.GetFrame()->GetSize();
      oConfig.Write(goLogFrameSizeX, oDS.x);
      oConfig.Write(goLogFrameSizeY, oDS.y);
   }

   roEvent_p.Skip();
}