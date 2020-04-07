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
 */

#ifndef AVANTES_H
#define AVANTES_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <queue>
#include <stdint.h>
#include <BaseSpectrometer.h>

#include <byteswap.h>
#include <libusb-1.0/libusb.h>
#include <omnithread.h>

// Private header
#include "avaspec.h"


// Forward declaration of AvaSpec classes
class AvaSpec;
class AvaSpecUSBTransceiver;


// Vendor and product ID of AvaSpec spectrometers
#define USB_VENDOR           0x1992  // Avantes
#define USB_PRODUCT          0x0667  // AS5216
#define USB_INTERFACE        0x00    // USB interface
#define USB_OUT_EP           0x02    // Output endpoint (towards spectrometer)
#define USB_IN_EP            0x86    // Input endpoint  (from spectrometer)
#define USB_TIMEOUT          50      // Transfer timeout
#define REQUEST_TIMEOUT      500
#define DEFAULT_QUEUE_LENGTH 1000

#define MAX_REQUEST_SIZE     (0xFFFF - 4)

// Send and receive thread error codes
#define ERR_SUCCESS         0
#define ERR_NO_RESULT_PTR  -1001
#define ERR_BAD_BUFFER     -1002
#define ERR_BAD_SEMAPHORE  -1003
#define ERR_TIMEOUT        -1004
#define ERR_INCOMPLETE     -1005


// Serialization macros
#define SERIALIZE_VAR(buffer, offset, type, var) (*(type*)((buffer)+(offset))) = (var); (offset) += (sizeof(type))
#define SERIALIZE_OBJ(buffer, offset, var, length) (offset) += serialize(var, (buffer)+(offset), (length-offset))
#define SERIALIZE_STR(buffer, offset, var, length) memcpy((buffer)+(offset), (var), (length)); (offset) += (length)

// Un-serialization macros
#define UNSERIALIZE_VAR(buffer, offset, type, var) (var) = (*(type*)((buffer)+(offset))); (offset) += (sizeof(type))
#define UNSERIALIZE_OBJ(buffer, offset, var, length) (offset) += unserialize(var, (buffer)+(offset), (length-offset))
#define UNSERIALIZE_STR(buffer, offset, var, length) memcpy((var), (buffer)+(offset), (length)); (offset) += (length)



/* Avaspec internal structures
 */

typedef struct _SendRequest {
	omni_semaphore* sem;    // Wait semaphore
	uint8_t seq;            // Sequence number
	uint8_t command;        // Command code
	const uint8_t* payload; // Request payload
	size_t payload_len;     // Length of payload
	uint8_t** response;     // Pointer for returned response buffer
	size_t* response_len;   // Returned response buffer length
	int* result;            // Pointer to result
	struct timeval ts;      // Timestamp of request submission
	uint32_t timeout;       // Request timeout in ms
} SendRequest;

typedef struct _WaitRequest {
	omni_semaphore* sem;    // Wait semaphore
	uint8_t command;        // Command code
	uint8_t** response;     // Pointer for returned response buffer
	size_t* response_len;   // Returned response buffer length
	int* result;            // Pointer to result
	struct timeval ts;      // Timestamp of request submission
	struct timeval* outts;  // Pointer to return request received timestamp
	uint32_t timeout;       // Request timeout in ms
} WaitRequest;

typedef struct _OutPacket {
	uint8_t seq;            // Sequence number
	uint8_t command;        // Response code
	const uint8_t *buffer;  // Payload buffer
	size_t length;          // Payload length
} OutPacket;

typedef struct _InPacket {
	uint8_t seq;          // Sequence number
	uint8_t command;      // Response code
	uint8_t *buffer;      // Payload buffer
	size_t length;        // Payload length
	struct timeval ts;    // Receive timestamp
} InPacket;


/*
 *  Main AvaSpec class
 *  Application using the spectrometer should interface only with this class
 */
class AvaSpec : public BaseSpectrometer {
public:
	//! Constructor
	AvaSpec(libusb_device *dev, size_t queue_length=DEFAULT_QUEUE_LENGTH);

	//! Destructor
	~AvaSpec();

public:
	/*
	 * Standard interface
	 */

	//! Configure integration time
	void setIntegrationTime(uint32_t t);
	uint32_t getIntegrationTime()const { return uint32_t(measconf.m_IntegrationTime * 1000); };

	//! Get minimum and maximum integration time
	uint32_t getMinimumIntegrationTime() { return 0; }; // TODO
	uint32_t getMaximumIntegrationTime() { return MAX_INTEGRATION_TIME * 1000; };

	//! Configure number of averages
	void setScansToAverage(uint32_t avg);
	uint32_t getScansToAverage()const { return measconf.m_NrAverages; };

	//! Configure boxcar filter width
	void setBoxcarWidth(uint32_t n) {};  // TODO
	uint32_t getBoxcarWidth() { return 0; }; // TODO

	//! Set non-linear correction
	void setCorrectForDetectorNonlinearity(bool enable) {};  // TODO
	bool getCorrectForDetectorNonlinearity() { return false; };  // TODO

	//! Set electrical dark correction
	void setCorrectForElectricalDark(bool enable) {};  // TODO
	bool getCorrectForElectricalDark() { return false; };  // TODO

	// Enable edge-sensitive external trigger
	void setEdgeTrigger(bool enable) {};  // TODO
	bool getEdgeTrigger() { return false; };  // TODO

	//! Get number of pixels
	uint32_t getNumberOfPixels()const { return conf.m_Detector.m_NrPixels; };

	// Get serial number
	const char* getSerialNumber()const;

	// Return model of spectrometer
	const char* getModel() { return "AvaSpec"; }; // TODO
	
	// Return firmware version of spectrometer
	const char* getFirmwareVersion() { return "0.0"; }; // TODO

	// Get board temperature
	double getBoardTemperature();

	// Check if thermoelectric cooler is available
	bool isCoolingAvailable() { return false; }; // TODO

	// Enable cooler
	void setTECEnable(bool enable) {}; // TODO
	bool getTECEnable() { return false; }; // TODO

	// Enable cooler fan
	void setTECFanEnable(bool enable) {}; // TODO
	bool getTECFanEnable() { return false; }; // TODO

	// Set setpoint in celsius
	void setTECSetpoint(double temperature) {}; // TODO
	double getTECSetpoint() { return std::nan(""); }; // TODO

	// Get detector temperature
	double getTECTemperature() { return std::nan(""); }; // TODO

	//! Open the spectrometer
	void open();

	//! Close spectrometer
	void close();

	//! Get device information
	std::string getAS5216Version()const;
	std::string getFPGAVersion()const;
	std::string getHwVersion()const;

	//========================================================
	// Specialized methods (should be called only internally
protected:

	//! Get reference to device configuration structure
	DeviceConfigType& getDeviceConfig() { return conf; }

	//! Configure pixel range
	void setPixelRange(uint16_t  b, uint16_t  e);
	void getPixelRange(uint16_t& b, uint16_t& e)const { b = measconf.m_StartPixel; e = measconf.m_StopPixel; }

	//! Configure integration delay
	void setIntegrationDelay(uint32_t d);
	uint32_t getIntegrationDelay()const { return measconf.m_IntegrationDelay; }

	//! Start measurement
	void startMeasurement(int16_t n = -1);

	//! Stop measurement
	void stopMeasurement();

	//! Check if a measure is running
	bool isRunning()const { return _running; }

	//! Return size of spectra queue
	size_t size()const { return _queue.size(); }

	//! Get spectrum (id -1 means last spectrum)
	double* getSpectrum(ssize_t id=-1);

	//! Get analog input value
	// Valid IDs:
	// 0 = thermistor on optical bench (NIR2.2)
	// 1 = 1V2
	// 2 = 5VIO
	// 3 = 5VUSB
	// 4 = AI2 = pin 18 at 26-pins connector
	// 5 = AI1 = pin 9 at 26-pins connector
	// 6 = NTC1 onboard thermistor
	// 7 = Not used
	float getAnalogIn(uint8_t id);

	//! Set analog output value
	// Valid IDs:
	// 0 = AO1 = pin 17 at 26-pins connector
	// 1 = AO2 = pin 26 at 26-pins connector
	void setAnalogOut(uint8_t id, float value);

	//! Terminate measurement thread
	void terminate() { _terminate = true; }

protected:
	//! Thread entrypoint
	void* run_undetached(void* arg);

private:
	//! Read identification from spectrometer
	void readIdentification();

	//! Read configuration from spectrometer
	void readConfiguration();

	//! Write configuration to spectrometer
	void writeConfiguration();

	//! Initialize measure configuration structure
	void measConfigurationInit();

	//! Update wavelength vector to reflect pixel range selected in measurement
	void updateWavelength();

	//! Internal serializers
	static int serialize(const DarkCorrectionType& obj, uint8_t *buffer, int length);
	static int serialize(const SmoothingType& obj, uint8_t *buffer, int length);
	static int serialize(const TriggerType& obj, uint8_t *buffer, int length);
	static int serialize(const ControlSettingsType& obj, uint8_t *buffer, int length);
	static int serialize(const MeasConfigType& obj, uint8_t *buffer, int length);
	static int serialize(const DetectorType& obj, uint8_t *buffer, int length);
	static int serialize(const SpectrumCalibrationType& obj, uint8_t *buffer, int length);
	static int serialize(const IrradianceType& obj, uint8_t *buffer, int length);
	static int serialize(const SpectrumCorrectionType& obj, uint8_t *buffer, int length);
	static int serialize(const TimeStampType& obj, uint8_t *buffer, int length);
	static int serialize(const SDCardType& obj, uint8_t *buffer, int length);
	static int serialize(const StandAloneType& obj, uint8_t *buffer, int length);
	static int serialize(const TempSensorType& obj, uint8_t *buffer, int length);
	static int serialize(const TecControlType& obj, uint8_t *buffer, int length);
	static int serialize(const ProcessControlType& obj, uint8_t *buffer, int length);
	static int serialize(const DeviceConfigType& obj, uint8_t *buffer, int length);

	//! Internal unserializers
	static int unserialize(AvsIdentityType& obj, const uint8_t *buffer, int length);
	static int unserialize(DarkCorrectionType& obj, const uint8_t *buffer, int length);
	static int unserialize(SmoothingType& obj, const uint8_t *buffer, int length);
	static int unserialize(TriggerType& obj, const uint8_t *buffer, int length);
	static int unserialize(ControlSettingsType& obj, const uint8_t *buffer, int length);
	static int unserialize(MeasConfigType& obj, const uint8_t *buffer, int length);
	static int unserialize(DetectorType& obj, const uint8_t *buffer, int length);
	static int unserialize(SpectrumCalibrationType& obj, const uint8_t *buffer, int length);
	static int unserialize(IrradianceType& obj, const uint8_t *buffer, int length);
	static int unserialize(SpectrumCorrectionType& obj, const uint8_t *buffer, int length);
	static int unserialize(TimeStampType& obj, const uint8_t *buffer, int length);
	static int unserialize(SDCardType& obj, const uint8_t *buffer, int length);
	static int unserialize(StandAloneType& obj, const uint8_t *buffer, int length);
	static int unserialize(TempSensorType& obj, const uint8_t *buffer, int length);
	static int unserialize(TecControlType& obj, const uint8_t *buffer, int length);
	static int unserialize(ProcessControlType& obj, const uint8_t *buffer, int length);
	static int unserialize(DeviceConfigType& obj, const uint8_t *buffer, int length);

	//! Service function to swap bytes for little/big endian conversion
	static inline int16_t swap(int16_t value) { return bswap_16(value); }
	static inline uint16_t swap(uint16_t value) { return bswap_16(value); }
	static inline int32_t swap(int32_t value) { return bswap_32(value); }
	static inline uint32_t swap(uint32_t value) { return bswap_32(value); }
	static float swap(float value);
	static double swap(double value);

	//! Byte swapping functions for structures
	static void swapStruct(AvsIdentityType& obj);
	static void swapStruct(SmoothingType& obj);
	static void swapStruct(ControlSettingsType& obj);
	static void swapStruct(MeasConfigType& obj);
	static void swapStruct(DetectorType& obj);
	static void swapStruct(SpectrumCalibrationType& obj);
	static void swapStruct(IrradianceType& obj);
	static void swapStruct(SpectrumCorrectionType& obj);
	static void swapStruct(TimeStampType& obj);
	static void swapStruct(SDCardType& obj);
	static void swapStruct(StandAloneType& obj);
	static void swapStruct(TempSensorType& obj);
	static void swapStruct(TecControlType& obj);
	static void swapStruct(ProcessControlType& obj);
	static void swapStruct(DeviceConfigType& obj);

private:
	// Libusb context
	libusb_context* usbctx;

	// Spectrometer device handle
	libusb_device_handle* dev;

	// Spectrometer identification
	AvsIdentityType id;

	// Device configuration
	DeviceConfigType conf;

	// MeasConfigType
	MeasConfigType measconf;

	// USB communication thread
	AvaSpecUSBTransceiver* _transceiver;

	// Terminate flag
	bool _terminate;

	// Measure running flag
	bool _running;

	// Averaging flag
	bool _averaging;

	// Spectrum counter
	size_t _counter;

	// Number of measures
	int16_t _nmeasures;
};


/*! AvaSpecUSBTransceiver
 *  Thread class that handle the send and receive queue of the USB communication
 */
class AvaSpecUSBTransceiver : public omni_thread {
public:
	AvaSpecUSBTransceiver(libusb_device_handle* dev) : _dev(dev), _terminate(false) {
		// Start thread
		start_undetached();
	}
	~AvaSpecUSBTransceiver() {}

	//! Send request to the spectrometer
	int send(uint8_t command, const uint8_t* payload, size_t length, uint8_t** response, size_t* resp_len, uint32_t timeout=REQUEST_TIMEOUT);

	//! Wait for a requests from the spectrometer
	int wait(uint8_t command, uint8_t** response, size_t* resp_len, uint32_t timeout=REQUEST_TIMEOUT, struct timeval* outts=NULL);

	//! Terminate thread
	void terminate() { _terminate = true; }

protected:
	//! Thread entrypoint
	void* run_undetached(void* arg);

private:
	// Terminate flag
	bool _terminate;

	// Queue of requests to spectrometer
	std::queue<SendRequest> input_queue;

	// Queue of requests to spectrometer awaiting response
	std::deque<SendRequest> request_wait;

	// Queue of packets to be sent
	std::deque<OutPacket> out_packets;

	// Queue of packets from the spectrometer not yet reclaimed
	std::deque<InPacket> in_packets;

	// Queue of wait request on packets from the spectrometer
	std::deque<WaitRequest> wait_queue;

	// Mutex to protect request queue
	omni_mutex _req_lock;

	// Mutex to protect wait queue
	omni_mutex _wait_lock;

	// USB device handle
	libusb_device_handle* _dev;
};


#endif
