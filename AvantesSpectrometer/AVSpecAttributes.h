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

#ifndef OOSRV_ATTRIBUTES_H
#define OOSRV_ATTRIBUTES_H

#include <tango.h>
#include <AvantesSpectrometer.h>
#include <libavs/avaspec.h>


namespace AvantesSpectrometer_ns
{


//    Attribute IntegrationTime class definition
class IntegrationTimeAttrib : public Tango::Attr {
public:
    IntegrationTimeAttrib() : Attr("IntegrationTime", Tango::DEV_FLOAT, Tango::READ_WRITE) {};
    ~IntegrationTimeAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute ScansToAverage class definition
class ScansToAverageAttrib: public Tango::Attr {
public:
    ScansToAverageAttrib():Attr("ScansToAverage", Tango::DEV_ULONG, Tango::READ_WRITE) {};
    ~ScansToAverageAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute enableElectricalDarkCorrection class definition
class enableElectricalDarkCorrectionAttrib: public Tango::Attr
{
public:
    enableElectricalDarkCorrectionAttrib():Attr("enableElectricalDarkCorrection", Tango::DEV_BOOLEAN, Tango::READ_WRITE) {};
    ~enableElectricalDarkCorrectionAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute BoxcarWidth class definition
class BoxcarWidthAttrib: public Tango::Attr
{
public:
    BoxcarWidthAttrib():Attr("BoxcarWidth", Tango::DEV_ULONG, Tango::READ_WRITE) {};
    ~BoxcarWidthAttrib() {};
    virtual void read(Tango::DeviceImpl *dev, Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev, Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev, Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute enableBackgroundSubtraction class definition
class enableBackgroundSubtractionAttrib: public Tango::Attr {
public:
    enableBackgroundSubtractionAttrib():Attr("enableBackgroundSubtraction", Tango::DEV_BOOLEAN, Tango::READ_WRITE) {};
    ~enableBackgroundSubtractionAttrib() {};
    virtual void read(Tango::DeviceImpl *dev, Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev, Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev, Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute Spectrum class definition
class SpectrumAttrib: public Tango::SpectrumAttr
{
public:
    SpectrumAttrib():SpectrumAttr("Spectrum", Tango::DEV_DOUBLE, Tango::READ, 4000) {};
    ~SpectrumAttrib() {};
    virtual void read(Tango::DeviceImpl *dev, Tango::Attribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev, Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};

//    Attribute Wavelength class definition
class WavelengthAttrib: public Tango::SpectrumAttr
{
public:
    WavelengthAttrib():SpectrumAttr("Wavelength", Tango::DEV_DOUBLE, Tango::READ, 4000) {};
    ~WavelengthAttrib() {};
    virtual void read(Tango::DeviceImpl *dev, Tango::Attribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev, Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute EnableTEC class definition
class EnableTECAttrib: public Tango::Attr
{
public:
    EnableTECAttrib():Attr("EnableTEC", Tango::DEV_BOOLEAN, Tango::READ_WRITE) {};
    ~EnableTECAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute TECSetPoint class definition
class TECSetPointAttrib: public Tango::Attr
{
public:
    TECSetPointAttrib():Attr("TECSetPoint", Tango::DEV_DOUBLE, Tango::READ_WRITE) {};
    ~TECSetPointAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute TECTemperature class definition
class TECTemperatureAttrib: public Tango::Attr
{
public:
    TECTemperatureAttrib() : Attr("TECTemperature", Tango::DEV_DOUBLE, Tango::READ) {};
    ~TECTemperatureAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute BoardTemperature class definition
class BoardTemperatureAttrib: public Tango::Attr
{
public:
    BoardTemperatureAttrib() : Attr("BoardTemperature", Tango::DEV_DOUBLE, Tango::READ) {};
    ~BoardTemperatureAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute SerialNumberAttrib class definition
class SerialNumberAttrib: public Tango::Attr
{
public:
    SerialNumberAttrib() : Attr("SerialNumber", Tango::DEV_STRING, Tango::READ) {};
    ~SerialNumberAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute FirmwareVersionAttrib class definition
class FirmwareVersionAttrib: public Tango::Attr
{
public:
    FirmwareVersionAttrib() : Attr("FirmwareVersion", Tango::DEV_STRING, Tango::READ) {};
    ~FirmwareVersionAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute ModelAttrib class definition
class ModelAttrib: public Tango::Attr
{
public:
    ModelAttrib() : Attr("Model", Tango::DEV_STRING, Tango::READ) {};
    ~ModelAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//     Attribute enableTrigger
class EnableTriggerAttrib: public Tango::Attr
{
public:
    EnableTriggerAttrib():Attr("enableTrigger", Tango::DEV_BOOLEAN, Tango::READ_WRITE) {};
    ~EnableTriggerAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};


//    Attribute enableNLCorrection class definition
class enableNLCorrectionAttrib: public Tango::Attr
{
public:
    enableNLCorrectionAttrib() : Attr("enableNLCorrection", Tango::DEV_BOOLEAN, Tango::READ_WRITE) {};
    ~enableNLCorrectionAttrib() {};
    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty) { return (dev->get_state() != Tango::FAULT); }
};

} // End of namespace
#endif
