/*----- PROTECTED REGION ID(Polyscience.h) ENABLED START -----*/
// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//=============================================================================
//
// file :        Polyscience.h
//
// description : Include file for the Polyscience class
//
// project :     Polyscience series 6000
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


#ifndef Polyscience_H
#define Polyscience_H

#include <tango.h>

// Macro to measure elapsed time in ms
#ifndef ELAPSED_TIME_MS
#define ELAPSED_TIME_MS(p, l) ( ((l.tv_sec) - (p.tv_sec)) * 1000 + ((l.tv_usec) - (p.tv_usec)) / 1000 )
#endif

/*----- PROTECTED REGION END -----*/	//	Polyscience.h

/**
 *  Polyscience class description:
 *    Device server to control a Polyscience series 6000 chiller
 */

namespace Polyscience_ns
{
/*----- PROTECTED REGION ID(Polyscience::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations
class PolyMonitor;

/*----- PROTECTED REGION END -----*/	//	Polyscience::Additional Class Declarations

class Polyscience : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(Polyscience::Data Members) ENABLED START -----*/

//	Add your own data members

private:
	// Monitoring thread
	PolyMonitor* _dev;

/*----- PROTECTED REGION END -----*/	//	Polyscience::Data Members

//	Device property data members
public:
	//	proxy:	Communication proxy
	string	proxy;
	//	polling:	Polling period (ms)
	Tango::DevULong	polling;

	bool	mandatoryNotDefined;

//	Attribute data members
public:
	Tango::DevDouble	*attr_Temperature_read;
	Tango::DevDouble	*attr_Setpoint_read;
	Tango::DevDouble	*attr_Flow_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	Polyscience(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	Polyscience(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	Polyscience(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~Polyscience() {delete_device();};


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Read the device properties from database
	 */
	void get_device_property();
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();

	/*
	 *	Check if mandatory property has been set
	 */
	 void check_mandatory_property(Tango::DbDatum &class_prop, Tango::DbDatum &dev_prop);

//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : Polyscience::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);
	//--------------------------------------------------------
	/*
	 *	Method      : Polyscience::write_attr_hardware()
	 *	Description : Hardware writing for attributes.
	 */
	//--------------------------------------------------------
	virtual void write_attr_hardware(vector<long> &attr_list);

/**
 *	Attribute Temperature related methods
 *	Description: Actual temperature of the cooling circuit
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_Temperature(Tango::Attribute &attr);
	virtual bool is_Temperature_allowed(Tango::AttReqType type);
/**
 *	Attribute Setpoint related methods
 *	Description: Temperature setpoint
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_Setpoint(Tango::Attribute &attr);
	virtual void write_Setpoint(Tango::WAttribute &attr);
	virtual bool is_Setpoint_allowed(Tango::AttReqType type);
/**
 *	Attribute Flow related methods
 *	Description: Water flow in liter per minute
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_Flow(Tango::Attribute &attr);
	virtual bool is_Flow_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : Polyscience::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:
	/**
	 *	Command Start related method
	 *	Description:
	 *
	 */
	virtual void start();
	virtual bool is_Start_allowed(const CORBA::Any &any);
	/**
	 *	Command Stop related method
	 *	Description:
	 *
	 */
	virtual void stop();
	virtual bool is_Stop_allowed(const CORBA::Any &any);


	//--------------------------------------------------------
	/**
	 *	Method      : Polyscience::add_dynamic_commands()
	 *	Description : Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(Polyscience::Additional Method prototypes) ENABLED START -----*/

//	Additional Method prototypes

/*----- PROTECTED REGION END -----*/	//	Polyscience::Additional Method prototypes
};

/*----- PROTECTED REGION ID(Polyscience::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions
class PolyMonitor : public omni_thread {
public:
	// Constructor
	PolyMonitor(const char* device, Polyscience* parent);

	// Destructor
	~PolyMonitor();

	// Terminate
	void terminate() { _terminate = true; }

	// Setpoint
	void setSetpoint(double temperature);
	double getSetpoint()const { return _setpoint; };

	// Temperature
	double getTemperature()const { return _temperature; };

	// Flow
	double getFlow()const { return _flow; };

	// Start
	void start();

	// Stop
	void stop();

protected:
	// Send a command that expect a response
	std::string SendCommandWithResponse(const std::string& command);

	// Millisecond sleep
	static void msleep(uint32_t msec);

	double readTemperature();
	double readSetpoint();
	double readFlow();
	int readState();
	bool checkRunning();

protected:
	// Main loop function
	void* run_undetached(void *arg);

private:
	// Terminate flag
	bool _terminate;

	// Access lock
	omni_mutex _lock;

	// PicomotorProxy device proxy
	Tango::DeviceProxy* _device;

	// Parent device
	Polyscience* _parent;

	// Variables
	double _setpoint;
	double _temperature;
	double _flow;

	// Starting flag and timestamp
	bool starting;
	struct timeval st_time;
};

/*----- PROTECTED REGION END -----*/	//	Polyscience::Additional Classes Definitions

}	//	End of namespace

#endif   //	Polyscience_H
