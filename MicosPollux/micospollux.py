#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MicosPollux
Device server to control a Micos translation stages connected to Pollux controllers

@author: Michele Devetta <michele.devetta@cnr.it>
"""

import time
import numpy as np
import threading

import PyTango as PT
import PyTango.server as PTS


class PolluxPolling(threading.Thread):

    def __init__(self, parent):
        # Parent constructor
        threading.Thread.__init__(self)

        # Init variables
        self._parent = parent
        self._polling = float(parent.polling) / 1000.0
        self._axis = parent.axis
        self._terminate = False
        self._pos_value = 0
        self._vel_value = 0
        self._acc_value = 0.0

        # Connect to proxy
        try:
            self.dev = PT.DeviceProxy(self._parent.proxy)
            self.dev.ping()
        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e,
                                         "Proxy not available",
                                         "Proxy device is offline or not reachable",
                                         "MicosPollux::init_device()")

        # Read position, velocity and acceleration
        self._pos_value = self.__get_pos()
        self._vel_value = self.__get_vel()
        self._acc_value = self.__get_acc()

        # Start thread
        self.start()

    def terminate(self):
        self._terminate = True

    def __query(self, command):
        command += " "
        ans = self.dev.SyncSendRecv(np.frombuffer(command.encode('ascii'), np.int8))
        return ans.tostring().strip()

    def __command(self, command):
        command += " "
        self.dev.Send(np.frombuffer(command.encode('ascii'), np.int8))

    def __get_pos(self):
        pos = self.__query("{0:d} np".format(self._axis))
        try:
            return float(pos)
        except ValueError as e:
            PT.Except.throw_exception("Bad value", "Value conversion failed with error {0!s}".format(e), "PolluxPolling::__get_pos()")

    def __get_vel(self):
        vel = self.__query("{0:d} gnv".format(self._axis))
        try:
            return float(vel)
        except ValueError as e:
            PT.Except.throw_exception("Bad value", "Value conversion failed with error {0!s}".format(e), "PolluxPolling::__get_vel()")

    def getVelocity(self):
        return self._vel_value

    def setVelocity(self, vel):
        if vel == self._vel_value:
            return
        self.__command("{0:.4f} {1:d} snv".format(vel, self._axis))
        time.sleep(0.1)
        # Check if the value was set
        new_vel = self.__get_vel()
        if new_vel != vel:
            # Set failed
            PT.Except.throw_exception("Set failed", "Failed to set velocity", "PolluxPolling::__set_vel()")
        else:
            self._vel_value = vel
            self._parent.push_change_event("Velocity", self._vel_value)

    def __get_acc(self):
        acc = self.__query("{0:d} gna".format(self._axis))
        try:
            return float(acc)
        except ValueError as e:
            PT.Except.throw_exception("Bad value", "Value conversion failed with error {0!s}".format(e), "PolluxPolling::__get_acc()")

    def getAcceleration(self):
        return self._acc_value

    def setAcceleration(self, acc):
        if acc == self._acc_value:
            return
        self.__command("{0:.4f} {1:d} snv".format(acc, self._axis))
        time.sleep(0.1)
        # Check if the value was set
        new_acc = self.__get_acc()
        if new_acc != acc:
            # Set failed
            PT.Except.throw_exception("Set failed", "Failed to set acceleration", "PolluxPolling::__set_acc()")
        else:
            self._acc_value = acc
            self._parent.push_change_event("Acceleration", self._acc_value)

    def getPosition(self):
        return self._pos_value

    def moveTo(self, pos):
        if pos == self._pos_value:
            return
        self.__command("{0:.4f} {1:d} nm".format(pos, self._axis))

    def __get_status(self):
        st = self.__query("{0:d} nst".format(self._axis))
        try:
            return int(st)
        except ValueError as e:
            PT.Except.throw_exception("Bad value", "Value conversion failed with error {0!s}".format(e), "PolluxPolling::__get_status()")

    def setHome(self):
        self.__command("0 {0:d} setnpos".format(self._axis))

    def run(self):
        while not self._terminate:
            s = time.time()

            try:
                # Poll position
                pos = self.__get_pos()
                if pos != self._pos_value:
                    self._pos_value = pos
                    self._parent.push_change_event("Position", self._pos_value)

                # Poll status
                status = self.__get_status()
                if status & 0x01 == 1:
                    # Moving
                    self._parent.set_state(PT.DevState.MOVING)
                else:
                    # Motion done
                    self._parent.set_state(PT.DevState.STANDBY)

            except PT.DevFailed as e:
                self._parent.error_stream("[Polling error] {0!s}".format(e.args[0].desc))

            e = time.time() - s
            if e < self._polling:
                time.sleep(self._polling - e)


class MicosPollux(PTS.Device, metaclass=PTS.DeviceMeta):

    # Device properties
    proxy = PTS.device_property(dtype=str, doc="Proxy device for serial communication")
    polling = PTS.device_property(dtype=np.uint32, doc="Polling period in ms")
    axis = PTS.device_property(dtype=np.uint32, doc="Axis number")

    # Attributes
    Position = PTS.attribute(
        label="Position", dtype=PT.DevDouble, format="%.4f",
        access=PT.AttrWriteType.READ_WRITE, unit="mm", doc="")

    Velocity = PTS.attribute(
        label="Velocity", dtype=PT.DevDouble, format="%.2f",
        access=PT.AttrWriteType.READ_WRITE, unit="mm/s", memorized=True, hw_memorized=True,
        doc="")

    Acceleration = PTS.attribute(
        label="Acceleration", dtype=PT.DevDouble, format="%.2f",
        access=PT.AttrWriteType.READ_WRITE, unit="mm/s^2", memorized=True, hw_memorized=True,
        doc="")

    def init_device(self):
        """ Initialize device
        """
        # Set INIT state
        self.set_state(PT.DevState.INIT)

        # Call parent init
        PTS.Device.init_device(self)

        # Set polling on State
        self.poll_command("State", 500)

        # Start monitor thread
        self.monitor = PolluxPolling(self)

    def delete_device(self):
        if self.monitor:
            self.monitor.terminate()
            self.monitor.join()

    def read_Position(self):
        return self.monitor.getPosition()

    def write_Position(self, value):
        self.monitor.moveTo(value)

    def read_Velocity(self):
        return self.monitor.getVelocity()

    def write_Velocity(self, value):
        self.monitor.setVelocity(value)

    def read_Acceleration(self):
        return self.monitor.getAcceleration()

    def write_Acceleration(self, value):
        self.monitor.setAcceleration(value)

    @PTS.command()
    def setHome(self):
        self.monitor.setHome()

    @PTS.command()
    def goHome(self):
        self.monitor.moveTo(0.0)


if __name__ == "__main__":
    # Start device server
    try:
        PTS.run( (MicosPollux, ) )
    except PT.DevFailed as e:
        print("Tango exception: {:}".format(e.args[0].desc))
    except Exception as e:
        print("Python exception: {:}".format(repr(e)))
    except str as e:
        print("String exception: {:}".format(e))