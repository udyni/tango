#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Picomotor8081
Device server to control a 8081 XYZθxθy Motorized Five-Axis Tilt Aligner from newport
Relies on Picomotor device server to control the motors

@author: Michele Devetta <michele.devetta@cnr.it>
"""

import time
import re
import numpy as np

import PyTango as PT
import PyTango.server as PTS


class Picomotor8081(PTS.Device, metaclass=PTS.DeviceMeta):

    # Device properties
    x1_axis = PTS.device_property(dtype=str, doc="Picomotor device for X1 axis")
    x2_axis = PTS.device_property(dtype=str, doc="Picomotor device for X2 axis")
    y1_axis = PTS.device_property(dtype=str, doc="Picomotor device for Y1 axis")
    y2_axis = PTS.device_property(dtype=str, doc="Picomotor device for Y2 axis")
    z_axis = PTS.device_property(dtype=str, doc="Picomotor device for Z axis")

    # Conversion from steps to mm
    linear_conversion = PTS.device_property(dtype=float, default_value=30e-6, doc="Conversion factor from steps to mm")
    angular_conversion = PTS.device_property(dtype=float, default_value=3e-1, doc="Conversion factor from step difference to degrees")

    # Attributes
    X = PTS.attribute(
        label="X", dtype=PT.DevDouble, format="%.4f",
        access=PT.AttrWriteType.READ_WRITE, unit="mm", doc="")

    Y = PTS.attribute(
        label="Y", dtype=PT.DevDouble, format="%.4f",
        access=PT.AttrWriteType.READ_WRITE, unit="mm", doc="")

    Z = PTS.attribute(
        label="Z", dtype=PT.DevDouble, format="%.4f",
        access=PT.AttrWriteType.READ_WRITE, unit="mm", doc="")

    thetaX = PTS.attribute(
        label="Theta X", dtype=PT.DevDouble, format="%.1f",
        access=PT.AttrWriteType.READ_WRITE, unit="urad", doc="")

    thetaY = PTS.attribute(
        label="Theta Y", dtype=PT.DevDouble, format="%.1f",
        access=PT.AttrWriteType.READ_WRITE, unit="urad", doc="")

    velocity = PTS.attribute(
        label="X", dtype=PT.DevULong, format="%d",
        access=PT.AttrWriteType.READ_WRITE, unit="step/s", memorized=True, hw_memorized=True,
        doc="")

    acceleration = PTS.attribute(
        label="X", dtype=PT.DevULong, format="%d",
        access=PT.AttrWriteType.READ_WRITE, unit="step/s^2", memorized=True, hw_memorized=True,
        doc="")

    def init_device(self):
        """ Initialize device
        """
        # Set INIT state
        self.set_state(PT.DevState.INIT)

        # Call parent init
        PTS.Device.init_device(self)

        # Enable change events
        self.X.set_change_event(True, False)
        self.Y.set_change_event(True, False)
        self.Z.set_change_event(True, False)
        self.thetaX.set_change_event(True, False)
        self.thetaY.set_change_event(True, False)
        self.velocity.set_change_event(True, False)
        self.acceleration.set_change_event(True, False)

        # Connect to axes
        try:
            self.dev_x1 = PT.DeviceProxy(self.x1_axis)
            self.dev_x1.ping()
            self.dev_x2 = PT.DeviceProxy(self.x2_axis)
            self.dev_x2.ping()
            self.dev_y1 = PT.DeviceProxy(self.y1_axis)
            self.dev_y1.ping()
            self.dev_y2 = PT.DeviceProxy(self.y2_axis)
            self.dev_y2.ping()
            self.dev_z = PT.DeviceProxy(self.z_axis)
            self.dev_z.ping()
        except PT.DevFailed as e:
            self.error_stream("Failed to connect to axis. One or more devices are unavailable (Error: {0!s})".format(e.args[0].desc))
            PT.Except.re_throw_exception(e, "Axes offlines", "Failed to connect to axis. One or more devices are unavailable", "Picomotor8081::init_device()")

        # Make velocity and acceleration uniform across devices
        v = []
        a = []
        for d in [self.dev_x1, self.dev_x2, self.dev_y1, self.dev_y2, self.dev_z]:
            v.append(d.Velocity)
            a.append(d.Acceleration)
        self._acceleration = int(np.mean(a))
        self._velocity = int(np.min(v))
        for d in [self.dev_x1, self.dev_x2, self.dev_y1, self.dev_y2, self.dev_z]:
            d.Velocity = self._velocity
            d.Acceleration = self._acceleration

        # Initialize position
        self._x1 = self.dev_x1.Position
        self._x2 = self.dev_x2.Position
        self._y1 = self.dev_y1.Position
        self._y2 = self.dev_y2.Position
        self._z = self.dev_z.Position * self.linear_conversion

        # Initialize state
        self._x1_state = self.dev_x1.State()
        self._x2_state = self.dev_x2.State()
        self._y1_state = self.dev_y1.State()
        self._y2_state = self.dev_y2.State()
        self._z_state = self.dev_z.State()

        # Call update_position
        self.update_position(True)

        # Subscribe events
        self.evid = {}
        for dev in [self.dev_x1, self.dev_x2, self.dev_y1, self.dev_y2, self.dev_z]:
            for attr in ['Position', 'State']:
                dev.subscribe_event(attr, PT.EventType.CHANGE_EVENT, self.event_callback)

        self.update_state()

    def event_callback(self, event):
        if event.err:
            # Event error
            self.error_stream("Event error ({0!s})".format(event.errors[0].desc))

        else:
            attr_name = event.attr_value.name.lower()
            self.debug_stream("Got event from attribute '{0}' of device {1}. Value: {2!s}".format(attr_name, event.device.name(), event.attr_value.value))
            if attr_name == 'position':
                if event.device.name() == self.dev_x1.name():
                    self._x1 = event.attr_value.value

                elif event.device.name() == self.dev_x2.name():
                    self._x2 = event.attr_value.value

                elif event.device.name() == self.dev_y1.name():
                    self._y1 = event.attr_value.value

                elif event.device.name() == self.dev_y2.name():
                    self._y2 = event.attr_value.value

                elif event.device.name() == self.dev_z.name():
                    new_z = event.attr_value.value * self.linear_conversion
                    if new_z != self._z:
                        self._z = new_z
                        self.push_change_event("Z", self._z)

                else:
                    # Unexpected device
                    self.error_stream("Unexpected event from device {0!s}".format(event.device.name()))
                    return

                self.update_position()
                return

            elif attr_name == 'state':
                # Update state
                if event.device.name() == self.dev_x1.name():
                    if self._x1_state == event.attr_value.value:
                        # Noting to do
                        return
                    self._x1_state = event.attr_value.value

                elif event.device.name() == self.dev_x2.name():
                    if self._x2_state == event.attr_value.value:
                        # Noting to do
                        return
                    self._x2_state = event.attr_value.value

                elif event.device.name() == self.dev_y1.name():
                    if self._y1_state == event.attr_value.value:
                        # Noting to do
                        return
                    self._y1_state = event.attr_value.value

                elif event.device.name() == self.dev_y2.name():
                    if self._y2_state == event.attr_value.value:
                        # Noting to do
                        return
                    self._y2_state = event.attr_value.value

                elif event.device.name() == self.dev_z.name():
                    if self._z_state == event.attr_value.value:
                        # Noting to do
                        return
                    self._z_state = event.attr_value.value

                else:
                    # Unexpected device
                    self.error_stream("Unexpected event from device {0!s}".format(event.device.name()))
                    return

                self.update_state()

            else:
                # Unexpected event
                self.error_stream("Unexpected event from attribute '{0!s}' of device {1!s}".format(attr_name, event.device.name()))

    def update_position(self, init=False):
        """ Update position for the values of single axes
        """
        new_x = ((self._x1 + self._x2) / 2) * self.linear_conversion
        new_tx = ((self._x1 - self._x2) / 2) * self.angular_conversion
        new_y = ((self._y1 + self._y2) / 2) * self.linear_conversion
        new_ty = ((self._y1 - self._y2) / 2) * self.angular_conversion

        if init:
            self._x = new_x
            self._tx = new_tx
            self._y = new_y
            self._ty = new_ty
        else:
            if new_x != self._x:
                self._x = new_x
                self.push_change_event("X", self._x)
            if new_tx != self._tx:
                self._tx = new_tx
                self.push_change_event("thetaX", self._tx)
            if new_y != self._y:
                self._y = new_y
                self.push_change_event("Y", self._y)
            if new_ty != self._ty:
                self._ty = new_ty
                self.push_change_event("thetaY", self._ty)

    def update_state(self):
        for s in ['x1', 'x2', 'y1', 'y2', 'z']:
            val = getattr(self, "_{0:s}_state".format(s))
            if val == PT.DevState.MOVING:
                self.set_state(PT.DevState.MOVING)
                return
        else:
            self.set_state(PT.DevState.STANDBY)

    def read_X(self):
        return self._x

    def write_X(self, w_val):
        try:
            delta = int((w_val - self._x) / self.linear_conversion)
            self.debug_stream("Moving X by {0:d}".format(delta))
            self.dev_x1.Position = self._x1 + delta
            self.dev_x2.Position = self._x2 + delta
        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Failed to set X target", e.args[0].desc, "Picomotor8081::write_X()")

    def read_Y(self):
        return self._y

    def write_Y(self, w_val):
        try:
            delta = int((w_val - self._x) / self.linear_conversion)
            self.debug_stream("Moving Y by {0:d}".format(delta))
            self.dev_y1.Position = self._y1 + delta
            self.dev_y2.Position = self._y2 + delta
        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Failed to set Y target", e.args[0].desc, "Picomotor8081::write_Y()")

    def read_Z(self):
        return self._z

    def write_Z(self, w_val):
        try:
            self.dev_z.Position = int(w_val / self.linear_conversion)
        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Failed to set Z target", e.args[0].desc, "Picomotor8081::write_Z()")

    def read_thetaX(self):
        return self._tx

    def write_thetaX(self, w_val):
        try:
            delta = int((w_val - self._tx) / self.angular_conversion)
            self.dev_x1.Position = self._x1 + delta
            self.dev_x2.Position = self._x2 - delta
        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Failed to set theta X target", e.args[0].desc, "Picomotor8081::write_thetaX()")

    def read_thetaY(self):
        return self._ty

    def write_thetaY(self, w_val):
        try:
            delta = int((w_val - self._ty) / self.angular_conversion)
            self.dev_y1.Position = self._y1 + delta
            self.dev_y2.Position = self._y2 - delta
        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Failed to set theta Y target", e.args[0].desc, "Picomotor8081::write_thetaY()")

    def read_velocity(self):
        return self._velocity

    def write_velocity(self, w_val):
        dev_list = [self.dev_x1, self.dev_x2, self.dev_y1, self.dev_y2, self.dev_z]
        try:
            for d in dev_list:
                d.Velocity = w_val
            self._velocity = w_val
            self.push_change_event("Velocity", self._velocity)
        except PT.DevFailed as e:
            # Rollback
            try:
                for d in dev_list:
                    d.Velocity = self._velocity
                PT.Except.re_throw_exception(e, "Failed to set velocity", e.args[0].desc, "Picomotor8081::write_velocity()")
            except PT.DevFailed as e:
                self.set_state(PT.DevState.FAULT)
                self.error_stream()
                PT.Except.re_throw_exception(e, "Rollback failed", e.args[0].desc, "Picomotor8081::write_velocity()")

    def read_acceleration(self):
        return self._acceleration

    def write_acceleration(self, w_val):
        dev_list = [self.dev_x1, self.dev_x2, self.dev_y1, self.dev_y2, self.dev_z]
        try:
            for d in dev_list:
                d.Acceleration = w_val
            self._acceleration = w_val
            self.push_change_event("Acceleration", self._acceleration)
        except PT.DevFailed as e:
            # Rollback
            try:
                for d in dev_list:
                    d.Acceleration = self._acceleration
                PT.Except.re_throw_exception(e, "Failed to set acceleration", e.args[0].desc, "Picomotor8081::write_acceleration()")
            except PT.DevFailed as e:
                self.set_state(PT.DevState.FAULT)
                self.error_stream()
                PT.Except.re_throw_exception(e, "Rollback failed", e.args[0].desc, "Picomotor8081::write_acceleration()")


if __name__ == "__main__":
    # Start device server
    try:
        PTS.run( (Picomotor8081, ) )
    except PT.DevFailed as e:
        print("Tango exception: {:}".format(e.args[0].desc))
    except Exception as e:
        print("Python exception: {:}".format(repr(e)))
    except str as e:
        print("String exception: {:}".format(e))