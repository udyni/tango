//=============================================================================
//
//  This file is part of AvantesSpectrometer.
// 
//  AvantesSpectrometer is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  AvantesSpectrometer is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with AvantesSpectrometer.  If not, see <http://www.gnu.org/licenses/>.
//=============================================================================

#ifndef AVSPEC_COMMANDS_H
#define AVSPEC_COMMANDS_H

#include <tango.h>
#include <AvantesSpectrometer.h>
#include <libavs/avaspec.h>


namespace AvantesSpectrometer_ns
{


//    Command storeBackground class definition
class storeBackgroundClass : public Tango::Command
{
public:
    storeBackgroundClass(
        const char        *name,
        Tango::CmdArgType in,
        Tango::CmdArgType out,
        const char        *in_desc,
        const char        *out_desc,
        Tango::DispLevel  level)
    : Command(name, in, out, in_desc, out_desc, level) {};

    storeBackgroundClass(
        const char        *name,
        Tango::CmdArgType in,
        Tango::CmdArgType out)
    : Command(name, in, out) {};
    ~storeBackgroundClass() {};

    virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
    virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any) { return (dev->get_state() != Tango::FAULT); }
};

//    Command getSpectrum class definition
class resetClass : public Tango::Command
{
public:
    resetClass(
        const char        *name,
        Tango::CmdArgType in,
        Tango::CmdArgType out,
        const char        *in_desc,
        const char        *out_desc,
        Tango::DispLevel  level)
    : Command(name, in, out, in_desc, out_desc, level) {};

    resetClass(
        const char        *name,
        Tango::CmdArgType in,
        Tango::CmdArgType out)
    : Command(name, in, out) {};
    ~resetClass() {};

    virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
    virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any) { return true; }
};


} // End of namespace
#endif
