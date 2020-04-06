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

#include "OOSrvAttributes.h"

namespace OOSpectrometer_ns
{


void IntegrationTimeAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_IntegrationTime_read) = float(WrapperOfWrapper::instance()->getIntegrationTime(srv->spec_id)) / 1000.0;
        att.set_value(srv->attr_IntegrationTime_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read integration time (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"IntegrationTimeAttrib::read()");
    }
}


void IntegrationTimeAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        Tango::DevFloat w_val;
        att.get_write_value(w_val);
        uint32_t time_micros = uint32_t(w_val * 1000.0);
        WrapperOfWrapper::instance()->setIntegrationTime(srv->spec_id, time_micros);
        *(srv->attr_IntegrationTime_read) = w_val;
        srv->push_change_event(att.get_name(), srv->attr_IntegrationTime_read);

        // == Update DB ==
        Tango::DbData set_prop;
        set_prop.push_back(Tango::DbDatum(srv->prop_inttime));
        set_prop[0] << w_val;
        srv->db->put_class_property("OOSpectrometer", set_prop);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to write integration time (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute write failed",
            msg.str(),
            (const char*)"IntegrationTimeAttrib::write()");
    }
}


void ScansToAverageAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_ScansToAverage_read) = WrapperOfWrapper::instance()->getScansToAverage(srv->spec_id);
        att.set_value(srv->attr_ScansToAverage_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read scans to average (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"ScansToAverageAttrib::read()");
    }
}


void ScansToAverageAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        Tango::DevULong w_val;
        att.get_write_value(w_val);
        WrapperOfWrapper::instance()->setScansToAverage(srv->spec_id, w_val);
        *(srv->attr_ScansToAverage_read) = w_val;
        srv->push_change_event(att.get_name(), srv->attr_ScansToAverage_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to write scans to average (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute write failed",
            msg.str(),
            (const char*)"ScansToAverageAttrib::write()");
    }
}


void enableElectricalDarkCorrectionAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_enableElectricalDarkCorrection_read) = WrapperOfWrapper::instance()->getCorrectForElectricalDark(srv->spec_id);
        att.set_value(srv->attr_enableElectricalDarkCorrection_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read electrical dark correction status (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"enableElectricalDarkCorrectionAttrib::read()");
    }
}


void enableElectricalDarkCorrectionAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        Tango::DevBoolean w_val;
        att.get_write_value(w_val);
        WrapperOfWrapper::instance()->setCorrectForElectricalDark(srv->spec_id, bool(w_val));
        *(srv->attr_enableElectricalDarkCorrection_read) = w_val;
        srv->push_change_event(att.get_name(), srv->attr_enableElectricalDarkCorrection_read);

        // == Update DB ==
        Tango::DbData set_prop;
        set_prop.push_back(Tango::DbDatum(srv->prop_darksub));
        set_prop[0] << w_val;
        srv->db->put_class_property("OOSpectrometer", set_prop);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to write electrical dark correction status (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute write failed",
            msg.str(),
            (const char*)"enableElectricalDarkCorrectionAttrib::write()");
    }
}


void BoxcarWidthAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_BoxcarWidth_read) = WrapperOfWrapper::instance()->getBoxcarWidth(srv->spec_id);
        att.set_value(srv->attr_BoxcarWidth_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read boxcar width (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"BoxcarWidthAttrib::read()");
    }
}


void BoxcarWidthAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        Tango::DevULong w_val;
        att.get_write_value(w_val);
        WrapperOfWrapper::instance()->setBoxcarWidth(srv->spec_id, w_val);
        *(srv->attr_BoxcarWidth_read) = w_val;
        srv->push_change_event(att.get_name(), srv->attr_BoxcarWidth_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to write boxcar width (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute write failed",
            msg.str(),
            (const char*)"BoxcarWidthAttrib::write()");
    }
}


void enableBackgroundSubtractionAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    att.set_value(srv->attr_enableBackgroundSubtraction_read);
}


void enableBackgroundSubtractionAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    Tango::DevBoolean w_val;
    att.get_write_value(w_val);
    *(srv->attr_enableBackgroundSubtraction_read) = w_val;
    srv->push_change_event(att.get_name(), srv->attr_enableBackgroundSubtraction_read);
}


void SpectrumAttrib::read(Tango::DeviceImpl *dev, Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    {
        // Lock spectrum
        omni_mutex_lock access_lock(*(srv->spectrum_lock));
        // Allocate buffer if needed
        if(srv->attr_Spectrum_read && srv->spectrum->length() != srv->attr_Spectrum_read_len) {
            delete[] srv->attr_Spectrum_read;
            srv->attr_Spectrum_read = NULL;
        }
        if(!srv->attr_Spectrum_read) {
            srv->attr_Spectrum_read = new double[srv->spectrum->length()];
            srv->attr_Spectrum_read_len = srv->spectrum->length();
        }
        // Copy spectrum
        memcpy(srv->attr_Spectrum_read, srv->spectrum->get_buffer(), srv->spectrum->length());
    }
    // Return spectrum
    att.set_value(srv->attr_Spectrum_read, srv->attr_Spectrum_read_len);
}


void WavelengthAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    att.set_value(srv->wavelength->get_buffer(), srv->wavelength->length());
}


void EnableTECAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_enableTEC_read) = WrapperOfWrapper::instance()->getTECEnable(srv->spec_id);
        att.set_value(srv->attr_enableTEC_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read TEC status (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"EnableTECAttrib::read()");
    }
}


void EnableTECAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        Tango::DevBoolean w_val;
        att.get_write_value(w_val);
        WrapperOfWrapper::instance()->setTECEnable(srv->spec_id, bool(w_val));
        *(srv->attr_enableTEC_read) = w_val;
        srv->push_change_event(att.get_name(), srv->attr_enableTEC_read);

        // == Update DB ==
        Tango::DbData set_prop;
        set_prop.push_back(Tango::DbDatum(srv->prop_tecenable));
        set_prop[0] << w_val;
        srv->db->put_class_property("OOSpectrometer", set_prop);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to write TEC status (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute write failed",
            msg.str(),
            (const char*)"EnableTECAttrib::write()");
    }
}


void TECSetPointAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_TECSetpoint_read) = WrapperOfWrapper::instance()->getTECSetpoint(srv->spec_id);
        att.set_value(srv->attr_TECSetpoint_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read TEC setpoint (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"TECSetPointAttrib::read()");
    }
}


void TECSetPointAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        Tango::DevDouble w_val;
        att.get_write_value(w_val);
        WrapperOfWrapper::instance()->setTECSetpoint(srv->spec_id, w_val);
        *(srv->attr_TECSetpoint_read) = w_val;
        srv->push_change_event(att.get_name(), srv->attr_TECSetpoint_read);

        // == Update DB ==
        Tango::DbData set_prop;
        set_prop.push_back(Tango::DbDatum(srv->prop_tecsetpoint));
        set_prop[0] << w_val;
        srv->db->put_class_property("OOSpectrometer", set_prop);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to write TEC setpoint (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute write failed",
            msg.str(),
            (const char*)"TECSetPointAttrib::write()");
    }
}


void TECTemperatureAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_TECTemperature_read) = WrapperOfWrapper::instance()->getTECTemperature(srv->spec_id);
        att.set_value(srv->attr_TECTemperature_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read TEC temperature (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"TECTemperatureAttrib::read()");
    }
}


void BoardTemperatureAttrib::read(Tango::DeviceImpl *dev, Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_boardTemperature_read) = WrapperOfWrapper::instance()->getBoardTemperature(srv->spec_id);
        att.set_value(srv->attr_boardTemperature_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read board temperature (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"BoardTemperatureAttrib::read()");
    }
}


void SerialNumberAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    att.set_value(srv->serial);
}


void FirmwareVersionAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    att.set_value(srv->firmwareVersion);
}


void ModelAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    att.set_value(srv->spectrometerModel);
}


void enableTriggerAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_enableTrigger_read) = WrapperOfWrapper::instance()->getEdgeTrigger(srv->spec_id);
        att.set_value(srv->attr_enableTrigger_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read trigger mode (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"enableTriggerAttrib::read()");
    }
}


void enableTriggerAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        Tango::DevBoolean w_val;
        att.get_write_value(w_val);
        WrapperOfWrapper::instance()->setEdgeTrigger(srv->spec_id, w_val);
        *(srv->attr_enableTrigger_read) = w_val;
        srv->push_change_event(att.get_name(), srv->attr_enableTrigger_read);

        // DEBUG checks
        if(srv->get_logger()->is_debug_enabled()) {
            try {
                srv->get_logger()->debug("The new trigger mode is: 0x%02X", WrapperOfWrapper::instance()->readFPGARegister(srv->spec_id, 0x2C));
            } catch(WoWException &e) {
                srv->get_logger()->debug("Error: %s", e.what());
            }
        }

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to set trigger mode (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute write failed",
            msg.str(),
            (const char*)"enableTriggerAttrib::write()");
    }
}


void TimeoutAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        *(srv->attr_timeout_read) = WrapperOfWrapper::instance()->getTimeout(srv->spec_id);
        att.set_value(srv->attr_timeout_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to read timeout (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"TimeoutAttrib::read()");
    }
}


void TimeoutAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    OOSpectrometer *srv = static_cast<OOSpectrometer*>(dev);
    try {
        Tango::DevULong w_val;
        att.get_write_value(w_val);
        WrapperOfWrapper::instance()->setTimeout(srv->spec_id, w_val);
        *(srv->attr_timeout_read) = w_val;
        srv->push_change_event(att.get_name(), srv->attr_timeout_read);

    } catch(WoWException &e) {
        TangoSys_OMemStream msg;
        msg << "Failed to write timeout (Error: " << e.what() << ")";
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Attribute write failed",
            msg.str(),
            (const char*)"TimeoutAttrib::write()");
    }
}


} // End of namespace
