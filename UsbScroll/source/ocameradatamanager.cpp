///@file ocameradatamanager.cpp

#include "ocameradatamanager.h"
#include <cassert>
#include <algorithm>
#include <thread>
#include <sstream>
#include <array>

namespace
{
   void CameraCallback (const char* szCameraName,
								int EventID,
								int EventDataSize,
								void* pEventData,
                        unsigned char* pFrame,
								void*  pUserData)
   {
      OCameraDataManager& roManager = OCameraDataManager::GetInstance();
      try
      {
         if (EventID == NEW_FRAME ||
             EventID == NEW_FRAME_SYNCHRO_ERROR)
         {
            roManager.UpdateFPS();
            
            if (roManager.CheckFrameProcessingIsFinished(false))
            {
               roManager.ProcessNewFrame(pFrame);
            }
         }
         else
         {
            std::string Message;
            if (EventDataSize != 0 && pEventData)
            {
               Message.insert(0, static_cast<char*>(pEventData), EventDataSize);
            }
            roManager.CheckFrameProcessingIsFinished();
            roManager.GetLogReceiver().AddMessage(Message);
            std::thread oProcessingThread(&OCameraDataManager::StopVideoStreamOnError, &roManager, std::move(Message));
            oProcessingThread.detach();
         }
      }
      catch(...)
      {
      }
   }

   struct radar_packet_header
   {
      unsigned char ucPrefix = 0;
      unsigned char ucAddress = 0;
      unsigned char ucCmd = 0;
      unsigned char ucLen = 0;
      unsigned short FirstData = 0;
   };

   unsigned char repairByte(const unsigned char* ptmp_ptr)
   {
      if (ptmp_ptr[0] == 0x21)
      {
         switch(ptmp_ptr[1])
         {
         case 0xDE:
            return 0x21;
            break;
         case 0x23:
            return 0xDC;
            break;
         case 0x24:
            return 0xDB;
            break;
         case 0x20:
            return 0xDF;
            break;
         }
      }
      return ptmp_ptr[0];
   }
   //функция поиска и верификации радарных пакетов в буфере; 0 - выполнено без ошибок, >0 - код ошибки
   eseusbcam_unsigned_long_t RadarBufferCheck(const unsigned char* pBuff, 
      eseusbcam_unsigned_long_t ulBuffSize,
      OCameraDataManager::FrameData& roFrameData)
   {
      //if (IsBadReadPtr(pBuff, ulBuffSize)) return 1;

      roFrameData.m_RetValidNum = 0;
      roFrameData.m_RetDamagedNum = 0;
      for (unsigned long ulCnt = 0; ulCnt < ulBuffSize;)
      {
         auto ptr = pBuff + ulCnt;

         if ((ptr[0] == 0xDB) && (ptr[1] == 0x22) && (ptr[2] == 0x00))
         {
            int offset = 4;
            unsigned char check_summ = 0;
            if (ptr[offset] == 0x21)
            {
               ++offset;
            }
            const int packetLen = 2 * static_cast<char>(repairByte(ptr + offset)) + 5;
            std::vector<unsigned char> packetsBeforeByteStuffing(packetLen, 0);
            offset = 1;

            for (int i = 0; i < packetLen; ++i, ++offset) 
            {
               packetsBeforeByteStuffing[i] = repairByte(ptr + offset);

               if (i != packetLen - 1) 
               {
                  check_summ = check_summ + packetsBeforeByteStuffing[i];
               }

               if (*(ptr + offset) == 0x21)
               {
                  ++offset;
               }
            }

            if (check_summ == packetsBeforeByteStuffing[packetLen - 1])
            {
               ++roFrameData.m_RetValidNum;
            }
            else
            {
               ++roFrameData.m_RetDamagedNum;
            }
            auto curPacketId = reinterpret_cast<const radar_packet_header*>(packetsBeforeByteStuffing.data())->FirstData;

            // init 
            if (roFrameData.m_RetlastPacketId == 0)
            {
               roFrameData.m_RetlastPacketId = curPacketId - 1;
            }

            // lost
            if (curPacketId > roFrameData.m_RetlastPacketId && curPacketId != roFrameData.m_RetlastPacketId + 1)
            {
               roFrameData.m_RetcntLostPacks += std::min(curPacketId - roFrameData.m_RetlastPacketId - 1, eseusbcam_unsigned_long_t(1000));
            }

            // shuffled
            if ((curPacketId < roFrameData.m_RetlastPacketId) && (curPacketId != 0))
            {
               ++roFrameData.m_RetcntShuffledPacks;
            }

            // continue search next packet
            roFrameData.m_RetlastPacketId = curPacketId;
            ulCnt += packetLen;
         }
         else
         {
            // if we didn't find start of a packet - try again
            ++ulCnt;
         }
      }
      return 0;
   }

   struct Recode12BitTo8BitTable
   {
      static constexpr int array_size = 4096;
      Recode12BitTo8BitTable()
      {
         double dGamma = 100.;
         double dArgument = (static_cast<double>(array_size) + dGamma) / dGamma;
         double dMCoeff = 255. / (std::log(dArgument));
         Data[0] = 0;
         for (int iIndex = 1; iIndex < array_size; ++iIndex)
         {
            Data[iIndex] = static_cast<unsigned char>(dMCoeff * std::log((static_cast<double>(iIndex) + dGamma) / dGamma));
         }
      };
      std::array<unsigned char, array_size> Data = {};
   };

   void Mono12bitToMono8bit(wxNativePixelData rOutputBuffer, const unsigned char* InputBuffer, int Width, int Height, bool bMirrorX, bool bMirrorY)
   {
      assert(Height % 2 == 0 && Width % 2 == 0);
      static Recode12BitTo8BitTable RecodeTable;
      const int iStride = (Width / 2) * 3;
#pragma omp parallel
      {
         wxNativePixelData::Iterator oOutputIter(rOutputBuffer);
#pragma omp for schedule(dynamic, 10)
         for (eseusbcam_long_t HCount = 0; HCount < Height; ++HCount)
         {
            oOutputIter.MoveTo(rOutputBuffer, 0, HCount);
            const unsigned char* pTmpSource = InputBuffer + HCount*iStride;
            for (eseusbcam_long_t WCount2 = 0; WCount2 < (Width / 2); ++WCount2)
            {
               const auto pByte0 = pTmpSource;
               const auto pByte1 = pTmpSource + 1;
               const auto pByte2 = pTmpSource + 2;

               //нелинейно
               unsigned long ulIndex = (*pByte0) * 16 + (((*pByte1) & 0xF0) >> 4);
               oOutputIter.Blue() = oOutputIter.Green() = oOutputIter.Red() = RecodeTable.Data[ulIndex];
               ++oOutputIter;

               ulIndex = ((*pByte1) & 0x0F) * 256 + *pByte2;
               oOutputIter.Blue() = oOutputIter.Green() = oOutputIter.Red() = RecodeTable.Data[ulIndex];
               ++oOutputIter;

               pTmpSource += 3;
            }
         }
      }
   }

   struct YUV420_TO_RGB_Tables
   {
      static constexpr int array_size = 256;
      static constexpr int Offset1 = -128;
      YUV420_TO_RGB_Tables()
      {
         for (int iIndex = 0; iIndex < array_size; ++iIndex)
         {
            auto iVal = iIndex + Offset1;

            B[iIndex] = (iVal * 1772) / 1000;
            R[iIndex] = (iVal * 1402) / 1000;
            GR[iIndex] = (iVal * 7141) / 10000;
            GB[iIndex] = (iVal * 3441) / 10000;
         }
      };
      std::array<int, array_size> R = {};
      std::array<int, array_size> B = {};
      std::array<int, array_size> GR = {};
      std::array<int, array_size> GB = {};
   };

   //данные уложены : 2 строки - яркостный сигнал, 1 строка - Cr,Cb,Cr,Cb .....
   void YUV420_TO_RGB(wxNativePixelData rOutputBuffer, const unsigned char *InputBuffer, int Width, int Height, bool bMirrorX, bool bMirrorY)
   {
      assert(Height%2==0 && Width%2==0);
      static YUV420_TO_RGB_Tables oTable;

#pragma omp parallel
      {
         wxNativePixelData::Iterator oOutputIter1(rOutputBuffer);
         wxNativePixelData::Iterator oOutputIter2(rOutputBuffer);
#pragma omp for schedule(dynamic, 10)
         for (eseusbcam_long_t H2count = 0; H2count < Height/2; ++H2count)
         {
            if (bMirrorY)
            {
               oOutputIter1.MoveTo(rOutputBuffer, 0, 2*H2count);
               oOutputIter2.MoveTo(rOutputBuffer, 0, 2*H2count+1);
            }
            else
            {
               oOutputIter1.MoveTo(rOutputBuffer, 0, Height - 2*H2count - 1);
               oOutputIter2.MoveTo(rOutputBuffer, 0, Height - 2*H2count - 2);
            }

            auto pCurrInputRow1 = InputBuffer + 3*H2count*Width;
            auto pCurrInputRow2 = InputBuffer + (3*H2count+1)*Width;
            auto pCrCbLine = pCurrInputRow2 + Width;
            if (bMirrorX)
            {
               pCurrInputRow1 += Width-1;
               pCurrInputRow2 += Width-1;
               pCrCbLine += Width-2;
            }

            for (eseusbcam_unsigned_long_t Wcount = 0; Wcount < Width/2; ++Wcount)
            {

               int tempR = oTable.R[*pCrCbLine];
               int tempG = oTable.GR[*pCrCbLine];
               
               pCrCbLine++;
               int tempB =  oTable.B[*pCrCbLine];
               tempG += oTable.GB[*pCrCbLine];

               bMirrorX ? (pCrCbLine -= 3) : pCrCbLine++;

               for (eseusbcam_unsigned_long_t Pcount = 0; Pcount < 2; ++Pcount)
               {

                  int tempY = (int)(*pCurrInputRow1);
                  //BLUE
                  int x = tempY + tempB;
                  oOutputIter1.Blue() = x <= 0 ? 0 : ((x >= 255) ? 255 : static_cast<unsigned char>(x)); //B
                  //GREEN
                  x = tempY - tempG;
                  oOutputIter1.Green() = x <= 0 ? 0 : ((x >= 255) ? 255 : static_cast<unsigned char>(x)); //G
                  //RED
                  x = tempY + tempR;
                  oOutputIter1.Red() = x <= 0 ? 0 : ((x >= 255) ? 255 : static_cast<unsigned char>(x)); //R

                  tempY = (int)(*pCurrInputRow2);
                  //BLUE
                  x = tempY + tempB;
                  oOutputIter2.Blue() = x <= 0 ? 0 : ((x >= 255) ? 255 : static_cast<unsigned char>(x)); //B
                  //GREEN
                  x = tempY - tempG;
                  oOutputIter2.Green() = x <= 0 ? 0 : ((x >= 255) ? 255 : static_cast<unsigned char>(x)); //G
                  //RED
                  x = tempY + tempR;
                  oOutputIter2.Red() = x <= 0 ? 0 : ((x >= 255) ? 255 : static_cast<unsigned char>(x)); //R

                  bMirrorX ? (--pCurrInputRow1,--pCurrInputRow2) : (++pCurrInputRow1, ++pCurrInputRow2);
                  ++oOutputIter1;
                  ++oOutputIter2;
               } //for Pcount

            } //for Wcount
         } //for Hcount
      }
   } //YUV420_TO_RGB

   void FilterBayerRGB_5GREEN_F(wxNativePixelData rOutputBuffer, const unsigned char *InputBuffer, int Width, int Height/*, bool bMirrorX, bool bMirrorY*/)
   {
      wxNativePixelData::Iterator oOutputIter(rOutputBuffer);

      eseusbcam_unsigned_long_t i, j;
      eseusbcam_unsigned_long_t Wid = 2 * Width;
      eseusbcam_unsigned_long_t Widt = 3 * Width;
      eseusbcam_unsigned_long_t LastStr = (Height - 1)*Width;
      eseusbcam_unsigned_long_t OutLastStr = (Height - 1)*Width * 3;
      eseusbcam_unsigned_long_t OutWidt = Width * 6;

      eseusbcam_unsigned_long_t WidthMinus1 = Width - 1;

      const unsigned char* InputBuffPlusWidth = (InputBuffer + Width);
      const unsigned char* InputBuffMinusWidth = (InputBuffer - Width);

      const unsigned char* InputBuffPlusWidthPlus1 = (InputBuffer + Width + 1);
      const unsigned char* InputBuffPlusWidthPlus2 = (InputBuffer + Width + 2);
      const unsigned char* InputBuffMinus1 = (InputBuffer - 1);
      const unsigned char* InputBuffPlus1 = (InputBuffer + 1);
      const unsigned char* InputBuffPlus2 = (InputBuffer + 2);
      const unsigned char* InputBuffPlusJ = InputBuffer;
      const unsigned char* InputBuffPlus1PlusJ = InputBuffer;
      const unsigned char* InputBuffPlusWidthPlusJ = InputBuffer;

      const unsigned char* InputBuffPlusI = InputBuffer;
      const unsigned char* InputBuffPlusIMinus1 = InputBuffer;

      const unsigned char* InputBuffPlusWidthPlusI = InputBuffer;
      const unsigned char* InputBuffPlusWidthPlusIMinus1 = InputBuffer;

      const unsigned char* InputBuffMinusWidthPlusI = InputBuffer;
      const unsigned char* InputBuffPlusIPlusJ = InputBuffer;
      const unsigned char* InputBuffPlusIPlusJPlus1 = InputBuffer;

      const unsigned char* InputBuffPlusWidthPlusIPlusJ = InputBuffer;
      const unsigned char* InputBuffMinusWidthPlusIPlusJ = InputBuffer;

      const unsigned char* InputBuffPlusIPlusWid = InputBuffer;
      const unsigned char* InputBuffPlusIPlusWidMinus1 = InputBuffer;
      const unsigned char* InputBuffPlusIPlusWidMinus2 = InputBuffer;

      const unsigned char* InputBuffPlusIPlus1 = InputBuffer;

      const unsigned char* InputBuffPlusIPlusJPlusWid = InputBuffer;
      const unsigned char* InputBuffMinusWidthPlusIPlusJPlus1 = InputBuffer;
      const unsigned char* InputBuffPlusWidthPlusIPlusJPlus1 = InputBuffer;

      const unsigned char* InputBuffPlusLastStr = (InputBuffer + LastStr);
      const unsigned char* InputBuffPlusLastStrPlusJ = InputBuffer;
      const unsigned char* InputBuffPlusLastStrPlusJPlus1 = InputBuffer;

      const unsigned char* InputBuffPlusLastStrMinus1 = (InputBuffer + LastStr - 1);
      const unsigned char* InputBuffPlusLastStrPlus1 = (InputBuffer + LastStr + 1);
      const unsigned char* InputBuffPlusLastStrMinus2 = (InputBuffer + LastStr - 2);
      const unsigned char* InputBuffPlusLastStrMinusWidth = (InputBuffer + LastStr - Width);
      const unsigned char* InputBuffPlusLastStrMinusWidthPlus1 = (InputBuffer + LastStr - Width + 1);
      const unsigned char* InputBuffPlusLastStrMinusWidthPlus1PlusJ = InputBuffer;

      // первая строка

      // первый пиксел - красный!!!
      oOutputIter.Red() = *(InputBuffPlusWidthPlus1);
      oOutputIter.Green() = ((*(InputBuffPlus1)+*(InputBuffPlusWidth)) >> 1);
      oOutputIter.Blue() = *(InputBuffer);
      ++oOutputIter;
      // строка
      for (j = 1; j < WidthMinus1; j += 2)
      {
         InputBuffPlusJ = InputBuffer + j;
         InputBuffPlus1PlusJ = InputBuffPlus1 + j;
         InputBuffPlusWidthPlusJ = InputBuffPlusWidth + j;


         oOutputIter.Red() = *(InputBuffPlusWidthPlusJ);
         oOutputIter.Green() = (*(InputBuffPlusJ) >> 1) + ((*(InputBuffPlusWidthPlusJ - 1) + *(InputBuffPlusWidthPlusJ + 1)) >> 2);//*(InputBuffPlusJ);
         oOutputIter.Blue() = ((*(InputBuffMinus1 + j) + *(InputBuffPlus1PlusJ)) >> 1);
         ++oOutputIter;

         oOutputIter.Red() = ((*(InputBuffPlusWidthPlusJ)+*(InputBuffPlusWidthPlus2 + j)) >> 1);
         oOutputIter.Green() = (*(InputBuffPlusJ)+*(InputBuffPlus2 + j) + *(InputBuffPlusWidthPlus1 + j)) / 3;
         oOutputIter.Blue() = *(InputBuffPlus1PlusJ);
         ++oOutputIter;
      }
      // последний пиксел
      oOutputIter.Red() = *(InputBuffMinus1 + Wid);
      oOutputIter.Green() = (*(InputBuffPlusWidth - 1) + *(InputBuffMinus1 + Wid - 1)) >> 1;
      oOutputIter.Blue() = *(InputBuffPlusWidth - 2);
      ++oOutputIter;


      // основной массив
      for (i = Width; i < LastStr; i += Wid)
      {
         InputBuffPlusI = InputBuffer + i;
         InputBuffPlusIMinus1 = InputBuffPlusI - 1;
         InputBuffPlusIPlus1 = InputBuffPlusI + 1;
         InputBuffPlusIPlusWid = InputBuffPlusI + Wid;
         InputBuffPlusIPlusWidMinus1 = InputBuffPlusIPlusWid - 1;
         InputBuffPlusIPlusWidMinus2 = InputBuffPlusIPlusWid - 2;
         InputBuffPlusWidthPlusI = InputBuffPlusWidth + i;
         InputBuffMinusWidthPlusI = InputBuffMinusWidth + i;
         InputBuffPlusWidthPlusIMinus1 = InputBuffPlusWidthPlusI - 1;

         // первый пиксел
         oOutputIter.Red() = *(InputBuffPlusIPlus1);                                                                                 //1blue for blue
         oOutputIter.Green() = (*(InputBuffPlusI) >> 1) + ((*(InputBuffMinusWidthPlusI + 1) + *(InputBuffPlusWidthPlusI + 1)) >> 2); //1 green for blue
         oOutputIter.Blue() = ((*(InputBuffMinusWidthPlusI) + *(InputBuffPlusWidthPlusI)) >> 1);                                     //1 red for blue
         ++oOutputIter;

         // строка
         for (j = 1; j < WidthMinus1; j += 2)
         {
            InputBuffPlusIPlusJ = InputBuffPlusI + j;
            InputBuffPlusIPlusJPlus1 = InputBuffPlusIPlus1 + j;
            InputBuffPlusWidthPlusIPlusJ = InputBuffPlusWidthPlusI + j;
            InputBuffMinusWidthPlusIPlusJ = InputBuffMinusWidthPlusI + j;
            InputBuffMinusWidthPlusIPlusJPlus1 = InputBuffMinusWidthPlusIPlusJ + 1;
            InputBuffPlusWidthPlusIPlusJPlus1 = InputBuffPlusWidthPlusIPlusJ + 1;

            oOutputIter.Red() = *(InputBuffPlusIPlusJ);                                                                                                                                              //blue for blue
            oOutputIter.Green() = ((*(InputBuffPlusIPlusJ - 1) + *(InputBuffPlusIPlusJPlus1) + *(InputBuffMinusWidthPlusIPlusJ) + *(InputBuffPlusWidthPlusIPlusJ)) >> 2);                            //green for blue
            oOutputIter.Blue() = ((*(InputBuffMinusWidthPlusIPlusJ - 1) + *(InputBuffMinusWidthPlusIPlusJPlus1) + *(InputBuffPlusWidthPlusIPlusJ - 1) + *(InputBuffPlusWidthPlusIPlusJPlus1)) >> 2); //red for blue
            ++oOutputIter;

            oOutputIter.Red() = ((*(InputBuffPlusIPlusJ) + *(InputBuffPlusIPlusJ + 2)) >> 1);                                                                                                                                    //blue for green in blue
            oOutputIter.Green() = (*(InputBuffPlusIPlusJPlus1) >> 1) + ((*(InputBuffMinusWidthPlusIPlusJ) + *(InputBuffPlusWidthPlusIPlusJ) + *(InputBuffMinusWidthPlusIPlusJ + 2) + *(InputBuffPlusWidthPlusIPlusJ + 2)) >> 3); //green for green in blue
            oOutputIter.Blue() = ((*(InputBuffMinusWidthPlusIPlusJPlus1) + *(InputBuffPlusWidthPlusIPlusJPlus1)) >> 1);                                                                                                          //red for green in blue
            ++oOutputIter;
         }

         oOutputIter.Red() = *(InputBuffPlusWidthPlusIMinus1);                                                                  //last blue for blue
         oOutputIter.Green() = (*(InputBuffPlusIMinus1) + *(InputBuffPlusWidthPlusI - 2) + *(InputBuffPlusIPlusWidMinus1)) / 3; //last green for blue
         oOutputIter.Blue() = ((*(InputBuffPlusI - 2) + *(InputBuffPlusIPlusWidMinus2)) >> 1);                                  //last red for blue
         ++oOutputIter;

         oOutputIter.Red() = ((*(InputBuffPlusIPlus1) + *(InputBuffPlusIPlusWid + 1)) >> 1);                        //1 blue for red
         oOutputIter.Green() = (*(InputBuffPlusI) + *(InputBuffPlusIPlusWid) + *(InputBuffPlusWidthPlusI + 1)) / 3; //1 green for red
         oOutputIter.Blue() = *(InputBuffPlusWidthPlusI);                                                           //1 red for red
         ++oOutputIter;

         for (j = 1; j < WidthMinus1; j += 2)
         {
            InputBuffPlusIPlusJ = InputBuffPlusI + j;
            InputBuffPlusIPlusJPlusWid = InputBuffPlusIPlusWid + j;
            InputBuffPlusWidthPlusIPlusJ = InputBuffPlusWidthPlusI + j;
            InputBuffPlusWidthPlusIPlusJPlus1 = InputBuffPlusWidthPlusIPlusJ + 1;

            oOutputIter.Red() = ((*(InputBuffPlusIPlusJ) + *(InputBuffPlusIPlusJPlusWid)) >> 1);                                                                                                                     //blue for green in red
            oOutputIter.Green() = (*(InputBuffPlusWidthPlusIPlusJ) >> 1) + ((*(InputBuffPlusIPlusJ + 1) + *(InputBuffPlusIPlusJPlusWid + 1) + *(InputBuffPlusIPlusJ - 1) + *(InputBuffPlusIPlusJPlusWid - 1)) >> 3); //green for green in red
            oOutputIter.Blue() = ((*(InputBuffPlusWidthPlusIPlusJ - 1) + *(InputBuffPlusWidthPlusIPlusJPlus1)) >> 1);                                                                                                //red for green in red
            ++oOutputIter;

            oOutputIter.Red() = ((*(InputBuffPlusIPlusJ) + *(InputBuffPlusIPlusJ + 2) + *(InputBuffPlusIPlusJPlusWid) + *(InputBuffPlusIPlusJPlusWid + 2)) >> 2);                  //blue for red
            oOutputIter.Green() = ((*(InputBuffPlusWidthPlusIPlusJ) + *(InputBuffPlusWidthPlusIPlusJ + 2) + *(InputBuffPlusIPlusJ + 1) + *(InputBuffPlusIPlusJPlusWid + 1)) >> 2); //green for red
            oOutputIter.Blue() = *(InputBuffPlusWidthPlusIPlusJPlus1);                                                                                                             //red for red
            ++oOutputIter;
         }

         oOutputIter.Red() = ((*(InputBuffPlusWidthPlusIMinus1) + *(InputBuffPlusIMinus1 + Widt)) >> 1);                                        //last blue for green in red
         oOutputIter.Green() = (*(InputBuffPlusIPlusWidMinus1) >> 1) + ((*(InputBuffPlusIPlusJ + 1) + *(InputBuffPlusIPlusJPlusWid + 1)) >> 2); //last green for green in red
         oOutputIter.Blue() = *(InputBuffPlusIPlusWidMinus2);                                                                                   //last red for green in red
         ++oOutputIter;
      }

      // последняя строка

      // первый пиксел
      oOutputIter.Red() = *(InputBuffPlusLastStrPlus1);
      oOutputIter.Green() = (*(InputBuffPlusLastStr) + *(InputBuffPlusLastStrMinusWidth + 1)) >> 1;
      oOutputIter.Blue() = *(InputBuffPlusLastStrMinusWidth);
      ++oOutputIter;

      // строка
      for (j = 1; j < WidthMinus1; j += 2)
      {
         InputBuffPlusLastStrPlusJPlus1 = InputBuffPlusLastStrPlus1 + j;
         InputBuffPlusLastStrPlusJ = InputBuffPlusLastStr + j;
         InputBuffPlusLastStrMinusWidthPlus1PlusJ = InputBuffPlusLastStrMinusWidthPlus1 + j;

         oOutputIter.Red() = *(InputBuffPlusLastStrPlusJ);
         oOutputIter.Green() = (*(InputBuffPlusLastStrPlusJ - 1) + *(InputBuffPlusLastStrPlusJPlus1) + *(InputBuffPlusLastStrMinusWidthPlus1PlusJ - 1)) / 3;
         oOutputIter.Blue() = ((*(InputBuffPlusLastStrMinusWidthPlus1PlusJ - 2) + *(InputBuffPlusLastStrMinusWidthPlus1PlusJ)) >> 1);
         ++oOutputIter;

         oOutputIter.Red() = ((*(InputBuffPlusLastStrPlusJ) + *(InputBuffPlusLastStrPlusJ + 2)) >> 1);
         oOutputIter.Green() = (*(InputBuffPlusLastStrPlusJPlus1) >> 1) + ((*(InputBuffPlusLastStrMinusWidthPlus1PlusJ + 1) + *(InputBuffPlusLastStrMinusWidthPlus1PlusJ - 1)) >> 2);
         oOutputIter.Blue() = *(InputBuffPlusLastStrMinusWidthPlus1PlusJ);
         ++oOutputIter;
      }
      // последний пиксел
      oOutputIter.Red() = *(InputBuffPlusLastStrMinus1 + Width);
      oOutputIter.Green() = ((*(InputBuffPlusLastStrMinus1) + *(InputBuffPlusLastStrMinus2 + Width)) >> 1);
      oOutputIter.Blue() = *(InputBuffPlusLastStrMinus2);

   }//FilterBayerRGB_5GREEN_F
}

OCameraDataManager::~OCameraDataManager()
{
   StopVideoStream();
}

OCameraDataManager& OCameraDataManager::GetInstance()
{
    static OCameraDataManager oManager;
    return oManager;
}

const std::string& OCameraDataManager::GetCurrentCameraName() const
{
    return m_oCurrentCameraName;
}

std::string OCameraDataManager::GetCurrentCameraModelName() const
{
   return m_oCurrentCameraParam.ModelName;
}

std::string OCameraDataManager::GetCurrentCameraSerial() const
{
   std::stringstream oStr;
   oStr << (m_oCurrentCameraParam.CameraSerial >> 24) << '.' 
        << ((m_oCurrentCameraParam.CameraSerial & 0xFF0000) >> 16) << '.'
        << ((m_oCurrentCameraParam.CameraSerial & 0xF000) >> 12) << '.'
        << (m_oCurrentCameraParam.CameraSerial & 0xFFF);
   return oStr.str();
}

const std::vector<std::string>& OCameraDataManager::GetCameraNames() const
{
   return m_oCamNames;
}

void OCameraDataManager::LoadCamerasList()
{
   m_bInitialRequestWasDone = true;

   bool bStreamWasRun = IsStreamWorked();
   StopVideoStream();

   DEVICE_DATA oCameraData;
   std::string sMessage;
   if (USB_GetCameraList(&oCameraData) != 1)
   {
      sMessage = std::string(USB_GetLastError());
   }
   m_oCamNames.clear();
   for (std::size_t iIndex = 0; iIndex < oCameraData.numDevices; ++iIndex)
   {
      m_oCamNames.emplace_back(oCameraData.deviceList[iIndex].DeviceName);
   }
   if (m_oCamNames.empty())
   {
      m_oCurrentCameraName.clear();
   }
   else if (m_oCurrentCameraName.empty() || std::find(m_oCamNames.begin(),m_oCamNames.end(), m_oCurrentCameraName)==m_oCamNames.end())
   {
      m_oCurrentCameraName = m_oCamNames.front();
   }
   UpdateCameraCapabilities();

   if (bStreamWasRun && !m_oCurrentCameraName.empty())
   {
      StartVideoStream();
   }

   if (m_oCamerasListUpdatedAct)
   {
      m_oCamerasListUpdatedAct(sMessage);
   }
}

void OCameraDataManager::SetLoadCamerasListAct(const std::function<void(const std::string&)>& roAct_p)
{
   m_oCamerasListUpdatedAct = roAct_p;
}
void OCameraDataManager::SetActiveCamera(const std::string& rCamName)
{
   if (m_oCurrentCameraName == rCamName)
   {
      return;
   }
   bool bStreamWasRun = IsStreamWorked();
   StopVideoStream();
   if (std::find(m_oCamNames.begin(),m_oCamNames.end(), rCamName) == m_oCamNames.end())
   {
      return;
   }
   m_oCurrentCameraName = rCamName;
   UpdateCameraCapabilities();
   if (bStreamWasRun)
   {
      StartVideoStream();
   }
}

void OCameraDataManager::UpdateCameraCapabilities()
{
   if (m_oCurrentCameraName.empty())
   {
      return;
   }
   m_oCurrentCameraParam = USB_CAMERA_CAPABILITES{};
   m_oCurrentCameraScanArea = FrameScanArea();
   m_oCurrentCameraImageOps = CameraImageOptions{};
   PUSB_CAMERA_CAPABILITES pCamCap = nullptr;
   if (USB_GetCameraCapabilites(m_oCurrentCameraName.c_str(), &pCamCap)!=0)
   {
      if (pCamCap)
      {
         m_oCurrentCameraParam =  *pCamCap;
         FillFrameAreaFromCurrentFormat();
         m_oCurrentCameraImageOps.FillAvailability(m_oCurrentCameraParam.CameraFormats.FormatOps);
      }
   }
   ReceiveTriggerModeValue();
   ReceiveCameraImageOpsValue();
   
   m_CurrentCameraSignalsControlReg = GetFeatureValue(EFeature::eSignalsControlReg);
}

OCameraDataManager::feature_t OCameraDataManager::GetFeature(EFeature eFeature_p) const
{
   switch(eFeature_p)
   {
   case EFeature::eBrightness:
      return &m_oCurrentCameraParam.Brightness;
      break;
   case EFeature::eContrast:
      return &m_oCurrentCameraParam.Contrast;
      break;
   case EFeature::eSharpness:
      return &m_oCurrentCameraParam.Sharpness;
      break;
   case EFeature::eWhiteBalance:
   case EFeature::eRed:
   case EFeature::eGreen:
   case EFeature::eBlue:
      return &m_oCurrentCameraParam.WhiteBalance;
      break;
   case EFeature::eSaturation:
      return &m_oCurrentCameraParam.Saturation;
      break;
   case EFeature::eGamma:
      return &m_oCurrentCameraParam.Gamma;
      break;
   case EFeature::eGain:
      return &m_oCurrentCameraParam.Gain;
      break;
   case EFeature::eIris:
      return &m_oCurrentCameraParam.Iris;
      break;
   case EFeature::eFocus:
      return &m_oCurrentCameraParam.Focus;
      break;
   case EFeature::eTemperature:
      return &m_oCurrentCameraParam.Temperature;
      break;
   case EFeature::eZoom:
      return &m_oCurrentCameraParam.Zoom;
      break;
   case EFeature::ePan:
      return &m_oCurrentCameraParam.Pan;
      break;
   case EFeature::eTilt:
      return &m_oCurrentCameraParam.Tilt;
      break;
   case EFeature::eOpticalFilter:
      return &m_oCurrentCameraParam.OpticalFilter;
      break;
   case EFeature::eCompressionLevel:
      return &m_oCurrentCameraParam.CompressionLevel;
      break;
   case EFeature::eAutoLevel:
      return &m_oCurrentCameraParam.AutoLevel;
      break;
   case EFeature::eAutoPeakAverage:
      return &m_oCurrentCameraParam.AutoPeakAverage;
      break;
   case EFeature::eShutter:
      return &m_oCurrentCameraParam.Shutter;
      break;
   case EFeature::eFrameExposure:
      return &m_oCurrentCameraParam.FrameExposure;
      break;
   case EFeature::eTrigger:
      return &m_oCurrentCameraParam.TriggerInq;
      break;
   //case EFeature::eIrisInqOffset - that's for Set functionality only
   //case EFeature::eBits - don't have descriptive field
   //case EFeature::eLeftFramePos - isn't real feature
   //case EFeature::eTopFramePos - isn't real feature
   //case EFeature::eWidthFramePos - isn't real feature
   //case EFeature::eHeightFramePos - isn't real feature
   // eOneMultiFrameRequest
   // eSignalsControlReg
   // eDSTSyncOut0,
   // eDSTSyncOut1,
   // eDSTSyncOut2,
   // eDSTSyncOut3,
   // eETSyncOut0,
   // eETSyncOut1,
   // eETSyncOut2,
   // eETSyncOut3
   // eSNTPTimerPeriod
  }
      
   assert(false);
   return feature_t();
}

bool OCameraDataManager::GetFeaturePresence(EFeature eFeature_p) const
{
   switch (eFeature_p)
   {
   case EFeature::eBits:
   case EFeature::eSNTPTimerPeriod:
      return true;
      break;
   case EFeature::eLeftFramePos:
   case EFeature::eTopFramePos:
   case EFeature::eWidthFramePos:
   case EFeature::eHeightFramePos:
      return m_oCurrentCameraParam.CameraFormats.formatsList[GetCurrentFormat()].bPartial;
      break;
   case EFeature::eDSTSyncOut0:
   case EFeature::eETSyncOut0:
      return (m_CurrentCameraSignalsControlReg & 0x1) > 0;
      break;
   case EFeature::eDSTSyncOut1:
   case EFeature::eETSyncOut1:
      return (m_CurrentCameraSignalsControlReg & 0x2) > 0;
      break;
   case EFeature::eDSTSyncOut2:
   case EFeature::eETSyncOut2:
      return (m_CurrentCameraSignalsControlReg & 0x4) > 0;
      break;
   case EFeature::eDSTSyncOut3:
   case EFeature::eETSyncOut3:
      return (m_CurrentCameraSignalsControlReg & 0x8) > 0;
      break;
   default:
      {
         feature_t oFeature = GetFeature(eFeature_p);
         switch (oFeature.index())
         {
         case 0:
            return std::get<0>(oFeature)->Presence;
            break;
         case 1:
            return std::get<1>(oFeature)->Presence;
            break;
         case 2:
            return std::get<2>(oFeature)->Presence;
            break;
         }
      }
      break;
   }
   assert(false);
   return false;
}

std::pair<eseusbcam_unsigned_long_t, eseusbcam_unsigned_long_t> OCameraDataManager::GetFeatureRange(EFeature eFeature_p) const
{
   std::pair<eseusbcam_unsigned_long_t, eseusbcam_unsigned_long_t> oRange(0, 0);
   switch(eFeature_p)
   {
   case EFeature::eBits:
      oRange = std::make_pair(0,4);
      break;
   case EFeature::eLeftFramePos:
      oRange = std::make_pair(0,m_oCurrentCameraScanArea.MaxWidth-m_oCurrentCameraScanArea.WidthFrame);
      break;
   case EFeature::eTopFramePos:
      oRange = std::make_pair(0,m_oCurrentCameraScanArea.MaxHeight-m_oCurrentCameraScanArea.HeightFrame);
      break;
   case EFeature::eWidthFramePos:
      oRange = std::make_pair(m_oCurrentCameraScanArea.MinWidth, m_oCurrentCameraScanArea.MaxWidth);
      break;
   case EFeature::eHeightFramePos:
      oRange = std::make_pair(m_oCurrentCameraScanArea.MinHeight,m_oCurrentCameraScanArea.MaxHeight);
      break;
   case EFeature::eDSTSyncOut0:
   case EFeature::eETSyncOut0:
   case EFeature::eDSTSyncOut1:
   case EFeature::eETSyncOut1:
   case EFeature::eDSTSyncOut2:
   case EFeature::eETSyncOut2:
   case EFeature::eDSTSyncOut3:
   case EFeature::eETSyncOut3:
      oRange = std::make_pair(0,6000);
      break;
   case EFeature::eSNTPTimerPeriod:
      oRange = std::make_pair(0,3600);
      break;
   default:
      {
         feature_t oFeature = GetFeature(eFeature_p);
         switch (oFeature.index())
         {
         case 0:
            {
               const FEATURE_ELEMENTS_REG& rFeature = *std::get<0>(oFeature);
               oRange = std::make_pair(rFeature.MinValue, rFeature.MaxValue);
            }
            break;
         case 1:
            {
               const SHUTTER_INQ_REG& rShutter = *std::get<1>(oFeature);
               oRange = std::make_pair(rShutter.MinValue, rShutter.MaxValue);
            }
            break;
         case 2:
            assert(false);
            break;
         }
      }
      break;
   }
   return oRange;
}

eseusbcam_unsigned_long_t OCameraDataManager::GetFeatureValue(EFeature eFeature_p) const
{
   eseusbcam_unsigned_long_t lValue = 0;
   switch(eFeature_p)
   {
   case EFeature::eLeftFramePos:
      lValue = m_oCurrentCameraScanArea.LeftFrame;
      break;
   case EFeature::eTopFramePos:
      lValue = m_oCurrentCameraScanArea.TopFrame;
      break;
   case EFeature::eWidthFramePos:
      lValue = m_oCurrentCameraScanArea.WidthFrame;
      break;
   case EFeature::eHeightFramePos:
      lValue = m_oCurrentCameraScanArea.HeightFrame;
      break;
   default:
      USB_GetCameraFeature(m_oCurrentCameraName.c_str(), GetFeatureID(eFeature_p), &lValue);
      break;
   }
   return lValue;
}

bool OCameraDataManager::SetFeatureValue(EFeature eFeature_p, eseusbcam_unsigned_long_t lVal_p)
{
   bool bRet = true;
   switch(eFeature_p)
   {
   case EFeature::eLeftFramePos:
      m_oCurrentCameraScanArea.LeftFrame = lVal_p;
      break;
   case EFeature::eTopFramePos:
      m_oCurrentCameraScanArea.TopFrame = lVal_p;
      break;
   case EFeature::eWidthFramePos:
      m_oCurrentCameraScanArea.SetWidth(lVal_p);
      break;
   case EFeature::eHeightFramePos:
      m_oCurrentCameraScanArea.SetHeight(lVal_p);
      break;
   default:
      {
         bRet = USB_SetCameraFeature(m_oCurrentCameraName.c_str(),GetFeatureID(eFeature_p),lVal_p) > 0;
         if (bRet)
         {
            if (eFeature_p == EFeature::eFrameExposure)
            {
               eseusbcam_unsigned_long_t InqShutter = 0;
               if (USB_GetCameraFeature(m_oCurrentCameraName.c_str(), SHUTTER_INQ_OFFSET, &InqShutter) > 0)
               {
                  SHUTTER_INQ_REG& tmpInqShutter = reinterpret_cast<SHUTTER_INQ_REG&>(InqShutter);
                  m_oCurrentCameraParam.Shutter.MaxValue = tmpInqShutter.MaxValue;
               }
            }
            else if (eFeature_p == EFeature::eIrisInqOffset)
            {
               m_oCurrentCameraParam.Iris.MaxValue = lVal_p;
            }
         }
      }
      break;
   }
   return bRet;
}

eseusbcam_unsigned_long_t OCameraDataManager::ConvertShutterToDirect(eseusbcam_unsigned_long_t Shutter) const
{
   if (m_oCurrentCameraParam.ShutterStepTime == 0)
   {
      return Shutter;
   }
   return (m_oCurrentCameraParam.ShutterZeroTime + m_oCurrentCameraParam.ShutterStepTime * Shutter) / 1000;
}

eseusbcam_unsigned_long_t OCameraDataManager::ConvertShutterFromDirect(eseusbcam_unsigned_long_t Shutter) const
{
   if (m_oCurrentCameraParam.ShutterStepTime != 0)
   {
      if ((Shutter * 1000) <= m_oCurrentCameraParam.ShutterZeroTime)
      {
         Shutter = 0;
      }
      else
      {
         Shutter = (Shutter * 1000 - m_oCurrentCameraParam.ShutterZeroTime) / m_oCurrentCameraParam.ShutterStepTime;
      }
   }
   return std::min(std::max(m_oCurrentCameraParam.Shutter.MinValue, Shutter), m_oCurrentCameraParam.Shutter.MaxValue);
}

bool OCameraDataManager::GetFeatureAuto(EFeature eFeature_p) const
{
   eseusbcam_unsigned_long_t lValue = 0;
   USB_GetAutoFeature(m_oCurrentCameraName.c_str(), GetFeatureID(eFeature_p), &lValue);
   return lValue != 0;
}

void OCameraDataManager::SetFeatureAuto(EFeature eFeature_p, bool bOn)
{
   USB_SetAutoFeature(m_oCurrentCameraName.c_str(),GetFeatureID(eFeature_p), bOn ? 1 : 0);
}

eseusbcam_unsigned_long_t OCameraDataManager::GetFeatureID(EFeature eFeature_p)
{
   switch(eFeature_p)
   {
   case EFeature::eBrightness:
      return BRIGHTNESS;
      break;
   case EFeature::eContrast:
      return CONTRAST;
      break;
   case EFeature::eSharpness:
      return SHARPNESS;
      break;
   case EFeature::eWhiteBalance:
      return WHITE_BALANCE;
      break;
   case EFeature::eRed:
      return WB_RED;
      break;
   case EFeature::eGreen:
      return WB_GREEN;
      break;
   case EFeature::eBlue:
      return WB_BLUE;
      break;
   case EFeature::eSaturation:
      return SATURATION;
      break;
   case EFeature::eGamma:
      return GAMMA;
      break;
   case EFeature::eGain:
      return GAIN;
      break;
   case EFeature::eIris:
      return IRIS;
      break;
   case EFeature::eFocus:
      return FOCUS;
      break;
   case EFeature::eTemperature:
      return TEMPERATURE;
      break;
   case EFeature::eZoom:
      return ZOOM;
      break;
   case EFeature::ePan:
      return PAN;
      break;
   case EFeature::eTilt:
      return TILT;
      break;
   case EFeature::eOpticalFilter:
      return OPTICAL_FILTER;
      break;
   case EFeature::eCompressionLevel:
      return CAPTURE_QUALITY;
      break;
   case EFeature::eAutoLevel:
      return AUTO_LEVEL;
      break;
   case EFeature::eAutoPeakAverage:
      return AUTO_PEAK_AVERAGE;
      break;
   case EFeature::eShutter:
      return SHUTTER;
      break;
   case EFeature::eFrameExposure:
      return FRAMES_EXPOSURE;
      break;
   case EFeature::eIrisInqOffset:
      return IRIS_INQ_OFFSET;
      break;
   case EFeature::eBits:
      return BITS;
      break;
   case EFeature::eTrigger:
      return TRIGGER_MODE;
      break;
   case EFeature::eOneMultiFrameRequest:
      return ONE_MULTI_FRAME_REQUEST;
      break;
   case EFeature::eSignalsControlReg:
      return SIGNALS_CONTROL_REG;
      break;
   case EFeature::eDSTSyncOut0:
      return DST_SYNCOUT0;
      break;
   case EFeature::eETSyncOut0:
      return ET_SYNCOUT0;
      break;
   case EFeature::eDSTSyncOut1:
      return DST_SYNCOUT1;
      break;
   case EFeature::eETSyncOut1:
      return ET_SYNCOUT1;
      break;
   case EFeature::eDSTSyncOut2:
      return DST_SYNCOUT2;
      break;
   case EFeature::eETSyncOut2:
      return ET_SYNCOUT2;
      break;
   case EFeature::eDSTSyncOut3:
      return DST_SYNCOUT3;
      break;
   case EFeature::eETSyncOut3:
      return ET_SYNCOUT3;
      break;
   case EFeature::eSNTPTimerPeriod:
      return SNTP_TIMER_PERIOD_REG;
      break;
   case EFeature::eFormatOps:
      return FORMAT_IMAGE_OPS;
      break;
   }

   assert(false);
   return 0;
}

std::string OCameraDataManager::GetFeatureName(EFeature eID)
{
   switch(eID)
   {
   case EFeature::eBrightness:
      return "Brightness";
      break;
   case EFeature::eContrast:
      return "Contrast";
      break;
   case EFeature::eSharpness:
      return "Sharpness";
      break;
   case EFeature::eWhiteBalance:
      return "White Balance";
      break;
   case EFeature::eRed:
      return "Red";
      break;
   case EFeature::eGreen:
      return "Green";
      break;
   case EFeature::eBlue:
      return "Blue";
      break;
   case EFeature::eSaturation:
      return "Saturation";
      break;
   case EFeature::eGamma:
      return "Gamma";
      break;
   case EFeature::eGain:
      return "Gain";
      break;
   case EFeature::eIris:
      return "Iris";
      break;
   case EFeature::eFocus:
      return "Focus";
      break;
   case EFeature::eTemperature:
      return "Temperature";
      break;
   case EFeature::eZoom:
      return "Zoom";
      break;
   case EFeature::ePan:
      return "Pan";
      break;
   case EFeature::eTilt:
      return "Tilt";
      break;
   case EFeature::eOpticalFilter:
      return "Optical Filter";
      break;
   case EFeature::eCompressionLevel:
      return "Compression Level";
      break;
   case EFeature::eAutoLevel:
      return "Auto Level";
      break;
   case EFeature::eAutoPeakAverage:
      return "Auto Peak Average";
      break;
   case EFeature::eShutter:
      return "Shutter";
      break;
   case EFeature::eFrameExposure:
      return "Add Lines";
      break;
   case EFeature::eBits:
      return "Select Bits";
      break;
   case EFeature::eLeftFramePos:
      return "Left Offset";
      break;
   case EFeature::eTopFramePos:
      return "Top Offset";
      break;
   case EFeature::eWidthFramePos:
      return "Width";
      break;
   case EFeature::eHeightFramePos:
      return "Height";
      break;
   case EFeature::eTrigger:
      return "Trigger Mode";
      break;
   case EFeature::eDSTSyncOut0:
   case EFeature::eDSTSyncOut1:
   case EFeature::eDSTSyncOut2:
   case EFeature::eDSTSyncOut3:
      return "Delay Start Time, uS";
      break;
   case EFeature::eETSyncOut0:
   case EFeature::eETSyncOut1:
   case EFeature::eETSyncOut2:
   case EFeature::eETSyncOut3:
      return "Active Period Time, uS";
      break;
   case EFeature::eSNTPTimerPeriod:
      return "SNTP Update Interval";
      break;
   }

   assert(false);
   return "";
}

bool OCameraDataManager::IsAutoSupported(EFeature eID)
{
   switch(eID)
   {
   case EFeature::eGain:
   case EFeature::eIris:
   case EFeature::eShutter:
      return true;
      break;
/*   case EFeature::eIrisInqOffset: // special features for Set functionality only
   case EFeature::eOneMultiFrameRequest:
      break;
*/      
/*   case EFeature::eSharpness:
   case EFeature::eBrightness:
   case EFeature::eContrast:
   case EFeature::eWhiteBalance:
   case EFeature::eSaturation:
   case EFeature::eGamma:
   case EFeature::eFocus:
   case EFeature::eTemperature:
   case EFeature::eZoom:
   case EFeature::ePan:
   case EFeature::eTilt:
   case EFeature::eOpticalFilter:
   case EFeature::eCompressionLevel:
   case EFeature::eAutoLevel:
   case EFeature::eAutoPeakAverage:
   case EFeature::eFrameExposure:
   case EFeature::eBits:
   case EFeature::eLeftFramePos:
   case EFeature::eTopFramePos:
   case EFeature::eWidthFramePos:
   case EFeature::eHeightFramePos:
   case EFeature::eTrigger:
*/   default:
      return false;
   }

   assert(false);
   return false;
}

bool OCameraDataManager::StartVideoStream()
{
   if (!m_bInitialRequestWasDone)
   {
      LoadCamerasList();
   }

   if (m_oCurrentCameraName.empty())
   {
      return false;
   }

   if (IsStreamWorked())
   {
      return true;
   }

	auto CurrentCameraFormat = m_oCurrentCameraParam.CameraFormats.CurrFormat;
	auto Width = m_oCurrentCameraParam.CameraFormats.formatsList[CurrentCameraFormat].WidthFrame;
	auto Height = m_oCurrentCameraParam.CameraFormats.formatsList[CurrentCameraFormat].HeightFrame;

   VIDEO_STREAM_PARAM_EX oStreamParam = {};
   oStreamParam.ppReturnedParams = nullptr;
   oStreamParam.pBuffBitmap = nullptr;
   oStreamParam.pDirectBuffer = nullptr;
   oStreamParam.pEvent = nullptr;

   m_oFrameData.first->Init(Width, Height, m_oFrameObservers);
   m_oFrameData.second->Init(Width, Height, m_oFrameObservers);
   m_oFramesReceivingFPS.Init();

   m_bStreamWorked = USB_VideoOnCallbackStart(m_oCurrentCameraName.c_str(), &oStreamParam, CameraCallback, nullptr);
   if (m_bStreamWorked)
   {
      m_poLogReceiver->AddEvent(ILogMessagesReceiver::EEvents::eStartStream);
   }
   return m_bStreamWorked;
}

bool OCameraDataManager::StopVideoStream()
{
   if (!IsStreamWorked())
   {
      return true;
   }
   m_bStreamWorked = !USB_StopVideoStream(m_oCurrentCameraName.c_str());
   if (!m_bStreamWorked)
   {
      CheckFrameProcessingIsFinished();
      m_poLogReceiver->AddEvent(ILogMessagesReceiver::EEvents::eStopStream);
   }
   return !m_bStreamWorked;
}

void OCameraDataManager::StopVideoStreamOnError(std::string&& Message)
{
   if (!IsStreamWorked())
   {
      return;
   }
   CheckFrameProcessingIsFinished();
   m_bStreamWorked = false;
   if (m_poSteramEventsObserver)
   {
      m_poSteramEventsObserver->StopStreamOnRequest(IStreamEventsObserver::ERquestType::eError, std::move(Message));
   }
}

bool OCameraDataManager::IsStreamWorked()
{
   return m_bStreamWorked;
}

void OCameraDataManager::AddNewFrameObserver(INewFrameObserver* poFrameObserver)
{
   if (poFrameObserver)
   {
      if (std::find(m_oFrameObservers.begin(), m_oFrameObservers.end(), poFrameObserver) == m_oFrameObservers.end())
      {
         m_oFrameObservers.push_back(poFrameObserver);
      }
   }
}

void OCameraDataManager::SetStreamErrorObserver(IStreamEventsObserver* poSteramErrorObserver)
{
   m_poSteramEventsObserver = poSteramErrorObserver;
}

bool OCameraDataManager::CheckFrameProcessingIsFinished(bool bBlocking_p)
{
   if (bBlocking_p)
   {
      std::lock_guard oLock(m_oFrameProcessingMutex);
   }
   else
   {
      std::unique_lock oLock(m_oFrameProcessingMutex, std::defer_lock);
      return oLock.try_lock();
   }
   return true;

   //m_pCurrentBuffer = m_oCamBuffers.pCurrentBuffer;
}

std::unique_ptr<std::shared_lock<std::shared_mutex>> OCameraDataManager::SharedLockFrameData()
{
   return std::make_unique<std::shared_lock<std::shared_mutex>>(m_oFrameData.second->Access_Mutex());
}

void OCameraDataManager::ProcessNewFrame(unsigned char* pFrame)
{
   std::lock_guard oLock(m_oFrameProcessingMutex);
   m_oFrameData.first->m_eState = INewFrameObserver::ETransferState::eNormal;
   try
   {
      if (pFrame)
      {
         RET_SERV_DATA_HEADER &rRetHeader = *reinterpret_cast<RET_SERV_DATA_HEADER *>(pFrame);
         unsigned char *pDirectImageData = nullptr;
         unsigned short Width = 0;
         unsigned short Height = 0;
         eseusbcam_unsigned_long_t lFrameNumber = 0;
         // !!!!!!!!!!!!!!!!!!!!!
         //rRetHeader.ulServDataType = 0; // !!!!!!!!!!!!!!!!!!!!!
         //rRetHeader.ulNextDataSize = 1; // !!!!!!!!!!!!!!!!!!!!!
         // !!!!!!!!!!!!!!!!!!!!!
         if ((rRetHeader.ulNextDataSize == 0))
         {
            m_oFrameData.first->m_eState = INewFrameObserver::ETransferState::eHeaderIssue;
            m_poLogReceiver->AddEvent(ILogMessagesReceiver::EEvents::eFrameHeaderIssue);
         }
         else
         {
            auto CurrentCameraFormat = m_oCurrentCameraParam.CameraFormats.CurrFormat;
            unsigned char BitsPerPixel = m_oCurrentCameraParam.CameraFormats.formatsList[CurrentCameraFormat].bitCountBitMap;
            unsigned char chColorCoding = 0;
            switch (rRetHeader.ulServDataType)
            {
            case 3:
            {
               RET_SERV_DATA_ITK4_0 &rRetSrv3 = *reinterpret_cast<RET_SERV_DATA_ITK4_0 *>(pFrame);
               m_oFrameData.first->m_oServiceData = rRetSrv3.StreamServData;
               STREAM_SERV_DATA_ITK4_0 &sServDataItk4 = std::get<3>(m_oFrameData.first->m_oServiceData);

               eseusbcam_unsigned_long_t ulServTableSize = sizeof(RET_SERV_DATA_ITK4_0);
               pDirectImageData = (pFrame + ulServTableSize); //указатель на видеоданные
               BitsPerPixel = sServDataItk4.chBitPerPixel;
               Width = sServDataItk4.usWidth;
               Height = sServDataItk4.usHeight;

               lFrameNumber = sServDataItk4.ulFrameNum;

               chColorCoding = sServDataItk4.chColorCoding;

               m_oFrameData.first->m_RadarErrorRet = RadarBufferCheck(&sServDataItk4.RadarData[0], 1600, *m_oFrameData.first);
            }
            break;
            case 2:
            {
               RET_SERV_DATA_TYPE_2 &rRetSrv2 = *reinterpret_cast<RET_SERV_DATA_TYPE_2 *>(pFrame);
               m_oFrameData.first->m_oServiceData = rRetSrv2.StreamServData;
               STREAM_SERV_DATA_TYPE_2 &sServDataType2 = std::get<2>(m_oFrameData.first->m_oServiceData);

               eseusbcam_unsigned_long_t ulServTableSize = sizeof(RET_SERV_DATA_TYPE_2);
               pDirectImageData = (pFrame + ulServTableSize); //указатель на видеоданные
               BitsPerPixel = sServDataType2.chBitPerPixel;
               Width = sServDataType2.ulWidth;
               Height = sServDataType2.ulHeight;

               lFrameNumber = sServDataType2.ulFrameNum;
            }
            break;
            case 0:
            {
               pDirectImageData = pFrame;
               BitsPerPixel = 8;
               Width = m_oFrameData.first->GetFrameBuffer().GetWidth();
               Height = m_oFrameData.first->GetFrameBuffer().GetHeight();
               lFrameNumber = 0;
               m_oFrameData.first->m_oServiceData = std::monostate();
            }
            break;
            }

            if (m_oFrameData.first->GetFrameBuffer().GetWidth() != Width ||
                m_oFrameData.first->GetFrameBuffer().GetHeight() != Height ||
                BitsPerPixel == 0 ||
                pDirectImageData == nullptr)
            {
               m_oFrameData.first->m_eState = INewFrameObserver::ETransferState::eFrameInfoIssue;
               m_poLogReceiver->AddEvent(ILogMessagesReceiver::EEvents::eCorruptFrameInfo);
            }
            else
            {
               if (m_oFrameData.first->m_ReceivedFramesCount > 0 && lFrameNumber > 0)
               {
                  eseusbcam_long_t lMissed = lFrameNumber - m_oFrameData.first->m_PreviousFrameNumber - 1;
                  if (lMissed > 0)
                  {
                     m_poLogReceiver->AddEvent(ILogMessagesReceiver::EEvents::eMissedFrame, lMissed);
                     m_oFrameData.first->m_ErrorFramesCount += lMissed;
                  }
               }
               m_oFrameData.first->m_PreviousFrameNumber = lFrameNumber;
               ++m_oFrameData.first->m_ReceivedFramesCount;

               if (GetFramesAreRequired())
               {
                  if (chColorCoding > 0) // color image
                  {
                     if (GetDecode())
                     {
                        if (chColorCoding == 4) // 4 - YUV 4:2:0 (T.871)
                        {
                           YUV420_TO_RGB(
                              m_oFrameData.first->GetNativeFrameData(),
                              pDirectImageData,
                              Width,
                              Height,
                              GetFlipImageH(),
                              GetFlipImageV());
                        }
                        else
                        {
                           FilterBayerRGB_5GREEN_F(m_oFrameData.first->GetNativeFrameData(),
                              pDirectImageData,
                              Width,
                              Height/*,
                              GetFlipImageH(),
                              GetFlipImageV()*/);
                        }
                     }
                     else
                     {
                        wxNativePixelData oBitmapData = m_oFrameData.first->GetNativeFrameData();
                        #pragma omp parallel
                        {
                           wxNativePixelData::Iterator oOutputIter(oBitmapData);

                        #pragma omp for schedule(dynamic, 10)
                           for (eseusbcam_long_t lIndexY = 0; lIndexY < Height; ++lIndexY)
                           {
                              oOutputIter.MoveTo(oBitmapData, 0, lIndexY);
                              auto pSourceRow = pDirectImageData + Width * lIndexY;
                              for (std::size_t lIndexX = 0; lIndexX < Width; ++lIndexX)
                              {
                                 oOutputIter.Red() = oOutputIter.Green() = oOutputIter.Blue() = pSourceRow[lIndexX];
                                 ++oOutputIter;
                              }
                           }
                        }
                     }
                  }
                  else 
                  {
                     if (BitsPerPixel == 12)
                     {
                        Mono12bitToMono8bit(
                           m_oFrameData.first->GetNativeFrameData(),
                           pDirectImageData,
                           Width,
                           Height,
                           GetFlipImageH(),
                           GetFlipImageV());
                     }
                     else
                     {
                        wxNativePixelData oBitmapData = m_oFrameData.first->GetNativeFrameData();
                        bool bMirrorX = GetFlipImageH();
                        bool bMirrorY = GetFlipImageV();
                        #pragma omp parallel
                        {
                           wxNativePixelData::Iterator oOutputIter(oBitmapData);
                        #pragma omp for schedule(dynamic, 10)
                           for (eseusbcam_long_t lIndexY = 0; lIndexY < Height; ++lIndexY)
                           {
                              oOutputIter.MoveTo(oBitmapData, 0, lIndexY);
                              auto pSourceRow = pDirectImageData + Width*(bMirrorY ? Height-lIndexY-1 : lIndexY);
                              for (std::size_t lIndexX = 0; lIndexX < Width; ++lIndexX)
                              {
                                 oOutputIter.Red() = oOutputIter.Green() = oOutputIter.Blue() = pSourceRow[bMirrorX ? Width-lIndexX-1 : lIndexX];
                                 ++oOutputIter;
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
      else
      {
         m_oFrameData.first->m_eState = INewFrameObserver::ETransferState::eStreamSynchronizationIssue;
         m_poLogReceiver->AddEvent(ILogMessagesReceiver::EEvents::eStreamSynchroIssue);
      }

      FrameData::Swap(m_oFrameData.first, m_oFrameData.second);

      for (auto &roObserver : m_oFrameObservers)
      {
         roObserver->ProcessNewFrame();
      }
   }
   catch(...)
   {
   }
}
void OCameraDataManager::SetFramesAreRequired(bool bKey)
{
   std::lock_guard oLock(m_oFramesAreRequiredMutex);
   m_oFramesAreRequired = bKey;
}

bool OCameraDataManager::GetFramesAreRequired()
{
   std::lock_guard oLock(m_oFramesAreRequiredMutex);
   return m_oFramesAreRequired;
}

void OCameraDataManager::SetFlipImageH(bool bKey)
{
   std::lock_guard<std::mutex> oLock(m_oFlipMutex);
   m_oFlip.first = bKey;
}

void OCameraDataManager::SetFlipImageV(bool bKey)
{
   std::lock_guard<std::mutex> oLock(m_oFlipMutex);
   m_oFlip.second = bKey;
}

bool OCameraDataManager::GetFlipImageH()
{
   std::lock_guard<std::mutex> oLock(m_oFlipMutex);
   return m_oFlip.first;
}

void OCameraDataManager::SetDecode(bool bKey)
{
   m_bDecode = bKey;
}

bool OCameraDataManager::GetDecode()
{
   return m_bDecode;
}

bool OCameraDataManager::GetFlipImageV()
{
   std::lock_guard<std::mutex> oLock(m_oFlipMutex);
   return m_oFlip.second;
}

const OCameraDataManager::FrameData& OCameraDataManager::GetFrameData() const
{
   return *m_oFrameData.second;
}

unsigned short OCameraDataManager::GetStream0Port() const
{
   return m_oCurrentCameraParam.Stream0_port;
}

unsigned short OCameraDataManager::GetStream0PacketSize() const
{
   return m_oCurrentCameraParam.Stream0_packet_size;
}

bool OCameraDataManager::SetStream0Params(unsigned short usPort, unsigned short PacketSize)
{
   if (USB_SetCameraFeature(GetCurrentCameraName().c_str(), STREAM0_PARAM_REG_OFFSET, (PacketSize << 16) | (usPort))>0)
   {
      m_oCurrentCameraParam.Stream0_packet_size = PacketSize;
      m_oCurrentCameraParam.Stream0_port = usPort;
      return true;
   }
   return false;
}

bool OCameraDataManager::SaveSettings(eseusbcam_unsigned_long_t lPreset)
{
   return USB_SaveParams(GetCurrentCameraName().c_str(), lPreset) > 0;
}

std::list<std::string> OCameraDataManager::GetFormatNames() const
{
   std::list<std::string> oFormats;
   for (std::size_t i = 0; i < m_oCurrentCameraParam.CameraFormats.numFormats; ++i)
   {
      oFormats.emplace_back(m_oCurrentCameraParam.CameraFormats.formatsList[i].FormatName);
   }
   return oFormats;
}

eseusbcam_unsigned_long_t OCameraDataManager::GetCurrentFormat() const
{
   return m_oCurrentCameraParam.CameraFormats.numFormats > 0 ? m_oCurrentCameraParam.CameraFormats.CurrFormat : 0;
}

bool OCameraDataManager::SetCurrentFormat(eseusbcam_unsigned_long_t lFormatIndex)
{
   bool bStream = IsStreamWorked();
   StopVideoStream();

   bool bRet = false;
   if (USB_SetFormat(GetCurrentCameraName().c_str(), lFormatIndex) > 0)
   {
      UpdateCameraCapabilities();
      bRet = true;
   }

   if (bStream)
   {
      if (m_bAllowStreamRestartAfterParamsChange)
      {
         StartVideoStream();
      }
      else
      {
         m_poSteramEventsObserver->StopStreamOnRequest(IStreamEventsObserver::ERquestType::eSimple, {});
      }
   }

   return bRet;
}

void OCameraDataManager::FillFrameAreaFromCurrentFormat()
{
   m_oCurrentCameraScanArea.Fill_From_IP_FORMATS_LIST(m_oCurrentCameraParam.CameraFormats.formatsList[GetCurrentFormat()]);
}

unsigned short OCameraDataManager::ConvertPartialWidthToFull(unsigned short Width) const
{
   return m_oCurrentCameraScanArea.ConvertPartialWidthToFull(Width);
}

unsigned short OCameraDataManager::ConvertPartialHeightToFull(unsigned short Height) const
{
   return m_oCurrentCameraScanArea.ConvertPartialHeightToFull(Height);
}

bool OCameraDataManager::SetScanArea()
{
   bool bStream = IsStreamWorked();
   StopVideoStream();
   bool bRet = USB_SetScanArea(GetCurrentCameraName().c_str(),
         ConvertPartialWidthToFull(m_oCurrentCameraScanArea.LeftFrame),
         ConvertPartialHeightToFull(m_oCurrentCameraScanArea.TopFrame),
         ConvertPartialWidthToFull(m_oCurrentCameraScanArea.WidthFrame),
         ConvertPartialHeightToFull(m_oCurrentCameraScanArea.HeightFrame)) > 0;
   if (bRet)
   {
      UpdateCameraCapabilities();
   }
   else
   {
      m_oCurrentCameraScanArea.Set_To_IP_FORMATS_LIST(m_oCurrentCameraParam.CameraFormats.formatsList[GetCurrentFormat()]);
   }
   if (bStream)
   {
      if (m_bAllowStreamRestartAfterParamsChange)
      {
         StartVideoStream();
      }
      else
      {
         m_poSteramEventsObserver->StopStreamOnRequest(IStreamEventsObserver::ERquestType::eSimple, {});
      }
   }
   return bRet;
}

void OCameraDataManager::ClearUARTBuffer(EUART eUart_p)
{
   switch (eUart_p)
   {
   case EUART::eUART1:
      USB_SetCameraFeature(m_oCurrentCameraName.c_str(), RST_REG, FLAG_RST_UART1);
      break;
   case EUART::eUART2:
      USB_SetCameraFeature(m_oCurrentCameraName.c_str(), RST_REG, FLAG_RST_UART2);
      break;
   default:
      assert(false && "Unsupported UART");
      std::logic_error("Unsupported UART");
   }
}

// true - ok, false - overflow
bool OCameraDataManager::ReceiveUARTBuffer(EUART eUart_p, std::vector<unsigned char>& roBuffer_p)
{
   bool bOk = true;
   eseusbcam_unsigned_long_t ulSize = 0;
   if (USB_GetCameraFeature(m_oCurrentCameraName.c_str(), 
         eUart_p == EUART::eUART1 ? UART1_RECEIVE_SIZE_REG : UART2_RECEIVE_SIZE_REG,
         &ulSize) > 0)
   {
      if (ulSize == 0)
      {
         roBuffer_p.clear();
      }
      else
      {
         eseusbcam_unsigned_long_t lTableID = 0;
         switch (eUart_p)
         {
         case EUART::eUART1:
            bOk = ulSize <= UART1_RECEIVE_MAX_SIZE;
            ulSize = std::min(ulSize, static_cast<eseusbcam_unsigned_long_t>(UART1_RECEIVE_MAX_SIZE));
            lTableID = TABLE_TYPE_UART1;
            break;
         case EUART::eUART2:
            bOk = ulSize <= UART2_RECEIVE_MAX_SIZE;
            ulSize = std::min(ulSize, static_cast<eseusbcam_unsigned_long_t>(UART2_RECEIVE_MAX_SIZE));
            lTableID = TABLE_TYPE_UART2;
            break;
         default:
            assert(false && "Unsupported UART");
            std::logic_error("Unsupported UART");
         }
         roBuffer_p.resize(ulSize);
         if (USB_ReceiveTable(m_oCurrentCameraName.c_str(), lTableID, ulSize, roBuffer_p.data()) == 0)
         {
            roBuffer_p.clear();
         }
      }
   }
   else
   {
      GetLogReceiver().AddMessage("Unable to receive UART table size from camera");
   }
   return bOk;
}

// true - ok, false - issue
bool OCameraDataManager::SendUARTBuffer(EUART eUart_p, const std::vector<unsigned char>& roBuffer_p)
{
   // Hotfix until camera will clear transmit size after transmission is done
   #pragma message "Remove hotfix after the issue is resolved on camera side"
   USB_SetCameraFeature(m_oCurrentCameraName.c_str(), eUart_p == EUART::eUART1 ? UART1_TRANSMIT_SIZE_REG : UART2_TRANSMIT_SIZE_REG, 0);
   // end of Hotfix
   bool bOk = true;
   eseusbcam_unsigned_long_t ulSize = 0;
	if (USB_GetCameraFeature(m_oCurrentCameraName.c_str(),
         eUart_p == EUART::eUART1 ? UART1_TRANSMIT_SIZE_REG : UART2_TRANSMIT_SIZE_REG,
         &ulSize))
	{
      if (ulSize == 0) //проверяем, что UART не находится в состоянии передачи (регистр UART1_TRANSMIT_SIZE_REG равен "0")
      {
         if (USB_SendTable(m_oCurrentCameraName.c_str(), eUart_p == EUART::eUART1 ? TABLE_TYPE_UART1 : TABLE_TYPE_UART2, roBuffer_p.size(), roBuffer_p.data()))//записываем данные в буфер передачи UART1
			{
				USB_SetCameraFeature(m_oCurrentCameraName.c_str(), eUart_p == EUART::eUART1 ? UART1_TRANSMIT_SIZE_REG : UART2_TRANSMIT_SIZE_REG, roBuffer_p.size());//отправляем команду на передачу заданного числа байт через UART1
			}
         else
         {
            bOk = false;
            GetLogReceiver().AddMessage(std::string(eUart_p == EUART::eUART1 ? "UART1" : "UART2") + " data table sending issue");
         }
		}
		else
		{
         bOk = false;
         GetLogReceiver().AddMessage(std::string(eUart_p == EUART::eUART1 ? "UART1" : "UART2") + " is already transferring data");
		}
   }
   else
   {
      bOk = false;
      GetLogReceiver().AddMessage(std::string(eUart_p == EUART::eUART1 ? "UART1" : "UART2") + " communication issue");
   }
   return bOk;
}


void OCameraDataManager::ReceiveTriggerModeValue()
{
   eseusbcam_unsigned_long_t FlatTriggerMode = GetFeatureValue(EFeature::eTrigger);
   m_oCurrentCameraTrigReg.Parameter = FlatTriggerMode & 0xfff;       //	      :12,
   m_oCurrentCameraTrigReg.Reserved3 = (FlatTriggerMode >> 12) & 0xf;   //	   :4,
   m_oCurrentCameraTrigReg.TriggerMode = (FlatTriggerMode >> 16) & 0xf; //  :4,		//режим триггера
   m_oCurrentCameraTrigReg.Reserved2 = (FlatTriggerMode >> 20) & 0xf;   //	   :4,
   m_oCurrentCameraTrigReg.Polarity = (FlatTriggerMode >> 24) & 0x1;    //	   :1,      //полярность внешнего сигнала  0 - активен переход 1->0; 1 - активен переход 0->1
   m_oCurrentCameraTrigReg.OnOff = (FlatTriggerMode >> 25) & 0x1;       //		   :1,      //вкл/откл управления регулировкой
   m_oCurrentCameraTrigReg.Reserved1 = (FlatTriggerMode >> 26) & 0x1f;  //	:5,
   m_oCurrentCameraTrigReg.Presence = (FlatTriggerMode >> 31) & 0x1;    //     :1;      //доступность регулировки
}

void OCameraDataManager::ReceiveCameraImageOpsValue()
{
   eseusbcam_unsigned_long_t FlatTriggerMode = GetFeatureValue(EFeature::eFormatOps);
   m_oCurrentCameraImageOps.FillValues(FlatTriggerMode);
}

bool OCameraDataManager::SetTriggerModeValue(const TRIGGER_CTRL_REG& rTCR)
{
   eseusbcam_unsigned_long_t FlatTriggerMode =
      rTCR.Parameter
      | (rTCR.Reserved3 << 12)
      | (rTCR.TriggerMode << 16)
      | (rTCR.Reserved2 << 20)
      | (rTCR.Polarity << 24)
      | (rTCR.OnOff << 25)
      | (rTCR.Reserved1 << 26)
      | (rTCR.Presence << 31);

   if (SetFeatureValue(EFeature::eTrigger, FlatTriggerMode))
   {
      m_oCurrentCameraTrigReg = rTCR;
      return true;
   }
   return false;
}

const TRIGGER_CTRL_REG& OCameraDataManager::GetTriggerMode() const
{
   return m_oCurrentCameraTrigReg;
}

bool OCameraDataManager::SetCameraImageFormatOptions(const CameraImageOptions& rFO)
{
   if (SetFeatureValue(EFeature::eFormatOps, rFO.FlattenValues()))
   {
      m_oCurrentCameraImageOps = rFO;
      return true;
   }
   return false;
}

const OCameraDataManager::CameraImageOptions& OCameraDataManager::GetCameraImageFormatOptions() const
{
   return m_oCurrentCameraImageOps;
}

bool OCameraDataManager::SetEnableDelayTiming(eseusbcam_unsigned_long_t OutIndex, bool Enable)
{
   auto InitState = m_CurrentCameraSignalsControlReg;
   m_CurrentCameraSignalsControlReg = m_CurrentCameraSignalsControlReg & (1 << OutIndex ^ 0xf);
   if (Enable)
   {
      m_CurrentCameraSignalsControlReg += (1 << OutIndex);
   }
   if (!SetFeatureValue(EFeature::eSignalsControlReg, m_CurrentCameraSignalsControlReg))
   {
      m_CurrentCameraSignalsControlReg = InitState;
      return false;
   }
   return true;
}

bool OCameraDataManager::SendLutTable(std::vector<unsigned char>& roTable_p) const
{
   if (m_oCurrentCameraName.empty())
   {
      return false;
   }
   return USB_SendTable(m_oCurrentCameraName.c_str(), TABLE_TYPE_LUT1, roTable_p.size(), roTable_p.data()) == 1;
}

bool OCameraDataManager::ReceiveLutTable(std::vector<unsigned char>& roTable_p) const
{
   if (m_oCurrentCameraName.empty())
   {
      return false;
   }
   return USB_ReceiveTable(m_oCurrentCameraName.c_str(), TABLE_TYPE_LUT1, roTable_p.size(), roTable_p.data()) == 1;
}

void OCameraDataManager::UpdateFPS()
{
   m_oFramesReceivingFPS.Next();
}

double OCameraDataManager::GetFPS()
{
   return m_oFramesReceivingFPS.GetFPS();
}

void OCameraDataManager::SetLogReceiver(ILogMessagesReceiver& poLog)
{
   m_poLogReceiver = &poLog;
}

ILogMessagesReceiver& OCameraDataManager::GetLogReceiver()
{
   assert(m_poLogReceiver!=nullptr);
   return *m_poLogReceiver;
}