// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//+*********************************************************************
//
// File:        ModbusCoreSL.cpp
//
// Project:     Device Servers in C++
//
// Description: Code for implementing the ModbusCoreSL serial line access.
//              The current code uses a TANGO Serial Line Device Server.
//
// Author(s);   JL PONS
//
// Original:    Jan 2005
//
// $Log: not supported by cvs2svn $
// Revision 1.2  2011/05/19 14:58:34  jensmeyer
// Added a flushing of the serial line bufferes before and write request
//
// Revision 1.1  2009/02/25 13:58:41  buteau
// - files moved to src subdirectory
//
// Revision 1.7  2008/11/19 14:09:06  jean_coquet
// #ifdef changed in case of NOSERIAL special case for SOLEIL (in ModbusCoreSL.cpp .h)
// so now it compiles and runs under W32
//
// Revision 1.6  2008/07/01 10:54:13  taurel
// - Remove some printf
//
// Revision 1.5  2008/06/24 10:40:47  buteau
// Soleil added an #ifdef to avoid linking with Serial
//
// Revision 1.4  2006/11/02 10:33:32  fbecheri
// Try/Catch missing.
//
// Revision 1.3  2005/03/31 15:07:05  jlpons
// Changed namespace name
//
// Revision 1.2  2005/03/01 17:53:35  jlpons
// Few updates.
//
// Revision 1.1  2005/01/14 15:36:55  jlpons
// Initial import
//
// Revision 2.0  2004/12/02 14:23:59  perez
//
//
// Copyright (c) 2001 by European Synchrotron Radiation Facility,
//                       Grenoble, France
//
//-*********************************************************************

#include <ModbusCoreSL.h>


//+======================================================================
// Function:    ModbusCoreSL::ModbusCoreSL()
//
// Description: create a ModbusCoreSL object
//
//-=====================================================================
ModbusCoreSL::ModbusCoreSL(const char *serialline_name) : _dev(NULL)
{
	// Connect to raw serial device
	try {
		// Connect to Tango Device
		_dev = new Tango::DeviceProxy(serialline_name);

		// Flush serial device
		_dev->command_inout("Flush");

	} catch(Tango::DevFailed &e) {
		std::string msg(e.errors[0].desc);
		throw ModbusException("ModbusCoreSL(): error connecting to serial device (Error: %s)", msg.c_str());
	}
}


//+=====================================================================
// Function:    ModbusCoreSL::~ModbusCoreSL()
//
// Description: destructor to destroy an object of the ModbusCoreSL class
//
//-=====================================================================
ModbusCoreSL::~ModbusCoreSL()
{
	if(_dev)
		delete _dev;
}


//+=====================================================================
// Function:    ModbusCoreSL::write()
//-=====================================================================
int ModbusCoreSL::write(uint8_t *frame, size_t ncharout)
{
	if(_dev == NULL)
		throw ModbusException("ModbusCoreSL::write(): serial line not connected.");

	Tango::DeviceData argin;
	Tango::DeviceData argout;
	Tango::DevVarCharArray vcharr;

	vcharr.length(ncharout);
	for(size_t i = 0; i<ncharout; i++)
		vcharr[i] = frame[i];

	try{
		// flush serial device
		_dev->command_inout("Flush");

		// write the frame
		argin << vcharr;
		argout = _dev->command_inout("Send",argin);
		Tango::DevLong ans;
		argout >> ans;
		return ans;

	} catch(Tango::DevFailed &e) {
		std::string msg(e.errors[0].desc);
		throw ModbusException("ModbusCoreSL::write(): %s", msg.c_str());
	}
}


//+=====================================================================
// Function:    ModbusCoreSL::read()
//-=====================================================================
int ModbusCoreSL::read(uint8_t *frame, size_t ncharexp, size_t *ncharin)
{
	if(_dev == NULL)
		throw ModbusException("ModbusCoreSL::read(): serial line not connected.");

	Tango::DeviceData argin;
	Tango::DeviceData argout;
	const Tango::DevVarCharArray *vcharr;

	try {
		argin << (Tango::DevLong)ncharexp;
		argout = _dev->command_inout("Recv", argin);
		argout >> vcharr;

	} catch(Tango::DevFailed &e) {
		std::string msg(e.errors[0].desc);
		throw ModbusException("ModbusCoreSL::read(): %s", msg.c_str());
	}

	// Copy data to output buffer
	*ncharin = vcharr->length();
	for(size_t i = 0; i < vcharr->length(); i++) {
		frame[i] = (*vcharr)[i];
	}
	return *ncharin;
}
