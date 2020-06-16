/*----- PROTECTED REGION ID(Picomotor.cpp) ENABLED START -----*/
// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//=============================================================================
//
// file :        Picomotor.cpp
//
// description : C++ source for the Picomotor class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               Picomotor are implemented in this file.
//
// project :     Picomotor single-axis
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


#include <Picomotor.h>
#include <PicomotorClass.h>

/*----- PROTECTED REGION END -----*/	//	Picomotor.cpp

/**
 *  Picomotor class description:
 *    Picomotor single axis device server
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name  |  Method name
//================================================================
//  State         |  Inherited (no method)
//  Status        |  Inherited (no method)
//  goHome        |  go_home
//  setHome       |  set_home
//  Stop          |  stop
//================================================================

//================================================================
//  Attributes managed are:
//================================================================
//  Velocity      |  Tango::DevUShort	Scalar
//  Acceleration  |  Tango::DevULong	Scalar
//  Position      |  Tango::DevLong	Scalar
//================================================================

namespace Picomotor_ns
{
/*----- PROTECTED REGION ID(Picomotor::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	Picomotor::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : Picomotor::Picomotor()
 *	Description : Constructors for a Tango device
 *                implementing the classPicomotor
 */
//--------------------------------------------------------
Picomotor::Picomotor(Tango::DeviceClass *cl, string &s)
 : TANGO_BASE_CLASS(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(Picomotor::constructor_1) ENABLED START -----*/
	init_device();

	/*----- PROTECTED REGION END -----*/	//	Picomotor::constructor_1
}
//--------------------------------------------------------
Picomotor::Picomotor(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s)
{
	/*----- PROTECTED REGION ID(Picomotor::constructor_2) ENABLED START -----*/
	init_device();

	/*----- PROTECTED REGION END -----*/	//	Picomotor::constructor_2
}
//--------------------------------------------------------
Picomotor::Picomotor(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d)
{
	/*----- PROTECTED REGION ID(Picomotor::constructor_3) ENABLED START -----*/
	init_device();

	/*----- PROTECTED REGION END -----*/	//	Picomotor::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : Picomotor::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void Picomotor::delete_device()
{
	DEBUG_STREAM << "Picomotor::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(Picomotor::delete_device) ENABLED START -----*/

	//	Delete device allocated objects
	if(dev) {
		dev->terminate();
		int *retval;
		dev->join((void**)&retval);
		delete retval;
		dev = NULL;
	}

	/*----- PROTECTED REGION END -----*/	//	Picomotor::delete_device
	delete[] attr_Velocity_read;
	delete[] attr_Acceleration_read;
	delete[] attr_Position_read;
}

//--------------------------------------------------------
/**
 *	Method      : Picomotor::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void Picomotor::init_device()
{
	DEBUG_STREAM << "Picomotor::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(Picomotor::init_device_before) ENABLED START -----*/

	//	Initialization before get_device_property() call

	/*----- PROTECTED REGION END -----*/	//	Picomotor::init_device_before


	//	Get the device properties from database
	get_device_property();

	attr_Velocity_read = new Tango::DevUShort[1];
	attr_Acceleration_read = new Tango::DevULong[1];
	attr_Position_read = new Tango::DevLong[1];
	//	No longer if mandatory property not set.
	if (mandatoryNotDefined)
		return;

	/*----- PROTECTED REGION ID(Picomotor::init_device) ENABLED START -----*/

	//	Initialize device
	try{
		dev = new PicomotorDev(device.c_str(), axis, this);
		set_state(Tango::STANDBY);

	} catch(Tango::DevFailed &e) {
		TangoSys_OMemStream message;
		message << "failed to create a proxy to '" << device << "'";
		ERROR_STREAM << "Picomotor::init_device()  " << message.str() << endl;
		Tango::Except::re_throw_exception (e, "Picomotor", message.str(), "Picomotor::init_device()");
	}

	/*----- PROTECTED REGION END -----*/	//	Picomotor::init_device
}

//--------------------------------------------------------
/**
 *	Method      : Picomotor::get_device_property()
 *	Description : Read database to initialize property data members.
 */
//--------------------------------------------------------
void Picomotor::get_device_property()
{
	/*----- PROTECTED REGION ID(Picomotor::get_device_property_before) ENABLED START -----*/

	//	Initialize property data members

	/*----- PROTECTED REGION END -----*/	//	Picomotor::get_device_property_before

	mandatoryNotDefined = false;

	//	Read device properties from database.
	Tango::DbData	dev_prop;
	dev_prop.push_back(Tango::DbDatum("device"));
	dev_prop.push_back(Tango::DbDatum("axis"));
	dev_prop.push_back(Tango::DbDatum("polling"));

	//	is there at least one property to be read ?
	if (dev_prop.size()>0)
	{
		//	Call database and extract values
		if (Tango::Util::instance()->_UseDb==true)
			get_db_device()->get_property(dev_prop);

		//	get instance on PicomotorClass to get class property
		Tango::DbDatum	def_prop, cl_prop;
		PicomotorClass	*ds_class =
			(static_cast<PicomotorClass *>(get_device_class()));
		int	i = -1;

		//	Try to initialize device from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  device;
		else {
			//	Try to initialize device from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  device;
		}
		//	And try to extract device value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  device;
		//	Property StartDsPath is mandatory, check if has been defined in database.
		check_mandatory_property(cl_prop, dev_prop[i]);

		//	Try to initialize axis from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  axis;
		else {
			//	Try to initialize axis from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  axis;
		}
		//	And try to extract axis value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  axis;
		//	Property StartDsPath is mandatory, check if has been defined in database.
		check_mandatory_property(cl_prop, dev_prop[i]);

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

	/*----- PROTECTED REGION ID(Picomotor::get_device_property_after) ENABLED START -----*/

	//	Check device property data members init

	/*----- PROTECTED REGION END -----*/	//	Picomotor::get_device_property_after
}
//--------------------------------------------------------
/**
 *	Method      : Picomotor::check_mandatory_property()
 *	Description : For mandatory properties check if defined in database.
 */
//--------------------------------------------------------
void Picomotor::check_mandatory_property(Tango::DbDatum &class_prop, Tango::DbDatum &dev_prop)
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
		/*----- PROTECTED REGION ID(Picomotor::check_mandatory_property) ENABLED START -----*/
		cerr << tms.str() << " for " << device_name << endl;

		/*----- PROTECTED REGION END -----*/	//	Picomotor::check_mandatory_property
	}
}


//--------------------------------------------------------
/**
 *	Method      : Picomotor::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void Picomotor::always_executed_hook()
{
	DEBUG_STREAM << "Picomotor::always_executed_hook()  " << device_name << endl;
	if (mandatoryNotDefined)
	{
		string	status(get_status());
		Tango::Except::throw_exception(
					(const char *)"PROPERTY_NOT_SET",
					status.c_str(),
					(const char *)"Picomotor::always_executed_hook()");
	}
	/*----- PROTECTED REGION ID(Picomotor::always_executed_hook) ENABLED START -----*/

	//	code always executed before all requests

	/*----- PROTECTED REGION END -----*/	//	Picomotor::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : Picomotor::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void Picomotor::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "Picomotor::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(Picomotor::read_attr_hardware) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	Picomotor::read_attr_hardware
}
//--------------------------------------------------------
/**
 *	Method      : Picomotor::write_attr_hardware()
 *	Description : Hardware writing for attributes
 */
//--------------------------------------------------------
void Picomotor::write_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "Picomotor::write_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(Picomotor::write_attr_hardware) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	Picomotor::write_attr_hardware
}

//--------------------------------------------------------
/**
 *	Read attribute Velocity related method
 *	Description: Motor velocity
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void Picomotor::read_Velocity(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Picomotor::read_Velocity(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(Picomotor::read_Velocity) ENABLED START -----*/
	//	Set the attribute value
	*attr_Velocity_read = dev->getVelocity();
	attr.set_value(attr_Velocity_read);

	/*----- PROTECTED REGION END -----*/	//	Picomotor::read_Velocity
}
//--------------------------------------------------------
/**
 *	Write attribute Velocity related method
 *	Description: Motor velocity
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void Picomotor::write_Velocity(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "Picomotor::write_Velocity(Tango::WAttribute &attr) entering... " << endl;
	//	Retrieve write value
	Tango::DevUShort	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(Picomotor::write_Velocity) ENABLED START -----*/

	try {
		dev->setVelocity(w_val);
		*attr_Velocity_read = w_val;
		push_change_event("Velocity", attr_Velocity_read);

	} catch (Tango::DevFailed &e) {
		Tango::Except::re_throw_exception(e, "Picomotor", "Failed to write velocity", "Picomotor::write_Velocity()");
	}

	/*----- PROTECTED REGION END -----*/	//	Picomotor::write_Velocity
}
//--------------------------------------------------------
/**
 *	Read attribute Acceleration related method
 *	Description: Motor acceleration
 *
 *	Data type:	Tango::DevULong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void Picomotor::read_Acceleration(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Picomotor::read_Acceleration(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(Picomotor::read_Acceleration) ENABLED START -----*/
	//	Set the attribute value
	*attr_Acceleration_read = dev->getAcceleration();
	attr.set_value(attr_Acceleration_read);

	/*----- PROTECTED REGION END -----*/	//	Picomotor::read_Acceleration
}
//--------------------------------------------------------
/**
 *	Write attribute Acceleration related method
 *	Description: Motor acceleration
 *
 *	Data type:	Tango::DevULong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void Picomotor::write_Acceleration(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "Picomotor::write_Acceleration(Tango::WAttribute &attr) entering... " << endl;
	//	Retrieve write value
	Tango::DevULong	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(Picomotor::write_Acceleration) ENABLED START -----*/

	try {
		dev->setAcceleration(w_val);
		*attr_Acceleration_read = w_val;
		push_change_event("Acceleration", attr_Acceleration_read);

	} catch (Tango::DevFailed &e) {
		Tango::Except::re_throw_exception(e, "Picomotor", "Failed to write acceleration", "Picomotor::write_Acceleration()");
	}

	/*----- PROTECTED REGION END -----*/	//	Picomotor::write_Acceleration
}
//--------------------------------------------------------
/**
 *	Read attribute Position related method
 *	Description: Motor position
 *
 *	Data type:	Tango::DevLong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void Picomotor::read_Position(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Picomotor::read_Position(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(Picomotor::read_Position) ENABLED START -----*/

	try {
		*attr_Position_read = dev->getPosition();

	} catch (Tango::DevFailed &e) {
		Tango::Except::re_throw_exception(e, "Picomotor", "Failed to read position", "Picomotor::read_Position()");
	}

	//	Set the attribute value
	attr.set_value(attr_Position_read);

	/*----- PROTECTED REGION END -----*/	//	Picomotor::read_Position
}
//--------------------------------------------------------
/**
 *	Write attribute Position related method
 *	Description: Motor position
 *
 *	Data type:	Tango::DevLong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void Picomotor::write_Position(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "Picomotor::write_Position(Tango::WAttribute &attr) entering... " << endl;
	//	Retrieve write value
	Tango::DevLong	w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(Picomotor::write_Position) ENABLED START -----*/

	try {
		dev->moveTo(w_val);
	} catch (Tango::DevFailed &e) {
		Tango::Except::re_throw_exception(e, "Picomotor", "Failed to move", "Picomotor::write_Position()");
	}

	/*----- PROTECTED REGION END -----*/	//	Picomotor::write_Position
}

//--------------------------------------------------------
/**
 *	Method      : Picomotor::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void Picomotor::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(Picomotor::add_dynamic_attributes) ENABLED START -----*/

	//	Add your own code to create and add dynamic attributes if any

	/*----- PROTECTED REGION END -----*/	//	Picomotor::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command goHome related method
 *	Description: Set the home (zero position)
 *
 */
//--------------------------------------------------------
void Picomotor::go_home()
{
	DEBUG_STREAM << "Picomotor::goHome()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(Picomotor::go_home) ENABLED START -----*/

	try {
		dev->moveTo(0);
	} catch (Tango::DevFailed &e) {
		Tango::Except::re_throw_exception(e, "Picomotor", "Failed to go home", "Picomotor::goHome()");
	}

	/*----- PROTECTED REGION END -----*/	//	Picomotor::go_home
}
//--------------------------------------------------------
/**
 *	Command setHome related method
 *	Description: Set the home (zero position)
 *
 */
//--------------------------------------------------------
void Picomotor::set_home()
{
	DEBUG_STREAM << "Picomotor::setHome()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(Picomotor::set_home) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	Picomotor::set_home
}
//--------------------------------------------------------
/**
 *	Command Stop related method
 *	Description: Stop any running movement
 *
 */
//--------------------------------------------------------
void Picomotor::stop()
{
	DEBUG_STREAM << "Picomotor::Stop()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(Picomotor::stop) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	Picomotor::stop
}
//--------------------------------------------------------
/**
 *	Method      : Picomotor::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void Picomotor::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(Picomotor::add_dynamic_commands) ENABLED START -----*/

	//	Add your own code to create and add dynamic commands if any

	/*----- PROTECTED REGION END -----*/	//	Picomotor::add_dynamic_commands
}

/*----- PROTECTED REGION ID(Picomotor::namespace_ending) ENABLED START -----*/

//	Additional Methods

//	Additional Methods
PicomotorDev::PicomotorDev(const char* device, int axis, Picomotor* parent) :
	_terminate(false),
	_device(NULL),
	_parent(parent)
{
	// Extract address and axis
	axis = axis - 1;
	_address = (axis / 4) + 1;
	_axis = (axis % 4) + 1;

	if(_parent->get_logger()->is_debug_enabled()) {
		_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Address: " << _address << ", Axis: " << _axis << endl;
	}

	// Connect to communication proxy
	_device = new Tango::DeviceProxy(device);

	// Initializate velocity and acceleration values
	std::string response = SendCommandWithResponse("VA?");
	TangoSys_MemStream(response) >> _velocity;
	response = SendCommandWithResponse("AC?");
	TangoSys_MemStream(response) >> _acceleration;
	_position = readPos();

	// Start thread
	start_undetached();
}


// Destructor
PicomotorDev::~PicomotorDev() {
	if(_device)
		delete _device;
}


std::string PicomotorDev::SendCommandWithResponse(const char* command) {
	std::string response = "";
	if(_device) {
		Tango::DeviceData din;
		Tango::DeviceData dout;

		// Build input parameter
		Tango::DevString cmd = new char[32];
		snprintf(cmd, 32, "%d>%d%s", _address, _axis, command);
		cmd[31] = 0x00;
		din << cmd;

		if(_parent->get_logger()->is_debug_enabled()) {
			_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Sending command: " << cmd;
		}

		// Send command
		{
			omni_mutex_lock access_lock(this->_lock);
			dout = _device->command_inout("SendCommandWithResponse", din);
		}
		// Extrac response
		dout >> response;

		if(_parent->get_logger()->is_debug_enabled()) {
			_parent->get_logger()->debug_stream() << ". Answer: " << response << endl;
		}
	}
	std::size_t found = response.find(">");
	if(found!=std::string::npos) {
		return response.substr(found+1);
	} else {
		return response;
	}
}


void PicomotorDev::SendCommand(const char* command) {
	if(_device) {
		Tango::DeviceData din;
		// Build input parameter
		Tango::DevString cmd = new char[32];
		snprintf(cmd, 32, "%d>%d%s", _address, _axis, command);
		cmd[31] = 0x00;
		din << cmd;

		if(_parent->get_logger()->is_debug_enabled()) {
			_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Sending command: " << cmd << endl;
		}

		// Send command
		{
			omni_mutex_lock access_lock(this->_lock);
			_device->command_inout("SendCommand", din);
		}
	}
}

// Set velocity
void PicomotorDev::setVelocity(uint16_t vel) {
	// Format command
	TangoSys_OMemStream command;
	command << "VA" << vel;
	// Send commmand
	SendCommand(command.str().c_str());
	_velocity = vel;
}

// Set velocity
void PicomotorDev::setAcceleration(uint32_t acc) {
	// Format command
	TangoSys_OMemStream command;
	command << "AC" << acc;
	// Send commmand
	SendCommand(command.str().c_str());
	_acceleration = acc;
}

// Move to position
void PicomotorDev::moveTo(int32_t pos) {
	// Format command
	TangoSys_OMemStream command;
	command << "PA" << pos;
	// Send commmand
	SendCommand(command.str().c_str());
}

// Set home
void PicomotorDev::setHome() {
	SendCommand("DH");
}

// Read state from device
int32_t PicomotorDev::readState() {
	// Get state
	std::string ans = SendCommandWithResponse("MD?");
	int32_t state = 0;
	TangoSys_MemStream response(ans);
	response >> state;
	return state;
}

// Read position from device
int32_t PicomotorDev::readPos() {
	// Get state
	std::string ans = SendCommandWithResponse("TP?");
	int32_t pos = 0;
	TangoSys_MemStream response(ans);
	response >> pos;
	return pos;
}

// Main loop
void* PicomotorDev::run_undetached(void *arg) {

	// Starting state
	if(_parent) {
		_parent->set_state(Tango::STANDBY);
		_parent->get_logger()->info_stream() << log4tango::LogInitiator::_begin_log << " Started monitoring thread." << endl;
	}

	struct timeval b, e;
	while(!_terminate) {

		gettimeofday(&b, NULL);

		// If parent device is set check the device status
		if(_parent && _device) {
			try {
				int st = readState();
				if(_parent->get_logger()->is_debug_enabled()) {
					_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Got device state: " << st << endl;
				}

				// Update device state
				if(st == 0)
					_parent->set_state(Tango::MOVING);
				else
					_parent->set_state(Tango::STANDBY);

			} catch(Tango::DevFailed &e) {
				_parent->set_state(Tango::FAULT);
				TangoSys_OMemStream msg;
				msg << "Error getting motion status: " << e.errors[0].desc;
				_parent->set_status(msg.str());
			}

			try {
				int32_t pos = readPos();
				if(_parent->get_logger()->is_debug_enabled()) {
					_parent->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << "Got device position: " << pos << endl;
				}
				if(pos != _position) {
					_position = pos;
					*(_parent->attr_Position_read) = _position;
					_parent->push_change_event("Position", _parent->attr_Position_read);
				}

			} catch(Tango::DevFailed &e) {
				_parent->set_state(Tango::FAULT);
				TangoSys_OMemStream msg;
				msg << "Error getting position: " << e.errors[0].desc;
				_parent->set_status(msg.str());
			}
		}

		// Sleep
		gettimeofday(&e, NULL);

		int elapsed = ELAPSED_TIME_MS(b, e);

		if(elapsed < (int)_parent->polling)
			msleep(_parent->polling - elapsed);
	}

	int *retval = new int();
	*retval = 0;
	return (void*)retval;
}

void PicomotorDev::msleep(uint32_t msec) {
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

/*----- PROTECTED REGION END -----*/	//	Picomotor::namespace_ending
} //	namespace
