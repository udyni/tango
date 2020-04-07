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

#include "AVSpecAttributes.h"
#include "AVCallbackDispatcher.h"
#include "AVSerializer.h"
#include <time.h>


namespace AvantesSpectrometer_ns
{


void IntegrationTimeAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    *(srv->attr_IntegrationTime_read) = srv->config.m_IntegrationTime;
    att.set_value(srv->attr_IntegrationTime_read);
}


void IntegrationTimeAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);

    struct timespec slp;
    slp.tv_sec = 0;
    slp.tv_nsec = 10000000;

    // Get write value
    Tango::DevFloat w_val;
    att.get_write_value(w_val);

    // Check if we need to change something
    if(w_val == srv->config.m_IntegrationTime)
        // No change
        return;

    int retval = 0;
    retval = AVSerializer::AVS_StopMeasure(srv->handle);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to stop measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to stop",
            msg.str(),
            (const char*)"IntegrationTimeAttrib::write()");
    }
    nanosleep(&slp, NULL);

    // Update integration time
    Tango::DevFloat old_val = srv->config.m_IntegrationTime;
    srv->config.m_IntegrationTime = w_val;

    // Configure measurement
    retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
    if(retval) {
        // Configuration failed
        TangoSys_OMemStream msg;
        msg << "Measure config failed with error " << retval;

        // Bad parameter. Try to roll back the old value
        srv->get_logger()->warn("Failed to set integration time. Rolling back.");
        srv->config.m_IntegrationTime = old_val;
        retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
        if(retval) {
            // Rollback failed
            msg << ". Rollback failed with error " << retval;
            srv->set_state(Tango::FAULT);

        } else {
            // Successfully set back the old value. Restart measurement
            retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
            if(retval) {
                // Restart failed
                srv->set_state(Tango::FAULT);
                msg << ". Failed to restart measurement after rollback with error " << retval;
            } else {
                msg << ". Restarted with old value.";
            }
        }
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
                (const char*)"Failed to configure",
                msg.str(),
                (const char*)"IntegrationTimeAttrib::write()");
    }

    // Restart measurement
    retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
    if(retval) {
        srv->set_state(Tango::FAULT);
        TangoSys_OMemStream msg;
        msg << "Failed to start measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to start",
            msg.str(),
            (const char*)"IntegrationTimeAttrib::write()");
    }

    // Send change change event
    *(srv->attr_IntegrationTime_read) = w_val;
    srv->push_change_event(att.get_name(), srv->attr_IntegrationTime_read);

    // == Update DB ==
    Tango::DbData set_prop;
    set_prop.push_back(Tango::DbDatum(srv->prop_inttime));
    set_prop[0] << w_val;
    srv->db->put_class_property("AvantesSpectrometer", set_prop);
}


void ScansToAverageAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    *(srv->attr_ScansToAverage_read) = srv->config.m_NrAverages;
    att.set_value(srv->attr_ScansToAverage_read);
}


void ScansToAverageAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);

    struct timespec slp;
    slp.tv_sec = 0;
    slp.tv_nsec = 10000000;

    // Get write value
    Tango::DevULong w_val;
    att.get_write_value(w_val);

    // Check if we need to change something
    if(w_val == srv->config.m_NrAverages)
        // No change
        return;

    int retval = 0;
    retval = AVSerializer::AVS_StopMeasure(srv->handle);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to stop measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to stop",
            msg.str(),
            (const char*)"ScansToAverageAttrib::write()");
    }
    nanosleep(&slp, NULL);

    // Update scans to average
    Tango::DevULong old_val = srv->config.m_NrAverages;
    srv->config.m_NrAverages = w_val;

    // Configure measurement
    retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
    if(retval) {
        // Configuration failed
        TangoSys_OMemStream msg;
        msg << "Measure config failed with error " << retval;

        // Bad parameter. Try to roll back the old value
        srv->get_logger()->warn("Failed to set number of averages. Rolling back.");
        srv->config.m_NrAverages = old_val;
        retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
        if(retval) {
            // Rollback failed
            msg << ". Rollback failed with error " << retval;
            srv->set_state(Tango::FAULT);

        } else {
            // Successfully set back the old value. Restart measurement
            retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
            if(retval) {
                // Restart failed
                srv->set_state(Tango::FAULT);
                msg << ". Failed to restart measurement after rollback with error " << retval;
            } else {
                msg << ". Restarted with old value.";
            }
        }
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
                (const char*)"Failed to configure",
                msg.str(),
                (const char*)"ScansToAverageAttrib::write()");
    }

    // Restart measurement
    retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
    if(retval) {
        srv->set_state(Tango::FAULT);
        TangoSys_OMemStream msg;
        msg << "Failed to start measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to start",
            msg.str(),
            (const char*)"ScansToAverageAttrib::write()");
    }

    // Send change change event
    *(srv->attr_ScansToAverage_read) = w_val;
    srv->push_change_event(att.get_name(), srv->attr_ScansToAverage_read);
}


void enableElectricalDarkCorrectionAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    *(srv->attr_enableElectricalDarkCorrection_read) = srv->config.m_CorDynDark.m_Enable;
    att.set_value(srv->attr_enableElectricalDarkCorrection_read);
}


void enableElectricalDarkCorrectionAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);

    struct timespec slp;
    slp.tv_sec = 0;
    slp.tv_nsec = 10000000;

    // Get write value
    Tango::DevBoolean w_val;
    att.get_write_value(w_val);

    // Check if we need to change something
    if(w_val == srv->config.m_CorDynDark.m_Enable)
        // No change
        return;

    int retval = 0;
    retval = AVSerializer::AVS_StopMeasure(srv->handle);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to stop measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to stop",
            msg.str(),
            (const char*)"enableElectricalDarkCorrectionAttrib::write()");
    }
    nanosleep(&slp, NULL);

    // Update dark subtraction
    Tango::DevBoolean old_val = srv->config.m_CorDynDark.m_Enable;
    srv->config.m_CorDynDark.m_Enable = w_val;

    // Configure measurement
    retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
    if(retval) {
        // Configuration failed
        TangoSys_OMemStream msg;
        msg << "Measure config failed with error " << retval;

        // Bad parameter. Try to roll back the old value
        srv->get_logger()->warn("Failed to enable dark correction. Rolling back.");
        srv->config.m_CorDynDark.m_Enable = old_val;
        retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
        if(retval) {
            // Rollback failed
            msg << ". Rollback failed with error " << retval;
            srv->set_state(Tango::FAULT);

        } else {
            // Successfully set back the old value. Restart measurement
            retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
            if(retval) {
                // Restart failed
                srv->set_state(Tango::FAULT);
                msg << ". Failed to restart measurement after rollback with error " << retval;
            } else {
                msg << ". Restarted with old value.";
            }
        }
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
                (const char*)"Failed to configure",
                msg.str(),
                (const char*)"enableElectricalDarkCorrectionAttrib::write()");
    }

    // Restart measurement
    retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
    if(retval) {
        srv->set_state(Tango::FAULT);
        TangoSys_OMemStream msg;
        msg << "Failed to start measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to start",
            msg.str(),
            (const char*)"enableElectricalDarkCorrectionAttrib::write()");
    }

    // Send change change event
    *(srv->attr_enableElectricalDarkCorrection_read) = w_val;
    srv->push_change_event(att.get_name(), srv->attr_enableElectricalDarkCorrection_read);

    // == Update DB ==
    Tango::DbData set_prop;
    set_prop.push_back(Tango::DbDatum(srv->prop_darksub));
    set_prop[0] << w_val;
    srv->db->put_class_property("AvantesSpectrometer", set_prop);
}


void BoxcarWidthAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    *(srv->attr_BoxcarWidth_read) = srv->config.m_Smoothing.m_SmoothPix * 2 + 1;
    att.set_value(srv->attr_BoxcarWidth_read);
}


void BoxcarWidthAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);

    struct timespec slp;
    slp.tv_sec = 0;
    slp.tv_nsec = 10000000;

    // Get write value
    Tango::DevULong w_val;
    att.get_write_value(w_val);
    w_val = w_val / 2;

    // Check if we need to change something
    if(w_val == srv->config.m_Smoothing.m_SmoothPix)
        // No change
        return;

    int retval = 0;
    retval = AVSerializer::AVS_StopMeasure(srv->handle);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to stop measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to stop",
            msg.str(),
            (const char*)"BoxcarWidthAttrib::write()");
    }
    nanosleep(&slp, NULL);

    // Update boxcar
    Tango::DevULong old_val = srv->config.m_Smoothing.m_SmoothPix;
    srv->config.m_Smoothing.m_SmoothPix = w_val;

    // Configure measurement
    retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
    if(retval) {
        // Configuration failed
        TangoSys_OMemStream msg;
        msg << "Measure config failed with error " << retval;

        // Bad parameter. Try to roll back the old value
        srv->get_logger()->warn("Failed to set boxcar width. Rolling back.");
        srv->config.m_Smoothing.m_SmoothPix = old_val;
        retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
        if(retval) {
            // Rollback failed
            msg << ". Rollback failed with error " << retval;
            srv->set_state(Tango::FAULT);

        } else {
            // Successfully set back the old value. Restart measurement
            retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
            if(retval) {
                // Restart failed
                srv->set_state(Tango::FAULT);
                msg << ". Failed to restart measurement after rollback with error " << retval;
            } else {
                msg << ". Restarted with old value.";
            }
        }
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
                (const char*)"Failed to configure",
                msg.str(),
                (const char*)"BoxcarWidthAttrib::write()");
    }

    // Restart measurement
    retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
    if(retval) {
        srv->set_state(Tango::FAULT);
        TangoSys_OMemStream msg;
        msg << "Failed to start measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to start",
            msg.str(),
            (const char*)"BoxcarWidthAttrib::write()");
    }

    // Send change change event
    *(srv->attr_BoxcarWidth_read) = w_val;
    srv->push_change_event(att.get_name(), srv->attr_BoxcarWidth_read);
}


void enableBackgroundSubtractionAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    att.set_value(srv->attr_enableBackgroundSubtraction_read);
}


void enableBackgroundSubtractionAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    Tango::DevBoolean w_val;
    att.get_write_value(w_val);
    *(srv->attr_enableBackgroundSubtraction_read) = w_val;
    srv->push_change_event(att.get_name(), srv->attr_enableBackgroundSubtraction_read);
}


void SpectrumAttrib::read(Tango::DeviceImpl *dev, Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
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
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    att.set_value(srv->wavelength->get_buffer(), srv->wavelength->length());
}


void EnableTECAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    *(srv->attr_enableTEC_read) = srv->dev_config.m_TecControl.m_Enable;
    att.set_value(srv->attr_enableTEC_read);
}


void EnableTECAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);

    struct timespec slp;
    slp.tv_sec = 0;
    slp.tv_nsec = 10000000;

    // Get write value
    Tango::DevBoolean w_val;
    att.get_write_value(w_val);

    // Check if we need to change something
    if(w_val == srv->dev_config.m_TecControl.m_Enable)
        // No change
        return;

    int retval = 0;
    retval = AVSerializer::AVS_StopMeasure(srv->handle);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to stop measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to stop",
            msg.str(),
            (const char*)"EnableTECAttrib::write()");
    }
    nanosleep(&slp, NULL);

    // Update TEC enable
    Tango::DevBoolean old_val = srv->dev_config.m_TecControl.m_Enable;
    srv->dev_config.m_TecControl.m_Enable = w_val;

    // Set device parameters
    retval = AVSerializer::AVS_SetParameter(srv->handle, &(srv->dev_config));
    if(retval) {
        // Configuration failed
        TangoSys_OMemStream msg;
        msg << "Device config failed with error " << retval;

        // Bad parameter. Try to roll back the old value
        srv->get_logger()->warn("Failed to enable/disable TEC. Rolling back.");
        srv->dev_config.m_TecControl.m_Enable = old_val;

        retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
        if(retval) {
            // Restart failed
            srv->set_state(Tango::FAULT);
            msg << ". Failed to restart measurement after rollback with error " << retval;
        } else {
            msg << ". Restarted with old value.";
        }
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
                (const char*)"Failed to configure",
                msg.str(),
                (const char*)"EnableTECAttrib::write()");
    }

    // Configure measurement
//     retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
//     if(retval) {
//         TangoSys_OMemStream msg;
//         msg << "Failed to configure measurement with error " << retval;
//         srv->get_logger()->error(msg.str());
//         srv->set_state(Tango::FAULT);
//         Tango::Except::throw_exception(
//             (const char*)"Failed to configure",
//             msg.str(),
//             (const char*)"EnableTECAttrib::write()");
//     }

    // Restart measurement
    retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
    if(retval) {
        srv->set_state(Tango::FAULT);
        TangoSys_OMemStream msg;
        msg << "Failed to start measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to start",
            msg.str(),
            (const char*)"EnableTECAttrib::write()");
    }

    // Send change change event
    *(srv->attr_enableTEC_read) = w_val;
    srv->push_change_event(att.get_name(), srv->attr_enableTEC_read);

    // == Update DB ==
    Tango::DbData set_prop;
    set_prop.push_back(Tango::DbDatum(srv->prop_tecenable));
    set_prop[0] << w_val;
    srv->db->put_class_property("AvantesSpectrometer", set_prop);
}


void TECSetPointAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    *(srv->attr_TECSetpoint_read) = srv->dev_config.m_TecControl.m_Setpoint;
    att.set_value(srv->attr_TECSetpoint_read);
}


void TECSetPointAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);

    struct timespec slp;
    slp.tv_sec = 0;
    slp.tv_nsec = 10000000;

    // Get write value
    Tango::DevDouble w_val;
    att.get_write_value(w_val);

    // Check if we need to change something
    if(w_val == srv->dev_config.m_TecControl.m_Enable)
        // No change
        return;

    int retval = 0;
    retval = AVSerializer::AVS_StopMeasure(srv->handle);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to stop measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        srv->set_state(Tango::FAULT);
        Tango::Except::throw_exception(
            (const char*)"Failed to stop",
            msg.str(),
            (const char*)"TECSetPointAttrib::write()");
    }
    nanosleep(&slp, NULL);

    // Update TEC enable
    Tango::DevDouble old_val = srv->dev_config.m_TecControl.m_Setpoint;
    srv->dev_config.m_TecControl.m_Setpoint = w_val;

    // Set device parameters
    retval = AVSerializer::AVS_SetParameter(srv->handle, &(srv->dev_config));
    if(retval) {
        // Configuration failed
        TangoSys_OMemStream msg;
        msg << "Device config failed with error " << retval;

        // Bad parameter. Try to roll back the old value
        srv->get_logger()->warn("Failed to set TEC setpoint. Rolling back.");
        srv->dev_config.m_TecControl.m_Setpoint = old_val;

        retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
        if(retval) {
            // Restart failed
            srv->set_state(Tango::FAULT);
            msg << ". Failed to restart measurement after rollback with error " << retval;
        } else {
            msg << ". Restarted with old value.";
        }
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
                (const char*)"Failed to configure",
                msg.str(),
                (const char*)"TECSetPointAttrib::write()");
    }

    // Configure measurement
//     retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
//     if(retval) {
//         TangoSys_OMemStream msg;
//         msg << "Failed to configure measurement with error " << retval;
//         srv->get_logger()->error(msg.str());
//         srv->set_state(Tango::FAULT);
//         Tango::Except::throw_exception(
//             (const char*)"Failed to configure",
//             msg.str(),
//             (const char*)"TECSetPointAttrib::write()");
//     }

    // Restart measurement
    retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, -1);
    if(retval) {
        srv->set_state(Tango::FAULT);
        TangoSys_OMemStream msg;
        msg << "Failed to start measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to start",
            msg.str(),
            (const char*)"TECSetPointAttrib::write()");
    }

    // Send change change event
    *(srv->attr_TECSetpoint_read) = w_val;
    srv->push_change_event(att.get_name(), srv->attr_TECSetpoint_read);

    // == Update DB ==
    Tango::DbData set_prop;
    set_prop.push_back(Tango::DbDatum(srv->prop_tecsetpoint));
    set_prop[0] << w_val;
    srv->db->put_class_property("AvantesSpectrometer", set_prop);
}


void TECTemperatureAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    // Read analog value
    float value = 0.0;
    int retval = AVSerializer::AVS_GetAnalogIn(srv->handle, 0, &value);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to read TEC temperature with error " << retval;
        srv->get_logger()->error(msg.str());
        srv->set_state(Tango::FAULT);
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"BoardTemperatureAttrib::read()");
    }
    *(srv->attr_TECTemperature_read) = srv->convert_analog_read(value, srv->dev_config.m_aTemperature[2].m_aFit, NR_TEMP_POL_COEF);
    att.set_value(srv->attr_TECTemperature_read);
}


void BoardTemperatureAttrib::read(Tango::DeviceImpl *dev, Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    // Read analog value
    float value = 0.0;
    int retval = AVSerializer::AVS_GetAnalogIn(srv->handle, 6, &value);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to read board temperature with error " << retval;
        srv->get_logger()->error(msg.str());
        srv->set_state(Tango::FAULT);
        Tango::Except::throw_exception(
            (const char*)"Attribute read failed",
            msg.str(),
            (const char*)"BoardTemperatureAttrib::read()");
    }
    *(srv->attr_boardTemperature_read) = srv->convert_analog_read(value, srv->dev_config.m_aTemperature[0].m_aFit, NR_TEMP_POL_COEF);
    att.set_value(srv->attr_boardTemperature_read);
}


void SerialNumberAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    att.set_value(srv->serial);
}


void FirmwareVersionAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    att.set_value(srv->firmwareVersion);
}


void ModelAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    att.set_value(srv->spectrometerModel);
}


void EnableTriggerAttrib::read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);
    *(srv->attr_enableTrigger_read) = (bool)(srv->config.m_Trigger.m_Mode > 0);
    att.set_value(srv->attr_enableTrigger_read);
}


void EnableTriggerAttrib::write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
    AvantesSpectrometer *srv = static_cast<AvantesSpectrometer*>(dev);

    struct timespec slp;
    slp.tv_sec = 0;
    slp.tv_nsec = 10000000;

    // Get write value
    Tango::DevBoolean w_val;
    att.get_write_value(w_val);

    // Check if we indeed need to change someting
    if(w_val && srv->config.m_Trigger.m_Mode > 0)
        // Nothing to do
        return;

    int retval = 0;
    retval = AVSerializer::AVS_StopMeasure(srv->handle);
    if(retval) {
        TangoSys_OMemStream msg;
        msg << "Failed to stop measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to stop",
            msg.str(),
            (const char*)"enableTriggerAttrib::write()");
    }
    nanosleep(&slp, NULL);

    // Update integration time
    srv->config.m_Trigger.m_Mode = (w_val) ? 2 : 0;

    if(srv->old_triggering && !w_val) {
        // Stop retriggering
        if(srv->retrigger) {
            int *ret = NULL;
            srv->retrigger->terminate();
            srv->retrigger->join((void**)&ret);
            delete ret;
            srv->retrigger = NULL;
        }
        srv->old_triggering = false;
    }

    // Configure measurement
    retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
    if(retval) {
        // Configuration failed
        if(w_val && retval == ERR_INVALID_PARAMETER) {
            // Try normal edge triggering
            srv->get_logger()->warn("Single scan triggering failed. Trying the old triggering mode.");
            srv->config.m_Trigger.m_Mode = 1;
            retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
            if(!retval) {
                // Setup old retriggering
                srv->old_triggering = true;
                srv->retrigger = new MeasureRetrigger(srv);
            }
        }

        // Still failed. Rollback!
        if(retval) {
            TangoSys_OMemStream msg;
            msg << "Measure config failed with error " << retval;

            // Bad parameter. Try to roll back the old value
            srv->get_logger()->warn("Failed to set trigger. Rolling back to no trigger.");
            srv->config.m_Trigger.m_Mode = 0;
            retval = AVSerializer::AVS_PrepareMeasure(srv->handle, &(srv->config));
            if(retval) {
                // Rollback failed
                msg << ". Rollback failed with error " << retval;
                srv->set_state(Tango::FAULT);

            } else {
                // Successfully set back the old value. Restart measurement
                retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
                if(retval) {
                    // Restart failed
                    srv->set_state(Tango::FAULT);
                    msg << ". Failed to restart measurement after rollback with error " << retval;
                } else {
                    msg << ". Restarted with old value.";
                }
            }
            srv->get_logger()->error(msg.str());
            Tango::Except::throw_exception(
                    (const char*)"Failed to configure",
                    msg.str(),
                    (const char*)"BoxcarWidthAttrib::write()");
        }
    }

    // Restart measurement
    retval = AVSerializer::AVS_Measure(srv->handle, AvsDispatcher::spectrum_callback, (srv->old_triggering) ? 1 : -1);
    if(retval) {
        srv->set_state(Tango::FAULT);
        TangoSys_OMemStream msg;
        msg << "Failed to start measurement with error " << retval;
        srv->get_logger()->error(msg.str());
        Tango::Except::throw_exception(
            (const char*)"Failed to start",
            msg.str(),
            (const char*)"enableTriggerAttrib::write()");
    }

    // Send change change event
    *(srv->attr_enableTrigger_read) = w_val;
    srv->push_change_event(att.get_name(), srv->attr_enableTrigger_read);
}


} // End of namespace
