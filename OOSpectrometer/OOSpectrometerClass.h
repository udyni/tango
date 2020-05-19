// kate: replace-tabs on; indent-width 4; indent-mode cstyle;
//=============================================================================
//
//  This file is part of OOSpectrometer.
//
//  Foobar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Foobar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//=============================================================================

#ifndef OOSpectrometerClass_H
#define OOSpectrometerClass_H

#include <tango.h>
#include <logging.h>
#include <OOSpectrometer.h>
#include <OOSrvAttributes.h>
#include <OOSrvCommands.h>


namespace OOSpectrometer_ns
{

/**
 *    The OOSpectrometerClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  OOSpectrometerClass : public Tango::DeviceClass {
#else
class OOSpectrometerClass : public Tango::DeviceClass {
#endif
public:
    //    write class properties data members
    Tango::DbData    cl_prop;
    Tango::DbData    cl_def_prop;
    Tango::DbData    dev_def_prop;

    //    Method prototypes
    static OOSpectrometerClass *init(const char *);
    static OOSpectrometerClass *instance();
    ~OOSpectrometerClass();
    Tango::DbDatum    get_class_property(string &);
    Tango::DbDatum    get_default_device_property(string &);
    Tango::DbDatum    get_default_class_property(string &);

protected:
    OOSpectrometerClass(string &);
    static OOSpectrometerClass *_instance;
    void command_factory();
    void attribute_factory(vector<Tango::Attr *> &);
    void write_class_property();
    void set_default_property();
    void get_class_property();

private:
    void device_factory(const Tango::DevVarStringArray *);
    void create_static_attribute_list(vector<Tango::Attr *> &);
    void erase_dynamic_attributes(const Tango::DevVarStringArray *,vector<Tango::Attr *> &);
    vector<string>    defaultAttList;
    Tango::Attr *get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname);

    // Logger
    log4tango::Logger *get_logger() { return Tango::Logging::get_core_logger(); }
};

}    //    End of namespace

#endif   //    OOSpectrometer_H
