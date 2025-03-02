

#include "ofeaturecontrol.h"

#include <map>
namespace
{
   // 0.45 => 9, 0.7 => 14, 1 => 20
   constexpr int gGamma045 = 9;
   constexpr int gGamma070 = 14;
   constexpr int gGamma100 = 20;
}

OFeatureControl::OFeatureControl(OCameraDataManager::EFeature eFeatureID,
                                 wxWindow* parent,
                                 ELook eLook,
                                 wxWindowID id,
                                 const wxPoint& pos,
                                 const wxSize& size,
                                 long style)
 : wxPanel(parent, id, pos, size, style)
 , m_eFeatureID(eFeatureID)
{
   switch(eLook)
   {
   case ELook::eControlOnly:
      {
         wxBoxSizer* poSizer = new wxBoxSizer( wxHORIZONTAL );
         
         m_poFeatureSlider = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
         poSizer->Add( m_poFeatureSlider, 1, wxALL, 5 );
         
         m_poFeatureValue = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
         m_poFeatureValue->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
         
         poSizer->Add( m_poFeatureValue, 0, wxALL, 5 );
         
         SetSizer( poSizer );
         Layout();
         poSizer->Fit( this );
      }
      break;
   case ELook::eNameControl:
      {
         wxBoxSizer* poMainSizer = new wxBoxSizer( wxVERTICAL );

         m_poFeatureNameLabel = new wxStaticText( this, wxID_ANY, GetFeatureName(), wxDefaultPosition, wxDefaultSize, 0 );
         m_poFeatureNameLabel->Wrap( -1 );
	      poMainSizer->Add( m_poFeatureNameLabel, 0, wxALL, 5 );
         
         wxBoxSizer* poSizer = new wxBoxSizer( wxHORIZONTAL );
         
         m_poFeatureSlider = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
         poSizer->Add( m_poFeatureSlider, 1, wxALL, 5 );
         
         m_poFeatureValue = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
         m_poFeatureValue->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
         
         poSizer->Add( m_poFeatureValue, 0, wxALL, 5 );
         
         poMainSizer->Add( poSizer, 0, wxEXPAND, 5 );
         
         SetSizer( poMainSizer );
         Layout();
         poMainSizer->Fit( this );
      }
      break;
   case ELook::eNamedBox:
      {
         m_oFeatureName = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, GetFeatureName() ), wxVERTICAL );
         
         wxBoxSizer* bSizer7;
         bSizer7 = new wxBoxSizer( wxHORIZONTAL );
         
         m_poFeatureSlider = new wxSlider( m_oFeatureName->GetStaticBox(), wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
         bSizer7->Add( m_poFeatureSlider, 1, wxALL, 5 );
         
         m_poFeatureValue = new wxTextCtrl( m_oFeatureName->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
         m_poFeatureValue->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
         
         bSizer7->Add( m_poFeatureValue, 0, wxALL, 5 );
         
         m_oFeatureName->Add( bSizer7, 0, wxEXPAND, 5 );
         
         SetSizer( m_oFeatureName );
         Layout();
         m_oFeatureName->Fit( this );
      }
      break;
   }
	
	// Connect Events
	m_poFeatureSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );

   Enable(OCameraDataManager::GetInstance().GetFeaturePresence(m_eFeatureID));
}

std::string OFeatureControl::GetFeatureName() const
{
   return OCameraDataManager::GetFeatureName(m_eFeatureID);
}

OFeatureControl::~OFeatureControl()
{
	// Disconnect Events
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
	m_poFeatureSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( OFeatureControl::OnScrollFeatureSlider ), NULL, this );
}

void OFeatureControl::OnScrollFeatureSlider( wxScrollEvent& event )
{
   SetValueToTextField(event.GetPosition());
   if (event.GetEventType() == wxEVT_SCROLL_THUMBTRACK)
   {
      return;
   }
   OCameraDataManager::GetInstance().SetFeatureValue(m_eFeatureID, event.GetPosition());
   ReportFeatureChanged(event.GetPosition());
}

void  OFeatureControl::ChangeControlledFeature(OCameraDataManager::EFeature eFeatureID)
{
   std::string OldName = GetFeatureName();
   m_eFeatureID = eFeatureID;
   if (OldName != GetFeatureName())
   {
      if (m_poFeatureNameLabel != nullptr)
      {
         m_poFeatureNameLabel->SetLabelText(GetFeatureName());
      }
      else if (m_oFeatureName != nullptr)
      {
         m_oFeatureName->GetStaticBox()->SetLabelText(GetFeatureName());
      }
   }
   UpdateControlsAfterCameraChanged();
}

void OFeatureControl::UpdateControlsAfterCameraChanged()
{
   const OCameraDataManager& roCamMgr = OCameraDataManager::GetInstance();
   Enable(roCamMgr.GetFeaturePresence(m_eFeatureID));
   auto oRange = roCamMgr.GetFeatureRange(m_eFeatureID);
   m_poFeatureSlider->SetRange(oRange.first, oRange.second);
   m_poFeatureSlider->Enable(oRange.first != oRange.second);
   eseusbcam_unsigned_long_t lVal = roCamMgr.GetFeatureValue(m_eFeatureID);
   SetFeatureSliderValue(lVal, true);
}

void OFeatureControl::UpdateRangeAndCheckValue()
{
   OCameraDataManager& roCamMgr = OCameraDataManager::GetInstance();
   eseusbcam_unsigned_long_t Val = m_poFeatureSlider->GetValue();
   auto oRange = roCamMgr.GetFeatureRange(m_eFeatureID);
   m_poFeatureSlider->SetRange(oRange.first, oRange.second);
   m_poFeatureSlider->Enable(oRange.first != oRange.second);
   if (Val < oRange.first || Val > oRange.second)
   {
      Val = Val > oRange.second ? oRange.second : oRange.first;
      roCamMgr.SetFeatureValue(m_eFeatureID, Val);
      SetFeatureSliderValue(Val, true);
   }
}

void OFeatureControl::SetValueToTextField(eseusbcam_unsigned_long_t lVal)
{
   switch(m_eFeatureID)
   {
   case OCameraDataManager::EFeature::eShutter:
      m_poFeatureValue->SetValue(std::to_string(OCameraDataManager::GetInstance().ConvertShutterToDirect(lVal)) + " uS");
      break;
   case OCameraDataManager::EFeature::eLeftFramePos:
   case OCameraDataManager::EFeature::eWidthFramePos:
      m_poFeatureValue->SetValue(std::to_string(OCameraDataManager::GetInstance().ConvertPartialWidthToFull(lVal)));
      break;
   case OCameraDataManager::EFeature::eTopFramePos:
   case OCameraDataManager::EFeature::eHeightFramePos:
      m_poFeatureValue->SetValue(std::to_string(OCameraDataManager::GetInstance().ConvertPartialHeightToFull(lVal)));
      break;
   default:
      m_poFeatureValue->SetValue(std::to_string(lVal));
      break;
   }
}
void OFeatureControl::SetFeatureSliderValue(eseusbcam_unsigned_long_t lVal, bool bForceUpdate)
{
   if (lVal == m_poFeatureSlider->GetValue() && !bForceUpdate)
   {
      return;
   }
   m_poFeatureSlider->SetValue(lVal);
   SetValueToTextField(lVal);
   ReportFeatureChanged(lVal);
}

bool OFeatureControl::SetFeatureValue(eseusbcam_unsigned_long_t Val)
{
   if (Val == m_poFeatureSlider->GetValue())
   {
      return false;
   }
   m_poFeatureSlider->SetValue(Val);
   wxScrollEvent oEvt(wxEVT_SCROLL_CHANGED, m_poFeatureSlider->GetId(), m_poFeatureSlider->GetValue());
   OFeatureControl::OnScrollFeatureSlider(oEvt);
   return true;
}

void OFeatureControl::ReportFeatureChanged(eseusbcam_unsigned_long_t Val)
{
   if (m_pFeatureChangesObserver)
   {
      m_pFeatureChangesObserver->FeatureChanged(m_eFeatureID, Val);
   }
}

OFeatureControlAuto::OFeatureControlAuto(OCameraDataManager::EFeature eFeatureID,
                                         wxWindow* parent,
                                         wxWindowID id,
                                         const wxPoint& pos,
                                         const wxSize& size,
                                         long style)
 : OFeatureControl(eFeatureID, parent, ELook::eNamedBox, id, pos, size, style)
{
   assert(m_oFeatureName != nullptr && "Feature with Auto possibility can be used with group box only");
   m_poFeatureAutoChk = new wxCheckBox( m_oFeatureName->GetStaticBox(), wxID_ANY, wxT("Auto"), wxDefaultPosition, wxDefaultSize, 0 );
   m_oFeatureName->Add( m_poFeatureAutoChk, 0, wxALL, 5 );

   Layout();

   m_poFeatureAutoChk->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( OFeatureControlAuto::OnFeatureAutoChk ), nullptr, this );
}

OFeatureControlAuto::~OFeatureControlAuto()
{
   m_poFeatureAutoChk->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( OFeatureControlAuto::OnFeatureAutoChk ), nullptr, this );
}

void OFeatureControlAuto::OnFeatureAutoChk( wxCommandEvent& event )
{
   bool bIsOn = event.IsChecked();

   OCameraDataManager::GetInstance().SetFeatureAuto(m_eFeatureID, bIsOn);
   m_poFeatureSlider->Enable(!bIsOn);
   m_poFeatureValue->Enable(!bIsOn);
}

void OFeatureControlAuto::UpdateControlsAfterCameraChanged()
{
   OFeatureControl::UpdateControlsAfterCameraChanged();
   bool bIsOn = OCameraDataManager::GetInstance().GetFeatureAuto(m_eFeatureID);
   m_poFeatureSlider->Enable(!bIsOn);
   m_poFeatureValue->Enable(!bIsOn);
}

OFeatureControlGamma::OFeatureControlGamma(wxWindow* parent,
                                         wxWindowID id,
                                         const wxPoint& pos,
                                         const wxSize& size,
                                         long style)
 : OFeatureControl(OCameraDataManager::EFeature::eGamma, parent, ELook::eNamedBox, id, pos, size, style)
{
   assert(m_oFeatureName != nullptr && "Gamma feature control can be used with group box only");

	wxBoxSizer* ValuesSizer;
	ValuesSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_RB045 = new wxRadioButton( m_oFeatureName->GetStaticBox(), wxID_ANY, wxT("0.45"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	ValuesSizer->Add( m_RB045, 0, wxALL, 5 );
	
	m_RB070 = new wxRadioButton( m_oFeatureName->GetStaticBox(), wxID_ANY, wxT("0.7"), wxDefaultPosition, wxDefaultSize, 0 );
	ValuesSizer->Add( m_RB070, 0, wxALL, 5 );
	
	m_RB100 = new wxRadioButton( m_oFeatureName->GetStaticBox(), wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	ValuesSizer->Add( m_RB100, 0, wxALL, 5 );

	m_RBNone = new wxRadioButton( m_oFeatureName->GetStaticBox(), wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
	m_RBNone->SetValue( true ); 
   m_RBNone->Hide();
	
	m_oFeatureName->Add( ValuesSizer, 1, wxEXPAND, 5 );

   Layout();

	m_RB045->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( OFeatureControlGamma::OnSelectValue ), NULL, this );
	m_RB070->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( OFeatureControlGamma::OnSelectValue ), NULL, this );
	m_RB100->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( OFeatureControlGamma::OnSelectValue ), NULL, this );
}

OFeatureControlGamma::~OFeatureControlGamma()
{
	m_RB045->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( OFeatureControlGamma::OnSelectValue ), NULL, this );
	m_RB070->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( OFeatureControlGamma::OnSelectValue ), NULL, this );
	m_RB100->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( OFeatureControlGamma::OnSelectValue ), NULL, this );
}

void OFeatureControlGamma::OnScrollFeatureSlider(wxScrollEvent &event)
{
   OFeatureControl::OnScrollFeatureSlider(event);
   int iPos = event.GetPosition();
   if (iPos == gGamma045)
   {
      m_RB045->SetValue(true);
   }
   else if (iPos == gGamma070)
   {
      m_RB070->SetValue(true);
   }
   else if (iPos == gGamma100)
   {
      m_RB100->SetValue(true);
   }
   else
   {
      m_RBNone->SetValue(true);
   }
}

void OFeatureControlGamma::OnSelectValue(wxCommandEvent &event)
{
   // в общем случае значение для 0.45 => 9, 0.7 => 14, 1 => 20
   if (m_RB045->GetValue())
   {
      SetFeatureValue(gGamma045);
   }
   else if (m_RB070->GetValue())
   {
      SetFeatureValue(gGamma070);
   }
   else if (m_RB100->GetValue())
   {
      SetFeatureValue(gGamma100);
   }
}

void OFeatureControlGamma::UpdateControlsAfterCameraChanged()
{
   OFeatureControl::UpdateControlsAfterCameraChanged();
   wxScrollEvent oEvt(wxEVT_SCROLL_THUMBTRACK, m_poFeatureSlider->GetId(), m_poFeatureSlider->GetValue());
   OnScrollFeatureSlider(oEvt);
}

std::unique_ptr<OFeatureControl> OFeatureControlHelper::CreateFeatureControl(OCameraDataManager::EFeature eFeatureID,
                                                                             wxWindow *parent,
                                                                             OFeatureControl::ELook eLook,
                                                                             wxWindowID id,
                                                                             const wxPoint &pos,
                                                                             const wxSize &size,
                                                                             long style)
{
   if (eFeatureID == OCameraDataManager::EFeature::eGamma)
   {
      return std::unique_ptr<OFeatureControl>(new OFeatureControlGamma(parent, id, pos, size, style));
   }
   else if (OCameraDataManager::IsAutoSupported(eFeatureID))
   {
      return std::unique_ptr<OFeatureControl>(new OFeatureControlAuto(eFeatureID, parent, id, pos, size, style));
   }
   else
   {
      return std::unique_ptr<OFeatureControl>(new OFeatureControl(eFeatureID, parent, eLook, id, pos, size, style));
   }
}
