//=============================================================================
//
//  This file is part of OOSpectrometer.
//
//  Foobar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Foobar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//=============================================================================

#define WOW_IMPLEMENTATION
#include <iostream>
#include <libusb-1.0/libusb.h>
#include "WrapperOfWrapper.h"


namespace OOSpectrometer_ns
{

// Device parameters constructor
DevParam::DevParam(long int _id, long int _spec_id, std::string _serial, uint32_t _integration) :
    timeout(0),
    boxcar_width(0),
    scans2average(1),
    dark_enable(false),
    tec_enable(false),
    tec_setpoint(0.0),
    trigger_enable(false)
{
    id = _id;
    spec_id = _spec_id;
    serial = _serial;
    integration_time = _integration;
}


// Device parameters collection add device method
void DevParameters::add_device(long int id, long int spec_id, std::string serial, uint32_t integration) {
    DevParam* dev = new DevParam(id, spec_id, serial, integration);
    _params.push_back(dev);
}


// Device parameters collection clear method
void DevParameters::clear() {
    for(size_t i = 0; i < _params.size(); i++) {
        delete _params[i];
    }
    _params.clear();
}


// Static instance of wrapper
WrapperOfWrapper* WrapperOfWrapper::_instance = NULL;


// WrapperOfWrapper constructor
WrapperOfWrapper::WrapperOfWrapper() : _spec_open(false)
{
    _param_lock = new omni_mutex();
    sbapi = SeaBreezeAPI::getInstance();
};


// WrapperOfWrapper destructor
WrapperOfWrapper::~WrapperOfWrapper() {
    // Close all spectrometers
    this->closeAllSpectrometers();
    // Delete parameters lock
    delete _param_lock;
}


// Get singleton instance of WrapperOfWrapper
WrapperOfWrapper* WrapperOfWrapper::instance() {
    if(_instance == NULL) {
        _instance = new WrapperOfWrapper();
    }
    return _instance;
}


// Shutdown method. Delete Wow object
void WrapperOfWrapper::shutdown() {
    if(NULL != _instance) {
        delete _instance;
        _instance = NULL;
    }
}


// Reset all Ocean Optics USB devices
void WrapperOfWrapper::usb_reset() {
    // Proceed only if the WrapperOfWrapper is not initialized
    if(_instance)
        return;

    // Init libusb
    libusb_init(NULL);

    // Get device list
    libusb_device **dev_list;
    ssize_t cnt = libusb_get_device_list(NULL, &dev_list);
    if(cnt < 0) {
        std::cout << "Failed to get usb device list (Error: " << libusb_strerror(libusb_error(cnt)) << ")" << std::endl;
        return;
    }
    std::cout << "Found " << cnt << " devices" << std::endl;

    struct libusb_device_descriptor desc;
    for(ssize_t i = 0; i < cnt; i++) {
        // Clear struct
        memset(&desc, 0, sizeof(desc));

        // Get device descriptor
        libusb_get_device_descriptor(dev_list[i], &desc);

        // Check vendor ID
        if(desc.idVendor == 0x2457) {
                // Ocean Optics device!
            libusb_device_handle *handle;
            int err = libusb_open(dev_list[i], &handle);
            if(err != 0) {
                std::cout << "Failed to open device " << std::hex << desc.idVendor << ":" << desc.idProduct << " (Error: " << libusb_strerror(libusb_error(err)) << ")" << std::endl;
            } else {
                std::cout << "Resetting device " << std::hex << desc.idVendor << ":" << desc.idProduct << "... ";
                err = libusb_reset_device(handle);
                if(err != 0)
                    std::cout << "Failed (Error: " << libusb_strerror(libusb_error(err)) << ")" << std::endl;
                else
                    std::cout << "OK!" << std::endl;
                libusb_close(handle);
            }
        }
    }

    // Clear device list
    libusb_free_device_list(dev_list, 1);

    // Close libusb
    libusb_exit(NULL);
}


// Rescan spectrometers
void WrapperOfWrapper::rescan() {
    // Close all spectrometers
    closeAllSpectrometers();

    // Probe spectrometers
    sbapi->probeDevices();

    // Re-open all spectrometers
    openAllSpectrometers();
}


// Open all spectrometers found
int WrapperOfWrapper::openAllSpectrometers() {

    // Clear parameters vector
    _dev_params.clear();

    // Get device IDs
    int num_ids = sbapi->getNumberOfDeviceIDs();
    long int ids[num_ids];
    sbapi->getDeviceIDs(ids, num_ids);

    int err_code = 0;
    int ans = 0;

    // Cycle over IDs and open spectrometers
    for(int i = 0; i < num_ids; i++) {

        // Open spectrometer
        err_code = 0;
        sbapi->openDevice(ids[i], &err_code);
        if(err_code != 0) {
            // Failed to open device
            log_error("Failed to open spectrometer with ID %d (Error code: %d)", ids[i], err_code);
            continue;
        }

        // Check if we have a serial number available
        err_code = 0;
        int num_ser = sbapi->getNumberOfSerialNumberFeatures(ids[i], &err_code);
        if(num_ser == 0 || err_code != 0) {
            // No serial number available
            log_warning("Skipping device with ID %d because of missing serial number (Error code: %d)", ids[i], err_code);
            sbapi->closeDevice(ids[i], &err_code);
            continue;
        }

        // Get serial number features IDs
        err_code = 0;
        long feat_id[num_ser];
        sbapi->getSerialNumberFeatures(ids[i], &err_code, feat_id, num_ser);
        if(err_code != 0) {
            log_error("Failed to get serial number features from device with ID %d (Error code: %d)", ids[i], err_code);
            sbapi->closeDevice(ids[i], &err_code);
            continue;
        }

        // Get serial number
        err_code = 0;
        char buffer[256];
        sbapi->getSerialNumber(ids[i], feat_id[0], &err_code, buffer, 256);
        if(err_code != 0) {
            log_error("Failed to get serial number from device with ID %d, feature ID %d (Error code: %d)", ids[i], feat_id[i], err_code);
            sbapi->closeDevice(ids[i], &err_code);
            continue;
        }

        // Get number of spectrometer features
        err_code = 0;
        int num_spec = sbapi->getNumberOfSpectrometerFeatures(ids[i], &err_code);
        if(num_spec == 0 || err_code != 0) {
            // No serial number available
            log_warning("Skipping device with ID %d because of missing spectrometer feature (Error code: %d)", ids[i], err_code);
            sbapi->closeDevice(ids[i], &err_code);
            continue;
        }

        // Get spectrometer feature ID
        err_code = 0;
        long spec_id[num_spec];
        sbapi->getSpectrometerFeatures(ids[i], &err_code, spec_id, num_spec);
        if(err_code != 0) {
            log_error("Failed to get spectrometer features from device with ID %d (Error code: %d)", ids[i], err_code);
            sbapi->closeDevice(ids[i], &err_code);
            continue;
        }

        // Set integration time to minimum time
        err_code = 0;
        unsigned long min_time = sbapi->spectrometerGetMinimumIntegrationTimeMicros(ids[i], spec_id[0], &err_code);
        sbapi->spectrometerSetIntegrationTimeMicros(ids[i], spec_id[0], &err_code, min_time);
        if(err_code != 0) {
            // Failure to set integration time
            log_error("Failed to set integration time to minimum on device with ID %d (Error code: %d)", ids[i], err_code);
            sbapi->closeDevice(ids[i], &err_code);
            continue;
        }

        // Make sure there's no external trigger set on device
        err_code = 0;
        sbapi->spectrometerSetTriggerMode(ids[i], spec_id[0], &err_code, 0);
        if(err_code != 0) {
            log_error("Failed to set trigger mode to internal on device with ID %d (Error code: %d)", ids[i], err_code);
            sbapi->closeDevice(ids[i], &err_code);
            continue;
        }

        {
            omni_mutex_lock access_lock(*_param_lock);
            _dev_params.add_device(ids[i], spec_id[0], std::string(buffer), min_time);
        }
    }

    if(_dev_params.size()) {
        _spec_open = true;
    }
}


// Close all spectrometers
void WrapperOfWrapper::closeAllSpectrometers() {
    if(_spec_open) {
        int err_code = 0;
        for(size_t i = 0; i < _dev_params.size(); i++) {
            omni_mutex_lock access_lock(_dev_params[i].lock);
            sbapi->closeDevice(_dev_params[i].id, &err_code);
        }
        _dev_params.clear();
        _spec_open = false;
    }
}


// Find spectrometer ID by serial number
int WrapperOfWrapper::findSpectrometer(const std::string& serial) {
    for(size_t i = 0; i < _dev_params.size(); i++)
        if(_dev_params[i].serial == serial)
            return i;
    return -1;
}


// Integration time
void WrapperOfWrapper::setIntegrationTime(int id, uint32_t usec) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Set integration time
        int err_code = 0;
        sbapi->spectrometerSetIntegrationTimeMicros(_dev_params[id].id, _dev_params[id].spec_id, &err_code, (long int)usec);
        if(err_code != 0) {
            throw WoWException("WoW::setIntegrationTime(): failed to set integration time (Error: %d)", err_code);
        }
        _dev_params[id].integration_time = usec;

    } else {
        throw WoWException("WoW::setIntegrationTime(): invalid ID");
    }
}

uint32_t WrapperOfWrapper::getIntegrationTime(int id) {
    // Check ID
    if(check_id(id)) {
        return _dev_params[id].integration_time;

    } else {
        throw WoWException("WoW::getIntegrationTime(): invalid ID");
    }
}


// Get minimum and maximum integration time
uint32_t WrapperOfWrapper::getMinimumIntegrationTime(int id) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get minimum integration time
        int err_code = 0;
        unsigned long time = sbapi->spectrometerGetMinimumIntegrationTimeMicros(_dev_params[id].id, _dev_params[id].spec_id, &err_code);
        if(err_code != 0) {
            throw WoWException("WoW::getMinimumIntegrationTime(): failed to get minimum integration time (Error: %d)", err_code);
        }
        return static_cast<uint32_t>(time);

    } else {
        throw WoWException("WoW::getMinimumIntegrationTime(): invalid ID");
    }
}

uint32_t WrapperOfWrapper::getMaximumIntegrationTime(int id) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get maximum integration time
        int err_code = 0;
        unsigned long time = sbapi->spectrometerGetMaximumIntegrationTimeMicros(_dev_params[id].id, _dev_params[id].spec_id, &err_code);
        if(err_code != 0) {
            throw WoWException("WoW::getMaximumIntegrationTime(): failed to get maximum integration time (Error: %d)", err_code);
        }
        return static_cast<uint32_t>(time);

    } else {
        throw WoWException("WoW::getMaximumIntegrationTime(): invalid ID");
    }
}


// Scans to average
void WrapperOfWrapper::setScansToAverage(int id, uint16_t n) {
    // Check ID
    if(check_id(id)) {
        if(n < 1) {
            n = 1;
        }
        // Set scan to average
        _dev_params[id].scans2average = n;

    } else {
        throw WoWException("WoW::setScansToAverage(): invalid ID");
    }
}

uint16_t WrapperOfWrapper::getScansToAverage(int id)const {
    // Check ID
    if(check_id(id)) {
        return _dev_params[id].scans2average;

    } else {
        throw WoWException("WoW::getScansToAverage(): invalid ID");
    }
}


// Boxcar width
void WrapperOfWrapper::setBoxcarWidth(int id, uint8_t n) {
    // Check ID
    if(check_id(id)) {
        _dev_params[id].boxcar_width = n;

    } else {
        throw WoWException("WoW::setBoxcarWidth(): invalid ID");
    }
}

uint8_t WrapperOfWrapper::getBoxcarWidth(int id)const {
    // Check ID
    if(check_id(id)) {
        return _dev_params[id].boxcar_width;

    } else {
        throw WoWException("WoW::getBoxcarWidth(): invalid ID");
    }
}


// Enable electrical dark correction
void WrapperOfWrapper::setCorrectForElectricalDark(int id, bool enable) {
    // Check ID
    if(check_id(id)) {
        if(enable) {
            // Lock spectrometer
            omni_mutex_lock access_lock(_dev_params[id].lock);
            // Get electrical dark correction pixel count
            int err_code = 0;
            int count = sbapi->spectrometerGetElectricDarkPixelCount(_dev_params[id].id, _dev_params[id].spec_id, &err_code);
            if(err_code) {
                _dev_params[id].dark_enable = false;
                throw WoWException("WoW::setCorrectForElectricalDark(): failed to check the number of dark pixels");
            }
            if(count == 0) {
                _dev_params[id].dark_enable = false;
                throw WoWException("WoW::setCorrectForElectricalDark(): electric dark compensation not supported");
            }
            // Get pixel IDs
            err_code = 0;
            int indices[count];
            sbapi->spectrometerGetElectricDarkPixelIndices(_dev_params[id].id, _dev_params[id].spec_id, &err_code, indices, count);
            if(err_code) {
                _dev_params[id].dark_enable = false;
                throw WoWException("WoW::setCorrectForElectricalDark(): failed to get the indices of dark pixels");
            }
            {
                omni_mutex_lock p_lock(*_param_lock);
                _dev_params[id].dark_indices.clear();
                for(int i = 0; i < count; i++)
                    _dev_params[id].dark_indices.push_back(indices[i]);
            }
        }
        _dev_params[id].dark_enable = enable;

    } else {
        throw WoWException("WoW::setCorrectForElectricalDark(): invalid ID");
    }
}

bool WrapperOfWrapper::getCorrectForElectricalDark(int id)const {
    // Check ID
    if(check_id(id)) {
        return _dev_params[id].dark_enable;

    } else {
        throw WoWException("WoW::getCorrectForElectricalDark(): invalid ID");
    }
}

void WrapperOfWrapper::subtract_dark(double* spec, size_t len, const std::vector<int>* dark_pixels) {
    double background = 0.0;
    size_t count = 0;
    for(size_t i = 0; i < dark_pixels->size(); i++) {
        if((*dark_pixels)[i] < (int)len) {
            background += spec[(*dark_pixels)[i]];
            count++;
        }
    }
    if(count)
        background /= count;
    for(size_t i = 0; i < len; i++)
        spec[i] -= background;
}


// Get number of pixels (including dark ones)
uint32_t WrapperOfWrapper::getNumberOfPixels(int id) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get number of pixels
        int err_code = 0;
        int pixels = sbapi->spectrometerGetFormattedSpectrumLength(_dev_params[id].id, _dev_params[id].spec_id, &err_code);
        if(err_code) {
            throw WoWException("WoW::getNumberOfPixels(): failed to get number of pixels (Error: %d)", err_code);
        }
        return (uint32_t)pixels;

    } else {
        throw WoWException("WoW::getNumberOfPixels(): invalid ID");
    }
}


// Get wavelength
void WrapperOfWrapper::getWavelength(int id, double *data, size_t len) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get wavelength array
        int err_code = 0;
        sbapi->spectrometerGetWavelengths(_dev_params[id].id, _dev_params[id].spec_id, &err_code, data, len);
        if(err_code) {
            throw WoWException("WoW::getWavelength(): failed to get wavelengths (Error: %d)", err_code);
        }

    } else {
        throw WoWException("WoW::getWavelength(): invalid ID");
    }
}


// Get spectrum
void WrapperOfWrapper::getSpectrum(int id, double *data, size_t len) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get spectrum
        int err_code = 0;

        // Check if we need to average more than one spectrum
        if(_dev_params[id].scans2average > 1) {
            // Spectrum sum
            double *spectrum = new double[len];
            double *residual = new double[len];
            memset(spectrum, 0, sizeof(double)*len);
            memset(residual, 0, sizeof(double)*len);

            for(size_t i = 0; i < _dev_params[id].scans2average; i++) {
                // Sum UP the number of required spectra

                // 1) Get the spectrum
                err_code = 0;
                sbapi->spectrometerGetFormattedSpectrum(_dev_params[id].id, _dev_params[id].spec_id, &err_code, spectrum, len);
                if(err_code) {
                    throw WoWException("WoW::getSpectrum(): failed to get spectrum (Error: %d)", err_code);
                }

                // 2) Sum it up using the Kahan summing algorithm
                for(size_t j = 0; j < len; j++) {
                    double y = spectrum[j] - residual[j];
                    double t = data[j] + y;
                    residual[j] = (t - data[j]) - y;
                    data[j] = t;
                }
            }
            delete [] spectrum;
            delete [] residual;

            // Divide by the number of spectr
            for(size_t j = 0; j < len; j++) {
                data[j] /= _dev_params[id].scans2average;
            }

        } else {
            // Acquire a single spectrum
            sbapi->spectrometerGetFormattedSpectrum(_dev_params[id].id, _dev_params[id].spec_id, &err_code, data, len);
            if(err_code) {
                throw WoWException("WoW::getSpectrum(): failed to get spectrum (Error: %d)", err_code);
            }
        }

        // Subtract electric dark if enabled
        if(_dev_params[id].dark_enable) {
            subtract_dark(data, len, &(_dev_params[id].dark_indices));
        }

        // Apply BOXCAR average if needed
        if(_dev_params[id].boxcar_width > 1) {
            compute_boxcar(data, len, _dev_params[id].boxcar_width);
        }

    } else {
        throw WoWException("WoW::getSpectrum(): invalid ID");
    }
}


void WrapperOfWrapper::compute_boxcar(double *data, size_t len, uint8_t window) {

    double *result = new double[len];
    if(!result) {
        log_error("WoW::compute_boxcar(): failed to allocate memory to compute boxcar average");
        return;
    }

    // Check if window is even or odd
    size_t left = 0, right = 0;
    if(window % 2 == 1) {
        // ODD
        left = window / 2;
        right = left;
    } else {
        // EVEN
        left = window / 2;
        right = left - 1;
    }

    for(size_t i = 0; i < len; i++) {
        result[i] = 0.0;
        if(i < left) {
            for(size_t j = 0; j < i + right; j++) {
                result[i] += data[j];
            }
            result[i] /= (i + right + 1);

        } else if(i >= (len - right)) {
            for(size_t j = i - left; j < len; j++) {
                result[i] += data[j];
            }
            result[i] /= (len - i + left);

        } else {
            for(size_t j = i - left; j <= i + right; j++) {
                result[i] += data[j];
            }
            result[i] /= window;
        }
    }
    // Copy buffer
    memcpy(data, result, sizeof(double)*len);
    delete [] result;
}


// Enable edge-sensitive external trigger
void WrapperOfWrapper::setEdgeTrigger(int id, bool enable) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);

        // Set trigger mode
        bool use_fpga = false;
        long fpga_id = 0;
        int err_code = 0;
        char buffer[128];
        sbapi->getDeviceType(_dev_params[id].id, &err_code, buffer, 127);
        if(!err_code && strcmp(buffer, "USB4000") == 0) {
            // Use FPGA register
            // Check if we have a fpga register feature available
            err_code = 0;
            int num_fr = sbapi->getNumberOfFPGARegisterFeatures(_dev_params[id].id, &err_code);
            if(num_fr == 0 || err_code != 0) {
                // No firmware version available
                log_warning("Device with ID %d is missing fpga register access (Error code: %d)", _dev_params[id].id, err_code);

            } else {
                // Get fpga register features IDs
                err_code = 0;
                sbapi->getFPGARegisterFeatures(_dev_params[id].id, &err_code, &fpga_id, 1);
                if(err_code != 0) {
                    log_error("Failed to get fpga register features from device with ID %d (Error code: %d)", _dev_params[id].id, err_code);
                } else {
                    use_fpga = true;
                }
            }
        }

        if(use_fpga) {
            // Set fpga register
            err_code = 0;
            sbapi->FPGAWriteRegister(_dev_params[id].id, fpga_id, &err_code, 0x2C, (enable) ? 4 : 0);
            if(err_code != 0) {
                throw WoWException("WoW::setEdgeTrigger(): failed to set the trigger mode (Error: %d)", err_code);
            }

        } else {
            sbapi->spectrometerSetTriggerMode(_dev_params[id].id, _dev_params[id].spec_id, &err_code, (enable) ? 3 : 0);
            if(err_code) {
                throw WoWException("WoW::setEdgeTrigger(): failed to set the trigger mode (Error: %d)", err_code);
            }
        }
        _dev_params[id].trigger_enable = enable;

    } else {
        throw WoWException("WoW::setEdgeTrigger(): invalid ID");
    }
}

bool WrapperOfWrapper::getEdgeTrigger(int id)const {
    // Check ID
    if(check_id(id)) {
        return _dev_params[id].trigger_enable;

    } else {
        throw WoWException("WoW::getEdgeTrigger(): invalid ID");
    }
}


// Enable edge-sensitive external trigger
void WrapperOfWrapper::setTimeout(int id, unsigned int timeout) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);

        // Set trigger mode
        int err_code = 0;
        sbapi->spectrometerSetUSBTimeout(_dev_params[id].id, _dev_params[id].spec_id, &err_code, timeout);
        if(err_code) {
            throw WoWException("WoW::setTimeout(): failed to set USB timeout (Error: %d)", err_code);
        }
        _dev_params[id].timeout = timeout;

    } else {
        throw WoWException("WoW::setTimeout(): invalid ID");
    }
}

unsigned int WrapperOfWrapper::getTimeout(int id)const {
    // Check ID
    if(check_id(id)) {
        return _dev_params[id].timeout;

    } else {
        throw WoWException("WoW::getTimeout(): invalid ID");
    }
}


// Return serial number of spectrometer
size_t WrapperOfWrapper::getSerialNumber(int id, char* str, size_t len) {
    // Check ID
    if(check_id(id)) {
        // Copy serial from _dev_params
        if(str && len > 0) {
            strncpy(str, _dev_params[id].serial.c_str(), len);
            str[len-1] = '\0';
        }

        return _dev_params[id].serial.size();

    } else {
        throw WoWException("WoW::getSerialNumber(): invalid ID");
    }
}


// Return model of spectrometer
size_t WrapperOfWrapper::getModel(int id, char* str, size_t len) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get model
        int err_code = 0;
        sbapi->getDeviceType(_dev_params[id].id, &err_code, str, len);
        if(err_code) {
            throw WoWException("WoW::getModel(): failed to get device model (Error: %d)", err_code);
        }
        return strlen(str);

    } else {
        throw WoWException("WoW::getModel(): invalid ID");
    }
}


// Return firmware version of spectrometer
size_t WrapperOfWrapper::getFirmwareVersion(int id, char* str, size_t len) {
    int err_code = 0;

    // Check ID
    if(check_id(id)) {
        // Check if we have a firmware version available
        err_code = 0;
        int num_fv = sbapi->getNumberOfFirmwareVersionFeatures(_dev_params[id].id, &err_code);
        if(num_fv == 0 || err_code != 0) {
            // No firmware version available
            log_warning("Device with ID %d is missing firmware version (Error code: %d)", _dev_params[id].id, err_code);
            strncpy(str, "Unknown", len);
            return strlen("Unknown");
        }

        // Get firmware version features IDs
        err_code = 0;
        long feat_id;
        sbapi->getFirmwareVersionFeatures(_dev_params[id].id, &err_code, &feat_id, 1);
        if(err_code != 0) {
            log_error("Failed to get firmware version features from device with ID %d (Error code: %d)", _dev_params[id].id, err_code);
            strncpy(str, "Unknown", len);
            return strlen("Unknown");
        }

        // Get firmware version
        err_code = 0;
        size_t sz = sbapi->getFirmwareVersion(_dev_params[id].id, feat_id, &err_code, str, len);
        if(err_code != 0) {
            log_error("Failed to get firmware version from device with ID %d, feature ID %d (Error code: %d)", _dev_params[id].id, feat_id, err_code);
            strncpy(str, "Unknown", len);
            return strlen("Unknown");
        }
        return sz;

    } else {
        throw WoWException("WoW::getFirmwareVersion(): invalid ID");
    }
}


// Get board temperature
double WrapperOfWrapper::getBoardTemperature(int id) {
    double temperature = ::nan("");

    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Check availability of board temperature
        int err_code = 0;
        int num_feat = sbapi->getNumberOfTemperatureFeatures(_dev_params[id].id, &err_code);
        if(err_code) {
            log_error("Failed to get temperature feature count (Error: %d)", err_code);

        } else {
            if(num_feat > 0) {
                err_code = 0;
                long feat_id;
                sbapi->getTemperatureFeatures(_dev_params[id].id, &err_code, &feat_id, 1);
                if(err_code) {
                    log_error("Failed to get temperature feature (Error: %d)", err_code);

                } else {
                    err_code = 0;
                    uint8_t count = sbapi->temperatureCountGet(_dev_params[id].id, feat_id, &err_code);
                    if(err_code) {
                        log_error("Failed to get temperature sensor count (Error: %d)", err_code);

                    } else {
                        if(count > 0) {
                            err_code = 0;
                            double t = sbapi->temperatureGet(_dev_params[id].id, feat_id, &err_code, 0);
                            if(err_code) {
                                log_error("Failed to get temperature (Error: %d)", err_code);
                            } else {
                                temperature = t;
                            }
                        }
                    }
                }
            }
        }
        return temperature;

    } else {
        throw WoWException("WoW::getBoardTemperature(): invalid ID");
    }
}


// Check if thermoelectric cooler is available
bool WrapperOfWrapper::coolingAvailable(int id) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get availability of TEC
        int err_code = 0;
        int tec = sbapi->getNumberOfThermoElectricFeatures(_dev_params[id].id, &err_code);
        if(err_code) {
            throw WoWException("WoW::coolingAvailable(): failed to get TEC features count");
        }
        return (tec != 0);

    } else {
        throw WoWException("WoW::coolingAvailable(): invalid ID");
    }
}


// Enable cooler
void WrapperOfWrapper::setTECEnable(int id, bool enable) {
    // Check ID
    if(check_id(id) && coolingAvailable(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get TEC feature ID
        long tec_id;
        int err_code = 0;
        sbapi->getThermoElectricFeatures(_dev_params[id].id, &err_code, &tec_id, 1);
        if(err_code) {
            throw WoWException("WoW::setTECEnable(): failed to get TEC feature ID (Error: %d)", err_code);
        }
        // Enable/Disable TEC
        err_code = 0;
        sbapi->tecSetEnable(_dev_params[id].id, tec_id, &err_code, (uint8_t)enable);
        if(err_code) {
            throw WoWException("WoW::setTECEnable(): failed to %s TEC (Error: %d)", (enable) ? "enable" : "disable", err_code);
        }
        _dev_params[id].tec_enable = enable;

    } else {
        throw WoWException("WoW::setTECEnable(): invalid ID");
    }
}

bool WrapperOfWrapper::getTECEnable(int id) {
    // Check ID
    if(check_id(id)) {
        return _dev_params[id].tec_enable;

    } else {
        throw WoWException("WoW::getTECEnable(): invalid ID");
    }
}


// Set setpoint in celsius
void WrapperOfWrapper::setTECSetpoint(int id, double temperature) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get TEC feature ID
        long tec_id;
        int err_code = 0;
        sbapi->getThermoElectricFeatures(_dev_params[id].id, &err_code, &tec_id, 1);
        if(err_code) {
            throw WoWException("WoW::setTECSetpoint(): failed to get TEC feature ID (Error: %d)", err_code);
        }
        // Set TEC setpoint
        err_code = 0;
        sbapi->tecSetTemperatureSetpointDegreesC(_dev_params[id].id, tec_id, &err_code, temperature);
        if(err_code) {
            throw WoWException("WoW::setTECSetpoint(): failed to set TEC setpoint (Error: %d)", err_code);
        }
        _dev_params[id].tec_setpoint = temperature;

    } else {
        throw WoWException("WoW::setTECSetpoint(): invalid ID");
    }
}

double WrapperOfWrapper::getTECSetpoint(int id) {
    // Check ID
    if(check_id(id)) {
        return _dev_params[id].tec_setpoint;

    } else {
        throw WoWException("WoW::getTECSetpoint(): invalid ID");
    }
}


// Get detector temperature
double WrapperOfWrapper::getTECTemperature(int id) {
    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);
        // Get TEC feature ID
        long tec_id;
        int err_code = 0;
        sbapi->getThermoElectricFeatures(_dev_params[id].id, &err_code, &tec_id, 1);
        if(err_code) {
            throw WoWException("WoW::getTECTemperature(): failed to get TEC feature ID (Error: %d)", err_code);
        }
        // Read TEC temperature
        err_code = 0;
        double temperature = sbapi->tecReadTemperatureDegreesC(_dev_params[id].id, tec_id, &err_code);
        if(err_code) {
            throw WoWException("WoW::getTECTemperature(): failed to get TEC temperature (Error: %d)", err_code);
        }
        return temperature;

    } else {
        throw WoWException("WoW::getTECTemperature(): invalid ID");
    }
}


// Read FPGA register
unsigned int WrapperOfWrapper::readFPGARegister(int id, uint8_t address) {
    int err_code = 0;

    // Check ID
    if(check_id(id)) {
        // Lock spectrometer
        omni_mutex_lock access_lock(_dev_params[id].lock);

        // Check if we have a fpga register feature available
        err_code = 0;
        int num_fr = sbapi->getNumberOfFPGARegisterFeatures(_dev_params[id].id, &err_code);
        if(num_fr == 0 || err_code != 0) {
            // No firmware version available
            log_warning("Device with ID %d is missing fpga register access (Error code: %d)", _dev_params[id].id, err_code);
            return 0;
        }

        // Get fpga register features IDs
        err_code = 0;
        long feat_id;
        sbapi->getFPGARegisterFeatures(_dev_params[id].id, &err_code, &feat_id, 1);
        if(err_code != 0) {
            log_error("Failed to get fpga register features from device with ID %d (Error code: %d)", _dev_params[id].id, err_code);
            return 0;
        }

        // Get fpga register version
        err_code = 0;
        unsigned int value = sbapi->FPGAReadRegister(_dev_params[id].id, feat_id, &err_code, address);
        if(err_code != 0) {
            log_error("Failed to get register value from device with ID %d, feature ID %d (Error code: %d)", _dev_params[id].id, feat_id, err_code);
            return 0;
        }
        return value;

    } else {
        throw WoWException("WoW::getFirmwareVersion(): invalid ID");
    }
}

} // End of namespace
