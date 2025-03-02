
#include "oframeinfo.h"
#include <ctime>

wxDEFINE_EVENT(NEW_FRAME_RECEIVED_EVENT, wxCommandEvent);

namespace
{
   wxString GenerateState(INewFrameObserver::ETransferState eState)
   {
      wxString oStr("\tState = ");
      switch(eState)
      {
      case INewFrameObserver::ETransferState::eHeaderIssue:
         oStr += "Header NOk";
         break;
      case INewFrameObserver::ETransferState::eFrameInfoIssue:
         oStr += "Frame Info NOk";
         break;
      case INewFrameObserver::ETransferState::eStreamSynchronizationIssue:
         oStr += "Frame Synchronization NOk";
         break;
      case INewFrameObserver::ETransferState::eNormal:
         default:
         oStr += "Ok";
         break;
      }
      oStr += '\n';
      return oStr;
   }
}

OFrameInfo::OFrameInfo(wxWindow* poParent)
    : OFrameInfoBase(poParent)
{
#ifndef USE_SIMPLE_TEXT_CTRL
   m_poTextCtrl->Hide();
   m_poListCtrl->AppendColumn("Name");
   m_poListCtrl->AppendColumn("Value");
#else // USE_SIMPLE_TEXT_CTRL
   //m_poListCtrl->Hide();
#endif
   
   Bind(NEW_FRAME_RECEIVED_EVENT, &OFrameInfo::OnProcessNewFrame, this, GetId()); 
}

void OFrameInfo::OnFrameClose(wxCloseEvent &roEvent_p)
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
   roEvent_p.Skip(false);
}

void OFrameInfo::SetCloseEventProcessor(const std::function<void(wxCommandEvent&)>& poCloseProcessor)
{
   m_poCloseProcessor = poCloseProcessor;
}

void OFrameInfo::DisplayNoData(ETransferState eState)
{
   auto& roFrameData = CamM.GetFrameData();
#ifndef USE_SIMPLE_TEXT_CTRL
   if (!m_poListCtrl->GetFirstItem().IsOk())
   {
      auto oGroupItem = m_poListCtrl->AppendItem(m_poListCtrl->GetRootItem(),"Frame");
      m_poListCtrl->AppendItem(oGroupItem,"Received");
      m_poListCtrl->AppendItem(oGroupItem,"Draw FPS");
      m_poListCtrl->Expand(oGroupItem);
   }

   auto oItem = m_poListCtrl->GetRootItem();
   auto SetNextULItem = [&oItem, this](eseusbcam_unsigned_long_t lVal)
   {
      oItem = m_poListCtrl->GetNextItem(oItem);
      m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%u", lVal));
   };

   oItem = m_poListCtrl->GetNextItem(oItem); // Frame
   SetNextULItem(roFrameData.m_ReceivedFramesCount); // Received
   oItem = m_poListCtrl->GetNextItem(oItem); // FPS
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%.2f", CamM.GetFPS()));
#else // USE_SIMPLE_TEXT_CTRL
   wxString oStr("Frame\n");
   oStr += wxString::Format("\tReceived = %u\n", roFrameData.m_ReceivedFramesCount);
   oStr += wxString::Format("\tFPS = %.2f\n", CamM.GetFPS());
   oStr += GenerateState(eState);

   m_poTextCtrl->SetValue(oStr);
#endif // USE_SIMPLE_TEXT_CTRL

}

void OFrameInfo::DisplayData_Impl(const STREAM_SERV_DATA_TYPE_1& roData_p, ETransferState eState)
{
//   if (!m_poListCtrl->GetFirstItem().IsOk())
   {
      
   }
}

void OFrameInfo::DisplayData_Impl(const STREAM_SERV_DATA_TYPE_2& roServData, ETransferState eState)
{
   auto& roFrameData = CamM.GetFrameData();
#ifndef USE_SIMPLE_TEXT_CTRL
   if (!m_poListCtrl->GetFirstItem().IsOk())
   {
      auto oGroupItem = m_poListCtrl->AppendItem(m_poListCtrl->GetRootItem(),"Frame");
      m_poListCtrl->AppendItem(oGroupItem,"Number");
      m_poListCtrl->AppendItem(oGroupItem,"Received");
      m_poListCtrl->AppendItem(oGroupItem,"Missed");
      m_poListCtrl->AppendItem(oGroupItem,"Draw FPS");
      m_poListCtrl->Expand(oGroupItem);

      oGroupItem = m_poListCtrl->AppendItem(m_poListCtrl->GetRootItem(),"Device");
      m_poListCtrl->AppendItem(oGroupItem,"Iris");
      m_poListCtrl->AppendItem(oGroupItem,"Exposure");
      m_poListCtrl->AppendItem(oGroupItem,"Gain");
      m_poListCtrl->AppendItem(oGroupItem,"Width");
      m_poListCtrl->AppendItem(oGroupItem,"Height");
      m_poListCtrl->AppendItem(oGroupItem,"Temperature");
      m_poListCtrl->Expand(oGroupItem);
   }

   auto oItem = m_poListCtrl->GetRootItem();
   auto SetNextULItem = [&oItem, this](eseusbcam_unsigned_long_t lVal)
   {
      oItem = m_poListCtrl->GetNextItem(oItem);
      m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%u", lVal));
   };

   oItem = m_poListCtrl->GetNextItem(oItem); // Frame
   SetNextULItem(roServData.ulFrameNum); // Number
   SetNextULItem(roFrameData.m_ReceivedFramesCount); // Received
   SetNextULItem(roFrameData.m_ErrorFramesCount); // Missed
   oItem = m_poListCtrl->GetNextItem(oItem); // FPS
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%.2f", CamM.GetFPS()));

   oItem = m_poListCtrl->GetNextItem(oItem); // Device
   SetNextULItem(roServData.ulCurrIris); // Iris
   SetNextULItem(roServData.ulCurrShutter); // Exposure
   SetNextULItem(roServData.ulCurrGain); // Gain
   SetNextULItem(roServData.ulWidth); // Width
   SetNextULItem(roServData.ulHeight); // Height
   oItem = m_poListCtrl->GetNextItem(oItem); // Temperature
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%d", static_cast<char>(roServData.ulCurrTemperature)));
#else // USE_SIMPLE_TEXT_CTRL
   wxString oStr("Frame\n");
   oStr += wxString::Format("\tNumber = %u\n", roServData.ulFrameNum);
   oStr += wxString::Format("\tReceived = %u\n", roFrameData.m_ReceivedFramesCount);
   oStr += wxString::Format("\tMissed = %u\n", roFrameData.m_ErrorFramesCount);
   oStr += wxString::Format("\tFPS = %.2f\n", CamM.GetFPS());
   oStr += GenerateState(eState);
 
   oStr += wxString::Format("Device\n");
   oStr += wxString::Format("\tIris = %u\n", roServData.ulCurrIris);
   oStr += wxString::Format("\tExposure = %u\n", roServData.ulCurrShutter);
   oStr += wxString::Format("\tGain = %u\n", roServData.ulCurrGain);
   oStr += wxString::Format("\tWidth = %u\n", roServData.ulWidth);
   oStr += wxString::Format("\tHeight = %u\n", roServData.ulHeight);
   oStr += wxString::Format("\tTemperature = %d\n", static_cast<char>(roServData.ulCurrTemperature));

   m_poTextCtrl->SetValue(oStr);
#endif // USE_SIMPLE_TEXT_CTRL
}

void OFrameInfo::DisplayData_Impl(const STREAM_SERV_DATA_ITK4_0& roServData, ETransferState eState)
{
   constexpr std::size_t ciRadarDataMaxIndex = 7;
   constexpr eseusbcam_unsigned_long_t NTP_TIMESTAMP_DIFF = 2208988800;
   auto& roFrameData = CamM.GetFrameData();
#ifndef USE_SIMPLE_TEXT_CTRL
   if (!m_poListCtrl->GetFirstItem().IsOk())
   {
      auto oGroupItem = m_poListCtrl->AppendItem(m_poListCtrl->GetRootItem(), "Frame");
      m_poListCtrl->AppendItem(oGroupItem, "Number");
      m_poListCtrl->AppendItem(oGroupItem, "Received");
      m_poListCtrl->AppendItem(oGroupItem, "Missed");
      m_poListCtrl->AppendItem(oGroupItem, "Draw FPS");
      m_poListCtrl->Expand(oGroupItem);

      oGroupItem = m_poListCtrl->AppendItem(m_poListCtrl->GetRootItem(), "Device");
      m_poListCtrl->AppendItem(oGroupItem, "Iris");
      m_poListCtrl->AppendItem(oGroupItem, "Exposure");
      m_poListCtrl->AppendItem(oGroupItem, "Gain");
      m_poListCtrl->AppendItem(oGroupItem, "Width");
      m_poListCtrl->AppendItem(oGroupItem, "Height");
      m_poListCtrl->AppendItem(oGroupItem, "Temperature");
      m_poListCtrl->Expand(oGroupItem);

      oGroupItem = m_poListCtrl->AppendItem(m_poListCtrl->GetRootItem(), "GNSS");
      m_poListCtrl->AppendItem(oGroupItem, "Type");
      m_poListCtrl->AppendItem(oGroupItem, "Frame date");
      m_poListCtrl->AppendItem(oGroupItem, "Frame time");
      m_poListCtrl->AppendItem(oGroupItem, "Ref clock period (ps)");
      m_poListCtrl->AppendItem(oGroupItem, "Clock start exposure");
      m_poListCtrl->AppendItem(oGroupItem, "Clock end exposure");
      m_poListCtrl->AppendItem(oGroupItem, "Flags");
      m_poListCtrl->Expand(oGroupItem);

      oGroupItem = m_poListCtrl->AppendItem(m_poListCtrl->GetRootItem(), "SNTP");
      m_poListCtrl->AppendItem(oGroupItem, "Start exposure date");
      m_poListCtrl->AppendItem(oGroupItem, "Start exposure time");
      m_poListCtrl->AppendItem(oGroupItem, "End exposure date");
      m_poListCtrl->AppendItem(oGroupItem, "End exposure time");
      m_poListCtrl->Expand(oGroupItem);

      oGroupItem = m_poListCtrl->AppendItem(m_poListCtrl->GetRootItem(), "Radar");
      m_poListCtrl->AppendItem(oGroupItem, "Messages");
      m_poListCtrl->AppendItem(oGroupItem, "Data Size");
      auto oSubGroupItem = m_poListCtrl->AppendItem(oGroupItem, "Data");
      for (eseusbcam_unsigned_long_t i=0; i<=ciRadarDataMaxIndex; ++i)
      {
         m_poListCtrl->AppendItem(oSubGroupItem, wxString::Format("%u",i));
      }
      m_poListCtrl->Expand(oSubGroupItem);
      oSubGroupItem = m_poListCtrl->AppendItem(oGroupItem, "Buffer Packets");
      m_poListCtrl->AppendItem(oSubGroupItem, "Valid");
      m_poListCtrl->AppendItem(oSubGroupItem, "Damaged");
      m_poListCtrl->AppendItem(oSubGroupItem, "Lost");
      m_poListCtrl->AppendItem(oSubGroupItem, "Shuffled");
      m_poListCtrl->AppendItem(oSubGroupItem, "LastID");
      m_poListCtrl->Expand(oSubGroupItem);
      m_poListCtrl->AppendItem(oGroupItem, "Buffer Verification Error");
      m_poListCtrl->Expand(oGroupItem);
   }

   auto oItem = m_poListCtrl->GetRootItem();
   auto SetNextULItem = [&oItem, this](eseusbcam_unsigned_long_t lVal)
   {
      oItem = m_poListCtrl->GetNextItem(oItem);
      m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%u", lVal));
   };

   oItem = m_poListCtrl->GetNextItem(oItem); // Frame
   SetNextULItem(roServData.ulFrameNum); // Number
   SetNextULItem(roFrameData.m_ReceivedFramesCount); // Received
   SetNextULItem(roFrameData.m_ErrorFramesCount); // Missed
   oItem = m_poListCtrl->GetNextItem(oItem); // FPS
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%.2f", CamM.GetFPS()));

   oItem = m_poListCtrl->GetNextItem(oItem); // Device
   SetNextULItem(roServData.usCurrIris); // Iris
   SetNextULItem(roServData.ulCurrShutter); // Exposure
   SetNextULItem(roServData.usCurrGain); // Gain
   SetNextULItem(roServData.usWidth); // Width
   SetNextULItem(roServData.usHeight); // Height
   oItem = m_poListCtrl->GetNextItem(oItem); // Temperature
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%d", static_cast<char>(roServData.usCurrTemperature)));

   oItem = m_poListCtrl->GetNextItem(oItem); // GNSS
   oItem = m_poListCtrl->GetNextItem(oItem); // Type
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%c", roServData.gnssExposure.chGnssType));
   oItem = m_poListCtrl->GetNextItem(oItem); // Frame date
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%u.%u.%u", roServData.gnssExposure.chDay, roServData.gnssExposure.chMonth, roServData.gnssExposure.chYear));
   oItem = m_poListCtrl->GetNextItem(oItem); // Frame time
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%u.%u.%u", roServData.gnssExposure.chHour, roServData.gnssExposure.chMinute, roServData.gnssExposure.chSecond));
   SetNextULItem(roServData.gnssExposure.ulRefClockPeriod); // ref clock period (ps)
   SetNextULItem(roServData.gnssExposure.ulPartSecondExposureStart); // clock start exposure
   SetNextULItem(roServData.gnssExposure.ulPartSecondExposureEnd); // clock end exposure
   oItem = m_poListCtrl->GetNextItem(oItem); // flags
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("0x%x", roServData.gnssExposure.chFlags));

   double ntp_es = static_cast<double>(roServData.ulNTP_PartSecondExposureStart);
   ntp_es /= 4294967296;
   double ntp_ee = static_cast<double>(roServData.ulNTP_PartSecondExposureEnd);
   ntp_ee /= 4294967296;
   std::time_t rawtime = roServData.ulNTP_SecondExposureStart - NTP_TIMESTAMP_DIFF;
   auto ptm = std::localtime(&rawtime);
   oItem = m_poListCtrl->GetNextItem(oItem); // SNTP
   oItem = m_poListCtrl->GetNextItem(oItem); // start exposure date
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%u.%u.%u", ptm->tm_mday, (ptm->tm_mon + 1), (ptm->tm_year + 1900)));
   oItem = m_poListCtrl->GetNextItem(oItem); // start exposure time
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%u.%u.%u, + %10fs", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, ntp_es));

   rawtime = roServData.ulNTP_SecondExposureEnd - NTP_TIMESTAMP_DIFF;
   ptm = std::localtime(&rawtime);
   oItem = m_poListCtrl->GetNextItem(oItem); // end exposure date
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%u.%u.%u", ptm->tm_mday, (ptm->tm_mon + 1), (ptm->tm_year + 1900)));
   oItem = m_poListCtrl->GetNextItem(oItem); // end exposure time
   m_poListCtrl->SetItemText(oItem, 1, wxString::Format("%u.%u.%u, + %10fs", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, ntp_ee));

   oItem = m_poListCtrl->GetNextItem(oItem); // Radar
   SetNextULItem(roServData.chRadarMsgCount); // Messages
   SetNextULItem(roServData.usRadarDataBytes); // Data Size
   oItem = m_poListCtrl->GetNextItem(oItem); // Data
   for(std::size_t i=0; i<=ciRadarDataMaxIndex; ++i)
   {
      oItem = m_poListCtrl->GetNextItem(oItem); // i
      m_poListCtrl->SetItemText(oItem, 1, wxString::Format("0x%x", roServData.RadarData[i]));
   }
   oItem = m_poListCtrl->GetNextItem(oItem); // Buffer Packets
   if (roFrameData.m_RadarErrorRet == 0)
   {
      SetNextULItem(roFrameData.m_RetValidNum); // Valid
      SetNextULItem(roFrameData.m_RetDamagedNum); // Damaged
      SetNextULItem(roFrameData.m_RetcntLostPacks); // Lost
      SetNextULItem(roFrameData.m_RetcntShuffledPacks); // Shuffled
      SetNextULItem(roFrameData.m_RetlastPacketId); // LastID
      oItem = m_poListCtrl->GetNextItem(oItem); // Buffer Verification Error
      m_poListCtrl->SetItemText(oItem, 1, "no errors");
   }
   else
   {
      for (std::size_t i=0; i<5; ++i)
      {
         oItem = m_poListCtrl->GetNextItem(oItem); // Valid, Damaged, Lost, Shuffled, LastID
         m_poListCtrl->SetItemText(oItem, 1, "error");
      }
      SetNextULItem(roFrameData.m_RadarErrorRet); // Buffer Verification Error
   }
#else // USE_SIMPLE_TEXT_CTRL
   wxString oStr("Frame\n");
   oStr += wxString::Format("\tNumber = %u\n", roServData.ulFrameNum);
   oStr += wxString::Format("\tReceived = %u\n", roFrameData.m_ReceivedFramesCount);
   oStr += wxString::Format("\tMissed = %u\n", roFrameData.m_ErrorFramesCount);
   oStr += wxString::Format("\tFPS = %.2f\n", CamM.GetFPS());
   oStr += GenerateState(eState);

   oStr += wxString::Format("Device\n");
   oStr += wxString::Format("\tIris = %u\n", roServData.usCurrIris);
   oStr += wxString::Format("\tExposure = %u\n", roServData.ulCurrShutter);
   oStr += wxString::Format("\tGain = %u\n", roServData.usCurrGain);
   oStr += wxString::Format("\tWidth = %u\n", roServData.usWidth);
   oStr += wxString::Format("\tHeight = %u\n", roServData.usHeight);
   oStr += wxString::Format("\tTemperature = %d\n", static_cast<char>(roServData.usCurrTemperature));

   oStr += wxString::Format("GNSS\n");
   oStr += wxString::Format("\tType = %c\n", roServData.gnssExposure.chGnssType);
   oStr += wxString::Format("\tFrame date = %u.%u.%u\n", roServData.gnssExposure.chDay, roServData.gnssExposure.chMonth, roServData.gnssExposure.chYear);
   oStr += wxString::Format("\tFrame time = %u.%u.%u\n", roServData.gnssExposure.chHour, roServData.gnssExposure.chMinute, roServData.gnssExposure.chSecond);
   oStr += wxString::Format("\tRef clock period (ps) = %u\n", roServData.gnssExposure.ulRefClockPeriod);
   oStr += wxString::Format("\tClock start exposure = %u\n", roServData.gnssExposure.ulPartSecondExposureStart);
   oStr += wxString::Format("\tClock end exposure = %u\n", roServData.gnssExposure.ulPartSecondExposureEnd);
   oStr += wxString::Format("\tFlags = 0x%x\n", roServData.gnssExposure.chFlags);

   double ntp_es = static_cast<double>(roServData.ulNTP_PartSecondExposureStart);
   ntp_es /= 4294967296;
   double ntp_ee = static_cast<double>(roServData.ulNTP_PartSecondExposureEnd);
   ntp_ee /= 4294967296;
   std::time_t rawtime = roServData.ulNTP_SecondExposureStart - NTP_TIMESTAMP_DIFF;
   auto ptm = std::localtime(&rawtime);

   oStr += wxString::Format("SNTP\n");
   oStr += wxString::Format("\tStart exposure date = %u.%u.%u\n", ptm->tm_mday, (ptm->tm_mon + 1), (ptm->tm_year + 1900));
   oStr += wxString::Format("\tStart exposure time = %u.%u.%u, + %10fs\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, ntp_es);
   rawtime = roServData.ulNTP_SecondExposureEnd - NTP_TIMESTAMP_DIFF;
   ptm = std::localtime(&rawtime);
   oStr += wxString::Format("\tEnd exposure date = %u.%u.%u\n", ptm->tm_mday, (ptm->tm_mon + 1), (ptm->tm_year + 1900));
   oStr += wxString::Format("\tEnd exposure time = %u.%u.%u, + %10fs\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, ntp_ee);

   oStr += wxString::Format("Radar\n");
   oStr += wxString::Format("\tMessages = %u\n", roServData.chRadarMsgCount);
   oStr += wxString::Format("\tData Size = %u\n", roServData.usRadarDataBytes);
   oStr += wxString::Format("\tData\n");
   for(std::size_t i=0; i<=ciRadarDataMaxIndex; ++i)
   {
      oStr += wxString::Format("\t\tFlags = 0x%x\n", roServData.RadarData[i]);
   }
   oStr += wxString::Format("\tBuffer Packets\n");
   if (roFrameData.m_RadarErrorRet == 0)
   {
      oStr += wxString::Format("\t\tValid = %u\n", roFrameData.m_RetValidNum);
      oStr += wxString::Format("\t\tDamaged = %u\n", roFrameData.m_RetDamagedNum);
      oStr += wxString::Format("\t\tLost = %u\n", roFrameData.m_RetcntLostPacks);
      oStr += wxString::Format("\t\tShuffled = %u\n", roFrameData.m_RetcntShuffledPacks);
      oStr += wxString::Format("\t\tLastID = %u", roFrameData.m_RetlastPacketId);
   }
   else
   {
      oStr += wxString::Format("\t\tBuffer Verification Error = %u", roFrameData.m_RadarErrorRet);
   }
   m_poTextCtrl->SetValue(oStr);
#endif // USE_SIMPLE_TEXT_CTRL
}

void OFrameInfo::DisplayData_Impl(const STREAM_SERV_DATA_BRZ_5012HM_GE_V1& roServData, ETransferState eState)
{
   //if (!m_poListCtrl->GetFirstItem().IsOk())
   {
      
   }
}

void OFrameInfo::ProcessNewFrame()
{
   std::lock_guard oLock(m_oUpdateProcessedMutex);
   if (m_bUpdateProcessed)
   {
      m_bUpdateProcessed = false;
      CallAfter(&OFrameInfo::ProcessNewFrameImpl);
      //wxQueueEvent(this, new wxCommandEvent(NEW_FRAME_RECEIVED_EVENT, GetId()));
   }
}

void OFrameInfo::OnProcessNewFrame(wxCommandEvent& roEvent_p)
{
   ProcessNewFrameImpl();
}

void OFrameInfo::ProcessNewFrameImpl()
{
   m_oNewFrameFPSCalculator.Next();

   {
      std::lock_guard oLock(m_oUpdateProcessedMutex);
      m_bUpdateProcessed = true;
   }

   if (!IsVisible())
   {
      return;
   }
   auto& roCamM = CamM;
   auto oLocker = roCamM.SharedLockFrameData();
   const auto& roFrameData = roCamM.GetFrameData();
   if (!roFrameData.GetFrameBuffer().IsOk())
   {
      return;
   }

   switch (roFrameData.m_oServiceData.index())
   {
   case std::variant_npos:
   case 0:
      DisplayNoData(roFrameData.m_eState);
      break;
   case 1:
      DisplayData(std::get<1>(roFrameData.m_oServiceData), roFrameData.m_eState);
      break;
   case 2:
      DisplayData(std::get<2>(roFrameData.m_oServiceData), roFrameData.m_eState);
      break;
   case 3:
      DisplayData(std::get<3>(roFrameData.m_oServiceData), roFrameData.m_eState);
      break;
   case 4:
      DisplayData(std::get<4>(roFrameData.m_oServiceData), roFrameData.m_eState);
      break;
   }
}

void OFrameInfo::PrepareForStreamStart(eseusbcam_long_t FrameSizeX, eseusbcam_long_t FrameSizeY)
{
}
