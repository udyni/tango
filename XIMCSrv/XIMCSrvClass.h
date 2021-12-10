/*----- PROTECTED REGION ID(XIMCSrvClass.h) ENABLED START -----*/
// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//=============================================================================
//
// file :        XIMCSrvClass.h
//
// description : Include for the XIMCSrv root class.
//               This class is the singleton class for
//                the XIMCSrv device class.
//               It contains all properties and methods which the
//               XIMCSrv requires only once e.g. the commands.
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


#ifndef XIMCSrvClass_H
#define XIMCSrvClass_H

#include <tango.h>
#include <XIMCSrv.h>


/*----- PROTECTED REGION END -----*/	//	XIMCSrvClass.h


namespace XIMCSrv_ns
{
/*----- PROTECTED REGION ID(XIMCSrvClass::classes for dynamic creation) ENABLED START -----*/


/*----- PROTECTED REGION END -----*/	//	XIMCSrvClass::classes for dynamic creation

//=========================================
//	Define classes for attributes
//=========================================
//	Attribute Position class definition
class PositionAttrib: public Tango::Attr
{
public:
	PositionAttrib():Attr("Position",
			Tango::DEV_DOUBLE, Tango::READ_WRITE) {};
	~PositionAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_Position(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<XIMCSrv *>(dev))->write_Position(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_Position_allowed(ty);}
};

//	Attribute Velocity class definition
class VelocityAttrib: public Tango::Attr
{
public:
	VelocityAttrib():Attr("Velocity",
			Tango::DEV_DOUBLE, Tango::READ_WRITE) {};
	~VelocityAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_Velocity(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<XIMCSrv *>(dev))->write_Velocity(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_Velocity_allowed(ty);}
};

//	Attribute Temperature class definition
class TemperatureAttrib: public Tango::Attr
{
public:
	TemperatureAttrib():Attr("Temperature",
			Tango::DEV_FLOAT, Tango::READ) {};
	~TemperatureAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_Temperature(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_Temperature_allowed(ty);}
};

//	Attribute Acceleration class definition
class AccelerationAttrib: public Tango::Attr
{
public:
	AccelerationAttrib():Attr("Acceleration",
			Tango::DEV_DOUBLE, Tango::READ_WRITE) {};
	~AccelerationAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_Acceleration(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<XIMCSrv *>(dev))->write_Acceleration(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_Acceleration_allowed(ty);}
};

//	Attribute USBvoltage class definition
class USBvoltageAttrib: public Tango::Attr
{
public:
	USBvoltageAttrib():Attr("USBvoltage",
			Tango::DEV_FLOAT, Tango::READ) {};
	~USBvoltageAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_USBvoltage(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_USBvoltage_allowed(ty);}
};

//	Attribute USBcurrent class definition
class USBcurrentAttrib: public Tango::Attr
{
public:
	USBcurrentAttrib():Attr("USBcurrent",
			Tango::DEV_LONG, Tango::READ) {};
	~USBcurrentAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_USBcurrent(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_USBcurrent_allowed(ty);}
};

//	Attribute PwrVoltage class definition
class PwrVoltageAttrib: public Tango::Attr
{
public:
	PwrVoltageAttrib():Attr("PwrVoltage",
			Tango::DEV_FLOAT, Tango::READ) {};
	~PwrVoltageAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_PwrVoltage(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_PwrVoltage_allowed(ty);}
};

//	Attribute PwrCurrent class definition
class PwrCurrentAttrib: public Tango::Attr
{
public:
	PwrCurrentAttrib():Attr("PwrCurrent",
			Tango::DEV_LONG, Tango::READ) {};
	~PwrCurrentAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_PwrCurrent(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_PwrCurrent_allowed(ty);}
};

//	Attribute FirmwareVersion class definition
class FirmwareVersionAttrib: public Tango::Attr
{
public:
	FirmwareVersionAttrib():Attr("FirmwareVersion",
			Tango::DEV_STRING, Tango::READ) {};
	~FirmwareVersionAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_FirmwareVersion(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_FirmwareVersion_allowed(ty);}
};

//	Attribute PowerState class definition
class PowerStateAttrib: public Tango::Attr
{
public:
	PowerStateAttrib():Attr("PowerState",
			Tango::DEV_ENUM, Tango::READ) {};
	~PowerStateAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_PowerState(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_PowerState_allowed(ty);}
	virtual bool same_type(const type_info &in_type) {return typeid(PowerStateEnum) == in_type;}
	virtual std::string get_enum_type() {return std::string("PowerStateEnum");}
};

//	Attribute Referenced class definition
class ReferencedAttrib: public Tango::Attr
{
public:
	ReferencedAttrib():Attr("Referenced",
			Tango::DEV_BOOLEAN, Tango::READ) {};
	~ReferencedAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<XIMCSrv *>(dev))->read_Referenced(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<XIMCSrv *>(dev))->is_Referenced_allowed(ty);}
};


//=========================================
//	Define classes for commands
//=========================================
//	Command GoHome class definition
class GoHomeClass : public Tango::Command
{
public:
	GoHomeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	GoHomeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~GoHomeClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XIMCSrv *>(dev))->is_GoHome_allowed(any);}
};

//	Command SetZero class definition
class SetZeroClass : public Tango::Command
{
public:
	SetZeroClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	SetZeroClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~SetZeroClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XIMCSrv *>(dev))->is_SetZero_allowed(any);}
};

//	Command MoveAbsolute class definition
class MoveAbsoluteClass : public Tango::Command
{
public:
	MoveAbsoluteClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	MoveAbsoluteClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~MoveAbsoluteClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XIMCSrv *>(dev))->is_MoveAbsolute_allowed(any);}
};

//	Command MoveRelative class definition
class MoveRelativeClass : public Tango::Command
{
public:
	MoveRelativeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	MoveRelativeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~MoveRelativeClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XIMCSrv *>(dev))->is_MoveRelative_allowed(any);}
};

//	Command PowerOff class definition
class PowerOffClass : public Tango::Command
{
public:
	PowerOffClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	PowerOffClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~PowerOffClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XIMCSrv *>(dev))->is_PowerOff_allowed(any);}
};

//	Command MoveLeft class definition
class MoveLeftClass : public Tango::Command
{
public:
	MoveLeftClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	MoveLeftClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~MoveLeftClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XIMCSrv *>(dev))->is_MoveLeft_allowed(any);}
};

//	Command MoveRight class definition
class MoveRightClass : public Tango::Command
{
public:
	MoveRightClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	MoveRightClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~MoveRightClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XIMCSrv *>(dev))->is_MoveRight_allowed(any);}
};

//	Command Stop class definition
class StopClass : public Tango::Command
{
public:
	StopClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	StopClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~StopClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XIMCSrv *>(dev))->is_Stop_allowed(any);}
};

//	Command Reset class definition
class ResetClass : public Tango::Command
{
public:
	ResetClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ResetClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ResetClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<XIMCSrv *>(dev))->is_Reset_allowed(any);}
};


/**
 *	The XIMCSrvClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  XIMCSrvClass : public Tango::DeviceClass
#else
class XIMCSrvClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(XIMCSrvClass::Additionnal DServer data members) ENABLED START -----*/


	/*----- PROTECTED REGION END -----*/	//	XIMCSrvClass::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
	
		//	Method prototypes
		static XIMCSrvClass *init(const char *);
		static XIMCSrvClass *instance();
		~XIMCSrvClass();
		Tango::DbDatum	get_class_property(std::string &);
		Tango::DbDatum	get_default_device_property(std::string &);
		Tango::DbDatum	get_default_class_property(std::string &);
	
	protected:
		XIMCSrvClass(std::string &);
		static XIMCSrvClass *_instance;
		void command_factory();
		void attribute_factory(std::vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		std::string get_cvstag();
		std::string get_cvsroot();
	
	private:
		void device_factory(const Tango::DevVarStringArray *);
		void create_static_attribute_list(std::vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,std::vector<Tango::Attr *> &);
		std::vector<std::string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(std::vector<Tango::Attr *> &att_list, std::string attname);
};

}	//	End of namespace

#endif   //	XIMCSrv_H