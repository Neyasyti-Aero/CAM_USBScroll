///@file ocameradatamanager.h

#pragma once

#include "eseusbcam.h"
#include "ofpscalculator.h"
#include "ilogmessagesreceiver.h"
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <variant>
#include <deque>
#include "wx/bitmap.h"
#include "wx/rawbmp.h"

class INewFrameObserver
{
public:
   enum class ETransferState
   {
      eNormal,
      eStreamSynchronizationIssue,
      eHeaderIssue,
      eFrameInfoIssue
   };

   virtual void ProcessNewFrame() = 0;
   virtual void PrepareForStreamStart(eseusbcam_long_t FrameSizeX, eseusbcam_long_t FrameSizeY) = 0;
};

class IStreamEventsObserver
{
public:
   enum class ERquestType
   {
      eSimple,
      eError
   };
public:
   virtual void StopStreamOnRequest(ERquestType eType_p, std::string&& Message) = 0;
};

class OCameraDataManager
{
public:
   
   enum class EFeature
   {
      eBrightness,
      eContrast,
      eSharpness,
      eWhiteBalance,
      eRed,
      eGreen,
      eBlue,
      eSaturation,
      eGamma,
      eGain,
      eIris,
      eFocus,
      eTemperature,
      eZoom,
      ePan,
      eTilt,
      eOpticalFilter,
      eCompressionLevel,
      eAutoLevel,
      eAutoPeakAverage,
      eShutter,
      eFrameExposure,
      eIrisInqOffset,
      eBits,
      eLeftFramePos,
      eTopFramePos,
      eWidthFramePos,
      eHeightFramePos,
      eTrigger,
      eOneMultiFrameRequest,
      eSignalsControlReg,
      eDSTSyncOut0,
      eDSTSyncOut1,
      eDSTSyncOut2,
      eDSTSyncOut3,
      eETSyncOut0,
      eETSyncOut1,
      eETSyncOut2,
      eETSyncOut3,
      eSNTPTimerPeriod,
      eFormatOps
   };

   enum class EUART
   {
      eUART1,
      eUART2
   };

private:
   static constexpr bool m_bAllowStreamRestartAfterParamsChange = true;
   struct FrameScanArea
   {
      void Fill_From_IP_FORMATS_LIST(const IP_FORMATS_LIST &rOne)
      {
         if (rOne.bPartial)
         {
            StepWidth = rOne.StepWidth != 0 ? rOne.StepWidth : 1;
            StepHeight = rOne.StepHeight != 0 ? rOne.StepHeight : 1;
            MaxWidth = rOne.MaxWidth / StepWidth;
            MaxHeight = rOne.MaxHeight / StepHeight;
            MinWidth = rOne.MinWidth / StepWidth;
            MinHeight = rOne.MinHeight / StepHeight;
            WidthFrame = rOne.WidthFrame / StepWidth;
            HeightFrame = rOne.HeightFrame / StepHeight;
            LeftFrame = rOne.LeftFrame / StepWidth;
            TopFrame = rOne.TopFrame / StepHeight;
         }
         else
         {
            MaxWidth = rOne.MaxWidth;
            MaxHeight = rOne.MaxHeight;
            MinWidth = rOne.MinWidth;
            MinHeight = rOne.MinHeight;
            WidthFrame = MaxWidth;
            HeightFrame = MaxHeight;
            LeftFrame = 0;
            TopFrame = 0;
         }
      }
      void SetWidth(unsigned short NewWidth)
      {
         WidthFrame = NewWidth;
         if (WidthFrame + LeftFrame > MaxWidth)
         {
            LeftFrame = MaxWidth - WidthFrame;
         }
      }
      void SetHeight(unsigned short NewHeight)
      {
         HeightFrame = NewHeight;
         if (HeightFrame + TopFrame > MaxHeight)
         {
            TopFrame = MaxHeight - HeightFrame;
         }
      }
      void Set_To_IP_FORMATS_LIST(IP_FORMATS_LIST &rOne) const
      {
         if (rOne.bPartial)
         {
            rOne.WidthFrame = ConvertPartialWidthToFull(WidthFrame);
            rOne.HeightFrame = ConvertPartialHeightToFull(HeightFrame);
            rOne.LeftFrame = ConvertPartialWidthToFull(LeftFrame);
            rOne.TopFrame = ConvertPartialHeightToFull(TopFrame);
         }
      }
      unsigned short ConvertPartialWidthToFull(unsigned short Width) const
      {
         return Width * StepWidth;
      }
      unsigned short ConvertPartialHeightToFull(unsigned short Height) const
      {
         return Height * StepHeight;
      }
      FrameScanArea() = default;
      FrameScanArea &operator=(const FrameScanArea &) = default;
      unsigned short WidthFrame = 0;  //текущая ширина изображения (в пикселах)
      unsigned short HeightFrame = 0; //текущая высота изображения (в пикселах)
      unsigned short LeftFrame = 0;   //левый верхний угол запрашиваемой части изображения (в пикселах)
      unsigned short TopFrame = 0;    //левый верхний угол запрашиваемой части изображения (в пикселах)
      unsigned short StepWidth = 1;
      unsigned short StepHeight = 1;
      unsigned short MaxWidth = 1;
      unsigned short MaxHeight = 1;
      unsigned short MinWidth = 1;
      unsigned short MinHeight = 1;
   };
   
   struct CameraImageOptions
   {
      void FillAvailability(const FORMAT_OPS& Ops)
      {
         MirrorAvailable = Ops.PresenceMirror;
         FlipAvailable   = Ops.PresenceFlip;
         FDGainAvailable = Ops.PresenceFDG;
      }
      void FillValues(eseusbcam_unsigned_long_t oVal)
      {
         Mirror = oVal & FLAG_IMAGE_OPS_MIRROR; // : 1
         Flip   = oVal & FLAG_IMAGE_OPS_FLIP;   // : 1
         FDGain = oVal & FLAG_IMAGE_OPS_FDGAIN; // : 1
      }
      eseusbcam_unsigned_long_t FlattenValues() const
      {
         eseusbcam_unsigned_long_t oVal = 0;
         if (Mirror)
         {
            oVal |= FLAG_IMAGE_OPS_MIRROR; // : 1
         }
         if (Flip)
         {
            oVal |= FLAG_IMAGE_OPS_FLIP; // : 1
         }
         if (FDGain)
         {
            oVal |= FLAG_IMAGE_OPS_FDGAIN; // : 1
         }
         return oVal;
      }
      bool MirrorAvailable = false;
      bool Mirror = false;
      bool FlipAvailable = false;
      bool Flip = false;
      bool FDGainAvailable = false;
      bool FDGain = false;
   };
   using feature_t = std::variant<const FEATURE_ELEMENTS_REG *, const SHUTTER_INQ_REG *, const TRIGGER_INQ_REG *>;

public:
   using service_data_t = std::variant<std::monostate, STREAM_SERV_DATA_TYPE_1, STREAM_SERV_DATA_TYPE_2, STREAM_SERV_DATA_ITK4_0, STREAM_SERV_DATA_BRZ_5012HM_GE_V1>;

   struct FrameData
   {
      void Init(unsigned short Width, unsigned short Height, const std::deque<INewFrameObserver*>& roObservers)
      {
         m_oBitmapFrameBuffer.Create(Width, Height);
         wxNativePixelData oBitmapData = GetNativeFrameData();
         m_ReceivedFramesCount = 0;
         m_ErrorFramesCount = 0;
         m_PreviousFrameNumber = 0;
        
         m_RetValidNum = 0;
         m_RetDamagedNum = 0;
         m_RetlastPacketId = 0;
         m_RetcntShuffledPacks = 0;
         m_RetcntLostPacks = 0;
         m_RadarErrorRet = 0;

         for (auto& roObserver : roObservers)
         {
            roObserver->PrepareForStreamStart(Width, Height);
         }
      }
      const wxBitmap& GetFrameBuffer() const
      {
         return m_oBitmapFrameBuffer;
      }
      wxNativePixelData GetNativeFrameData()
      {
         return wxNativePixelData(m_oBitmapFrameBuffer);
      }
      std::shared_mutex& Access_Mutex()
      {
         return m_oMutex;
      }
      std::unique_lock<std::shared_mutex>& Access_Unique_Lock()
      {
         return m_oLock;
      }
      service_data_t m_oServiceData;
      eseusbcam_unsigned_long_t m_ReceivedFramesCount = 0;
      eseusbcam_unsigned_long_t m_ErrorFramesCount = 0;
      eseusbcam_unsigned_long_t m_PreviousFrameNumber = 0;

      eseusbcam_unsigned_long_t m_RetValidNum = 0;
      eseusbcam_unsigned_long_t m_RetDamagedNum = 0;
      eseusbcam_unsigned_long_t m_RetlastPacketId = 0;
      eseusbcam_unsigned_long_t m_RetcntShuffledPacks = 0;
      eseusbcam_unsigned_long_t m_RetcntLostPacks = 0;
      eseusbcam_unsigned_long_t m_RadarErrorRet = 0;
      INewFrameObserver::ETransferState m_eState = INewFrameObserver::ETransferState::eNormal;

      static bool Swap(std::unique_ptr<FrameData>& poReceiver_p, std::unique_ptr<FrameData>& poDraw_p)
      {
         if (poDraw_p->Access_Unique_Lock().try_lock())
         {
            poReceiver_p.swap(poDraw_p);

            poReceiver_p->m_ReceivedFramesCount = poDraw_p->m_ReceivedFramesCount;
            poReceiver_p->m_ErrorFramesCount = poDraw_p->m_ErrorFramesCount;
            poReceiver_p->m_PreviousFrameNumber = poDraw_p->m_PreviousFrameNumber;

            poReceiver_p->m_RetValidNum = poDraw_p->m_RetValidNum;
            poReceiver_p->m_RetDamagedNum = poDraw_p->m_RetDamagedNum;
            poReceiver_p->m_RetlastPacketId = poDraw_p->m_RetlastPacketId;
            poReceiver_p->m_RetcntShuffledPacks = poDraw_p->m_RetcntShuffledPacks;
            poReceiver_p->m_RetcntLostPacks = poDraw_p->m_RetcntLostPacks;
            poReceiver_p->m_RadarErrorRet = poDraw_p->m_RadarErrorRet;
            poReceiver_p->m_eState = poDraw_p->m_eState;

            poReceiver_p->Access_Unique_Lock().unlock();

            return true;
         }
         return false;
      } 
   private:
      wxBitmap m_oBitmapFrameBuffer;
      std::shared_mutex m_oMutex;
      std::unique_lock<std::shared_mutex> m_oLock = std::unique_lock<std::shared_mutex>(m_oMutex, std::defer_lock);
   };

private:
   OCameraDataManager() = default;
   ~OCameraDataManager();

public:
   static OCameraDataManager &GetInstance();

   void LoadCamerasList();
   void SetLoadCamerasListAct(const std::function<void(const std::string&)>& roAct_p);
   const std::string &GetCurrentCameraName() const;
   const std::vector<std::string> &GetCameraNames() const;
   void SetActiveCamera(const std::string &rCamName);

   std::string GetCurrentCameraModelName() const;
   std::string GetCurrentCameraSerial() const;

   bool GetFeaturePresence(EFeature eFeature_p) const;
   std::pair<eseusbcam_unsigned_long_t, eseusbcam_unsigned_long_t> GetFeatureRange(EFeature eFeature_p) const;
   eseusbcam_unsigned_long_t GetFeatureValue(EFeature eFeature_p) const;
   bool GetFeatureAuto(EFeature eFeature_p) const;
   static std::string GetFeatureName(EFeature eID);
   static bool IsAutoSupported(EFeature eID);
   bool SetFeatureValue(EFeature eFeature_p, eseusbcam_unsigned_long_t lVal_p);
   void SetFeatureAuto(EFeature eFeature_p, bool bOn);
   std::list<std::string> GetFormatNames() const;
   eseusbcam_unsigned_long_t GetCurrentFormat() const;
   bool SetCurrentFormat(eseusbcam_unsigned_long_t lFormatIndex);
   bool SetScanArea();

   void ClearUARTBuffer(EUART eUart_p);
   // true - ok, false - overflow
   bool ReceiveUARTBuffer(EUART eUart_p, std::vector<unsigned char>& roBuffer_p);
   // true - ok, false - issue
   bool SendUARTBuffer(EUART eUart_p, const std::vector<unsigned char>& roBuffer_p);
   
   const TRIGGER_CTRL_REG &GetTriggerMode() const;
   bool SetTriggerModeValue(const TRIGGER_CTRL_REG &rTCR);
   
   const CameraImageOptions& GetCameraImageFormatOptions() const;
   bool SetCameraImageFormatOptions(const CameraImageOptions& rFO);

   bool SetEnableDelayTiming(eseusbcam_unsigned_long_t OutIndex, bool Enable);

   bool SendLutTable(std::vector<unsigned char>& roTable_p) const;
   bool ReceiveLutTable(std::vector<unsigned char>& roTable_p) const;

   unsigned short ConvertPartialWidthToFull(unsigned short Width) const;
   unsigned short ConvertPartialHeightToFull(unsigned short Height) const;

   eseusbcam_unsigned_long_t ConvertShutterToDirect(eseusbcam_unsigned_long_t Shutter) const;
   eseusbcam_unsigned_long_t ConvertShutterFromDirect(eseusbcam_unsigned_long_t Shutter) const;

   unsigned short GetStream0Port() const;
   unsigned short GetStream0PacketSize() const;
   bool SetStream0Params(unsigned short usPort, unsigned short PacketSize);
   bool SaveSettings(eseusbcam_unsigned_long_t lPreset = 1);

   void AddNewFrameObserver(INewFrameObserver* poFrameObserver);
   void SetStreamErrorObserver(IStreamEventsObserver *poSteramErrorObserver);

   bool StartVideoStream();
   bool StopVideoStream();
   void StopVideoStreamOnError(std::string&& Message);
   bool IsStreamWorked();

   bool CheckFrameProcessingIsFinished(bool bBlocking_p = true);
   void ProcessNewFrame(unsigned char* pFrame);
   const FrameData& GetFrameData() const;
   std::unique_ptr<std::shared_lock<std::shared_mutex>> SharedLockFrameData();

   void UpdateFPS();
   double GetFPS();

   void SetFramesAreRequired(bool bKey);
   bool GetFramesAreRequired();

   void SetFlipImageH(bool bKey);
   void SetFlipImageV(bool bKey);
   bool GetFlipImageH();
   bool GetFlipImageV();

   void SetDecode(bool bKey);
   bool GetDecode();

   void SetLogReceiver(ILogMessagesReceiver& poLog);
   ILogMessagesReceiver& GetLogReceiver();
private:
   void UpdateCameraCapabilities();
   feature_t GetFeature(EFeature eFeature_p) const;
   static eseusbcam_unsigned_long_t GetFeatureID(EFeature eFeature_p);
   void FillFrameAreaFromCurrentFormat();
   void ReceiveTriggerModeValue();
   void ReceiveCameraImageOpsValue();

private:
   bool m_bInitialRequestWasDone = false;
   std::function<void(const std::string&)> m_oCamerasListUpdatedAct;

   std::string m_oCurrentCameraName;
   std::vector<std::string> m_oCamNames;
   USB_CAMERA_CAPABILITES m_oCurrentCameraParam = {};
   FrameScanArea m_oCurrentCameraScanArea;
   TRIGGER_CTRL_REG m_oCurrentCameraTrigReg = {};
   eseusbcam_unsigned_long_t m_CurrentCameraSignalsControlReg = 0;
   CameraImageOptions m_oCurrentCameraImageOps;

   bool m_bStreamWorked = false;

   std::deque<INewFrameObserver*> m_oFrameObservers;
   IStreamEventsObserver *m_poSteramEventsObserver = nullptr;
   std::mutex m_oFrameProcessingMutex;
   std::pair<std::unique_ptr<FrameData>, std::unique_ptr<FrameData>> m_oFrameData = 
      std::make_pair(std::make_unique<FrameData>(), std::make_unique<FrameData>());
   OFPSCalculator m_oFramesReceivingFPS;

   std::mutex m_oFramesAreRequiredMutex;
   bool m_oFramesAreRequired = true;

   std::mutex m_oFlipMutex;
   std::pair<bool, bool> m_oFlip = std::make_pair(false, false);
   bool m_bDecode = true;

   ILogMessagesReceiver* m_poLogReceiver = static_cast<ILogMessagesReceiver*>(&ONoLog::Get());
};

#define CamM OCameraDataManager::GetInstance()