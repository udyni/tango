/*
 * Copyright 2014 Michele Devetta
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * All the information contained in this file has been retrieved from:
 *
 * AS-5216-DLL User's Manual v1.3.0.0 (March 2007) (c) Avantes B.V.
 * AvaSpec AS-5216 OEM Manual (May 2006) (c) Avantes B.V.
 * USB low level demo code from Avantes
 *
 */

#ifndef AVASPEC_H
#define AVASPEC_H

// Include only from AVANTES.H
#ifndef AVANTES_H
#error AvaSpec.h should be included only from Avantes.h
#else

// AvaSpec message types
#define PC2S_TYPE        0x20
#define S2PC_TYPE        0x21
#define RESP_TYPE        0x01

// AvaSpec commands from PC to spectrometer
#define CMD_GETCONF      0x01
#define CMD_SETCONF      0x02
#define CMD_PREPMEAS     0x05
#define CMD_STARTMEAS    0x06
#define CMD_GETMEAS      0x07
#define CMD_GETAVG       0x08
#define CMD_GETDIGIN     0x0A
#define CMD_SETDIGOUT    0x0B
#define CMD_GETANAIN     0x0C
#define CMD_SETANAOUT    0x0D
#define CMD_GETVERSION   0x0E
#define CMD_STOPMEAS     0x0F
#define CMD_SETPWM       0x10
#define CMD_SPECMEAS     0x11
#define CMD_GETRES       0x12
#define CMD_GETID        0x13
#define CMD_SETSYNC      0x19
#define CMD_SETTRIG      0x1A

// AvaSpec responses to commands from PC
// NOTE: responses code are commands + 0x80
#define RSP_OFFSET       0x80
#define RSP_ERROR        0x00
#define RSP_GETCONF      0x81
#define RSP_SETCONF      0x82
#define RSP_PREPMEAS     0x85
#define RSP_STARTMEAS    0x86
#define RSP_GETMEAS      0x87
#define RSP_GETAVG       0x88
#define RSP_GETDIGIN     0x8A
#define RSP_SETDIGOUT    0x8B
#define RSP_GETANAIN     0x8C
#define RSP_SETANAOUT    0x8D
#define RSP_GETVERSION   0x8E
#define RSP_STOPMEAS     0x8F
#define RSP_SETPWM       0x90
#define RSP_SPECMEAS     0x91
#define RSP_GETRES       0x92
#define RSP_GETID        0x93
#define RSP_SETSYNC      0x99
#define RSP_SETTRIG      0x9A

// AvaSpec commands from spectrometer to PC
#define CMD_MEASDATA     0xB0
#define CMD_AVGDATA      0xB1
#define CMD_MEASRAM      0xB2
#define CMD_AVGRAM       0xB3
#define CMD_MEASERR      0xB5

// Acknowledge of unsolicited message from spectrometer to PC
#define RSP_ACK          0xC0


/*
 *  AvaSpec command structures
 */

//! Constants
#define AVS_SERIAL_LEN       9    // On test program was 10, but documentation says 9.
#define USER_ID_LEN          64

#define MAX_TEMP_SENSORS     3
#define MAX_VIDEO_CHANNELS   2
#define MAX_NR_PIXELS        4096

#define NR_WAVELEN_POL_COEF  5
#define NR_NONLIN_POL_COEF   8
#define NR_DEFECTIVE_PIXELS  30
#define NR_TEMP_POL_COEF     5
#define NR_DAC_POL_COEF      2

#define ROOT_NAME_LEN        6

#define MAX_INTEGRATION_TIME 600000  // In milliseconds, max. 600 seconds
#define MAX_PIXEL_VALUE      0xFFFC

// Sensor types
#define SENS_HAMS8378_256   0
#define SENS_HAMS8378_1024  1
#define SENS_ILX554         2
#define SENS_HAMS9201       3
#define SENS_TCD1304        4
#define SENS_TSL1301        5
#define SENS_TSL1401        6
#define SENS_HAMS9840       7

// Sensors minimum integration times in us
#define SENS_HAMS8378_256_MININT    560
#define SENS_HAMS8378_1024_MININT  2200
#define SENS_ILX554_MININT         1100
#define SENS_HAMS9201_MININT        520
#define SENS_TCD1304_MININT          10
#define SENS_TSL1301_MININT          60
#define SENS_TSL1401_MININT          60
#define SENS_HAMS9840_MININT       2240


//! Reason of error response
typedef enum {
  ERR_UNKNOWN,
  ERR_INV_PARAM,
  ERR_INV_PASSWD,
  ERR_INV_CMD,
  ERR_INV_SIZE,
  ERR_MEAS_PENDING,
  ERR_INV_RANGE,
  ERR_INV_TIME,
  ERR_NOT_SUPP,
  ERR_INV_COMB,
  ERR_NO_BUFFER,
  ERR_NO_SPECTRA,
  ERR_INV_STATE,
  ERR_UNEXP_INT,
  ERR_INV_FPGA
} ErrorReason;

//! Spectrometer identification
#define AVSIDENTITY_LEN (int(sizeof(uint32_t)*3 + sizeof(char) * (AVS_SERIAL_LEN + USER_ID_LEN) + sizeof(uint8_t)))
typedef struct {
  uint32_t m_as5216_version;
  uint32_t m_fpga_version;
  uint32_t m_hw_version;
  char     m_SerialId[AVS_SERIAL_LEN];
  char     m_UserId[USER_ID_LEN];
  uint8_t  Status;
} AvsIdentityType;

//! Dark correction
#define DARKCORRECTION_LEN (int(sizeof(uint8_t) * 2))
typedef struct {
  uint8_t  m_Enable;
  uint8_t  m_ForgetPercentage;
} DarkCorrectionType;

//! Smoothing
#define SMOOTHING_LEN (int(sizeof(uint16_t) + sizeof(uint8_t)))
typedef struct {
  uint16_t  m_SmoothPix;
  uint8_t   m_SmoothModel;
} SmoothingType;

//! Trigger
#define TRIGGER_LEN (int(sizeof(uint8_t) * 3))
typedef struct {
  uint8_t  m_Mode;
  uint8_t  m_Source;
  uint8_t  m_SourceType;
} TriggerType;

typedef enum {
  TRIGGER_SW,   // Software trigger (0)
  TRIGGER_HW    // External hardware trigger (1)
} TriggerMode;

typedef enum {
  TRIGGER_EXT,  // External trigger input
  TRIGGER_SYNC  // Sync input
} TriggerSource;

typedef enum {
  TRIGGER_EDGE,  // Edge trigger
  TRIGGER_LEVEL  // Level trigger (measure averaged until the signal goes low)
} TriggerLevel;

//! Control settings
#define CONTROLSETTINGS_LEN (int(sizeof(uint16_t) * 2 + sizeof(uint32_t) * 2 + sizeof(float)))
typedef struct {
  uint16_t  m_StrobeControl;
  uint32_t  m_LaserDelay;
  uint32_t  m_LaserWidth;
  float     m_LaserWaveLength;
  uint16_t  m_StoreToRam;
} ControlSettingsType;

//! Measurement configuration
#define MEASCONFIG_LEN (int(sizeof(uint16_t) * 2 + sizeof(float) + sizeof(uint32_t) * 2 + DARKCORRECTION_LEN + SMOOTHING_LEN + sizeof(uint8_t) + TRIGGER_LEN + CONTROLSETTINGS_LEN))
typedef struct {
  uint16_t            m_StartPixel;
  uint16_t            m_StopPixel;
  float               m_IntegrationTime;
  uint32_t            m_IntegrationDelay;
  uint32_t            m_NrAverages;
  DarkCorrectionType  m_CorDynDark;
  SmoothingType       m_Smoothing;
  uint8_t             m_SaturationDetection;
  TriggerType         m_Trigger;
  ControlSettingsType m_Control;
} MeasConfigType;

typedef enum {
  SAT_DISABLE,
  SAT_ENABLE,
  SAT_PEAK_INVERSION
} SaturationType;

//! Detector type
#define DETECTOR_LEN (int(sizeof(uint8_t) + sizeof(uint16_t) * (1 + NR_DEFECTIVE_PIXELS) + \
                          sizeof(float) * (NR_WAVELEN_POL_COEF + 2 * MAX_VIDEO_CHANNELS + 2) + \
                          sizeof(bool) + sizeof(double) * (NR_NONLIN_POL_COEF + 2)))
typedef struct {
  uint8_t    m_SensorType;
  uint16_t   m_NrPixels;
  float      m_aFit[NR_WAVELEN_POL_COEF];
  bool       m_NLEnable;
  double     m_aNLCorrect[NR_NONLIN_POL_COEF];
  double     m_aLowNLCounts;
  double     m_aHighNLCounts;
  float      m_Gain[MAX_VIDEO_CHANNELS];
  float      m_Reserved;
  float      m_Offset[MAX_VIDEO_CHANNELS];
  float      m_ExtOffset;
  uint16_t   m_DefectivePixels[NR_DEFECTIVE_PIXELS];
} DetectorType;

//! Spectrum calibration
#define SPECTRUMCALIBRATION_LEN (int(SMOOTHING_LEN + sizeof(float) * (1 + MAX_NR_PIXELS)))
typedef struct {
  SmoothingType   m_Smoothing;
  float           m_CalInttime;
  float           m_aCalibConvers[MAX_NR_PIXELS];
} SpectrumCalibrationType;

//! Irradiance
#define IRRADIANCE_LEN (int(SPECTRUMCALIBRATION_LEN + sizeof(uint8_t) + sizeof(uint32_t)))
typedef struct {
  SpectrumCalibrationType m_IntensityCalib;
  uint8_t                 m_CalibrationType;
  uint32_t                m_FiberDiameter;
} IrradianceType;

//! Spectrum correction
#define SPECTRUMCORRECTION_LEN (int(sizeof(float) * MAX_NR_PIXELS))
typedef struct {
  float m_aSpectrumCorrect[MAX_NR_PIXELS];
} SpectrumCorrectionType;

//! Timestamp
#define TIMESTAMP_LEN (int(sizeof(uint16_t) * 2))
typedef struct {
  uint16_t  m_Date;
  uint16_t  m_Time;
} TimeStampType;

//! SD Card
#define SDCARD_LEN (int(sizeof(bool) + sizeof(uint8_t) + sizeof(char) * ROOT_NAME_LEN + TIMESTAMP_LEN))
typedef struct {
  bool           m_Enable;
  uint8_t        m_SpectrumType;
  char           m_aFileRootName[ROOT_NAME_LEN];
  TimeStampType  m_TimeStamp;
} SDCardType;

//! Parameters to run the spectrometer stand-alone
#define STANDALONE_LEN (int(sizeof(bool) + MEASCONFIG_LEN + sizeof(int16_t) + SDCARD_LEN))
typedef struct {
  bool            m_Enable;
  MeasConfigType  m_Meas;
  int16_t         m_Nmsr;
  SDCardType      m_SDCard;
} StandAloneType;

//! Temperature sensor calibration
#define TEMPSENSOR_LEN (int(sizeof(float) * NR_TEMP_POL_COEF))
typedef struct {
  float m_aFit[NR_TEMP_POL_COEF];
} TempSensorType;

//! TEC control parameters
#define TECCONTROL_LEN (int(sizeof(bool) + sizeof(float) * (1 + NR_DAC_POL_COEF)))
typedef struct {
  bool   m_Enable;
  float  m_Setpoint;     // [degree Celsius]
  float  m_aFit[NR_DAC_POL_COEF];
} TecControlType;

//! Process control
#define PROCESSCONTROL_LEN (int(sizeof(float) * 24))
typedef struct {
  float AnalogLow[2];
  float AnalogHigh[2];
  float DigitalLow[10];
  float DigitalHigh[10];
} ProcessControlType;

//! Device configuration
#define AVASPEC_DEVCONF_SIZE 63484
#define DEVICECONFIG_LEN (int(sizeof(uint16_t) * 2 + sizeof(char) * USER_ID_LEN + DETECTOR_LEN + IRRADIANCE_LEN + SPECTRUMCALIBRATION_LEN + SPECTRUMCORRECTION_LEN + \
                              STANDALONE_LEN + TEMPSENSOR_LEN * MAX_TEMP_SENSORS + TECCONTROL_LEN + PROCESSCONTROL_LEN))
typedef struct {
  uint16_t                 m_Len;
  uint16_t                 m_ConfigVersion;
  char                     m_aUserFriendlyId[USER_ID_LEN];
  DetectorType             m_Detector;
  IrradianceType           m_Irradiance;
  SpectrumCalibrationType  m_Reflectance;
  SpectrumCorrectionType   m_SpectrumCorrect;
  StandAloneType           m_StandAlone;
  TempSensorType           m_aTemperature[MAX_TEMP_SENSORS];
  TecControlType           m_TecControl;
  ProcessControlType       m_ProcessControl;
} DeviceConfigType;

#endif
#endif
