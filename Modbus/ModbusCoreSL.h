// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//+*********************************************************************
//
// File:        ModbusCoreSL.cpp
//
// Project:     Device Servers in C++
//
// Description: public include file containing definitions and declarations
//		of serial line access function.
//
// Author(s);   Michele Devetta
//
// Original:    March 2015
//
// NOTE: reimplementation of ModbusCoreSL by Andy Gotz
//
//-*********************************************************************

#ifndef _ModbusCoreSL_H
#define _ModbusCoreSL_H

class ModbusCoreSL;

#include <tango.h>
#include "ModbusCore.h"

//+=====================================================================
// Class definition
//-=====================================================================
class ModbusCoreSL {
public:
	// Constructor
	ModbusCoreSL(const char* device);
	~ModbusCoreSL();

	int write(uint8_t* frameout, size_t ncharout);
	int read(uint8_t* framein, size_t ncharexp, size_t* ncharin);

private:
	Tango::DeviceProxy   *_dev;     /* Tango device handle */
};

#endif /* _ModbusCoreSL_H */
