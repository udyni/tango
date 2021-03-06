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
#include <OOSpectrometerClass.h>

/**
 *    Create OOSpectrometer Class singleton and store it in DServer object.
 */

void Tango::DServer::class_factory()
{
    //    Add method class init if needed
    add_class(OOSpectrometer_ns::OOSpectrometerClass::init("OOSpectrometer"));
}
