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

#ifndef WRAPPER_OF_WRAPPER_H
#define WRAPPER_OF_WRAPPER_H

#include <exception>
#include <string>
#include <string.h>
#include <vector>
#include <cmath>
#include <stdint.h>
#include <cstdarg>
#include <cstdio>
#include <omnithread.h>
#include <api/seabreezeapi/SeaBreezeAPI.h>
#include <GeneralLogger.h>


namespace OOSpectrometer_ns
{

// Extern declaration of global wrapper object
#ifndef WOW_IMPLEMENTATION
class WrapperOfWrapper;
extern WrapperOfWrapper wow_main;
#endif


#define WOWEXCEPTION_MSG_LEN 256
// Wrapper of Wrapper exception
class WoWException : public std::exception {
public:
    //! Constructor
    /*! Constructor of the exception. Can format an error message.
    *  @param f the format string (as in printf)
    *  @param ... variable parameters, corresponding to the format string.
    */
    WoWException(const char* f, ...) {
        va_list ap;
        va_start(ap, f);
        vsnprintf(_msg, WOWEXCEPTION_MSG_LEN, f, ap);
        va_end(ap);
    }
    WoWException(const char* f, va_list ap) {
        vsnprintf(_msg, WOWEXCEPTION_MSG_LEN, f, ap);
    }

    //! what(), return the error string
    virtual const char* what() const throw() {
        return _msg;
    }

private:
  char _msg[WOWEXCEPTION_MSG_LEN];
};


// Device parameters class
class DevParam {
public:
    DevParam(long int _id, long int _spec_id, std::string _serial, uint32_t _integration);
    ~DevParam() {};

public:
    // Device ID
    long int id;

    // Spectrometer feature ID
    long int spec_id;

    // Serial number
    std::string serial;

    // Timeout
    uint32_t timeout;

    // Integration time
    uint32_t integration_time;

    // Boxcar width
    uint8_t boxcar_width;

    // Scans to average
    uint16_t scans2average;

    // Correct for electical dark
    bool dark_enable;

    // Dark pixels indices
    std::vector<int> dark_indices;

    // TEC status
    bool tec_enable;

    // TEC setpoint
    double tec_setpoint;

    // Trigger enable
    bool trigger_enable;

    // Lock
    omni_mutex lock;
};


class DevParameters {
public:
    // Constructor and destructor
    DevParameters() {};
    ~DevParameters() { clear(); }

private:
    // Prohibit copy
    DevParameters(const DevParameters& obj) {}
    DevParameters& operator=(const DevParameters& obj) {}

public:
    void add_device(long int _id, long int _spec_id, std::string _serial, uint32_t _integration);
    DevParam& operator[](size_t i) { return *(_params[i]); }
    const DevParam& operator[](size_t i)const { return *(_params[i]); }
    size_t size()const { return _params.size(); }
    void clear();
private:
    std::vector<DevParam*> _params;
};


// Wrapper of Wrapper class
class WrapperOfWrapper : public GeneralLogger {
private:
    // Constructor
    WrapperOfWrapper();

    // Private copy constructor to prevent copying
    WrapperOfWrapper(const WrapperOfWrapper& obj) {};

public:
    static void usb_reset();

public:
    // Instance method
    static WrapperOfWrapper* instance();

    // Shutdown method
    static void shutdown();

    // Destructor
    ~WrapperOfWrapper();

    // Rescan spectrometers
    void rescan();

    // Open all spectrometers found
    int openAllSpectrometers();

    // Close all spectrometers
    void closeAllSpectrometers();

    // Find spectrometer ID
    int findSpectrometer(const std::string& serial);

    // Check if the ID is valid
    bool check_id(int id)const { return (id >= 0 && (unsigned int)id < _dev_params.size()); }

    // Get serials
    std::vector<std::string> getSerials()const {
        std::vector<std::string> _serials;
        for(size_t i = 0; i < _dev_params.size(); i++)
            _serials.push_back(_dev_params[i].serial);
        return _serials;
    }

// PUBLIC INTERFACE FOR SPECTROMETERS
public:
    // Integration time
    void setIntegrationTime(int id, uint32_t usec);
    uint32_t getIntegrationTime(int id);

    // Get minimum and maximum integration time
    uint32_t getMinimumIntegrationTime(int id);
    uint32_t getMaximumIntegrationTime(int id);

    // Scans to average
    void setScansToAverage(int id, uint16_t n);
    uint16_t getScansToAverage(int id)const;

    // Boxcar width
    void setBoxcarWidth(int id, uint8_t n);
    uint8_t getBoxcarWidth(int id)const;

    // Set electrical dark correction
    void setCorrectForElectricalDark(int id, bool enable);
    bool getCorrectForElectricalDark(int id)const;

    // Get number of pixels
    uint32_t getNumberOfPixels(int id);

    // Get wavelength
    void getWavelength(int id, double *data, size_t len);

    // Get spectrum
    void getSpectrum(int id, double *data, size_t len);

    // Enable edge-sensitive external trigger
    void setEdgeTrigger(int id, bool enable);
    bool getEdgeTrigger(int id)const;

    // Set timeout for hardware trigger
    void setTimeout(int id, unsigned int timeout);
    unsigned int getTimeout(int id)const;

    // Get serial number
    size_t getSerialNumber(int id, char* str, size_t len);

    // Return model of spectrometer
    size_t getModel(int id, char* str, size_t len);

    // Return firmware version of spectrometer
    size_t getFirmwareVersion(int id, char* str, size_t len);

    // Get board temperature
    double getBoardTemperature(int id);

    // Check if thermoelectric cooler is available
    bool coolingAvailable(int id);

    // Enable cooler
    void setTECEnable(int id, bool enable);
    bool getTECEnable(int id);

    // Set setpoint in celsius
    void setTECSetpoint(int id, double temperature);
    double getTECSetpoint(int id);

    // Get detector temperature
    double getTECTemperature(int id);

    // Read FPGA register
    unsigned int readFPGARegister(int id, uint8_t address);

protected:
    // Utility function to subtract dark noise
    void subtract_dark(double* spec, size_t len, const std::vector<int>* dark_pixels);

    // Compute boxcar average of a spectrum (WARNING: the average is computed in place!)
    void compute_boxcar(double* spec, size_t len, uint8_t window);

private:
    // SeaBreeze API
    SeaBreezeAPI* sbapi;

    // Flag to remember if spectrometers are open or not
    bool _spec_open;

    // List of device ids
    DevParameters _dev_params;

    // Parameter list lock
    omni_mutex* _param_lock;

    // Instance
    static WrapperOfWrapper* _instance;
};

} // End of namespace

#endif
