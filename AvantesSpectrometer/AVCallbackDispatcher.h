// kate: replace-tabs on; indent-width 4; indent-mode cstyle;
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

#ifndef AVSPEC_CALLBACKDISPATCHER_H
#define AVSPEC_CALLBACKDISPATCHER_H

#include <tango.h>
#include <libavs/avaspec.h>
#include <sstream>
#include <iostream>
#include <AvantesSpectrometer.h>


namespace AvantesSpectrometer_ns
{

class AvsDispatcher {
private:
    // Private constructor
    AvsDispatcher();
    // Private copy constructor and assignment operator
    AvsDispatcher(const AvsDispatcher& obj);
    AvsDispatcher& operator=(AvsDispatcher& obj);

public:
    // Instance method
    static AvsDispatcher* instance();

    // Shutdown method
    static void shutdown();

    // Destructor
    ~AvsDispatcher();

    // Register new callback
    void register_callback(AvsHandle handle, AvantesSpectrometer* srv);

    // Un-register callback
    void unregister_callback(AvsHandle handle);

    // General spectrum notification callback
    static void spectrum_callback(AvsHandle* handle, int* retval);

private:
    // Lock
    omni_mutex* _lock;

    // Instance
    static AvsDispatcher* _instance;

    // Storage
    std::map<AvsHandle, AvantesSpectrometer*> _storage;
};


} // End of namespace
#endif
