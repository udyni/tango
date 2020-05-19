// kate: replace-tabs on; indent-width 4; indent-mode cstyle;
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


#include <AvantesSpectrometer.h>
#include <AvantesSpectrometerClass.h>
#include <AVCallbackDispatcher.h>
#include <AVSerializer.h>

namespace AvantesSpectrometer_ns
{

// Delete device
void AvantesSpectrometer::delete_device()
{
    DEBUG_STREAM << "AvantesSpectrometer::delete_device() " << device_name << endl;

    // Stop measurement
    AVSerializer::AVS_StopMeasure(handle);

    // Register spectrum callback
    AvsDispatcher* dispatcher = AvsDispatcher::instance();
    dispatcher->unregister_callback(handle);

    // Deactivate device
    AVSerializer::AVS_Deactivate(handle);
    handle = 0;

    if(retrigger) {
        int *ret = NULL;
        retrigger->terminate();
        retrigger->join((void**)&ret);
        delete ret;
        retrigger = NULL;
    }

    // Delete device allocated objects
    if(background)
        delete background;
    if(wavelength)
        delete wavelength;
    if(spectrum)
        delete spectrum;

    delete spectrum_lock;

    if(attr_Spectrum_read)
        delete[] attr_Spectrum_read;

    delete attr_IntegrationTime_read;
    delete attr_ScansToAverage_read;
    delete attr_enableElectricalDarkCorrection_read;
    delete attr_BoxcarWidth_read;
    delete attr_enableBackgroundSubtraction_read;
    delete attr_enableNLCorrection_read;
    delete attr_boardTemperature_read;
    delete attr_enableTEC_read;
    delete attr_TECSetpoint_read;
    delete attr_TECTemperature_read;
    delete attr_enableTrigger_read;

    if(*serial)
        CORBA::string_free(*serial);
    delete serial;
    if(*firmwareVersion)
        CORBA::string_free(*firmwareVersion);
    delete firmwareVersion;
    if(*spectrometerModel)
        CORBA::string_free(*spectrometerModel);
    delete spectrometerModel;
}

// Init device
void AvantesSpectrometer::init_device()
{
    DEBUG_STREAM << "AvantesSpectrometer::init_device() create device " << device_name << endl;

    int retval = 0;
    unsigned int req_size = 0;
    old_triggering = false;
    retrigger = NULL;

    // Get database proxy
    db = Tango::Util::instance()->get_database();

    // Allocate attributes value variables
    attr_IntegrationTime_read = new Tango::DevFloat(0.0);
    attr_ScansToAverage_read = new Tango::DevULong(0);
    attr_enableElectricalDarkCorrection_read = new Tango::DevBoolean(false);
    attr_BoxcarWidth_read = new Tango::DevULong(0);
    attr_enableBackgroundSubtraction_read = new Tango::DevBoolean(false);
    attr_enableNLCorrection_read = new Tango::DevBoolean(false);
    attr_boardTemperature_read = new Tango::DevDouble(0.0);
    attr_enableTEC_read = new Tango::DevBoolean(false);
    attr_TECSetpoint_read = new Tango::DevDouble(0.0);
    attr_TECTemperature_read = new Tango::DevDouble(0.0);
    attr_enableTrigger_read = new Tango::DevBoolean(false);
    attr_Spectrum_read = NULL;
    attr_Spectrum_read_len = 0;
    serial = new Tango::DevString();
    firmwareVersion = new Tango::DevString();
    spectrometerModel = new Tango::DevString();
    spectrum_lock = new omni_mutex();
    background = NULL;
    wavelength = NULL;
    spectrum = NULL;
    background_ok = false;

    // Create property names
    stringstream propname;
    propname << "inttime_" << devid->SerialNumber;
    prop_inttime = propname.str();
    propname.str(std::string());
    propname << "darksub_" << devid->SerialNumber;
    prop_darksub = propname.str();
    propname.str(std::string());
    propname << "tecenable_" << devid->SerialNumber;
    prop_tecenable = propname.str();
    propname.str(std::string());
    propname << "tecsetpoint_" << devid->SerialNumber;
    prop_tecsetpoint = propname.str();

    // Get properties from database
    Tango::DbData get_prop;
    get_prop.push_back(Tango::DbDatum(prop_inttime));         // 0
    get_prop.push_back(Tango::DbDatum(prop_darksub));         // 1
    get_prop.push_back(Tango::DbDatum(prop_tecenable));       // 2
    get_prop.push_back(Tango::DbDatum(prop_tecsetpoint));     // 3
    // Default values
    get_prop.push_back(Tango::DbDatum("DefaultSpecIntTime")); // 4
    get_prop.push_back(Tango::DbDatum("DefaultSpecDarkSub")); // 5

    db->get_class_property("AvantesSpectrometer", get_prop);

    // Defaul properties
    Tango::DevFloat def_inttime = 100.0;
    Tango::DevBoolean def_darksub = false;
    Tango::DevBoolean def_tecenable = true;
    Tango::DevFloat def_tecsetpoint = -10.0;

    // integration time
    if(!get_prop[0].is_empty()) {
        get_prop[0] >> def_inttime;
    } else if(!get_prop[4].is_empty()) {
        get_prop[4] >> def_inttime;
    }
    // Dynamic dark subtraction
    if(!get_prop[1].is_empty()) {
        get_prop[1] >> def_darksub;
    } else if(!get_prop[5].is_empty()) {
        get_prop[5] >> def_darksub;
    }
    if(!get_prop[2].is_empty()) {
        get_prop[2] >> def_tecenable;
    }
    if(!get_prop[3].is_empty()) {
        get_prop[3] >> def_tecsetpoint;
    }

    // Store serial and model
    *serial = CORBA::string_dup(devid->SerialNumber);

    // Activate spectrometer
    handle = AVSerializer::AVS_Activate(devid);
    if(handle < 0) {
        stringstream msg;
        msg << "Activation of the spectrometer failed with error " << handle;
        Tango::Except::throw_exception(
            (const char*)"Activation failed",
            msg.str(),
            (const char*)"AvantesSpectrometer::init_device()");
    }

    // Get firmware version
    #define AVASPEC_VERLEN 16
    char FPGAVersion[AVASPEC_VERLEN];
    char FirmwareVersion[AVASPEC_VERLEN];
    char LibVersion[AVASPEC_VERLEN];

    retval = AVSerializer::AVS_GetVersionInfo(handle, FPGAVersion, FirmwareVersion, LibVersion);
    if(retval < 0) {
        stringstream msg;
        msg << "Failed to get firmware version from spectrometer with error " << retval;
        Tango::Except::throw_exception(
            (const char*)"Get version failed",
            msg.str(),
            (const char*)"AvantesSpectrometer::init_device()");
    }
    DEBUG_STREAM << "FPGA version: " << FPGAVersion << " - Firmware version: " << FirmwareVersion << " - Library version: " << LibVersion << endl;
    int major = 0, minor = 0, patch1 = 0, patch2 = 0;
    retval = sscanf(FPGAVersion, "%d.%d.%d.%d", &major, &minor, &patch1, &patch2);
    if(retval == 4) {
        snprintf(FPGAVersion, AVASPEC_VERLEN, "%d.%d.%d.%d", major, minor, patch1, patch2);
    }
    *firmwareVersion = CORBA::string_dup(FPGAVersion);

    // Get device configuration
    memset(&dev_config, 0, sizeof(DeviceConfigType));
    retval = AVSerializer::AVS_GetParameter(handle, sizeof(DeviceConfigType), &req_size, &dev_config);
    if(retval < 0) {
        stringstream msg;
        msg << "Failed to get device configuration with error " << retval;
        Tango::Except::throw_exception(
            (const char*)"Get device configuration failed",
            msg.str(),
            (const char*)"AvantesSpectrometer::init_device()");
    }

    // Try to read temperature sensor on channel 0. If value is saturated at 5V, no TEC cooling is available
    float val = 0.0;
    retval = AVSerializer::AVS_GetAnalogIn(handle, 0, &val);
    coolingAvailable = false;
    if(retval) {
        ERROR_STREAM << "Failed to read analog input channel 0 with error " << retval << endl;
    } else {
        if(val < 5.0) {
            coolingAvailable = true;
        }
    }

    // Set default TEC parameters
    if(coolingAvailable) {
        bool modified = false;

        if(dev_config.m_TecControl.m_Enable != def_tecenable) {
            modified = true;
            dev_config.m_TecControl.m_Enable = def_tecenable;
        }
        if(dev_config.m_TecControl.m_Setpoint != def_tecsetpoint) {
            modified = true;
            dev_config.m_TecControl.m_Enable = def_tecenable;
        }
        if(modified) {
            retval = AVSerializer::AVS_SetParameter(handle, &dev_config);
            if(retval) {
                TangoSys_OMemStream msg;
                msg << "Failed to configure device with error " << retval;
                ERROR_STREAM << msg.str() << endl;
                Tango::Except::throw_exception(
                    (const char*)"Failed to configure",
                    msg.str(),
                    (const char*)"AvantesSpectrometer::init_device()");
            }
        }
    }

    // Store spectrometer model
    *spectrometerModel = CORBA::string_dup(dev_config.m_aUserFriendlyId);

    // Get number of pixels
    uint16_t numOfPixels = 0;
    retval = AVSerializer::AVS_GetNumPixels(handle, &numOfPixels);
    if(retval < 0) {
        stringstream msg;
        msg << "Failed to get number of pixels from spectrometer with error " << retval;
        Tango::Except::throw_exception(
            (const char*)"Failed to get number of pixels failed",
            msg.str(),
            (const char*)"AvantesSpectrometer::init_device()");
    }

    // Backgroud spectrum
    updateBackground = false;
    background = new Tango::DevVarDoubleArray(numOfPixels);
    background->length(numOfPixels);
    memset(background->get_buffer(), 0, sizeof(Tango::DevDouble) * numOfPixels);

    // Spectrum
    spectrum = new Tango::DevVarDoubleArray(numOfPixels);
    spectrum->length(numOfPixels);
    memset(spectrum->get_buffer(), 0, sizeof(Tango::DevDouble) * numOfPixels);

    // Wavelength
    wavelength = new Tango::DevVarDoubleArray(numOfPixels);
    wavelength->length(numOfPixels);
    // Read wavelength
    retval = AVSerializer::AVS_GetLambda(handle, wavelength->get_buffer());
    if(retval < 0) {
        stringstream msg;
        msg << "Failed to get pixels wavelength from spectrometer with error " << retval;
        Tango::Except::throw_exception(
            (const char*)"Failed to get wavelength of pixels",
            msg.str(),
            (const char*)"AvantesSpectrometer::init_device()");
    }

    // Confiugure measurement with default values
    memset(&config, 0, sizeof(MeasConfigType));
    config.m_StartPixel = 0;
    config.m_StopPixel = numOfPixels - 1;
    config.m_IntegrationTime = def_inttime;
    config.m_IntegrationDelay = 0;
    config.m_NrAverages = 1;
    config.m_CorDynDark.m_Enable = (def_darksub) ? 1 : 0;
    config.m_Smoothing.m_SmoothPix = 0;
    config.m_Smoothing.m_SmoothModel = 0;
    config.m_SaturationDetection = 0    ;
    config.m_Trigger.m_Mode = 0,
    config.m_Trigger.m_Source = 0,
    config.m_Trigger.m_SourceType = 0;

    // Configure measurement
    retval = AVSerializer::AVS_PrepareMeasure(handle, &config);
    if(retval) {
        stringstream msg;
        msg << "Failed to configure acquisition from spectrometer with error " << retval;
        Tango::Except::throw_exception(
            (const char*)"Failed to configure acquisition",
            msg.str(),
            (const char*)"AvantesSpectrometer::init_device()");
    }

    // Register spectrum callback
    AvsDispatcher* dispatcher = AvsDispatcher::instance();
    dispatcher->register_callback(handle, this);

    // Start measurement
    retval = AVSerializer::AVS_Measure(handle, AvsDispatcher::spectrum_callback, -1);
    if(retval) {
        stringstream msg;
        msg << "Failed to start acquisition from spectrometer with error " << retval;
        Tango::Except::throw_exception(
            (const char*)"Failed to start acquisition",
            msg.str(),
            (const char*)"AvantesSpectrometer::init_device()");
    }

    INFO_STREAM << "Started spectrometer with serial " << devid->SerialNumber << endl;
    set_state(Tango::ON);
}


// Add dynamic attributes
void AvantesSpectrometer::add_dynamic_attributes() {

    // Check if spectrometer support TEC
    if(coolingAvailable) {
        // Add TEC enable attribute
        EnableTECAttrib *enabletec = new EnableTECAttrib();
        Tango::UserDefaultAttrProp enabletec_prop;
        enabletec_prop.set_description("Enable thermoelectric cooling if supported");
        enabletec_prop.set_label("Enable TEC");
        enabletec->set_default_properties(enabletec_prop);
        enabletec->set_change_event(true, false);
        enabletec->set_disp_level(Tango::OPERATOR);
        add_attribute(enabletec);

        // Add TEC setpoint attribute
        TECSetPointAttrib *tecsetpoint = new TECSetPointAttrib();
        Tango::UserDefaultAttrProp tecsetpoint_prop;
        tecsetpoint_prop.set_description("Thermo-electric cooler setpoint");
        tecsetpoint_prop.set_label("TEC setpoint");
        tecsetpoint_prop.set_unit("°C");
        tecsetpoint_prop.set_standard_unit("°C");
        tecsetpoint_prop.set_display_unit("°C");
        tecsetpoint_prop.set_format("%.1f");
        tecsetpoint_prop.set_max_value("25.0");
        tecsetpoint_prop.set_min_value("-50.0");
        tecsetpoint->set_default_properties(tecsetpoint_prop);
        tecsetpoint->set_change_event(true, false);
        tecsetpoint->set_disp_level(Tango::OPERATOR);
        add_attribute(tecsetpoint);

        // Add TEC temperature attribute
        TECTemperatureAttrib *tectemperature = new TECTemperatureAttrib();
        Tango::UserDefaultAttrProp tectemperature_prop;
        tectemperature_prop.set_description("Thermo-electric cooler actual temperature");
        tectemperature_prop.set_label("TEC temperature");
        tectemperature_prop.set_unit("°C");
        tectemperature_prop.set_standard_unit("°C");
        tectemperature_prop.set_display_unit("°C");
        tectemperature_prop.set_format("%.1f");
//         tectemperature_prop.set_event_abs_change("0.1");
        tectemperature->set_default_properties(tectemperature_prop);
//         tectemperature->set_polling_period(3000);
//         tectemperature->set_change_event(true, true);
        tectemperature->set_change_event(true, false);
        tectemperature->set_disp_level(Tango::OPERATOR);
        add_attribute(tectemperature);
    }

    try {
        update_analog_attributes();
    } catch(Tango::DevFailed &e) {
        ERROR_STREAM << "Failed to read analog attributes after INIT (Error: " << e.errors[0].desc << ")" << endl;
    }
}


// Spectrum callback
void AvantesSpectrometer::spectrum_callback(AvsHandle* handle, int* retval) {

    // Check retval
    if(*retval < 0) {
        WARN_STREAM << "Spectrum acquisition failed with error " << (*retval) << endl;
        set_state(Tango::FAULT);
        return;
    }

    // Get spectrum data
    omni_mutex_lock access_lock(*spectrum_lock);
    unsigned int timestamp = 0;
    int ans = AVSerializer::AVS_GetScopeData(*handle, &timestamp, spectrum->get_buffer());
    if(ans) {
        WARN_STREAM << "Spectrum retrieval failed with error " << ans << endl;
        set_state(Tango::FAULT);
        return;
    }

    // Got a spectrum. Spectrometer is online!
    set_state(Tango::ON);

    if(updateBackground) {
        // Store spectrum as background
        memcpy(background->get_buffer(), spectrum->get_buffer(), sizeof(Tango::DevDouble) * spectrum->length());
        updateBackground = false;
        background_ok = true;
        return;
    }

    if(*attr_enableBackgroundSubtraction_read) {
        for(size_t i = 0; i < spectrum->length(); i++)
            spectrum->get_buffer()[i] -= background->get_buffer()[i];
    }

    // Allocate buffer if needed
    if(attr_Spectrum_read && spectrum->length() != attr_Spectrum_read_len) {
        delete[] attr_Spectrum_read;
        attr_Spectrum_read = NULL;
    }
    if(!attr_Spectrum_read) {
        attr_Spectrum_read = new double[spectrum->length()];
        attr_Spectrum_read_len = spectrum->length();
    }
    // Copy spectrum
    memcpy(attr_Spectrum_read, spectrum->get_buffer(), spectrum->length() * sizeof(Tango::DevDouble));

    // Push change event on the spectrum attribute
    push_change_event("Spectrum", attr_Spectrum_read, attr_Spectrum_read_len);

    DEBUG_STREAM << "Got a new spectrum with timestamp " << timestamp << endl;

    // If the old triggering scheme is on we need to start a new measurement
    if(old_triggering) {
        if(retrigger)
            retrigger->signal();
    }
}


// Convert analog read to temperature
double AvantesSpectrometer::convert_analog_read(float value, float* coeff, size_t ncoeff) {
    double out = 0.0;
    for(size_t i = 0; i < ncoeff; i++) {
        out += pow(value, i) * coeff[i];
    }
    return out;
}


// Update analog attributes
void AvantesSpectrometer::update_analog_attributes() {

    // Stop acquisition and force a readout of analog sensors
    int retval = 0;
    struct timespec slp;
    slp.tv_sec = 0;
    slp.tv_nsec = 10000000;

    // Stop measurement
    AVSerializer::AVS_StopMeasure(handle);
    nanosleep(&slp, NULL);

    float value = 0.0;
    retval = AVSerializer::AVS_GetAnalogIn(handle, 6, &value);
    if(retval) {
        ERROR_STREAM << "Failed to read board temperature with error " << retval << endl;
    } else {
        *(attr_boardTemperature_read) = convert_analog_read(value, dev_config.m_aTemperature[0].m_aFit, NR_TEMP_POL_COEF);
        push_change_event("BoardTemperature", attr_boardTemperature_read);
    }

    if(coolingAvailable) {
        value = 0.0;
        retval = 0;
        retval = AVSerializer::AVS_GetAnalogIn(handle, 0, &value);
        if(retval) {
            TangoSys_OMemStream msg;
            ERROR_STREAM << "Failed to read TEC temperature with error " << retval << endl;
            return;
        } else {
            *(attr_TECTemperature_read) = convert_analog_read(value, dev_config.m_aTemperature[2].m_aFit, NR_TEMP_POL_COEF);
            push_change_event("TECTemperature", attr_TECTemperature_read);
        }
    }

    // Configure measurement
    retval = AVSerializer::AVS_PrepareMeasure(handle, &config);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to configure measurement with error " << retval;
        ERROR_STREAM << msg.str() << endl;
        set_state(Tango::FAULT);
        Tango::Except::throw_exception(
            (const char*)"Failed to configure",
            msg.str(),
            (const char*)"AvantesSpectrometer::update_analog_attributes()");
    }

    // Restart measurement
    retval = AVSerializer::AVS_Measure(handle, AvsDispatcher::spectrum_callback, (old_triggering) ? 1 : -1);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to start measurement with error " << retval;
        ERROR_STREAM << msg.str() << endl;
        set_state(Tango::FAULT);
        Tango::Except::throw_exception(
            (const char*)"Failed to start",
            msg.str(),
            (const char*)"AvantesSpectrometer::update_analog_attributes()");
    }
}


// Retrigger thread constructor
MeasureRetrigger::MeasureRetrigger(AvantesSpectrometer* parent) :
    _srv(parent),
    _terminate(false)
{
    _lock = new omni_mutex();
    _cond = new omni_condition(_lock);
    start_undetached();
}

// Retrigger thread destructor
MeasureRetrigger::~MeasureRetrigger() {
    delete _cond;
    delete _lock;
}

// Retrigger thread loop
void *MeasureRetrigger::run_undetached(void* arg) {
    while(!_terminate) {
        int ans = _cond->timedwait(0, 200000000);
        if(ans) {
            ans = AVSerializer::AVS_Measure(_srv->handle, AvsDispatcher::spectrum_callback, 1);
            if(ans) {
                _srv->get_logger()->error("Failed to re-start acquisition in old-triggering mode with error %d", ans);
                _srv->set_state(Tango::FAULT);
            }
        }
    }

    int* ret = new int(0);
    return ret;
}

// Signal thread
void MeasureRetrigger::signal() {
    _cond->signal();
}


} // End of namespace
