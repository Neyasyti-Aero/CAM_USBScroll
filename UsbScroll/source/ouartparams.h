// @file ouartparams.h

#ifndef OUARTPARAMS_h
#define OUARTPARAMS_h

#include "usbscrollguibase.h"

#include "ouartmodesmanager.h"

class OUARTParams : public OUARTParamsBase
{
public:
   OUARTParams(wxWindow* poParent, const wxString& roFilePath_p, int lTimeInterval_p, OUARTModesManager::ESelfTestMode eSelfTestMode_p);

   wxString GetLogFilePath() const;
   int GetTimeInterval() const;
   OUARTModesManager::ESelfTestMode GetSelfTestMode() const;
};

#endif // OUARTPARAMS_h