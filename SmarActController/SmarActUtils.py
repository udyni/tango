# -*- coding: utf-8 -*-
"""
SmarAct Utilities

Created on Mon Jan 11 09:48:18 2021

@author: Michele Devetta <michele.devetta@cnr.it>

"""


class SAEnum(object):

    def __init__(self, value):
        self._val = self.mask(value)

    def getValue(self):
        return self._val

    def mask(self, value):
        return value

    def isValid(self):
        return True

    @classmethod
    def getEnum(cls):
        return []

    def __str__(self):
        if len(self.getEnum()):
            return self.getEnum(self.getValue())
        else:
            return repr(self)


class AxisState(SAEnum):

    SA_CTL_CH_STATE_BIT_ACTIVELY_MOVING = 0x0001
    SA_CTL_CH_STATE_BIT_CLOSED_LOOP_ACTIVE = 0x0002
    SA_CTL_CH_STATE_BIT_CALIBRATING = 0x0004
    SA_CTL_CH_STATE_BIT_REFERENCING = 0x0008
    SA_CTL_CH_STATE_BIT_MOVE_DELAYED = 0x0010
    SA_CTL_CH_STATE_BIT_SENSOR_PRESENT = 0x0020
    SA_CTL_CH_STATE_BIT_IS_CALIBRATED = 0x0040
    SA_CTL_CH_STATE_BIT_IS_REFERENCED = 0x0080
    SA_CTL_CH_STATE_BIT_END_STOP_REACHED = 0x0100
    SA_CTL_CH_STATE_BIT_RANGE_LIMIT_REACHED = 0x0200
    SA_CTL_CH_STATE_BIT_FOLLOWING_LIMIT_REACHED = 0x0400
    SA_CTL_CH_STATE_BIT_MOVEMENT_FAILED = 0x0800
    SA_CTL_CH_STATE_BIT_IS_STREAMING = 0x1000
    SA_CTL_CH_STATE_BIT_OVER_TEMPERATURE = 0x4000
    SA_CTL_CH_STATE_BIT_REFERENCE_MARK = 0x8000

    def __str__(self):
        out = ""
        if self.is_moving():
            out += "MOV "
        if self.is_closed_loop():
            out += "CL "
        else:
            out += "OL "
        if self.is_calibrating():
            out += "CAL "
        if self.is_referencing():
            out += "REF "
        if self.is_delayed():
            out += "DEL "
        if self.has_sensor():
            out += "SENS "
        if self.is_calibrated():
            out += "CALd "
        if self.is_referenced():
            out += "REFd "
        if self.end_stop_reached():
            out += "END "
        if self.range_limit_reached():
            out += "LIM "
        if self.following_limit_reached():
            out += "FLIM "
        if self.move_failed():
            out += "FAIL "
        if self.is_streaming():
            out += "STREAM "
        if self.over_temperature():
            out += "OT "
        if self.on_ref_mark():
            out += "ONREF "
        return out[:-1]

    def is_moving(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_ACTIVELY_MOVING)

    def is_closed_loop(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_CLOSED_LOOP_ACTIVE)

    def is_calibrating(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_CALIBRATING)

    def is_referencing(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_REFERENCING)

    def is_delayed(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_MOVE_DELAYED)

    def has_sensor(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_SENSOR_PRESENT)

    def is_calibrated(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_IS_CALIBRATED)

    def is_referenced(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_IS_REFERENCED)

    def end_stop_reached(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_END_STOP_REACHED)

    def range_limit_reached(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_RANGE_LIMIT_REACHED)

    def following_limit_reached(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_FOLLOWING_LIMIT_REACHED)

    def move_failed(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_MOVEMENT_FAILED)

    def is_streaming(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_IS_STREAMING)

    def over_temperature(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_OVER_TEMPERATURE)

    def on_ref_mark(self):
        return bool(self._val & self.SA_CTL_CH_STATE_BIT_REFERENCE_MARK)


class CalibrationOptions(SAEnum):

    SA_CTL_CALIB_OPT_BIT_DIRECTION = 0x00000001
    SA_CTL_CALIB_OPT_BIT_DIST_CODE_INV_DETECT = 0x00000002
    SA_CTL_CALIB_OPT_BIT_ASC_CALIBRATION = 0x00000004
    SA_CTL_CALIB_OPT_BIT_LIMITED_TRAVEL_RANGE = 0x00000100

    def isValid(self):
        mask = (self.SA_CTL_CALIB_OPT_BIT_DIRECTION |
                self.SA_CTL_CALIB_OPT_BIT_DIST_CODE_INV_DETECT |
                self.SA_CTL_CALIB_OPT_BIT_ASC_CALIBRATION |
                self.SA_CTL_CALIB_OPT_BIT_LIMITED_TRAVEL_RANGE)
        return self._val == (self._val & mask)

    def getValue(self):
        return self._val

    def getCalibrationDirection(self):
        return bool(self._val & self.SA_CTL_CALIB_OPT_BIT_DIRECTION)

    def setCalibrationDirection(self, value):
        if value:
            self._val = self._val | self.SA_CTL_CALIB_OPT_BIT_DIRECTION
        else:
            self._val = self._val & (~self.SA_CTL_CALIB_OPT_BIT_DIRECTION)

    def getDetectCodeInversion(self):
        return bool(self._val & self.SA_CTL_CALIB_OPT_BIT_DIST_CODE_INV_DETECT)

    def setDetectCodeInversion(self, value):
        if value:
            self._val = self._val | self.SA_CTL_CALIB_OPT_BIT_DIST_CODE_INV_DETECT
        else:
            self._val = self._val & (~self.SA_CTL_CALIB_OPT_BIT_DIST_CODE_INV_DETECT)

    def getAdvancedSensorCorrection(self):
        return bool(self._val & self.SA_CTL_CALIB_OPT_BIT_ASC_CALIBRATION)

    def setAdvancedSensorCorrection(self, value):
        if value:
            self._val = self._val | self.SA_CTL_CALIB_OPT_BIT_ASC_CALIBRATION
        else:
            self._val = self._val & (~self.SA_CTL_CALIB_OPT_BIT_ASC_CALIBRATION)

    def getLimitedTravelRange(self):
        return bool(self._val & self.SA_CTL_CALIB_OPT_BIT_LIMITED_TRAVEL_RANGE)

    def setLimitedTravelRange(self, value):
        if value:
            self._val = self._val | self.SA_CTL_CALIB_OPT_BIT_LIMITED_TRAVEL_RANGE
        else:
            self._val = self._val & (~self.SA_CTL_CALIB_OPT_BIT_LIMITED_TRAVEL_RANGE)


class ReferencingOptions(SAEnum):

    SA_CTL_REF_OPT_BIT_START_DIR = 0x00000001
    SA_CTL_REF_OPT_BIT_REVERSE_DIR = 0x00000002
    SA_CTL_REF_OPT_BIT_AUTO_ZERO = 0x00000004
    SA_CTL_REF_OPT_BIT_ABORT_ON_ENDSTOP = 0x00000008
    SA_CTL_REF_OPT_BIT_CONTINUE_ON_REF_FOUND = 0x00000010
    SA_CTL_REF_OPT_BIT_STOP_ON_REF_FOUND = 0x00000020

    def isValid(self):
        mask = (self.SA_CTL_REF_OPT_BIT_START_DIR |
                self.SA_CTL_REF_OPT_BIT_REVERSE_DIR |
                self.SA_CTL_REF_OPT_BIT_AUTO_ZERO |
                self.SA_CTL_REF_OPT_BIT_ABORT_ON_ENDSTOP |
                self.SA_CTL_REF_OPT_BIT_CONTINUE_ON_REF_FOUND |
                self.SA_CTL_REF_OPT_BIT_STOP_ON_REF_FOUND)
        return self._val == (self._val & mask)

    def getStartDirection(self):
        return bool(self._val & self.SA_CTL_REF_OPT_BIT_START_DIR)

    def setStartDirection(self, value):
        if value:
            self._val = self._val | self.SA_CTL_REF_OPT_BIT_START_DIR
        else:
            self._val = self._val & (~self.SA_CTL_REF_OPT_BIT_START_DIR)

    def getReverseDirection(self):
        return bool(self._val & self.SA_CTL_REF_OPT_BIT_REVERSE_DIR)

    def setReverseDirection(self, value):
        if value:
            self._val = self._val | self.SA_CTL_REF_OPT_BIT_REVERSE_DIR
        else:
            self._val = self._val & (~self.SA_CTL_REF_OPT_BIT_REVERSE_DIR)

    def getAutoZero(self):
        return bool(self._val & self.SA_CTL_REF_OPT_BIT_AUTO_ZERO)

    def setAutoZero(self, value):
        if value:
            self._val = self._val | self.SA_CTL_REF_OPT_BIT_AUTO_ZERO
        else:
            self._val = self._val & (~self.SA_CTL_REF_OPT_BIT_AUTO_ZERO)

    def getAbortOnEndstop(self):
        return bool(self._val & self.SA_CTL_REF_OPT_BIT_ABORT_ON_ENDSTOP)

    def setAbortOnEndstop(self, value):
        if value:
            self._val = self._val | self.SA_CTL_REF_OPT_BIT_ABORT_ON_ENDSTOP
        else:
            self._val = self._val & (~self.SA_CTL_REF_OPT_BIT_ABORT_ON_ENDSTOP)

    def getContinueOnRefFound(self):
        return bool(self._val & self.SA_CTL_REF_OPT_BIT_CONTINUE_ON_REF_FOUND)

    def setContinueOnRefFound(self, value):
        if value:
            self._val = self._val | self.SA_CTL_REF_OPT_BIT_CONTINUE_ON_REF_FOUND
        else:
            self._val = self._val & (~self.SA_CTL_REF_OPT_BIT_CONTINUE_ON_REF_FOUND)

    def getStopOnRefFound(self):
        return bool(self._val & self.SA_CTL_REF_OPT_BIT_STOP_ON_REF_FOUND)

    def setStopOnRefFound(self, value):
        if value:
            self._val = self._val | self.SA_CTL_REF_OPT_BIT_STOP_ON_REF_FOUND
        else:
            self._val = self._val & (~self.SA_CTL_REF_OPT_BIT_STOP_ON_REF_FOUND)


class PosMovementType(SAEnum):

    SA_CTL_POS_MOVEMENT_TYPE_LINEAR = 0
    SA_CTL_POS_MOVEMENT_TYPE_ROTATORY = 1
    SA_CTL_POS_MOVEMENT_TYPE_GONIOMETER = 2
    SA_CTL_POS_MOVEMENT_TYPE_TIP_TILT = 3

    @classmethod
    def getEnum(self):
        return ["Linear", "Rotary", "Goniometer", "Tip-Tilt"]

    def isLinear(self):
        return self._val == self.SA_CTL_POS_MOVEMENT_TYPE_LINEAR

    def isRotatory(self):
        return self._val == self.SA_CTL_POS_MOVEMENT_TYPE_ROTATORY

    def isGoniometer(self):
        return self._val == self.SA_CTL_POS_MOVEMENT_TYPE_GONIOMETER

    def isTipTilt(self):
        return self._val == self.SA_CTL_POS_MOVEMENT_TYPE_TIP_TILT


class MoveMode(SAEnum):

    SA_CTL_MOVE_MODE_CL_ABSOLUTE = 0
    SA_CTL_MOVE_MODE_CL_RELATIVE = 1
    SA_CTL_MOVE_MODE_SCAN_ABSOLUTE = 2
    SA_CTL_MOVE_MODE_SCAN_RELATIVE = 3
    SA_CTL_MOVE_MODE_STEP = 4

    @classmethod
    def getEnum(self):
        return ["CL Absolute", "CL Relative", "Scan Absolute", "Scan Relative", "Step"]

    def isValid(self):
        return self._val in (self.SA_CTL_MOVE_MODE_CL_ABSOLUTE,
                             self.SA_CTL_MOVE_MODE_CL_RELATIVE,
                             self.SA_CTL_MOVE_MODE_SCAN_ABSOLUTE,
                             self.SA_CTL_MOVE_MODE_SCAN_RELATIVE,
                             self.SA_CTL_MOVE_MODE_STEP)

    def isClosedLoop(self):
        return self._val == self.SA_CTL_MOVE_MODE_CL_ABSOLUTE or self._val == self.SA_CTL_MOVE_MODE_CL_RELATIVE

    def isOpenLoop(self):
        return self._val == self.SA_CTL_MOVE_MODE_SCAN_ABSOLUTE or self._val == self.SA_CTL_MOVE_MODE_SCAN_RELATIVE

    def isStepMode(self):
        return self._val == self.SA_CTL_MOVE_MODE_STEP

    def isAbsolute(self):
        return self._val == self.SA_CTL_MOVE_MODE_CL_ABSOLUTE or self._val == self.SA_CTL_MOVE_MODE_SCAN_ABSOLUTE

    def isRelative(self):
        return self._val == self.SA_CTL_MOVE_MODE_CL_RELATIVE or self._val == self.SA_CTL_MOVE_MODE_SCAN_RELATIVE

    def setAbsolute(self):
        if self._val == self.SA_CTL_MOVE_MODE_CL_RELATIVE:
            self._val = self.SA_CTL_MOVE_MODE_CL_ABSOLUTE
        elif self._val == self.SA_CTL_MOVE_MODE_SCAN_RELATIVE:
            self._val = self.SA_CTL_MOVE_MODE_SCAN_ABSOLUTE

    def setRelative(self):
        if self._val == self.SA_CTL_MOVE_MODE_CL_ABSOLUTE:
            self._val = self.SA_CTL_MOVE_MODE_CL_RELATIVE
        elif self._val == self.SA_CTL_MOVE_MODE_SCAN_ABSOLUTE:
            self._val = self.SA_CTL_MOVE_MODE_SCAN_RELATIVE

    def setClosedLoop(self):
        if self._val == self.SA_CTL_MOVE_MODE_SCAN_RELATIVE:
            self._val = self.SA_CTL_MOVE_MODE_CL_RELATIVE
        elif self._val == self.SA_CTL_MOVE_MODE_SCAN_ABSOLUTE:
            self._val = self.SA_CTL_MOVE_MODE_CL_ABSOLUTE
        else:
            self._val = self.SA_CTL_MOVE_MODE_CL_ABSOLUTE

    def setOpenLoop(self):
        if self._val == self.SA_CTL_MOVE_MODE_CL_RELATIVE:
            self._val = self.SA_CTL_MOVE_MODE_SCAN_RELATIVE
        elif self._val == self.SA_CTL_MOVE_MODE_CL_ABSOLUTE:
            self._val = self.SA_CTL_MOVE_MODE_SCAN_ABSOLUTE
        else:
            self._val = self.SA_CTL_MOVE_MODE_SCAN_ABSOLUTE

    def setStep(self):
        self._val = self.SA_CTL_MOVE_MODE_STEP


class ActuatorMode(SAEnum):

    SA_CTL_ACTUATOR_MODE_NORMAL = 0
    SA_CTL_ACTUATOR_MODE_QUIET = 1
    SA_CTL_ACTUATOR_MODE_LOW_VIBRATION = 2

    @classmethod
    def getEnum(self):
        return ["Normal", "Quiet", "Low vibration"]

    def isValid(self):
        return (self.val == self.SA_CTL_ACTUATOR_MODE_NORMAL or
                self.val == self.SA_CTL_ACTUATOR_MODE_QUIET or
                self.val == self.SA_CTL_ACTUATOR_MODE_LOW_VIBRATION)

    def isNormal(self):
        return self._val == self.SA_CTL_ACTUATOR_MODE_NORMAL

    def setNormal(self):
        self._val = self.SA_CTL_ACTUATOR_MODE_NORMAL

    def isQuiet(self):
        return self._val == self.SA_CTL_ACTUATOR_MODE_QUIET

    def setQuiet(self):
        self._val = self.SA_CTL_ACTUATOR_MODE_QUIET

    def isLowVibration(self):
        return self._val == self.SA_CTL_ACTUATOR_MODE_LOW_VIBRATION

    def setLowVibration(self):
        self._val = self.SA_CTL_ACTUATOR_MODE_LOW_VIBRATION


class SensorType(SAEnum):

    SA_CTL_REF_TYPE_NONE = 0
    SA_CTL_REF_TYPE_END_STOP = 1
    SA_CTL_REF_TYPE_SINGLE_CODED = 2
    SA_CTL_REF_TYPE_DISTANCE_CODED = 3

    @classmethod
    def getEnum(self):
        return ["None", "End stop", "Single coded", "Distance coded"]

    def isValid(self):
        return (self.val == self.SA_CTL_REF_TYPE_NONE or
                self.val == self.SA_CTL_REF_TYPE_END_STOP or
                self.val == self.SA_CTL_REF_TYPE_SINGLE_CODED or
                self.val == self.SA_CTL_REF_TYPE_DISTANCE_CODED)

    def isNone(self):
        return self._val == self.SA_CTL_REF_TYPE_NONE

    def isEndStop(self):
        return self._val == self.SA_CTL_REF_TYPE_END_STOP

    def isSingleCoded(self):
        return self._val == self.SA_CTL_REF_TYPE_SINGLE_CODED

    def isDistanceCoded(self):
        return self._val == self.SA_CTL_REF_TYPE_DISTANCE_CODED


class BaseUnit(SAEnum):

    SA_CTL_UNIT_NONE = 0x00000000
    SA_CTL_UNIT_METER = 0x00000002
    SA_CTL_UNIT_DEGREE = 0x00000003

    @classmethod
    def getEnum(self):
        return ["None", "", "Meter", "Degree"]

    def isValid(self):
        return (self.val == self.SA_CTL_UNIT_NONE or
                self.val == self.SA_CTL_UNIT_METER or
                self.val == self.SA_CTL_UNIT_DEGREE)

    def isNone(self):
        return self._val == self.SA_CTL_UNIT_NONE

    def isMeter(self):
        return self._val == self.SA_CTL_UNIT_METER

    def isDegree(self):
        return self._val == self.SA_CTL_UNIT_DEGREE


class DeviceState(SAEnum):
    SA_CTL_DEV_STATE_BIT_HM_PRESENT = 0x00000001
    SA_CTL_DEV_STATE_BIT_MOVEMENT_LOCKED = 0x00000002
    SA_CTL_DEV_STATE_BIT_INTERNAL_COMM_FAILURE = 0x00000100
    SA_CTL_DEV_STATE_BIT_IS_STREAMING = 0x00001000

    def __str__(self):
        return

    def hasHM(self):
        return self._val & self.SA_CTL_DEV_STATE_BIT_HM_PRESENT

    def isLocked(self):
        return self._val & self.SA_CTL_DEV_STATE_BIT_MOVEMENT_LOCKED

    def internalFailure(self):
        return self._val & self.SA_CTL_DEV_STATE_BIT_INTERNAL_COMM_FAILURE

    def isStreaming(self):
        return self._val & self.SA_CTL_DEV_STATE_BIT_IS_STREAMING


class ModuleState(SAEnum):
    SA_CTL_MOD_STATE_BIT_SM_PRESENT = 0x00000001
    SA_CTL_MOD_STATE_BIT_BOOSTER_PRESENT = 0x00000002
    SA_CTL_MOD_STATE_BIT_ADJUSTMENT_ACTIVE = 0x00000004
    SA_CTL_MOD_STATE_BIT_IOM_PRESENT = 0x00000008
    SA_CTL_MOD_STATE_BIT_INTERNAL_COMM_FAILURE = 0x00000100
    SA_CTL_MOD_STATE_BIT_FAN_FAILURE = 0x00000800
    SA_CTL_MOD_STATE_BIT_POWER_SUPPLY_FAILURE = 0x00001000
    SA_CTL_MOD_STATE_BIT_HIGH_VOLTAGE_FAILURE = 0x00001000
    SA_CTL_MOD_STATE_BIT_POWER_SUPPLY_OVERLOAD = 0x00002000
    SA_CTL_MOD_STATE_BIT_HIGH_VOLTAGE_OVERLOAD = 0x00002000
    SA_CTL_MOD_STATE_BIT_OVER_TEMPERATURE = 0x00004000

    def __str__(self):
        out = ""
        if self.hasSensorModule():
            out += "SM "
        if self.hasBooster():
            out += "BO "
        if self.isAdjusting():
            out += "ADJ "
        if self.hasIOModule():
            out += "IOM "
        if self.hasErrors():
            out += "Errors: "
            if self.internalFailure():
                out += "IF "
            if self.fanFailure():
                out += "FF "
            if self.psuFailure():
                out += "PF "
            if self.hvFailure():
                out += "HF "
            if self.psuOverload():
                out += "PO "
            if self.hvOverload():
                out += "HO "
            if self.overTemperature():
                out += "OT "
        return out[:-1]

    def hasErrors(self):
        mask = (self.SA_CTL_MOD_STATE_BIT_FAN_FAILURE |
                self.SA_CTL_MOD_STATE_BIT_HIGH_VOLTAGE_FAILURE |
                self.SA_CTL_MOD_STATE_BIT_HIGH_VOLTAGE_OVERLOAD |
                self.SA_CTL_MOD_STATE_BIT_INTERNAL_COMM_FAILURE |
                self.SA_CTL_MOD_STATE_BIT_OVER_TEMPERATURE |
                self.SA_CTL_MOD_STATE_BIT_POWER_SUPPLY_FAILURE |
                self.SA_CTL_MOD_STATE_BIT_POWER_SUPPLY_OVERLOAD)
        return bool(self._val & mask)

    def hasSensorModule(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_SM_PRESENT

    def hasBooster(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_BOOSTER_PRESENT

    def isAdjusting(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_ADJUSTMENT_ACTIVE

    def hasIOModule(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_IOM_PRESENT

    def internalFailure(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_INTERNAL_COMM_FAILURE

    def fanFailure(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_FAN_FAILURE

    def psuFailure(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_POWER_SUPPLY_FAILURE

    def hvFailure(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_HIGH_VOLTAGE_FAILURE

    def psuOverload(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_POWER_SUPPLY_OVERLOAD

    def hvOverload(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_HIGH_VOLTAGE_OVERLOAD

    def overTemperature(self):
        return self._val & self.SA_CTL_MOD_STATE_BIT_OVER_TEMPERATURE
