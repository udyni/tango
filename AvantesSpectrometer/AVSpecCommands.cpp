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

#include "AVSpecCommands.h"
#include <AVSerializer.h>
#include <AVCallbackDispatcher.h>

namespace AvantesSpectrometer_ns
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
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer *>(device);
    srv->updateBackground = true;
    return new CORBA::Any();
}

CORBA::Any *resetClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any)) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer *>(device);

    // Reset acquisition
    int retval = 0;
    struct timespec slp;
    slp.tv_sec = 0;
    slp.tv_nsec = 10000000;

    // Stop measurement
    AVSerializer::AVS_StopMeasure(srv->handle);
    nanosleep(&slp, NULL);

    // Check that external trigger is off
    srv->config.m_Trigger.m_Mode = 0,
    srv->config.m_Trigger.m_Source = 0,
    srv->config.m_Trigger.m_SourceType = 0;

    // Configure measurement
    retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to configure measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        srv->set_state(Tango::FAULT);
        Tango::Except::throw_exception(
            (const char*)"Failed to configure",
            msg.str(),
            (const char*)"resetClass::execute()");
    }

    // Restart measurement
    retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, -1);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to start measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        srv->set_state(Tango::FAULT);
        Tango::Except::throw_exception(
            (const char*)"Failed to start",
            msg.str(),
            (const char*)"resetClass::execute()");
    }

    // Restart done
    srv->set_state(Tango::ON);
    return new CORBA::Any();
}


} // End of namespace
