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


#ifndef OOSpectrometer_H
#define OOSpectrometer_H

#include <tango.h>
#include <omnithread.h>
#include <api/SeaBreezeWrapper.h>


namespace OOSpectrometer_ns
{

// Wrapper of wrapper
class WrapperOfWrapper;
extern WrapperOfWrapper ww_main;

// Acquisition thread
class SpecThread;


class OOSpectrometer : public TANGO_BASE_CLASS
{
public:
    // Acquisition thread
    SpecThread* acq;

    // Spectrometer ID
    int spec_id;

    // Database instance
    Tango::Database* db;

    // DB property names
    std::string prop_inttime;
    std::string prop_darksub;
    std::string prop_tecenable;
    std::string prop_tecsetpoint;

//    Device property data members
public:
    //    SerialNumber:    Serial number of the spectrometer we want to use on the OOSpectroProxy.
    string    serialNumber;
    //    BufferSize:    Number of spectra to hold in memory (0 means no buffer)
    Tango::DevULong    bufferSize;

    bool    mandatoryNotDefined;

//    Attribute data members
public:
    // Firmware version and model number
    Tango::DevString *serial;
    Tango::DevString *firmwareVersion;
    Tango::DevString *spectrometerModel;

    // Scalar attributes
    Tango::DevFloat   *attr_IntegrationTime_read;
    Tango::DevULong   *attr_ScansToAverage_read;
    Tango::DevBoolean *attr_enableElectricalDarkCorrection_read;
    Tango::DevULong   *attr_BoxcarWidth_read;
    Tango::DevBoolean *attr_enableBackgroundSubtraction_read;
    Tango::DevBoolean *attr_enableNLCorrection_read;
    Tango::DevDouble  *attr_boardTemperature_read;
    Tango::DevBoolean *attr_enableTEC_read;
    Tango::DevDouble  *attr_TECSetpoint_read;
    Tango::DevDouble  *attr_TECTemperature_read;
    Tango::DevBoolean *attr_enableTrigger_read;
    Tango::DevULong   *attr_timeout_read;
    Tango::DevDouble  *attr_Spectrum_read;
    size_t attr_Spectrum_read_len;

    // Spectrum attributes
    Tango::DevVarDoubleArray *wavelength;
    Tango::DevVarDoubleArray *background; // Background spectrum
    Tango::DevVarDoubleArray *spectrum; // Spectrum buffer
    omni_mutex* spectrum_lock; // Spectrum access lock
    bool updateBackground;
    bool background_ok;

//    Constructors and destructors
public:
    OOSpectrometer(Tango::DeviceClass *cl, const string& _serialNumber, string &s) : serialNumber(_serialNumber), TANGO_BASE_CLASS(cl, s.c_str()) { init_device(); }
    OOSpectrometer(Tango::DeviceClass *cl, const string& _serialNumber, const char *s) : serialNumber(_serialNumber), TANGO_BASE_CLASS(cl, s) { init_device(); }
    OOSpectrometer(Tango::DeviceClass *cl, const string& _serialNumber, const char *s, const char *d) : serialNumber(_serialNumber), TANGO_BASE_CLASS(cl, s, d) { init_device(); }
    ~OOSpectrometer() { delete_device(); };

//    Miscellaneous methods
public:
    // Delete device
    void delete_device();

    // Initialize the device
    virtual void init_device();

    // Get device property
    void get_device_property();

    // Always executed hook
    virtual void always_executed_hook() {}

    // Add dynamic attributes
    void add_dynamic_attributes();
};


// Acquisition thread
class SpecThread : public omni_thread {
public:
    SpecThread(OOSpectrometer* parent) :
        _parent(parent),
        _terminate(false)
    {
        start_undetached();
    }
    ~SpecThread() {};
    
protected:
    // Main loop
    void *run_undetached(void* arg);
    
public:
    // Terminate thread
    void terminate() { _terminate = true; }
    
private:
    // Private copy constructor
    SpecThread(const SpecThread& obj) : omni_thread() {};
    
    // Parent device
    OOSpectrometer *_parent;
    
    // Terminate flag
    bool _terminate;
};

}    //    End of namespace

#endif   //    OOSpectrometer_H
