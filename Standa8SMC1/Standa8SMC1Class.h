/*----- PROTECTED REGION ID(Standa8SMC1Class.h) ENABLED START -----*/
//=============================================================================
//
// file :        Standa8SMC1Class.h
//
// description : Include for the Standa8SMC1 root class.
//               This class is the singleton class for
//                the Standa8SMC1 device class.
//               It contains all properties and methods which the 
//               Standa8SMC1 requires only once e.g. the commands.
//
// project :     Standa 8SMC1 Device Server
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


#ifndef Standa8SMC1Class_H
#define Standa8SMC1Class_H

#include <tango.h>
#include <Standa8SMC1.h>


/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class.h


namespace Standa8SMC1_ns
{
/*----- PROTECTED REGION ID(Standa8SMC1Class::classes for dynamic creation) ENABLED START -----*/


/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::classes for dynamic creation

//=========================================
//	Define classes for attributes
//=========================================
//	Attribute Position class definition
class PositionAttrib: public Tango::Attr
{
public:
	PositionAttrib():Attr("Position",
			Tango::DEV_LONG, Tango::READ_WRITE) {};
	~PositionAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<Standa8SMC1 *>(dev))->read_Position(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<Standa8SMC1 *>(dev))->write_Position(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<Standa8SMC1 *>(dev))->is_Position_allowed(ty);}
};

//	Attribute Acceleration class definition
class AccelerationAttrib: public Tango::Attr
{
public:
	AccelerationAttrib():Attr("Acceleration",
			Tango::DEV_DOUBLE, Tango::READ_WRITE) {};
	~AccelerationAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<Standa8SMC1 *>(dev))->read_Acceleration(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<Standa8SMC1 *>(dev))->write_Acceleration(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<Standa8SMC1 *>(dev))->is_Acceleration_allowed(ty);}
};

//	Attribute Velocity class definition
class VelocityAttrib: public Tango::Attr
{
public:
	VelocityAttrib():Attr("Velocity",
			Tango::DEV_DOUBLE, Tango::READ_WRITE) {};
	~VelocityAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<Standa8SMC1 *>(dev))->read_Velocity(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{(static_cast<Standa8SMC1 *>(dev))->write_Velocity(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<Standa8SMC1 *>(dev))->is_Velocity_allowed(ty);}
};

//	Attribute Temperature class definition
class TemperatureAttrib: public Tango::Attr
{
public:
	TemperatureAttrib():Attr("Temperature",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~TemperatureAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<Standa8SMC1 *>(dev))->read_Temperature(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<Standa8SMC1 *>(dev))->is_Temperature_allowed(ty);}
};

//	Attribute Voltage class definition
class VoltageAttrib: public Tango::Attr
{
public:
	VoltageAttrib():Attr("Voltage",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~VoltageAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<Standa8SMC1 *>(dev))->read_Voltage(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<Standa8SMC1 *>(dev))->is_Voltage_allowed(ty);}
};


//=========================================
//	Define classes for commands
//=========================================
//	Command setHome class definition
class setHomeClass : public Tango::Command
{
public:
	setHomeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	setHomeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~setHomeClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Standa8SMC1 *>(dev))->is_setHome_allowed(any);}
};

//	Command goHome class definition
class goHomeClass : public Tango::Command
{
public:
	goHomeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	goHomeClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~goHomeClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<Standa8SMC1 *>(dev))->is_goHome_allowed(any);}
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
	{return (static_cast<Standa8SMC1 *>(dev))->is_Stop_allowed(any);}
};


/**
 *	The Standa8SMC1Class singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  Standa8SMC1Class : public Tango::DeviceClass
#else
class Standa8SMC1Class : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(Standa8SMC1Class::Additionnal DServer data members) ENABLED START -----*/
	
	
	/*----- PROTECTED REGION END -----*/	//	Standa8SMC1Class::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
	
		//	Method prototypes
		static Standa8SMC1Class *init(const char *);
		static Standa8SMC1Class *instance();
		~Standa8SMC1Class();
		Tango::DbDatum	get_class_property(string &);
		Tango::DbDatum	get_default_device_property(string &);
		Tango::DbDatum	get_default_class_property(string &);
	
	protected:
		Standa8SMC1Class(string &);
		static Standa8SMC1Class *_instance;
		void command_factory();
		void attribute_factory(vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		string get_cvstag();
		string get_cvsroot();
	
	private:
		void device_factory(const Tango::DevVarStringArray *);
		void create_static_attribute_list(vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,vector<Tango::Attr *> &);
		vector<string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname);
};

}	//	End of namespace

#endif   //	Standa8SMC1_H
