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

#include <tango.h>
#include <WrapperOfWrapper.h>

int main(int argc,char *argv[])
{
    try
    {
        cout << "Starting OOSpectrometer..." << endl;

        // Full USB reset
        OOSpectrometer_ns::WrapperOfWrapper::usb_reset();

        // Initialise WoW
        OOSpectrometer_ns::WrapperOfWrapper::instance();

        // Initialise the device server
        Tango::Util *tg = Tango::Util::init(argc, argv);

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

    OOSpectrometer_ns::WrapperOfWrapper::shutdown();

    return(0);
}
