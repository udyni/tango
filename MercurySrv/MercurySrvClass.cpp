/*----- PROTECTED REGION ID(MercurySrvClass.cpp) ENABLED START -----*/
static const char *RcsId      = "$Id:  $";
static const char *TagName    = "$Name:  $";
static const char *CvsPath    = "$Source:  $";
static const char *SvnPath    = "$HeadURL:  $";
static const char *HttpServer = "http://www.esrf.eu/computing/cs/tango/tango_doc/ds_doc/";
//=============================================================================
//
// file :        MercurySrvClass.cpp
//
// description : C++ source for the MercurySrvClass.
//               A singleton class derived from DeviceClass.
//               It implements the command and attribute list
//               and all properties and methods required
//               by the MercurySrv once per process.
//
// project :     Mercury C863 Server
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
// $Author:  $
//
// $Revision:  $
// $Date:  $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#include <MercurySrvClass.h>

/*----- PROTECTED REGION END -----*/	//	MercurySrvClass.cpp

//-------------------------------------------------------------------
/**
 *	Create MercurySrvClass singleton and
 *	return it in a C function for Python usage
 */
//-------------------------------------------------------------------
extern "C" {
#ifdef _TG_WINDOWS_

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_MercurySrv_class(const char *name) {
		return MercurySrv_ns::MercurySrvClass::init(name);
	}
}

namespace MercurySrv_ns
{
//===================================================================
//	Initialize pointer for singleton pattern
//===================================================================
MercurySrvClass *MercurySrvClass::_instance = NULL;

//--------------------------------------------------------
/**
 * method : 		MercurySrvClass::MercurySrvClass(string &s)
 * description : 	constructor for the MercurySrvClass
 *
 * @param s	The class name
 */
//--------------------------------------------------------
MercurySrvClass::MercurySrvClass(string &s):Tango::DeviceClass(s)
{
	cout2 << "Entering MercurySrvClass constructor" << endl;
	set_default_property();
	write_class_property();

	/*----- PROTECTED REGION ID(MercurySrvClass::constructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::constructor

	cout2 << "Leaving MercurySrvClass constructor" << endl;
}

//--------------------------------------------------------
/**
 * method : 		MercurySrvClass::~MercurySrvClass()
 * description : 	destructor for the MercurySrvClass
 */
//--------------------------------------------------------
MercurySrvClass::~MercurySrvClass()
{
	/*----- PROTECTED REGION ID(MercurySrvClass::destructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::destructor

	_instance = NULL;
}


//--------------------------------------------------------
/**
 * method : 		MercurySrvClass::init
 * description : 	Create the object if not already done.
 *                  Otherwise, just return a pointer to the object
 *
 * @param	name	The class name
 */
//--------------------------------------------------------
MercurySrvClass *MercurySrvClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new MercurySrvClass(s);
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
 * method : 		MercurySrvClass::instance
 * description : 	Check if object already created,
 *                  and return a pointer to the object
 */
//--------------------------------------------------------
MercurySrvClass *MercurySrvClass::instance()
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
	((static_cast<MercurySrv *>(device))->set_home());
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
	((static_cast<MercurySrv *>(device))->go_home());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		timeStepClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *timeStepClass::execute(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
	cout2 << "timeStepClass::execute(): arrived" << endl;
	Tango::DevDouble argin;
	extract(in_any, argin);
	((static_cast<MercurySrv *>(device))->time_step(argin));
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		posStepClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *posStepClass::execute(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
	cout2 << "posStepClass::execute(): arrived" << endl;
	Tango::DevDouble argin;
	extract(in_any, argin);
	((static_cast<MercurySrv *>(device))->pos_step(argin));
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
	((static_cast<MercurySrv *>(device))->stop());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		AbortClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *AbortClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "AbortClass::execute(): arrived" << endl;
	((static_cast<MercurySrv *>(device))->abort());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		ResetClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *ResetClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "ResetClass::execute(): arrived" << endl;
	((static_cast<MercurySrv *>(device))->reset());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		StartMacroClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *StartMacroClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "StartMacroClass::execute(): arrived" << endl;
	((static_cast<MercurySrv *>(device))->start_macro());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		StopMacroClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *StopMacroClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "StopMacroClass::execute(): arrived" << endl;
	((static_cast<MercurySrv *>(device))->stop_macro());
	return new CORBA::Any();
}


//===================================================================
//	Properties management
//===================================================================
//--------------------------------------------------------
/**
 *	Method      : MercurySrvClass::get_class_property()
 *	Description : Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum MercurySrvClass::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, returns  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrvClass::get_default_device_property()
 *	Description : Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum MercurySrvClass::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrvClass::get_default_class_property()
 *	Description : Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum MercurySrvClass::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *	Method      : MercurySrvClass::set_default_property()
 *	Description : Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void MercurySrvClass::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;
	vector<string>	vect_data;

	//	Set Default Class Properties

	//	Set Default device Properties
	prop_name = "Proxy";
	prop_desc = "Communication proxy for the mercury controller";
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
	prop_name = "Address";
	prop_desc = "Controller address on the RS232 daisy chain (default to 1)";
	prop_def  = "1";
	vect_data.clear();
	vect_data.push_back("1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "Referenced";
	prop_desc = "Decide wether the motion of the stage should be referenced or not. Default to `true`.";
	prop_def  = "true";
	vect_data.clear();
	vect_data.push_back("true");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "TimeConversion";
	prop_desc = "Conversion factor from position to time (Default to 2L / c in femtoseconds)";
	prop_def  = "6671.282";
	vect_data.clear();
	vect_data.push_back("6671.282");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "PollingPeriod";
	prop_desc = "Polling thread period in ms";
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
	prop_name = "StageConfiguration";
	prop_desc = "Stage configuration with the comma separated list pairs <hex index>=<value>";
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
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrvClass::write_class_property()
 *	Description : Set class description fields as property in database
 */
//--------------------------------------------------------
void MercurySrvClass::write_class_property()
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
	string	str_title("Mercury C863 Server");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("Tango device server for a Mercury C863 motion controller");
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
 *	Method      : MercurySrvClass::device_factory()
 *	Description : Create the device object(s)
 *                and store them in the device list
 */
//--------------------------------------------------------
void MercurySrvClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	/*----- PROTECTED REGION ID(MercurySrvClass::device_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::device_factory_before

	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
		device_list.push_back(new MercurySrv(this, (*devlist_ptr)[i]));
	}

	//	Manage dynamic attributes if any
	erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		MercurySrv *dev = static_cast<MercurySrv *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}

	/*----- PROTECTED REGION ID(MercurySrvClass::device_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::device_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : MercurySrvClass::attribute_factory()
 *	Description : Create the attribute object(s)
 *                and store them in the attribute list
 */
//--------------------------------------------------------
void MercurySrvClass::attribute_factory(vector<Tango::Attr *> &att_list)
{
	/*----- PROTECTED REGION ID(MercurySrvClass::attribute_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::attribute_factory_before
	//	Attribute : Acceleration
	AccelerationAttrib	*acceleration = new AccelerationAttrib();
	Tango::UserDefaultAttrProp	acceleration_prop;
	acceleration_prop.set_description("Stage acceleration");
	acceleration_prop.set_label("Acceleration");
	acceleration_prop.set_unit("mm/s^2");
	acceleration_prop.set_standard_unit("mm/s^2");
	//	display_unit	not set for Acceleration
	//	format	not set for Acceleration
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
	acceleration->set_memorized_init(false);
	acceleration->set_change_event(true, false);
	att_list.push_back(acceleration);

	//	Attribute : Velocity
	VelocityAttrib	*velocity = new VelocityAttrib();
	Tango::UserDefaultAttrProp	velocity_prop;
	velocity_prop.set_description("Stage velocity");
	velocity_prop.set_label("Velocity");
	velocity_prop.set_unit("mm/s");
	velocity_prop.set_standard_unit("mm/s");
	//	display_unit	not set for Velocity
	//	format	not set for Velocity
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
	velocity->set_memorized_init(false);
	velocity->set_change_event(true, false);
	att_list.push_back(velocity);

	//	Attribute : RawPosition
	RawPositionAttrib	*rawposition = new RawPositionAttrib();
	Tango::UserDefaultAttrProp	rawposition_prop;
	//	description	not set for RawPosition
	rawposition_prop.set_label("Raw position");
	rawposition_prop.set_unit("mm");
	rawposition_prop.set_standard_unit("mm");
	rawposition_prop.set_display_unit("mm");
	rawposition_prop.set_format("%.3f");
	//	max_value	not set for RawPosition
	//	min_value	not set for RawPosition
	//	max_alarm	not set for RawPosition
	//	min_alarm	not set for RawPosition
	//	max_warning	not set for RawPosition
	//	min_warning	not set for RawPosition
	//	delta_t	not set for RawPosition
	//	delta_val	not set for RawPosition
	
	rawposition->set_default_properties(rawposition_prop);
	//	Not Polled
	rawposition->set_disp_level(Tango::OPERATOR);
	rawposition->set_memorized();
	rawposition->set_memorized_init(false);
	rawposition->set_change_event(true, false);
	att_list.push_back(rawposition);

	//	Attribute : Position
	PositionAttrib	*position = new PositionAttrib();
	Tango::UserDefaultAttrProp	position_prop;
	//	description	not set for Position
	position_prop.set_label("Position");
	position_prop.set_unit("mm");
	position_prop.set_standard_unit("mm");
	position_prop.set_display_unit("mm");
	position_prop.set_format("%.3f");
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

	//	Attribute : Time
	TimeAttrib	*time = new TimeAttrib();
	Tango::UserDefaultAttrProp	time_prop;
	time_prop.set_description("Time in femtoseconds (for 2 way pass)");
	time_prop.set_label("Time");
	time_prop.set_unit("fs");
	time_prop.set_standard_unit("fs");
	time_prop.set_display_unit("fs");
	time_prop.set_format("%.1f");
	//	max_value	not set for Time
	//	min_value	not set for Time
	//	max_alarm	not set for Time
	//	min_alarm	not set for Time
	//	max_warning	not set for Time
	//	min_warning	not set for Time
	//	delta_t	not set for Time
	//	delta_val	not set for Time
	
	time->set_default_properties(time_prop);
	//	Not Polled
	time->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	time->set_change_event(true, false);
	att_list.push_back(time);

	//	Attribute : HomePosition
	HomePositionAttrib	*homeposition = new HomePositionAttrib();
	Tango::UserDefaultAttrProp	homeposition_prop;
	//	description	not set for HomePosition
	homeposition_prop.set_label("Home position");
	homeposition_prop.set_unit("mm");
	homeposition_prop.set_standard_unit("mm");
	homeposition_prop.set_display_unit("mm");
	homeposition_prop.set_format("%.3f");
	//	max_value	not set for HomePosition
	//	min_value	not set for HomePosition
	//	max_alarm	not set for HomePosition
	//	min_alarm	not set for HomePosition
	//	max_warning	not set for HomePosition
	//	min_warning	not set for HomePosition
	//	delta_t	not set for HomePosition
	//	delta_val	not set for HomePosition
	
	homeposition->set_default_properties(homeposition_prop);
	//	Not Polled
	homeposition->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	homeposition->set_change_event(true, false);
	att_list.push_back(homeposition);

	//	Attribute : StageName
	StageNameAttrib	*stagename = new StageNameAttrib();
	Tango::UserDefaultAttrProp	stagename_prop;
	//	description	not set for StageName
	//	label	not set for StageName
	//	unit	not set for StageName
	//	standard_unit	not set for StageName
	//	display_unit	not set for StageName
	//	format	not set for StageName
	//	max_value	not set for StageName
	//	min_value	not set for StageName
	//	max_alarm	not set for StageName
	//	min_alarm	not set for StageName
	//	max_warning	not set for StageName
	//	min_warning	not set for StageName
	//	delta_t	not set for StageName
	//	delta_val	not set for StageName
	
	stagename->set_default_properties(stagename_prop);
	//	Not Polled
	stagename->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	att_list.push_back(stagename);

	//	Attribute : LastError
	LastErrorAttrib	*lasterror = new LastErrorAttrib();
	Tango::UserDefaultAttrProp	lasterror_prop;
	lasterror_prop.set_description("Store the last error got from the controller");
	lasterror_prop.set_label("Last error");
	//	unit	not set for LastError
	//	standard_unit	not set for LastError
	//	display_unit	not set for LastError
	//	format	not set for LastError
	//	max_value	not set for LastError
	//	min_value	not set for LastError
	//	max_alarm	not set for LastError
	//	min_alarm	not set for LastError
	//	max_warning	not set for LastError
	//	min_warning	not set for LastError
	//	delta_t	not set for LastError
	//	delta_val	not set for LastError
	
	lasterror->set_default_properties(lasterror_prop);
	//	Not Polled
	lasterror->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	lasterror->set_change_event(true, false);
	att_list.push_back(lasterror);

	//	Attribute : MotionDone
	MotionDoneAttrib	*motiondone = new MotionDoneAttrib();
	Tango::UserDefaultAttrProp	motiondone_prop;
	//	description	not set for MotionDone
	motiondone_prop.set_label("Motion Done");
	//	unit	not set for MotionDone
	//	standard_unit	not set for MotionDone
	//	display_unit	not set for MotionDone
	//	format	not set for MotionDone
	//	max_value	not set for MotionDone
	//	min_value	not set for MotionDone
	//	max_alarm	not set for MotionDone
	//	min_alarm	not set for MotionDone
	//	max_warning	not set for MotionDone
	//	min_warning	not set for MotionDone
	//	delta_t	not set for MotionDone
	//	delta_val	not set for MotionDone
	
	motiondone->set_default_properties(motiondone_prop);
	//	Not Polled
	motiondone->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	motiondone->set_change_event(true, false);
	att_list.push_back(motiondone);

	//	Attribute : Macro
	MacroAttrib	*macro = new MacroAttrib();
	Tango::UserDefaultAttrProp	macro_prop;
	//	description	not set for Macro
	macro_prop.set_label("Macro");
	//	unit	not set for Macro
	//	standard_unit	not set for Macro
	//	display_unit	not set for Macro
	//	format	not set for Macro
	//	max_value	not set for Macro
	//	min_value	not set for Macro
	//	max_alarm	not set for Macro
	//	min_alarm	not set for Macro
	//	max_warning	not set for Macro
	//	min_warning	not set for Macro
	//	delta_t	not set for Macro
	//	delta_val	not set for Macro
	
	macro->set_default_properties(macro_prop);
	//	Not Polled
	macro->set_disp_level(Tango::OPERATOR);
	macro->set_memorized();
	macro->set_memorized_init(true);
	att_list.push_back(macro);


	//	Create a list of static attributes
	create_static_attribute_list(get_class_attr()->get_attr_list());
	/*----- PROTECTED REGION ID(MercurySrvClass::attribute_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::attribute_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : MercurySrvClass::pipe_factory()
 *	Description : Create the pipe object(s)
 *                and store them in the pipe list
 */
//--------------------------------------------------------
void MercurySrvClass::pipe_factory()
{
	/*----- PROTECTED REGION ID(MercurySrvClass::pipe_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::pipe_factory_before
	/*----- PROTECTED REGION ID(MercurySrvClass::pipe_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::pipe_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : MercurySrvClass::command_factory()
 *	Description : Create the command object(s)
 *                and store them in the command list
 */
//--------------------------------------------------------
void MercurySrvClass::command_factory()
{
	/*----- PROTECTED REGION ID(MercurySrvClass::command_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::command_factory_before


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

	//	Command timeStep
	timeStepClass	*ptimeStepCmd =
		new timeStepClass("timeStep",
			Tango::DEV_DOUBLE, Tango::DEV_VOID,
			"Time step in fs",
			"",
			Tango::OPERATOR);
	command_list.push_back(ptimeStepCmd);

	//	Command posStep
	posStepClass	*pposStepCmd =
		new posStepClass("posStep",
			Tango::DEV_DOUBLE, Tango::DEV_VOID,
			"Step in phys unit",
			"",
			Tango::OPERATOR);
	command_list.push_back(pposStepCmd);

	//	Command Stop
	StopClass	*pStopCmd =
		new StopClass("Stop",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pStopCmd);

	//	Command Abort
	AbortClass	*pAbortCmd =
		new AbortClass("Abort",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pAbortCmd);

	//	Command Reset
	ResetClass	*pResetCmd =
		new ResetClass("Reset",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pResetCmd);

	//	Command StartMacro
	StartMacroClass	*pStartMacroCmd =
		new StartMacroClass("StartMacro",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pStartMacroCmd);

	//	Command StopMacro
	StopMacroClass	*pStopMacroCmd =
		new StopMacroClass("StopMacro",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pStopMacroCmd);

	/*----- PROTECTED REGION ID(MercurySrvClass::command_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::command_factory_after
}

//===================================================================
//	Dynamic attributes related methods
//===================================================================

//--------------------------------------------------------
/**
 * method : 		MercurySrvClass::create_static_attribute_list
 * description : 	Create the a list of static attributes
 *
 * @param	att_list	the ceated attribute list
 */
//--------------------------------------------------------
void MercurySrvClass::create_static_attribute_list(vector<Tango::Attr *> &att_list)
{
	for (unsigned long i=0 ; i<att_list.size() ; i++)
	{
		string att_name(att_list[i]->get_name());
		transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
		defaultAttList.push_back(att_name);
	}

	cout2 << defaultAttList.size() << " attributes in default list" << endl;

	/*----- PROTECTED REGION ID(MercurySrvClass::create_static_att_list) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::create_static_att_list
}


//--------------------------------------------------------
/**
 * method : 		MercurySrvClass::erase_dynamic_attributes
 * description : 	delete the dynamic attributes if any.
 *
 * @param	devlist_ptr	the device list pointer
 * @param	list of all attributes
 */
//--------------------------------------------------------
void MercurySrvClass::erase_dynamic_attributes(const Tango::DevVarStringArray *devlist_ptr, vector<Tango::Attr *> &att_list)
{
	Tango::Util *tg = Tango::Util::instance();

	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		Tango::DeviceImpl *dev_impl = tg->get_device_by_name(((string)(*devlist_ptr)[i]).c_str());
		MercurySrv *dev = static_cast<MercurySrv *> (dev_impl);

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
	/*----- PROTECTED REGION ID(MercurySrvClass::erase_dynamic_attributes) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::erase_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrvClass::get_attr_by_name()
 *	Description : returns Tango::Attr * object found by name
 */
//--------------------------------------------------------
Tango::Attr *MercurySrvClass::get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname)
{
	vector<Tango::Attr *>::iterator it;
	for (it=att_list.begin() ; it<att_list.end() ; ++it)
		if ((*it)->get_name()==attname)
			return (*it);
	//	Attr does not exist
	return NULL;
}


/*----- PROTECTED REGION ID(MercurySrvClass::Additional Methods) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	MercurySrvClass::Additional Methods
} //	namespace
