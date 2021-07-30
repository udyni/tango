/*----- PROTECTED REGION ID(VibrationMeterClass.cpp) ENABLED START -----*/
// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//=============================================================================
//
// file :        VibrationMeterClass.cpp
//
// description : C++ source for the VibrationMeterClass.
//               A singleton class derived from DeviceClass.
//               It implements the command and attribute list
//               and all properties and methods required
//               by the VibrationMeter once per process.
//
// project :     Vibration meter
//
// This file is part of Tango device class.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
//
// 2021 CNR-IFN
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#include <VibrationMeterClass.h>

/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass.cpp

//-------------------------------------------------------------------
/**
 *	Create VibrationMeterClass singleton and
 *	return it in a C function for Python usage
 */
//-------------------------------------------------------------------
extern "C" {
#ifdef _TG_WINDOWS_

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_VibrationMeter_class(const char *name) {
		return VibrationMeter_ns::VibrationMeterClass::init(name);
	}
}

namespace VibrationMeter_ns
{
//===================================================================
//	Initialize pointer for singleton pattern
//===================================================================
VibrationMeterClass *VibrationMeterClass::_instance = NULL;

//--------------------------------------------------------
/**
 * method : 		VibrationMeterClass::VibrationMeterClass(string &s)
 * description : 	constructor for the VibrationMeterClass
 *
 * @param s	The class name
 */
//--------------------------------------------------------
VibrationMeterClass::VibrationMeterClass(string &s):Tango::DeviceClass(s)
{
	cout2 << "Entering VibrationMeterClass constructor" << endl;
	set_default_property();
	write_class_property();

	/*----- PROTECTED REGION ID(VibrationMeterClass::constructor) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::constructor

	cout2 << "Leaving VibrationMeterClass constructor" << endl;
}

//--------------------------------------------------------
/**
 * method : 		VibrationMeterClass::~VibrationMeterClass()
 * description : 	destructor for the VibrationMeterClass
 */
//--------------------------------------------------------
VibrationMeterClass::~VibrationMeterClass()
{
	/*----- PROTECTED REGION ID(VibrationMeterClass::destructor) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::destructor

	_instance = NULL;
}


//--------------------------------------------------------
/**
 * method : 		VibrationMeterClass::init
 * description : 	Create the object if not already done.
 *                  Otherwise, just return a pointer to the object
 *
 * @param	name	The class name
 */
//--------------------------------------------------------
VibrationMeterClass *VibrationMeterClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new VibrationMeterClass(s);
		}
		catch (bad_alloc &)
		{
			throw;
		}
	}
	return _instance;
}

//--------------------------------------------------------
/**
 * method : 		VibrationMeterClass::instance
 * description : 	Check if object already created,
 *                  and return a pointer to the object
 */
//--------------------------------------------------------
VibrationMeterClass *VibrationMeterClass::instance()
{
	if (_instance == NULL)
	{
		cerr << "Class is not initialised !!" << endl;
		exit(-1);
	}
	return _instance;
}



//===================================================================
//	Command execution method calls
//===================================================================

//===================================================================
//	Properties management
//===================================================================
//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::get_class_property()
 *	Description : Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum VibrationMeterClass::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, returns  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::get_default_device_property()
 *	Description : Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum VibrationMeterClass::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::get_default_class_property()
 *	Description : Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum VibrationMeterClass::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::set_default_property()
 *	Description : Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void VibrationMeterClass::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;
	vector<string>	vect_data;

	//	Set Default Class Properties

	//	Set Default device Properties
	prop_name = "bus";
	prop_desc = "I2C bus number";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "address";
	prop_desc = "I2C address";
	prop_def  = "53";
	vect_data.clear();
	vect_data.push_back("53");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "disable_fft";
	prop_desc = "Disable FFT calculation";
	prop_def  = "False";
	vect_data.clear();
	vect_data.push_back("False");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::write_class_property()
 *	Description : Set class description fields as property in database
 */
//--------------------------------------------------------
void VibrationMeterClass::write_class_property()
{
	//	First time, check if database used
	if (Tango::Util::_UseDb == false)
		return;

	Tango::DbData	data;
	string	classname = get_name();
	string	header;
	string::size_type	start, end;

	//	Put title
	Tango::DbDatum	title("ProjectTitle");
	string	str_title("Vibration meter");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("Vibration meter based on ADXL345");
	description << str_desc;
	data.push_back(description);

	//  Put inheritance
	Tango::DbDatum	inher_datum("InheritedFrom");
	vector<string> inheritance;
	inheritance.push_back("TANGO_BASE_CLASS");
	inher_datum << inheritance;
	data.push_back(inher_datum);

	//	Call database and and values
	get_db_class()->put_property(data);
}

//===================================================================
//	Factory methods
//===================================================================

//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::device_factory()
 *	Description : Create the device object(s)
 *                and store them in the device list
 */
//--------------------------------------------------------
void VibrationMeterClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	/*----- PROTECTED REGION ID(VibrationMeterClass::device_factory_before) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::device_factory_before

	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
		device_list.push_back(new VibrationMeter(this, (*devlist_ptr)[i]));
	}

	//	Manage dynamic attributes if any
	erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		VibrationMeter *dev = static_cast<VibrationMeter *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}

	/*----- PROTECTED REGION ID(VibrationMeterClass::device_factory_after) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::device_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::attribute_factory()
 *	Description : Create the attribute object(s)
 *                and store them in the attribute list
 */
//--------------------------------------------------------
void VibrationMeterClass::attribute_factory(vector<Tango::Attr *> &att_list)
{
	/*----- PROTECTED REGION ID(VibrationMeterClass::attribute_factory_before) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::attribute_factory_before
	//	Attribute : Samples
	SamplesAttrib	*samples = new SamplesAttrib();
	Tango::UserDefaultAttrProp	samples_prop;
	samples_prop.set_description("Number of samples");
	samples_prop.set_label("Samples");
	//	unit	not set for Samples
	//	standard_unit	not set for Samples
	//	display_unit	not set for Samples
	//	format	not set for Samples
	//	max_value	not set for Samples
	//	min_value	not set for Samples
	//	max_alarm	not set for Samples
	//	min_alarm	not set for Samples
	//	max_warning	not set for Samples
	//	min_warning	not set for Samples
	//	delta_t	not set for Samples
	//	delta_val	not set for Samples
	
	samples->set_default_properties(samples_prop);
	//	Not Polled
	samples->set_disp_level(Tango::OPERATOR);
	samples->set_memorized();
	samples->set_memorized_init(true);
	samples->set_change_event(true, false);
	att_list.push_back(samples);

	//	Attribute : SampleFrequency
	SampleFrequencyAttrib	*samplefrequency = new SampleFrequencyAttrib();
	Tango::UserDefaultAttrProp	samplefrequency_prop;
	samplefrequency_prop.set_description("Sample Frequency");
	samplefrequency_prop.set_label("Sample Frequecy");
	//	unit	not set for SampleFrequency
	//	standard_unit	not set for SampleFrequency
	//	display_unit	not set for SampleFrequency
	//	format	not set for SampleFrequency
	//	max_value	not set for SampleFrequency
	//	min_value	not set for SampleFrequency
	//	max_alarm	not set for SampleFrequency
	//	min_alarm	not set for SampleFrequency
	//	max_warning	not set for SampleFrequency
	//	min_warning	not set for SampleFrequency
	//	delta_t	not set for SampleFrequency
	//	delta_val	not set for SampleFrequency
	
	{
		vector<string> labels;
		labels.push_back("4000Hz");
		labels.push_back("2000Hz");
		labels.push_back("1000Hz");
		labels.push_back("500Hz");
		labels.push_back("250Hz");
		labels.push_back("125Hz");
		labels.push_back("62.5Hz");
		labels.push_back("31.25Hz");
		labels.push_back("15.625Hz");
		labels.push_back("7.8125Hz");
		labels.push_back("3.90625Hz");
		samplefrequency_prop.set_enum_labels(labels);
	}
	samplefrequency->set_default_properties(samplefrequency_prop);
	//	Not Polled
	samplefrequency->set_disp_level(Tango::OPERATOR);
	samplefrequency->set_memorized();
	samplefrequency->set_memorized_init(true);
	samplefrequency->set_change_event(true, false);
	att_list.push_back(samplefrequency);

	//	Attribute : Period
	PeriodAttrib	*period = new PeriodAttrib();
	Tango::UserDefaultAttrProp	period_prop;
	period_prop.set_description("Sample period in seconds");
	period_prop.set_label("Sample period");
	//	unit	not set for Period
	//	standard_unit	not set for Period
	period_prop.set_display_unit("s");
	period_prop.set_format("%.3f");
	//	max_value	not set for Period
	//	min_value	not set for Period
	//	max_alarm	not set for Period
	//	min_alarm	not set for Period
	//	max_warning	not set for Period
	//	min_warning	not set for Period
	//	delta_t	not set for Period
	//	delta_val	not set for Period
	
	period->set_default_properties(period_prop);
	//	Not Polled
	period->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	att_list.push_back(period);

	//	Attribute : FrequencyStep
	FrequencyStepAttrib	*frequencystep = new FrequencyStepAttrib();
	Tango::UserDefaultAttrProp	frequencystep_prop;
	frequencystep_prop.set_description("Frequecy step of the FFT");
	frequencystep_prop.set_label("Frequecy step");
	//	unit	not set for FrequencyStep
	//	standard_unit	not set for FrequencyStep
	//	display_unit	not set for FrequencyStep
	//	format	not set for FrequencyStep
	//	max_value	not set for FrequencyStep
	//	min_value	not set for FrequencyStep
	//	max_alarm	not set for FrequencyStep
	//	min_alarm	not set for FrequencyStep
	//	max_warning	not set for FrequencyStep
	//	min_warning	not set for FrequencyStep
	//	delta_t	not set for FrequencyStep
	//	delta_val	not set for FrequencyStep
	
	frequencystep->set_default_properties(frequencystep_prop);
	//	Not Polled
	frequencystep->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	frequencystep->set_change_event(true, false);
	att_list.push_back(frequencystep);

	//	Attribute : X_spectrum
	X_spectrumAttrib	*x_spectrum = new X_spectrumAttrib();
	Tango::UserDefaultAttrProp	x_spectrum_prop;
	x_spectrum_prop.set_description("Vibration spectrum on X axis");
	x_spectrum_prop.set_label("X spectrum");
	//	unit	not set for X_spectrum
	//	standard_unit	not set for X_spectrum
	//	display_unit	not set for X_spectrum
	//	format	not set for X_spectrum
	//	max_value	not set for X_spectrum
	//	min_value	not set for X_spectrum
	//	max_alarm	not set for X_spectrum
	//	min_alarm	not set for X_spectrum
	//	max_warning	not set for X_spectrum
	//	min_warning	not set for X_spectrum
	//	delta_t	not set for X_spectrum
	//	delta_val	not set for X_spectrum
	
	x_spectrum->set_default_properties(x_spectrum_prop);
	//	Not Polled
	x_spectrum->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	x_spectrum->set_change_event(true, false);
	att_list.push_back(x_spectrum);

	//	Attribute : Y_spectrum
	Y_spectrumAttrib	*y_spectrum = new Y_spectrumAttrib();
	Tango::UserDefaultAttrProp	y_spectrum_prop;
	y_spectrum_prop.set_description("Vibration spectrum on Y axis");
	y_spectrum_prop.set_label("Y spectrum");
	//	unit	not set for Y_spectrum
	//	standard_unit	not set for Y_spectrum
	//	display_unit	not set for Y_spectrum
	//	format	not set for Y_spectrum
	//	max_value	not set for Y_spectrum
	//	min_value	not set for Y_spectrum
	//	max_alarm	not set for Y_spectrum
	//	min_alarm	not set for Y_spectrum
	//	max_warning	not set for Y_spectrum
	//	min_warning	not set for Y_spectrum
	//	delta_t	not set for Y_spectrum
	//	delta_val	not set for Y_spectrum
	
	y_spectrum->set_default_properties(y_spectrum_prop);
	//	Not Polled
	y_spectrum->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	y_spectrum->set_change_event(true, false);
	att_list.push_back(y_spectrum);

	//	Attribute : Z_spectrum
	Z_spectrumAttrib	*z_spectrum = new Z_spectrumAttrib();
	Tango::UserDefaultAttrProp	z_spectrum_prop;
	z_spectrum_prop.set_description("Vibration spectrum on Z axis");
	z_spectrum_prop.set_label("Z spectrum");
	//	unit	not set for Z_spectrum
	//	standard_unit	not set for Z_spectrum
	//	display_unit	not set for Z_spectrum
	//	format	not set for Z_spectrum
	//	max_value	not set for Z_spectrum
	//	min_value	not set for Z_spectrum
	//	max_alarm	not set for Z_spectrum
	//	min_alarm	not set for Z_spectrum
	//	max_warning	not set for Z_spectrum
	//	min_warning	not set for Z_spectrum
	//	delta_t	not set for Z_spectrum
	//	delta_val	not set for Z_spectrum
	
	z_spectrum->set_default_properties(z_spectrum_prop);
	//	Not Polled
	z_spectrum->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	z_spectrum->set_change_event(true, false);
	att_list.push_back(z_spectrum);

	//	Attribute : RawX
	RawXAttrib	*rawx = new RawXAttrib();
	Tango::UserDefaultAttrProp	rawx_prop;
	//	description	not set for RawX
	//	label	not set for RawX
	//	unit	not set for RawX
	//	standard_unit	not set for RawX
	//	display_unit	not set for RawX
	//	format	not set for RawX
	//	max_value	not set for RawX
	//	min_value	not set for RawX
	//	max_alarm	not set for RawX
	//	min_alarm	not set for RawX
	//	max_warning	not set for RawX
	//	min_warning	not set for RawX
	//	delta_t	not set for RawX
	//	delta_val	not set for RawX
	
	rawx->set_default_properties(rawx_prop);
	//	Not Polled
	rawx->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	rawx->set_change_event(true, false);
	att_list.push_back(rawx);

	//	Attribute : RawY
	RawYAttrib	*rawy = new RawYAttrib();
	Tango::UserDefaultAttrProp	rawy_prop;
	//	description	not set for RawY
	//	label	not set for RawY
	//	unit	not set for RawY
	//	standard_unit	not set for RawY
	//	display_unit	not set for RawY
	//	format	not set for RawY
	//	max_value	not set for RawY
	//	min_value	not set for RawY
	//	max_alarm	not set for RawY
	//	min_alarm	not set for RawY
	//	max_warning	not set for RawY
	//	min_warning	not set for RawY
	//	delta_t	not set for RawY
	//	delta_val	not set for RawY
	
	rawy->set_default_properties(rawy_prop);
	//	Not Polled
	rawy->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	rawy->set_change_event(true, false);
	att_list.push_back(rawy);

	//	Attribute : RawZ
	RawZAttrib	*rawz = new RawZAttrib();
	Tango::UserDefaultAttrProp	rawz_prop;
	//	description	not set for RawZ
	//	label	not set for RawZ
	//	unit	not set for RawZ
	//	standard_unit	not set for RawZ
	//	display_unit	not set for RawZ
	//	format	not set for RawZ
	//	max_value	not set for RawZ
	//	min_value	not set for RawZ
	//	max_alarm	not set for RawZ
	//	min_alarm	not set for RawZ
	//	max_warning	not set for RawZ
	//	min_warning	not set for RawZ
	//	delta_t	not set for RawZ
	//	delta_val	not set for RawZ
	
	rawz->set_default_properties(rawz_prop);
	//	Not Polled
	rawz->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	rawz->set_change_event(true, false);
	att_list.push_back(rawz);


	//	Create a list of static attributes
	create_static_attribute_list(get_class_attr()->get_attr_list());
	/*----- PROTECTED REGION ID(VibrationMeterClass::attribute_factory_after) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::attribute_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::pipe_factory()
 *	Description : Create the pipe object(s)
 *                and store them in the pipe list
 */
//--------------------------------------------------------
void VibrationMeterClass::pipe_factory()
{
	/*----- PROTECTED REGION ID(VibrationMeterClass::pipe_factory_before) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::pipe_factory_before
	/*----- PROTECTED REGION ID(VibrationMeterClass::pipe_factory_after) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::pipe_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::command_factory()
 *	Description : Create the command object(s)
 *                and store them in the command list
 */
//--------------------------------------------------------
void VibrationMeterClass::command_factory()
{
	/*----- PROTECTED REGION ID(VibrationMeterClass::command_factory_before) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::command_factory_before

	//	Set polling perod for command State
	Tango::Command	&stateCmd = get_cmd_by_name("State");
	stateCmd.set_polling_period(500);
	

	/*----- PROTECTED REGION ID(VibrationMeterClass::command_factory_after) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::command_factory_after
}

//===================================================================
//	Dynamic attributes related methods
//===================================================================

//--------------------------------------------------------
/**
 * method : 		VibrationMeterClass::create_static_attribute_list
 * description : 	Create the a list of static attributes
 *
 * @param	att_list	the ceated attribute list
 */
//--------------------------------------------------------
void VibrationMeterClass::create_static_attribute_list(vector<Tango::Attr *> &att_list)
{
	for (unsigned long i=0 ; i<att_list.size() ; i++)
	{
		string att_name(att_list[i]->get_name());
		transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
		defaultAttList.push_back(att_name);
	}

	cout2 << defaultAttList.size() << " attributes in default list" << endl;

	/*----- PROTECTED REGION ID(VibrationMeterClass::create_static_att_list) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::create_static_att_list
}


//--------------------------------------------------------
/**
 * method : 		VibrationMeterClass::erase_dynamic_attributes
 * description : 	delete the dynamic attributes if any.
 *
 * @param	devlist_ptr	the device list pointer
 * @param	list of all attributes
 */
//--------------------------------------------------------
void VibrationMeterClass::erase_dynamic_attributes(const Tango::DevVarStringArray *devlist_ptr, vector<Tango::Attr *> &att_list)
{
	Tango::Util *tg = Tango::Util::instance();

	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		Tango::DeviceImpl *dev_impl = tg->get_device_by_name(((string)(*devlist_ptr)[i]).c_str());
		VibrationMeter *dev = static_cast<VibrationMeter *> (dev_impl);

		vector<Tango::Attribute *> &dev_att_list = dev->get_device_attr()->get_attribute_list();
		vector<Tango::Attribute *>::iterator ite_att;
		for (ite_att=dev_att_list.begin() ; ite_att != dev_att_list.end() ; ++ite_att)
		{
			string att_name((*ite_att)->get_name_lower());
			if ((att_name == "state") || (att_name == "status"))
				continue;
			vector<string>::iterator ite_str = find(defaultAttList.begin(), defaultAttList.end(), att_name);
			if (ite_str == defaultAttList.end())
			{
				cout2 << att_name << " is a UNWANTED dynamic attribute for device " << (*devlist_ptr)[i] << endl;
				Tango::Attribute &att = dev->get_device_attr()->get_attr_by_name(att_name.c_str());
				dev->remove_attribute(att_list[att.get_attr_idx()], true, false);
				--ite_att;
			}
		}
	}
	/*----- PROTECTED REGION ID(VibrationMeterClass::erase_dynamic_attributes) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::erase_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeterClass::get_attr_by_name()
 *	Description : returns Tango::Attr * object found by name
 */
//--------------------------------------------------------
Tango::Attr *VibrationMeterClass::get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname)
{
	vector<Tango::Attr *>::iterator it;
	for (it=att_list.begin() ; it<att_list.end() ; ++it)
		if ((*it)->get_name()==attname)
			return (*it);
	//	Attr does not exist
	return NULL;
}


/*----- PROTECTED REGION ID(VibrationMeterClass::Additional Methods) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	VibrationMeterClass::Additional Methods
} //	namespace
