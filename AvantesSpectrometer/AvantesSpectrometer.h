//=============================================================================
//
//  This file is part of AvantesSpectrometer.
// 
//  AvantesSpectrometer is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  AvantesSpectrometer is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with AvantesSpectrometer.  If not, see <http://www.gnu.org/licenses/>.
//=============================================================================


#ifndef AvantesSpectrometer_H
#define AvantesSpectrometer_H

#include <tango.h>
#include <omnithread.h>
#include <libavs/avaspec.h>


namespace AvantesSpectrometer_ns
{

// Acquisition thread
class MeasureRetrigger;


class AvantesSpectrometer : public TANGO_BASE_CLASS
{
public:
    // Spectrometer ID
    AvsIdentityType* devid;

    // Spectrometer handle
    AvsHandle handle;

    // Spectrometer configuration
    MeasConfigType config;

    // Device configuration
    DeviceConfigType dev_config;

    // Database instance
    Tango::Database* db;

    // DB property names
    std::string prop_inttime;
    std::string prop_darksub;
    std::string prop_tecenable;
    std::string prop_tecsetpoint;

    // Cooling available flag
    bool coolingAvailable;

    // Old triggering scheme flag
    bool old_triggering;
    MeasureRetrigger* retrigger;

    // Attribute data members
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
    Tango::DevDouble  *attr_boardTemperature_read;
    Tango::DevBoolean *attr_enableTEC_read;
    Tango::DevDouble  *attr_TECSetpoint_read;
    Tango::DevDouble  *attr_TECTemperature_read;
    Tango::DevBoolean *attr_enableTrigger_read;
    Tango::DevDouble  *attr_Spectrum_read;
    size_t            attr_Spectrum_read_len;

    // Spectrum attributes
    Tango::DevVarDoubleArray* wavelength;
    Tango::DevVarDoubleArray* background; // Background spectrum
    Tango::DevVarDoubleArray* spectrum;   // Current spectrum
    omni_mutex* spectrum_lock;            // Spectrum access lock
    bool updateBackground;

//    Constructors and destructors
public:
    AvantesSpectrometer(Tango::DeviceClass *cl, AvsIdentityType* _devid, string &s) :
        TANGO_BASE_CLASS(cl, s.c_str()),
        devid(_devid)
    {
        init_device();
    }
    AvantesSpectrometer(Tango::DeviceClass *cl, AvsIdentityType* _devid, const char *s) : 
        TANGO_BASE_CLASS(cl, s),
        devid(_devid)
    {
        init_device();
    }
    AvantesSpectrometer(Tango::DeviceClass *cl, AvsIdentityType* _devid, const char *s, const char *d) : 
        TANGO_BASE_CLASS(cl, s, d),
        devid(_devid)
    {
        init_device();
    }
    ~AvantesSpectrometer() {
        delete_device();
        if(devid)
            delete devid;
    };

//    Miscellaneous methods
public:
    // Delete device
    void delete_device();

    // Initialize the device
    virtual void init_device();

    // Always executed hook
    virtual void always_executed_hook() {}

    // Add dynamic attributes
    void add_dynamic_attributes();

    // Process new spectrm
    void spectrum_callback(AvsHandle* handle, int* retval);

    // Convert analog read to temperature
    double convert_analog_read(float value, float* coeff, size_t ncoeff);
};


// Acquisition thread
class MeasureRetrigger : public omni_thread {
public:
    MeasureRetrigger(AvantesSpectrometer* parent);
    ~MeasureRetrigger();

protected:
    // Main loop
    void *run_undetached(void* arg);

public:
    // Terminate thread
    void terminate() { _terminate = true; }

    // Retrigger measurement
    void signal();

private:
    // Private copy constructor
    MeasureRetrigger(const MeasureRetrigger& obj);
    MeasureRetrigger& operator=(const MeasureRetrigger& obj);

    // Parent device
    AvantesSpectrometer *_srv;

    // Terminate flag
    bool _terminate;

    omni_mutex* _lock;
    omni_condition* _cond;
};

}    //    End of namespace

#endif   //    AvantesSpectrometer_H
