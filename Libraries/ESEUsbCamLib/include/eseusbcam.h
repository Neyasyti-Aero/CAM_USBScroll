/*
  Файл определяет прототипы функций, структуры и константы, используемые библиотекой eseusbcam.dll
*/

#ifndef _ESEUSBCAM_H_
#define _ESEUSBCAM_H_

#ifdef __GNUC__
   #define DCAMAPI __attribute__ ((visibility("default")))
#elif _MSC_VER
   #ifdef ESEUsbCam_EXPORTS
      #define DCAMAPI __declspec(dllexport)
   #else
      #define DCAMAPI __declspec(dllimport)
   #endif
#endif

#ifdef __GNUC__
   #if __x86_64__ || __ppc64__ || __aarch64__
      typedef unsigned int eseusbcam_unsigned_long_t;
      typedef int eseusbcam_long_t;
   #else
      typedef unsigned long eseusbcam_unsigned_long_t;
      typedef long eseusbcam_long_t;
   #endif
#elif _WIN32 || _WIN64
   #if _WIN64
      typedef unsigned long eseusbcam_unsigned_long_t;
      typedef long eseusbcam_long_t;
   #else
      typedef unsigned long eseusbcam_unsigned_long_t;
      typedef long eseusbcam_long_t;
   #endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define SUPP_DEVICE_CNT 62
#define SUPP_DEVICE_NAME_LENGTH 255

//регистры управления и состояния для настроек

#define BRIGHTNESS             0x00000800   //установка уровня яркости
#define CONTRAST               0x00000804   //установка контраста
#define SHARPNESS              0x00000808   //установка четкости
#define WHITE_BALANCE          0x0000080C   //установка баланса белого 
#define WB_RED                 0x0000080D   //установка баланса белого красный
#define WB_GREEN               0x0000080E   //установка баланса белого зеленый
#define WB_BLUE                0x0000080F   //установка баланса белого синий
#define LUT_TABLE              0x00001600   //установка значений таблицы перекодировки
#define DST                    0x00001E50   //установка задержки начала экспозиции по внешнему сигналу
#define ET                     0x00001E54   //установка времени экспозиции по внешнему сигналу
   
#define DST_SYNCOUT0           0x00001E80   //установка задержки начала выходного сигнала "0" относительно начала экспозиции
#define ET_SYNCOUT0            0x00001E84   //установка длительности выходного сигнала "0"
#define DST_SYNCOUT1           0x00001E88   //установка задержки начала выходного сигнала "1" относительно начала экспозиции
#define ET_SYNCOUT1            0x00001E8C   //установка длительности выходного сигнала "1"
#define DST_SYNCOUT2           0x00001E90   //установка задержки начала выходного сигнала "2" относительно начала экспозиции
#define ET_SYNCOUT2            0x00001E94   //установка длительности выходного сигнала "2"
#define DST_SYNCOUT3           0x00001E98   //установка задержки начала выходного сигнала "3" относительно начала экспозиции
#define ET_SYNCOUT3            0x00001E9C   //установка длительности выходного сигнала "3"

#define FRAMES_EXPOSURE        0x00000810   //установка числа кадров для накопления
   
#define SATURATION             0x00000814   //установка насыщения
#define GAMMA                  0x00000818   //гамма-коррекция
#define SHUTTER                0x0000081C   //установка затвора
#define GAIN                   0x00000820   //установка усиления
#define IRIS                   0x00000824   //регулировка диафрагмы
#define FOCUS                  0x00000828   //установка фокуса
#define TEMPERATURE            0x0000082C   //установка 
#define TRIGGER_MODE           0x00000830   //установка
#define ZOOM                   0x00000880   //установка увеличения
#define PAN                    0x00000884   //установка
#define TILT                   0x00000888   //установка
#define OPTICAL_FILTER         0x0000088C   //установка

#define CAPTURE_QUALITY        0x000008F0   //установка качества компрессии
#define AUTO_LEVEL             0x000008F4   //установка среднего уровня автоматики
#define AUTO_PEAK_AVERAGE      0x000008F8   //установка баланса пик-среднее автоматики

#define AUTO_RESERVED0         0x00000900   //AUTO_BLACK_LEVEL 
#define AUTO_RESERVED1         0x00000904
#define AUTO_RESERVED2         0x00000908
#define AUTO_RESERVED3         0x0000090C

#define AUTO_BLC_LEFT          0x00000920   //
#define AUTO_BLC_TOP           0x00000924
#define AUTO_BLC_RIGHT         0x00000928
#define AUTO_BLC_BOTTOM        0x0000092C


//коды регистров
//регистры управления и состояния камеры

#define OFFSET_USB_CAMERA_SERIAL   0         //серийный номер камеры
#define STREAM0_PARAM_REG_OFFSET   24         //номер порта и размер пакета для потока "0"

#define SHUTTER_INQ_OFFSET         144      //параметры электронного затвора
#define IRIS_INQ_OFFSET            152      //параметры диафрагмы

#define FORMAT_IMAGE_OPS         0x00000604   //установка опций изображения

#define CAM_PWR_CTRL_REG         0x00000610   //управление питанием камеры
#define ISO_START_STOP           0x00000614   //старт/стоп изохронной передачи
#define MEMORY_SAVE              0x00000618   //сохранение текущих настроек (self cleared)
#define ONE_MULTI_FRAME_REQUEST  0x0000061C   //в режиме внешней синхронизации - запрос одного/нескольких кадров видео (self cleared)
#define MEM_SAVE_CHANNEL         0x00000620   //канал сохранения настроек
#define CURRENT_MEM_CHANNEL      0x00000624   //установка сохраненных настроек
   
#define ALARM_BUFF_START_STOP    0x00000664   //старт/стоп записи в тревожный буфер
   
#define RST_REG                  0x00003000   //регистр сброса 

#define FLAG_RST_FULL            0x00000001   //сброс полный
#define FLAG_RST_HSPEED_TIMER    0x00000100   //сброс таймера 
#define FLAG_RST_FRAME_CNT       0x00000200   //сброс счетчика кадров
#define FLAG_RST_ENCODER         0x00000400   //сброс блока компрессии
#define FLAG_RST_INTERFACE       0x00000800   //сброс интерфейса
#define FLAG_RST_MEMORY          0x00001000   //сброс блока памяти   


#define FRAME_NUM_REG            0x00003200   //регистр содержит последовательный номер текущего кадра (после включения или сброса) 
#define REF_TIMER_REG            0x00003204   //регистр внутреннего таймера. Содержит отсчеты в частоте 90кГц. 
#define SENSOR_STATE_REG_0       0x00003208   //регистр состояния ФЭП

#define SNTP_TIMER_PERIOD_REG    0x00003220   //регистр управления периодом генерации sntp-запросов. В сек. 

#define INT_SYNC_REG             0x00003800   //установка захвата кадра по команде с ПК - имитация внешнего захвата по сигналу
#define LIGHT_SYNC_LEVEL       0x00003820   //установка порога захвата кадра по уровню яркости
#define BITS                     0x00003840   //установка вывода старших/младших бит
#define BITS_WIDTH               0x00003844   //установка количества передаваемых при сжатии бит

//регистры управления и состояния для внешних сигналов

#define   SIGNALS_INQ_REG        0x00001C14   //информационный регистр о наличии доступных для управления сигналов
#define   SIGNALS_CONTROL_REG    0x00001E14   //регистр управления и состояния внешних сигналов

//выходы
#define   OUT0_SIGNAL_REG        0x00001E00   //регистр управления и состояния внешнего выхода 0
#define   OUT1_SIGNAL_REG        0x00001E01   //регистр управления и состояния внешнего выхода 1
#define   OUT2_SIGNAL_REG        0x00001E02   //регистр управления и состояния внешнего выхода 2
#define   OUT3_SIGNAL_REG        0x00001E03   //регистр управления и состояния внешнего выхода 3

#define   OPTO_OUT0_SIGNAL_REG   0x00001E04   //регистр управления и состояния выхода оптрона 0
#define   OPTO_OUT1_SIGNAL_REG   0x00001E05   //регистр управления и состояния выхода оптрона 1
#define   OPTO_OUT2_SIGNAL_REG   0x00001E06   //регистр управления и состояния выхода оптрона 2
#define   OPTO_OUT3_SIGNAL_REG   0x00001E07   //регистр управления и состояния выхода оптрона 3

//входы
#define   IN0_SIGNAL_REG         0x00001E08   //регистр управления и состояния внешнего входа 0
#define   IN1_SIGNAL_REG         0x00001E09   //регистр управления и состояния внешнего входа 1
#define   IN2_SIGNAL_REG         0x00001E0A   //регистр управления и состояния внешнего входа 2
#define   IN3_SIGNAL_REG         0x00001E0B   //регистр управления и состояния внешнего входа 3

#define   OPTO_IN0_SIGNAL_REG    0x00001E0C   //регистр управления и состояния входа оптрона 0
#define   OPTO_IN1_SIGNAL_REG    0x00001E0D   //регистр управления и состояния входа оптрона 1
#define   OPTO_IN2_SIGNAL_REG    0x00001E0E   //регистр управления и состояния входа оптрона 2
#define   OPTO_IN3_SIGNAL_REG    0x00001E0F   //регистр управления и состояния входа оптрона 3

// регистры для работы с таблицами (тип таблицы)
#define   TABLE_TYPE_LUT1         100
#define   TABLE_TYPE_EXPOSURE_16  101
#define   TABLE_TYPE_HOT_CORRECT1 102
#define   TABLE_TYPE_UART1        200
#define   TABLE_TYPE_UART2        201

//регистры управления встроенным мостом UART
#define UART1_TRANSMIT_SIZE_REG			0x0000D000	//регистр числа байт для передачи из буфера передачи UART1
#define UART1_RECEIVE_SIZE_REG			0x0000D004	//регистр числа байт для чтения в буфере приема UART1
#define UART2_TRANSMIT_SIZE_REG			0x0000D008	//регистр числа байт для передачи из буфера UART2
#define UART2_RECEIVE_SIZE_REG			0x0000D00C	//регистр числа байт для чтения в буфере приема UART2 

#define UART1_TRANSMIT_MAX_SIZE				1024	//размер буфера передачи UART1
#define UART1_RECEIVE_MAX_SIZE				1024	//размер буфера приема UART1
#define UART2_TRANSMIT_MAX_SIZE				1024	//размер буфера передачи UART2
#define UART2_RECEIVE_MAX_SIZE				4096	//размер буфера приема UART2

#define FLAG_RST_UART1					0x00004000	//сброс блока UART 1	
#define FLAG_RST_UART2					0x00008000	//сброс блока UART 2	

//имя устройства
typedef struct _DEVICE_LIST {
    char DeviceName[SUPP_DEVICE_NAME_LENGTH];   //имя устройства
} DEVICE_LIST, *PDEVICE_LIST;

//список доступных устройств
typedef struct _DEVICE_DATA {
    eseusbcam_unsigned_long_t numDevices;//число камер в списке
    DEVICE_LIST deviceList[SUPP_DEVICE_CNT];//список имен камер
} DEVICE_DATA, *PDEVICE_DATA;

//описываются поля регистра основных функций камеры
typedef struct _BASIC_FUNC_REG
{
   eseusbcam_unsigned_long_t 
      
       MemoryChannel      :4,      //число доступных пресетов (напр., значение 3 - доступно 3 пресета для записи/чтения и 1 только для чтения(default) - всего 4 пресета)
       Reserved3         :7,
         MultiFrameEna      :1,      //не исп.      
         OneFrameEna      :1,      //не исп.    
         Reserved2         :2,    
         PowerControlEna   :1,      //возможность управления вкл/выкл питания камеры 
         Reserved1         :15,          
         AdvancedFeature   :1;      //не исп. 

} BASIC_FUNC_REG, *PBASIC_FUNC_REG;

//описываются поля HI-регистра настроек камеры 
typedef struct _FEATURE_HI_REG
{
   eseusbcam_unsigned_long_t               
       Reserved1         :19,
       Trigger         :1,      // доступность триггерного режима
       Temperature      :1,      // доступность утановки/контроля температуры
       Focus            :1,      // доступность регулировки фокусировки объектива
       Iris            :1,      // доступность управления диафрагмой объектива
       Gain            :1,      // доступность регулировки усиления
       Shutter         :1,      // доступность регулировки электронного затвора
       Gamma            :1,      // доступность регулировки гамма-корректора
         Saturation         :1,      // доступность регулировки насыщенности
         FrameExposure      :1,      // доступность регулировки кадрового накопления     
         WhiteBalance      :1,      // доступность регулировки баланса белого
         Sharpness         :1,      // доступность регулировки четкости
         Contrast         :1,      // доступность регулировки контраста
         Brightness          :1;      // доступность регулировки яркости

} FEATURE_HI_REG, *PFEATURE_HI_REG;

//описываются поля LO-регистра настроек камеры 
typedef struct _FEATURE_LO_REG
{
   eseusbcam_unsigned_long_t               
       Reserved2         :12,
       AutoPeakAverage   :1,      // доступность управления балансом пик-среднее схемы авторегулирования
       AutoLevel          :1,      // доступность управления уровнем яркости схемы авторегулирования
       CaptureQuality      :1,      //не исп. 
         CaptureSize      :1,      //не исп.    
         Reserved1         :12,       
         OpticalFilter      :1,      // доступность управления сменой оптического фильтра
         Tilt            :1,      // доступность регулировки поворота камеры в вертикальной плоскости
         Pan            :1,      // доступность регулировки поворота камеры в горизонтальной плоскости
         Zoom             :1;      // доступность регулировки масштаба изображения  

} FEATURE_LO_REG, *PFEATURE_LO_REG;


//описываются поля информационных регистров настроек 
typedef struct _FEATURE_ELEMENTS_REG
{
   eseusbcam_unsigned_long_t               
       MaxValue         :12,      //максимальное значение для регулировки
       MinValue         :12,      //минимальное значение для регулировки
       ManualMode         :1,      //доступность ручного режима управления регулировки
         AutoMode         :1,      //доступность автоматического режима управления регулировки 
         OnOff            :1,      //доступность отключения управления регулировкой 
         ReadOut         :1,      //доступность чтения значения регулировки
         OnePush         :1,      //доступность однократного режима регулировки
         Reserved1         :2, 
         Presence          :1;      //доступность регулировки    

} FEATURE_ELEMENTS_REG, *PFEATURE_ELEMENTS_REG;

//описываются поля информационного регистра для функции SHUTTER
typedef struct _SHUTTER_INQ_REG
{
   eseusbcam_unsigned_long_t
      MaxValue : 16,      //максимальное значение для регулировки
      MinValue : 8,      //минимальное значение для регулировки
      ManualMode : 1,      //доступность ручного режима управления регулировки
      AutoMode : 1,      //доступность автоматического режима управления регулировки 
      OnOff : 1,      //доступность отключения управления регулировкой 
      ReadOut : 1,      //доступность чтения значения регулировки
      OnePush : 1,      //доступность однократного режима регулировки
      Reserved1 : 2,
      Presence : 1;      //доступность регулировки    

} SHUTTER_INQ_REG, *PSHUTTER_INQ_REG;

//описываются поля информационного регистра для функции Trigger 
typedef struct _TRIGGER_INQ_REG
{
   eseusbcam_unsigned_long_t               
       Reserved3         :12,
       TriggerMode3      :1,      //доступность режима триггера 3
       TriggerMode2      :1,      //доступность режима триггера 2
       TriggerMode1      :1,      //доступность режима триггера 1
       TriggerMode0      :1,      //доступность режима триггера 0 
       Reserved2         :9,
         Polarity         :1,      //доступность изменения полярности внешнего сигнала   
         OnOff            :1,      //доступность отключения управления регулировкой 
         ReadOut         :1,      //доступность чтения значения регулировки
         Reserved1         :3, 
         Presence          :1;      //доступность регулировки

} TRIGGER_INQ_REG, *PTRIGGER_INQ_REG;

//описываются поля регистра управления и состояния для функции Trigger 
typedef struct _TRIGGER_CTRL_REG{
   eseusbcam_unsigned_long_t
       Parameter         :12,
       Reserved3         :4,
       TriggerMode      :4,      //режим триггера
       Reserved2         :4,
         Polarity         :1,      //полярность внешнего сигнала  0 - активен переход 1->0; 1 - активен переход 0->1
         OnOff            :1,      //вкл/откл управления регулировкой 
         Reserved1      :5, 
         Presence         :1;      //доступность регулировки

} TRIGGER_CTRL_REG, *PTRIGGER_CTRL_REG;

//структура определяет параметры встроенной в камеру памяти
typedef struct _INTERNAL_MEM_INFO
{
   eseusbcam_unsigned_long_t MemoryType;         //Тип используемой памяти
   eseusbcam_unsigned_long_t MemStep;         //минимальная порция работы с памятью в байтах
   eseusbcam_unsigned_long_t FullMemSize;    //объем памяти в MemStep
   eseusbcam_unsigned_long_t Reserved_0;      
   eseusbcam_unsigned_long_t MinAlarmBuffSize;   //минимальный размер тревожного буфера в MemStep
   eseusbcam_unsigned_long_t Reserved_1;      
   eseusbcam_unsigned_long_t MaxAlarmBuffSize;   //максимальный размер тревожного буфера в MemStep
   eseusbcam_unsigned_long_t Reserved_2;      
   eseusbcam_unsigned_long_t MinContainerSize;   //минимальный размер буфера для ролика в MemStep
   eseusbcam_unsigned_long_t MaxContainerSize;   //максимальный размер буфера для ролика в MemStep
   eseusbcam_unsigned_long_t AlarmSizeStep;      //шаг изменения размера тревожного буфера в MemStep
   eseusbcam_unsigned_long_t ContainerSizeStep;   //шаг изменения размера буфера для ролика в MemStep   
   eseusbcam_unsigned_long_t CurrNumContainers;   //доступное число контейнеров от 1 до CurrNumContainers
   eseusbcam_unsigned_long_t CurrSizeContainer;  //текущий размер контейнера в MemStep
   eseusbcam_unsigned_long_t Reserved_3;
   eseusbcam_unsigned_long_t Reserved_4;

} INTERNAL_MEM_INFO, *PINTERNAL_MEM_INFO;

//структура содержит поля для работы с режимами видео камеры
typedef struct _IP_FORMATS_LIST
{

   char FormatName[48]; //текстовое описание режима
   unsigned char bitCountCamera;   //число бит на элемент изображения от камеры
   unsigned char bitCountBitMap;   //число бит на элемент изображения для отображения на ПК (напр., bitCountBitMap = 24 соответствует RGB24)
   unsigned char bitCountADC;   //число бит на элемент изображения    
   unsigned char bPartial;      //доступность изменения в режиме высоты, ширины, положения левого верхнего угла с шагами в UNIT_SIZE UnitSize
   unsigned short FilterID;      //
   unsigned short CodecID;      //
   unsigned short StepWidth;      //шаг изменения ширины кадра (в пикселах)
   unsigned short StepHeight;      //шаг изменения высоты кадра (в пикселах)      
   unsigned short MaxWidth;      //максимальный размер ширины изображения для режима (в пикселах)
   unsigned short MaxHeight;      //максимальный размер высоты изображения для режима (в пикселах)
   unsigned short MinWidth;      //минимальный размер ширины изображения для режима (в пикселах)
   unsigned short MinHeight;      //минимальный размер высоты изображения для режима (в пикселах)   
   unsigned short WidthFrame;      //текущая ширина изображения (в пикселах)   
   unsigned short HeightFrame;   //текущая высота изображения (в пикселах)
   unsigned short LeftFrame;      //левый верхний угол запрашиваемой части изображения (в пикселах)   
   unsigned short TopFrame;      //левый верхний угол запрашиваемой части изображения (в пикселах)
   unsigned short Reserved_0;      //
   unsigned short Reserved_1;      //
} IP_FORMATS_LIST, *PIP_FORMATS_LIST;

typedef struct _FORMAT_OPS
{
   unsigned char
      PresenceMirror : 1,      //доступность опции зеркалирования изображения     
      PresenceFlip   : 1,      //доступность опции переворота изображения
      PresenceFDG	   : 1,	    //доступность опции переключения высокого/низкого коэффициента предусиления
      Reserved0      : 5;

} FORMAT_OPS, *PFORMAT_OPS;
#define FLAG_IMAGE_OPS_MIRROR    1    //флаг зеркального изображения
#define FLAG_IMAGE_OPS_FLIP      2    //флаг переворота изображения
#define FLAG_IMAGE_OPS_FDGAIN    4    //флаг переключения высокого/низкого коэффициента предусиления

typedef struct _IP_FORMATS_INFO
{

   unsigned char numFormats;         //число режимов
   unsigned char CurrFormat;         //текущий режим из списка
   FORMAT_OPS    FormatOps;         //доступные опции для изображения 
   unsigned char Reserved_1;         //
   unsigned char Reserved_2;         //
   unsigned char Reserved_3;         //
   unsigned char Reserved_4;         //
   unsigned char Reserved_5;         //

   IP_FORMATS_LIST      formatsList[16];   //список доступных режимов

} IP_FORMATS_INFO, *PIP_FORMATS_INFO;

typedef struct _USB_CAMERA_CAPABILITES
{

   eseusbcam_unsigned_long_t CameraSerial;   // 
   unsigned short CameraSWVersion;// версия микропрограммы   
   unsigned short CameraFWVersion;// версия прошивки   
/**/
   eseusbcam_unsigned_long_t HiCameraMAC;   // 
   eseusbcam_unsigned_long_t LoCameraMAC;   // 
   eseusbcam_unsigned_long_t HiStaticIP;   // 
   eseusbcam_unsigned_long_t LoStaticIP;   // 

   unsigned short Stream0_port;
   unsigned short Stream0_packet_size;
   unsigned short SDHC_port;
   unsigned short Reserved_1;
   eseusbcam_unsigned_long_t Reserved_2;

   unsigned char PresetNumber;   // текущий пресет
   unsigned char Reserved_3;   //
   unsigned char Reserved_4;   //
   unsigned char Reserved_5;   //

    char ModelName[16];   // текстовое название модели
    char VendorName[16]; // имя производителя
    char UserDescription[32];// текстовое поле пользователя

    BASIC_FUNC_REG         BasicFunction;   // функциональные возможности камеры
   FEATURE_HI_REG         FeatureHi;      // доступные регулировки
   FEATURE_LO_REG         FeatureLo;      // доступные регулировки
   FEATURE_ELEMENTS_REG   Brightness;      // описание регистра управления яркостью
   FEATURE_ELEMENTS_REG   Contrast;      // описание регистра управления контрастом
   FEATURE_ELEMENTS_REG   Sharpness;      // описание регистра управления четкостью
   FEATURE_ELEMENTS_REG   WhiteBalance;   // описание регистра управления балансом белого
   SHUTTER_INQ_REG         FrameExposure;   // описание регистра управления кадровым накоплением
   FEATURE_ELEMENTS_REG   Saturation;      // описание регистра управления насыщенностью
   FEATURE_ELEMENTS_REG   Gamma;         // описание регистра управления гаммой
   SHUTTER_INQ_REG         Shutter;      // описание регистра управления электронным затвором
   FEATURE_ELEMENTS_REG   Gain;         // описание регистра управления усилением
   FEATURE_ELEMENTS_REG   Iris;         // описание регистра управления диафрагмой
   FEATURE_ELEMENTS_REG   Focus;         // описание регистра управления фокусировкой
   FEATURE_ELEMENTS_REG   Temperature;   // описание регистра управления температурой
   TRIGGER_INQ_REG         TriggerInq;      // описание регистра управления режимами работы с внешними сигналами синхронизации
   FEATURE_ELEMENTS_REG   Zoom;         // описание регистра управления масштабом
   FEATURE_ELEMENTS_REG   Pan;         // описание регистра управления горизонтальным поворотом
   FEATURE_ELEMENTS_REG   Tilt;         // описание регистра управления вертикальным поворотом
   FEATURE_ELEMENTS_REG   OpticalFilter;   // описание регистра управления светофильтрами

   FEATURE_ELEMENTS_REG   CompressionLevel;   // описание регистра управления степенью сжатия

   FEATURE_ELEMENTS_REG   AutoLevel;      // описание регистра управления уровнем яркости схемы авторегулирования
   FEATURE_ELEMENTS_REG   AutoPeakAverage;// описание регистра управления балансом пик-среднее схемы авторегулирования
   eseusbcam_unsigned_long_t ShutterStepTime; //длительность шага изменения электронного затвора в наносекундах
   eseusbcam_unsigned_long_t ShutterZeroTime; //постоянная составляющая электронного затвора в наносекундах
   eseusbcam_unsigned_long_t Reserved_10;

   INTERNAL_MEM_INFO      MemInfo;

   IP_FORMATS_INFO         CameraFormats;   // доступные режимы работы камеры

} USB_CAMERA_CAPABILITES, *PUSB_CAMERA_CAPABILITES;

//структура передает параметры для захвата с камеры
//последнего кадра
typedef struct _CAP_FRAME_INFO
{

   eseusbcam_unsigned_long_t inFlags;      //in
   eseusbcam_unsigned_long_t TimeOut;      //in таймаут ожидания кадра в мс
   eseusbcam_unsigned_long_t bDecode;      //1 - раскодировать исходный формат в DIB, 0-передать в исходном виде JPEG
   eseusbcam_unsigned_long_t Status;         //out
   eseusbcam_unsigned_long_t FrameWidth;      //out ширина принятого кадра
   eseusbcam_unsigned_long_t FrameHeight;   //out высота принятого кадра
   eseusbcam_unsigned_long_t FormatNumber;   //out
   eseusbcam_unsigned_long_t BitPerPixel;   //out число бит на пиксел в буфере (для отображения раскодированного изображения)
   eseusbcam_unsigned_long_t ulReserve1;
   eseusbcam_unsigned_long_t ulReserve2;
   eseusbcam_unsigned_long_t ulReserve3;
   eseusbcam_unsigned_long_t ulReserve4;
   eseusbcam_unsigned_long_t ulReserve5;
   eseusbcam_unsigned_long_t TimeCntRef;      //out значение временной метки кадра в частоте 90кГц
   eseusbcam_unsigned_long_t FrameNumRef;   //out порядковый номер кадра камеры
   eseusbcam_unsigned_long_t DataSize;      //out размер возвращаемого буфера с данными
   void*         pReserved;
   void*         pData;         //out - возвращается указатель на буфер с данными, данные сохраняются до следующего вызова Net_CaptureStreamFrame

} CAP_FRAME_INFO, *PCAP_FRAME_INFO;

#define MAX_BUFFERS      32   

//структура используется для передачи драйверу буферов для приема необработанных данных
//используется для прямого доступа к потоку данных. Необходимый размер буферов определяется высотой, шириной кадра, 
//полем bitCountCamera (FORMATS_LIST_D) текущего режима
typedef struct _DIRECT_BUFFER_ARRAY{

   unsigned char*   pCurrentBuffer;         //указатель на текущий приемный буфер (OUT)
   unsigned char   BufferCnt;            //число буферов
   eseusbcam_unsigned_long_t ulReserve0;
   eseusbcam_unsigned_long_t ulReserve1;
   eseusbcam_unsigned_long_t ulReserve2;
   eseusbcam_unsigned_long_t BufferSize; // размер каждого буфера из pBuffer в байтах
   unsigned char*   pBuffer[MAX_BUFFERS];   //буферы

} DIRECT_BUFFER_ARRAY, *PDIRECT_BUFFER_ARRAY;

//структура передает параметры для начала трансляции видео в режиме реального времени
//с возможностью синхронизации
typedef struct _VIDEO_STREAM_PARAM_EX{

   void*            pBuffBitmap;      //указатель на буфер для приема обработанного изображения (размер - width*height*bitCountBitMap)
   void*            pEvent;            //указатель на элемент синхронизации "Event". Устанавливается в библиотеке по готовности кадра в pBuffBitmap. Сбрасывается пользователем после обработки pBuffBitmap. Пока Event не сброшен, новые данные в pBuffBitmap не пишутся
   void*            pDirectBuffer;      //указатель на структуру DIRECT_BUFFER_ARRAY. Если NULL, то используются внутренние буферы.
   void**         ppReturnedParams;   //не исп.
   eseusbcam_unsigned_long_t            ulMapdataBuffSize;
   eseusbcam_unsigned_long_t            ulReserve1;
   eseusbcam_unsigned_long_t            ulReserve2;
   eseusbcam_unsigned_long_t            ulReserve3;

} VIDEO_STREAM_PARAM_EX, *PVIDEO_STREAM_PARAM_EX;

//*************** for callback ****************************************************************

// Event IDs:   stream events
#define NEW_FRAME                  100      //получен новый кадр
#define NEW_FRAME_SYNCHRO_ERROR   101      //ошибка синхронизации при получении кадра, но поток продолжает работать
#define INCORRECT_INPUT_PARAMETR   102      //неправильные входные параметры
#define INTERNAL_CAMERA_ERROR      103      //ошибка камеры
#define USB_TRANSFER_ERROR         104      //ошибка передачи потока

//тип функции обратного вызова при получении кадра изображения
//
typedef void  (*PCOMPLETE_ROUTINE) (const char* szCameraName,
                           int pEventID,
                           int pEventDataSize,
                           void* pEventData,
                           unsigned char* pFrame,
                           void* pUserData);

//*************** for stream ****************************************************************
typedef struct _GNSS_TIME_ITK4_0{

   unsigned char      chYear;
   unsigned char      chMonth;
   unsigned char      chDay;
   unsigned char      chHour;
   unsigned char      chMinute;
   unsigned char      chSecond;
   unsigned char      chFlags;
   unsigned char      chGnssType;
   eseusbcam_unsigned_long_t      ulRefClockPeriod;//ps
   eseusbcam_unsigned_long_t      ulPartSecondExposureStart;// in ulRefClockPeriod
   eseusbcam_unsigned_long_t      ulPartSecondExposureEnd;// in ulRefClockPeriod
   eseusbcam_unsigned_long_t      ulReserved0;
   eseusbcam_unsigned_long_t      ulReserved1;
   eseusbcam_unsigned_long_t      ulReserved2;
} GNSS_TIME_ITK4_0, *PGNSS_TIME_ITK4_0;//32 byte

typedef struct _STREAM_SERV_DATA_ITK4_0{
   eseusbcam_unsigned_long_t            ulServDataType;//3
   unsigned short            usWidth;
   unsigned short            usHeight;
   unsigned short            usLeft;
   unsigned short            usTop;
   unsigned char            chBitPerPixel;
   unsigned char            chColorCoding;
   unsigned char            chRes0;
   unsigned char            chCurrGamma;
   eseusbcam_unsigned_long_t            ulCurrShutter;
   unsigned short            usRes0;
   unsigned short            usCurrGain;
   unsigned short            usCurrIris;
   unsigned short            usCurrBrightness;
   unsigned short            usCurrSharpness;
   unsigned short            usCurrTemperature; // Note: it must be interpreted as char for usage
   eseusbcam_unsigned_long_t            ulFlags;
   eseusbcam_unsigned_long_t            ulFrameNum;
   GNSS_TIME_ITK4_0   gnssExposure;
   eseusbcam_unsigned_long_t            ulNTP_SecondExposureStart;
   eseusbcam_unsigned_long_t            ulNTP_PartSecondExposureStart;
   eseusbcam_unsigned_long_t            ulNTP_SecondExposureEnd;
   eseusbcam_unsigned_long_t            ulNTP_PartSecondExposureEnd;
   eseusbcam_unsigned_long_t            ulReserved[7];
   unsigned char            chRes1;
   unsigned char            chRadarMsgCount;
   unsigned short            usRadarDataBytes;
   unsigned char            RadarData[1600];
} STREAM_SERV_DATA_ITK4_0, *PSTREAM_SERV_DATA_ITK4_0;//1720 byte

typedef struct _STREAM_SERV_DATA_TYPE_1
{
   eseusbcam_unsigned_long_t ulServDataType; //1
   eseusbcam_unsigned_long_t ulCurrExposure;
   eseusbcam_unsigned_long_t ulCurrGain;
   eseusbcam_unsigned_long_t ulCurrBrightness;
   eseusbcam_unsigned_long_t ulCurrTemperature;
   eseusbcam_unsigned_long_t ulDelay1;
   eseusbcam_unsigned_long_t ulDelay2;
   eseusbcam_unsigned_long_t ulRefPeriod;
   eseusbcam_unsigned_long_t ulReserved[8];
   unsigned char ExtRAM[512];
} STREAM_SERV_DATA_TYPE_1, *PSTREAM_SERV_DATA_TYPE_1;

typedef struct _STREAM_SERV_DATA_TYPE_2
{
   eseusbcam_unsigned_long_t ulServDataType; //2
   unsigned short ulWidth;
   unsigned short ulHeight;
   unsigned char chBitPerPixel;
   unsigned char chColorCoding;
   unsigned char chRes0;
   unsigned char chCurrGamma;
   eseusbcam_unsigned_long_t ulCurrShutter;
   unsigned short ulCurrFrameExposure;
   unsigned short ulCurrGain;
   unsigned short ulCurrIris;
   unsigned short ulCurrBrightness;
   unsigned short ulCurrSharpness;
   unsigned short ulCurrTemperature;
   eseusbcam_unsigned_long_t ulFlags;
   eseusbcam_unsigned_long_t ulFrameNum;
   eseusbcam_unsigned_long_t ulFrameTime90kHz;
   eseusbcam_unsigned_long_t ulReserved[10];
} STREAM_SERV_DATA_TYPE_2, *PSTREAM_SERV_DATA_TYPE_2;

typedef struct _PIXEL_PARAM
{
   unsigned short usAmplitude;
   unsigned short ucX_coord;
   unsigned short ucY_coord;
} PIXEL_PARAM, *PPIXEL_PARAM;

typedef struct _STREAM_SERV_DATA_BRZ_5012HM_GE_V1
{
   eseusbcam_unsigned_long_t ulServDataType;    //4
   eseusbcam_unsigned_long_t ulSensFrameNumber; //считанный с матрицы
   eseusbcam_unsigned_long_t ulNetFrameNumber;  //отправленный в сеть
   unsigned short usCurrGain;
   unsigned short usCurrShutter;
   unsigned short usLeft;
   unsigned short usTop;
   unsigned short usWidth;
   unsigned short usHeight;
   unsigned short usShutterZeroTime;
   unsigned short usShutterStepTime;
   eseusbcam_unsigned_long_t ulNTP_SecondExposureStart;
   eseusbcam_unsigned_long_t ulNTP_PartSecondExposureStart;
   unsigned char chMode;
   unsigned char chBitPerPixel;
   unsigned short usPixThreshold;
   unsigned short usCurrTemperature; // Note: it must be interpreted as char for usage
   unsigned short usCurrBrightness;

   eseusbcam_unsigned_long_t ulReserved[6];

   eseusbcam_unsigned_long_t ulPixelsNumber;
   unsigned short usXc;
   unsigned short chXc_Remain;
   unsigned short usYc;
   unsigned short chYc_Remain;

   PIXEL_PARAM PixelsData[512];
} STREAM_SERV_DATA_BRZ_5012HM_GE_V1, *PSTREAM_SERV_DATA_BRZ_5012HM_GE_V1; //  3152 byte

typedef struct _RET_SERV_DATA_HEADER{
   eseusbcam_unsigned_long_t      ulNextDataSize;// 
   eseusbcam_unsigned_long_t      ulServDataType;
} RET_SERV_DATA_HEADER, *PRET_SERV_DATA_HEADER;

typedef struct _RET_SERV_DATA_ITK4_0{
   eseusbcam_unsigned_long_t                  ulNextDataSize;// 
   STREAM_SERV_DATA_ITK4_0      StreamServData;
} RET_SERV_DATA_ITK4_0, *PRET_SERV_DATA_ITK4_0;

typedef struct _RET_SERV_DATA_TYPE_1
{
   eseusbcam_unsigned_long_t ulNextDataSize; //
   STREAM_SERV_DATA_TYPE_1 StreamServData;
} RET_SERV_DATA_TYPE_1, *PRET_SERV_DATA_TYPE_1;

typedef struct _RET_SERV_DATA_TYPE_2
{
   eseusbcam_unsigned_long_t ulNextDataSize; //
   STREAM_SERV_DATA_TYPE_2 StreamServData;
} RET_SERV_DATA_TYPE_2, *PRET_SERV_DATA_TYPE_2;

typedef struct _RET_SERV_DATA_BRZ_5012HM_GE_V1
{
   eseusbcam_unsigned_long_t ulNextDataSize; //
   STREAM_SERV_DATA_BRZ_5012HM_GE_V1 StreamServData;
} RET_SERV_DATA_BRZ_5012HM_GE_V1, *PRET_SERV_DATA_BRZ_5012HM_GE_V1;

typedef struct _USB_CAMERA_STATE
{

   eseusbcam_unsigned_long_t ulCurrentPreset;   
   eseusbcam_unsigned_long_t ulPresetsNum;
   eseusbcam_unsigned_long_t ulCurrentLeft;
   eseusbcam_unsigned_long_t ulCurrentTop;
   eseusbcam_unsigned_long_t ulCurrentWidth;
   eseusbcam_unsigned_long_t ulCurrentHeight;
   eseusbcam_unsigned_long_t ulCurrentShutter;
   eseusbcam_unsigned_long_t ulCurrentShutterZeroTime;
   eseusbcam_unsigned_long_t ulCurrentShutterStepTime;
   eseusbcam_unsigned_long_t ulCurrentBrightness;
   eseusbcam_unsigned_long_t ulCurrentGain;
   eseusbcam_unsigned_long_t ulCurrentRed;
   eseusbcam_unsigned_long_t ulCurrentGreen;
   eseusbcam_unsigned_long_t ulCurrentBlue;
   eseusbcam_unsigned_long_t ulCurrentGamma;
   eseusbcam_unsigned_long_t ulCurrenntSaturation;
   eseusbcam_unsigned_long_t ulCurrentBits;
   eseusbcam_unsigned_long_t ulCurrentFormat;
   eseusbcam_unsigned_long_t ulCurrentTriggerType;
   eseusbcam_unsigned_long_t ulCurrentOutpuStates;
   eseusbcam_unsigned_long_t ulCurrentFrameExposure;
   eseusbcam_unsigned_long_t ulCurrentSharpness;
   eseusbcam_unsigned_long_t ulCurrentContrast;
   eseusbcam_unsigned_long_t ulCurrentIris;
   eseusbcam_unsigned_long_t ulCurrentPan;
   eseusbcam_unsigned_long_t ulCurrentTilt;
   eseusbcam_unsigned_long_t ulCurrentHue;
   eseusbcam_unsigned_long_t ulCurrentFocus;
   eseusbcam_unsigned_long_t ulCurrentZoom;
   eseusbcam_unsigned_long_t ulCurrentOpticalFilter;
   eseusbcam_unsigned_long_t ulCurrentAutoPeakAvrg;
   eseusbcam_unsigned_long_t ulCurrentAutoStatus;
   eseusbcam_unsigned_long_t ulCurrentAutoLevel;
   eseusbcam_unsigned_long_t ulCurrentET0;
   eseusbcam_unsigned_long_t ulCurrentET1;
   eseusbcam_unsigned_long_t ulCurrentET2;
   eseusbcam_unsigned_long_t ulCurrentET3;
   eseusbcam_unsigned_long_t ulCurrentDST0;
   eseusbcam_unsigned_long_t ulCurrentDST1;
   eseusbcam_unsigned_long_t ulCurrentDST2;
   eseusbcam_unsigned_long_t ulCurrentDST3;
   eseusbcam_unsigned_long_t ulCurrentBlkLeft;
   eseusbcam_unsigned_long_t ulCurrentBlkTop;
   eseusbcam_unsigned_long_t ulCurrentBlkRight;
   eseusbcam_unsigned_long_t ulCurrentBlkBottom;
   SHUTTER_INQ_REG ulCurrentShutterInq;
   SHUTTER_INQ_REG ulCurrentFrameExposureInq;
   FEATURE_ELEMENTS_REG ulCurrentIrisInq;
   eseusbcam_unsigned_long_t ulSerialNum;
   eseusbcam_unsigned_long_t ulVendorID;
   eseusbcam_unsigned_long_t ulProductID;

} USB_CAMERA_STATE, *PUSB_CAMERA_STATE;

typedef struct _U3_ESECAM_START_MARKER
{
   eseusbcam_unsigned_long_t   ulRes0;
   eseusbcam_unsigned_long_t   ulMarkerSize;
   eseusbcam_unsigned_long_t   ulRes1;
   eseusbcam_unsigned_long_t   ulCameraSerial;
   eseusbcam_unsigned_long_t   ulRes2;
   eseusbcam_unsigned_long_t   ulFrameNumber;
   eseusbcam_unsigned_long_t   ulRes3;
   eseusbcam_unsigned_long_t   ulTimeStampLo;
   eseusbcam_unsigned_long_t   ulTimeStampHi;
   eseusbcam_unsigned_long_t   ulRes4;
   eseusbcam_unsigned_long_t   ulTimeStampRefClk;
   eseusbcam_unsigned_long_t   ulRes5;
   eseusbcam_unsigned_long_t   ulServDataSize;
   eseusbcam_unsigned_long_t   ulServDataType;
   eseusbcam_unsigned_long_t   ulFrameWidth;
   eseusbcam_unsigned_long_t   ulFrameHeight;
   unsigned char   chColorCoding;
   unsigned char   chBitPerPixelADC;
   unsigned char   chBitPerPixelStream;
   unsigned char   chRes0;
} U3_ESECAM_START_MARKER, *PU3_ESECAM_START_MARKER;

typedef struct _U3_ESECAM_END_MARKER
{
   eseusbcam_unsigned_long_t   ulRes0;
   eseusbcam_unsigned_long_t   ulMarkerSize;
   eseusbcam_unsigned_long_t   ulRes1;
   eseusbcam_unsigned_long_t   ulCameraSerial;
   eseusbcam_unsigned_long_t   ulRes2;
   eseusbcam_unsigned_long_t   ulFrameNumber;
   eseusbcam_unsigned_long_t   ulRes3;
   eseusbcam_unsigned_long_t   ulTimeStampLo;
   eseusbcam_unsigned_long_t   ulTimeStampHi;
   eseusbcam_unsigned_long_t   ulRes4;
   eseusbcam_unsigned_long_t   ulTimeStampRefClk;
   eseusbcam_unsigned_long_t   ulRes5;
   eseusbcam_unsigned_long_t   ulServDataSize;
   eseusbcam_unsigned_long_t   ulServDataType;
   eseusbcam_unsigned_long_t   ulImageDataSize;
} U3_ESECAM_END_MARKER, *PU3_ESECAM_END_MARKER;

//*******************************************************************************
//прототипы функций
//*******************************************************************************
//Коды возврата:
//0 - ошибка (можно получить информацию через USB_GetLastError)
//1 - выполнено
//2 - есть предупреждения (можно получить информацию через USB_GetLastError)

//функция читает список доступных устройств
DCAMAPI
eseusbcam_unsigned_long_t 
USB_GetCameraList(
           PDEVICE_DATA    DeviceData      //указатель на структуру DEVICE_DATA, в которую записывается список устройств
           );

//функция устанавливает заданное значение регистров управления и состояния камеры
DCAMAPI
eseusbcam_unsigned_long_t
USB_SetCameraFeature(
          const char* szCameraName,   //имя устройства
          eseusbcam_unsigned_long_t FeatureID,      //код регистра управления и состояния камеры   
          eseusbcam_unsigned_long_t FeatureValue   //новое значение   
         );

//функция читает текущее значение регистров управления и состояния камеры
DCAMAPI
eseusbcam_unsigned_long_t
USB_GetCameraFeature(
          const char* szCameraName,   //имя устройства
          eseusbcam_unsigned_long_t FeatureID,      //код регистра управления и состояния камеры        
          eseusbcam_unsigned_long_t *pFeatureValue   //указатель на переменную для возврата значения регистра         
         );

DCAMAPI
eseusbcam_unsigned_long_t
USB_GetAutoFeature(
          const char* szCameraName,
          eseusbcam_unsigned_long_t FeatureID,         
          eseusbcam_unsigned_long_t *pFlagValue      
         );

DCAMAPI
eseusbcam_unsigned_long_t
USB_SetAutoFeature(
          const char* szCameraName,
          eseusbcam_unsigned_long_t FeatureID,         
          eseusbcam_unsigned_long_t FlagValue      
         );

//функция запускает видеопоток с использованием функции обратного вызова для информирования о готовности нового кадра
DCAMAPI
eseusbcam_unsigned_long_t
USB_VideoOnCallbackStart(
             const char* szCameraName,            //имя устройства из CAMERA_DATA
            PVIDEO_STREAM_PARAM_EX      pVideoStreamParamEx,      //параметры запуска видеопотока
            PCOMPLETE_ROUTINE         pCompleteCallBackRoutine,   //функция обратного вызова для события готовности нового кадра в буфере pVideoStreamParamEx->pBuffBitmap
            void*                  pUserData               //указатель на данные пользователя, возвращаемый в параметрах pCompleteCallBackRoutine() при вызове
             );

/// возвращаемое значение 0 - ошибка, положительное значение - размер буфера
DCAMAPI
eseusbcam_unsigned_long_t
USB_GetRequiredDirectBufferSize(const char* szCameraName);

/// возвращаемое значение 0 - ошибка, положительное значение - размер буфера
// Deprecated, use USB_GetRequiredDirectBufferSize instead
DCAMAPI
eseusbcam_unsigned_long_t
USB_GetRequiredBufferSize(const char* szCameraName);

//функция останавливает видеопоток, запущенный функциями StartVideoStream, StartVideoStreamEx, VideoOnCallbackStart
DCAMAPI
eseusbcam_unsigned_long_t
USB_StopVideoStream(
             const char* szCameraName      //имя устройства
             );

DCAMAPI
eseusbcam_unsigned_long_t
USB_GetCameraCapabilites(
                  const char* szCameraName,
                  PUSB_CAMERA_CAPABILITES* ppCameraCapabilites
                  );

//функция сохраняет текущий набор регулировок и параметров камеры в профиль настроек (пресет) с заданным номером
DCAMAPI
eseusbcam_unsigned_long_t
USB_SaveParams(
          const char* szCameraName,   //имя устройства
          eseusbcam_unsigned_long_t PresetNumber   //номер сохраняемого профиля настроек из диапазона от 1 до CAMERA_INQ_PARAM.BasicFunction.MemoryChannel. 
                                 //Профиль 0 (производственные настройки) недоступен для сохранения настроек.
                                 //При инициализации камеры (InitCamera) все параметры загружаются с последнего сохраненного профиля
         );

//функция устанавливает требуемую зону сканирования для режима, который поддерживает такую возможность 
//(FORMATS_LIST_D.bPartial == TRUE). Значения LeftOffset и Width должны в сумме находиться в диапазоне 
//FORMATS_LIST_D.MaxImageSize.Hmax и быть кратны FORMATS_LIST_D.UnitSize.Hunit,а значения TopOffset и
//Height должны в сумме находиться в диапазоне FORMATS_LIST_D.MaxImageSize.Vmax и 
//быть кратны FORMATS_LIST_D.UnitSize.Vunit. 
//Видеопоток должен быть предварительно остановлен.
DCAMAPI
eseusbcam_unsigned_long_t
USB_SetScanArea(
         const char* szCameraName,             //имя устройства
         eseusbcam_unsigned_long_t LeftOffset,//сдвиг слева
         eseusbcam_unsigned_long_t TopOffset, //сдвиг сверху
         eseusbcam_unsigned_long_t Width,       //ширина
         eseusbcam_unsigned_long_t Height       //высота
         );

DCAMAPI
eseusbcam_unsigned_long_t
USB_SetFormat(
         const char* szCameraName,
         eseusbcam_unsigned_long_t FormatNumber
         );

//функция производит загрузку служебных таблиц в камеру.
DCAMAPI
eseusbcam_unsigned_long_t
USB_SendTable(const char* szCameraName,   //имя устройства
              eseusbcam_unsigned_long_t TableID,      //тип таблицы
              eseusbcam_unsigned_long_t TableSize,   //размер таблицы
              const unsigned char* pTable      //указатель на таблицу
             );

//функция производит получение служебных таблиц из камеру.
DCAMAPI
eseusbcam_unsigned_long_t
USB_ReceiveTable(const char* szCameraName,   //имя устройства
              eseusbcam_unsigned_long_t TableID,      //тип таблицы
              eseusbcam_unsigned_long_t TableSize,   //размер таблицы
              unsigned char* pTable      //указатель на таблицу
             );

// функция возвращает описание последней возникшей ошибки
DCAMAPI
const char*
USB_GetLastError();

#ifdef __cplusplus
}
#endif

#endif // _ESEUSBCAM_H_
