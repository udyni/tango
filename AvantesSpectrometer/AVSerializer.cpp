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

#include "AVSerializer.h"

namespace AvantesSpectrometer_ns
{

AVSerializer* AVSerializer::_instance = NULL;

AVSerializer::AVSerializer() {
    _lock = new omni_mutex();
    ::AVS_Init(0);
}

// Instance method
AVSerializer* AVSerializer::instance() {
    if(_instance == NULL) {
        _instance = new AVSerializer();
    }
    return _instance;
}

// Shutdown method
void AVSerializer::shutdown() {
    if(NULL != _instance) {
        delete _instance;
        _instance = NULL;
    }
}

// Destructor
AVSerializer::~AVSerializer() {
    ::AVS_Done();
    if(_lock)
        delete _lock;
}


int AVSerializer::AVS_UpdateUSBDevices() {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_UpdateUSBDevices();
}

int AVSerializer::AVS_GetList(unsigned int a_ListSize, unsigned int* a_pRequiredSize, AvsIdentityType* a_pList) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_GetList(a_ListSize, a_pRequiredSize, a_pList);
}

AvsHandle AVSerializer::AVS_Activate(AvsIdentityType* a_pDeviceId) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_Activate(a_pDeviceId);
}

bool AVSerializer::AVS_Deactivate(AvsHandle a_hDevice) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_Deactivate(a_hDevice);
}

int AVSerializer::AVS_GetVersionInfo(AvsHandle a_hDevice, char* a_pFPGAVersion, char* a_pFirmwareVersion, char* a_pDLLVersion) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_GetVersionInfo(a_hDevice, a_pFPGAVersion, a_pFirmwareVersion, a_pDLLVersion);
}

int AVSerializer::AVS_GetParameter(AvsHandle a_hDevice, unsigned int a_Size, unsigned int* a_pRequiredSize, DeviceConfigType* a_pDeviceParm) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_GetParameter(a_hDevice, a_Size, a_pRequiredSize, a_pDeviceParm);
}

int AVSerializer::AVS_SetParameter(AvsHandle a_hDevice, DeviceConfigType* a_pDeviceParm) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_SetParameter(a_hDevice, a_pDeviceParm);
}

int AVSerializer::AVS_GetLambda(AvsHandle a_hDevice, double* a_pWaveLength) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_GetLambda(a_hDevice, a_pWaveLength);
}

int AVSerializer::AVS_GetNumPixels(AvsHandle a_hDevice, unsigned short* a_pNumPixels) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_GetNumPixels(a_hDevice, a_pNumPixels);
}

int AVSerializer::AVS_GetAnalogIn(AvsHandle a_hDevice, unsigned char a_AnalogInId, float* a_pAnalogIn) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_GetAnalogIn(a_hDevice, a_AnalogInId, a_pAnalogIn);
}

int AVSerializer::AVS_PrepareMeasure(AvsHandle a_hDevice, MeasConfigType* a_pMeasConfig) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_PrepareMeasure(a_hDevice, a_pMeasConfig);
}

int AVSerializer::AVS_Measure(AvsHandle a_hDevice, void (*__Done)(AvsHandle*, int*), short a_Nmsr) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_Measure(a_hDevice, __Done, a_Nmsr);
}

int AVSerializer::AVS_GetScopeData(AvsHandle a_hDevice, unsigned int* a_pTimeLabel, double* a_pSpectrum) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_GetScopeData(a_hDevice, a_pTimeLabel, a_pSpectrum);
}

int AVSerializer::AVS_StopMeasure(AvsHandle a_hDevice) {
    omni_mutex_lock access_lock(*(AVSerializer::instance()->_lock));
    return ::AVS_StopMeasure(a_hDevice);
}


} // End of namespace
