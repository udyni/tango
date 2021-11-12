/*----- PROTECTED REGION ID(XIMCSrv.h) ENABLED START -----*/
// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//=============================================================================
//
// file :        XIMCSrv.h
//
// description : Include file for the XIMCSrv class
//
// project :     Device server for XIMC controller
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


#ifndef XIMCSrv_H
#define XIMCSrv_H

#include <tango.h>
#include <omnithread.h>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <ximc.h>

#define XIMC_MAX_RETRY 4

// Macro to measure elapsed time in ms
#ifndef ELAPSED_TIME_MS
#define ELAPSED_TIME_MS(p, l) ( ((l.tv_sec) - (p.tv_sec)) * 1000 + ((l.tv_usec) - (p.tv_usec)) / 1000 )
#endif


/*----- PROTECTED REGION END -----*/	//	XIMCSrv.h

/**
 *  XIMCSrv class description:
 *    Device server for XIMC controllers like Standa 8SMC4-USB
 */

namespace XIMCSrv_ns
{
enum _PowerStateEnum {
	_UNKNOWN,
	_OFF,
	_UNDEFINED,
	_NORMAL,
	_REDUCED,
	_MAXIMUM,
} ;
typedef _PowerStateEnum PowerStateEnum;

/*----- PROTECTED REGION ID(XIMCSrv::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations
class XIMCThread;


/*----- PROTECTED REGION END -----*/	//	XIMCSrv::Additional Class Declarations

class XIMCSrv : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(XIMCSrv::Data Members) ENABLED START -----*/

//	Add your own data members
private:
	XIMCThread *dev;

/*----- PROTECTED REGION END -----*/	//	XIMCSrv::Data Members

//	Device property data members
public:
	//	SerialNumber:	XIMC device serial number
	std::string	serialNumber;
	//	Referenced:	If set to true at init the stage will search for it`s home position and set it to zero, then move to the last saved position.
	Tango::DevBoolean	referenced;
	//	Polling:	Polling period in ms
	Tango::DevULong	polling;
	//	Conversion:	Conversion of steps to physical units
	Tango::DevDouble	conversion;
	//	ConversionEnc:	Conversion of encoder counts to physical units
	Tango::DevDouble	conversionEnc;
	//	UseEncoder:	Enable the use of the encoder
	Tango::DevBoolean	useEncoder;
	//	Units:	Units for position (for velocity will be units/s, for acceleration units/s^2)
	std::string	units;
	//	PositionFormat:	Format string for stage position
	std::string	positionFormat;
	//	FirstHomingSpeed:	Speed of the first homing move for the standard protocol (step/s)
	Tango::DevLong	firstHomingSpeed;
	//	SecondHomingSpeed:	Speed of the second homing move for the standard algorithm (step/s).
	//  If the Fast Homing Algorithm is enabled, is the speed of the homing operation.
	Tango::DevLong	secondHomingSpeed;
	//	UseFastAlgorithm:	Use fast homing algorithm
	Tango::DevBoolean	useFastAlgorithm;
	//	HomingDirection:	Homing move direction: false for left, true for right (default left, i.e. negative direction))
	Tango::DevBoolean	homingDirection;
	//	BlindMoveBeforeHoming:	Move by this relative amount (in user units) before starting home search
	Tango::DevDouble	blindMoveBeforeHoming;
	//	HomingDelta:	Move to reach home position after limit swith trigger.
	Tango::DevLong	homingDelta;
	//	PositionAtHome:	Position value at home
	Tango::DevDouble	positionAtHome;

	bool	mandatoryNotDefined;

//	Attribute data members
public:
	Tango::DevDouble	*attr_Position_read;
	Tango::DevDouble	*attr_Velocity_read;
	Tango::DevFloat	*attr_Temperature_read;
	Tango::DevDouble	*attr_Acceleration_read;
	Tango::DevFloat	*attr_USBvoltage_read;
	Tango::DevLong	*attr_USBcurrent_read;
	Tango::DevFloat	*attr_PwrVoltage_read;
	Tango::DevLong	*attr_PwrCurrent_read;
	Tango::DevString	*attr_FirmwareVersion_read;
	PowerStateEnum	*attr_PowerState_read;
	Tango::DevBoolean	*attr_Referenced_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	XIMCSrv(Tango::DeviceClass *cl,std::string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	XIMCSrv(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	XIMCSrv(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~XIMCSrv() {delete_device();};


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
	 *	Method      : XIMCSrv::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(std::vector<long> &attr_list);
	//--------------------------------------------------------
	/*
	 *	Method      : XIMCSrv::write_attr_hardware()
	 *	Description : Hardware writing for attributes.
	 */
	//--------------------------------------------------------
	virtual void write_attr_hardware(std::vector<long> &attr_list);

/**
 *	Attribute Position related methods
 *	Description: Position in steps
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_Position(Tango::Attribute &attr);
	virtual void write_Position(Tango::WAttribute &attr);
	virtual bool is_Position_allowed(Tango::AttReqType type);
/**
 *	Attribute Velocity related methods
 *	Description: Velocity in steps/s
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_Velocity(Tango::Attribute &attr);
	virtual void write_Velocity(Tango::WAttribute &attr);
	virtual bool is_Velocity_allowed(Tango::AttReqType type);
/**
 *	Attribute Temperature related methods
 *	Description: Controller temperature
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Scalar
 */
	virtual void read_Temperature(Tango::Attribute &attr);
	virtual bool is_Temperature_allowed(Tango::AttReqType type);
/**
 *	Attribute Acceleration related methods
 *	Description: Acceleration and deceleration
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_Acceleration(Tango::Attribute &attr);
	virtual void write_Acceleration(Tango::WAttribute &attr);
	virtual bool is_Acceleration_allowed(Tango::AttReqType type);
/**
 *	Attribute USBvoltage related methods
 *	Description: USB voltage
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Scalar
 */
	virtual void read_USBvoltage(Tango::Attribute &attr);
	virtual bool is_USBvoltage_allowed(Tango::AttReqType type);
/**
 *	Attribute USBcurrent related methods
 *	Description: USB current
 *
 *	Data type:	Tango::DevLong
 *	Attr type:	Scalar
 */
	virtual void read_USBcurrent(Tango::Attribute &attr);
	virtual bool is_USBcurrent_allowed(Tango::AttReqType type);
/**
 *	Attribute PwrVoltage related methods
 *	Description: Motor power supply voltage
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Scalar
 */
	virtual void read_PwrVoltage(Tango::Attribute &attr);
	virtual bool is_PwrVoltage_allowed(Tango::AttReqType type);
/**
 *	Attribute PwrCurrent related methods
 *	Description: Motor supply current consumption
 *
 *	Data type:	Tango::DevLong
 *	Attr type:	Scalar
 */
	virtual void read_PwrCurrent(Tango::Attribute &attr);
	virtual bool is_PwrCurrent_allowed(Tango::AttReqType type);
/**
 *	Attribute FirmwareVersion related methods
 *	Description:
 *
 *	Data type:	Tango::DevString
 *	Attr type:	Scalar
 */
	virtual void read_FirmwareVersion(Tango::Attribute &attr);
	virtual bool is_FirmwareVersion_allowed(Tango::AttReqType type);
/**
 *	Attribute PowerState related methods
 *	Description:
 *
 *	Data type:	Tango::DevEnum
 *	Attr type:	Scalar
 */
	virtual void read_PowerState(Tango::Attribute &attr);
	virtual bool is_PowerState_allowed(Tango::AttReqType type);
/**
 *	Attribute Referenced related methods
 *	Description:
 *
 *	Data type:	Tango::DevBoolean
 *	Attr type:	Scalar
 */
	virtual void read_Referenced(Tango::Attribute &attr);
	virtual bool is_Referenced_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : XIMCSrv::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:
	/**
	 *	Command GoHome related method
	 *	Description: Search home position
	 *
	 */
	virtual void go_home();
	virtual bool is_GoHome_allowed(const CORBA::Any &any);
	/**
	 *	Command SetZero related method
	 *	Description: Set current position as zero
	 *
	 */
	virtual void set_zero();
	virtual bool is_SetZero_allowed(const CORBA::Any &any);
	/**
	 *	Command MoveAbsolute related method
	 *	Description: Move to an absolute position
	 *
	 *	@param argin Position
	 */
	virtual void move_absolute(Tango::DevDouble argin);
	virtual bool is_MoveAbsolute_allowed(const CORBA::Any &any);
	/**
	 *	Command MoveRelative related method
	 *	Description: Move to an relative position
	 *
	 *	@param argin Position
	 */
	virtual void move_relative(Tango::DevDouble argin);
	virtual bool is_MoveRelative_allowed(const CORBA::Any &any);
	/**
	 *	Command PowerOff related method
	 *	Description: Power off motor
	 *
	 */
	virtual void power_off();
	virtual bool is_PowerOff_allowed(const CORBA::Any &any);
	/**
	 *	Command MoveLeft related method
	 *	Description: Start continuous move to the left
	 *
	 */
	virtual void move_left();
	virtual bool is_MoveLeft_allowed(const CORBA::Any &any);
	/**
	 *	Command MoveRight related method
	 *	Description: Start continuous move to the right
	 *
	 */
	virtual void move_right();
	virtual bool is_MoveRight_allowed(const CORBA::Any &any);
	/**
	 *	Command Stop related method
	 *	Description: Stop motion
	 *
	 */
	virtual void stop();
	virtual bool is_Stop_allowed(const CORBA::Any &any);
	/**
	 *	Command Reset related method
	 *	Description: Reset the controller sending a hard stop command
	 *
	 */
	virtual void reset();
	virtual bool is_Reset_allowed(const CORBA::Any &any);


	//--------------------------------------------------------
	/**
	 *	Method      : XIMCSrv::add_dynamic_commands()
	 *	Description : Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(XIMCSrv::Additional Method prototypes) ENABLED START -----*/

//	Additional Method prototypes

/*----- PROTECTED REGION END -----*/	//	XIMCSrv::Additional Method prototypes
};

/*----- PROTECTED REGION ID(XIMCSrv::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions
class XIMCThread : public omni_thread {
public:
	XIMCThread(const std::string &serial, XIMCSrv *parent);
	~XIMCThread();

	// Command STOP: stop all movements immediately
	void stop();

	// Command PWOF: turn off motor power
	void powerOff();

	// Command MOVE: absolute move
	void moveAbs(double fpos);

	// Command MOVR: relative move
	void moveRel(double fpos);

	// Command HOME: go home
	void goHome();

	// Command LEFT: start a continuous move to the left
	void moveLeft();

	// Command RIGT: start a continuous move to the right
	void moveRight();

	// Command SSTP: soft stop (with deceleration)
	void softStop();

	// Command ZERO: set current position as zero
	void zero();

	// Terminate thread
	void terminate() { _terminate = true; }

	// Return supply information from status struct
	float getUSB_V()const { return _st_usb_v; }
	Tango::DevLong getUSB_I()const { return _st_usb_i; }
	float getPWR_V()const { return _st_pwr_v; }
	Tango::DevLong getPWR_I()const { return _st_pwr_i; }

	// Return controller temperature from status struct
	float getTemperature()const { return _st_temp; }

	// Get stage position in physical units
	double getPosition()const;

	// Get current velocity
	double getVelocity()const;

	// Set current velocity
	void setVelocity(double vel);

	// Get current acceleration and deceleration
	double getAcceleration()const;

	// Set current acceleration and deceleration
	void setAcceleration(double acc);

	// Get motor power state
	unsigned int getPowerState()const { return _power_state; }

	// Return true if the stage is homed
	bool isReferenced()const { return _is_homed; }

protected:
	// Search home routine
	void searchHome();

	// Main polling thread
	void* run_undetached(void *arg);

	// Initialize device
	void init_device();

	// Check that the device is open
	void check_device();

	// Close the device
	void delete_device();

	// Logging callback
	static void logging_callback(int loglevel, const wchar_t* message, void* user_data);

	// Check return code from libximc calls
	void check_error_code(result_t code, const char* operation, const char* function);

	// Conversion from steps to physical units
	void convert_steps2phys(int32_t steps, int32_t usteps, double &phys)const;
	void convert_steps2phys(uint32_t steps, uint32_t usteps, double &phys)const;

	// Conversion from physical units to steps
	void convert_phys2steps(double phys, int32_t &steps, int32_t &usteps)const;
	void convert_phys2steps(double phys, uint32_t &steps, uint32_t &usteps)const;

	// Wait for a move to finish
	void waitForMove(int timeout);

private:
	// Device URI
	std::string _device_uri;

	// Device
	device_t _dev;

	// Terminate flag
	bool _terminate;

	// Parent device
	XIMCSrv *_parent;

	// Flags and values from device properties
	bool _referenced;
	bool _encoder;
	double _conv_steps;
	double _conv_enc;

	// Status variables for attributes
	double _st_pos;
	int _curr_pos;
	int _curr_upos;
	int64_t _st_enc;
	float _st_usb_v;
	Tango::DevLong _st_usb_i;
	float _st_pwr_v;
	Tango::DevLong _st_pwr_i;
	float _st_temp;
	bool _is_homed;

	// Controller motion settings
	move_settings_t _motion;

	// Microstepping mode
	uint16_t _microsteps;

	// Encoder feedback enabled
	bool _enc_enabled;

	// Home search requested
	bool _home_search;

	// Motor power state
	Tango::DevShort _power_state;
};


/*----- PROTECTED REGION END -----*/	//	XIMCSrv::Additional Classes Definitions

}	//	End of namespace

#endif   //	XIMCSrv_H
