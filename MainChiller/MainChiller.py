#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Device server to control the Udyni water chiller

@author: Michele Devetta <michele.devetta@cnr.it>

"""

import time
import inspect
import threading
import PyTango as PT
from PyTango import server as PTS


class MainChiller(PTS.Device, metaclass=PTS.DeviceMeta):
    """
    Chiller monitoring server
    """

    # Systemair chiller
    chiller_proxy = PTS.device_property(dtype=str)

    # External controller
    controller_proxy = PTS.device_property(dtype=str)

    # Polling time (ms)
    polling = PTS.device_property(dtype=int, default_value=500)

    # Attributes from IR33 controller
    Temperature = PTS.attribute(
        label="Temperature", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="Temperature of the outer water circuit")

    TemperatureSetpoint = PTS.attribute(
        label="Temperature setpoint", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ_WRITE,
        doc="Temperature setpoint of the outer water circuit")

    PercentOutput = PTS.attribute(
        label="Percent output", dtype=int, unit="%", format="%d",
        access=PT.AttrWriteType.READ,
        doc="Driving level of the three-way valve")

    # Expert attributes from IR33 controller (read and write as needed)
    MaximumTemperature = PTS.attribute(
        label="Maximum temperature", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ_WRITE, display_level=PT.DispLevel.EXPERT,
        doc="Maximum temperature of the outer water circuit")
    MinimumTemperature = PTS.attribute(
        label="Temperature setpoint", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ_WRITE, display_level=PT.DispLevel.EXPERT,
        doc="Minimum temperature of the outer water circuit")
    AlarmDelta = PTS.attribute(
        label="Temperature alarm delta", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ_WRITE, display_level=PT.DispLevel.EXPERT,
        doc="Temperature alarm delta for the outer water circuit")
    EnablePID = PTS.attribute(
        label="PID Enabled", dtype=bool,
        access=PT.AttrWriteType.READ_WRITE, display_level=PT.DispLevel.EXPERT,
        doc="PID enabled status")
    PID_TI = PTS.attribute(
        label="PID TI", dtype=int, unit="", format="%d",
        access=PT.AttrWriteType.READ_WRITE, display_level=PT.DispLevel.EXPERT,
        doc="PID integral parameter")
    PID_TD = PTS.attribute(
        label="PID TD", dtype=int, unit="", format="%d",
        access=PT.AttrWriteType.READ_WRITE, display_level=PT.DispLevel.EXPERT,
        doc="PID differential parameter")
    SetpointDiff = PTS.attribute(
        label="Setpoint differential", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ_WRITE, display_level=PT.DispLevel.EXPERT,
        doc="Setpoint differential")

    # Attributes from the SystemAir chiller
    WaterReturn = PTS.attribute(
        label="Water return", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="Temperature of return water")
    WaterOut = PTS.attribute(
        label="Water out", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="Temperature of output water")
    CoilTemperature = PTS.attribute(
        label="Coil temperature", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="Coil temperature")
    ExternalTemperature = PTS.attribute(
        label="", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="External air temperature")
    EvaporationTemperature = PTS.attribute(
        label="Evaporation temperature", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="Evaporation temperature")
    SuctionTemperature = PTS.attribute(
        label="Suction temperature", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="Suction temperature")
    CondensingTemperature = PTS.attribute(
        label="Condensing temperature", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="Condensing temperature")
    DischargeTemperature = PTS.attribute(
        label="Discharge temperature", dtype=float, unit="degC", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="Discharge temperature")
    EvaporationPressure = PTS.attribute(
        label="Evaporation pressure", dtype=float, unit="bar", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="")
    CondensingPressure = PTS.attribute(
        label="Condensing pressure", dtype=float, unit="bar", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="")
    MotorCurrent = PTS.attribute(
        label="Motor current", dtype=float, unit="A", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="")
#    MotorVoltage = PTS.attribute(
#                        label="Motor voltage", dtype=int, unit="V", format="%d",
#                        access=PT.AttrWriteType.READ,
#                        doc="Motor voltage")
#    MotorFrequency = PTS.attribute(
#                        label="Motor frequency", dtype=float, unit="Hz", format="%4.1f",
#                        access=PT.AttrWriteType.READ,
#                        doc="Motor frequency")
#    RotorSpeed = PTS.attribute(
#                        label="Rotor speed", dtype=float, unit="%", format="%4.1f",
#                        access=PT.AttrWriteType.READ,
#                        doc="Rotor speed")
    RotorFrequency = PTS.attribute(
        label="Rotor frequecy", dtype=float, unit="Hz", format="%4.1f",
        access=PT.AttrWriteType.READ,
        doc="Rotor frequency")
    FanSpeed = PTS.attribute(
        label="Fan speed", dtype=int, unit="%", format="%d",
        access=PT.AttrWriteType.READ,
        doc="Fan speed")

    def init_device(self):
        """ Initialize device
        """
        self.set_state(PT.DevState.INIT)
        PTS.Device.init_device(self)

        # Recovery thread
        self._recovery_thread = None

        # Service varibles
        self._Temperature_value = None
        self._Temperature_ts = None
        self._TemperatureSetpoint_value = None
        self._TemperatureSetpoint_ts = None
        self._PercentOutput_value = None
        self._PercentOutput_ts = None
        self._MaximumTemperature_value = None
        self._MaximumTemperature_ts = None
        self._MinimumTemperature_value = None
        self._MinimumTemperature_ts = None
        self._AlarmDelta_value = None
        self._AlarmDelta_ts = None
        self._EnablePID_value = None
        self._EnablePID_ts = None
        self._PID_TI_value = None
        self._PID_TI_ts = None
        self._PID_TD_value = None
        self._PID_TD_ts = None
        self._SetpointDiff_value = None
        self._SetpointDiff_ts = None
        self._WaterReturn_value = None
        self._WaterReturn_ts = None
        self._WaterOut_value = None
        self._WaterOut_ts = None
        self._CoilTemperature_value = None
        self._CoilTemperature_ts = None
        self._ExternalTemperature_value = None
        self._ExternalTemperature_ts = None
        self._EvaporationTemperature_value = None
        self._EvaporationTemperature_ts = None
        self._SuctionTemperature_value = None
        self._SuctionTemperature_ts = None
        self._CondensingTemperature_value = None
        self._CondensingTemperature_ts = None
        self._DischargeTemperature_value = None
        self._DischargeTemperature_ts = None
        self._EvaporationPressure_value = None
        self._EvaporationPressure_ts = None
        self._CondensingPressure_value = None
        self._CondensingPressure_ts = None
        self._MotorCurrent_value = None
        self._MotorCurrent_ts = None
#        self._MotorVoltage_value = None
#        self._MotorVoltage_ts = None
#        self._MotorFrequency_value = None
#        self._MotorFrequency_ts = None
#        self._RotorSpeed_value = None
#        self._RotorSpeed_ts = None
        self._RotorFrequency_value = None
        self._RotorFrequency_ts = None
        self._FanSpeed_value = None
        self._FanSpeed_ts = None

        # Connect to parent devices
        self.chiller = PT.DeviceProxy(self.chiller_proxy)
        self.chiller_lock = threading.Lock()
        self.controller = PT.DeviceProxy(self.controller_proxy)
        self.controller_lock = threading.Lock()

        # Enable events on attributes
        self.Temperature.set_change_event(True, False)
        self.TemperatureSetpoint.set_change_event(True, False)
        self.PercentOutput.set_change_event(True, False)
        self.MaximumTemperature.set_change_event(True, False)
        self.MinimumTemperature.set_change_event(True, False)
        self.AlarmDelta.set_change_event(True, False)
        self.EnablePID.set_change_event(True, False)
        self.PID_TI.set_change_event(True, False)
        self.PID_TD.set_change_event(True, False)
        self.SetpointDiff.set_change_event(True, False)
        self.WaterReturn.set_change_event(True, False)
        self.WaterOut.set_change_event(True, False)
        self.CoilTemperature.set_change_event(True, False)
        self.ExternalTemperature.set_change_event(True, False)
        self.EvaporationTemperature.set_change_event(True, False)
        self.SuctionTemperature.set_change_event(True, False)
        self.CondensingTemperature.set_change_event(True, False)
        self.DischargeTemperature.set_change_event(True, False)
        self.EvaporationPressure.set_change_event(True, False)
        self.CondensingPressure.set_change_event(True, False)
        self.MotorCurrent.set_change_event(True, False)
#        self.MotorVoltage.set_change_event(True, False)
#        self.MotorFrequency.set_change_event(True, False)
#        self.RotorSpeed.set_change_event(True, False)
        self.RotorFrequency.set_change_event(True, False)
        self.FanSpeed.set_change_event(True, False)

        # Set polling time for State command
        self.poll_command("State", 500)

        # Start monitoring device
        self._terminate = False
        self.monitor = threading.Thread(target=self.monitor_loop)
        self.monitor.start()

        # Set device status
        self.set_state(PT.DevState.ON)

    def delete_device(self):
        """ Delete device
        """
        # Terminate and join monitor thread
        self._terminate = True
        if self.monitor:
            self.monitor.join()

    def process_parameters_holding(self, registers, attrs, factor=1, dtype=int):
        if len(registers) != len(attrs):
            raise ValueError("The number of value should match number of attributes")

        ts = time.time()
        for i, r in enumerate(registers):
            r = dtype(dtype(r) / factor)
            v = getattr(self, "_{0:s}_value".format(attrs[i]))
            if v != r:
                setattr(self, "_{0:s}_value".format(attrs[i]), r)
                self.push_change_event(attrs[i], r)
            setattr(self, "_{0:s}_ts".format(attrs[i]), ts)

    def monitor_loop(self):
        """ Monitor loop
        """
        counter = 0
        switch = 0
        errors = 0

        generic_attributes = [
            {   # 3-way control valve - percent output
                'device': 'controller',
                'holding': (227, 1),
                'attrs': ("PercentOutput", ),
                'factor': 1,
                'dtype': int,
            },
            {   # Maximum and minimum temperature
                'device': 'controller',
                'holding': (18, 3),
                'attrs': ("MinimumTemperature", "MaximumTemperature", "AlarmDelta"),
                'factor': 10.0,
                'dtype': float,
            },
            {   # Enable PID
                'device': 'controller',
                'coil': 24,
                'attrs': "EnablePID",
            },
            {   # PID TI and TD
                'device': 'controller',
                'holding': (160, 2),
                'attrs': ("PID_TI", "PID_TD"),
                'factor': 1,
                'dtype': int,
            },
            {   # Setpoint differential
                'device': 'controller',
                'holding': (5, 1),
                'attrs': ("SetpointDiff", ),
                'factor': 10.0,
                'dtype': float,
            },
            {   # Water return and water out (tP->t01 e tP->t02)
                'device': 'chiller',
                'holding': (1, 2),
                'attrs': ("WaterReturn", "WaterOut"),
                'factor': 10.0,
                'dtype': float,
            },
            {   # Coil temperature and external air (tP->t03 e tP->t04)
                'device': 'chiller',
                'holding': (5, 2),
                'attrs': ("CoilTemperature", "ExternalTemperature"),
                'factor': 10.0,
                'dtype': float,
            },
            {   # Evaporation pressure, evaporation and suction temperatures (tP->P01, None, tP->t06)
                'device': 'chiller',
                'holding': (19, 3),
                'attrs': ("EvaporationPressure", "EvaporationTemperature", "SuctionTemperature"),
                'factor': 10.0,
                'dtype': float,
            },
            {   # Condensing pressure, condensing and discharge temperatures (tP->P02, None, tP->t05)
                'device': 'chiller',
                'holding': (23, 3),
                'attrs': ("CondensingPressure", "CondensingTemperature", "DischargeTemperature"),
                'factor': 10.0,
                'dtype': float,
            },
            {   # Motor current (I/O->DCI->Cur)
                'device': 'chiller',
                'holding': (12, 1),
                'attrs': ("MotorCurrent", ),
                'factor': 10.0,
                'dtype': float,
            },
            # {   # Rotor speed and frequency (None, I/O->DCI-Spd)
            #     'device': 'chiller',
            #     'holding': (37, 2),
            #     'attrs': ("RotorSpeed", "RotorFrequency"),
            #     'factor': 10.0,
            #     'dtype': float,
            # },
            {   # Fan speed (I/O->Fan)
                'device': 'chiller',
                'holding': (134, 1),
                'attrs': ("FanSpeed", ),
                'factor': 1,
                'dtype': int,
            },
        ]

        controller_alarm = False
        controller_fault = False
        chiller_fault = False

        while not self._terminate:
            s = time.time()

            try:
                if switch == 0:
                    with self.controller_lock:
                        # Temperature and temperature setpoint
                        ans = self.controller.ReadHoldingRegisters((1, 3))
                        self.process_parameters_holding([ans[i] for i in (0, 2)], ("Temperature", "TemperatureSetpoint"), 10.0, float)

                elif switch == 1:
                    # Read controller error codes
                    controller_alarm = False
                    controller_fault = False
                    with self.controller_lock:
                        # Broken probe error
                        ans = self.controller.ReadCoilStatus(8)
                        if ans:
                            self.error_stream("Controller error: broken probe")
                            controller_fault = True

                        # Temperature errors
                        ans = self.controller.ReadMultipleCoilsStatus((11, 2))
                        if ans[0]:
                            self.error_stream("Controller alarm: high temperature")
                            controller_alarm = True
                        elif ans[1]:
                            self.error_stream("Controller alarm: low temperature")
                            controller_alarm = True

                        # Controller errors
                        ans = self.controller.ReadMultipleCoilsStatus((15, 3))
                        if ans[0]:
                            self.error_stream("Controller error: broken RTC")
                            controller_fault = True
                        elif ans[1]:
                            self.error_stream("Controller error: error in machine parametrs EEPROM")
                            controller_fault = True
                        elif ans[2]:
                            self.error_stream("Controller error: error in work parametrs EEPROM")
                            controller_fault = True

                        # Probe read error
                        ans = self.controller.ReadCoilStatus(55)
                        if ans:
                            self.error_stream("Controller error: probe read error")
                            controller_fault = True

                elif switch == 2:
                    # Read chiller error codes
                    chiller_fault = False
                    with self.chiller_lock:
                        # DCI alarm code
                        ans = self.chiller.ReadHoldingRegisters((149, 1))
                        if ans[0] != 0:
                            self.error_stream("Chiller error: DCI error code {:d}".format(ans[0]))
                            chiller_fault = True

                        # General alarm
                        ans = self.chiller.ReadCoilStatus(7)
                        if bool(ans):
                            self.error_stream("Chiller error: General alarm set")
                            chiller_fault = True

                        # LP and flow alarms
                        ans = self.chiller.ReadMultipleCoilsStatus((20, 2))
                        if bool(ans[0]):
                            self.error_stream("Chiller error: Low pressure alarm set")
                            chiller_fault = True
                        if bool(ans[1]):
                            self.error_stream("Chiller error: Flow alarm set")
                            chiller_fault = True

                        # Fan alarm
                        ans = self.chiller.ReadCoilStatus(24)
                        if bool(ans):
                            self.error_stream("Chiller error: Fan alarm set")
                            chiller_fault = True

                        # Inverter offline and inverter alarm
                        ans = self.chiller.ReadMultipleCoilsStatus((30, 2))
                        if bool(ans[0]):
                            self.error_stream("Chiller error: Inverter is offline")
                            chiller_fault = True
                        if bool(ans[1]):
                            self.error_stream("Chiller error: Inverter alarm set")
                            chiller_fault = True

                else:
                    with getattr(self, "{0:s}_lock".format(generic_attributes[counter]['device'])):
                        if 'holding' in generic_attributes[counter]:
                            ans = getattr(self, generic_attributes[counter]['device']).ReadHoldingRegisters(generic_attributes[counter]['holding'])
                            self.process_parameters_holding(
                                ans,
                                generic_attributes[counter]['attrs'],
                                generic_attributes[counter]['factor'],
                                generic_attributes[counter]['dtype'])
                        elif 'coil' in generic_attributes[counter]:
                            ans = getattr(self, generic_attributes[counter]['device']).ReadCoilStatus(generic_attributes[counter]['coil'])
                            v = getattr(self, "_{0:s}_value".format(generic_attributes[counter]['attrs']))
                            if v != bool(ans):
                                setattr(self, "_{0:s}_value".format(generic_attributes[counter]['attrs']), bool(ans))
                                self.push_change_event(generic_attributes[counter]['attrs'], bool(ans))
                                setattr(self, "_{0:s}_ts".format(generic_attributes[counter]['attrs']), time.time())

                    # Update counter
                    counter += 1
                    if counter >= len(generic_attributes):
                        counter = 0

                # Update switch
                switch += 1
                if switch > 4:
                    switch = 0

                # Update device state
                if chiller_fault or controller_fault:
                    self.set_state(PT.DevState.FAULT)
                elif self._recovery_thread is not None and self._recovery_thread.is_alive():
                    self.set_state(PT.DevState.MOVING)
                elif controller_alarm:
                    self.set_state(PT.DevState.ALARM)
                else:
                    self.set_state(PT.DevState.RUNNING)

            except PT.DevFailed as e:
                errors += 1
                if errors > 5:
                    self.error_stream("Read failed with TANGO exceptions (block={:d}):".format(counter))
                    for i in range(len(e.args)):
                        self.error_stream("[{:d}] {:} (Origin: {:})".format(i, e.args[i].desc, e.args[i].origin))
                    errors = 0
            except Exception as e:
                self.error_stream("Read failed with exception {:} (block={:d}):".format(e, counter))

            if self.get_logger().is_debug_enabled():
                self.debug_stream("Done a polling loop in {:4.3f} s (switch={:d}, counter={:d})".format(time.time() - s, switch, counter))

            # Sleep until polling period has expired
            elapsed = time.time() - s
            if elapsed < self.polling / 1000.0:
                time.sleep(self.polling / 1000.0 - elapsed)

    def read_Temperature(self):
        try:
            if self._Temperature_value is not None:
                return (self._Temperature_value, self._Temperature_ts, PT.AttrQuality.ATTR_VALID)
            else:
                return None
        except Exception as e:
            print(e)

    def read_TemperatureSetpoint(self):
        return self._TemperatureSetpoint_value

    def write_TemperatureSetpoint(self, value):
        try:
            value = int(value * 10)
            self.controller.PresetSingleRegister((3, value))
        except PT.DevFailed as e:
            self.error_stream("Failed to set setpoint (Error: {0!s})".format(e.args[0].desc))
            PT.Except.re_throw_exception(e, "Set failed", "Failed to set setpoint", "MainChiller::write_TemperatureSetpoint()")

    def read_PercentOutput(self):
        if self._PercentOutput_value is not None:
            return (self._PercentOutput_value, self._PercentOutput_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_MaximumTemperature(self):
        if self._MaximumTemperature_value is not None:
            return (self._MaximumTemperature_value, self._MaximumTemperature_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def write_MaximumTemperature(self, value):
        try:
            value = int(value * 10)
            self.controller.PresetSingleRegister((19, value))
        except PT.DevFailed as e:
            self.error_stream("Failed to set maximum temperature (Error: {0!s})".format(e.args[0].desc))
            PT.Except.re_throw_exception(e, "Set failed", "Failed to set maximum temperature", "MainChiller::write_MaximumTemperature()")

    def read_MinimumTemperature(self):
        if self._MinimumTemperature_value is not None:
            return (self._MinimumTemperature_value, self._MinimumTemperature_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def write_MinimumTemperature(self, value):
        try:
            value = int(value * 10)
            self.controller.PresetSingleRegister((18, value))
        except PT.DevFailed as e:
            self.error_stream("Failed to set minimum temperature (Error: {0!s})".format(e.args[0].desc))
            PT.Except.re_throw_exception(e, "Set failed", "Failed to set minimum temperature", "MainChiller::write_MinimumTemperature()")

    def read_AlarmDelta(self):
        if self._AlarmDelta_value is not None:
            return (self._AlarmDelta_value, self._AlarmDelta_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def write_AlarmDelta(self, value):
        try:
            value = int(value * 10)
            self.controller.PresetSingleRegister((20, value))
        except PT.DevFailed as e:
            self.error_stream("Failed to set alarm delta temperature (Error: {0!s})".format(e.args[0].desc))
            PT.Except.re_throw_exception(e, "Set failed", "Failed to set alarm delta temperature", "MainChiller::write_AlarmDelta()")

    def read_EnablePID(self):
        if self._EnablePID_value is not None:
            return (self._EnablePID_value, self._EnablePID_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def write_EnablePID(self, value):
        pass

    def read_PID_TI(self):
        if self._PID_TI_value is not None:
            return (self._PID_TI_value, self._PID_TI_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def write_PID_TI(self, value):
        try:
            value = int(value)
            self.controller.PresetSingleRegister((160, value))
        except PT.DevFailed as e:
            self.error_stream("Failed to set PID integral parameter (Error: {0!s})".format(e.args[0].desc))
            PT.Except.re_throw_exception(e, "Set failed", "Failed to set PID integral parameter", "MainChiller::write_PID_TI()")

    def read_PID_TD(self):
        if self._PID_TD_value is not None:
            return (self._PID_TD_value, self._PID_TD_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def write_PID_TD(self, value):
        try:
            value = int(value)
            self.controller.PresetSingleRegister((161, value))
        except PT.DevFailed as e:
            self.error_stream("Failed to set PID differential parameter (Error: {0!s})".format(e.args[0].desc))
            PT.Except.re_throw_exception(e, "Set failed", "Failed to set PID differential parameter", "MainChiller::write_PID_TD()")

    def read_SetpointDiff(self):
        if self._SetpointDiff_value is not None:
            return (self._SetpointDiff_value, self._SetpointDiff_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def write_SetpointDiff(self, value):
        try:
            value = int(value * 10)
            self.controller.PresetSingleRegister((5, value))
        except PT.DevFailed as e:
            self.error_stream("Failed to set setpoint differential (Error: {0!s})".format(e.args[0].desc))
            PT.Except.re_throw_exception(e, "Set failed", "Failed to set setpoint differential", "MainChiller::write_SetpointDiff()")

    def read_WaterReturn(self):
        if self._WaterReturn_value is not None:
            return (self._WaterReturn_value, self._WaterReturn_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_WaterOut(self):
        if self._WaterOut_value is not None:
            return (self._WaterOut_value, self._WaterOut_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_CoilTemperature(self):
        if self._CoilTemperature_value is not None:
            return (self._CoilTemperature_value, self._CoilTemperature_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_ExternalTemperature(self):
        if self._ExternalTemperature_value is not None:
            return (self._ExternalTemperature_value, self._ExternalTemperature_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_EvaporationTemperature(self):
        if self._EvaporationTemperature_value is not None:
            return (self._EvaporationTemperature_value, self._EvaporationTemperature_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_SuctionTemperature(self):
        if self._SuctionTemperature_value is not None:
            return (self._SuctionTemperature_value, self._SuctionTemperature_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_CondensingTemperature(self):
        if self._CondensingTemperature_value is not None:
            return (self._CondensingTemperature_value, self._CondensingTemperature_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_DischargeTemperature(self):
        if self._DischargeTemperature_value is not None:
            return (self._DischargeTemperature_value, self._DischargeTemperature_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_EvaporationPressure(self):
        if self._EvaporationPressure_value is not None:
            return (self._EvaporationPressure_value, self._EvaporationPressure_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_CondensingPressure(self):
        if self._CondensingPressure_value is not None:
            return (self._CondensingPressure_value, self._CondensingPressure_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_MotorCurrent(self):
        if self._MotorCurrent_value is not None:
            return (self._MotorCurrent_value, self._MotorCurrent_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_MotorVoltage(self):
        if self._MotorVoltage_value is not None:
            return (self._MotorVoltage_value, self._MotorVoltage_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_MotorFrequency(self):
        if self._MotorFrequency_value is not None:
            return (self._MotorFrequency_value, self._MotorFrequency_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_RotorSpeed(self):
        if self._RotorSpeed_value is not None:
            return (self._RotorSpeed_value, self._RotorSpeed_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_RotorFrequency(self):
        if self._RotorFrequency_value is not None:
            return (self._RotorFrequency_value, self._RotorFrequency_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def read_FanSpeed(self):
        if self._FanSpeed_value is not None:
            return (self._FanSpeed_value, self._FanSpeed_ts, PT.AttrQuality.ATTR_VALID)
        else:
            return None

    def recoveryProcess(self):
        self.set_state(PT.DevState.MOVING)

        # Start time
        s = time.time()

        # Get actual temperature
        start_t = self._Temperature_value

        # Get current maximum temperature
        old_max_t = self._MaximumTemperature_value

        # Increase maximum temperature
        if start_t < 40:
            max_t = start_t + 10
        else:
            max_t = 50
        self.write_MaximumTemperature(max_t)

        while True:
            # Wait for the temperature to drop to a safe level
            if self._Temperature_value < self._TemperatureSetpoint_value + 1:
                # Recovery complete. Restore maximum temperature and exit
                self.write_MaximumTemperature(old_max_t)
                break

            if time.time() - s > 1800:
                self.error_stream("Recovery failed. Temperature is still {0:.1f} after 30 minutes".format(self._Temperature_value))
                self.write_MaximumTemperature(old_max_t)

    @PTS.command()
    def startRecovery(self):
        """ Start the recovery process after a system block
        """
        if self._recovery_thread is not None:
            if self._recovery_thread.is_alive():
                PT.Except.throw_exception("Still running", "Recovery procedure is still running", "MainChiller::startRecovery()")
            else:
                self._recovery_thread.join()
                self._recovery_thread = None

        if self._Temperature_value > self._TemperatureSetpoint_value + 2:
            self._recovery_thread = threading.Thread(target=self.RecoveryProcess)
            self._recovery_thread.start()


if __name__ == "__main__":
    # Start device server
    try:
        PTS.run((MainChiller, ))
    except PT.DevFailed as e:
        print("Tango exception: {:}".format(e.args[0].desc))
    except Exception as e:
        print("Python exception: {:}".format(repr(e)))
    except str as e:
        print("String exception: {:}".format(e))
