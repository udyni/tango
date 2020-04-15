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

#include "OOSrvCommands.h"

namespace OOSpectrometer_ns
{

//--------------------------------------------------------
/**
 * method :         storeBackgroundClass::execute()
 * description :     method to trigger the execution of the command.
 *
 * @param    device    The device on which the command must be executed
 * @param    in_any    The command input data
 *
 *    returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *storeBackgroundClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any)) {
    OOSpectrometer *srv = static_cast<OOSpectrometer *>(device);
    srv->updateBackground = true;
    return new CORBA::Any();
}

CORBA::Any *resetClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any)) {
    // Nothing to do...
    return new CORBA::Any();
}

} // End of namespace
