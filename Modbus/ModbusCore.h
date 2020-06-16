// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//+*********************************************************************
//
// File:        ModbusCore.cpp
//
// Project:     Device Servers in C++
//
// Description: public include file containing definitions and declarations
//		for implementing the Modbus protocol class in C++
//              independently of TACO or TANGO.
//
// Author(s);   Andy Gotz
//
// Original:    August 2001
//
// $Log: not supported by cvs2svn $
// Revision 1.5  2008/07/18 10:40:26  taurel
// - Add a new TCPTimeout property used during communication between
// the device and the Modbus equipment
//
// Revision 1.4  2008/07/03 09:16:26  jensmeyer
// Added a SocketConnectionSleep property to define the wait time between
// a socket closing and the reopening of a new socket to connect to the hardware.
//
// Revision 1.3  2008/03/17 14:37:35  taurel
// - Add a data cache for the ReadHoldingRegisters, ReadMultipleCoilsStatus, ReadInputRegisters and ReadInputStatus commands
// - Fix some bugs related to closing the sockets in TCP mode
// - The Address property is used for the modbus ID sent at the frame
// beginning
//
// Revision 1.2  2008/02/11 14:29:29  taurel
// - Ported to Windows VC8
//
// Revision 1.1  2005/01/14 15:36:55  jlpons
// Initial import
//
// Revision 2.0  2004/12/02 14:24:06  perez
// Split Modbus.cpp
//
//
//
//
//
// Copyright (c) 2001 by European Synchrotron Radiation Facility,
//                       Grenoble, France
//
//
//
//-*********************************************************************

#ifndef _ModbusCore_H
#define _ModbusCore_H

#ifdef WIN32
	#ifndef __WIN32_STDINT
		#define __WIN32_STDINT
		typedef unsigned short  uint16_t;
		typedef unsigned char   uint8_t;
	#endif
#else
	#include <stdint.h>
#endif

#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <stdarg.h>
#include <exception>
#include <fcntl.h>
#include <sys/types.h>
#ifdef WIN32
	#include <winsock2.h>
#else
	#include <unistd.h>
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <sys/time.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
#endif
#include <ModbusCoreSL.h>


//+=====================================================================
// Global definitions
//-=====================================================================
#define MBUS_RTU	1
#define MBUS_ASCII	2
#define MBUS_TCP	3

#define READ_COIL_STATUS                        1
#define READ_INPUT_STATUS                       2
#define READ_HOLDING_REGISTERS                  3
#define READ_INPUT_REGISTERS                    4
#define FORCE_SINGLE_COIL                       5
#define PRESET_SINGLE_REGISTER                  6
#define READ_EXCEPTION_STATUS                   7
#define FETCH_COMM_EVENT_CTR                    11
#define FETCH_COMM_EVENT_LOG                    12
#define FORCE_MULTIPLE_COILS                    15
#define PRESET_MULTIPLE_REGISTERS               16
#define REPORT_SLAVE_ID                         17
#define READ_GENERAL_REFERENCE                  20
#define WRITE_GENERAL_REFERENCE                 21
#define MASK_WRITE_REGISTER                     22
#define READ_WRITE_REGISTERS                    23
#define READ_FIFO_QUEUE                         24

// Amplitude extra function codes
#define READ_INPUT_UNS40                        65
#define READ_HOLDING_UNS40                      66
#define WRITE_HOLDING_UNS40                     67
#define READ_INPUT_UNS32                        68
#define READ_HOLDING_UNS32                      69
#define WRITE_HOLDING_UNS32                     70
#define READ_INPUT_STR20                        73
#define READ_HOLDING_STR20                      71
#define WRITE_HOLDING_STR20                     72
#define READ_INPUT_SIG16                       100
#define READ_INPUT_FLOAT                       103


//+=====================================================================
// Class definition
//-=====================================================================

#define MODBUS_ERR_MSG_LEN 128
/* @class ModbusException
 * Default exception for communication socket classes
 * Extends std::exception to be able to format messages
 */
class ModbusException : public std::exception {
public:
	//! Constructor
	/*! Constructor of the exception. Can format an error message.
	 *  @param f the format string (as in printf)
	 *  @param ... variable parameters, corresponding to the format string.
	 */
	ModbusException(const char* f, ...) {
		va_list ap;
		va_start(ap, f);
		vsnprintf(_msg, MODBUS_ERR_MSG_LEN, f, ap);
		va_end(ap);
	}
	ModbusException(const char* f, va_list ap) {
		vsnprintf(_msg, MODBUS_ERR_MSG_LEN, f, ap);
	}

	//! what(), return the error string
	virtual const char* what() const throw() {
		return _msg;
	}

private:
	char _msg[MODBUS_ERR_MSG_LEN];
};


class ModbusCore {

//
// public members
//
public:

	short   protocol;         /* Modbus protocol RTU/ASCII */
	bool    ip_connection;    /* true if sockect is connected*/
	short   address;          /* modbus node address */
	char*   serialline_name;  /* device file */
	char*   ip_host;          /* ip host name for tcp/ip */
	int     ip_timeout;       /* Timeout used for TCP commmunication */

	ModbusCore (
		char  *serialline_name,
		short protocol,
		short address,
		char  *ip_host,
		long  socketConnectionSleep,
		int   tcp_to);
	~ModbusCore ();

	char *Status();
	void SendGet(uint8_t *frame, size_t frame_length, uint8_t *response, size_t response_length);
	void Send(uint8_t *frame, size_t frame_length);

//
// protected members
//
protected:
	int                  ip_socket;           /* open socket for tcp/ip */
	struct sockaddr_in   ip_address;          /* tcp/ip address */
	int                  ip_status;           /* status of last tcp/ip call */
	long                 connection_sleep;    /* sleep time in ms between two socket connections */
	omni_mutex           modb_access;

	ModbusCoreSL*        sl;
	uint8_t *            _query;
	size_t               _query_length;

	void TCPOpenSocket (void);
	void CalculateCRC (uint8_t *frame, size_t frame_length, uint8_t *crc);
	void SendFrame (uint8_t *frame, size_t frame_length);
	void SendRTUFrame (uint8_t *frame, size_t frame_length);
	void SendTCPFrame (uint8_t *frame, size_t frame_length);
	void GetResponse (uint8_t *frame, size_t frame_length);
	void GetRTUResponse (uint8_t *frame, size_t frame_length);
	void GetTCPResponse (uint8_t *frame, size_t frame_length);
};

#endif /* _ModbusCore_H */
