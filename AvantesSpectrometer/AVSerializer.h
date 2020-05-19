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

#ifndef AVSPEC_SERIALIZER_H
#define AVSPEC_SERIALIZER_H

#include <tango.h>
#include <libavs/avaspec.h>
#include <sstream>
#include <iostream>


namespace AvantesSpectrometer_ns
{

class AVSerializer {
private:
    // Private constructor
    AVSerializer();
    // Private copy constructor and assignment operator
    AVSerializer(const AVSerializer& obj);
    AVSerializer& operator=(AVSerializer& obj);

public:
    // Instance method
    static AVSerializer* instance();

    // Shutdown method
    static void shutdown();

    // Destructor
    ~AVSerializer();

    // AVS library interface
    static int AVS_UpdateUSBDevices();
    static int AVS_GetList(unsigned int a_ListSize, unsigned int* a_pRequiredSize, AvsIdentityType* a_pList);
    static AvsHandle AVS_Activate(AvsIdentityType* a_pDeviceId);
    static bool AVS_Deactivate(AvsHandle a_hDevice);
    static int AVS_GetVersionInfo(AvsHandle a_hDevice, char* a_pFPGAVersion, char* a_pFirmwareVersion, char* a_pDLLVersion);
    static int AVS_GetParameter(AvsHandle a_hDevice, unsigned int a_Size, unsigned int* a_pRequiredSize, DeviceConfigType* a_pDeviceParm);
    static int AVS_SetParameter(AvsHandle a_hDevice, DeviceConfigType* a_pDeviceParm);
    static int AVS_GetLambda(AvsHandle a_hDevice, double* a_pWaveLength);
    static int AVS_GetNumPixels(AvsHandle a_hDevice, unsigned short* a_pNumPixels);
    static int AVS_GetAnalogIn(AvsHandle a_hDevice, unsigned char a_AnalogInId, float* a_pAnalogIn);
    static int AVS_PrepareMeasure(AvsHandle a_hDevice, MeasConfigType* a_pMeasConfig);
    static int AVS_Measure(AvsHandle a_hDevice, void (*__Done)(AvsHandle*, int*), short a_Nmsr);
    static int AVS_GetScopeData(AvsHandle a_hDevice, unsigned int* a_pTimeLabel, double* a_pSpectrum);
    static int AVS_StopMeasure(AvsHandle a_hDevice);

private:
    // Lock
    omni_mutex* _lock;

    // Instance
    static AVSerializer* _instance;
};


} // End of namespace
#endif
