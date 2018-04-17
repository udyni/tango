/*----- PROTECTED REGION ID(VacuumValveSrvClass.h) ENABLED START -----*/
//=============================================================================
//
// file :        VacuumValveSrvClass.h
//
// description : Include for the VacuumValveSrv root class.
//               This class is the singleton class for
//                the VacuumValveSrv device class.
//               It contains all properties and methods which the 
//               VacuumValveSrv requires only once e.g. the commands.
//
// project :     Valve control server
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


#ifndef VacuumValveSrvClass_H
#define VacuumValveSrvClass_H

#include <tango.h>
#include <VacuumValveSrv.h>


/*----- PROTECTED REGION END -----*/	//	VacuumValveSrvClass.h


namespace VacuumValveSrv_ns
{
/*----- PROTECTED REGION ID(VacuumValveSrvClass::classes for dynamic creation) ENABLED START -----*/


/*----- PROTECTED REGION END -----*/	//	VacuumValveSrvClass::classes for dynamic creation

//=========================================
//	Define classes for attributes
//=========================================
//	Attribute RawVoltage class definition
class RawVoltageAttrib: public Tango::Attr
{
public:
	RawVoltageAttrib():Attr("RawVoltage",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~RawVoltageAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<VacuumValveSrv *>(dev))->read_RawVoltage(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<VacuumValveSrv *>(dev))->is_RawVoltage_allowed(ty);}
};

//	Attribute Pressure class definition
class PressureAttrib: public Tango::Attr
{
public:
	PressureAttrib():Attr("Pressure",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~PressureAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<VacuumValveSrv *>(dev))->read_Pressure(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<VacuumValveSrv *>(dev))->is_Pressure_allowed(ty);}
};


//=========================================
//	Define classes for commands
//=========================================
//	Command Open class definition
class OpenClass : public Tango::Command
{
public:
	OpenClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	OpenClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~OpenClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<VacuumValveSrv *>(dev))->is_Open_allowed(any);}
};

//	Command Close class definition
class CloseClass : public Tango::Command
{
public:
	CloseClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	CloseClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~CloseClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<VacuumValveSrv *>(dev))->is_Close_allowed(any);}
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
	{return (static_cast<VacuumValveSrv *>(dev))->is_Reset_allowed(any);}
};


/**
 *	The VacuumValveSrvClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  VacuumValveSrvClass : public Tango::DeviceClass
#else
class VacuumValveSrvClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(VacuumValveSrvClass::Additionnal DServer data members) ENABLED START -----*/
	
	
	/*----- PROTECTED REGION END -----*/	//	VacuumValveSrvClass::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
	
		//	Method prototypes
		static VacuumValveSrvClass *init(const char *);
		static VacuumValveSrvClass *instance();
		~VacuumValveSrvClass();
		Tango::DbDatum	get_class_property(string &);
		Tango::DbDatum	get_default_device_property(string &);
		Tango::DbDatum	get_default_class_property(string &);
	
	protected:
		VacuumValveSrvClass(string &);
		static VacuumValveSrvClass *_instance;
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

#endif   //	VacuumValveSrv_H
