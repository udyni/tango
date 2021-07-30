#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jan  8 14:49:24 2021

Controller interface for a SmarAct MCS2 controller

NOTE: this version only support the ethernet version

@author: Michele Devetta (michele.devetta@cnr.it)
"""

import time
import socket
import PyTango as PT
import PyTango.server as PTS
from SmarActUtils import MoveMode, CalibrationOptions, ReferencingOptions, ActuatorMode

class SmarActController(PTS.Device, metaclass=PTS.DeviceMeta):

    #host = PTS.device_property(dtype=str, doc="MCS2 IP address", mandatory=True)
    host = PTS.device_property(dtype=str, doc="MCS2 IP address")
    #port = PTS.device_property(dtype=PT.DevShort, doc="MCS2 port number", mandatory=False, default_value=55551)
    port = PTS.device_property(dtype=PT.DevShort, doc="MCS2 port number", default_value=55551)


    def init_device(self):
        """ Initialize device
        """
        # Parent init
        PTS.Device.init_device(self)

        # Create socket and connect to device
        self.s = None
        try:
            self.__mcs_connect()

        except OSError as e:
            # Connection failed
            self.s = None
            self.set_state(PT.DevState.FAULT)
            # Log error
            self.error_stream("init_device(): error while connecting to device (Error: {0!s})".format(e))

    def __mcs_connect(self):
        if self.s is None:
            self.debug_stream("Connecting to: {0!s}".format(self.host))
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.s.connect((self.host, self.port))
            self.s.settimeout(0.1)

            # Store number of channels in the controller
            self._nch = 0
            try:
                self.s.send(b":DEV:NOCH?\r\n")
                rsp = self.s.recv(10).strip()
                self._nch = int(rsp)
                self.debug_stream("Controller has {0:d} channels".format(self._nch))
            except OSError as e:
                self.error_stream("__mcs_connect(): Failed to get number of channels (Error: {0!s})".format(e))
                self.s.close()
                self.s = None
                raise OSError("Failed to get number of channles")
            except ValueError as e:
                self.error_stream("__mcs_connect(): Failed to convert number of channels (Error: {0!s})".format(e))
                self.s.close()
                self.s = None
                raise OSError("Failed to get number of channles")

    def always_executed_hook(self):
        """ Check at every call if the connection is ok. In case reconnect
        """
        if self.s is None:
            try:
                self.__mcs_connect()

            except OSError as e:
                # Failed to reconnect
                self.error_stream("always_executed_hook(): failed to reconnect to device (Error: {0!s})".format(e))
                PT.Except.throw_exception("Reconnection failed", str(e), "SmarActController::always_executed_hook()")

    def delete_device(self):
        """ Delete device
        """
        if self.s is not None:
            try:
                self.s.close()
            except OSError:
                pass

    def _send_command(self, cmd):
        """ Send a command without response
        """
        self.debug_stream("Sending command: {0!s}".format(cmd))
        try:
            if type(cmd) != bytes:
                cmd = cmd.encode()


            cmd = cmd + b"\r\n"

            self.s.send(cmd)

        except OSError as e:
            try:
                self.s.close()
            except OSError:
                self.error_stream("_send_command(): Failed to close socket (Error: {0!s})".format(e))
            self.s = None
            PT.Except.throw_exception("Send failed", "Failed to send command (Error: {0!s})".format(e), "SmarActController::_send_command()")

    def _send_command_with_response(self, cmd):
        """ Send a command with a response
        """
        self.debug_stream("Sending command: {0!s}".format(cmd))
        try:
            if type(cmd) != bytes:
                cmd = cmd.encode()
            cmd = cmd + b"\r\n"

            self.s.send(cmd)

        except OSError as e:
            # Close connection. We will reconnect at the next command
            try:
                self.s.close()
            except OSError:
                self.error_stream("_send_command_with_response(): Failed to close socket (Error: {0!s})".format(e))
            self.s = None
            PT.Except.throw_exception("Send failed", "Failed to send command (Error: {0!s})".format(e), "SmarActController::_send_command_with_response()")

        try:
            begin = time.time()
            rsp = b""
            while True:
                tmp = self.s.recv(100)
                if tmp[-2:] == b"\r\n":
                    rsp += tmp
                    break
                else:
                    rsp += tmp

                if time.time() - begin > 1:
                    PT.Except.throw_exception("Receive timeout", "Receive timeout", "SmarActController::_send_command_with_response()")
        except OSError as e:
            try:
                self.s.close()
            except OSError:
                self.error_stream("_send_command_with_response(): Failed to close socket (Error: {0!s})".format(e))
            self.s = None
            PT.Except.throw_exception("Receive failed", "Failed to receive response (Error: {0!s})".format(e), "SmarActController::_send_command_with_response()")

        self.debug_stream("Got response: {0!s}".format(rsp.strip()))
        return rsp.strip()

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number',
             dtype_out=PT.DevULong, doc_out='Unsigned short describing axis state')
    def AxisState(self, argin):
        """
        Returns 16-bit value describing channel state:

            0  SA_CTL_CH_STATE_BIT_ACTIVELY_MOVING 0x0001
            1  SA_CTL_CH_STATE_BIT_CLOSED_LOOP_ACTIVE 0x0002
            2  SA_CTL_CH_STATE_BIT_CALIBRATING 0x0004
            3  SA_CTL_CH_STATE_BIT_REFERENCING 0x0008
            4  SA_CTL_CH_STATE_BIT_MOVE_DELAYED 0x0010
            5  SA_CTL_CH_STATE_BIT_SENSOR_PRESENT 0x0020
            6  SA_CTL_CH_STATE_BIT_IS_CALIBRATED 0x0040
            7  SA_CTL_CH_STATE_BIT_IS_REFERENCED 0x0080
            8  SA_CTL_CH_STATE_BIT_END_STOP_REACHED 0x0100
            9  SA_CTL_CH_STATE_BIT_RANGE_LIMIT_REACHED 0x0200
            10 SA_CTL_CH_STATE_BIT_FOLLOWING_LIMIT_REACHED 0x0400
            11 SA_CTL_CH_STATE_BIT_MOVEMENT_FAILED 0x0800
            12 SA_CTL_CH_STATE_BIT_IS_STREAMING 0x1000
            14 SA_CTL_CH_STATE_BIT_OVER_TEMPERATURE 0x4000
            15 SA_CTL_CH_STATE_BIT_REFERENCE_MARK 0x8000

        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::AxisState()")

        self.debug_stream("Value: {0!s}, type: {1!s}".format(argin, type(argin)))
        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:STAT?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get axis state", "SmarActController::AxisState()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::AxisState()")
        except Exception as e:
            self.error_stream("Unexpected error: {0!s}".format(e))
            return 0

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number',
             dtype_out=PT.DevLong64, doc_out='Position in encoder steps')
    def GetPosition(self, argin):
        """ Returns current position in encoder steps
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetPosition()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:POS?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get axis position", "SmarActController::GetPosition()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetPosition()")

    @PTS.command(dtype_in=PT.DevVarLong64Array, doc_in='Axis number, current position', dtype_out=None, doc_out='')
    def SetPosition(self, argin):
        """
        Command SetPosition related method
        Description: change current position to a defined value. Does not move axis
            @param argin 0: Axis number
                         1: current position
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetPosition()")

        if argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetPosition()")

        try:
            self._send_command(":CHAN{0:d}:POS {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set axis current position", "SmarActController::SetPosition()")

    @PTS.command(dtype_in=PT.DevVarLong64Array, doc_in='Axis number, target position', dtype_out=None, doc_out='')
    def Move(self, argin):
        """
        Command Move related method
        Description: moves axis
        the effect of the given position value depends on the move mode
            @param argin 0: Axis number
                         1: target position
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::Move()")

        if argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::Move()")

        try:
            self._send_command(":MOVE{0:d} {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to move axis", "SmarActController::Move()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong, doc_out='Move mode')
    def GetMoveMode(self, argin):
        """
        Command GetMoveMode related method
        Description: get move mode

         0 : closed loop absolute
         1 : closed loop relative
         2 : scan mode absolute
         3 : scan mode relative
         4 : step mode
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetMoveMode()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:MMOD?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get position", "SmarActController::GetMoveMode()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetMoveMode()")

    @PTS.command(dtype_in=PT.DevVarLongArray, doc_in='Axis number, move mode', dtype_out=None, doc_out='')
    def SetMoveMode(self, argin):
        """
        Command SetMoveMode related method
        Description: set move mode

            0 : closed loop absolute
            1 : closed loop relative
            2 : scan mode absolute
            3 : scan mode relative
            4 : step mode

        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetMoveMode()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetMoveMode()")

        mm = MoveMode(argin[1])
        if not mm.isValid():
            PT.Except.throw_exception("Bad option", "Selected move mode {0:d} is not valid".format(mm.getValue()), "SmarActController::SetMoveMode()")

        try:
            self._send_command(":CHAN{0:d}:MMOD {1:d}".format(argin[0], mm.getValue()))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set move mode", "SmarActController::SetMoveMode()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevULong, doc_out='Step amplitude in open-loop')
    def GetStepAmplitude(self, argin):
        """
        Command GetStepAmplitude related method
        Description: Gets step amplitude for open-loop movements.
	     Value range: 1 - 65535
	     65535 corresponds to 100V
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetStepAmplitude()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:STEP:AMPL?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get step amplitude", "SmarActController::GetStepAmplitude()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetStepAmplitude()")

    @PTS.command(dtype_in=PT.DevVarLongArray, doc_in='Axis number, step amplitude in open-loop', dtype_out=None, doc_out='')
    def SetStepAmplitude(self, argin):
        """
        Command SetStepAmplitude related method
        Description: Sets step amplitude for open-loop movements.
            Value range: 1 - 65535
            65535 corresponds to 100V
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetStepAmplitude()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetStepAmplitude()")

        if argin[1] < 1 or argin[1] > 65535:
            PT.Except.throw_exception("Bad option", "Selected step amplitude {0:d} is not valid".format(argin[1]), "SmarActController::SetStepAmplitude()")

        try:
            self._send_command(":CHAN{0:d}:STEP:AMPL {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set step amplitude", "SmarActController::SetStepAmplitude()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevUShort, doc_out='Step frequency')
    def GetStepFrequency(self, argin):
        """
        Command GetStepFrequency related method
        Description: Gets step frequency for open-loop movements.
            Value range: 1 - 20000 Hz
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetStepFrequency()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:STEP:FREQ?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get step frequency", "SmarActController::GetStepFrequency()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetStepFrequency()")

    @PTS.command(dtype_in=PT.DevVarLongArray, doc_in='Axis number, step frequency', dtype_out=None, doc_out='')
    def SetStepFrequency(self, argin):
        """
        Command SetStepFrequency related method
        Description: Sets step frequency for open-loop movements.
            Value range: 1 - 20000 Hz
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetStepFrequency()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetStepFrequency()")

        if argin[1] < 1 or argin[1] > 20000:
            PT.Except.throw_exception("Bad option", "Selected step frequency {0:d} is not valid".format(argin[1]), "SmarActController::SetStepFrequency()")

        try:
            self._send_command(":CHAN{0:d}:STEP:FREQ {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set step frequency", "SmarActController::SetStepFrequency()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong64, doc_out='Closed loop velocity')
    def GetVelocity(self, argin):
        """
        Command GetVelocity related method
        Description: Gets closed loop velocity.
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetVelocity()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:VEL?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get velocity", "SmarActController::GetVelocity()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetVelocity()")

    @PTS.command(dtype_in=PT.DevVarLong64Array, doc_in='Axis number', dtype_out=None, doc_out='')
    def SetVelocity(self, argin):
        """
        Command SetVelocity related method
        Description: Sets closed loop velocity
            Value range: 0 - 100000000000 (1e11)
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetVelocity()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetVelocity()")

        if argin[1] < 0 or argin[1] > 100000000000:
            PT.Except.throw_exception("Bad option", "Selected velocity {0:d} is not valid".format(argin[1]), "SmarActController::SetVelocity()")

        try:
            self._send_command(":CHAN{0:d}:VEL {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set velocity", "SmarActController::SetVelocity()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevULong, doc_out='Current hold time')
    def GetHoldTime(self, argin):
        """
        Command GetHoldTime related method
        Description: Gets hold time for closed-loop movements.
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetHoldTime()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:HOLD?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get hold time", "SmarActController::GetHoldTime()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetHoldTime()")

    @PTS.command(dtype_in=PT.DevVarLongArray, doc_in='Axis number, hold time', dtype_out=None, doc_out='')
    def SetHoldTime(self, argin):
        """
        Command SetHoldTime related method
        Description: Sets hold time (time in ms the position is actively held after reaching the
	                target posi-tion) for closed-loop movements.
	                Value range: 0 - 4294967295 (0 disables holding,
	                max value is for infinite, i.e. hold until motor is stopped)
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetHoldTime()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetHoldTime()")

        if argin[1] < 0 or argin[1] > 4294967295:
            PT.Except.throw_exception("Bad option", "Selected hold time {0:d} is not valid".format(argin[1]), "SmarActController::SetHoldTime()")

        try:
            self._send_command(":CHAN{0:d}:HOLD {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set hold time", "SmarActController::SetHoldTime()")

#    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevBoolean, doc_out='True if position is known')
#    def GetPositionKnown(self, argin):
#        """
#        Command GetPositionKnown related method
#        Description: Finds out whether the physical position of this axis is known.
#                     If false and the motor has an encoder with reference mark,
#	                run command `Home`.
#        """
#        #TODO

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong, doc_out='Axis type')
    def GetAxisType(self, argin):
        """
        Command GetAxisType related method
        Description: return axis type as follows

	            0: linear movement
	            1: rotary movement
	            2: goniometer
	            3: tilt movement
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetAxisType()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:TUN:MTYP?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get movement type", "SmarActController::GetAxisType()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetAxisType()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong, doc_out='')
    def GetMaxFrequency(self, argin):
        """
        Command GetMaxFrequency related method
        Description: Gets maximum frequency for closed-loop movements.
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetMaxFrequency()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:MCLF?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get maximum frequency", "SmarActController::GetMaxFrequency()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetMaxFrequency()")

    @PTS.command(dtype_in=PT.DevVarLongArray, doc_in='Axis number, maximum frequecy', dtype_out=None, doc_out='')
    def SetMaxFrequency(self, argin):
        """
        Command SetMaxFrequency related method
        Description: Sets maximum frequency for closed-loop movements.
                     Value range: 50 - 20000 Hz
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetMaxFrequency()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetMaxFrequency()")

        if argin[1] < 50 or argin[1] > 20000:
            PT.Except.throw_exception("Bad option", "Selected maximum frequency {0:d} is not valid".format(argin[1]), "SmarActController::SetMaxFrequency()")

        try:
            self._send_command(":CHAN{0:d}:MCLF {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set maximum frequency", "SmarActController::SetMaxFrequency()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevULong, doc_out='Positioner type')
    def GetPositionerType(self, argin):
        """
        Command GetSensorType related method
        Description: Get positioner type number, check with hardware doc
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetPositionerType()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:PTYP?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get positioner type", "SmarActController::GetPositionerType()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetPositionerType()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong64, doc_out='')
    def GetMaxStepLimit(self, argin):
        """
        Command GetMaxStepLimit related method
        Description: returns currently set range limit maximum
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetMaxStepLimit()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:RLIM:MAX?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to maximum of range limit", "SmarActController::GetMaxStepLimit()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetMaxStepLimit()")

    @PTS.command(dtype_in=PT.DevVarLong64Array, doc_in='Axis number, max step limit', dtype_out=None, doc_out='')
    def SetMaxStepLimit(self, argin):
        """
        Command SetMaxStepLimit related method
        Description:
            @param argin 0: axis number
	                    1: maximum range limit
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetMaxStepLimit()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetMaxStepLimit()")

        if argin[1] < -100e12 or argin[1] > 100e12:
            PT.Except.throw_exception("Bad option", "Selected maximum step limit {0:d} is not valid".format(argin[1]), "SmarActController::SetMaxStepLimit()")

        try:
            self._send_command(":CHAN{0:d}:RLIM:MAX {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set maximum step limit", "SmarActController::SetMaxStepLimit()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong64, doc_out='')
    def GetMinStepLimit(self, argin):
        """
        Command GetMinStepLimit related method
        Description: returns currently set range limit minimum
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetMinStepLimit()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:RLIM:MIN?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get minimum of range limit", "SmarActController::GetMinStepLimit()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetMinStepLimit()")

    @PTS.command(dtype_in=PT.DevVarLong64Array, doc_in='Axis number', dtype_out=None, doc_out='')
    def SetMinStepLimit(self, argin):
        """
        Command SetMinStepLimit related method
        Description:
            @param argin 0: axis number
                         1: minimum range limit
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetMinStepLimit()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetMinStepLimit()")

        if argin[1] < -100e12 or argin[1] > 100e12:
            PT.Except.throw_exception("Bad option", "Selected minimum step limit {0:d} is not valid".format(argin[1]), "SmarActController::SetMinStepLimit()")

        try:
            self._send_command(":CHAN{0:d}:RLIM:MIN {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set minimum step limit", "SmarActController::SetMinStepLimit()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=None, doc_out='')
    def Calibrate(self, argin):
        """
        Command Calibrate related method
        Description: Perform calibration for channnel <axis number>. Should only be necessary
                     if setup was changed (e.g. new / different positioner connected).
	                Requires encoded positioner, encoder has to be enabled.
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::Calibrate()")

        try:
            self._send_command(":CALIBRATE{0:d}".format(argin))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to start calibration", "SmarActController::Calibrate()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::Calibrate()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevULong, doc_out='Calibration options')
    def GetCalibrationOptions(self, argin):
        """
        Command GetCalibrationOptions related method
        Description: Get calibration option (used with Calibrate command)

	                bit 0: Direction
	                bit 1: Detect Distance Code Inversion
	                bit 2: Advanced Sensor Correction
	                bit 8: Limited Travel Range
	                all other bits are reserved
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetCalibrationOptions()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:CAL:OPT?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get calibration options", "SmarActController::GetCalibrationOptions()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetCalibrationOptions()")

    @PTS.command(dtype_in=PT.DevVarULongArray, doc_in='Axis number, calibration options bit field', dtype_out=None, doc_out='')
    def SetCalibrationOptions(self, argin):
        """
        Command SetCalibrationOptions related method
        Description: Set calibration option (used with Calibrate command)
                        bit 0: Direction
                        bit 1: Detect Distance Code Inversion
                        bit 2: Advanced Sensor Correction
                        bit 8: Limited Travel Range
                        all other bits are reserved
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetCalibrationOptions()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetCalibrationOptions()")


        try:
            opt = CalibrationOptions(argin[1])
            if not opt.isValid():
                PT.Except.throw_exception("Bad option", "Selected calibration options {0:d} are not valid".format(argin[1]), "SmarActController::SetCalibrationOptions()")
        except Exception as e:
            self.debug_stream("ERROR: {0!s}".format(e))

        try:
            self._send_command(":CHAN{0:d}:CAL:OPT {1:d}".format(argin[0], opt.getValue()))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set calibration options", "SmarActController::SetCalibrationOptions()")

#    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=None, doc_out='')
#    def HoldPosition(self, argin):
#        """
#        Command HoldPosition related method
#        Description: Enables holding the current position by setting the move mode to
#	                closed loop relative movement and then driving by 0 encoder steps.
#	                (c.f. programmer manual, 2.6.4)
#        """
#        pass

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=None, doc_out='')
    def Stop(self, argin):
        """
        Command Stop related method
        Description: Stop movement on axis <axis number>
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::Stop()")

        try:
            self._send_command(":STOP{0:d}".format(argin))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to stop", "SmarActController::Stop()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong, doc_out='Sensor mode')
    def GetSensorMode(self, argin):
        """
        Command GetSensorMode related method
        Description: returns sensor mode
	                0: sensor disabled
	                1: sensor continuously supplied with power
	                2: sensor power supply pulsed to keep the heat generation low
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetSensorMode()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:SENS:MODE?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get sensor mode", "SmarActController::GetSensorMode()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetSensorMode()")

    @PTS.command(dtype_in=PT.DevVarLongArray, doc_in='Axis number, sensor mode', dtype_out=None, doc_out='')
    def SetSensorMode(self, argin):
        """
        Command SetSensorMode related method
        Description:
	 	@param argin 0: axis number
	                  1: sensor mode
	                     0: sensor disabled
	                     1: sensor continuously supplied with power
	                     2: sensor power supply pulsed to keep the heat generation low
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetSensorMode()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetSensorMode()")

        if argin[1]< 0 or argin[1] > 2:
            PT.Except.throw_exception("Bad option", "Selected sensor mode {0:d} is not valid".format(argin[1]), "SmarActController::SetSensorMode()")

        try:
            self._send_command(":CHAN{0:d}:SENS:MODE {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set sensor mode", "SmarActController::SetSensorMode()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong64, doc_out='Acceleration')
    def GetAcceleration(self, argin):
        """
        Command GetAcceleration related method
        Description: Gets closed loop acceleration
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetAcceleration()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:ACC?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get acceleration", "SmarActController::GetAcceleration()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetAcceleration()")

    @PTS.command(dtype_in=PT.DevVarLong64Array, doc_in='Axis number, acceleration', dtype_out=None, doc_out='')
    def SetAcceleration(self, argin):
        """
        Command SetAcceleration related method
        Description: Sets closed loop acceleration
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetAcceleration()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetAcceleration()")

        try:
            self._send_command(":CHAN{0:d}:ACC {1:d}".format(argin[0], argin[1]))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set acceleration", "SmarActController::SetAcceleration()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong, doc_out='Actuator mode')
    def GetActuatorMode(self, argin):
        """
        Command GetActuatorMode related method
        Description: Gets currently set actuator mode
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetActuatorMode()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:ACT:MODE?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get actuator mode", "SmarActController::GetActuatorMode()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetActuatorMode()")

    @PTS.command(dtype_in=PT.DevVarLongArray, doc_in='Axis number, actuator mode', dtype_out=None, doc_out='')
    def SetActuatorMode(self, argin):
        """
        Command SetActuatorMode related method
        Description: set actuator mode
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetActuatorMode()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetActuatorMode()")

        act = ActuatorMode(argin[1])
        if not act.isValid():
            PT.Except.throw_exception("Bad option", "Selected actuator mode {0:d} is not valid".format(argin[1]), "SmarActController::SetActuatorMode()")

        try:
            self._send_command(":CHAN{0:d}:ACT:MODE {1:d}".format(argin[0], act.getValue()))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set actuator mode", "SmarActController::SetActuatorMode()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=None, doc_out='')
    def Reference(self, argin):
        """
        Command Home related method
        Description: home axis (start referencing procedure)
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::Stop()")

        try:
            self._send_command(":REF{0:d}".format(argin))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to reference axis", "SmarActController::Stop()")

#    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong64, doc_out='Following error limit')
#    def GetFollowingErrorLimit(self, argin):
#        """
#        Command GetFollowingErrorLimit related method
#        Description: Gets currently set following error limit
#        """
#        return 0
#
#    @PTS.command(dtype_in=PT.DevVarLong64Array, doc_in='Axis number', dtype_out=None, doc_out='')
#    def SetFollowingErrorLimit(self, argin):
#        """
#        Command SetFollowingErrorLimit related method
#        Description: Sets following error limit
#                     Value range: 0 - 100000000000000
#        """
#        pass

#    @PTS.command(dtype_in=PT.DevString, doc_in='Trajectory data file name', dtype_out=None, doc_out='')
#    def StartStreaming(self, argin):
#        """
#        Command StartStreaming related method
#        Description: Start trajectoy streaming. A file with trajectory data must exist and it`s
#                     path/name supplied as argument to this command.
#        """
#        pass

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong, doc_out='Referencing options')
    def GetReferencingOptions(self, argin):
        """
        Command GetReferencingOptions related method
        Description: gets referencing options
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetReferencingOptions()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:REF:OPT?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get sensor reference type", "SmarActController::GetReferencingOptions()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetReferencingOptions()")


    @PTS.command(dtype_in=PT.DevVarLongArray, doc_in='Axis number, referencing options', dtype_out=None, doc_out='')
    def SetReferencingOptions(self, argin):
        """
        Command SetReferencingOptions related method
        Description: sets search direction property
            @param argin 0: axis number
                         1: referencing options
        """
        if len(argin) != 2:
            PT.Except.throw_exception("Bad number of parameters", "This command requires two input parameters", "SmarActController::SetReferencingOptions()")

        if argin[0] < 0 or argin[0] >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::SetReferencingOptions()")

        opt = ReferencingOptions(argin[1])
        if not opt.isValid():
            PT.Except.throw_exception("Bad option", "Selected referencing options {0:d} are not valid".format(argin[1]), "SmarActController::SetReferencingOptions()")

        try:
            self._send_command(":CHAN{0:d}:ACT:MODE {1:d}".format(argin[0], opt.getValue()))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to set referencing options", "SmarActController::SetReferencingOptions()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevString, doc_out='')
    def GetPositionerName(self, argin):
        """
        Command GetSensorName related method
        Description: Get description of sensor type number, check with hardware doc
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetPositionerName()")

        try:
            return self._send_command_with_response(":CHAN{0:d}:PTYP:NAME?".format(argin))

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get positioner name", "SmarActController::GetPositionerName()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetPositionerName()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong, doc_out='')
    def GetSensorReferenceType(self, argin):
        """
        Command GetSensorReferenceType related method
        Description: Get sensor reference mark type:
	                  no reference mark: 0
	                  end stop:          1
	                  single mark:       2
	                  distance coded:    3
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetSensorReferenceType()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:TUN:RTYP?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get sensor reference type", "SmarActController::GetSensorReferenceType()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetSensorReferenceType()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong, doc_out='Base resolution exponent')
    def GetBaseResolution(self, argin):
        """
        Command GetBaseResolution related method
        Description: the basic resolution of the position value in powers of 10 (valid values: -12, -9, -6, -3, 0)
        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetBaseResolution()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:TUN:BASE:RES?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get base resolution", "SmarActController::GetBaseResolution()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetBaseResolution()")

    @PTS.command(dtype_in=PT.DevULong, doc_in='Axis number', dtype_out=PT.DevLong, doc_out='Base unit')
    def GetBaseUnit(self, argin):
        """
        Command GetBaseUnit related method
        Description: the basic unit of the position values a channel uses. (e.g. meter, degree)

        Values:
            SA_CTL_UNIT_NONE ( 0x00000000 ),
            SA_CTL_UNIT_METER ( 0x00000002 ),
            SA_CTL_UNIT_DEGREE ( 0x00000003 )

        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetBaseUnit()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:TUN:BASE:UNIT?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get base unit", "SmarActController::GetBaseUnit()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetBaseUnit()")

    @PTS.command(dtype_in=PT.DevULong, doc_in="Axis number", dtype_out=PT.DevLong, doc_out="Last error code")
    def GetLastError(self, argin):
        """
        Command GetLastError related method
        Description: return the last error of the selected channel


        """
        if argin >= self._nch:
            PT.Except.throw_exception("Bad channel", "Selected channel {0:d} is not available (number of channels {1:d})".format(argin, self._nch), "SmarActController::GetLastError()")

        try:
            rsp = self._send_command_with_response(":CHAN{0:d}:ERR?".format(argin))
            return int(rsp)

        except PT.DevFailed as e:
            PT.Except.re_throw_exception(e, "Communication error", "Failed to get base unit", "SmarActController::GetLastError()")
        except ValueError as e:
            PT.Except.throw_exception("Conversion failed", "Failed to convert string (Error: {0!s})".format(e), "SmarActController::GetLastError()")


if __name__ == "__main__":
    # Start device server
    try:
        PTS.run( (SmarActController, ) )
    except PT.DevFailed as e:
        print("Tango exception: {:}".format(e.args[0].desc))
    except Exception as e:
        print("Python exception: {:}".format(repr(e)))
    except str as e:
        print("String exception: {:}".format(e))
