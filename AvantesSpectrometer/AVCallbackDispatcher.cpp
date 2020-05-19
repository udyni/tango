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

#include "AVCallbackDispatcher.h"

namespace AvantesSpectrometer_ns
{

AvsDispatcher* AvsDispatcher::_instance = NULL;

AvsDispatcher::AvsDispatcher() {
    _lock = new omni_mutex();
}

// Instance method
AvsDispatcher* AvsDispatcher::instance() {
    if(_instance == NULL) {
        _instance = new AvsDispatcher();
    }
    return _instance;
}

// Shutdown method
void AvsDispatcher::shutdown() {
    if(NULL != _instance) {
        delete _instance;
        _instance = NULL;
    }
}

// Destructor
AvsDispatcher::~AvsDispatcher() {
    if(_lock)
        delete _lock;
}

// Register new callback
void AvsDispatcher::register_callback(AvsHandle handle, AvantesSpectrometer* srv) {
    omni_mutex_lock access_lock(*(this->_lock));
    if(_storage.find(handle) != _storage.end()) {
        stringstream msg;
        msg << "Callback for handle " << handle << " was already registered. Updating." << endl;
        srv->get_logger()->warn(msg.str());
    }
    _storage[handle] = srv;
}


// Un-register callback
void AvsDispatcher::unregister_callback(AvsHandle handle) {
    omni_mutex_lock access_lock(*(this->_lock));
    if(_storage.find(handle) != _storage.end()) {
        _storage.erase(_storage.find(handle));
    }
}


// General spectrum notification callback
void AvsDispatcher::spectrum_callback(AvsHandle* handle, int* retval) {
    AvsDispatcher* self = AvsDispatcher::instance();
    omni_mutex_lock access_lock(*(self->_lock));
    if(self->_storage.find(*handle) != self->_storage.end()) {
        AvantesSpectrometer* srv = self->_storage[*handle];
        srv->spectrum_callback(handle, retval);
    } else {
        // Handle is not registered. We print an error to stdout
        std::cout << "ERROR: Failed to dispatch callback from handle " << (*handle) << endl;
    }
}


} // End of namespace
