#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jan  8 14:49:24 2021

Controller interface for a SmarAct axis

NOTE: this version only support the ethernet version

@author: Michele Devetta (michele.devetta@cnr.it)
"""

import time
import math
import numpy as np
import threading
import PyTango as PT
import PyTango.server as PTS
from SmarActUtils import MoveMode, CalibrationOptions, ReferencingOptions, ActuatorMode, AxisState, PosMovementType, SensorType, BaseUnit


class SmarActPositioner(PTS.Device, metaclass=PTS.DeviceMeta):

    # proxy = PTS.device_property(dtype=str, doc="MCS2 proxy device", mandatory=True)
    # axis = PTS.device_property(dtype=PT.DevShort, doc="MCS2 axis number", mandatory=True)
    # autoreference = PTS.device_property(dtype=PT.DevBoolean, doc="Automatically reference upon startup", mandatory=False, default_value=False)
    # polling = PTS.device_property(dtype=PT.DevULong, doc="Polling period in ms", mandatory=False, default_value=500)
    proxy = PTS.device_property(dtype=str, doc="MCS2 proxy device")
    axis = PTS.device_property(dtype=PT.DevShort, doc="MCS2 axis number")
    autoreference = PTS.device_property(dtype=PT.DevBoolean, doc="Automatically reference upon startup", default_value=False)
    polling = PTS.device_property(dtype=PT.DevULong, doc="Polling period in ms", default_value=500)
    ref_reverse = PTS.device_property(dtype=PT.DevBoolean, doc="Invert referencing direction", default_value=False)
    ref_movebefore = PTS.device_property(dtype=PT.DevDouble, doc="Relative move before starting referencing", default_value=0.0)
    no_direct_control = PTS.device_property(dtype=PT.DevBoolean, doc="Forbid direct control with SmaractGUI", default_value=False)

#    MoveMode = PTS.attribute(
#                        label="Move mode", dtype=PT.DevEnum, enum_labels=MoveMode.getEnum(),
#                        access=PT.AttrWriteType.READ_WRITE, doc="Move mode", fisallowed='IsProxyOnline')
#
#    ActuatorMode = PTS.attribute(
#                        label="Actuator mode", dtype=PT.DevEnum, enum_labels=ActuatorMode.getEnum(),
#                        access=PT.AttrWriteType.READ_WRITE, doc="Actuator mode", fisallowed='IsProxyOnline')

    def init_device(self):
        """ Initialize device
        """
        # Set INIT state
        self.set_state(PT.DevState.INIT)

        # Call parent init
        PTS.Device.init_device(self)

        # Work variables
        self._do_reference = False

        # Create proxy
        self.dev = PT.DeviceProxy(self.proxy)
        self.debug_stream("Proxy device {0} up and running".format(self.dev.name()))

        try:
            self.initialize_device()
            self.set_state(PT.DevState.STANDBY)
        except PT.DevFailed as e:
            self.error_stream("Failed device init (Error: {0!s})".format(e.args[0].desc))
            self.set_state(PT.DevState.FAULT)

        # Enable events on attributes
        self.Position.set_change_event(True, False)
        self.Velocity.set_change_event(True, False)
        self.Acceleration.set_change_event(True, False)
        self.MotionDone.set_change_event(True, False)
        self.IsCalibrated.set_change_event(True, False)
        self.IsReferenced.set_change_event(True, False)

        # Set polling time for State command
        self.poll_command("State", 500)
        self.debug_stream("Configured events")

        # Start polling thread
        self._terminate = False
        self._th = threading.Thread(target=self.PollingThread)
        self._th.start()

    def initialize_device(self):

        st = AxisState(self.dev.AxisState(self.axis))
        self.debug_stream("Axis {0:d} state is: {1:d}".format(self.axis, st.getValue()))

        # Set move mode to absolute closed loop (the only mode supported yet)
        self.dev.SetMoveMode(np.array([self.axis, MoveMode.SA_CTL_MOVE_MODE_CL_ABSOLUTE], dtype=np.int32))

        # Set proper unit and conversion for position
        unit = BaseUnit(self.dev.GetBaseUnit(self.axis))
        resolution = self.dev.GetBaseResolution(self.axis)

        pos_pp = self.Position.get_properties()
        vel_pp = self.Velocity.get_properties()
        acc_pp = self.Acceleration.get_properties()
        if unit.isMeter():
            pos_pp.unit = "um"
            pos_pp.format = "%.3f"
            vel_pp.unit = "um/s"
            vel_pp.format = "%.3f"
            acc_pp.unit = "um/s^2"
            acc_pp.format = "%.3f"
            self._conv_factor = math.pow(10.0, resolution + 6)

        elif unit.isDegree():
            pos_pp.unit = "deg"
            pos_pp.format = "%.6f"
            vel_pp.unit = "deg/s"
            vel_pp.format = "%.6f"
            acc_pp.unit = "deg/s^2"
            acc_pp.format = "%.6f"
            self._conv_factor = math.pow(10.0, resolution)

        else:
            pos_pp.unit = "n.a."
            vel_pp.unit = "n.a."
            acc_pp.unit = "n.a."
            self._conv_factor = 1.0

        self.Position.set_properties(pos_pp)
        self.Velocity.set_properties(vel_pp)
        self.Acceleration.set_properties(acc_pp)
        self.debug_stream("Configured units. Conversion factor is: {0:e}".format(self._conv_factor))

        # Variable to store attributes
        self._pos = self.dev.GetPosition(self.axis) * self._conv_factor
        self._vel = self.dev.GetVelocity(self.axis) * self._conv_factor
        self._acc = self.dev.GetAcceleration(self.axis) * self._conv_factor
        self._mdone = not st.is_moving()
        self._iscal = st.is_calibrated()
        self._isref = st.is_referenced()
        self.debug_stream("Initialized attributes")

        # Check if device is calibrated
        if not st.is_calibrated():
            self.debug_stream("Positioner not calibrated on INTI. Start calibration")
            self.Calibrate()

        if self.autoreference and not st.is_referenced():
            self.Reference()

    def delete_device(self):
        """ Delete device
        """
        try:
            self._terminate = True
            if self._th is not None:
                self._th.join()
        except Exception as e:
            self.debug_stream("Error: {0!s}".format(e))

    def initialize_dynamic_attributes(self):
        """ Initialize dynamic attributes
        """
        pass

    def command_allowed(self, attr=None):
        """ Check device state
        """
        if self.get_state() == PT.DevState.FAULT:
            if attr is None:
                PT.Except.throw_exception("Fault state", "Command not allowed because device is in fault state", "SmarActPositioner::command_allowed()")
            else:
                return False
        else:
            return True

    def ConvertPosition(self, value):
        return float(value) * self._conv_factor

    def ConvertVelocity(self, value):
        return float(value) * self._conv_factor

    def ConvertAcceleration(self, value):
        return float(value) * self._conv_factor

    @PTS.command(dtype_in=None, doc_in='', dtype_out=None, doc_out='')
    def Stop(self):
        """ Stop movement
        """
        self.command_allowed()
        self.dev.Stop(self.axis)

    @PTS.command(dtype_in=None, doc_in='', dtype_out=None, doc_out='')
    def Calibrate(self):
        """ Calibration routine
        """
        self.command_allowed()
        self.debug_stream("Starting calibration")
        try:
            # Check calibration options
            opt = CalibrationOptions(self.dev.GetCalibrationOptions(self.axis))
            self.debug_stream("Current calibration options: {0:d}".format(opt.getValue()))

            ref = SensorType(self.dev.GetSensorReferenceType(self.axis))
            self.debug_stream("Reference type: {0:d}".format(opt.getValue()))

            # Check if positioner has multiple reference mark
            if ref.isDistanceCoded():
                opt.setDetectCodeInversion(True)
            else:
                opt.setDetectCodeInversion(False)

            # Enable advanced sensor correction
            # TODO: should check that the positioner is Stick-Slip Piezo
            opt.setAdvancedSensorCorrection(True)
            self.debug_stream("New calibration options: {0:d}".format(opt.getValue()))
            self.dev.SetCalibrationOptions(np.array([self.axis, opt.getValue()], dtype=np.int32))

            # Start calibration
            self.dev.Calibrate(self.axis)

        except PT.DevFailed as e:
            self.error_stream("Calibration failed (Error: {0!s})".format(e.args[0].desc))
            PT.Except.re_throw_exception(e, "Calibration failed", "Cablibration failed", "SmarActPositioner::Calibrate()")
        except Exception as e:
            self.error_stream("Calibration failed (Error: {0!s})".format(e))
            PT.Except.throw_exception(e, "Calibration failed", "Cablibration failed (Error: {0!s})".format(e), "SmarActPositioner::Calibrate()")

    @PTS.command(dtype_in=None, doc_in='', dtype_out=None, doc_out='')
    def Reference(self):
        """ Referencing routine
        """
        self.command_allowed()
        self.debug_stream("Booking reference move")
        self._do_reference = True

    def DoReference(self):
        # Check referencing options
        try:
            # Relative move before referencing
            if self.ref_movebefore != 0.0:
                pos = self.dev.GetPosition(self.axis)
                new_pos = pos + int(self.ref_movebefore / self.conv_factor)
                self.dev.Move(np.array([self.axis, new_pos], np.int64))
                self.waitForMotionDone()

            opt = ReferencingOptions(self.dev.GetReferencingOptions(self.axis))
            ref = SensorType(self.dev.GetSensorReferenceType(self.axis))
            self.debug_stream("Reference type: {0:d}".format(ref.getValue()))

            if ref.isDistanceCoded():
                opt.setReverseDirection(True)
            else:
                opt.setReverseDirection(self.ref_reverse)

            self.dev.SetReferencingOptions(np.array([self.axis, opt.getValue()], dtype=np.int32))
            self.debug_stream("Referencing options: {0:d}".format(opt.getValue()))

            # Start referencing
            self.dev.Reference(self.axis)
            self.waitForMotionDone()

        except PT.DevFailed as e:
            self.error_stream("Referencing failed (Error: {0!s})".format(e.args[0].desc))
        except Exception as e:
            self.error_stream("Referencing failed (Error: {0!s})".format(e))

        self._do_reference = False

    def waitForMotionDone(self):
        while True:
            time.sleep(0.2)
            st = AxisState(self.dev.AxisState(self.axis))
            if st.is_moving() or st.is_calibrating() or st.is_referencing():
                continue
            else:
                break

    def PollingThread(self):

        self.debug_stream("Polling thread started")
        polling = float(self.polling) / 1000.0

        count = 0
        while not self._terminate:
            s = time.time()

            if self.get_state() != PT.DevState.FAULT:
                # Poll only if state is not FAULT

                # Handle referencing
                if self._do_reference:
                    self.DoReference()

                # Poll state
                try:
                    st = AxisState(self.dev.AxisState(self.axis))

                    if self.get_logger().is_debug_enabled():
                        self.debug_stream("Polled state: {0:d} ({1!s})".format(st.getValue(), st))

                    new_state = None
                    if st.is_moving() or st.is_calibrating() or st.is_referencing():
                        if self._mdone:
                            self._mdone = False
                            self.push_change_event("MotionDone", self._mdone)
                        new_state = PT.DevState.MOVING

                    else:
                        if not self._mdone:
                            self._mdone = True
                            self.push_change_event("MotionDone", self._mdone)
                        new_state = PT.DevState.STANDBY

                    if st.move_failed():
                        new_state = PT.DevState.FAULT
                        err_code = self.dev.GetLastError(self.axis)
                        self.error_stream("Move failed with error: {0:d}".format(err_code))

                    elif st.over_temperature():
                        new_state = PT.DevState.ALARM

                    self.set_state(new_state)

                    if self._iscal != st.is_calibrated():
                        self._iscal = st.is_calibrated()
                        self.push_change_event("IsCalibrated", self._iscal)

                    if self._isref != st.is_referenced():
                        self._isref = st.is_referenced()
                        self.push_change_event("IsReferenced", self._isref)

                    # Poll postion
                    pos = self.dev.GetPosition(self.axis) * self._conv_factor

                    if self.get_logger().is_debug_enabled():
                        self.debug_stream("Polled position: {0:.3f}".format(pos))

                    if pos != self._pos:
                        self._pos = pos
                        self.push_change_event("Position", self._pos)

                except PT.DevFailed as e:
                    self.error_stream("Device polling failed. Errors:")
                    for i in range(len(e.args)):
                        self.error_stream("[{0:d}] {1!s} ({2!s})".format(i, e.args[i].desc, e.args[i].origin))

            else:
                if count > 10:
                    count = 0
                    # Try to reconnect to device
                    try:
                        self.initialize_device()
                        self.set_state(PT.DevState.STANDBY)
                    except PT.DevFailed:
                        self.error_stream("Failed to reconnect with device")
                else:
                    count += 1

            elapsed = time.time() - s
            if(elapsed < polling):
                time.sleep(polling - elapsed)

    @PTS.attribute(name="Position", label="Position", dtype=PT.DevDouble, doc="Axis position")
    def Position(self):
        return self._pos

    @Position.write
    def wPosition(self, value):
        self.debug_stream("Write Position: {0:f}".format(value))

        # Convert position to device units
        pos = int(value / self._conv_factor)

        # Start move
        self.dev.Move(np.array([self.axis, pos], np.int64))
        self.set_state(PT.DevState.MOVING)

    def is_Position_allowed(self, attr):
        general = self.command_allowed(attr)
        if not general:
            return False
        if general and attr != PT.AttReqType.READ_REQ and self.get_state() != PT.DevState.STANDBY:
            return False
        else:
            return True

    @PTS.attribute(name="Velocity", label="Velocity", dtype=PT.DevDouble, format="%.2f", unit="um/s", doc="Axis velocity", fisallowed='command_allowed')
    def Velocity(self):
        return self._vel

    @Velocity.write
    def wVelocity(self, value):
        self.debug_stream("Write velocity {0!s}".format(value))

        if value == self._vel:
            return

        # Convert velocity to device unit
        vel = int(value / self._conv_factor)

        # Write velocity to device
        self.dev.SetVelocity(np.array([self.axis, vel], dtype=np.int64))

        time.sleep(0.1)

        # Check written value
        new_vel = self.dev.GetVelocity(self.axis)

        if new_vel != vel:
            self.error_stream("Failed to set velocity (set={0:d}, actual={1:d})".format(vel, new_vel))
            PT.Except.throw_exception("Failed to set velocity (set={0:d}, actual={1:d})".format(vel, new_vel))

        # Push change event
        self._vel = value
        self.push_change_event("Velocity", self._vel)

    @PTS.attribute(name="Acceleration", label="Acceleration", dtype=PT.DevDouble, format="%.2f", unit="um/s^2", doc="Axis acceleration", fisallowed='command_allowed')
    def Acceleration(self):
        return self._acc

    @Acceleration.write
    def wAcceleration(self, value):
        self.debug_stream("Write acceleration {0!s}".format(value))

        if value == self._acc:
            return

        # Convert velocity to device unit
        acc = int(value / self._conv_factor)

        # Write velocity to device
        self.dev.SetAcceleration(np.array([self.axis, acc], dtype=np.int64))

        time.sleep(0.1)

        # Check written value
        new_acc = self.dev.GetAcceleration(self.axis)

        if new_acc != acc:
            self.error_stream("Failed to set acceleration (set={0:d}, actual={1:d})".format(acc, new_acc))
            PT.Except.throw_exception("Failed to set acceleration (set={0:d}, actual={1:d})".format(acc, new_acc))

        # Push change event
        self._acc = value
        self.push_change_event("Acceleration", self._acc)

    @PTS.attribute(name="MotionDone", label="Motion done", dtype=PT.DevBoolean, doc="Motion done", fisallowed='command_allowed')
    def MotionDone(self):
        return self._mdone

    @PTS.attribute(name="IsReferenced", label="Is referenced", dtype=PT.DevBoolean, doc="Axis is referenced", fisallowed='command_allowed')
    def IsReferenced(self):
        return self._isref

    @PTS.attribute(name="IsCalibrated", label="Is calibrated", dtype=PT.DevBoolean, doc="Axis is calibrated", fisallowed='command_allowed')
    def IsCalibrated(self):
        return self._iscal


if __name__ == "__main__":
    # Start device server
    try:
        PTS.run((SmarActPositioner, ))
    except PT.DevFailed as e:
        print("Tango exception: {:}".format(e.args[0].desc))
    except Exception as e:
        print("Python exception: {:}".format(repr(e)))
    except str as e:
        print("String exception: {:}".format(e))
