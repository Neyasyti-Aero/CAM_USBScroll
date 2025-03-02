
#pragma once

#include <variant>
#include <string>

class ILogMessagesReceiver
{
public:
   enum class EEvents
   {
      eStartStream,
      eStopStream,
      eMissedFrame,
      eFrameHeaderIssue,
      eCorruptFrameInfo,
      eStreamSynchroIssue,
      eUARTOverflow,
      eUARTNoData
   };
   using event_data_t = std::variant<std::monostate, int>;
public:
   virtual void AddEvent(EEvents eEvent, const event_data_t& roData = std::monostate()) = 0;
   virtual void AddMessage(const std::string& roMessage) = 0;
};

class ONoLog : public ILogMessagesReceiver
{
public:
   static ONoLog& Get()
   {
      static ONoLog oLog;
      return oLog;
   }
   void AddEvent(EEvents eEvent, const event_data_t& roData) override {};
   void AddMessage(const std::string& roMessage) override {};
};