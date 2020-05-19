#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Amplitude Cryostar device server

Created on Tue Nov 28 16:10:07 2017

@author: Michele Devetta <michele.devetta@mail.polimi.it>
"""

import inspect
import time
import threading
import PyTango as PT
from PyTango import server as PTS


class CryostarSrv(PTS.Device, metaclass=PTS.DeviceMeta):

    # MODBUS proxy
    proxy = PTS.device_property(dtype=str)

    # Water valve device
    water_valve = PTS.device_property(dtype=str)

    # Polling period
    polling = PTS.device_property(dtype=int, default_value=500)

    # Attributes
    temperature = PTS.attribute(
                        label="Temperature", dtype=PT.DevShort,
                        access=PT.AttrWriteType.READ, unit="degC",
                        doc="The temperature of the Ti:Saph crystal")

    delta = PTS.attribute(
                        label="Temperature delta", dtype=PT.DevShort,
                        access=PT.AttrWriteType.READ, unit="degC",
                        doc="Difference in temperature between the sides of the Ti:Saph crystal")

    pressure = PTS.attribute(
                        label="Pressure", dtype=float,
                        access=PT.AttrWriteType.READ, unit="mbar", format="%4.2e",
                        doc="Pressure in the cryostat chamber")

    pump_status = PTS.attribute(
                        label="Pump status", dtype="enum",
                        enum_labels=["OFF", "ON", "ERROR"],
                        access=PT.AttrWriteType.READ,
                        doc="The status of the vacuum pump")

    vacuum_status = PTS.attribute(
                        label="Vacuum status", dtype="enum",
                        enum_labels=["Wrong level", "Operational", "In process"],
                        access=PT.AttrWriteType.READ,
                        doc="The status of the vacuum in the cryostat chamber")

    compressor_status = PTS.attribute(
                        label="Compressor status", dtype="enum",
                        enum_labels=["OFF", "READY", "ON"],
                        access=PT.AttrWriteType.READ,
                        doc="The status of the cryostat compressor")

    temperature_status = PTS.attribute(
                        label="Temperature status", dtype="enum",
                        enum_labels=["Wrong level", "Cooling off", "In process", "Operational"],
                        access=PT.AttrWriteType.READ,
                        doc="The status of the crystal temperature")

    def init_device(self):
        """ Initialize device
        """
        self.set_state(PT.DevState.INIT)
        PTS.Device.init_device(self)

        # Service variables
        self._temperature = None
        self._temperature_ts = None
        self._delta = None
        self._delta_ts = None
        self._pressure = None
        self._pressure_ts = None
        self._pump_status = None
        self._pump_status_ts = None
        self._vacuum_status = None
        self._vacuum_status_ts = None
        self._compressor_status = None
        self._compressor_status_ts = None
        self._temperature_status = None
        self._temperature_status_ts = None
        self._water_status = PT.DevState.CLOSE

        # Connect to parent devices
        self.dev = PT.DeviceProxy(self.proxy)
        self.dev_lock = threading.Lock()
        self.water = PT.DeviceProxy(self.water_valve)
        self._check_water = None
        self.ws_id = self.water.subscribe_event("State", PT.EventType.CHANGE_EVENT, self.event_handler)

        # Enable change events
        self.temperature.set_change_event(True, False)
        self.delta.set_change_event(True, False)
        self.pressure.set_change_event(True, False)
        self.pump_status.set_change_event(True, False)
        self.vacuum_status.set_change_event(True, False)
        self.compressor_status.set_change_event(True, False)
        self.temperature_status.set_change_event(True, False)

        # Set polling time for State command
        self.poll_command("State", 500)

        # Start monitoring device
        self._terminate = False
        self.monitor = threading.Thread(target=self.monitor_loop)
        self.monitor.start()

        # Set device status
        self.set_state(PT.DevState.OFF)

    def delete_device(self):
        """ Delete device
        """
        # Terminate and join monitor thread
        self._terminate = True
        if self.monitor:
            self.monitor.join()
        self.water.unsubscribe_event(self.ws_id)

    def monitor_loop(self):
        """ Monitor loop
        """
        counter = 0
        while not self._terminate:
            s = time.time()
            if self.get_logger().is_debug_enabled():
                self.debug_stream("Staring polling loop. Block {:d}".format(counter))

            # Check water
            if self._check_water is not None and (time.time() - self._check_water) > 5.0:
                try:
                    self.stopCompressor()
                    self.error_stream("Failed to open water valve. Compressor stopped.")
                except PT.DevFailed as e:
                    self.error_stream("Failed to stop compressor. Errors: ".format(counter))
                    for i in range(len(e.args)):
                        self.error_stream("[{:d}] {:} (Origin: {:})".format(i, e.args[i].desc, e.args[i].origin))

            with self.dev_lock:
                try:
                    if counter == 0:
                        # Read instrument status
                        ans = self.dev.ReadInputRegisters( (0,4) )
                        ts = time.time()
                        if len(ans) == 4:
                            # Pump status
                            if ans[0] != self._pump_status:
                                self._pump_status = int(ans[0])
                                self._pump_status_ts = ts
                                self.push_change_event("pump_status", self._pump_status)

                            # Vacuum status
                            if ans[1] != self._vacuum_status:
                                self._vacuum_status = int(ans[1])
                                self._vacuum_status_ts = ts
                                self.push_change_event("vacuum_status", self._vacuum_status)

                            # Compressor status
                            if ans[2] != self._compressor_status:
                                self._compressor_status = int(ans[2])
                                self._compressor_status_ts = ts
                                self.push_change_event("compressor_status", self._compressor_status)

                            # Temperature status
                            if ans[3] != self._temperature_status:
                                self._temperature_status = int(ans[3])
                                self._temperature_status_ts = ts
                                self.push_change_event("temperature_status", self._temperature_status)
                        else:
                            PT.Except.throw_exception("Incomplete response", "MODBUS read returned incomplete response ({:d} instead of 4)".format(len(ans)), inspect.currentframe().f_code.co_name)

                    elif counter == 1:
                        # Read temperatures
                        ans = self.dev.ReadInputRegistersSIG16( (0,2) )
                        ts = time.time()
                        if len(ans) == 2:
                            # Temperature
                            if ans[0] != self._temperature:
                                self._temperature = ans[0]
                                self._temperature_ts = ts
                                self.push_change_event("temperature", self._temperature)

                            # Delta
                            if ans[1] != self._delta:
                                self._delta = ans[1]
                                self._delta_ts = ts
                                self.push_change_event("delta", self._delta)

                        else:
                            PT.Except.throw_exception("Incomplete response", "MODBUS read returned incomplete response ({:d} instead of 2)".format(len(ans)), inspect.currentframe().f_code.co_name)

                    elif counter == 2:
                        # Read pressure
                        ans = self.dev.ReadInputRegistersFloat( (0,1) )
                        ts = time.time()
                        if len(ans) == 1:
                            # Pressure
                            if ans[0] != self._pressure:
                                self._pressure = float(ans[0])
                                self._pressure_ts = ts
                                self.push_change_event("pressure", self._pressure)

                        else:
                            PT.Except.throw_exception("Incomplete response", "MODBUS read returned incomplete response ({:d} instead of 1)".format(len(ans)), inspect.currentframe().f_code.co_name)

                except PT.DevFailed as e:
                    self.error_stream("Polling thread failed at block {:d}. Errors: ".format(counter))
                    for i in range(len(e.args)):
                        self.error_stream("[{:d}] {:} (Origin: {:})".format(i, e.args[i].desc, e.args[i].origin))

            if self._compressor_status == 2 and self._water_status != PT.DevState.OPEN:
                # Error. Water is not open!!
                try:
                    # Try to open it
                    self.water.Open()
                    self._check_water = time.time()
                except PT.DevFailed as e:
                    self.error_stream("Failed to open water valve. Errors: ")
                    for i in range(len(e.args)):
                        self.error_stream("[{:d}] {:} (Origin: {:})".format(i, e.args[i].desc, e.args[i].origin))
                    # Stop compressor immediately
                    try:
                        self.stopCompressor()
                    except PT.DevFailed as e:
                        self.error_stream("Failed to stop compressor. Errors: ")
                        for i in range(len(e.args)):
                            self.error_stream("[{:d}] {:} (Origin: {:})".format(i, e.args[i].desc, e.args[i].origin))

            # Update status
            if self._pump_status == 0 and self._compressor_status == 0:
                 # Everything off
                self.set_state(PT.DevState.OFF)
            elif self._pump_status == 1 and self._compressor_status == 1:
                # System ready to start cooling
                self.set_state(PT.DevState.STANDBY)
            elif self._pump_status == 1 and self._compressor_status == 2 and self._temperature_status != 2:
                # System cooling down...
                self.set_state(PT.DevState.MOVING)
            elif self._pump_status == 1 and self._compressor_status == 2 and self._temperature_status == 2:
                # System reached temperature
                self.set_state(PT.DevState.ON)
            else:
                # Something wrong
                self.set_state(PT.DevState.FAULT)

            # Increment counter
            counter += 1
            if counter > 2:
                counter = 0

            # Sleep until polling period has expired
            elapsed = time.time() - s
            if self.get_logger().is_debug_enabled():
                self.debug_stream("Ended polling loop in {:4.3f} s. Block {:d}".format(elapsed, counter))
            if elapsed < self.polling / 1000.0:
                time.sleep(self.polling / 1000.0 - elapsed)

    # Event handler
    def event_handler(self, event):
        if self.get_logger().is_debug_enabled():
            self.debug_stream("Got event from {:}".format(event.attr_name))

        if not event.err:
            if event.attr_value.name.lower() == "state":
                # If the compressor is running and the valve has closed, we should open it again
                if self._compressor_status == 2 and event.attr_value.value == PT.DevState.CLOSE:
                    try:
                        self.water.Open()
                        self._check_water = time.time()
                    except PT.DevFailed as e:
                        self.error_stream("Failed to open water valve. Errors: ")
                        for i in range(len(e.args)):
                            self.error_stream("[{:d}] {:} (Origin: {:})".format(i, e.args[i].desc, e.args[i].origin))
                        # Stop compressor immediately
                        try:
                            self.stopCompressor()
                        except PT.DevFailed as e:
                            self.error_stream("Failed to stop compressor. Errors: ")
                            for i in range(len(e.args)):
                                self.error_stream("[{:d}] {:} (Origin: {:})".format(i, e.args[i].desc, e.args[i].origin))

                else:
                    if self._check_water is not None and event.attr_value.value == PT.DevState.OPEN:
                        self._check_water = None

                # Update value
                self._water_status = event.attr_value.value

            else:
                self.info_stream("Unexpected event from {:}".format(event.attr_name))

        else:
            # Event with errors
            if event.errors[0].reason == "API_EventTimeout":
                # Keep alive failed, device down
                try:
                    self.stopCompressor()
                except PT.DevFailed as e:
                    self.error_stream("Failed to stop compressor. Errors: ")
                    for i in range(len(e.args)):
                        self.error_stream("[{:d}] {:} (Origin: {:})".format(i, e.args[i].desc, e.args[i].origin))
            else:
                # Other errors
                self.error_stream("Got an event with errors from {:}. Errors:".format(event.attr_name))
                for i in range(len(event.errors)):
                    self.error_stream("[{:d}] {:} (Origin: {:})".format(i, event.errors[i].desc, event.errors[i].origin))

    # Attribute functions
    def read_temperature(self):
        if self._temperature is not None:
            return self._temperature, self._temperature_ts, PT.AttrQuality.ATTR_VALID
        else:
            return None
    def read_delta(self):
        if self._delta is not None:
            return self._delta, self._delta_ts, PT.AttrQuality.ATTR_VALID
        else:
            return None
    def read_pressure(self):
        if self._pressure is not None:
            return self._pressure, self._pressure_ts, PT.AttrQuality.ATTR_VALID
        else:
            return None
    def read_pump_status(self):
        if self._pump_status is not None:
            return self._pump_status, self._pump_status_ts, PT.AttrQuality.ATTR_VALID
        else:
            return None
    def read_vacuum_status(self):
        if self._vacuum_status is not None:
            return self._vacuum_status, self._vacuum_status_ts, PT.AttrQuality.ATTR_VALID
        else:
            return None
    def read_compressor_status(self):
        if self._compressor_status is not None:
            return self._compressor_status, self._compressor_status_ts, PT.AttrQuality.ATTR_VALID
        else:
            return None
    def read_temperature_status(self):
        if self._temperature_status is not None:
            return self._temperature_status, self._temperature_status_ts, PT.AttrQuality.ATTR_VALID
        else:
            return None

    # Commands
    @PTS.command()
    def startCompressor(self):
        """ Start cryostat compressor
        """
        if self._compressor_status == 1:
            # Check water valve. Open it if it's closed
            ws = self.water.State()
            if ws == PT.DevState.CLOSE:
                self.water.Open()
                time.sleep(0.2)
            elif ws == PT.DevState.FAULT:
                PT.Except.throw_exception("Water valve fault", "Water valve is in fault state. Cannot start compressor.", inspect.currentframe().f_code.co_name)
            # Start compressor
            with self.dev_lock:
                self.dev.ForceSingleCoilAmplitude( (0,1) ) # Start compressor
                self.dev.ForceSingleCoilAmplitude( (1,0) ) # Full heating
        elif self._compressor_status == 2:
            # Compressor already running...
            pass
        else:
            PT.Except.throw_exception("Cannot start compressor", "Cannot start compressor. Check vacuum and try again", inspect.currentframe().f_code.co_name)

    @PTS.command()
    def stopCompressor(self):
        """ Stop cryostat compressor
        """
        if self._compressor_status == 2:
            # Stop compressor
            self.dev.ForceSingleCoilAmplitude( (0,0) ) # Stop compressor
            self.dev.ForceSingleCoilAmplitude( (1,1) ) # Half heating
            # Close water valve
            th = threading.Thread(target=self.deferredWaterClose, daemon=True)
            th.start()

    def deferredWaterClose(self):
        """ Close the water valve with a delay after compressor has been switched off
        """
        time.sleep(10.0)
        self.water.Close()


if __name__ == "__main__":
    # Start device server
    try:
        PTS.run( (CryostarSrv, ) )
    except PT.DevFailed as e:
        print("Tango exception: {:}".format(e.args[0].desc))
    except Exception as e:
        print("Python exception: {:}".format(repr(e)))
    except str as e:
        print("String exception: {:}".format(e))