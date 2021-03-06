/*----- PROTECTED REGION ID(Standa8SMC1Class.cpp) ENABLED START -----*/
//=============================================================================
//
// file :        Standa8SMC1Class.cpp
//
// description : C++ source for the Standa8SMC1Class.
//               A singleton class derived from DeviceClass.
//               It implements the command and attribute list
//               and all properties and methods required
//               by the Standa8SMC1 once per process.
//
// project :     Standa 8SMC1 Device Server
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
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#include <Standa8SMC1Class.h>
#include <libusmc.h>

/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class.cpp

//-------------------------------------------------------------------
/**
 *	Create Standa8SMC1Class singleton and
 *	return it in a C function for Python usage
 */
//-------------------------------------------------------------------
extern "C" {
#ifdef _TG_WINDOWS_

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_Standa8SMC1_class(const char *name) {
		return Standa8SMC1_ns::Standa8SMC1Class::init(name);
	}
}

namespace Standa8SMC1_ns
{
//===================================================================
//	Initialize pointer for singleton pattern
//===================================================================
Standa8SMC1Class *Standa8SMC1Class::_instance = NULL;

//--------------------------------------------------------
/**
 * method : 		Standa8SMC1Class::Standa8SMC1Class(string &s)
 * description : 	constructor for the Standa8SMC1Class
 *
 * @param s	The class name
 */
//--------------------------------------------------------
Standa8SMC1Class::Standa8SMC1Class(string &s):Tango::DeviceClass(s)
{
	cout2 << "Entering Standa8SMC1Class constructor" << endl;
	set_default_property();
	write_class_property();

	/*----- PROTECTED REGION ID(Standa8SMC1Class::constructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::constructor

	cout2 << "Leaving Standa8SMC1Class constructor" << endl;
}

//--------------------------------------------------------
/**
 * method : 		Standa8SMC1Class::~Standa8SMC1Class()
 * description : 	destructor for the Standa8SMC1Class
 */
//--------------------------------------------------------
Standa8SMC1Class::~Standa8SMC1Class()
{
	/*----- PROTECTED REGION ID(Standa8SMC1Class::destructor) ENABLED START -----*/

	// Shutdown libusmc
	USMC::shutdown();

	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::destructor

	_instance = NULL;
}


//--------------------------------------------------------
/**
 * method : 		Standa8SMC1Class::init
 * description : 	Create the object if not already done.
 *                  Otherwise, just return a pointer to the object
 *
 * @param	name	The class name
 */
//--------------------------------------------------------
Standa8SMC1Class *Standa8SMC1Class::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new Standa8SMC1Class(s);
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
 * method : 		Standa8SMC1Class::instance
 * description : 	Check if object already created,
 *                  and return a pointer to the object
 */
//--------------------------------------------------------
Standa8SMC1Class *Standa8SMC1Class::instance()
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
//--------------------------------------------------------
/**
 * method : 		setHomeClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *setHomeClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "setHomeClass::execute(): arrived" << endl;
	((static_cast<Standa8SMC1 *>(device))->set_home());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		goHomeClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *goHomeClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "goHomeClass::execute(): arrived" << endl;
	((static_cast<Standa8SMC1 *>(device))->go_home());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		StopClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *StopClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "StopClass::execute(): arrived" << endl;
	((static_cast<Standa8SMC1 *>(device))->stop());
	return new CORBA::Any();
}


//===================================================================
//	Properties management
//===================================================================
//--------------------------------------------------------
/**
 *	Method      : Standa8SMC1Class::get_class_property()
 *	Description : Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum Standa8SMC1Class::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, returns  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : Standa8SMC1Class::get_default_device_property()
 *	Description : Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum Standa8SMC1Class::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : Standa8SMC1Class::get_default_class_property()
 *	Description : Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum Standa8SMC1Class::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *	Method      : Standa8SMC1Class::set_default_property()
 *	Description : Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void Standa8SMC1Class::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;
	vector<string>	vect_data;

	//	Set Default Class Properties

	//	Set Default device Properties
	prop_name = "SerialNumber";
	prop_desc = "Serial number of the device";
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
	prop_name = "polling";
	prop_desc = "Polling period for device position";
	prop_def  = "500";
	vect_data.clear();
	vect_data.push_back("500");
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
 *	Method      : Standa8SMC1Class::write_class_property()
 *	Description : Set class description fields as property in database
 */
//--------------------------------------------------------
void Standa8SMC1Class::write_class_property()
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
	string	str_title("Standa 8SMC1 Device Server");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("Device server for Standa 8SMC1-USBhF stepper motor controllers");
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
 *	Method      : Standa8SMC1Class::device_factory()
 *	Description : Create the device object(s)
 *                and store them in the device list
 */
//--------------------------------------------------------
void Standa8SMC1Class::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	/*----- PROTECTED REGION ID(Standa8SMC1Class::device_factory_before) ENABLED START -----*/

	// Load and initialize libusmc
	USMC* driver = USMC::getInstance();
	driver->probeDevices();

	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::device_factory_before

	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
		device_list.push_back(new Standa8SMC1(this, (*devlist_ptr)[i]));
	}

	//	Manage dynamic attributes if any
	erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		Standa8SMC1 *dev = static_cast<Standa8SMC1 *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}

	/*----- PROTECTED REGION ID(Standa8SMC1Class::device_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::device_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : Standa8SMC1Class::attribute_factory()
 *	Description : Create the attribute object(s)
 *                and store them in the attribute list
 */
//--------------------------------------------------------
void Standa8SMC1Class::attribute_factory(vector<Tango::Attr *> &att_list)
{
	/*----- PROTECTED REGION ID(Standa8SMC1Class::attribute_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::attribute_factory_before
	//	Attribute : Position
	PositionAttrib	*position = new PositionAttrib();
	Tango::UserDefaultAttrProp	position_prop;
	//	description	not set for Position
	position_prop.set_label("Position");
	position_prop.set_unit("steps");
	position_prop.set_standard_unit("steps");
	position_prop.set_display_unit("steps");
	position_prop.set_format("%d");
	//	max_value	not set for Position
	//	min_value	not set for Position
	//	max_alarm	not set for Position
	//	min_alarm	not set for Position
	//	max_warning	not set for Position
	//	min_warning	not set for Position
	//	delta_t	not set for Position
	//	delta_val	not set for Position
	
	position->set_default_properties(position_prop);
	//	Not Polled
	position->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	position->set_change_event(true, false);
	att_list.push_back(position);

	//	Attribute : Acceleration
	AccelerationAttrib	*acceleration = new AccelerationAttrib();
	Tango::UserDefaultAttrProp	acceleration_prop;
	//	description	not set for Acceleration
	acceleration_prop.set_label("Acceleration");
	acceleration_prop.set_unit("ms");
	acceleration_prop.set_standard_unit("ms");
	acceleration_prop.set_display_unit("ms");
	acceleration_prop.set_format("%.1f");
	//	max_value	not set for Acceleration
	//	min_value	not set for Acceleration
	//	max_alarm	not set for Acceleration
	//	min_alarm	not set for Acceleration
	//	max_warning	not set for Acceleration
	//	min_warning	not set for Acceleration
	//	delta_t	not set for Acceleration
	//	delta_val	not set for Acceleration
	
	acceleration->set_default_properties(acceleration_prop);
	//	Not Polled
	acceleration->set_disp_level(Tango::OPERATOR);
	acceleration->set_memorized();
	acceleration->set_memorized_init(true);
	acceleration->set_change_event(true, false);
	att_list.push_back(acceleration);

	//	Attribute : Velocity
	VelocityAttrib	*velocity = new VelocityAttrib();
	Tango::UserDefaultAttrProp	velocity_prop;
	//	description	not set for Velocity
	velocity_prop.set_label("Velocity");
	velocity_prop.set_unit("steps/s");
	velocity_prop.set_standard_unit("steps/s");
	velocity_prop.set_display_unit("steps/s");
	velocity_prop.set_format("%.0f");
	//	max_value	not set for Velocity
	//	min_value	not set for Velocity
	//	max_alarm	not set for Velocity
	//	min_alarm	not set for Velocity
	//	max_warning	not set for Velocity
	//	min_warning	not set for Velocity
	//	delta_t	not set for Velocity
	//	delta_val	not set for Velocity
	
	velocity->set_default_properties(velocity_prop);
	//	Not Polled
	velocity->set_disp_level(Tango::OPERATOR);
	velocity->set_memorized();
	velocity->set_memorized_init(true);
	velocity->set_change_event(true, false);
	att_list.push_back(velocity);

	//	Attribute : Temperature
	TemperatureAttrib	*temperature = new TemperatureAttrib();
	Tango::UserDefaultAttrProp	temperature_prop;
	//	description	not set for Temperature
	temperature_prop.set_label("Temperature");
	temperature_prop.set_unit("degC");
	temperature_prop.set_standard_unit("degC");
	temperature_prop.set_display_unit("degC");
	temperature_prop.set_format("%.1f");
	//	max_value	not set for Temperature
	//	min_value	not set for Temperature
	//	max_alarm	not set for Temperature
	//	min_alarm	not set for Temperature
	//	max_warning	not set for Temperature
	//	min_warning	not set for Temperature
	//	delta_t	not set for Temperature
	//	delta_val	not set for Temperature
	
	temperature->set_default_properties(temperature_prop);
	//	Not Polled
	temperature->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	temperature->set_change_event(true, false);
	att_list.push_back(temperature);

	//	Attribute : Voltage
	VoltageAttrib	*voltage = new VoltageAttrib();
	Tango::UserDefaultAttrProp	voltage_prop;
	//	description	not set for Voltage
	voltage_prop.set_label("Voltage");
	voltage_prop.set_unit("V");
	voltage_prop.set_standard_unit("V");
	voltage_prop.set_display_unit("V");
	voltage_prop.set_format("%.1f");
	//	max_value	not set for Voltage
	//	min_value	not set for Voltage
	//	max_alarm	not set for Voltage
	//	min_alarm	not set for Voltage
	//	max_warning	not set for Voltage
	//	min_warning	not set for Voltage
	//	delta_t	not set for Voltage
	//	delta_val	not set for Voltage
	
	voltage->set_default_properties(voltage_prop);
	//	Not Polled
	voltage->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	voltage->set_change_event(true, false);
	att_list.push_back(voltage);


	//	Create a list of static attributes
	create_static_attribute_list(get_class_attr()->get_attr_list());
	/*----- PROTECTED REGION ID(Standa8SMC1Class::attribute_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::attribute_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : Standa8SMC1Class::pipe_factory()
 *	Description : Create the pipe object(s)
 *                and store them in the pipe list
 */
//--------------------------------------------------------
void Standa8SMC1Class::pipe_factory()
{
	/*----- PROTECTED REGION ID(Standa8SMC1Class::pipe_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::pipe_factory_before
	/*----- PROTECTED REGION ID(Standa8SMC1Class::pipe_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::pipe_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : Standa8SMC1Class::command_factory()
 *	Description : Create the command object(s)
 *                and store them in the command list
 */
//--------------------------------------------------------
void Standa8SMC1Class::command_factory()
{
	/*----- PROTECTED REGION ID(Standa8SMC1Class::command_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::command_factory_before

	//	Set polling perod for command State
	Tango::Command	&stateCmd = get_cmd_by_name("State");
	stateCmd.set_polling_period(500);
	

	//	Command setHome
	setHomeClass	*psetHomeCmd =
		new setHomeClass("setHome",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(psetHomeCmd);

	//	Command goHome
	goHomeClass	*pgoHomeCmd =
		new goHomeClass("goHome",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pgoHomeCmd);

	//	Command Stop
	StopClass	*pStopCmd =
		new StopClass("Stop",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pStopCmd);

	/*----- PROTECTED REGION ID(Standa8SMC1Class::command_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::command_factory_after
}

//===================================================================
//	Dynamic attributes related methods
//===================================================================

//--------------------------------------------------------
/**
 * method : 		Standa8SMC1Class::create_static_attribute_list
 * description : 	Create the a list of static attributes
 *
 * @param	att_list	the ceated attribute list
 */
//--------------------------------------------------------
void Standa8SMC1Class::create_static_attribute_list(vector<Tango::Attr *> &att_list)
{
	for (unsigned long i=0 ; i<att_list.size() ; i++)
	{
		string att_name(att_list[i]->get_name());
		transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
		defaultAttList.push_back(att_name);
	}

	cout2 << defaultAttList.size() << " attributes in default list" << endl;

	/*----- PROTECTED REGION ID(Standa8SMC1Class::create_static_att_list) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::create_static_att_list
}


//--------------------------------------------------------
/**
 * method : 		Standa8SMC1Class::erase_dynamic_attributes
 * description : 	delete the dynamic attributes if any.
 *
 * @param	devlist_ptr	the device list pointer
 * @param	list of all attributes
 */
//--------------------------------------------------------
void Standa8SMC1Class::erase_dynamic_attributes(const Tango::DevVarStringArray *devlist_ptr, vector<Tango::Attr *> &att_list)
{
	Tango::Util *tg = Tango::Util::instance();

	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		Tango::DeviceImpl *dev_impl = tg->get_device_by_name(((string)(*devlist_ptr)[i]).c_str());
		Standa8SMC1 *dev = static_cast<Standa8SMC1 *> (dev_impl);

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
	/*----- PROTECTED REGION ID(Standa8SMC1Class::erase_dynamic_attributes) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::erase_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : Standa8SMC1Class::get_attr_by_name()
 *	Description : returns Tango::Attr * object found by name
 */
//--------------------------------------------------------
Tango::Attr *Standa8SMC1Class::get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname)
{
	vector<Tango::Attr *>::iterator it;
	for (it=att_list.begin() ; it<att_list.end() ; ++it)
		if ((*it)->get_name()==attname)
			return (*it);
	//	Attr does not exist
	return NULL;
}


/*----- PROTECTED REGION ID(Standa8SMC1Class::Additional Methods) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::Additional Methods
} //	namespace
