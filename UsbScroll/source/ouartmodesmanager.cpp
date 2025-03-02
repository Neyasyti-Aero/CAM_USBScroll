
#include "ouartmodesmanager.h"
#include "ocameradatamanager.h"
#include "ouartparams.h"

#include <fstream>
#include <thread>
#include <iostream>
#include <array>
#include <memory>
#ifdef _WIN32
   #include <filesystem>
   namespace usb_filesystem = std::filesystem;
#else
   #include <experimental/filesystem>
   namespace usb_filesystem = std::experimental::filesystem;
#endif

#include <wx/filedlg.h>
#include <wx/dirdlg.h>

OUARTModesManager::OUARTModesManager()
{

}

OUARTModesManager::~OUARTModesManager()
{
   Stop();
}

void OUARTModesManager::SelectParams(wxWindow* poParent_p)
{
   bool bExecuted = m_oExecuteThread;
   if (m_oExecuteThread)
   {
      Stop();
   }

   OUARTParams oLogParams(poParent_p, m_oFilePath, static_cast<int>(m_lUARTDataReceiveSleep), m_eSelfTestMode);
   if (oLogParams.ShowModal() == wxID_OK)
   {
      m_oFilePath = oLogParams.GetLogFilePath();
      m_lUARTDataReceiveSleep = oLogParams.GetTimeInterval();
      m_eSelfTestMode = oLogParams.GetSelfTestMode();
   }

   if (bExecuted)
   {
      Start();
   }
}

void OUARTModesManager::SetMode(EMode eMode_p)
{
   if (m_eMode == eMode_p)
   {
      return;
   }

   if (m_eMode != EMode::eNone)
   {
      Stop();
   }

   m_eMode = eMode_p;

   Start();
}

void OUARTModesManager::Start()
{
   if (m_eMode == EMode::eNone)
   {
      return;
   }

   if (CamM.GetCurrentCameraName().empty())
   {
      ReportAborted();
   }
   else
   {
      m_oExecuteThread = true;

      std::function<void(void)> oWorkerFunction;
      switch (m_eMode)
      {
      case EMode::eLogging:
         oWorkerFunction = [this](){DataReceiving();};
         break;
      case EMode::eSelfTest:
         oWorkerFunction = [this](){SelfTest();};
         break;
      }

      std::thread oWorker(oWorkerFunction);
      oWorker.detach();
   }
}

void OUARTModesManager::Stop()
{
   if (m_oExecuteThread)
   {
      m_oExecuteThread = false;
      std::lock_guard oLockThreadFinished(m_oWorkerThreadFinishedMutex);
   }
}

void OUARTModesManager::SelfTest()
{
   std::lock_guard oLockThreadFinished(m_oWorkerThreadFinishedMutex);

   std::vector<unsigned char> oSendBufferU1 = 
      {'U','A','R','T','1',9,8,7,6,5,4,3,2,1,0,1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1,0,'\n'};
   std::vector<unsigned char> oSendBufferU2 = 
      {'U','A','R','T','2',0,1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1,0,9,8,7,6,5,4,3,2,1,'\n'};
   std::vector<unsigned char> oReceiveBufferU1(oSendBufferU1.size(),0);
   std::vector<unsigned char> oReceiveBufferU2(oSendBufferU2.size(),0);

   std::array<std::pair<std::vector<unsigned char>*, std::vector<unsigned char>*>, 2> oCompare;
   if (m_eSelfTestMode == ESelfTestMode::eCrossOver)
   {
      oCompare[0] = std::make_pair(&oSendBufferU1, &oReceiveBufferU2);
      oCompare[1] = std::make_pair(&oSendBufferU2, &oReceiveBufferU1);
   } 
   else if (m_eSelfTestMode == ESelfTestMode::eShorting)
   {
      oCompare[0] = std::make_pair(&oSendBufferU1, &oReceiveBufferU1);
      oCompare[1] = std::make_pair(&oSendBufferU2, &oReceiveBufferU2);
   } 

   const auto oCameraName = CamM.GetCurrentCameraName();

   std::string oMessage;
   auto AddMessage = [&oMessage](const std::string& roAddon_p)
   {
      if (!oMessage.empty() && !roAddon_p.empty())
      {
         oMessage += ", ";
      }
      oMessage += roAddon_p;
   };

   while (m_oExecuteThread)
   {
      CamM.ClearUARTBuffer(OCameraDataManager::EUART::eUART1);
      CamM.ClearUARTBuffer(OCameraDataManager::EUART::eUART2);
      oMessage.clear();

      if (!OCameraDataManager::GetInstance().SendUARTBuffer(OCameraDataManager::EUART::eUART1, oSendBufferU1))
      {
         AddMessage("UART1 send error");
      }
      if (!OCameraDataManager::GetInstance().SendUARTBuffer(OCameraDataManager::EUART::eUART2, oSendBufferU2))
      {
         AddMessage("UART2 send error");
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(m_lUARTDataReceiveSleep));

      if (!OCameraDataManager::GetInstance().ReceiveUARTBuffer(OCameraDataManager::EUART::eUART1, oReceiveBufferU1))
      {
         AddMessage("UART1 receive overflow");
      }
      if (!OCameraDataManager::GetInstance().ReceiveUARTBuffer(OCameraDataManager::EUART::eUART2, oReceiveBufferU2))
      {
         AddMessage("UART2 receive overflow");
      }

      AddMessage(*oCompare[0].first == *oCompare[0].second ? "UART1 Ok" : "UART1 Mismatch");
      AddMessage(*oCompare[1].first == *oCompare[1].second ? "UART2 Ok" : "UART2 Mismatch");

      CamM.GetLogReceiver().AddMessage(oMessage);

      if (oCameraName != CamM.GetCurrentCameraName())
      {
         CamM.GetLogReceiver().AddMessage("Camera switched, UART operations stopped");
         m_oExecuteThread = false;
         ReportAborted();
      }
   }
}

void OUARTModesManager::DataReceiving()
{
   std::lock_guard oLockThreadFinished(m_oWorkerThreadFinishedMutex);

   usb_filesystem::path oPath(m_oFilePath.ToStdWstring());
   std::ofstream oOut(oPath, std::ios_base::out | std::ios_base::trunc);

   if (oOut.good())
   {
      const auto oCameraName = CamM.GetCurrentCameraName();
      CamM.ClearUARTBuffer(OCameraDataManager::EUART::eUART2);
      static constexpr unsigned long lBufferOptimumSize = 1024;
      std::array<std::vector<unsigned char>, 2> oBuffers;
      std::vector<unsigned char> oReceiveBuffer;
      int iActiveBuffer = 0;
      while(m_oExecuteThread)
      {
         if (!OCameraDataManager::GetInstance().ReceiveUARTBuffer(OCameraDataManager::EUART::eUART2, oReceiveBuffer))
         {
            CamM.GetLogReceiver().AddEvent(ILogMessagesReceiver::EEvents::eUARTOverflow);
         }
         if (!oReceiveBuffer.empty())
         {
            std::copy(oReceiveBuffer.begin(), oReceiveBuffer.end(),
              std::back_inserter(oBuffers[iActiveBuffer]));

            if (oBuffers[iActiveBuffer].size() > lBufferOptimumSize)
            {
               std::unique_lock<std::mutex> oWritingLock(m_oWritingMutex, std::defer_lock);
               if (oWritingLock.try_lock())
               {
                  oWritingLock.unlock();
                  std::thread oWriterThread([this, &oOut, &oBuffers, iActiveBuffer]{WriteBuffer(oBuffers[iActiveBuffer], oOut);});
                  oWriterThread.detach();
                  iActiveBuffer = iActiveBuffer == 0 ? 1 : 0;
               }
            }
         }
         else
         {
            CamM.GetLogReceiver().AddEvent(ILogMessagesReceiver::EEvents::eUARTNoData);
         }

         std::this_thread::sleep_for(std::chrono::milliseconds(m_lUARTDataReceiveSleep));

         if (oCameraName != CamM.GetCurrentCameraName())
         {
            CamM.GetLogReceiver().AddMessage("Camera switched, UART log stopped");
            m_oExecuteThread = false;
            ReportAborted();
         }
      }
      m_oWritingMutex.lock(); // barrier to wait of independent thread writing finishing
      m_oWritingMutex.unlock();
      if (!oBuffers[iActiveBuffer].empty())
      {
         WriteBuffer(oBuffers[iActiveBuffer], oOut);
      }
      oOut.close();
   }
   else
   {
      CamM.GetLogReceiver().AddMessage("Unable to open UART log file for writing");
      m_oExecuteThread = false;
      ReportAborted();
   }
}

void OUARTModesManager::WriteBuffer(std::vector<unsigned char>& roBuffer_p, std::ofstream& roOut_p)
{
   std::lock_guard oLock(m_oWritingMutex);
   roOut_p.write(reinterpret_cast<char*>(roBuffer_p.data()), roBuffer_p.size()*sizeof(unsigned char));
   roOut_p.flush();
   roBuffer_p.clear();
}

void OUARTModesManager::SetLogAbortedReporting(std::function<void()> poAbortReporter_p)
{
   m_poAbortReporter = poAbortReporter_p;
}

void OUARTModesManager::ReportAborted()
{
   m_eMode = EMode::eNone;
   CamM.GetLogReceiver().AddMessage("UART Mode has been switched to Off");
   if (m_poAbortReporter)
   {
      m_poAbortReporter();
   }
}