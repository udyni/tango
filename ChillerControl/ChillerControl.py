#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 28 17:12:12 2020

@author: Michele Devetta <michele.devetta@cnr.it>
"""

import time
import math
import threading
import PyTango as PT
from PyTango import server as PTS

class ChillerControl(PTS.Device, metaclass=PTS.DeviceMeta):

    # MODBUS proxy
    proxy = PTS.device_property(dtype=str)

    # Water valve device
    water_valve = PTS.device_property(dtype=str)

    # Chiller type (lytron or polyscience)
    chiller_type = PTS.device_property(dtype=str)
    # TYPE flags
    LYTRON = 0
    POLYSCIENCE = 1

    # Polling period
    polling = PTS.device_property(dtype=int, default_value=1000)

    # Water valve state
    WaterState = PTS.attribute(
        label="Water", dtype='state',
        access=PT.AttrWriteType.READ)

    WaterFlow = PTS.attribute(
        label="Water flow", dtype=PT.DevDouble, unit="lpm", format="%.2f",
        access=PT.AttrWriteType.READ)


    def init_device(self):
        """ Initialize device
        """
        # Set INIT state
        self.set_state(PT.DevState.INIT)

        # Call parent init
        PTS.Device.init_device(self)

        # Check properties
        if self.chiller_type.lower() == "lytron":
            # Just on-off control
            self.chiller_type = self.LYTRON

        elif self.chiller_type.lower() == "polyscience":
            # High level support
            self.chiller_type = self.POLYSCIENCE

        else:
            self.fatal_stream("Found an unsupported chiller type: {0}".format(self.chiller_type))
            PT.Except.throw_exception("Bad chiller type", "Unsupported chiller type {0}".format(self.chiller_type), "ChillerControl::init_device()")

        self.proxy_evs = []
        self.water_evs = []

        # Setup proxy
        self.dev = PT.DeviceProxy(self.proxy)
        self.dev.ping()

        # Setup water valve
        self.water = PT.DeviceProxy(self.water_valve)
        self.water.ping()

        self.attr_devstate = PT.DevState.UNKNOWN
        self.attr_temperature = math.nan
        self.attr_waterstate = PT.DevState.UNKNOWN
        self.attr_waterflow = math.nan

        # Monitoring thread
        self.monitor_th = None
        self.monitor_terminate = False
        self.monitor_running = True

        # Starting state
        self.starting = 0

    def delete_device(self):
        """ Delete device. Stop grabbing and close camera if needed
        """
        try:
            if self.dev:
                for ev in self.proxy_evs:
                    self.dev.unsubscribe_event(ev)
        except PT.DevFailed as e:
            self.log_error("Error while unsubscribing from main device events (Error: {0!s})".format(e.args[0].desc))

        try:
            if self.water:
                for ev in self.water_evs:
                    self.water.unsubscribe_event(ev)
        except PT.DevFailed as e:
            self.log_error("Error while unsubscribing from main device events (Error: {0!s})".format(e.args[0].desc))

    def initialize_dynamic_attributes(self):
        """ Initialize dynamic attributes
        """
        if self.chiller_type == self.POLYSCIENCE:
            # Add temperature attribute
            attr = PT.Attr("Temperature", PT.DevDouble, PT.AttrWriteType.READ)
            attr_prop = PT.UserDefaultAttrProp()
            attr_prop.label = "Temperature"
            attr_prop.format = "%.1f"
            attr_prop.unit = "°C"
            attr.set_default_properties(attr_prop)
            attr.set_change_event(True, False)
            self.add_attribute(attr, self.read_Temperature)

            # Subscribe events
            self.dev.subscribe_event("Temperature", PT.EventType.CHANGE_EVENT, self.event_callback)
            self.dev.subscribe_event("State", PT.EventType.CHANGE_EVENT, self.event_callback)

        else:
            # Subscribe events
            self.dev.subscribe_event("State", PT.EventType.CHANGE_EVENT, self.event_callback)

        # Subscribe events from water valve
        self.water.subscribe_event("State", PT.EventType.CHANGE_EVENT, self.event_callback)
        self.water.subscribe_event("WaterFlow", PT.EventType.CHANGE_EVENT, self.event_callback)

    def read_WaterState(self):
        return self.attr_waterstate

    def read_WaterFlow(self):
        return self.attr_waterflow

    def read_Temperature(self, attr):
        attr.set_value(self.attr_temperature)

    def event_callback(self, ev):
        if ev.err:
            # Log error
            # TODO: handle event errors! E.g. heartbeat timeouts!
            self.log_error("Event error: {0!s}".format(ev.errors[0].desc))

        else:
            attr_name = ev.attr_value.name.lower()

            if ev.device.name() == self.dev.name():
                if attr_name == "state":
                    # Ignore if in fault state
                    if self.get_state() == PT.DevState.FAULT:
                        return

                    state = ev.attr_value.value
                    if state != self.attr_devstate:
                        # State changed

                        if self.attr_devstate == PT.DevState.UNKNOWN:
                            # First event, initialize device state
                            self.attr_devstate = state

                            if state == PT.DevState.FAULT:
                                self.set_state(PT.DevState.FAULT)
                            else:
                                if self.attr_waterstate != PT.DevState.UNKNOWN:
                                    # Update device state
                                    try:
                                        self.Reset()
                                    except:
                                        self.set_state(PT.DevState.FAULT)

                        else:
                            self.attr_devstate = state
                            if state in (PT.DevState.ON, PT.DevState.RUNNING):
                                if self.starting == 2:
                                    # Chiller correctly started
                                    self.set_state(PT.DevState.ON)
                                    self.starting = 0
                                    self.monitor_terminate = True

                                else:
                                    # Is water on?
                                    if self.attr_waterstate == PT.DevState.OPEN:
                                        self.warn_stream("Got chiller start event but we were not starting. Keep running.")
                                        self.set_state(PT.DevState.ON)
                                        self.starting = 0
                                    else:
                                        # Stop chiller
                                        try:
                                            self.__stop_chiller()
                                        except PT.DevFailed as e:
                                            self.set_state(PT.DevState.FAULT)
                                            self.error_stream("Failed to stop chiller after unexpected start event (Error: {0!s})".format(e.args[0].desc))
                                    if self.monitor_running:
                                        self.monitor_terminate = True

                            elif state in (PT.DevState.OFF, PT.DevState.STANDBY):
                                # Check if water is open, in case close it
                                if self.attr_waterstate == PT.DevState.OPEN:
                                    try:
                                        self.water.Close()
                                    except PT.DevFailed as e:
                                        self.error_stream("Failed to close water valve (Error: {0!s})".format(e.args[0].desc))
                                self.set_state(PT.DevState.OFF)

                            elif state == PT.DevState.ALARM:
                                # Chiller alarm!
                                self.set_state(PT.DevState.ALARM)
                                self.error_stream("Chiller alarm: {0!s}".format(self.dev.Status()))

                            elif state == PT.DevState.FAULT:
                                # Chiller fault! Switch off!
                                try:
                                    self.__stop_chiller()
                                    self.water.Close()
                                except PT.DevFailed as e:
                                    pass
                                self.error_stream("Chiller fault. Switching off.")
                                self.set_state(PT.DevState.FAULT)

                elif attr_name == "temperature":
                    if self.attr_temperature is None or self.attr_temperature != ev.attr_value.value:
                        self.attr_temperature = ev.attr_value.value
                        self.push_change_event("Temperature", self.attr_temperature)

            elif ev.device.name() == self.water.name():
                if attr_name == "state":
                    # Ignore if in fault state
                    if self.get_state() == PT.DevState.FAULT:
                        return

                    state = ev.attr_value.value
                    if state != self.attr_waterstate:
                        # State changed

                        if self.attr_waterstate == PT.DevState.UNKNOWN:
                            # First event, nothing to do
                            if state == PT.DevState.FAULT:
                                self.set_state(PT.DevState.FAULT)
                            else:
                                if self.attr_devstate != PT.DevState.UNKNOWN:
                                    # Update device state
                                    try:
                                        self.Reset()
                                    except:
                                        self.set_state(PT.DevState.FAULT)

                        else:
                            if state == PT.DevState.OPEN:
                                # Valve opened
                                if self.starting == 1:
                                    # We are starting chiller. Water is open, we can start chiller
                                    try:
                                        self.__start_chiller()
                                        self.starting = 2
                                    except PT.DevFailed as e:
                                        # Failed to start chiller
                                        try:
                                            self.water.Close()
                                        except PT.DevFailed as e:
                                            self.error_stream("Failed to close water after failing to start chiller (Error: {0!s})".format(e.args[0].desc))
                                        self.set_state(PT.DevState.FAULT)
                                else:
                                    # We are not starting. Should we close the water?
                                    if self.attr_devstate in (PT.DevState.ON, PT.DevState.ALARM):
                                        # Ignore
                                        pass
                                    else:
                                        try:
                                            self.water.Close()
                                        except PT.DevFailed as e:
                                            self.error_stream("Failed to close water after unexpected OPEN event (Error: {0!s})".format(e.args[0].desc))

                            elif state == PT.DevState.CLOSE:
                                # We were switching off?
                                if self.attr_devstate == PT.DevState.ON:
                                    # Chiller is still on, we need to open water valve again
                                    try:
                                        self.water.Open()
                                        # TODO: we should add a timeout to check for the open to succeed??
                                    except PT.DevFailed as e:
                                        self.error_stream("Failed to re-open water valve. We switch off chiller (Error: {0!s})".format(e.args[0].desc))
                                        try:
                                            self.__stop_chiller()
                                        except PT.DevFailed as e:
                                            self.error_stream("Failed to stop chiller after failing to re-open water valve. Worst case! (Error: {0!s})".format(e.args[0].desc))
                                            self.set_state(PT.DevState.FAULT)

                            elif state == PT.DevState.FAULT:
                                # Valve fault! Try a reset
                                try:
                                    self.warn_stream("Detected water valve fault. Trying a reset.")
                                    self.water.Reset()
                                    time.sleep(0.5)
                                    if self.water.State() != PT.DevState.FAULT:
                                        # Recovered!
                                        return

                                except PT.DevFailed as e:
                                    self.error_stream("Failed to reset water valve fault (Error: {0!s})".format(e.args[0].desc))

                                # If we reach this point, the reset has failed
                                if self.attr_devstate == PT.DevState.ON:
                                    # Switch off chiller if it's on
                                    self.error_stream("Cannot reset water valve fault. Switching off chiller.")
                                    try:
                                        self.__stop_chiller()
                                    except PT.DevFailed as e:
                                        self.error_stream("Failed to switch off chiller after water valve fault (Error: {0!s})".format(e.args[0].desc))
                                # Set full device as FAULTY
                                self.set_state(PT.DevState.FAULT)

                            elif state == PT.DevState.MOVING:
                                # Ignore moving state
                                return

                        self.push_change_event("WaterState", self.attr_waterstate)
                        self.attr_waterstate = ev.attr_value.value

                elif attr_name == "flow":
                    if self.attr_waterflow is None or self.attr_waterflow != ev.attr_value.value:
                        self.attr_waterflow = ev.attr_value.value
                        self.push_change_event("WaterFlow", self.attr_waterflow)

            else:
                print("ERROR: unrecognized device {0}".format(ev.device.name()))

    def monitor_loop(self):
        """ Monitor thread to check chiller start-up
        """
        self.monitor_running = True
        s = time.time()
        while not self.monitor_terminate:   # Check status for 10 seconds
            time.sleep(0.2)
            if (time.time() - s) > 10:
                # Timeout elapsed. Failed to start
                if self.attr_waterstate == PT.DevState.OPEN:
                    try:
                        self.water.Close()
                    except:
                        pass
                self.set_state(PT.DevState.FAULT)
                break

        self.monitor_running = False

    def __start_chiller(self):
        # Stop chiller
        if self.chiller_type == self.LYTRON:
            self.dev.On()

        elif self.chiller_type == self.POLYSCIENCE:
            self.dev.Start()

    def __stop_chiller(self):
        # Stop chiller
        if self.chiller_type == self.LYTRON:
            self.dev.Off()

        elif self.chiller_type == self.POLYSCIENCE:
            self.dev.Stop()

    @PTS.command()
    def Start(self):
        """ Start chiller
        """
        if self.get_state() == PT.DevState.ON:
            return

        elif self.get_state() == PT.DevState.FAULT:
            PT.Except.throw_exception("Fault", "Device in fault state. Cannot start", "ChillerControl::Start()")

        elif self.get_state() == PT.DevState.MOVING:
            # Already turning on... ignoring
            return

        self.set_state(PT.DevState.MOVING)

        try:
            # Open cooling water
            self.water.Open()
            self.starting = 1

        except PT.DevFailed as e:
            self.set_state(PT.DevState.FAULT)
            raise e

        # Start monitoring loop
        if self.monitor_th is not None:
            if self.monitor_running:
                self.monitor_terminate = True
            self.monitor_th.join()
            self.monitor_th = None
        self.monitor_terminate = False
        self.monitor_th = threading.Thread(target=self.monitor_loop)
        self.monitor_th.start()

    @PTS.command()
    def Stop(self):
        """ Stop chiller
        """
        try:
            self.__stop_chiller()
        except PT.DevFailed as e:
            # Set status as FAULT
            self.log_stream("Failed to stop chiller (Error: {0!s})".format(e.args[0].desc))
            self.set_state(PT.DevState.FAULT)

    @PTS.command()
    def Reset(self):
        """ Reset device server
        """
        try:
            # Ping devices
            self.dev.ping()
            self.water.ping()
        except PT.DevFailed as e:
            self.error_stream("[Reset] Failed to ping devices (Error: {0!s})".format(e.args[0].desc))
            raise e

        try:
            # Get updated state
            dev_state = self.dev.State()
            water_state = self.water.State()
        except PT.DevFailed as e:
            self.error_stream("[Reset] Failed to get devices state (Error: {0!s})".format(e.args[0].desc))
            raise e

        if water_state == PT.DevState.FAULT:
            try:
                self.water.Reset()
                time.sleep(0.5)
                water_state = self.water.State()
                if water_state == PT.DevState.FAULT:
                    PT.Except.throw_exception("Reset failed", "Failed to reset faulty water valve", "ChillerControl::Reset()")

            except PT.DevFailed as e:
                self.error_stream("Reset failed (Error: {0!s})".format(e.args[0].desc))
                raise e

        if dev_state == PT.DevState.FAULT:
            PT.Except.throw_exception("Reset failed", "Chiller still in fault state", "ChillerControl::Reset()")


        if water_state == PT.DevState.OPEN:
            # Water is open
            if dev_state in (PT.DevState.ON, PT.DevState.RUNNING, PT.DevState.ALARM):
                # Chiller is on
                self.attr_devstate = dev_state
                self.attr_waterstate = water_state
                self.set_state(PT.DevState.ON)

            else:
                # Chiller is off, close water
                self.water.Close()
                self.attr_devstate = dev_state
                self.attr_waterstate = water_state
                self.set_state(PT.DevState.OFF)

        else:
            # Water is close
            if dev_state in (PT.DevState.ON, PT.DevState.RUNNING, PT.DevState.ALARM):
                # Chiller is on
                # TODO: this may need some more thinking
                self.__stop_chiller()
                self.attr_devstate = dev_state
                self.attr_waterstate = water_state
                self.set_state(PT.DevState.ON)

            else:
                # Chiller is off
                self.attr_devstate = dev_state
                self.attr_waterstate = water_state
                self.set_state(PT.DevState.OFF)



if __name__ == "__main__":
    # Start device server
    try:
        PTS.run( (ChillerControl, ) )
    except PT.DevFailed as e:
        print("Tango exception: {:}".format(e.args[0].desc))
    except Exception as e:
        print("Python exception: {:}".format(repr(e)))
    except str as e:
        print("String exception: {:}".format(e))