// @file oframeinfo.h

#pragma once

#include "usbscrollguibase.h"
#include "ocameradatamanager.h"
#include <functional>
#include <limits>
#include <typeinfo>

wxDECLARE_EVENT(NEW_FRAME_RECEIVED_EVENT, wxCommandEvent);

#define USE_SIMPLE_TEXT_CTRL

class OFrameInfo : public OFrameInfoBase, public INewFrameObserver
{
public:
   OFrameInfo(wxWindow* poParent);

   void OnFrameClose(wxCloseEvent& roEvent_p) override;

   void SetCloseEventProcessor(const std::function<void(wxCommandEvent&)>& poCloseProcessor);

   void ProcessNewFrame() override;
   void PrepareForStreamStart(eseusbcam_long_t FrameSizeX, eseusbcam_long_t FrameSizeY) override;
private:
   template<class T>
   void DisplayData(const T& roServData, ETransferState eState);
   void DisplayData_Impl(const STREAM_SERV_DATA_TYPE_1& roServData, ETransferState eState);
   void DisplayData_Impl(const STREAM_SERV_DATA_TYPE_2& roServData, ETransferState eState);
   void DisplayData_Impl(const STREAM_SERV_DATA_ITK4_0& roServData, ETransferState eState);
   void DisplayData_Impl(const STREAM_SERV_DATA_BRZ_5012HM_GE_V1& roServData, ETransferState eState);
   void DisplayNoData(ETransferState eState);
   
   void OnProcessNewFrame(wxCommandEvent& roEvent_p);
   void ProcessNewFrameImpl();
private:
   std::function<void(wxCommandEvent&)> m_poCloseProcessor;
   OFPSCalculator m_oNewFrameFPSCalculator;
   std::size_t m_lLastHash = 0;

   bool m_bUpdateProcessed = true;
   std::mutex m_oUpdateProcessedMutex;
};

template<class T>
void OFrameInfo::DisplayData(const T& roServData, ETransferState eState)
{
   if(typeid(roServData).hash_code() != m_lLastHash)
   {
      //m_poListCtrl->DeleteAllItems();
      m_lLastHash = typeid(roServData).hash_code();
   }
   DisplayData_Impl(roServData, eState);
}
