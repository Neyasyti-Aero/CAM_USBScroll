#ifndef OUARTMODESMANAGER_H
#define OUARTMODESMANAGER_H

#include <atomic>
#include <mutex>
#include <array>
#include <vector>
#include <fstream>
#include <functional>

#include <wx/string.h>
#include <wx/window.h>
#include <wx/stdpaths.h>

class OUARTModesManager
{
public:
   enum class EMode
   {
      eNone,
      eLogging,
      eSelfTest
   };

   enum class ESelfTestMode
   {
      eShorting, // каждый UART замкнут сам на себя
      eCrossOver // два UART замкнуты друг на друга
   };

public:
   OUARTModesManager();
   ~OUARTModesManager();

   void SelectParams(wxWindow* poParent_p);
   void SetMode(EMode eMode_p);

   void SetLogAbortedReporting(std::function<void()> poAbortReporter_p);

private:
   void Start();
   void Stop();

   void DataReceiving();
   void WriteBuffer(std::vector<unsigned char>& roBuffer_p, std::ofstream& roOut_p);

   void SelfTest();

   void ReportAborted();

private:
   EMode m_eMode = EMode::eNone;
   
   std::atomic_bool m_oExecuteThread = false;
   std::mutex m_oWorkerThreadFinishedMutex;
   std::function<void()> m_poAbortReporter;

   wxString m_oFilePath = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir::Dir_Documents) + "/uart_log.txt";
   ESelfTestMode m_eSelfTestMode = ESelfTestMode::eCrossOver;
   long m_lUARTDataReceiveSleep = 100; // ms

   std::mutex m_oWritingMutex;
};

#endif // OUARTMODESMANAGER_H