
#pragma once

#include "usbscrollguibase.h"
#include "ocameradatamanager.h"

#include "wx/checkbox.h"
#include "wx/radiobut.h"

class IFeatureChangesObserver
{
public:
   virtual void FeatureChanged(OCameraDataManager::EFeature eFeatureID, eseusbcam_unsigned_long_t NewValue) = 0;
};

class OFeatureControl : public wxPanel
{
public:
   enum class ELook
   {
      eControlOnly,
      eNameControl,
      eNamedBox
   };
public:
   OFeatureControl(OCameraDataManager::EFeature eFeatureID,
                   wxWindow* parent,
                   ELook eLook,
                   wxWindowID id = wxID_ANY,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxSize( 500,300 ),
                   long style = wxTAB_TRAVERSAL);
   virtual ~OFeatureControl();

   void ChangeControlledFeature(OCameraDataManager::EFeature eFeatureID);
   virtual void UpdateControlsAfterCameraChanged();
   void SetObserver(IFeatureChangesObserver* pFeatureChangesObserver)
   {
      m_pFeatureChangesObserver = pFeatureChangesObserver;
   }
   OCameraDataManager::EFeature GetFeature() const
   {
      return m_eFeatureID;
   }
   std::string GetFeatureName() const;
   void UpdateRangeAndCheckValue();

   /// Set new value similar to moving slider etc. - changes slider and send to camera
   /// @return is the feature value changed
   /// @see SetFeatureSliderValue
   bool SetFeatureValue(eseusbcam_unsigned_long_t Val);
protected:
   virtual void OnScrollFeatureSlider( wxScrollEvent& event );
   // Set slider only but don't send it to camera
   void SetFeatureSliderValue(eseusbcam_unsigned_long_t Val, bool bForceUpdate = false);
   void ReportFeatureChanged(eseusbcam_unsigned_long_t Val);
   void SetValueToTextField(eseusbcam_unsigned_long_t lVal);
protected:
	wxStaticBoxSizer* m_oFeatureName = nullptr;
   wxStaticText* m_poFeatureNameLabel = nullptr;
	wxSlider* m_poFeatureSlider = nullptr;
	wxTextCtrl* m_poFeatureValue = nullptr;

   OCameraDataManager::EFeature m_eFeatureID;
   IFeatureChangesObserver* m_pFeatureChangesObserver = nullptr;
};

class OFeatureControlAuto : public OFeatureControl
{
public:
   OFeatureControlAuto(OCameraDataManager::EFeature eFeatureID,
                       wxWindow* parent,
                       wxWindowID id = wxID_ANY,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxSize( 500,300 ),
                       long style = wxTAB_TRAVERSAL);

   virtual ~OFeatureControlAuto();
   virtual void UpdateControlsAfterCameraChanged() override;
protected:
   virtual void OnFeatureAutoChk( wxCommandEvent& event );

private:
   wxCheckBox* m_poFeatureAutoChk = nullptr;
};

class OFeatureControlGamma : public OFeatureControl
{
public:
   OFeatureControlGamma(wxWindow* parent,
                        wxWindowID id = wxID_ANY,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxSize( 500,300 ),
                        long style = wxTAB_TRAVERSAL);
   virtual ~OFeatureControlGamma();

   virtual void UpdateControlsAfterCameraChanged() override;
protected:
   virtual void OnScrollFeatureSlider(wxScrollEvent &event);
   virtual void OnSelectValue(wxCommandEvent &event);

private:
   wxRadioButton* m_RB045 = nullptr;
   wxRadioButton* m_RB070 = nullptr;
   wxRadioButton* m_RB100 = nullptr;
   wxRadioButton* m_RBNone = nullptr;
};

class OFeatureControlHelper
{
public:
   static std::unique_ptr<OFeatureControl> CreateFeatureControl(OCameraDataManager::EFeature eFeatureID,
                                                                wxWindow *parent,
                                                                OFeatureControl::ELook eLook,
                                                                wxWindowID id = wxID_ANY,
                                                                const wxPoint &pos = wxDefaultPosition,
                                                                const wxSize &size = wxDefaultSize,
                                                                long style = wxTAB_TRAVERSAL);
};