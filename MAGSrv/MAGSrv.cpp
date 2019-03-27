/*----- PROTECTED REGION ID(MAGSrv.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        MAGSrv.cpp
//
// description : C++ source for the MAGSrv class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               MAGSrv are implemented in this file.
//
// project :
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


#include <MAGSrv.h>
#include <MAGSrvClass.h>

/*----- PROTECTED REGION END -----*/	//	MAGSrv.cpp

/**
 *  MAGSrv class description:
 *    Device server for MagDrive frequency converters mounted on Leybold Turbopumps.
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name  |  Method name
//================================================================
//  State         |  Inherited (no method)
//  Status        |  Inherited (no method)
//  Start         |  start
//  Stop          |  stop
//  Reset         |  reset
//  getError      |  get_error
//================================================================

//================================================================
//  Attributes managed are:
//================================================================
//  Frequency             |  Tango::DevUShort	Scalar
//  MotorTemperature      |  Tango::DevUShort	Scalar
//  ConverterTemperature  |  Tango::DevUShort	Scalar
//  OperatingHours        |  Tango::DevFloat	Scalar
//  Current               |  Tango::DevFloat	Scalar
//  BearingTemperature    |  Tango::DevUShort	Scalar
//  Power                 |  Tango::DevFloat	Scalar
//  LastError             |  Tango::DevString	Scalar
//  SoftVersion           |  Tango::DevString	Scalar
//  SerialNo              |  Tango::DevString	Scalar
//  MagB_TD               |  Tango::DevUShort	Scalar
//  MagB_TD_Time          |  Tango::DevFloat	Scalar
//================================================================

namespace MAGSrv_ns
{
/*----- PROTECTED REGION ID(MAGSrv::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	MAGSrv::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : MAGSrv::MAGSrv()
 *	Description : Constructors for a Tango device
 *                implementing the classMAGSrv
 */
//--------------------------------------------------------
MAGSrv::MAGSrv(Tango::DeviceClass *cl, string &s)
 : TANGO_BASE_CLASS(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(MAGSrv::constructor_1) ENABLED START -----*/
	init_device();

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::constructor_1
}
//--------------------------------------------------------
MAGSrv::MAGSrv(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s)
{
	/*----- PROTECTED REGION ID(MAGSrv::constructor_2) ENABLED START -----*/
	init_device();

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::constructor_2
}
//--------------------------------------------------------
MAGSrv::MAGSrv(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d)
{
	/*----- PROTECTED REGION ID(MAGSrv::constructor_3) ENABLED START -----*/
	init_device();

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : MAGSrv::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void MAGSrv::delete_device()
{
	DEBUG_STREAM << "MAGSrv::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(MAGSrv::delete_device) ENABLED START -----*/

	//	Delete device allocated objects
	if(mt) {
		mt->terminate();
		int *retval;
		mt->join((void**)&retval);
		delete retval;
	}
	mt = NULL;

	if(*attr_SerialNo_read)
		CORBA::string_free(*attr_SerialNo_read);
	if(*attr_SoftVersion_read)
		CORBA::string_free(*attr_SoftVersion_read);
	if(*attr_LastError_read)
		CORBA::string_free(*attr_LastError_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::delete_device
	delete[] attr_Frequency_read;
	delete[] attr_MotorTemperature_read;
	delete[] attr_ConverterTemperature_read;
	delete[] attr_OperatingHours_read;
	delete[] attr_Current_read;
	delete[] attr_BearingTemperature_read;
	delete[] attr_Power_read;
	delete[] attr_LastError_read;
	delete[] attr_SoftVersion_read;
	delete[] attr_SerialNo_read;
	delete[] attr_MagB_TD_read;
	delete[] attr_MagB_TD_Time_read;
}

//--------------------------------------------------------
/**
 *	Method      : MAGSrv::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void MAGSrv::init_device()
{
	DEBUG_STREAM << "MAGSrv::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(MAGSrv::init_device_before) ENABLED START -----*/

	//	Initialization before get_device_property() call

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::init_device_before


	//	Get the device properties from database
	get_device_property();

	attr_Frequency_read = new Tango::DevUShort[1];
	attr_MotorTemperature_read = new Tango::DevUShort[1];
	attr_ConverterTemperature_read = new Tango::DevUShort[1];
	attr_OperatingHours_read = new Tango::DevFloat[1];
	attr_Current_read = new Tango::DevFloat[1];
	attr_BearingTemperature_read = new Tango::DevUShort[1];
	attr_Power_read = new Tango::DevFloat[1];
	attr_LastError_read = new Tango::DevString[1];
	attr_SoftVersion_read = new Tango::DevString[1];
	attr_SerialNo_read = new Tango::DevString[1];
	attr_MagB_TD_read = new Tango::DevUShort[1];
	attr_MagB_TD_Time_read = new Tango::DevFloat[1];
	//	No longer if mandatory property not set.
	if (mandatoryNotDefined)
		return;

	/*----- PROTECTED REGION ID(MAGSrv::init_device) ENABLED START -----*/

	*attr_SerialNo_read = CORBA::string_dup("");
	*attr_SoftVersion_read = CORBA::string_dup("");
	*attr_LastError_read = CORBA::string_dup("");

	//	Initialize device
	set_state(Tango::INIT);

	// Start monitoring thread
	mt = new MagSerial(this);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::init_device
}

//--------------------------------------------------------
/**
 *	Method      : MAGSrv::get_device_property()
 *	Description : Read database to initialize property data members.
 */
//--------------------------------------------------------
void MAGSrv::get_device_property()
{
	/*----- PROTECTED REGION ID(MAGSrv::get_device_property_before) ENABLED START -----*/

	//	Initialize property data members

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::get_device_property_before

	mandatoryNotDefined = false;

	//	Read device properties from database.
	Tango::DbData	dev_prop;
	dev_prop.push_back(Tango::DbDatum("proxy"));
	dev_prop.push_back(Tango::DbDatum("address"));
	dev_prop.push_back(Tango::DbDatum("polling"));

	//	is there at least one property to be read ?
	if (dev_prop.size()>0)
	{
		//	Call database and extract values
		if (Tango::Util::instance()->_UseDb==true)
			get_db_device()->get_property(dev_prop);

		//	get instance on MAGSrvClass to get class property
		Tango::DbDatum	def_prop, cl_prop;
		MAGSrvClass	*ds_class =
			(static_cast<MAGSrvClass *>(get_device_class()));
		int	i = -1;

		//	Try to initialize proxy from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  proxy;
		else {
			//	Try to initialize proxy from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  proxy;
		}
		//	And try to extract proxy value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  proxy;
		//	Property StartDsPath is mandatory, check if has been defined in database.
		check_mandatory_property(cl_prop, dev_prop[i]);

		//	Try to initialize address from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  address;
		else {
			//	Try to initialize address from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  address;
		}
		//	And try to extract address value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  address;

		//	Try to initialize polling from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  polling;
		else {
			//	Try to initialize polling from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  polling;
		}
		//	And try to extract polling value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  polling;

	}

	/*----- PROTECTED REGION ID(MAGSrv::get_device_property_after) ENABLED START -----*/

	//	Check device property data members init

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::get_device_property_after
}
//--------------------------------------------------------
/**
 *	Method      : MAGSrv::check_mandatory_property()
 *	Description : For mandatory properties check if defined in database.
 */
//--------------------------------------------------------
void MAGSrv::check_mandatory_property(Tango::DbDatum &class_prop, Tango::DbDatum &dev_prop)
{
	//	Check if all properties are empty
	if (class_prop.is_empty() && dev_prop.is_empty())
	{
		TangoSys_OMemStream	tms;
		tms << endl <<"Property \'" << dev_prop.name;
		if (Tango::Util::instance()->_UseDb==true)
			tms << "\' is mandatory but not defined in database";
		else
			tms << "\' is mandatory but cannot be defined without database";
		string	status(get_status());
		status += tms.str();
		set_status(status);
		mandatoryNotDefined = true;
		/*----- PROTECTED REGION ID(MAGSrv::check_mandatory_property) ENABLED START -----*/
		cerr << tms.str() << " for " << device_name << endl;

		/*----- PROTECTED REGION END -----*/	//	MAGSrv::check_mandatory_property
	}
}


//--------------------------------------------------------
/**
 *	Method      : MAGSrv::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void MAGSrv::always_executed_hook()
{
	//DEBUG_STREAM << "MAGSrv::always_executed_hook()  " << device_name << endl;
	if (mandatoryNotDefined)
	{
		string	status(get_status());
		Tango::Except::throw_exception(
					(const char *)"PROPERTY_NOT_SET",
					status.c_str(),
					(const char *)"MAGSrv::always_executed_hook()");
	}
	/*----- PROTECTED REGION ID(MAGSrv::always_executed_hook) ENABLED START -----*/

	//	code always executed before all requests

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : MAGSrv::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void MAGSrv::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	//DEBUG_STREAM << "MAGSrv::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_attr_hardware) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_attr_hardware
}

//--------------------------------------------------------
/**
 *	Read attribute Frequency related method
 *	Description: Actual output frequency of the converter
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_Frequency(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_Frequency(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_Frequency) ENABLED START -----*/

	*attr_Frequency_read = mt->getFrequency();

	//	Set the attribute value
	attr.set_value(attr_Frequency_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_Frequency
}
//--------------------------------------------------------
/**
 *	Read attribute MotorTemperature related method
 *	Description: Actual motor temperature
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_MotorTemperature(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_MotorTemperature(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_MotorTemperature) ENABLED START -----*/

	*attr_MotorTemperature_read = mt->getMotorT();

	//	Set the attribute value
	attr.set_value(attr_MotorTemperature_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_MotorTemperature
}
//--------------------------------------------------------
/**
 *	Read attribute ConverterTemperature related method
 *	Description: Actual converter temperature
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_ConverterTemperature(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_ConverterTemperature(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_ConverterTemperature) ENABLED START -----*/

	*attr_ConverterTemperature_read = mt->getConverterT();

	//	Set the attribute value
	attr.set_value(attr_ConverterTemperature_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_ConverterTemperature
}
//--------------------------------------------------------
/**
 *	Read attribute OperatingHours related method
 *	Description:
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_OperatingHours(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_OperatingHours(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_OperatingHours) ENABLED START -----*/

	*attr_OperatingHours_read = mt->getOperatingHours();

	//	Set the attribute value
	attr.set_value(attr_OperatingHours_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_OperatingHours
}
//--------------------------------------------------------
/**
 *	Read attribute Current related method
 *	Description: Actual current value of the intermediate circuit of the frequency converter
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_Current(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_Current(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_Current) ENABLED START -----*/

	*attr_Current_read = mt->getCurrent();

	//	Set the attribute value
	attr.set_value(attr_Current_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_Current
}
//--------------------------------------------------------
/**
 *	Read attribute BearingTemperature related method
 *	Description: Actual bearing temperature
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_BearingTemperature(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_BearingTemperature(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_BearingTemperature) ENABLED START -----*/

	*attr_BearingTemperature_read = mt->getBearingT();

	//	Set the attribute value
	attr.set_value(attr_BearingTemperature_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_BearingTemperature
}
//--------------------------------------------------------
/**
 *	Read attribute Power related method
 *	Description: Power consumption
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_Power(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_Power(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_Power) ENABLED START -----*/

	*attr_Power_read = mt->getPower();

	//	Set the attribute value
	attr.set_value(attr_Power_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_Power
}
//--------------------------------------------------------
/**
 *	Read attribute LastError related method
 *	Description:
 *
 *	Data type:	Tango::DevString
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_LastError(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_LastError(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_LastError) ENABLED START -----*/
	//	Set the attribute value
	attr.set_value(attr_LastError_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_LastError
}
//--------------------------------------------------------
/**
 *	Read attribute SoftVersion related method
 *	Description:
 *
 *	Data type:	Tango::DevString
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_SoftVersion(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_SoftVersion(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_SoftVersion) ENABLED START -----*/
	//	Set the attribute value
	attr.set_value(attr_SoftVersion_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_SoftVersion
}
//--------------------------------------------------------
/**
 *	Read attribute SerialNo related method
 *	Description:
 *
 *	Data type:	Tango::DevString
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_SerialNo(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_SerialNo(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_SerialNo) ENABLED START -----*/
	//	Set the attribute value
	attr.set_value(attr_SerialNo_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_SerialNo
}
//--------------------------------------------------------
/**
 *	Read attribute MagB_TD related method
 *	Description: Total number of recognized magnetic bearing touch downs
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_MagB_TD(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_MagB_TD(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_MagB_TD) ENABLED START -----*/

	*attr_MagB_TD_read = mt->getMB_TD();

	//	Set the attribute value
	attr.set_value(attr_MagB_TD_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_MagB_TD
}
//--------------------------------------------------------
/**
 *	Read attribute MagB_TD_Time related method
 *	Description: Total touch down time of magnetic bearings
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void MAGSrv::read_MagB_TD_Time(Tango::Attribute &attr)
{
	//DEBUG_STREAM << "MAGSrv::read_MagB_TD_Time(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(MAGSrv::read_MagB_TD_Time) ENABLED START -----*/

	*attr_MagB_TD_Time_read = mt->getMB_TD_Time();

	//	Set the attribute value
	attr.set_value(attr_MagB_TD_Time_read);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::read_MagB_TD_Time
}

//--------------------------------------------------------
/**
 *	Method      : MAGSrv::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void MAGSrv::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(MAGSrv::add_dynamic_attributes) ENABLED START -----*/

	//	Add your own code to create and add dynamic attributes if any

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command Start related method
 *	Description: Start the turbo pump
 *
 */
//--------------------------------------------------------
void MAGSrv::start()
{
	DEBUG_STREAM << "MAGSrv::Start()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(MAGSrv::start) ENABLED START -----*/

	mt->start();

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::start
}
//--------------------------------------------------------
/**
 *	Command Stop related method
 *	Description: Stop the turbo pump
 *
 */
//--------------------------------------------------------
void MAGSrv::stop()
{
	DEBUG_STREAM << "MAGSrv::Stop()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(MAGSrv::stop) ENABLED START -----*/

	mt->stop();

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::stop
}
//--------------------------------------------------------
/**
 *	Command Reset related method
 *	Description: Reset error on the turbo pump controller
 *
 */
//--------------------------------------------------------
void MAGSrv::reset()
{
	DEBUG_STREAM << "MAGSrv::Reset()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(MAGSrv::reset) ENABLED START -----*/

	mt->reset();

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::reset
}
//--------------------------------------------------------
/**
 *	Command getError related method
 *	Description: Get list of logged errors
 *
 *	@param argin Error index (min 0, max 39)
 *	@returns Error message
 */
//--------------------------------------------------------
Tango::DevString MAGSrv::get_error(Tango::DevUShort argin)
{
	Tango::DevString argout;
	DEBUG_STREAM << "MAGSrv::getError()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(MAGSrv::get_error) ENABLED START -----*/

	argout = mt->getError(argin);

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::get_error
	return argout;
}
//--------------------------------------------------------
/**
 *	Method      : MAGSrv::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void MAGSrv::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(MAGSrv::add_dynamic_commands) ENABLED START -----*/

	//	Add your own code to create and add dynamic commands if any

	/*----- PROTECTED REGION END -----*/	//	MAGSrv::add_dynamic_commands
}

/*----- PROTECTED REGION ID(MAGSrv::namespace_ending) ENABLED START -----*/

//	Additional Methods

// Monitor thread constructor
MagSerial::MagSerial(MAGSrv *parent) :
	_dev(NULL),
	_parent(parent),
	_terminate(false),
	_address(parent->address),
	_control_reg(0x0400),
	_status_reg(0),
	_conv_t(0),
	_motor_t(0),
	_bearing_t(0),
	_freq(0),
	_current(0.0),
	_power(0.0),
	_op_hours(0.0),
	_ev_ophours(NULL),
	_ev_current(NULL),
	_ev_freq(NULL),
	_ev_ct(NULL),
	_ev_mt(NULL),
	_ev_bt(NULL)
{
	// Create device proxy
	_dev = new Tango::DeviceProxy(_parent->proxy);

	// Allocate event variables
	CREATE_EVENT_MEMBER(_ev_ophours, float, 0.0);
	CREATE_EVENT_MEMBER(_ev_current, float, 0.0);
	CREATE_EVENT_MEMBER(_ev_power, float, 0.0);
	CREATE_EVENT_MEMBER(_ev_freq, uint16_t, 0);
	CREATE_EVENT_MEMBER(_ev_ct, uint16_t, 0);
	CREATE_EVENT_MEMBER(_ev_mt, uint16_t, 0);
	CREATE_EVENT_MEMBER(_ev_bt, uint16_t, 0);

	// Start monitor thread
	start_undetached();
}


// Monitor thread destructor
MagSerial::~MagSerial() {
	if(_dev)
		delete _dev;
	_dev = NULL;
	DELETE_EVENT_MEMBER(_ev_ophours);
	DELETE_EVENT_MEMBER(_ev_current);
	DELETE_EVENT_MEMBER(_ev_power);
	DELETE_EVENT_MEMBER(_ev_freq);
	DELETE_EVENT_MEMBER(_ev_ct);
	DELETE_EVENT_MEMBER(_ev_mt);
	DELETE_EVENT_MEMBER(_ev_bt);
}


uint16_t MagSerial::getIndexedValue(uint16_t param, uint16_t index) {
	// Message buffer
	Tango::DevVarCharArray *argin = new Tango::DevVarCharArray(MAG_USS_BUFF_LEN);
	argin->length(MAG_USS_BUFF_LEN);
	char *request = (char*)argin->get_buffer();
	memset(request, 0, MAG_USS_BUFF_LEN);
	request[0] = _address;

	// Format request
	*((uint32_t*)(request+5)) = 0;
	*((uint16_t*)(request+9)) = bswap_16(_control_reg);
	*((uint16_t*)(request+1)) = bswap_16(param | 0x6000);
	*((uint16_t*)(request+4)) = bswap_16(index);

	Tango::DeviceData din, dout;
	din << argin;
	{
		// Lock communication proxy
		omni_mutex_lock sync(this->_lock);
		// Send message
		dout = _dev->command_inout("SendTelegram", din);
	}
	const Tango::DevVarCharArray *argout;
	if(dout >> argout) {
		return bswap_16(*((uint16_t*)(argout->get_buffer()+7)));

	} else {
		Tango::Except::throw_exception(
		(const char*)"Conversion failed",
		(const char*)"Failed to convert response",
		(const char*)"MagSerial::getIndexedValue()");
	}
}


// Get error list from controller
Tango::DevString MagSerial::getError(uint16_t num) {
	if(num > 39)
		num = 39;

	uint16_t error, rpms, hours;

	size_t count = 0;
	while(1) {
		try {
			error = getIndexedValue(171, num);
			break;
		} catch(Tango::DevFailed &e) {
			count++;
			if(count > MAG_MAX_RETRY) {
				Tango::Except::throw_exception(
					(const char*)"Communication failed",
					(const char*)"Failed to retreive error code",
					(const char*)"MagSerial::getError()");
			}
		}
	}

	count = 0;
	while(1) {
		try {
			rpms = getIndexedValue(174, num);
			break;
		} catch(Tango::DevFailed &e) {
			count++;
			if(count > MAG_MAX_RETRY) {
				Tango::Except::throw_exception(
					(const char*)"Communication failed",
					(const char*)"Failed to retreive error rpms",
					(const char*)"MagSerial::getError()");
			}
		}
	}

	count = 0;
	while(1) {
		try {
			hours = getIndexedValue(176, num);
			break;
		} catch(Tango::DevFailed &e) {
			count++;
			if(count > MAG_MAX_RETRY) {
				Tango::Except::throw_exception(
					(const char*)"Communication failed",
					(const char*)"Failed to retreive error hours",
					(const char*)"MagSerial::getError()");
			}
		}
	}
	char buffer[256];
	buffer[255] = '\0';
	snprintf(buffer, 255, "Error %hu occurred at %.2f hours (pump at %hu rpm)", error, double(hours) / 100.0, rpms);
	return CORBA::string_dup(buffer);
}


void MagSerial::msleep(uint32_t msec) {
	if(msec <= 0)
		return;
	struct timespec st;
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	// Setup delay
	st.tv_sec = msec / 1000;
	st.tv_nsec = (msec % 1000) * 1000000;

	while(1) {
		int ans = nanosleep(&st, NULL);
		if(ans == -1 && errno == EINTR) {
			// Nanosleep was interrupted by a signal
			gettimeofday(&stop, NULL);
			uint32_t rem = msec - ELAPSED_TIME_MS(start, stop);
			if(rem > 0) {
				st.tv_sec = rem / 1000;
				st.tv_nsec = (rem % 1000) * 1000000;
			} else {
				break;
			}
		} else {
			break;
		}
	}
}


// Monitor thread main loop
void *MagSerial::run_undetached(void *arg) {

	struct timeval beg, end;

	const uint16_t parameters[] = {2, 52, 7, 44, 125, 105, 106};
	size_t param_counter = 0;

	// Telegram buffer
	char buffer[MAG_USS_BUFF_LEN];
	memset(buffer, 0, MAG_USS_BUFF_LEN);
	buffer[0] = _address;

	int comm_err_count = 0;

	if(_dev && _parent) {
		while(!_terminate) {
			// Starting time
			gettimeofday(&beg, NULL);

			// Clear buffer
			memset(buffer+1, 0, MAG_USS_BUFF_LEN-1);

			// Format telegram
			*((uint16_t*)(buffer+1)) = bswap_16(parameters[param_counter] | (0x1 << 12));
			*((uint32_t*)(buffer+5)) = 0;
			*((uint16_t*)(buffer+9)) = bswap_16(_control_reg);

			// Remove reset bit once sent
			if(_control_reg & 0x80) {
				_control_reg = _control_reg & ~0x80;
			}

			try {
				// Allocate buffer
				Tango::DevVarCharArray *telegram = new Tango::DevVarCharArray(MAG_USS_BUFF_LEN);
				telegram->length(MAG_USS_BUFF_LEN);
				// Copy telegram
				memcpy(telegram->get_buffer(), buffer, MAG_USS_BUFF_LEN);

				// Store buffer in DeviceData
				Tango::DeviceData din;
				din << telegram;

				// Send command
				Tango::DeviceData dout;
				{
					// Lock mutex
					omni_mutex_lock sync(this->_lock);
					dout = _dev->command_inout("SendTelegram", din);
				}
				comm_err_count = 0;

				// Extract answer
				const Tango::DevVarCharArray* response;
				if(dout >> response) {
					// Extract parameter number
					uint16_t num = bswap_16(*((uint16_t*)(response->get_buffer()+1))) & 0x07FF;

					if(num == 7) { // Motor temperature
						uint16_t val = uint16_t(bswap_32(*((uint32_t*)(response->get_buffer()+5))));
						if(val != _motor_t) {
							*_ev_mt = val;
							_parent->push_change_event("MotorTemperature", _ev_mt);
						}
						_motor_t = val;

					} else if(num == 2) { // Software version
						uint32_t val = bswap_32(*((uint32_t*)(response->get_buffer()+5)));
						stringstream ver;
						ver << (val/10000) << "." << ((val / 100) % 100) << "." << (val % 100);
						CORBA::string_free(*(_parent->attr_SoftVersion_read));
						*(_parent->attr_SoftVersion_read) = CORBA::string_dup(ver.str().c_str());

					} else if(num == 52) {
						uint32_t val = bswap_32(*((uint32_t*)(response->get_buffer()+5)));
						stringstream ver;
						ver << val;
						CORBA::string_free(*(_parent->attr_SerialNo_read));
						*(_parent->attr_SerialNo_read) = CORBA::string_dup(ver.str().c_str());

					} else if(num == 105) { // Number of touch downs
						uint16_t val = uint16_t(bswap_32(*((uint32_t*)(response->get_buffer()+5))));
						_mb_td = val;

					} else if(num == 106) { // Total touch down time
						float val = float(bswap_32(*((uint32_t*)(response->get_buffer()+5)))) / 100.0;
						_mb_td_time = val;

					} else if(num == 44) {
						float val = float(bswap_32(*((uint32_t*)(response->get_buffer()+5)))) / 100.0;
						if(::fabs((val - _op_hours) / _op_hours) > 0.01) {
							*_ev_ophours = val;
							_parent->push_change_event("OperatingHours", _ev_ophours);
						}
						_op_hours = val;

					} else if(num == 125) {
						uint16_t val = uint16_t(bswap_32(*((uint32_t*)(response->get_buffer()+5))));
						if(val != _bearing_t) {
							*_ev_bt = val;
							_parent->push_change_event("BearingTemperature", _ev_bt);
						}
						_bearing_t = val;
					}

					// Extract status
					_status_reg = bswap_16(*((uint16_t*)(response->get_buffer()+9)));

					if(_parent->get_logger()->is_debug_enabled())
						_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Pump status register: 0x" << std::hex << _status_reg << std::dec << endl;

					// Extract common parameters
					{
						uint16_t val = bswap_16(*((uint16_t*)(response->get_buffer()+11)));
						if(_parent->get_logger()->is_debug_enabled())
							_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Pump frequency: " << val << endl;
						if(val != _freq) {
							*_ev_freq = val;
							_parent->push_change_event("Frequency", _ev_freq);
						}
						_freq = val;
					}
					{
						uint16_t val = bswap_16(*((uint16_t*)(response->get_buffer()+13)));
						if(_parent->get_logger()->is_debug_enabled())
							_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Pump converter temperature: " << val << endl;
						if(val != _conv_t) {
							*_ev_ct = val;
							_parent->push_change_event("ConverterTemperature", _ev_ct);
						}
						_conv_t = val;
					}
					{
						float val = float(bswap_16(*((uint16_t*)(response->get_buffer()+15)))) / 10.0;
						if(_parent->get_logger()->is_debug_enabled())
							_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Pump current: " << val << endl;
						if(::fabs((_current - val) / _current) > 0.01) {
							*_ev_current = val;
							_parent->push_change_event("Current", _ev_current);
						}
						_current = val;
					}
					{
						float val = float(bswap_16(*((uint16_t*)(response->get_buffer()+19)))) / 10.0;
						if(_parent->get_logger()->is_debug_enabled())
							_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Pump voltage: " << val << endl;
						if(::fabs((_power - _current * val) / _power) > 0.01) {
							*_ev_power = _current * val;
							_parent->push_change_event("Power", _ev_power);
						}
						_power = _current * val;
						if(_parent->get_logger()->is_debug_enabled())
							_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Pump power: " << _current * val << endl;
					}

					if((_status_reg & MAG_STATUS_READY) && (_status_reg & MAG_STATUS_PARAM) && (_status_reg & MAG_STATUS_PROCESS)) {
						// Drive is operational
						if(_status_reg & MAG_STATUS_WTEMP) {
							_parent->set_state(Tango::ALARM);
							if(_status_reg & MAG_STATUS_ROTATING)
								_parent->set_status("Pump overtemperature. Pump is running.");
							else
								_parent->set_status("Pump overtemperature. Pump is off.");
						} else if(_status_reg & MAG_STATUS_WFAIL) {
							_parent->set_state(Tango::ALARM);
							if(_status_reg & MAG_STATUS_ROTATING)
								_parent->set_status("Pump failure counter triggered. Pump is running.");
							else
								_parent->set_status("Pump failure counter triggered. Pump is off.");
						} else if(_status_reg & MAG_STATUS_WOVERLOAD) {
							_parent->set_state(Tango::ALARM);
							if(_status_reg & MAG_STATUS_ROTATING)
								_parent->set_status("Pump overload. Pump is running.");
							else
								_parent->set_status("Pump overload. Pump is off.");
						} else {
							// Pump is working without faults
							if(_status_reg & MAG_STATUS_ENABLED) {
								// Drive active
								if(_status_reg & MAG_STATUS_ROTATING) {
									// Pump is rotating
									if(_status_reg & MAG_STATUS_NORMAL) {
										// Normal operation
										_parent->set_state(Tango::RUNNING);
										_parent->set_status("Normal operation");
									} else if(_status_reg & MAG_STATUS_ACCEL) {
										// Pump is accelerating
										_parent->set_state(Tango::MOVING);
										_parent->set_status("Pump is accelerating");
									} else if(_status_reg & MAG_STATUS_DECEL) {
										// Pump is accelerating
										_parent->set_state(Tango::MOVING);
										_parent->set_status("Pump is actively decelerating");
									} else {
										// Strange situation
										_parent->set_state(Tango::MOVING);
										_parent->set_status("Pump drive is on and the pump is rotating, but the status in unexpected");
										_parent->get_logger()->warn_stream() << log4tango::LogInitiator::_begin_log << "Pump drive is on and the pump is rotating, but the status is unexpected (Status register: 0x" << std::hex << _status_reg << std::dec << ")" << endl;
									}
								} else {
									// Starting
									_parent->set_state(Tango::MOVING);
									_parent->set_status("Pump is starting");
								}
							} else {
								// Drive not active
								if(_status_reg & MAG_STATUS_ROTATING) {
									// Pump is rotating
									if(_status_reg & MAG_STATUS_DECEL) {
										// Pump decelerating
										_parent->set_state(Tango::MOVING);
										_parent->set_status("Pump is decelerating");
									} else {
										// Strange situation
										_parent->set_state(Tango::MOVING);
										_parent->set_status("Drive is off but pump is not decelerating");
										_parent->get_logger()->warn_stream() << log4tango::LogInitiator::_begin_log << "Pump drive is off but it's not decelerating (Status register: 0x" << std::hex << _status_reg << std::dec << ")" << endl;
									}
								} else {
									// Pump off
									_parent->set_state(Tango::STANDBY);
									_parent->set_status("Pump off");
								}
							}
						}
					} else {
						// Pump not ready. May be a failure.
						if(_status_reg & MAG_STATUS_WTEMP) {
							_parent->set_state(Tango::FAULT);
							_parent->set_status("Pump not ready because of overtemperature error.");
						} else if(_status_reg & MAG_STATUS_WFAIL) {
							_parent->set_state(Tango::FAULT);
							_parent->set_status("Pump not ready because failure counter was triggered.");
						} else if(_status_reg & MAG_STATUS_WOVERLOAD) {
							_parent->set_state(Tango::FAULT);
							_parent->set_status("Pump not ready because of overload.");
						} else {
							_parent->set_state(Tango::FAULT);
							_parent->set_status("Pump not ready.");
						}
					}

				} else {
					// Extraction failed...
					_parent->get_logger()->error_stream() << log4tango::LogInitiator::_begin_log << "Extraction of response failed" << endl;
					_parent->set_state(Tango::FAULT);
					_parent->set_status("Extraction of response failed");
				}

			} catch(Tango::DevFailed &e) {
				comm_err_count++;
				if(comm_err_count > MAG_MAX_RETRY) {
					error_log_exception(_parent, "Got exception in polling loop", e);
					_parent->set_state(Tango::FAULT);
					_parent->set_status("Communication failed");
					try {
						_dev->command_inout("Flush");
					} catch(Tango::DevFailed &e) {
						error_log_exception(_parent, "Failed to flush buffer in exception handler", e);
					}
				} else if(_parent->get_logger()->is_debug_enabled()) {
					debug_log_exception(_parent, "Got exception in polling loop", e);
				}
			}

			// Increment parameter counter
			param_counter++;
			if(param_counter >= sizeof(parameters)/sizeof(uint16_t))
				param_counter = 2; // The first two parameters need to be read only once...

			// Sleep for polling time minus time spent polling the pump...
			gettimeofday(&end, NULL);
			int elapsed = ELAPSED_TIME_MS(beg, end);
			if(elapsed < (int)_parent->polling)
				msleep(_parent->polling - elapsed);
		}
	}

	int *result = new int(0);
	return result;
}


/*----- PROTECTED REGION END -----*/	//	MAGSrv::namespace_ending
} //	namespace
