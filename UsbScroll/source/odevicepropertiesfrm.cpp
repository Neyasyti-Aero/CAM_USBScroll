
#include "odevicepropertiesfrm.h"
#include "ocameradatamanager.h"
#include "ofeaturecontrol.h"
#include <string>
#include "wx/msgdlg.h"

namespace
{
   bool GetValueFromTC(const wxTextCtrl &TC, eseusbcam_unsigned_long_t &rVal)
   {
      try
      {
         rVal = std::stoul(TC.GetValue().ToStdString());
      }
      catch (const std::exception &e)
      {
         wxMessageBox("Please check the value is integer and positive");
         return false;
      }
      return true;
   }
} // namespace

ODevicePropertiesFrm::ODevicePropertiesFrm(wxWindow* poParent)
    : ODevicePropertiesFrmBase(poParent)
{
   m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(OCameraDataManager::EFeature::eBits,
                                                                            m_poDevicePanel,
                                                                            OFeatureControl::ELook::eNamedBox).release());
   m_poBitsAndSaveSizer->Insert(0, m_oFeatureControls.back(), 1, wxEXPAND | wxALL, 5);
   auto AddScanAreaFeatureControl = [&](OCameraDataManager::EFeature eFeature) 
   {
      m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(eFeature,
                                                                               m_poModeGB->GetStaticBox(),
                                                                               OFeatureControl::ELook::eControlOnly).release());
      wxStaticText* poLabel = new wxStaticText(m_poModeGB->GetStaticBox(), wxID_ANY, m_oFeatureControls.back()->GetFeatureName());
      poLabel->Wrap(-1);
      m_oScanAreaSizer->Add(poLabel, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5 );
      m_oScanAreaSizer->Add(m_oFeatureControls.back(), 1, wxEXPAND | wxALL | wxALIGN_CENTER_VERTICAL, 5 );
      m_oScanAreaControls.push_back(m_oFeatureControls.back());
   };
   AddScanAreaFeatureControl(OCameraDataManager::EFeature::eWidthFramePos);
   m_oFeatureControls.back()->SetObserver(this);
   AddScanAreaFeatureControl(OCameraDataManager::EFeature::eHeightFramePos);
   m_oFeatureControls.back()->SetObserver(this);
   AddScanAreaFeatureControl(OCameraDataManager::EFeature::eLeftFramePos);
   AddScanAreaFeatureControl(OCameraDataManager::EFeature::eTopFramePos);

   auto AddSignalFeatureControl = [&](OCameraDataManager::EFeature eFeature, bool bObserverRequired = false) 
   {
      m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(eFeature,
                                                                               m_poSignal,
                                                                               OFeatureControl::ELook::eNamedBox).release());
      m_poSignalSizer->Add(m_oFeatureControls.back(), 0, wxEXPAND | wxALL, 5 );
      if (bObserverRequired)
      {
         m_oFeatureControls.back()->SetObserver(this);
      }
   };
   AddSignalFeatureControl(OCameraDataManager::EFeature::eGain);
   AddSignalFeatureControl(OCameraDataManager::EFeature::eShutter, true);
   AddSignalFeatureControl(OCameraDataManager::EFeature::eIris);
   AddSignalFeatureControl(OCameraDataManager::EFeature::eSharpness);
   AddSignalFeatureControl(OCameraDataManager::EFeature::eBrightness);
   AddSignalFeatureControl(OCameraDataManager::EFeature::eFrameExposure, true);
   AddSignalFeatureControl(OCameraDataManager::EFeature::eGamma);

   auto AddColorFeatureControl = [&](OCameraDataManager::EFeature eFeature)
   {
      m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(eFeature,
                                                                               m_poColor,
                                                                               OFeatureControl::ELook::eNamedBox).release());
      m_poColorSizer->Add(m_oFeatureControls.back(), 0, wxEXPAND | wxALL, 5 );
   };
   AddColorFeatureControl(OCameraDataManager::EFeature::eRed);
   AddColorFeatureControl(OCameraDataManager::EFeature::eGreen);
   AddColorFeatureControl(OCameraDataManager::EFeature::eBlue);
   AddColorFeatureControl(OCameraDataManager::EFeature::eSaturation);

   m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(OCameraDataManager::EFeature::eCompressionLevel,
                                                                            m_poCompress,
                                                                            OFeatureControl::ELook::eNamedBox).release());
   m_poCompressSizer->Add(m_oFeatureControls.back(), 0, wxEXPAND | wxALL, 5);

   m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(OCameraDataManager::EFeature::eAutoPeakAverage,
                                                                            m_poAdvanced,
                                                                            OFeatureControl::ELook::eNamedBox).release());
   m_poAdvancedSizer->Insert(0,m_oFeatureControls.back(), 0, wxEXPAND | wxALL, 5);

   m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(OCameraDataManager::EFeature::eAutoLevel,
                                                                            m_poAdvanced,
                                                                            OFeatureControl::ELook::eNamedBox).release());
   m_poAdvancedSizer->Insert(1,m_oFeatureControls.back(), 0, wxEXPAND | wxALL, 5);

   m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(OCameraDataManager::EFeature::eDSTSyncOut0,
                                                                            m_poOutputTimesSizer->GetStaticBox(),
                                                                            OFeatureControl::ELook::eNameControl).release());
   m_poDSTControl = m_oFeatureControls.back();
   m_poOutputTimesSizer->Insert(0,m_oFeatureControls.back(), 0, wxEXPAND | wxALL, 5);

   m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(OCameraDataManager::EFeature::eETSyncOut0,
                                                                            m_poOutputTimesSizer->GetStaticBox(),
                                                                            OFeatureControl::ELook::eNameControl).release());
   m_poETControl = m_oFeatureControls.back();
   m_poOutputTimesSizer->Insert(1,m_oFeatureControls.back(), 0, wxEXPAND | wxALL, 5);
   
   m_oFeatureControls.push_back(OFeatureControlHelper::CreateFeatureControl(OCameraDataManager::EFeature::eSNTPTimerPeriod,
                                                                            m_poAdvanced,
                                                                            OFeatureControl::ELook::eNamedBox).release());
   m_poAdvancedSizer->Insert(4,m_oFeatureControls.back(), 0, wxEXPAND | wxALL, 5);

   OCameraDataManager::GetInstance().SetLoadCamerasListAct(
      std::bind(&ODevicePropertiesFrm::UpdateControlsOnDeviceListUpdated,this,std::placeholders::_1));

   m_poScrolledArea->Fit();
   //Fit();
   //SetMinSize(GetSize());
}

void ODevicePropertiesFrm::OnDevicesListChoice( wxCommandEvent& event )
{
   OCameraDataManager::GetInstance().SetActiveCamera(event.GetString().ToStdString());
   UpdateControlsAfterCameraChanged();
}

void ODevicePropertiesFrm::OnGetDevicesListButtonClick( wxCommandEvent& event )
{
    OCameraDataManager::GetInstance().LoadCamerasList();
}

void ODevicePropertiesFrm::UpdateControlsOnDeviceListUpdated(const std::string& sMessage)
{
    std::vector<std::string> oNames = OCameraDataManager::GetInstance().GetCameraNames();
    m_poDevicesListCh->Clear();
    for(const auto& oName : oNames)
    {
       m_poDevicesListCh->Append(wxString(oName));
    }
    m_poDevicesListCh->SetStringSelection(OCameraDataManager::GetInstance().GetCurrentCameraName());
    UpdateControlsAfterCameraChanged();
    if (!sMessage.empty())
    {
       wxMessageBox(sMessage);
    }
}

void ODevicePropertiesFrm::OnSaveSettingsBtnClick( wxCommandEvent& event )
{
   if (!OCameraDataManager::GetInstance().SaveSettings())
   {
      wxMessageBox("Error during parameters saving");
   }
}

void ODevicePropertiesFrm::OnApplyStream0BtnClick( wxCommandEvent& event )
{
   long lPort = 0;
   if (!m_poPortStream0TC->GetValue().ToLong(&lPort))
   {
      wxMessageBox("Port is wrong");
      return;
   }
   long lPacketSize = 0;
   if (!m_poPacketSizeStream0TC->GetValue().ToLong(&lPacketSize))
   {
      wxMessageBox("Packet Size is wrong");
      return;
   }
   if (lPacketSize < 1024 || lPacketSize > 8192)
   {
      wxMessageBox("Packet Size ,ust be in the range of 1024 and 8192");
      return;
   }
   if (!OCameraDataManager::GetInstance().SetStream0Params(lPort, lPacketSize))
   {
      wxMessageBox("Error during appling of Stream0 params");
   }
}

void ODevicePropertiesFrm::UpdateControlsAfterCameraChanged()
{
   const OCameraDataManager& roCamMgr = OCameraDataManager::GetInstance();
   // all scrolled features

   for (auto& FeatureControl : m_oFeatureControls)
   {
      FeatureControl->UpdateControlsAfterCameraChanged();
   }

   // Device
   m_poModelTxt->SetValue(roCamMgr.GetCurrentCameraModelName());
   m_poSerialTxt->SetValue(roCamMgr.GetCurrentCameraSerial());
   
   m_poFormatChs->Clear();
   auto oFormats = roCamMgr.GetFormatNames();
   for(const auto& oFormat : oFormats)
   {
      m_poFormatChs->AppendString(oFormat);
   }
   m_poFormatChs->SetSelection(roCamMgr.GetCurrentFormat());

   m_poTriggerModeSizer->GetStaticBox()->Enable(roCamMgr.GetTriggerMode().Presence > 0);
   UpdateTriggerModeControls();
   UpdateImageOptionsControls();

   // Signal
   m_poMaxIrisValueTC->SetValue(std::to_string(roCamMgr.GetFeatureRange(OCameraDataManager::EFeature::eIris).second));

   // Advanced
   m_poTemperaturetTC->SetValue("");

   m_poEnableDelayTimingCh->SetValue(CamM.GetFeaturePresence(m_poDSTControl->GetFeature()));

   m_poPortStream0TC->SetValue(std::to_string(roCamMgr.GetStream0Port()));
   m_poPacketSizeStream0TC->SetValue(std::to_string(roCamMgr.GetStream0PacketSize()));

}

void ODevicePropertiesFrm::UpdateTriggerModeControls()
{
   const auto &TriggerMode = OCameraDataManager::GetInstance().GetTriggerMode();
   m_poTriggerOnOffChb->SetValue(TriggerMode.OnOff > 0);
   m_poInversePolarityChb->Enable(TriggerMode.OnOff > 0);
   m_poInversePolarityChb->SetValue(TriggerMode.Polarity > 0);

   m_poTriggerModeHardwareShutterRB->Enable(TriggerMode.OnOff > 0);
   m_poTriggerModeHardwareSignalRB->Enable(TriggerMode.OnOff > 0);
   m_poTriggerModeInternalSoftwareRB->Enable(TriggerMode.OnOff > 0);
   UpdateTriggerModeRB(TriggerMode.TriggerMode);
   m_poGetFrameBtn->Enable(TriggerMode.TriggerMode == 3 && TriggerMode.OnOff > 0);
}

void ODevicePropertiesFrm::UpdateTriggerModeRB(eseusbcam_unsigned_long_t TMValue)
{
   switch (TMValue)
   {
   case 0:
      m_poTriggerModeHardwareShutterRB->SetValue(true);
      break;
   case 1:
      m_poTriggerModeHardwareSignalRB->SetValue(true);
      break;
   case 3:
      m_poTriggerModeInternalSoftwareRB->SetValue(true);
      break;
   }
}

void ODevicePropertiesFrm::OnFrameClose(wxCloseEvent &event)
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
   event.Skip(false);
}

void ODevicePropertiesFrm::SetCloseEventProcessor(const std::function<void(wxCommandEvent&)>& poCloseProcessor)
{
   m_poCloseProcessor = poCloseProcessor;
}

void ODevicePropertiesFrm::FeatureChanged(OCameraDataManager::EFeature eFeatureID, eseusbcam_unsigned_long_t NewValue)
{
   OFeatureControl* pControl = nullptr;
   switch(eFeatureID)
   {
   case OCameraDataManager::EFeature::eFrameExposure:
      pControl = GetFeatureControl(OCameraDataManager::EFeature::eShutter);
      break;
   case OCameraDataManager::EFeature::eWidthFramePos:
      pControl = GetFeatureControl(OCameraDataManager::EFeature::eLeftFramePos);
      break;
   case OCameraDataManager::EFeature::eHeightFramePos:
      pControl = GetFeatureControl(OCameraDataManager::EFeature::eTopFramePos);
      break;
   case OCameraDataManager::EFeature::eShutter:
      m_poDirectShutterValueTC->SetValue(std::to_string(OCameraDataManager::GetInstance().ConvertShutterToDirect(NewValue)));
      break;
   }
   if (pControl)
   {
      pControl->UpdateRangeAndCheckValue();
   }
}


void ODevicePropertiesFrm::OnSetMaxIrisValue(wxCommandEvent &event)
{
   eseusbcam_unsigned_long_t Val = 0;
   if (!GetValueFromTC(*m_poMaxIrisValueTC, Val))
   {
      return;
   }
   if (Val > 255)
   {
      wxMessageBox("Please specify the value in the range of 0, 255");
      return;
   }
   OCameraDataManager::GetInstance().SetFeatureValue(OCameraDataManager::EFeature::eIrisInqOffset, Val);
   auto Control = GetFeatureControl(OCameraDataManager::EFeature::eIris);
   if (Control)
   {
      Control->UpdateRangeAndCheckValue();
   }
}

void ODevicePropertiesFrm::OnSetDirectShutterValue(wxCommandEvent &event)
{
   eseusbcam_unsigned_long_t Val = 0;
   if (!GetValueFromTC(*m_poDirectShutterValueTC, Val))
   {
      return;
   }
   Val = OCameraDataManager::GetInstance().ConvertShutterFromDirect(Val);
   auto Control = GetFeatureControl(OCameraDataManager::EFeature::eShutter);
   if (Control)
   {
      Control->SetFeatureValue(Val);
   }
}

OFeatureControl* ODevicePropertiesFrm::GetFeatureControl(OCameraDataManager::EFeature eFeatureID)
{
   auto pControl = std::find_if(m_oFeatureControls.begin(), m_oFeatureControls.end(),
                                [&eFeatureID](const OFeatureControl *pFC) { return pFC->GetFeature() == eFeatureID; });
   if (pControl != m_oFeatureControls.end())
   {
      return *pControl;
   }
   return nullptr;
}

const OFeatureControl* ODevicePropertiesFrm::GetFeatureControl(OCameraDataManager::EFeature eFeatureID) const
{
   return const_cast<ODevicePropertiesFrm*>(this)->GetFeatureControl(eFeatureID);
}

void ODevicePropertiesFrm::OnFormatSelectionChanged( wxCommandEvent& event )
{
   if (OCameraDataManager::GetInstance().SetCurrentFormat(m_poFormatChs->GetSelection()))
   {
      UpdateControlsAfterCameraChanged();
   }
}

void ODevicePropertiesFrm::OnApplyScanArea( wxCommandEvent& event )
{
   if (OCameraDataManager::GetInstance().SetScanArea())
   {
      UpdateControlsAfterCameraChanged();
   }
}

void ODevicePropertiesFrm::OnGetTemperature( wxCommandEvent& event )
{
   m_poTemperaturetTC->SetValue(std::to_string(static_cast<int>(
      static_cast<char>(OCameraDataManager::GetInstance().GetFeatureValue(OCameraDataManager::EFeature::eTemperature)))));
}

void ODevicePropertiesFrm::OnTriggerOnOffChb( wxCommandEvent& event )
{
   TRIGGER_CTRL_REG oTCR = CamM.GetTriggerMode();
   oTCR.OnOff = m_poTriggerOnOffChb->GetValue() ? 1 : 0;
   if (CamM.SetTriggerModeValue(oTCR))
   {
      UpdateTriggerModeControls();
   }
   else
   {
      m_poTriggerOnOffChb->SetValue(CamM.GetTriggerMode().OnOff > 0);
   }
}

void ODevicePropertiesFrm::OnTriggerPolarityChb( wxCommandEvent& event )
{
   TRIGGER_CTRL_REG oTCR = CamM.GetTriggerMode();
   oTCR.Polarity = m_poInversePolarityChb->GetValue() ? 1 : 0;
   if (!CamM.SetTriggerModeValue(oTCR))
   {
      m_poInversePolarityChb->SetValue(CamM.GetTriggerMode().Polarity > 0);
   }
}

void ODevicePropertiesFrm::OnGetFrameBtn( wxCommandEvent& event )
{
   CamM.SetFeatureValue(OCameraDataManager::EFeature::eOneMultiFrameRequest, 1);
}

void ODevicePropertiesFrm::OnTriggerModeRB( wxCommandEvent& event )
{
   char NewTM = -1;
   if (m_poTriggerModeHardwareShutterRB->GetValue())
   {
      NewTM = 0;
   }
   else if (m_poTriggerModeHardwareSignalRB->GetValue())
   {
      NewTM = 1;
   }
   else if (m_poTriggerModeInternalSoftwareRB->GetValue())
   {
      NewTM = 3;
   }
   TRIGGER_CTRL_REG oTCR = CamM.GetTriggerMode();
   if (NewTM < 0 || static_cast<eseusbcam_unsigned_long_t>(NewTM) == oTCR.TriggerMode)
   {
      return;
   }
   oTCR.TriggerMode = static_cast<eseusbcam_unsigned_long_t>(NewTM);
   if (!CamM.SetTriggerModeValue(oTCR))
   {
      UpdateTriggerModeRB(CamM.GetTriggerMode().TriggerMode);
   }
   m_poGetFrameBtn->Enable(CamM.GetTriggerMode().TriggerMode == 3);
}

void ODevicePropertiesFrm::OnOutputRB( wxCommandEvent& event )
{
   if (m_poOut0R->GetValue())
   {
      m_poDSTControl->ChangeControlledFeature(OCameraDataManager::EFeature::eDSTSyncOut0);
      m_poETControl->ChangeControlledFeature(OCameraDataManager::EFeature::eETSyncOut0);
   }
   else if (m_poOut1R->GetValue())
   {
      m_poDSTControl->ChangeControlledFeature(OCameraDataManager::EFeature::eDSTSyncOut1);
      m_poETControl->ChangeControlledFeature(OCameraDataManager::EFeature::eETSyncOut1);
   }
   else if (m_poOut2R->GetValue())
   {
      m_poDSTControl->ChangeControlledFeature(OCameraDataManager::EFeature::eDSTSyncOut2);
      m_poETControl->ChangeControlledFeature(OCameraDataManager::EFeature::eETSyncOut2);
   }
   else if (m_poOut3R->GetValue())
   {
      m_poDSTControl->ChangeControlledFeature(OCameraDataManager::EFeature::eDSTSyncOut3);
      m_poETControl->ChangeControlledFeature(OCameraDataManager::EFeature::eETSyncOut3);
   }
   m_poEnableDelayTimingCh->SetValue(CamM.GetFeaturePresence(m_poDSTControl->GetFeature()));
}

void ODevicePropertiesFrm::OnEnableDelayTimingCh( wxCommandEvent& event )
{
   eseusbcam_unsigned_long_t Index = 0;
   switch(m_poDSTControl->GetFeature())
   {
   case OCameraDataManager::EFeature::eDSTSyncOut0:
      Index = 0;
      break;
   case OCameraDataManager::EFeature::eDSTSyncOut1:
      Index = 1;
      break;
   case OCameraDataManager::EFeature::eDSTSyncOut2:
      Index = 2;
      break;
   case OCameraDataManager::EFeature::eDSTSyncOut3:
      Index = 3;
      break;
   }
   if (CamM.SetEnableDelayTiming(Index,m_poEnableDelayTimingCh->GetValue()))
   {
      m_poDSTControl->UpdateControlsAfterCameraChanged();
      m_poETControl->UpdateControlsAfterCameraChanged();
   }
   else
   {
      m_poEnableDelayTimingCh->SetValue(CamM.GetFeaturePresence(m_poDSTControl->GetFeature()));
   }
}

namespace
{
   std::vector<unsigned char> GenerateLUTTableExampleData()
   {
      std::vector<unsigned char> oData(4096);
      for(eseusbcam_unsigned_long_t Index = 0; Index < oData.size(); ++Index)
      {
         oData[Index] = (Index >> 4) & 0xff;
      }
      return oData;
   }
}
void ODevicePropertiesFrm::OnSetLUTTableBtn( wxCommandEvent& event )
{
   std::vector<unsigned char> oData = GenerateLUTTableExampleData();
   wxString oStr("Set is Ok");
   if (!CamM.SendLutTable(oData))
   {
      oStr = "Set Table error";
   }
   m_poLUTGetResultTxt->SetLabelText(oStr);
}

void ODevicePropertiesFrm::OnGetLUTTableBtn( wxCommandEvent& event )
{
   std::vector<unsigned char> oDataToSend = GenerateLUTTableExampleData();
   std::vector<unsigned char> oData(oDataToSend.size(), 0);
   wxString oStr("Same as Set");
   auto oStartT = std::chrono::high_resolution_clock::now();
   if (CamM.ReceiveLutTable(oData))
   {
      auto oEndT = std::chrono::high_resolution_clock::now();
      if (oDataToSend != oData)
      {
         oStr = "Differs from Set";
      }
      oStr << ", " << std::chrono::duration<double, std::chrono::milliseconds::period>(oEndT-oStartT).count() << " ms";
   }
   else
   {
      oStr = "Get Table error";
   }

   m_poLUTGetResultTxt->SetLabelText(oStr);
}


void ODevicePropertiesFrm::OnImageMirrorFlipFDGainChB( wxCommandEvent& event )
{
   auto oFO = CamM.GetCameraImageFormatOptions();
   oFO.Mirror = oFO.MirrorAvailable && m_poMirrorImageChB->GetValue();
   oFO.Flip = oFO.FlipAvailable && m_poFlipImageChB->GetValue();
   oFO.FDGain = oFO.FDGainAvailable && m_poFDGainChB->GetValue();

   if (!CamM.SetCameraImageFormatOptions(oFO))
   {
      UpdateImageOptionsControls();
   }
}

void ODevicePropertiesFrm::UpdateImageOptionsControls()
{
   const auto& roFO = CamM.GetCameraImageFormatOptions();

   m_poMirrorImageChB->Enable(roFO.MirrorAvailable);
   m_poMirrorImageChB->SetValue(roFO.MirrorAvailable && roFO.Mirror);
   m_poFlipImageChB->Enable(roFO.FlipAvailable);
   m_poFlipImageChB->SetValue(roFO.FlipAvailable && roFO.Flip);
   m_poFDGainChB->Enable(roFO.FDGainAvailable);
   m_poFDGainChB->SetValue(roFO.FDGainAvailable && roFO.FDGain);
}