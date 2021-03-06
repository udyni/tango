/*----- PROTECTED REGION ID(USSProxy.h) ENABLED START -----*/
//=============================================================================
//
// file :        USSProxy.h
//
// description : Include file for the USSProxy class
//
// project :     USS Proxy
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


#ifndef USSProxy_H
#define USSProxy_H

#include <tango.h>
#include "SerialSocket.h"
#include "TcpSocket.h"

#ifndef ELAPSED_TIME_MS
#define ELAPSED_TIME_MS(p, l) ( ((l.tv_sec) - (p.tv_sec)) * 1000 + ((l.tv_usec) - (p.tv_usec)) / 1000 )
#endif

#define USS_TELEGRAM_LEN  256


/*----- PROTECTED REGION END -----*/	//	USSProxy.h

/**
 *  USSProxy class description:
 *    USS protocol (Universal Serial Interface Protocol) proxy interface. Support both local and TCP/IP communication.
 */

namespace USSProxy_ns
{
/*----- PROTECTED REGION ID(USSProxy::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations

/*----- PROTECTED REGION END -----*/	//	USSProxy::Additional Class Declarations

class USSProxy : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(USSProxy::Data Members) ENABLED START -----*/

//	Add your own data members
private:
	// Communication socket
	CommSocket *sock;

	// Force disconnect flag (to handle manual disconnection)
	Tango::DevBoolean force_disconnect;

/*----- PROTECTED REGION END -----*/	//	USSProxy::Data Members

//	Device property data members
public:
	//	DeviceAddress:	Address of the device. May be an IP address, a hostname or a device file.
	string	deviceAddress;
	//	Timeout:	Receive timeout in milliseconds
	Tango::DevLong	timeout;
	//	AutoReconnect:	Enable autoreconnection
	Tango::DevBoolean	autoReconnect;
	//	DeviceType:	Device type. May be `serial` or `tcp`.
	//  In `tcp` mode the DeviceAddress has the format `hostname:port`.
	string	deviceType;
	//	SerialBaud:	Serial port baud rate
	Tango::DevLong	serialBaud;
	//	SerialSetup:	Port setup in terms of data bits, parity and stop bits.
	//  A string with format <data><parity><stopbits>.
	//  Data may be 5, 6, 7 or 8.
	//  Parity may be N (none), E (even) or O (odd).
	//  Stopbits may be 1 or 2.
	//  Default is 8N1.
	string	serialSetup;

	bool	mandatoryNotDefined;


//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	USSProxy(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	USSProxy(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	USSProxy(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */	
	~USSProxy() {delete_device();};


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
	 *	Method      : USSProxy::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);


	//--------------------------------------------------------
	/**
	 *	Method      : USSProxy::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();



//	Command related methods
public:
	/**
	 *	Command Connect related method
	 *	Description: Open connection to device
	 *
	 */
	virtual void connect();
	virtual bool is_Connect_allowed(const CORBA::Any &any);
	/**
	 *	Command Disconnect related method
	 *	Description: Close connection to device
	 *
	 */
	virtual void disconnect();
	virtual bool is_Disconnect_allowed(const CORBA::Any &any);
	/**
	 *	Command Flush related method
	 *	Description: Make sure that the receive buffer is empty.
	 *
	 */
	virtual void flush();
	virtual bool is_Flush_allowed(const CORBA::Any &any);
	/**
	 *	Command SendTelegram related method
	 *	Description: Send a telegram with the USS protocol and return the answer
	 *
	 *	@param argin The telegram to send. The array length is the length of the telegram.
	 *	@returns Response telegram.
	 */
	virtual Tango::DevVarCharArray *send_telegram(const Tango::DevVarCharArray *argin);
	virtual bool is_SendTelegram_allowed(const CORBA::Any &any);


/*----- PROTECTED REGION ID(USSProxy::Additional Method prototypes) ENABLED START -----*/

//	Additional Method prototypes

	// Compute USS crc code
	char compute_uss_crc(char *buffer, size_t length);

/*----- PROTECTED REGION END -----*/	//	USSProxy::Additional Method prototypes
};

/*----- PROTECTED REGION ID(USSProxy::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions

/*----- PROTECTED REGION END -----*/	//	USSProxy::Additional Classes Definitions

}	//	End of namespace

#endif   //	USSProxy_H
