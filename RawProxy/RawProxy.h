/*----- PROTECTED REGION ID(RawProxy.h) ENABLED START -----*/
//=============================================================================
//
// file :        RawProxy.h
//
// description : Include file for the RawProxy class
//
// project :     Raw proxy for serial communication
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


#ifndef RawProxy_H
#define RawProxy_H

#include <tango.h>
#include "SerialSocket.h"
#include "TcpSocket.h"


/*----- PROTECTED REGION END -----*/	//	RawProxy.h

/**
 *  RawProxy class description:
 *    Raw proxy for binary serial communication. May be used locally or through TCP/IP
 */

namespace RawProxy_ns
{
/*----- PROTECTED REGION ID(RawProxy::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations

/*----- PROTECTED REGION END -----*/	//	RawProxy::Additional Class Declarations

class RawProxy : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(RawProxy::Data Members) ENABLED START -----*/

//	Add your own data members
private:
	// Communication socket
	CommSocket *sock;

	// Force disconnect flag (to handle manual disconnection)
	Tango::DevBoolean force_disconnect;

/*----- PROTECTED REGION END -----*/	//	RawProxy::Data Members

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
	RawProxy(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	RawProxy(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	RawProxy(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */	
	~RawProxy() {delete_device();};


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
	 *	Method      : RawProxy::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);


	//--------------------------------------------------------
	/**
	 *	Method      : RawProxy::add_dynamic_attributes()
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
	 *	Command Send related method
	 *	Description: Send a binary message to the socket
	 *
	 *	@param argin Message
	 *	@returns Number of bytes sent
	 */
	virtual Tango::DevLong send(const Tango::DevVarCharArray *argin);
	virtual bool is_Send_allowed(const CORBA::Any &any);
	/**
	 *	Command Recv related method
	 *	Description: Receive a message from the socket
	 *
	 *	@param argin Number of bytes to read
	 *	@returns Received message
	 */
	virtual Tango::DevVarCharArray *recv(Tango::DevLong argin);
	virtual bool is_Recv_allowed(const CORBA::Any &any);
	/**
	 *	Command SyncSendRecv related method
	 *	Description: Send a message and read output until timeout.
	 *               Receive timeout after first read is 1/10 of total timeout.
	 *
	 *	@param argin Character to send
	 *	@returns Response
	 */
	virtual Tango::DevVarCharArray *sync_send_recv(const Tango::DevVarCharArray *argin);
	virtual bool is_SyncSendRecv_allowed(const CORBA::Any &any);


/*----- PROTECTED REGION ID(RawProxy::Additional Method prototypes) ENABLED START -----*/

//	Additional Method prototypes

/*----- PROTECTED REGION END -----*/	//	RawProxy::Additional Method prototypes
};

/*----- PROTECTED REGION ID(RawProxy::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions

/*----- PROTECTED REGION END -----*/	//	RawProxy::Additional Classes Definitions

}	//	End of namespace

#endif   //	RawProxy_H
