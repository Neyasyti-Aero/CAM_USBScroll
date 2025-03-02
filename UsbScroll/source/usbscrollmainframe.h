#ifndef USBSCROLLMAINFRAME_H
#define USBSCROLLMAINFRAME_H

#include "usbscrollguibase.h"
#include "odevicepropertiesfrm.h"
#include "oframeinfo.h"
#include "ocameradatamanager.h"
#include "ouartmodesmanager.h"

#include "ocamlogwindow.h"

wxDECLARE_EVENT(STREAM_STOPPED_BY_REQUEST_EVENT, wxCommandEvent);

class UsbScrollMainFrame : public UsbScrollMainFrameBase, public IStreamEventsObserver
{
public:
   UsbScrollMainFrame(wxWindow* poParent_p);
   ~UsbScrollMainFrame();

   void OnFileSaveFrameAs(wxCommandEvent& roEvent_p) override;
   void OnCameraStartStopStream(wxCommandEvent& roEvent_p) override;
   void OnCameraProperties(wxCommandEvent& roEvent_p) override;
   void OnCameraShowImages(wxCommandEvent& roEvent_p) override;
   void OnCameraShowText(wxCommandEvent& roEvent_p) override;
   void OnSettingsFlipH(wxCommandEvent& roEvent_p) override;
   void OnSettingsFlipV(wxCommandEvent& roEvent_p) override;
   void OnSettingsDisableDecoding(wxCommandEvent& roEvent_p) override;
   void OnSettingsEnableLogging(wxCommandEvent& roEvent_p) override;
   void OnSettingsShowLog(wxCommandEvent& roEvent_p) override;
	void OnSettingsUARTConfigureParams(wxCommandEvent& roEvent_p) override;
   void OnUARTMode(wxCommandEvent& roEvent_p) override;
   void OnMenuHelpAbout(wxCommandEvent& roEvent_p) override;

   void StopStreamOnRequest(ERquestType eType_p, std::string&& Message) override;
private:
   void OnClose(wxCloseEvent& roEvent_p) override;
   void OnStopStreamOnError(wxCommandEvent& roEvent_p);
   void SetCheckForToolAndMenu(int iID_p, bool bChecked_p);
   void SetEnableForToolAndMenu(int iID_p, bool bChecked_p);
   void UARTLogAborted();
private:
   ODevicePropertiesFrm m_oDeviceProperties;
   OFrameInfo m_oFrameInfo;
   OCamLogWindow m_oLog;
   OUARTModesManager m_oUARTModesManager;
};

#endif // USBSCROLLMAINFRAME_H