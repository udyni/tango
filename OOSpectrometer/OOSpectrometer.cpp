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


#include <OOSpectrometer.h>
#include <OOSpectrometerClass.h>


namespace OOSpectrometer_ns
{

// Delete device
void OOSpectrometer::delete_device()
{
    DEBUG_STREAM << "OOSpectrometer::delete_device() " << device_name << endl;

    //    Delete device allocated objects
    if(acq) {
        acq->terminate();
        int *retval;
        acq->join((void**)&retval);
        acq = NULL;
    }

    if(background)
        delete background;
    if(wavelength)
        delete wavelength;
    if(spectrum)
        delete spectrum;

    delete spectrum_lock;

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
    delete attr_timeout_read;

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
void OOSpectrometer::init_device()
{
    DEBUG_STREAM << "OOSpectrometer::init_device() create device " << device_name << endl;

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
    attr_timeout_read = new Tango::DevULong(0);
    attr_Spectrum_read = NULL;
    attr_Spectrum_read_len = 0;
    serial = new Tango::DevString();
    firmwareVersion = new Tango::DevString();
    spectrometerModel = new Tango::DevString();
    spectrum_lock = new omni_mutex();
    spectrum = NULL;
    background = NULL;
    wavelength = NULL;
    background_ok = false;

    // Create property names
    stringstream propname;
    propname << "inttime_" << serialNumber;
    prop_inttime = propname.str();
    propname.str(std::string());
    propname << "darksub_" << serialNumber;
    prop_darksub = propname.str();
    propname.str(std::string());
    propname << "tecenable_" << serialNumber;
    prop_tecenable = propname.str();
    propname.str(std::string());
    propname << "tecsetpoint_" << serialNumber;
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

    db->get_class_property("OOSpectrometer", get_prop);

    // Defaul properties
    Tango::DevFloat def_inttime = 100.0;
    Tango::DevBoolean def_darksub = false;
    Tango::DevBoolean def_tecenable = true;
    Tango::DevFloat def_tecsetpoint = -5.0;

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

    // Find spectrometer ID
    spec_id = WrapperOfWrapper::instance()->findSpectrometer(serialNumber);
    if(spec_id == -1) {
        // This should never happen as the check is done by device factory...
        Tango::Except::throw_exception(
            (const char*)"Bad spectrometer serial",
            (const char*)"The spectrometer with the given serial number is not connected to this computer",
            (const char*)"OOSpectrometer::init_device()");
    }

    // Serial number
    *serial = CORBA::string_dup(serialNumber.c_str());

    // Get firmware version
    try {
        char str[64];
        str[63] = '\0';
        WrapperOfWrapper::instance()->getFirmwareVersion(spec_id, str, 63);
        *firmwareVersion = CORBA::string_dup(str);
    } catch(WoWException &e) {
        WARN_STREAM << "Failed to get firmware version (Error: " << e.what() << ")" << endl;
        *firmwareVersion = CORBA::string_dup("Unknown");
    }

    // Get model
    try {
        char str[128];
        str[127] = '\0';
        WrapperOfWrapper::instance()->getModel(spec_id, str, 127);
        *spectrometerModel = CORBA::string_dup(str);
    } catch(WoWException &e) {
        WARN_STREAM << "Failed to get spectrometer model (Error: " << e.what() << ")" << endl;
        *spectrometerModel = CORBA::string_dup("Unknown");
    }

    // Get number of pixels
    uint32_t numOfPixels = WrapperOfWrapper::instance()->getNumberOfPixels(spec_id);

    // Backgroud spectrum
    updateBackground = false;
    background = new Tango::DevVarDoubleArray(numOfPixels);
    background->length(numOfPixels);
    memset(background->get_buffer(), 0, sizeof(Tango::DevDouble) * numOfPixels);

    // Allocate spectrum buffer
    spectrum = new Tango::DevVarDoubleArray(numOfPixels);
    spectrum->length(numOfPixels);
    memset(spectrum->get_buffer(), 0, sizeof(Tango::DevDouble) * numOfPixels);

    // Wavelength
    wavelength = new Tango::DevVarDoubleArray(numOfPixels);
    wavelength->length(numOfPixels);
    // Read wavelength
    try {
        WrapperOfWrapper::instance()->getWavelength(spec_id, wavelength->get_buffer(), wavelength->length());
    } catch(WoWException &e) {
        stringstream msg;
        msg << "Failed to get wavelength vector (Error: " << e.what() << ")";
        Tango::Except::throw_exception(
            (const char*)"Failed to read spectrometer",
            msg.str(),
            (const char*)"OOSpectrometer::init_device()");
    }

    // Set default integration time and dark sub status
    try {
        WrapperOfWrapper::instance()->setIntegrationTime(spec_id, uint32_t(def_inttime * 1000.0));
        WrapperOfWrapper::instance()->setCorrectForElectricalDark(spec_id, bool(def_darksub));
        if(WrapperOfWrapper::instance()->coolingAvailable(spec_id)) {
            WrapperOfWrapper::instance()->setTECEnable(spec_id, bool(def_tecenable));
            WrapperOfWrapper::instance()->setTECSetpoint(spec_id, double(def_tecsetpoint));
        }

    } catch(WoWException &e) {
        ERROR_STREAM << "Failed to set default integration time and dark subtraction status (Error: " << e.what() << ")" << endl;
    }

    // Start acquisition thread
    acq = new SpecThread(this);

    INFO_STREAM << "Started spectrometer with serial '" << serialNumber << "' as ID " << spec_id << endl;
}


// Add dynamic attributes
void OOSpectrometer::add_dynamic_attributes() {

    // Add IntegrationTime attribute
    IntegrationTimeAttrib *integrationtime = new IntegrationTimeAttrib();
    Tango::UserDefaultAttrProp integrationtime_prop;
    integrationtime_prop.set_description("Spectrometer integration time");
    integrationtime_prop.set_label("Integration time");
    integrationtime_prop.set_unit("ms");
    integrationtime_prop.set_standard_unit("ms");
    integrationtime_prop.set_display_unit("ms");
    integrationtime_prop.set_format("%.2f");
    try {
        Tango::DevULong min_val = WrapperOfWrapper::instance()->getMinimumIntegrationTime(spec_id);
        Tango::DevULong max_val = WrapperOfWrapper::instance()->getMaximumIntegrationTime(spec_id);
        DEBUG_STREAM << "Setting integration time limits (" << min_val << " us, " << max_val << " us)" << endl;
        stringstream val;
        val << float(min_val) / 1000.0;
        integrationtime_prop.set_min_value(val.str().c_str());
        DEBUG_STREAM << "Min value string: " << val.str() << endl;
        val.str("");
        val << float(max_val) / 1000.0;
        DEBUG_STREAM << "Max value string: " << val.str() << endl;
        integrationtime_prop.set_max_value(val.str().c_str());
    } catch(...) {
        // Got an exception. Setting a conservative default
        DEBUG_STREAM << "Setting integration time limits to default (10ms, 3sec)" << endl;
        integrationtime_prop.set_min_value("10000"); // 10 ms
        integrationtime_prop.set_max_value("3000000"); // 3 sec
    }
    integrationtime->set_default_properties(integrationtime_prop);
    integrationtime->set_disp_level(Tango::OPERATOR);
    integrationtime->set_change_event(true, false);
    add_attribute(integrationtime);

    // Check if spectrometer support TEC
    if(WrapperOfWrapper::instance()->coolingAvailable(spec_id)) {
        // Add TEC enable attribute
        EnableTECAttrib* enabletec = new EnableTECAttrib("EnableTEC");
        Tango::UserDefaultAttrProp enabletec_prop;
        enabletec_prop.set_description("Enable thermoelectric cooling if supported");
        enabletec_prop.set_label("Enable TEC");
        enabletec->set_default_properties(enabletec_prop);
        enabletec->set_disp_level(Tango::OPERATOR);
        enabletec->set_change_event(true, false);
        add_attribute(enabletec);

        // Add TEC setpoint attribute
        TECSetPointAttrib* tecsetpoint = new TECSetPointAttrib("TECSetPoint");
        Tango::UserDefaultAttrProp tecsetpoint_prop;
        tecsetpoint_prop.set_description("Thermo-electric cooler setpoint");
        tecsetpoint_prop.set_label("TEC setpoint");
        tecsetpoint_prop.set_unit("\xB0" "C");
        tecsetpoint_prop.set_standard_unit("\xB0" "C");
        tecsetpoint_prop.set_display_unit("\xB0" "C");
        tecsetpoint_prop.set_format("%.1f");
        tecsetpoint->set_default_properties(tecsetpoint_prop);
        tecsetpoint->set_disp_level(Tango::OPERATOR);
        tecsetpoint->set_change_event(true, false);
        add_attribute(tecsetpoint);

        // Add TEC temperature attribute
        TECTemperatureAttrib* tectemperature = new TECTemperatureAttrib("TECTemperature");
        Tango::UserDefaultAttrProp tectemperature_prop;
        tectemperature_prop.set_description("Thermo-electric cooler actual temperature");
        tectemperature_prop.set_label("TEC temperature");
        tectemperature_prop.set_unit("\xB0" "C");
        tectemperature_prop.set_standard_unit("\xB0" "C");
        tectemperature_prop.set_display_unit("\xB0" "C");
        tectemperature_prop.set_format("%.1f");
        tectemperature_prop.set_event_rel_change("0.5");
        tectemperature_prop.set_event_abs_change("0.2");
        tectemperature->set_default_properties(tectemperature_prop);
        tectemperature->set_disp_level(Tango::OPERATOR);
        tectemperature->set_change_event(true, true);
        add_attribute(tectemperature);
    }
}


// SpecThread main loop
void *SpecThread::run_undetached(void* arg) {

    struct timespec _sl;
    _sl.tv_sec = 0;
    _sl.tv_nsec = 100000000;

    while(!_terminate) {
        try {
            // Lock spectrum
            omni_mutex_lock access_lock(*(_parent->spectrum_lock));
            // Acquire spectrum
            WrapperOfWrapper::instance()->getSpectrum(
                _parent->spec_id,
                _parent->spectrum->get_buffer(),
                _parent->spectrum->length());

            if(_parent->updateBackground) {
                // Store spectrum as background
                memcpy(_parent->background->get_buffer(), _parent->spectrum->get_buffer(), sizeof(Tango::DevDouble) * _parent->spectrum->length());
                _parent->updateBackground = false;
                _parent->background_ok = true;
                continue;
            }

            // Subtract background if needed
            if(*(_parent->attr_enableBackgroundSubtraction_read)) {
                double *sp = _parent->spectrum->get_buffer();
                double *bkg = _parent->background->get_buffer();
                for(size_t i = 0; i < _parent->spectrum->length(); i++)
                    sp[i] -= bkg[i];
            }

            // Allocate buffer if needed
            if(_parent->attr_Spectrum_read && _parent->spectrum->length() != _parent->attr_Spectrum_read_len) {
                delete[] _parent->attr_Spectrum_read;
                _parent->attr_Spectrum_read = NULL;
            }
            if(!_parent->attr_Spectrum_read) {
                _parent->attr_Spectrum_read = new double[_parent->spectrum->length()];
                _parent->attr_Spectrum_read_len = _parent->spectrum->length();
            }
            // Copy spectrum
            memcpy(_parent->attr_Spectrum_read, _parent->spectrum->get_buffer(), _parent->spectrum->length() * sizeof(Tango::DevDouble));

        } catch(WoWException &e) {
            stringstream msg;
            msg << "Acquisition failed (Error: " << e.what() << ")";
            _parent->get_logger()->error(msg.str());
            nanosleep(&_sl, NULL);
//             _parent->set_state(Tango::FAULT);
            continue;
        }

        try {
            // Send change event
            _parent->push_change_event("Spectrum", _parent->attr_Spectrum_read, _parent->attr_Spectrum_read_len);
        } catch(Tango::DevFailed &e) {
            stringstream msg;
            msg << "Failed to push spectrum event (Error: " << e.errors[0].desc << ")" << endl;
            _parent->get_logger()->error(msg.str());
            _parent->set_state(Tango::FAULT);
            continue;
        }

        if(_parent->get_logger()->is_debug_enabled())
            _parent->get_logger()->debug("Got a new spectrum");
        _parent->set_state(Tango::ON);

    }

    int *retval = new int(0);
    return (void*)retval;
}


} // End of namespace
