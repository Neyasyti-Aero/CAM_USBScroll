
#include "ocamlogwindow.h"

OCamLogWindow::OCamLogWindow(wxWindow *pParent,        // the parent frame (can be NULL)
                             const wxString& szTitle)  // pass messages to the old target?
 : wxLogWindow(pParent,szTitle,false,false)
{
   SetTimestamp("%d.%m.%y %T ");
}

bool OCamLogWindow::OnFrameClose(wxFrame *frame)
{
   if (m_poCloseProcessor)
   {
      wxCommandEvent oEvent;
      oEvent.SetInt(0);
      m_poCloseProcessor(oEvent);
   }
   else
   {
      Show(false);
   }
   return false;
}

void OCamLogWindow::SetCloseEventProcessor(const std::function<void(wxCommandEvent&)>& poCloseProcessor)
{
   m_poCloseProcessor = poCloseProcessor;
}

void OCamLogWindow::AddEvent(EEvents eEvent, const event_data_t& roData)
{
   std::string oMessage;
   try
   {
      switch (eEvent)
      {
      case EEvents::eStartStream:
         oMessage = "Start stream";
         break;
      case EEvents::eStopStream:
         oMessage = "Stop stream";
         break;
      case EEvents::eMissedFrame:
         oMessage = "Missed " + std::to_string(std::get<int>(roData)) + " frames";
         break;
      case EEvents::eFrameHeaderIssue:
         oMessage = "Frame header issue: next data is zero";
         break;
      case EEvents::eCorruptFrameInfo:
         oMessage = "Frame size, bit depth or data are corrupt";
         break;
      case EEvents::eStreamSynchroIssue:
         oMessage = "Frame marker ID is corrupt or start and end markers mismatch";
         break;
      case EEvents::eUARTOverflow:
         oMessage = "Camera UART buffer overflow";
         break;
      case EEvents::eUARTNoData:
         oMessage = "Camera UART buffer has no data";
         break;
      default:
         oMessage = "Unexpected log reason";
      }
   }
   catch (const std::exception& e)
   {
      oMessage = e.what();
   }
   wxLogMessage(oMessage.c_str());
}

void OCamLogWindow::AddMessage(const std::string& roMessage)
{
   wxLogMessage(roMessage.c_str());
}