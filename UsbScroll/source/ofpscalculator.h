#pragma once

#include <chrono>
#include <mutex>

class OFPSCalculator
{
public:
   OFPSCalculator() = default;
   OFPSCalculator& operator= (const OFPSCalculator&) = delete;
   void Init()
   {
      std::lock_guard oLock(m_oFPSMutex);
      m_dFPS = 0.;
      m_oFPSAccumulateStartTime = std::chrono::high_resolution_clock::now();
      m_iCounter = 0;
   }

   void Next()
   {
      m_oCounterMutex.lock();
      ++m_iCounter;
      m_oCounterMutex.unlock();
      if (m_iCounter >= m_iLimitCount)
      {
         auto End = std::chrono::high_resolution_clock::now();
         if (End!=m_oFPSAccumulateStartTime)
         {
            std::lock_guard oLock(m_oFPSMutex);
            try
            {
               m_dFPS = static_cast<double>(m_iCounter)/std::chrono::duration<double, std::chrono::seconds::period>(End-m_oFPSAccumulateStartTime).count();
            }
            catch(const std::exception& e)
            {
               std::string sTmp = e.what();
            }
         }
         m_oFPSAccumulateStartTime = End;

         m_oCounterMutex.lock();
         m_iCounter = 0;
         m_oCounterMutex.unlock();
      }
   }
   
   double GetFPS()
   {
      std::lock_guard oLock(m_oFPSMutex);
      return m_dFPS;
   }
private:
   std::mutex m_oFPSMutex;
   std::mutex m_oCounterMutex;
   std::chrono::time_point<std::chrono::high_resolution_clock> m_oFPSAccumulateStartTime = std::chrono::high_resolution_clock::now();
   double m_dFPS = 0.;
   int m_iCounter = 0;
   static constexpr int m_iLimitCount = 30;
};
