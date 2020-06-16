/*----- PROTECTED REGION ID(ModbusClass.h) ENABLED START -----*/
// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//=============================================================================
//
// file :        ModbusClass.h
//
// description : Include for the Modbus root class.
//               This class is the singleton class for
//                the Modbus device class.
//               It contains all properties and methods which the
//               Modbus requires only once e.g. the commands.
//
// project :     Modbus
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
// $Author: pascal_verdier $
//
// $Revision: 1.2 $
// $Date: 2012-11-07 08:56:13 $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef ModbusClass_H
#define ModbusClass_H

#include <tango.h>
#include <Modbus.h>

/*----- PROTECTED REGION END -----*/	//	ModbusClass.h


namespace Modbus_ns
{
/*----- PROTECTED REGION ID(ModbusClass::classes for dynamic creation) ENABLED START -----*/


/*----- PROTECTED REGION END -----*/	//	ModbusClass::classes for dynamic creation

//=========================================
//	Define classes for commands
//=========================================
//	Command ForceSingleCoil class definition
class ForceSingleCoilClass : public Tango::Command
{
public:
	ForceSingleCoilClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ForceSingleCoilClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ForceSingleCoilClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ForceSingleCoil_allowed(any);}
};

//	Command ReadCoilStatus class definition
class ReadCoilStatusClass : public Tango::Command
{
public:
	ReadCoilStatusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadCoilStatusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadCoilStatusClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadCoilStatus_allowed(any);}
};

//	Command ReadInputStatus class definition
class ReadInputStatusClass : public Tango::Command
{
public:
	ReadInputStatusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadInputStatusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadInputStatusClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadInputStatus_allowed(any);}
};

//	Command ReadHoldingRegisters class definition
class ReadHoldingRegistersClass : public Tango::Command
{
public:
	ReadHoldingRegistersClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadHoldingRegistersClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadHoldingRegistersClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadHoldingRegisters_allowed(any);}
};

//	Command ReadInputRegisters class definition
class ReadInputRegistersClass : public Tango::Command
{
public:
	ReadInputRegistersClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadInputRegistersClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadInputRegistersClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadInputRegisters_allowed(any);}
};

//	Command PresetSingleRegister class definition
class PresetSingleRegisterClass : public Tango::Command
{
public:
	PresetSingleRegisterClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	PresetSingleRegisterClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~PresetSingleRegisterClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_PresetSingleRegister_allowed(any);}
};

//	Command ReadExceptionStatus class definition
class ReadExceptionStatusClass : public Tango::Command
{
public:
	ReadExceptionStatusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadExceptionStatusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadExceptionStatusClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadExceptionStatus_allowed(any);}
};

//	Command FetchCommEventCtr class definition
class FetchCommEventCtrClass : public Tango::Command
{
public:
	FetchCommEventCtrClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	FetchCommEventCtrClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~FetchCommEventCtrClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_FetchCommEventCtr_allowed(any);}
};

//	Command ForceMultipleCoils class definition
class ForceMultipleCoilsClass : public Tango::Command
{
public:
	ForceMultipleCoilsClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ForceMultipleCoilsClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ForceMultipleCoilsClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ForceMultipleCoils_allowed(any);}
};

//	Command ReadMultipleCoilsStatus class definition
class ReadMultipleCoilsStatusClass : public Tango::Command
{
public:
	ReadMultipleCoilsStatusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadMultipleCoilsStatusClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadMultipleCoilsStatusClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadMultipleCoilsStatus_allowed(any);}
};

//	Command PresetMultipleRegisters class definition
class PresetMultipleRegistersClass : public Tango::Command
{
public:
	PresetMultipleRegistersClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	PresetMultipleRegistersClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~PresetMultipleRegistersClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_PresetMultipleRegisters_allowed(any);}
};

//	Command MaskWriteRegister class definition
class MaskWriteRegisterClass : public Tango::Command
{
public:
	MaskWriteRegisterClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	MaskWriteRegisterClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~MaskWriteRegisterClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_MaskWriteRegister_allowed(any);}
};

//	Command ReadWriteRegister class definition
class ReadWriteRegisterClass : public Tango::Command
{
public:
	ReadWriteRegisterClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadWriteRegisterClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadWriteRegisterClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadWriteRegister_allowed(any);}
};

//	Command PresetSingleRegisterBroadcast class definition
class PresetSingleRegisterBroadcastClass : public Tango::Command
{
public:
	PresetSingleRegisterBroadcastClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	PresetSingleRegisterBroadcastClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~PresetSingleRegisterBroadcastClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_PresetSingleRegisterBroadcast_allowed(any);}
};

//	Command ReadInputRegistersUNS40 class definition
class ReadInputRegistersUNS40Class : public Tango::Command
{
public:
	ReadInputRegistersUNS40Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadInputRegistersUNS40Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadInputRegistersUNS40Class() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadInputRegistersUNS40_allowed(any);}
};

//	Command ReadHoldingRegistersUNS40 class definition
class ReadHoldingRegistersUNS40Class : public Tango::Command
{
public:
	ReadHoldingRegistersUNS40Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadHoldingRegistersUNS40Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadHoldingRegistersUNS40Class() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadHoldingRegistersUNS40_allowed(any);}
};

//	Command PresetSingleRegisterUNS40 class definition
class PresetSingleRegisterUNS40Class : public Tango::Command
{
public:
	PresetSingleRegisterUNS40Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	PresetSingleRegisterUNS40Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~PresetSingleRegisterUNS40Class() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_PresetSingleRegisterUNS40_allowed(any);}
};

//	Command ReadInputRegistersUNS32 class definition
class ReadInputRegistersUNS32Class : public Tango::Command
{
public:
	ReadInputRegistersUNS32Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadInputRegistersUNS32Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadInputRegistersUNS32Class() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadInputRegistersUNS32_allowed(any);}
};

//	Command ReadHoldingRegistersUNS32 class definition
class ReadHoldingRegistersUNS32Class : public Tango::Command
{
public:
	ReadHoldingRegistersUNS32Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadHoldingRegistersUNS32Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadHoldingRegistersUNS32Class() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadHoldingRegistersUNS32_allowed(any);}
};

//	Command PresetSingleRegisterUNS32 class definition
class PresetSingleRegisterUNS32Class : public Tango::Command
{
public:
	PresetSingleRegisterUNS32Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	PresetSingleRegisterUNS32Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~PresetSingleRegisterUNS32Class() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_PresetSingleRegisterUNS32_allowed(any);}
};

//	Command ReadInputString class definition
class ReadInputStringClass : public Tango::Command
{
public:
	ReadInputStringClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadInputStringClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadInputStringClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadInputString_allowed(any);}
};

//	Command ReadHoldingString class definition
class ReadHoldingStringClass : public Tango::Command
{
public:
	ReadHoldingStringClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadHoldingStringClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadHoldingStringClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadHoldingString_allowed(any);}
};

//	Command PresetStringRegister class definition
class PresetStringRegisterClass : public Tango::Command
{
public:
	PresetStringRegisterClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	PresetStringRegisterClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~PresetStringRegisterClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_PresetStringRegister_allowed(any);}
};

//	Command ReadInputRegistersSIG16 class definition
class ReadInputRegistersSIG16Class : public Tango::Command
{
public:
	ReadInputRegistersSIG16Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadInputRegistersSIG16Class(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadInputRegistersSIG16Class() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadInputRegistersSIG16_allowed(any);}
};

//	Command ReadInputRegistersFloat class definition
class ReadInputRegistersFloatClass : public Tango::Command
{
public:
	ReadInputRegistersFloatClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ReadInputRegistersFloatClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ReadInputRegistersFloatClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ReadInputRegistersFloat_allowed(any);}
};

//	Command ForceSingleCoilAmplitude class definition
class ForceSingleCoilAmplitudeClass : public Tango::Command
{
public:
	ForceSingleCoilAmplitudeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ForceSingleCoilAmplitudeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ForceSingleCoilAmplitudeClass() {};

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Modbus *>(dev))->is_ForceSingleCoilAmplitude_allowed(any);}
};


/**
 *	The ModbusClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  ModbusClass : public Tango::DeviceClass
#else
class ModbusClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(ModbusClass::Additionnal DServer data members) ENABLED START -----*/


	/*----- PROTECTED REGION END -----*/	//	ModbusClass::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;

		//	Method prototypes
		static ModbusClass *init(const char *);
		static ModbusClass *instance();
		~ModbusClass();
		Tango::DbDatum	get_class_property(string &);
		Tango::DbDatum	get_default_device_property(string &);
		Tango::DbDatum	get_default_class_property(string &);

	protected:
		ModbusClass(string &);
		static ModbusClass *_instance;
		void command_factory();
		void attribute_factory(vector<Tango::Attr *> &);
		void write_class_property();
		void set_default_property();
		void get_class_property();

	private:
		void device_factory(const Tango::DevVarStringArray *);
		void create_static_attribute_list(vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,vector<Tango::Attr *> &);
		vector<string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname);
};

}	//	End of namespace

#endif   //	Modbus_H