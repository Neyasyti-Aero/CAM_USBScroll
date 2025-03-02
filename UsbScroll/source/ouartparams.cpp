// @file ouartparams.cpp

#include "ouartparams.h"

OUARTParams::OUARTParams(wxWindow* poParent, const wxString& roFilePath_p, int lTimeInterval_p, OUARTModesManager::ESelfTestMode eSelfTestMode_p)
   : OUARTParamsBase(poParent)
{
   m_poLogFilePath->SetPath(roFilePath_p);
   m_poPollingInterval->SetValue(lTimeInterval_p);
   m_poSelfTestModeRBG->Select(eSelfTestMode_p == OUARTModesManager::ESelfTestMode::eShorting ? 0 : 1);
}

wxString OUARTParams::GetLogFilePath() const
{
   return m_poLogFilePath->GetPath();
}

int OUARTParams::GetTimeInterval() const
{
   return m_poPollingInterval->GetValue();
}

OUARTModesManager::ESelfTestMode OUARTParams::GetSelfTestMode() const
{
   return m_poSelfTestModeRBG->GetSelection() == 0 ? OUARTModesManager::ESelfTestMode::eShorting : OUARTModesManager::ESelfTestMode::eCrossOver;
}