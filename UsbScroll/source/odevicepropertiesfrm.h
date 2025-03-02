/// @file odevicepropertiesdlg.h

#pragma once

#include "usbscrollguibase.h"
#include <vector>
#include "ofeaturecontrol.h"
#include <functional>

class ODevicePropertiesFrm : public ODevicePropertiesFrmBase, public IFeatureChangesObserver
{

public:
    ODevicePropertiesFrm(wxWindow* poParent);

   void OnFrameClose( wxCloseEvent& event ) override;
	void OnDevicesListChoice( wxCommandEvent& event ) override;
	void OnGetDevicesListButtonClick( wxCommandEvent& event ) override;
	void OnSaveSettingsBtnClick( wxCommandEvent& event ) override;
   void OnApplyStream0BtnClick( wxCommandEvent& event ) override;
   void OnSetMaxIrisValue(wxCommandEvent &event) override;
   void OnSetDirectShutterValue(wxCommandEvent &event) override;
   void OnFormatSelectionChanged( wxCommandEvent& event ) override;
   void OnApplyScanArea( wxCommandEvent& event ) override;
   void OnGetTemperature( wxCommandEvent& event ) override;
   void OnTriggerOnOffChb( wxCommandEvent& event ) override;
   void OnTriggerPolarityChb( wxCommandEvent& event ) override;
   void OnGetFrameBtn( wxCommandEvent& event ) override;
   void OnTriggerModeRB( wxCommandEvent& event ) override;
   void OnOutputRB( wxCommandEvent& event ) override;
   void OnEnableDelayTimingCh( wxCommandEvent& event ) override;
   void OnSetLUTTableBtn( wxCommandEvent& event ) override;
   void OnGetLUTTableBtn( wxCommandEvent& event ) override;
   void OnImageMirrorFlipFDGainChB( wxCommandEvent& event ) override;

   void FeatureChanged(OCameraDataManager::EFeature eFeatureID, eseusbcam_unsigned_long_t NewValue) override;
   void SetCloseEventProcessor(const std::function<void(wxCommandEvent&)>& poCloseProcessor);

private:
   void UpdateControlsOnDeviceListUpdated(const std::string& sMessage);
   void UpdateControlsAfterCameraChanged();
   void UpdateTriggerModeControls();
   void UpdateTriggerModeRB(eseusbcam_unsigned_long_t TMValue);
   void UpdateImageOptionsControls();

   OFeatureControl* GetFeatureControl(OCameraDataManager::EFeature eFeatureID);
   const OFeatureControl* GetFeatureControl(OCameraDataManager::EFeature eFeatureID) const;

private:
   std::function<void(wxCommandEvent&)> m_poCloseProcessor;
   std::vector<OFeatureControl*> m_oFeatureControls;
   std::vector<OFeatureControl*> m_oScanAreaControls;
   OFeatureControl* m_poDSTControl = nullptr;
   OFeatureControl* m_poETControl = nullptr;
};