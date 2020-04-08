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

#include <tango.h>
#include <libavs/avaspec.h>
#include <AVCallbackDispatcher.h>
#include <AVSerializer.h>

int main(int argc,char *argv[])
{
    try
    {
        cout << "Starting AvantesSpectrometer..." << endl;

        // Initialise the device server
        Tango::Util *tg = Tango::Util::init(argc, argv);

        // Initialise AvaSpec library
        AvantesSpectrometer_ns::AVSerializer::instance();

        // Create the device server singleton which will create everything
        tg->server_init(false);

        // Run the endless loop
        cout << "Ready to accept request" << endl;
        tg->server_run();
    }
    catch (bad_alloc &)
    {
        cout << "Can't allocate memory to store device object !!!" << endl;
        cout << "Exiting" << endl;
    }
    catch (CORBA::Exception &e)
    {
        Tango::Except::print_exception(e);

        cout << "Received a CORBA_Exception" << endl;
        cout << "Exiting" << endl;
    }

    Tango::Util::instance()->server_cleanup();

    // Shutdown callback dispatcher
    AvantesSpectrometer_ns::AvsDispatcher::shutdown();

    // AvaSpec library cleanup
    AvantesSpectrometer_ns::AVSerializer::shutdown();

    return(0);
}
