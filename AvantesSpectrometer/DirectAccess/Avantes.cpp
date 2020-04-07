/*
* Copyright 2016 Michele Devetta
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

#include "Avantes.h"


//! Class constructor
/** Class constructor.
*  Initialize libusb and searches for the spectrometer.
*/
AvaSpec::AvaSpec(size_t queue_length) :
	usbctx(NULL),
	dev(NULL),
	_transceiver(NULL),
	_terminate(false),
	_running(false),
	_averaging(false),
	_counter(0),
	_nmeasures(0),
	max_length(queue_length)
{
	// Initialize libusb context
	if(libusb_init(&usbctx)) {
		throw SpectroExcept("AvaSpec(): error initializing libusb");
	}

	// Clear structures
	memset(&id, 0, sizeof(AvsIdentityType));
	memset(&conf, 0, sizeof(DeviceConfigType));
	memset(&measconf, 0, sizeof(MeasConfigType));

	// Search for spectrometer
	openSpectrometer();

	// Start USB communication threads
	_transceiver = new AvaSpecUSBTransceiver(dev);

	// Wait for the threads tor start
	struct timespec ts = {2, 0};
	nanosleep(&ts, NULL);

	// Get identification info from spectrometer
	readIdentification();

	// Get device configuration
	readConfiguration();

	// Initialize measure parameters
	measConfigurationInit();

	// Start measurement thread
	start_undetached();
}


//! Class destructor
/** Class destructor.
*  Closes usb device and de-initialize libusb.
*/
AvaSpec::~AvaSpec() {
// If threads were created, stop them
int *retval;
	if(_transceiver) {
		_transceiver->terminate();
		_transceiver->join((void**)&retval);
		delete retval;
		_transceiver = NULL;
	}

	// Close usb device (if open)
	if(dev) {
		// NOTE: workaround for read stall bug
		libusb_clear_halt(dev, USB_IN_EP);
		libusb_release_interface(dev, USB_INTERFACE);
		libusb_close(dev);
		//libusb_reset_device(dev);
		dev = NULL;
	}

	// Destroy libusb context
	libusb_exit(usbctx);
}


//! Open spectrometer
/** Open spectrometer
*  Search for an AvaSpec spectrometer on the USB bus and open it
*/
void AvaSpec::openSpectrometer() {
	// List all USB devices
	libusb_device** dev_list = NULL;
	ssize_t num = libusb_get_device_list(usbctx, &dev_list);

	if(num > 0) {
		// Cycle over devices to find the spectrometer
		struct libusb_device_descriptor desc;
		for(ssize_t i = 0; i < num; i++) {
			memset(&desc, 0, sizeof(desc));

			// Get device descriptor
			libusb_get_device_descriptor(dev_list[i], &desc);

			// Check vendor and product IDs
			if(desc.idVendor == USB_VENDOR && desc.idProduct == USB_PRODUCT) {

				// Found!
				int ans = libusb_open(dev_list[i], &dev);

				// Clear device list
				libusb_free_device_list(dev_list, 1);

				// Throw exception if open was not successful
				if(ans) {
				throw SpectroExcept("AvaSpec::openSpectrometer(): error opening spectrometer (Error: %s)", libusb_error_name(ans));
				}

				// Claim interface
				libusb_claim_interface(dev, USB_INTERFACE);

				return;
			}
		}

		// Clear device list
		libusb_free_device_list(dev_list, 1);

		throw SpectroExcept("AvaSpec::openSpectrometer(): no spectrometer found");

	} else {
		throw SpectroExcept("AvaSpec::openSpectrometer(): no USB device found");
	}
}


//! Initialize measure configuration structure
void AvaSpec::measConfigurationInit() {
	// Full pixel range
	measconf.m_StartPixel = 0;
	measconf.m_StopPixel = conf.m_Detector.m_NrPixels - 1;
	measconf.m_IntegrationTime = 100.0;
	measconf.m_IntegrationDelay = 0;
	measconf.m_NrAverages = 1;
	measconf.m_CorDynDark.m_Enable = 0;
	measconf.m_CorDynDark.m_ForgetPercentage = 0;
	measconf.m_Smoothing.m_SmoothPix = 0;
	measconf.m_Smoothing.m_SmoothModel = 0;
	measconf.m_SaturationDetection = 0;
	measconf.m_Trigger.m_Mode = TRIGGER_SW;
	measconf.m_Trigger.m_Source = TRIGGER_EXT;
	measconf.m_Trigger.m_SourceType = TRIGGER_EDGE;
	measconf.m_Control.m_StrobeControl = 0;
	measconf.m_Control.m_LaserDelay = 0;
	measconf.m_Control.m_LaserWidth = 0;
	measconf.m_Control.m_LaserWaveLength = 0;
	measconf.m_Control.m_StoreToRam = 0;
	// Update wavelength
	updateWavelength();
}


//! Get device serial number
std::string AvaSpec::getSerialNumber()const {
	char serial[10];
	memcpy(serial, id.m_SerialId, 9);
	serial[9] = 0x00;
	return std::string(serial);
}


//! Get device AS5216 firmware version
std::string AvaSpec::getAS5216Version()const {
	std::stringstream ver;
	ver << ((id.m_as5216_version & 0xFF000000) >> 24) << ".";
	ver << ((id.m_as5216_version & 0x00FF0000) >> 16) << ".";
	ver << ((id.m_as5216_version & 0x0000FF00) >> 8) << ".";
	ver << (id.m_as5216_version & 0x000000FF);
	return ver.str();
}


//! Get device FPGA firmware version
std::string AvaSpec::getFPGAVersion()const {
	std::stringstream ver;
	ver << ((id.m_fpga_version & 0xFF000000) >> 24) << ".";
	ver << ((id.m_fpga_version & 0x00FF0000) >> 16) << ".";
	ver << ((id.m_fpga_version & 0x0000FF00) >> 8) << ".";
	ver << (id.m_fpga_version & 0x000000FF);
	return ver.str();
}


//! Get device hardware version
std::string AvaSpec::getHwVersion()const {
	std::stringstream ver;
	ver << ((id.m_hw_version & 0xFF000000) >> 24) << ".";
	ver << ((id.m_hw_version & 0x00FF0000) >> 16) << ".";
	ver << ((id.m_hw_version & 0x0000FF00) >> 8) << ".";
	ver << (id.m_hw_version & 0x000000FF);
	return ver.str();
}


//! Get analog input value
float AvaSpec::getAnalogIn(uint8_t id) {

	// Check channel ID
	if(id > 7)
		throw SpectroExcept("AvaSpec::getAnalogIn(): channel ID out of range. Analog output id should be between 0 and 7 (got %d)", int(id));

	// Buffer pointer
	uint8_t *response = NULL;
	size_t length = 0;

	// Send request
	int result = _transceiver->send(CMD_GETANAIN, &id, 1, &response, &length, 500);

	if(result != ERR_SUCCESS)
		throw SpectroExcept("AvaSpec::getAnalogIn(): failed to get analog input (Error code: %d)", result);

	if(!response || length < 2)
		throw SpectroExcept("AvaSpec::getAnalogIn(): failed to get analog input (Error: bad response)");

	uint16_t val = swap(*(uint16_t*)(response));

	// Clear buffer
	delete[] response;

	return float(val) / 1000.0;
}


//! Set analog output value
void AvaSpec::setAnalogOut(uint8_t id, float value) {

	// Check channel ID
	if(id > 1)
		throw SpectroExcept("AvaSpec::setAnalogOut(): channel ID out of range. Analog output id be 0 or 1 (got %d)", int(id));

	// Check value range
	if(value < 0.0 || value > 5.0)
		throw SpectroExcept("AvaSpec::setAnalogOut(): value out of range. Analog output should be between 0 and 5V (got %.2f)", value);

	// Convert value to unsigned short
	uint16_t raw = uint16_t(value * 100.0);

	// Payload
	uint8_t payload[3];
	payload[0] = id;                      // Analog channel ID
	*(uint16_t*)(payload+1) = swap(raw);  // Analog value

	// Send request
	int result = _transceiver->send(CMD_SETANAOUT, payload, 3, NULL, 0, 500);

	if(result != ERR_SUCCESS)
		throw SpectroExcept("AvaSpec::setAnalogOut(): failed to set analog output value (Error code: %d)", result);
	}


//! Get board temperature
double AvaSpec::getBoardTemperature() {
	float raw = getAnalogIn(6);
	float output = 0.0;
	for(size_t i = 0; i < NR_TEMP_POL_COEF; i++)
		output += conf.m_aTemperature[0].m_aFit[i] * pow(raw, i);
	return output;
}


//! Read identification from spectrometer
void AvaSpec::readIdentification() {

	// Buffer pointer
	uint8_t *response = NULL;
	size_t length = 0;

	// Send request
	int result = _transceiver->send(CMD_GETID, NULL, 0, &response, &length, 500);

	if(result != ERR_SUCCESS)
		throw SpectroExcept("AvaSpec::readIdentification(): failed to get identification info (Error code: %d)", result);

	if(!response)
		throw SpectroExcept("AvaSpec::readIdentification(): failed to get identification info (Error: bad response)");

	// Unserialize struct
	unserialize(id, response, length);

	// Clear buffer
	delete[] response;

	// Byteswap
	swapStruct(id);
}


//! Read configuration from spectrometer
void AvaSpec::readConfiguration() {

	// Buffer pointer
	uint8_t *response = NULL;
	size_t length = 0;

	// Send request
	int result = _transceiver->send(CMD_GETCONF, NULL, 0, &response, &length, 500);

	if(result != ERR_SUCCESS)
		throw SpectroExcept("AvaSpec::readConfiguration(): failed to get configuration (Error code: %d)", result);

	if(!response)
		throw SpectroExcept("AvaSpec::readConfiguration(): failed to get configuration (Error: bad response)");

	// Unserialize struct
	unserialize(conf, response, length);

	// Clear buffer
	delete[] response;

	// Byteswap
	swapStruct(conf);

	// Update wavelength
	updateWavelength();
}


//! Update calibrated wavelength vector
void AvaSpec::updateWavelength() {
	uint16_t b, e;
	getPixelRange(b, e);

	_wl.clear();
	_wl.reserve(e - b + 1);
	for(size_t i = b; i <= e; i++) {
		float val = 0.0;
		for(size_t j = 0; j < NR_WAVELEN_POL_COEF; j++)
		val += conf.m_Detector.m_aFit[j] * pow(i, j);
		_wl.push_back(val);
	}
}


// Write configuration to spectrometer
void AvaSpec::writeConfiguration() {

	// Buffer
	uint8_t buffer[AVASPEC_DEVCONF_SIZE];
	size_t len = AVASPEC_DEVCONF_SIZE;
	memset(buffer, 0, len);

	// Copy configuration
	DeviceConfigType obj = conf;

	// Byteswap
	swapStruct(obj);

	// Serialize
	serialize(obj, buffer, len);

	// Send request
	int result = _transceiver->send(CMD_SETCONF, buffer, len, NULL, 0, 500);

	if(result != ERR_SUCCESS) {
		throw SpectroExcept("AvaSpec::writeConfiguration(): failed to write configuration (Error code: %d)", result);
	}
}


//! Set pixel range
void AvaSpec::setPixelRange(uint16_t  b, uint16_t  e) {
	if(b > conf.m_Detector.m_NrPixels - 1 || e > conf.m_Detector.m_NrPixels - 1)
		throw SpectroExcept("AvaSpec::setPixelRange(): pixel range limits should be less than the maximum number of pixels");

	if(b == e)
		throw SpectroExcept("AvaSpec::setPixelRange(): pixel range cannot be null");

	if(b > e)
		throw SpectroExcept("AvaSpec::setPixelRange(): pixel range start should be less that the end");

	bool run = _running;

	// Stop measurement if it's running
	if(run)
		this->stopMeasurement();

	// Set range
	measconf.m_StartPixel = b;
	measconf.m_StopPixel = e;

	// Restart measurement if it was running in continuous mode
	if(run && _nmeasures == -1)
		this->startMeasurement(-1);
}


//! Configure integration time
void AvaSpec::setIntegrationTime(float t) {
	if(t > MAX_INTEGRATION_TIME)
		throw SpectroExcept("AvaSpec::setIntegrationTime(): max integration time is %.1f", MAX_INTEGRATION_TIME);

	float min_int = 0.0;
	switch(conf.m_Detector.m_SensorType) {
		case SENS_HAMS8378_256:
			min_int = SENS_HAMS8378_256_MININT;
			break;
		case SENS_HAMS8378_1024:
			min_int = SENS_HAMS8378_1024_MININT;
			break;
		case SENS_ILX554:
			min_int = SENS_ILX554_MININT;
			break;
		case SENS_HAMS9201:
			min_int = SENS_HAMS9201_MININT;
			break;
		case SENS_TCD1304:
			min_int = SENS_TCD1304_MININT;
			break;
		case SENS_TSL1301:
			min_int = SENS_TSL1301_MININT;
			break;
		case SENS_TSL1401:
			min_int = SENS_TSL1401_MININT;
			break;
		case SENS_HAMS9840:
			min_int = SENS_HAMS9840_MININT;
			break;
		default:
			throw SpectroExcept("AvaSpec::setIntegrationTime(): unknown sensor type %d", conf.m_Detector.m_SensorType);
	}
	if(t < min_int)
		throw SpectroExcept("AvaSpec::setIntegrationTime(): minimum integration time is %.1f", min_int);

	bool run = _running;

	// Stop measurement if it's running
	if(run)
		this->stopMeasurement();

	// Set range
	measconf.m_IntegrationTime = t;

	// Restart measurement if it was running in continuous mode
	if(run && _nmeasures == -1)
		this->startMeasurement(-1);
}


//! Configure integration delay
void AvaSpec::setIntegrationDelay(uint32_t d) {
	bool run = _running;

	// Stop measurement if it's running
	if(run)
		this->stopMeasurement();

	// Set range
	measconf.m_IntegrationDelay = d;

	// Restart measurement if it was running in continuous mode
	if(run && _nmeasures == -1)
		this->startMeasurement(-1);
}


//! Configure number of averages
void AvaSpec::setAverages(uint32_t avg) {
	if(avg == 0)
		avg = 1;

	bool run = _running;

	// Stop measurement if it's running
	if(run)
		this->stopMeasurement();

	// Set range
	measconf.m_NrAverages = avg;

	// Restart measurement if it was running in continuous mode
	if(run && _nmeasures == -1)
		this->startMeasurement(-1);
}


//! Start measurement
void AvaSpec::startMeasurement(int16_t n) {
	if(_running)
		throw SpectroExcept("AvaSpec::startMeasurement(): measurement already running");

	// Reset counter
	_counter = 0;

	// Set number of measures
	_nmeasures = n;

	// Buffer
	uint8_t buffer[MEASCONFIG_LEN];

	// Copy object
	MeasConfigType mc = measconf;

	// Byte swap
	swapStruct(mc);

	// Serialize struct
	serialize(mc, buffer, MEASCONFIG_LEN);

	// Prepare measurement
	int result = _transceiver->send(CMD_PREPMEAS, buffer, MEASCONFIG_LEN, NULL, 0, 100);

	if(result != ERR_SUCCESS)
		throw SpectroExcept("AvaSpec::startMeasurement(): failed to prepare measurement (Error code: %d)", result);

	// Check if averaging is enabled
	if(measconf.m_NrAverages > 1)
		_averaging = true;
	else
		_averaging = false;

	// Start measurement
	n = swap(n);
	result = _transceiver->send(CMD_STARTMEAS, (uint8_t*)&n, sizeof(int16_t), NULL, 0, 100);

	if(result != ERR_SUCCESS)
		throw SpectroExcept("AvaSpec::startMeasurement(): failed to start measurement (Error code: %d)", result);

	// Set running flag
	_running = true;
}


//! Stop measurement
void AvaSpec::stopMeasurement() {
	if(!_running)
		return;

	// Stop measurement
	int result = _transceiver->send(CMD_STOPMEAS, NULL, 0, NULL, 0, 100);

	if(result != ERR_SUCCESS)
		throw SpectroExcept("AvaSpec::stopMeasurement(): failed to stop measurement (Error code: %d)", result);

	_running = false;
}


//! Get spectrum from queue
double* AvaSpec::getSpectrum(ssize_t id) {



}


//! Spectra accumulation thread
void* AvaSpec::run_undetached(void* arg) {

	// Sleep delay (100ms)
	struct timespec st = {0, 100000000};

	// Temporary buffer
	uint8_t* buffer = NULL;
	size_t len = 0;

	// Spectrum timestamp
	struct timeval ts = {0, 0};

	while(!_terminate) {

		if(_running) {

		// Request spectrum
		int result = 0;
		if(_averaging)
			result = _transceiver->wait(CMD_AVGDATA, &buffer, &len, 500, &ts);
		else
			result = _transceiver->wait(CMD_MEASDATA, &buffer, &len, 500, &ts);

		if(result == ERR_SUCCESS) {
			// Got spectrum
			_counter++;

			if(buffer) {
			// Spectrum struct
			AvaSpectrum spec;

			// FPGA ticks
			spec.ticks = swap(*(uint32_t*)(buffer));

			// Spectrum length
			if(_averaging)
				spec.length = (len - 4) / sizeof(uint32_t);
			else
				spec.length = (len - 4) / sizeof(uint16_t);

			// Timestamp
			spec.ts = ts;

			// Allocate buffer for processed spectrum
			spec.spectrum = new double[spec.length];

			// Spectum
			if(_averaging) {
				for(size_t i = 0; i < spec.length; i++) {
				spec.spectrum[i] = double(swap(*(uint32_t*)(buffer+i*sizeof(uint32_t)+4)));
				// TODO: add non-linear correction
				}

			} else {
				for(size_t i = 0; i < spec.length; i++) {
				spec.spectrum[i] = double(swap(*(uint16_t*)(buffer+i*sizeof(uint16_t)+4)));
				// TODO: add non-linear correction
				}
			}

			// Clear buffer
			delete[] buffer;

			printf("[DEBUG] got spectrum with timesteamp %u (counter = %lu, nmeas = %u)\n", spec.ticks, _counter, _nmeasures);

			// Push spectrum on the queue
			_queue.push_back(spec);

			// Check queue length
			while(_queue.size() > max_length) {
				AvaSpectrum s = _queue.front();
				if(s.spectrum)
				delete[] s.spectrum;
				_queue.pop_front();
			}

			} else {
			// Got empty spectrum
			// TODO: handle error
			}

			if(_nmeasures != -1 && _counter >= _nmeasures) {  // TODO: avoid warning ensuring that _nmeasure is positive and then converting to unsigned!
			_running = false;
			_counter = 0;
			}

		} else {
			// TODO: handle error
			printf("Error receiving spectrum (Error: %d)\n", result);
		}

		} else {
		// Sleep
		nanosleep(&st, NULL);
		}
	}

	// Empty queue
	while(_queue.size() > 0) {
		AvaSpectrum s = _queue.front();
		if(s.spectrum)
		delete[] s.spectrum;
		_queue.pop_front();
	}

	int* retval = new int;
	*retval = 0;
	return retval;
}


//! Send request to the spectrometer
int AvaSpecUSBTransceiver::send(uint8_t command, const uint8_t* payload, size_t length, uint8_t** response, size_t* resp_len, uint32_t timeout) {

	// Semaphore
	omni_semaphore s(0);

	// Result pointer must be supplied
	int result = ERR_SUCCESS;

	// If payload is null, length must be zero
	if(length != 0 && payload == NULL)
		return ERR_BAD_BUFFER;

	// If response pointer is not null, a response length output is needed
	if(response != NULL && resp_len == NULL)
		return ERR_BAD_BUFFER;

	// Prepare request
	SendRequest req;
	req.sem = &s;
	req.command = command;
	req.payload = (length > 0) ? payload : NULL;
	req.payload_len = length;
	req.response = response;
	req.response_len = (response != NULL) ? resp_len : NULL;
	req.result = &result;
	gettimeofday(&req.ts, NULL);
	req.timeout = timeout;

	// Add request to queue
	{
		omni_mutex_lock(this->_req_lock);
		input_queue.push(req);
	}

	// Wait for the request to complete
	s.wait();

	return result;
}


//! Wait for a requests from the spectrometer
int AvaSpecUSBTransceiver::wait(uint8_t command, uint8_t** response, size_t* resp_len, uint32_t timeout, struct timeval* outts) {

	// Semaphore
	omni_semaphore s(0);

	// Result pointer must be supplied
	int result = ERR_SUCCESS;

	// If response pointer is not null, a response length output is needed
	if(response != NULL && resp_len == NULL)
		return ERR_BAD_BUFFER;

	// Prepare request
	WaitRequest req;
	req.sem = &s;
	req.command = command;
	req.response = response;
	req.response_len = (response != NULL) ? resp_len : NULL;
	req.result = &result;
	gettimeofday(&req.ts, NULL);
	req.outts = outts;
	req.timeout = timeout;

	// Add request to queue
	{
		omni_mutex_lock(this->_wait_lock);
		wait_queue.push_back(req);
	}

	// Wait for the request to complete
	s.wait();

	return result;
}


//! Thread entry point
/** Thread entry point
*  This function handle the communication and the sync of request and responses
*/
void* AvaSpecUSBTransceiver::run_undetached(void* arg) {

	// Static buffer
	uint8_t buffer[MAX_REQUEST_SIZE];

	// Sent request counter
	uint8_t seqnum = 0;

	// Sent packet counter
	int sent_count = 0;

	// Sleep time
	struct timespec sst = {0, 100000};    // 100us
	struct timespec lst = {0, 10000000};  // 10ms

	// Static acknowledge packet
	OutPacket ack;
	ack.seq = 0;
	ack.command = 0xC0;
	ack.buffer = NULL;
	ack.length = 0;

	while(!_terminate) {

		bool noop = true;

		// Check request queue: add packet to output queue and move request to wait queue
		{
			omni_mutex_lock(this->_req_lock);
			while(!input_queue.empty()) {

				// Get request
				SendRequest req = input_queue.front();
				input_queue.pop();

				// Build packet descriptor
				OutPacket pk;
				pk.command = req.command;
				pk.seq = seqnum;
				req.seq = seqnum;
				//seqnum++;
				(req.payload_len > 0) ? pk.buffer = req.payload : pk.buffer = NULL;
				pk.length = req.payload_len;

				// Add packet to output queue
				out_packets.push_back(pk);
				// Add request to wait queue
				request_wait.push_back(req);
			}
		}

		// If sent counter is less than maximum send a packet from the queue
		if(!out_packets.empty() && sent_count < 1) {
			noop = false;

			// Get packet descriptor
			OutPacket pk = out_packets.front();

			// Build request
			memset(buffer, 0, MAX_REQUEST_SIZE);
			buffer[0] = PC2S_TYPE;
			buffer[1] = pk.seq;
			*(uint16_t*)(buffer+2) = pk.length + 2;
			buffer[4] = pk.command;
			buffer[5] = 0x00;
			if(pk.length > 0)
				memcpy(buffer+6, pk.buffer, pk.length);

			// Send request on USB bus
			int transferred = 0;
			int ans = libusb_bulk_transfer(_dev, USB_OUT_EP, buffer, pk.length + 6, &transferred, USB_TIMEOUT);
			if(ans || (size_t)transferred != pk.length + 6) {
				// Transfer error!
				// TODO: handle error!
				if(ans != LIBUSB_ERROR_TIMEOUT)
				printf("[DEBUG] error sending USB packet (Error code: %d)\n", ans);
			}

			if(pk.command != RSP_ACK)
				sent_count++;

			// Pop request from queue
			out_packets.pop_front();
		}

		// Try to receive a packet
		{
			memset(buffer, 0, MAX_REQUEST_SIZE);
			int transferred = 0;
			int ans = libusb_bulk_transfer(_dev, USB_IN_EP, buffer, MAX_REQUEST_SIZE, &transferred, USB_TIMEOUT);

			if(ans != LIBUSB_ERROR_TIMEOUT) {
				noop = false;

				if(ans || transferred < 5) {
				// TODO: handle errors
				printf("[DEBUG] USB send failed with error %d (Transferred: %d)\n", ans, transferred);

				} else {
				// Check if it's an error code
				size_t len = *(uint16_t*)(buffer+2);
				if(len == 0) {
					// Error code
					printf("[DEBUG] Request %d returned error %d\n", int(buffer[1]), int(buffer[4]));

				} else {

					// Check if is a response or a request from the spectrometer
					if(buffer[4] == CMD_MEASDATA || buffer[4] == CMD_AVGDATA) {
					// Add packet to input packet queue
					InPacket ipk;
					// First get timestamp
					gettimeofday(&ipk.ts, NULL);
					// Packet info
					ipk.seq = buffer[1];
					ipk.command = buffer[4];
					// Payload length
					ipk.length = *(uint16_t*)(buffer+2) - 2;
					// Payload
					if(ipk.length > 0) {
						ipk.buffer = new uint8_t[ipk.length];
						memcpy(ipk.buffer, buffer+6, ipk.length);
					} else {
						ipk.buffer = NULL;
					}
					// Add packet to input queue
					in_packets.push_back(ipk);
					// Send acknowledge (acknowledges have higher priority than normal requests so we put them at the front of the queue)
					out_packets.push_front(ack);

					} else {
					// Search for a pending request
					for(std::deque<SendRequest>::iterator i = request_wait.begin(); i != request_wait.end(); ) {
						if(i->command + 0x80 == buffer[4]) {
						// Found request. Check sequence number
						if(i->seq != buffer[1])
							printf("[DEBUG] wrong sequence number! Expecting %d, got %d\n", int(i->seq), int(buffer[1]));
						// TODO: handle bad command code...

						// Return request
							if(i->response != NULL) {
							// Copy response
							*(i->response) = new uint8_t[transferred - 6];
							memcpy(*(i->response), buffer+6, transferred - 6);
							*(i->response_len) = transferred - 6;
						}
						*(i->result) = ERR_SUCCESS;
						i->sem->post();
						request_wait.erase(i);
						sent_count--;
						break;

						} else {
						i++;
						}
					}
					}
				}
				}
			}
		}

		// Get current time for timeout evaluation
		struct timeval now;
		gettimeofday(&now, NULL);

		// Check queue for timed out requests
		for(std::deque<SendRequest>::iterator i = request_wait.begin(); i != request_wait.end(); ) {
			if(ELAPSED_TIME_MS(i->ts, now) > i->timeout) {
				if(i->response != NULL) {
				*(i->response) = NULL;
				*(i->response_len) = 0;
				}
				*(i->result) = ERR_TIMEOUT;
				i->sem->post();
				i = request_wait.erase(i);
			} else {
				i++;
			}
		}

		// Check wait queue
		for(std::deque<WaitRequest>::iterator i = wait_queue.begin(); i != wait_queue.end(); ) {
		bool answered = false;

		if(ELAPSED_TIME_MS(i->ts, now) > i->timeout) {
			// Delete elapsed request
			if(i->response != NULL) {
				*(i->response) = NULL;
				*(i->response_len) = 0;
			}
			*(i->result) = ERR_TIMEOUT;
			i->sem->post();
			answered = true;

		} else {
			// Try to answer request
			for(std::deque<InPacket>::iterator j = in_packets.begin(); j != in_packets.end(); ) {
				if(j->command == i->command) {
					// Found suitable packet. Answer request
					// Set buffer
					if(i->response != NULL) {
					*(i->response) = j->buffer;
					*(i->response_len) = j->length;
					} else {
					// If no output buffer is specified we should free the payload buffer
					if(j->buffer != NULL)
						delete[] j->buffer;
					}
					// Set result code
					*(i->result) = ERR_SUCCESS;
					if(i->outts != NULL) {
					// Copy timestamp
					*(i->outts) = j->ts;
					}
					i->sem->post();
					answered = true;

					// Delete packet and stop search on packet queue
					in_packets.erase(j);
					break;
				}
				// Go to next packet
				j++;
			}
		}

		// Delete wait request if it was answered
		if(answered)
			i = wait_queue.erase(i);
		else
			i++;
		}

		if(noop) {
			nanosleep(&lst, NULL);
		} else {
			nanosleep(&sst, NULL);
		}
	}

	/* Before terminating the thread we should take care of answering all pending request with a timeout error and
	* clear all allocated buffers
	* NOTE: this is needed to prevent threads beeing locked on a semaphore that will never be signaled
	*/
	while(!request_wait.empty()) {
		SendRequest r = request_wait.front();
		if(r.response != NULL) {
			*(r.response) = NULL;
			*(r.response_len) = 0;
		}
		*(r.result) = ERR_TIMEOUT;
		r.sem->post();
		request_wait.pop_front();
	}
	while(!wait_queue.empty()) {
		WaitRequest r = wait_queue.front();
		if(r.response != NULL) {
			*(r.response) = NULL;
			*(r.response_len) = 0;
		}
		*(r.result) = ERR_TIMEOUT;
		r.sem->post();
		wait_queue.pop_front();
	}

	int* retval = new int();
	*retval = 0;
	return retval;
}


/**
**   == SERVIVCE FUNCTIONS FOR BYTE SWAPPING AND STRUCTURE SERIALIZATION ==
**/
//! Byteswap a float value
float AvaSpec::swap(float value) {
	float temp = 0.0;
	char *ptr1 = (char*) &value;
	char *ptr2 = (char*) &temp;
	for(size_t i = 0; i < sizeof(float); i++)
		ptr2[i] = ptr1[sizeof(float)-i-1];
	return temp;
}

//! Byteswap a double value
double AvaSpec::swap(double value) {
	double temp = 0.0;
	char *ptr1 = (char*) &value;
	char *ptr2 = (char*) &temp;
	for(size_t i = 0; i < sizeof(double); i++)
		ptr2[i] = ptr1[sizeof(double)-i-1];
	return temp;
}

//! Swap bytes of members of a AvsIdentityType structure
void AvaSpec::swapStruct(AvsIdentityType& obj) {
	obj.m_as5216_version = swap(obj.m_as5216_version);
	obj.m_fpga_version = swap(obj.m_fpga_version);
	obj.m_hw_version = swap(obj.m_hw_version);
}

//! Swap bytes of members of a SmoothingType structure
void AvaSpec::swapStruct(SmoothingType& obj) {
	obj.m_SmoothPix = swap(obj.m_SmoothPix);
}

//! Swap bytes of members of a ControlSettingsType structure
void AvaSpec::swapStruct(ControlSettingsType& obj) {
	obj.m_StrobeControl = swap(obj.m_StrobeControl);
	obj.m_LaserDelay = swap(obj.m_LaserDelay);
	obj.m_LaserWidth = swap(obj.m_LaserWidth);
	obj.m_LaserWaveLength = swap(obj.m_LaserWaveLength);
	obj.m_StoreToRam = swap(obj.m_StoreToRam);
}

//! Swap bytes of members of a MeasConfigType structure
void AvaSpec::swapStruct(MeasConfigType& obj) {
	obj.m_StartPixel = swap(obj.m_StartPixel);
	obj.m_StopPixel = swap(obj.m_StopPixel);
	obj.m_IntegrationTime = swap(obj.m_IntegrationTime);
	obj.m_IntegrationDelay = swap(obj.m_IntegrationDelay);
	obj.m_NrAverages = swap(obj.m_NrAverages);
	swapStruct(obj.m_Smoothing);
	swapStruct(obj.m_Control);
}

//! Swap bytes of members of a DetectorType structure
void AvaSpec::swapStruct(DetectorType& obj) {
	obj.m_NrPixels = swap(obj.m_NrPixels);
	for(size_t i = 0; i < NR_WAVELEN_POL_COEF; i++)
		obj.m_aFit[i] = swap(obj.m_aFit[i]);
	for(size_t i = 0; i < NR_NONLIN_POL_COEF; i++)
		obj.m_aNLCorrect[i] = swap(obj.m_aNLCorrect[i]);
	obj.m_aLowNLCounts = swap(obj.m_aLowNLCounts);
	obj.m_aHighNLCounts = swap(obj.m_aHighNLCounts);
	for(size_t i = 0; i < MAX_VIDEO_CHANNELS; i++) {
		obj.m_Gain[i] = swap(obj.m_Gain[i]);
		obj.m_Offset[i] = swap(obj.m_Offset[i]);
	}
	obj.m_ExtOffset = swap(obj.m_ExtOffset);
	for(size_t i = 0; i < NR_DEFECTIVE_PIXELS; i++)
		obj.m_DefectivePixels[i] = swap(obj.m_DefectivePixels[i]);
}

//! Swap bytes of members of a SpectrumCalibrationType
void AvaSpec::swapStruct(SpectrumCalibrationType& obj) {
	swapStruct(obj.m_Smoothing);
	obj.m_CalInttime = swap(obj.m_CalInttime);
	for(size_t i = 0; i < MAX_NR_PIXELS; i++)
		obj.m_aCalibConvers[i] = swap(obj.m_aCalibConvers[i]);
}

//! Swap bytes of members of a IrradianceType
void AvaSpec::swapStruct(IrradianceType& obj) {
	swapStruct(obj.m_IntensityCalib);
	obj.m_FiberDiameter = swap(obj.m_FiberDiameter);
}

//! Swap bytes of members of a SpectrumCorrectionType
void AvaSpec::swapStruct(SpectrumCorrectionType& obj) {
	for(size_t i = 0; i < MAX_NR_PIXELS; i++)
		obj.m_aSpectrumCorrect[i] = swap(obj.m_aSpectrumCorrect[i]);
}

//! Swap bytes of members of a TimeStampType
void AvaSpec::swapStruct(TimeStampType& obj) {
	obj.m_Date = swap(obj.m_Date);
	obj.m_Time = swap(obj.m_Time);
}

//! Swap bytes of members of a SDCardType
void AvaSpec::swapStruct(SDCardType& obj) {
	swapStruct(obj.m_TimeStamp);
}

//! Swap bytes of members of a StandAloneType
void AvaSpec::swapStruct(StandAloneType& obj) {
	swapStruct(obj.m_Meas);
	obj.m_Nmsr = swap(obj.m_Nmsr);
	swapStruct(obj.m_SDCard);
}

//! Swap bytes of members of a TempSensorType
void AvaSpec::swapStruct(TempSensorType& obj) {
	for(size_t i = 0; i < NR_TEMP_POL_COEF; i++)
		obj.m_aFit[i] = swap(obj.m_aFit[i]);
}

//! Swap bytes of members of a TecControlType
void AvaSpec::swapStruct(TecControlType& obj) {
	obj.m_Setpoint = swap(obj.m_Setpoint);
	for(size_t i = 0; i < NR_DAC_POL_COEF; i++)
		obj.m_aFit[i] = swap(obj.m_aFit[i]);
}

//! Swap bytes of members of a ProcessControlType
void AvaSpec::swapStruct(ProcessControlType& obj) {
	for(size_t i = 0; i < 2; i++)
		obj.AnalogLow[i] = swap(obj.AnalogLow[i]);
	for(size_t i = 0; i < 2; i++)
		obj.AnalogHigh[i] = swap(obj.AnalogHigh[i]);
	for(size_t i = 0; i < 10; i++)
		obj.DigitalLow[i] = swap(obj.DigitalLow[i]);
	for(size_t i = 0; i < 10; i++)
		obj.DigitalHigh[i] = swap(obj.DigitalHigh[i]);
}

//! Swap bytes of members of a DeviceConfigType
void AvaSpec::swapStruct(DeviceConfigType& obj) {
	obj.m_Len = swap(obj.m_Len);
	obj.m_ConfigVersion = swap(obj.m_ConfigVersion);
	swapStruct(obj.m_Detector);
	swapStruct(obj.m_Irradiance);
	swapStruct(obj.m_Reflectance);
	swapStruct(obj.m_SpectrumCorrect);
	swapStruct(obj.m_StandAlone);
	for(size_t i = 0; i < MAX_TEMP_SENSORS; i++)
		swapStruct(obj.m_aTemperature[i]);
	swapStruct(obj.m_TecControl);
	swapStruct(obj.m_ProcessControl);
}

//! DarkCorrectionType serializer
int AvaSpec::serialize(const DarkCorrectionType& obj, uint8_t *buffer, int length) {
	if(length < DARKCORRECTION_LEN)
		throw SpectroExcept("AvaSpec::serialize(const DarkCorrectionType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_Enable);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_ForgetPercentage);
	return offset;
}

//! SmoothingType serializer
int AvaSpec::serialize(const SmoothingType& obj, uint8_t *buffer, int length) {
	if(length < SMOOTHING_LEN)
		throw SpectroExcept("AvaSpec::serialize(const SmoothingType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_SmoothPix);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SmoothModel);
	return offset;
}

//! TriggerType serializer
int AvaSpec::serialize(const TriggerType& obj, uint8_t *buffer, int length) {
	if(length < TRIGGER_LEN)
		throw SpectroExcept("AvaSpec::serialize(const TriggerType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_Mode);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_Source);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SourceType);
	return offset;
}

//! ControlSettingsType serializer
int AvaSpec::serialize(const ControlSettingsType& obj, uint8_t *buffer, int length) {
	if(length < CONTROLSETTINGS_LEN)
		throw SpectroExcept("AvaSpec::serialize(const ControlSettingsType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_StrobeControl);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj.m_LaserDelay);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj.m_LaserWidth);
	SERIALIZE_VAR(buffer, offset, float, obj.m_LaserWaveLength);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_StoreToRam);
	return offset;
}

//! MeasConfigType serializer
int AvaSpec::serialize(const MeasConfigType& obj, uint8_t *buffer, int length) {
	if(length < MEASCONFIG_LEN)
		throw SpectroExcept("AvaSpec::serialize(const MeasConfigType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_StartPixel);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_StopPixel);
	SERIALIZE_VAR(buffer, offset, float, obj.m_IntegrationTime);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj.m_IntegrationDelay);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj.m_NrAverages);
	SERIALIZE_OBJ(buffer, offset, obj.m_CorDynDark, length);
	SERIALIZE_OBJ(buffer, offset, obj.m_Smoothing, length);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SaturationDetection);
	SERIALIZE_OBJ(buffer, offset, obj.m_Trigger, length);
	SERIALIZE_OBJ(buffer, offset, obj.m_Control, length);
	return offset;
}

//! DetectorType serializer
int AvaSpec::serialize(const DetectorType& obj, uint8_t *buffer, int length) {
	if(length < DETECTOR_LEN)
		throw SpectroExcept("AvaSpec::serialize(const DetectorType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SensorType);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_NrPixels);
	for(size_t i = 0; i < NR_WAVELEN_POL_COEF; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.m_aFit[i]);
	}
	SERIALIZE_VAR(buffer, offset, bool, obj.m_NLEnable);
	for(size_t i = 0; i < NR_NONLIN_POL_COEF; i++) {
		SERIALIZE_VAR(buffer, offset, double, obj.m_aNLCorrect[i]);
	}
	SERIALIZE_VAR(buffer, offset, double, obj.m_aLowNLCounts);
	SERIALIZE_VAR(buffer, offset, double, obj.m_aHighNLCounts);
	for(size_t i = 0; i < MAX_VIDEO_CHANNELS; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.m_Gain[i]);
	}
	SERIALIZE_VAR(buffer, offset, float, obj.m_Reserved);
	for(size_t i = 0; i < MAX_VIDEO_CHANNELS; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.m_Offset[i]);
	}
	SERIALIZE_VAR(buffer, offset, float, obj.m_ExtOffset);
	for(size_t i = 0; i < NR_DEFECTIVE_PIXELS; i++) {
		SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_DefectivePixels[i]);
	}
	return offset;
}

//! SpectrumCalibrationType serializer
int AvaSpec::serialize(const SpectrumCalibrationType& obj, uint8_t *buffer, int length) {
	if(length < SPECTRUMCALIBRATION_LEN)
		throw SpectroExcept("AvaSpec::serialize(const SpectrumCalibrationType&): buffer too short");
	int offset = 0;
	SERIALIZE_OBJ(buffer, offset, obj.m_Smoothing, length);
	SERIALIZE_VAR(buffer, offset, float, obj.m_CalInttime);
	for(size_t i = 0; i < MAX_NR_PIXELS; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.m_aCalibConvers[i]);
	}
	return offset;
}

//! IrradianceType serializer
int AvaSpec::serialize(const IrradianceType& obj, uint8_t *buffer, int length) {
	if(length < IRRADIANCE_LEN)
		throw SpectroExcept("AvaSpec::serialize(const IrradianceType&): buffer too short");
	int offset = 0;
	SERIALIZE_OBJ(buffer, offset, obj.m_IntensityCalib, length);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_CalibrationType);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj.m_FiberDiameter);
	return offset;
}

//! SpectrumCorrectionType serializer
int AvaSpec::serialize(const SpectrumCorrectionType& obj, uint8_t *buffer, int length) {
	if(length < SPECTRUMCORRECTION_LEN)
		throw SpectroExcept("AvaSpec::serialize(const SpectrumCorrectionType&): buffer too short");
	int offset = 0;
	for(size_t i = 0; i < MAX_NR_PIXELS; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.m_aSpectrumCorrect[i]);
	}
	return offset;
}

//! TimeStampType serializer
int AvaSpec::serialize(const TimeStampType& obj, uint8_t *buffer, int length) {
	if(length < TIMESTAMP_LEN)
		throw SpectroExcept("AvaSpec::serialize(const TimeStampType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_Date);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_Time);
	return offset;
}

//! SDCardType serializer
int AvaSpec::serialize(const SDCardType& obj, uint8_t *buffer, int length) {
	if(length < SDCARD_LEN)
		throw SpectroExcept("AvaSpec::serialize(const SDCardType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, bool, obj.m_Enable);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SpectrumType);
	SERIALIZE_STR(buffer, offset, obj.m_aFileRootName, ROOT_NAME_LEN);
	SERIALIZE_OBJ(buffer, offset, obj.m_TimeStamp, length);
	return offset;
}

//! StandAloneType serializer
int AvaSpec::serialize(const StandAloneType& obj, uint8_t *buffer, int length) {
	if(length < STANDALONE_LEN)
		throw SpectroExcept("AvaSpec::serialize(const StandAloneType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, bool, obj.m_Enable);
	SERIALIZE_OBJ(buffer, offset, obj.m_Meas, length);
	SERIALIZE_VAR(buffer, offset, int16_t, obj.m_Nmsr);
	SERIALIZE_OBJ(buffer, offset, obj.m_SDCard, length);
	return offset;
}

//! TempSensorType serializer
int AvaSpec::serialize(const TempSensorType& obj, uint8_t *buffer, int length) {
	if(length < TEMPSENSOR_LEN)
		throw SpectroExcept("AvaSpec::serialize(const TempSensorType&): buffer too short");
	int offset = 0;
	for(size_t i = 0; i < NR_TEMP_POL_COEF; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.m_aFit[i]);
	}
	return offset;
}

//! TecControlType serializer
int AvaSpec::serialize(const TecControlType& obj, uint8_t *buffer, int length) {
	if(length < TECCONTROL_LEN)
		throw SpectroExcept("AvaSpec::serialize(const TecControlType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, bool, obj.m_Enable);
	SERIALIZE_VAR(buffer, offset, float, obj.m_Setpoint);
	for(size_t i = 0; i < NR_DAC_POL_COEF; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.m_aFit[i]);
	}
	return offset;
}

//! ProcessControlType serializer
int AvaSpec::serialize(const ProcessControlType& obj, uint8_t *buffer, int length) {
	if(length < PROCESSCONTROL_LEN)
		throw SpectroExcept("AvaSpec::serialize(const ProcessControlType&): buffer too short");
	int offset = 0;
	for(size_t i = 0; i < 2; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.AnalogLow[i]);
	}
	for(size_t i = 0; i < 2; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.AnalogHigh[i]);
	}
	for(size_t i = 0; i < 10; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.DigitalLow[i]);
	}
	for(size_t i = 0; i < 10; i++) {
		SERIALIZE_VAR(buffer, offset, float, obj.DigitalHigh[i]);
	}
	return offset;
}

//! DeviceConfigType serializer
int AvaSpec::serialize(const DeviceConfigType& obj, uint8_t *buffer, int length) {
	if(length < DEVICECONFIG_LEN)
		throw SpectroExcept("AvaSpec::serialize(const DeviceConfigType&): buffer too short");
	int offset = 0;
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_Len);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj.m_ConfigVersion);
	SERIALIZE_STR(buffer, offset, obj.m_aUserFriendlyId, USER_ID_LEN);
	SERIALIZE_OBJ(buffer, offset, obj.m_Detector, length);
	SERIALIZE_OBJ(buffer, offset, obj.m_Irradiance, length);
	SERIALIZE_OBJ(buffer, offset, obj.m_Reflectance, length);
	SERIALIZE_OBJ(buffer, offset, obj.m_SpectrumCorrect, length);
	SERIALIZE_OBJ(buffer, offset, obj.m_StandAlone, length);
	for(size_t i = 0; i < MAX_TEMP_SENSORS; i++) {
		SERIALIZE_OBJ(buffer, offset, obj.m_aTemperature[i], length);
	}
	SERIALIZE_OBJ(buffer, offset, obj.m_TecControl, length);
	SERIALIZE_OBJ(buffer, offset, obj.m_ProcessControl, length);
	return offset;
}

//! AvsIdentityType unserializer
int AvaSpec::unserialize(AvsIdentityType& obj, const uint8_t *buffer, int length) {
	if(length < AVSIDENTITY_LEN)
		throw SpectroExcept("AvaSpec::unserialize(AvsIdentityType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj.m_as5216_version);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj.m_fpga_version);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj.m_hw_version);
	UNSERIALIZE_STR(buffer, offset, obj.m_SerialId, AVS_SERIAL_LEN);
	UNSERIALIZE_STR(buffer, offset, obj.m_UserId, USER_ID_LEN);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.Status);
	return offset;
}

//! DarkCorrectionType unserializer
int AvaSpec::unserialize(DarkCorrectionType& obj, const uint8_t *buffer, int length) {
	if(length < DARKCORRECTION_LEN)
		throw SpectroExcept("AvaSpec::unserialize(DarkCorrectionType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_Enable);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_ForgetPercentage);
	return offset;
}

//! SmoothingType unserializer
int AvaSpec::unserialize(SmoothingType& obj, const uint8_t *buffer, int length) {
	if(length < SMOOTHING_LEN)
		throw SpectroExcept("AvaSpec::unserialize(SmoothingType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_SmoothPix);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SmoothModel);
	return offset;
}

//! TriggerType unserializer
int AvaSpec::unserialize(TriggerType& obj, const uint8_t *buffer, int length) {
	if(length < TRIGGER_LEN)
		throw SpectroExcept("AvaSpec::unserialize(TriggerType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_Mode);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_Source);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SourceType);
	return offset;
}

//! ControlSettingsType unserializer
int AvaSpec::unserialize(ControlSettingsType& obj, const uint8_t *buffer, int length) {
	if(length < CONTROLSETTINGS_LEN)
		throw SpectroExcept("AvaSpec::unserialize(ControlSettingsType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_StrobeControl);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj.m_LaserDelay);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj.m_LaserWidth);
	UNSERIALIZE_VAR(buffer, offset, float, obj.m_LaserWaveLength);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_StoreToRam);
	return offset;
}

//! MeasConfigType unserializer
int AvaSpec::unserialize(MeasConfigType& obj, const uint8_t *buffer, int length) {
	if(length < MEASCONFIG_LEN)
		throw SpectroExcept("AvaSpec::unserialize(MeasConfigType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_StartPixel);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_StopPixel);
	UNSERIALIZE_VAR(buffer, offset, float, obj.m_IntegrationTime);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj.m_IntegrationDelay);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj.m_NrAverages);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_CorDynDark, length);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_Smoothing, length);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SaturationDetection);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_Trigger, length);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_Control, length);
	return offset;
}

//! DetectorType unserializer
int AvaSpec::unserialize(DetectorType& obj, const uint8_t *buffer, int length) {
	if(length < DETECTOR_LEN)
		throw SpectroExcept("AvaSpec::unserialize(DetectorType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SensorType);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_NrPixels);
	for(size_t i = 0; i < NR_WAVELEN_POL_COEF; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.m_aFit[i]);
	}
	UNSERIALIZE_VAR(buffer, offset, bool, obj.m_NLEnable);
	for(size_t i = 0; i < NR_NONLIN_POL_COEF; i++) {
		UNSERIALIZE_VAR(buffer, offset, double, obj.m_aNLCorrect[i]);
	}
	UNSERIALIZE_VAR(buffer, offset, double, obj.m_aLowNLCounts);
	UNSERIALIZE_VAR(buffer, offset, double, obj.m_aHighNLCounts);
	for(size_t i = 0; i < MAX_VIDEO_CHANNELS; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.m_Gain[i]);
	}
	UNSERIALIZE_VAR(buffer, offset, float, obj.m_Reserved);
	for(size_t i = 0; i < MAX_VIDEO_CHANNELS; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.m_Offset[i]);
	}
	UNSERIALIZE_VAR(buffer, offset, float, obj.m_ExtOffset);
	for(size_t i = 0; i < NR_DEFECTIVE_PIXELS; i++) {
		UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_DefectivePixels[i]);
	}
	return offset;
}

//! SpectrumCalibrationType unserializer
int AvaSpec::unserialize(SpectrumCalibrationType& obj, const uint8_t *buffer, int length) {
	if(length < SPECTRUMCALIBRATION_LEN)
		throw SpectroExcept("AvaSpec::unserialize(SpectrumCalibrationType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_OBJ(buffer, offset, obj.m_Smoothing, length);
	UNSERIALIZE_VAR(buffer, offset, float, obj.m_CalInttime);
	for(size_t i = 0; i < MAX_NR_PIXELS; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.m_aCalibConvers[i]);
	}
	return offset;
}

//! IrradianceType unserializer
	int AvaSpec::unserialize(IrradianceType& obj, const uint8_t *buffer, int length) {
	if(length < IRRADIANCE_LEN)
		throw SpectroExcept("AvaSpec::unserialize(IrradianceType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_OBJ(buffer, offset, obj.m_IntensityCalib, length);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_CalibrationType);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj.m_FiberDiameter);
	return offset;
}

//! SpectrumCorrectionType unserializer
int AvaSpec::unserialize(SpectrumCorrectionType& obj, const uint8_t *buffer, int length) {
	if(length < SPECTRUMCORRECTION_LEN)
		throw SpectroExcept("AvaSpec::unserialize(SpectrumCorrectionType&): buffer too short");
	int offset = 0;
	for(size_t i = 0; i < MAX_NR_PIXELS; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.m_aSpectrumCorrect[i]);
	}
	return offset;
}

//! TimeStampType unserializer
int AvaSpec::unserialize(TimeStampType& obj, const uint8_t *buffer, int length) {
	if(length < TIMESTAMP_LEN)
		throw SpectroExcept("AvaSpec::unserialize(TimeStampType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_Date);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_Time);
	return offset;
}

//! SDCardType unserializer
int AvaSpec::unserialize(SDCardType& obj, const uint8_t *buffer, int length) {
	if(length < SDCARD_LEN)
		throw SpectroExcept("AvaSpec::unserialize(SDCardType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, bool, obj.m_Enable);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj.m_SpectrumType);
	UNSERIALIZE_STR(buffer, offset, obj.m_aFileRootName, ROOT_NAME_LEN);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_TimeStamp, length);
	return offset;
}

//! StandAloneType unserializer
int AvaSpec::unserialize(StandAloneType& obj, const uint8_t *buffer, int length) {
	if(length < STANDALONE_LEN)
		throw SpectroExcept("AvaSpec::unserialize(StandAloneType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, bool, obj.m_Enable);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_Meas, length);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj.m_Nmsr);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_SDCard, length);
	return offset;
}

//! TempSensorType unserializer
int AvaSpec::unserialize(TempSensorType& obj, const uint8_t *buffer, int length) {
	if(length < TEMPSENSOR_LEN)
		throw SpectroExcept("AvaSpec::unserialize(TempSensorType&): buffer too short");
	int offset = 0;
	for(size_t i = 0; i < NR_TEMP_POL_COEF; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.m_aFit[i]);
	}
	return offset;
}

//! TecControlType unserializer
int AvaSpec::unserialize(TecControlType& obj, const uint8_t *buffer, int length) {
	if(length < TECCONTROL_LEN)
		throw SpectroExcept("AvaSpec::unserialize(TecControlType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, bool, obj.m_Enable);
	UNSERIALIZE_VAR(buffer, offset, float, obj.m_Setpoint);
	for(size_t i = 0; i < NR_DAC_POL_COEF; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.m_aFit[i]);
	}
	return offset;
}

//! ProcessControlType unserializer
int AvaSpec::unserialize(ProcessControlType& obj, const uint8_t *buffer, int length) {
	if(length < PROCESSCONTROL_LEN)
		throw SpectroExcept("AvaSpec::unserialize(ProcessControlType&): buffer too short");
	int offset = 0;
	for(size_t i = 0; i < 2; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.AnalogLow[i]);
	}
	for(size_t i = 0; i < 2; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.AnalogHigh[i]);
	}
	for(size_t i = 0; i < 10; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.DigitalLow[i]);
	}
	for(size_t i = 0; i < 10; i++) {
		UNSERIALIZE_VAR(buffer, offset, float, obj.DigitalHigh[i]);
	}
	return offset;
}

//! DeviceConfigType unserializer
int AvaSpec::unserialize(DeviceConfigType& obj, const uint8_t *buffer, int length) {
	if(length < DEVICECONFIG_LEN)
		throw SpectroExcept("AvaSpec::unserialize(DeviceConfigType&): buffer too short");
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_Len);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj.m_ConfigVersion);
	UNSERIALIZE_STR(buffer, offset, obj.m_aUserFriendlyId, USER_ID_LEN);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_Detector, length);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_Irradiance, length);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_Reflectance, length);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_SpectrumCorrect, length);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_StandAlone, length);
	for(size_t i = 0; i < MAX_TEMP_SENSORS; i++) {
		UNSERIALIZE_OBJ(buffer, offset, obj.m_aTemperature[i], length);
	}
	UNSERIALIZE_OBJ(buffer, offset, obj.m_TecControl, length);
	UNSERIALIZE_OBJ(buffer, offset, obj.m_ProcessControl, length);
	return offset;
}
