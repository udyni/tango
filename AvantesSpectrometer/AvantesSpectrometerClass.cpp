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

#include <AvantesSpectrometerClass.h>
#include <AVSerializer.h>

namespace AvantesSpectrometer_ns
{

// Initialize pointer for singleton pattern
AvantesSpectrometerClass *AvantesSpectrometerClass::_instance = NULL;


// AvantesSpectrometerClass constructor (@param s: class name)
AvantesSpectrometerClass::AvantesSpectrometerClass(string &s) : Tango::DeviceClass(s)
{
    // Set default properties
    set_default_property();
    // Write common properties in the DB
    write_class_property();
}


// AvantesSpectrometerClass destructor
AvantesSpectrometerClass::~AvantesSpectrometerClass()
{
    _instance = NULL;
}


// AvantesSpectrometerClass init method. Create the class singleton
AvantesSpectrometerClass *AvantesSpectrometerClass::init(const char *name)
{
    if (_instance == NULL) {
        try {
            string s(name);
            _instance = new AvantesSpectrometerClass(s);
        } catch (bad_alloc &e) {
            throw e;
        }
    }
    return _instance;
}


// Return instance of AvantesSpectrometerClass singleton
AvantesSpectrometerClass *AvantesSpectrometerClass::instance() {
    if (_instance == NULL) {
        cerr << "Class is not initialised !!" << endl;
        exit(-1);
    }
    return _instance;
}


//===================================================================
// Properties management
//===================================================================
//--------------------------------------------------------
/**
 *    Method      : AvantesSpectrometerClass::get_class_property()
 *    Description : Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum AvantesSpectrometerClass::get_class_property(string &prop_name)
{
    for (unsigned int i=0 ; i<cl_prop.size() ; i++)
        if (cl_prop[i].name == prop_name)
            return cl_prop[i];
    // if not found, returns  an empty DbDatum
    return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *    Method      : AvantesSpectrometerClass::get_default_device_property()
 *    Description : Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum AvantesSpectrometerClass::get_default_device_property(string &prop_name)
{
    for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
        if (dev_def_prop[i].name == prop_name)
            return dev_def_prop[i];
    // if not found, return  an empty DbDatum
    return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *    Method      : AvantesSpectrometerClass::get_default_class_property()
 *    Description : Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum AvantesSpectrometerClass::get_default_class_property(string &prop_name)
{
    for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
        if (cl_def_prop[i].name == prop_name)
            return cl_def_prop[i];
    // if not found, return  an empty DbDatum
    return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *    Method      : AvantesSpectrometerClass::set_default_property()
 *    Description : Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void AvantesSpectrometerClass::set_default_property()
{
    string    prop_name;
    string    prop_desc;
    string    prop_def;
    vector<string>    vect_data;

    // Set Default Class Properties
    prop_name = "DefaultSpecIntTime";
    prop_desc = "Default spectrometer integration time.";
    prop_def  = "100";
    vect_data.clear();
    if (prop_def.length()>0)
    {
        Tango::DbDatum    data(prop_name);
        data << vect_data ;
        cl_def_prop.push_back(data);
        add_wiz_class_prop(prop_name, prop_desc, prop_def);
    }
    else
        add_wiz_class_prop(prop_name, prop_desc);

    prop_name = "DefaultSpecDarkSub";
    prop_desc = "Default status of dark background subtraction.";
    prop_def  = "0";
    vect_data.clear();
    if (prop_def.length()>0)
    {
        Tango::DbDatum    data(prop_name);
        data << vect_data ;
        cl_def_prop.push_back(data);
        add_wiz_class_prop(prop_name, prop_desc, prop_def);
    }
    else
        add_wiz_class_prop(prop_name, prop_desc);


    // Set Default device Properties

}

//--------------------------------------------------------
/**
 *    Method      : AvantesSpectrometerClass::write_class_property()
 *    Description : Set class description fields as property in database
 */
//--------------------------------------------------------
void AvantesSpectrometerClass::write_class_property()
{
    // First time, check if database used
    if (Tango::Util::_UseDb == false)
        return;

    Tango::DbData    data;
    string    classname = get_name();
    string    header;
    string::size_type    start, end;

    // Put title
    Tango::DbDatum    title("ProjectTitle");
    string    str_title("Avantes spectrometer server");
    title << str_title;
    data.push_back(title);

    // Put Description
    Tango::DbDatum    description("Description");
    vector<string>    str_desc;
    str_desc.push_back("Avantes spectrometer server");
    description << str_desc;
    data.push_back(description);

    // Call database and and values
    get_db_class()->put_property(data);
}


//===================================================================
// Factory methods
//===================================================================

// Device factory
void AvantesSpectrometerClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
    DEBUG_STREAM << "Entering AvantesSpectrometerClass::device_factory()..." << endl;

    // Get database proxy
    Tango::Database *db = Tango::Util::instance()->get_database();

    // Delete old devices
    DEBUG_STREAM << "Deleting old devices:" << endl;
    for(size_t i = 0; i < devlist_ptr->length(); i++) {
        string dev = (*devlist_ptr)[i].in();
        db->delete_device(dev);
        DEBUG_STREAM << " - Removed " << dev << endl;
    }

    // Get number of devices
    int num_dev = AVSerializer::AVS_UpdateUSBDevices();

    // Allocate buffer to store device information
    AvsIdentityType* devices = new AvsIdentityType[num_dev];

    // Get device list
    int retval = 0;
    unsigned int req_size = 0;

    retval = AVSerializer::AVS_GetList(num_dev * sizeof(AvsIdentityType), &req_size, devices);
    if(retval < 0) {
        if(retval == ERR_INVALID_SIZE) {
            ERROR_STREAM << "Failed to get device information, invalid buffer size (Required " << req_size << ", allocated " << (num_dev * sizeof(AvsIdentityType)) << ")" << endl;
        } else {
            ERROR_STREAM << "Failed to get device information (Unexpected error code: " << retval << ")" << endl;
        }
        return;
    }

    // Look for device names in the DB (names are properties of the form name_<serial>)
    Tango::DbData get_prop;
    for(int i = 0; i < num_dev; i++) {
        stringstream prop;
        prop << "name_" << devices[i].SerialNumber;
        get_prop.push_back(Tango::DbDatum(prop.str().c_str()));
    }
    db->get_class_property("AvantesSpectrometer", get_prop);

    // List of devices that should be exported
    vector<string> new_devlist;

    // Re-create attached devices
    Tango::DbData set_prop;
    for(int i = 0; i < num_dev; i++) {
        DEBUG_STREAM << "Found spectrometer with serial " << devices[i].SerialNumber << endl;
        Tango::DbDevInfo dev_info;
        dev_info.server = Tango::Util::instance()->get_ds_name();
        dev_info._class = this->get_name();
        if(!get_prop[i].is_empty()) {
            DEBUG_STREAM << "Spectrometer found in class properties" << endl;
            // Found
            get_prop[i] >> dev_info.name;
            // Create device
            db->add_device(dev_info);

        } else {
            DEBUG_STREAM << "Spectrometer NOT fo und in class properties" << endl;
            // Not found
            // Get spectrometer serial number to lower case
            std::string devname(devices[i].SerialNumber);
            transform(devname.begin(), devname.end(), devname.begin(), ::tolower);

            // Compose device name
            dev_info.name = "udyni/spectrometers/avs_" + devname;
            DEBUG_STREAM << "Name for new device: " << dev_info.name << endl;

            // Store device name in class properties
            set_prop.push_back(get_prop[i]);
            set_prop.back() << dev_info.name;

            // Create device
            db->add_device(dev_info);
        }
        // Add device to the dev_list
        new_devlist.push_back(dev_info.name);
    }
    // Put new properties into database
    if(set_prop.size())
        db->put_class_property("AvantesSpectrometer", set_prop);

    // Create devices and add it into the device list
    try {
        for(int i = 0; i < num_dev && i < (int)new_devlist.size(); i++) {
            DEBUG_STREAM << "Device name : " << new_devlist[i] << endl;
            AvsIdentityType* dev = new AvsIdentityType(devices[i]);
            device_list.push_back(new AvantesSpectrometer(this, dev, new_devlist[i]));
        }
    } catch(Tango::DevFailed &e) {
        // Init device might throw exceptions. Must intercept to de-allocate devices
        delete devices;
        throw e;
    }

    // Manage dynamic attributes if any ... might be not needed as we created new devices...
    //erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

    // Export devices to the outside world
    for(size_t i = 0; i < device_list.size(); i++) {
        // Add dynamic attributes if any
        AvantesSpectrometer *dev = static_cast<AvantesSpectrometer *>(device_list[i]);
        dev->add_dynamic_attributes();

        // Check before if database used.
        if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
            export_device(dev);
        else
            export_device(dev, dev->get_name().c_str());
        DEBUG_STREAM << "Exported device " << dev->get_name() << endl;
    }
}


// Attribute factory
void AvantesSpectrometerClass::attribute_factory(vector<Tango::Attr *> &att_list) {

    // Attribure: IntegrationTime
    IntegrationTimeAttrib *integrationtime = new IntegrationTimeAttrib();
    Tango::UserDefaultAttrProp integrationtime_prop;
    integrationtime_prop.set_description("Spectrometer integration time");
    integrationtime_prop.set_label("Integration time");
    integrationtime_prop.set_unit("ms");
    integrationtime_prop.set_standard_unit("ms");
    integrationtime_prop.set_display_unit("ms");
    integrationtime_prop.set_format("%.2f");
    integrationtime_prop.set_max_value("600000.0");
    integrationtime->set_default_properties(integrationtime_prop);
    integrationtime->set_disp_level(Tango::OPERATOR);
    integrationtime->set_change_event(true, false);
    att_list.push_back(integrationtime);

    // Attribute : ScansToAverage
    ScansToAverageAttrib* scanstoaverage = new ScansToAverageAttrib();
    Tango::UserDefaultAttrProp scanstoaverage_prop;
    scanstoaverage_prop.set_description("Number of spectra to average");
    scanstoaverage_prop.set_label("Averages");
    scanstoaverage_prop.set_format("%d");
    scanstoaverage->set_default_properties(scanstoaverage_prop);
    scanstoaverage->set_disp_level(Tango::OPERATOR);
    scanstoaverage->set_change_event(true, false);
    att_list.push_back(scanstoaverage);

    // Attribute : enableElectricalDarkCorrection
    enableElectricalDarkCorrectionAttrib* enableelectricaldarkcorrection = new enableElectricalDarkCorrectionAttrib();
    Tango::UserDefaultAttrProp enableelectricaldarkcorrection_prop;
    enableelectricaldarkcorrection_prop.set_description("Enable electrical dark correction");
    enableelectricaldarkcorrection_prop.set_label("Enable dark correction");
    enableelectricaldarkcorrection->set_default_properties(enableelectricaldarkcorrection_prop);
    enableelectricaldarkcorrection->set_disp_level(Tango::OPERATOR);
    enableelectricaldarkcorrection->set_change_event(true, false);
    att_list.push_back(enableelectricaldarkcorrection);

    // Attribute : BoxcarWidth
    BoxcarWidthAttrib* boxcarwidth = new BoxcarWidthAttrib();
    Tango::UserDefaultAttrProp boxcarwidth_prop;
    boxcarwidth_prop.set_description("Set boxcar width");
    boxcarwidth_prop.set_label("Boxcar width");
    boxcarwidth_prop.set_unit("Pixel");
    boxcarwidth_prop.set_standard_unit("Pixel");
    boxcarwidth_prop.set_display_unit("Pixel");
    boxcarwidth_prop.set_format("%d");
    boxcarwidth->set_default_properties(boxcarwidth_prop);
    boxcarwidth->set_disp_level(Tango::OPERATOR);
    boxcarwidth->set_change_event(true, false);
    att_list.push_back(boxcarwidth);

    // Attribute : enableBackgroundSubtraction
    enableBackgroundSubtractionAttrib* enablebackgroundsubtraction = new enableBackgroundSubtractionAttrib();
    Tango::UserDefaultAttrProp enablebackgroundsubtraction_prop;
    enablebackgroundsubtraction_prop.set_description("Enable background subtraction");
    enablebackgroundsubtraction_prop.set_label("Background subtraction");
    enablebackgroundsubtraction->set_default_properties(enablebackgroundsubtraction_prop);
    enablebackgroundsubtraction->set_disp_level(Tango::OPERATOR);
    enablebackgroundsubtraction->set_change_event(true, false);
    att_list.push_back(enablebackgroundsubtraction);

    // Attribute : boardTemperature
    BoardTemperatureAttrib* btemp = new BoardTemperatureAttrib();
    Tango::UserDefaultAttrProp btemp_prop;
    btemp_prop.set_description("Spectrometer board temperature");
    btemp_prop.set_label("Board Temperature");
    btemp_prop.set_unit("\xB0" "C");
    btemp_prop.set_standard_unit("\xB0" "C");
    btemp_prop.set_display_unit("\xB0" "C");
    btemp_prop.set_format("%.1f");
    btemp_prop.set_event_rel_change("0.5");
	btemp_prop.set_event_abs_change("0.2");
    btemp->set_default_properties(btemp_prop);
    btemp->set_polling_period(3000);
    btemp->set_change_event(true, true);
    btemp->set_disp_level(Tango::OPERATOR);
    att_list.push_back(btemp);

    // Attribute : Spectrum
    SpectrumAttrib* spectrum = new SpectrumAttrib();
    Tango::UserDefaultAttrProp    spectrum_prop;
    spectrum_prop.set_description("Last acquired spectrum");
    spectrum_prop.set_label("Spectrum");
    spectrum->set_default_properties(spectrum_prop);
    spectrum->set_disp_level(Tango::OPERATOR);
    spectrum->set_change_event(true, false);
    att_list.push_back(spectrum);

    // Attribute : Wavelength
    WavelengthAttrib* wavelength = new WavelengthAttrib();
    Tango::UserDefaultAttrProp    wavelength_prop;
    wavelength_prop.set_description("Calibrated wavelength axis");
    wavelength_prop.set_label("Wavelength");
    wavelength->set_default_properties(wavelength_prop);
    wavelength->set_disp_level(Tango::OPERATOR);
    att_list.push_back(wavelength);

    // Attribute : SerialNumber
    SerialNumberAttrib* serial = new SerialNumberAttrib();
    Tango::UserDefaultAttrProp    serial_prop;
    serial_prop.set_description("Serial number");
    serial_prop.set_label("Serial number");
    serial->set_default_properties(serial_prop);
    serial->set_disp_level(Tango::OPERATOR);
    att_list.push_back(serial);

    // Attribute : FirmwareVersion
    FirmwareVersionAttrib* fw_ver = new FirmwareVersionAttrib();
    Tango::UserDefaultAttrProp    fw_ver_prop;
    fw_ver_prop.set_description("Firware version");
    fw_ver_prop.set_label("Firware version");
    fw_ver->set_default_properties(fw_ver_prop);
    fw_ver->set_disp_level(Tango::OPERATOR);
    att_list.push_back(fw_ver);

    // Attribute : Model
    ModelAttrib* model = new ModelAttrib();
    Tango::UserDefaultAttrProp model_prop;
    model_prop.set_description("Spectrometer model");
    model_prop.set_label("Model");
    model->set_default_properties(model_prop);
    model->set_disp_level(Tango::OPERATOR);
    att_list.push_back(model);

    // Attribure: IntegrationTime
    EnableTriggerAttrib *trigger = new EnableTriggerAttrib();
    Tango::UserDefaultAttrProp trigger_prop;
    trigger_prop.set_description("Enable externa hardware trigger");
    trigger_prop.set_label("Enable trigger");
    trigger->set_default_properties(trigger_prop);
    trigger->set_disp_level(Tango::OPERATOR);
    trigger->set_change_event(true, false);
    att_list.push_back(trigger);

    // Create a list of static attributes
    create_static_attribute_list(get_class_attr()->get_attr_list());
}


// Command factory
void AvantesSpectrometerClass::command_factory() {

    // Polling of device State
    Tango::Command &stateCmd = get_cmd_by_name("State");
    stateCmd.set_polling_period(500);

    // Command storeBackground
    storeBackgroundClass *pstoreBackgroundCmd = new storeBackgroundClass("storeBackground", Tango::DEV_VOID, Tango::DEV_VOID, "", "", Tango::OPERATOR);
    command_list.push_back(pstoreBackgroundCmd);

    // Command reset
    resetClass *presetCmd = new resetClass("reset", Tango::DEV_VOID, Tango::DEV_VOID, "", "", Tango::OPERATOR);
    command_list.push_back(presetCmd);
}

//===================================================================
// Dynamic attributes related methods
//===================================================================

// Create a list of static attributes
void AvantesSpectrometerClass::create_static_attribute_list(vector<Tango::Attr *> &att_list) {
    for (unsigned long i=0 ; i<att_list.size() ; i++) {
        string att_name(att_list[i]->get_name());
        transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
        defaultAttList.push_back(att_name);
    }
}


// Return attribute object given its name
Tango::Attr *AvantesSpectrometerClass::get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname) {
    vector<Tango::Attr *>::iterator it;
    for (it=att_list.begin() ; it<att_list.end() ; it++)
        if ((*it)->get_name()==attname)
            return (*it);
    // Attr does not exist
    return NULL;
}

} // End of namespace
