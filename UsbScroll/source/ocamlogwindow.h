
#pragma once

#include "ilogmessagesreceiver.h"
#include "wx/log.h"
#include "wx/event.h"
#include <functional>

class OCamLogWindow : public wxLogWindow , public ILogMessagesReceiver
{
public:
   OCamLogWindow(wxWindow* pParent,       // the parent frame (can be NULL)
      const wxString& szTitle);        // pass messages to the old target?

   bool OnFrameClose(wxFrame* frame) override;
   void SetCloseEventProcessor(const std::function<void(wxCommandEvent&)>& poCloseProcessor);

   void AddEvent(EEvents eEvent, const event_data_t& roData = std::monostate()) override;
   void AddMessage(const std::string& roMessage) override;
private:
   std::function<void(wxCommandEvent&)> m_poCloseProcessor;
};