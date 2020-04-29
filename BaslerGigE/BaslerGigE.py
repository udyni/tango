#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
BaslerGigE
Device server to read out Basler GigE cameras with the Pylon library

@author: Michele Devetta <michele.devetta@cnr.it>
"""

import time
import re
import threading
import numpy as np

from pypylon import pylon

import PyTango as PT
import PyTango.server as PTS


class TangoImageHandler(pylon.ImageEventHandler):
    def __init__(self, parent):
        self.parent = parent
        pylon.ImageEventHandler.__init__(self)

    def OnImageGrabbed(self, camera, result):
        if result.GrabSucceeded():
            try:
                img = result.GetArray()
                self.parent.debug_stream("Got new image (dtype={0!s}, shape={1!s})".format(img.dtype, img.shape))
                if len(img.shape) > 2:
                    self.parent.warn_stream("Multilayer images are not supported. Selecting only the first layer")
                    img = np.squeeze(img[:, :, 0])
                self.parent.process_image(np.uint16(img))
            except Exception as e:
                self.parent.error_stream("Pixel format not supported ({0:d}). Error: {1!s}".format(result.GetPixelType(), e))
        else:
            self.parent.error_stream("Grab failed (Error[0:d]: {0})".format(result.GetErrorCode(), result.GetErrorDescription()))
        result.Release()


class GainConverter(object):
    """ Class to convert raw gain to dB for Basler cameras
    """
    def __init__(self):

        self.switcher = {
            'acA640-120gm': {
                'd': lambda gain: gain * 0.0359,
                'r': lambda gain: gain / 0.0359
            },
        }
        self.default = {'d': lambda gain: gain, 'r': lambda gain: gain}

    def db2raw(self, gain, model):
        handler = self.switcher.get(model, self.default)
        return handler['r'](gain)

    def raw2db(self, gain, model):
        handler = self.switcher.get(model, self.default)
        return handler['d'](gain)


class BaslerGigE(PTS.Device, metaclass=PTS.DeviceMeta):

    # Device properties
    cameraSerial = PTS.device_property(dtype=str)
    rois = PTS.device_property(dtype=[str, ], default_value=[])

    # Attributes
    ExposureTime = PTS.attribute(
                        label="Exposure Time", dtype=PT.DevDouble, format="%.1f",
                        access=PT.AttrWriteType.READ_WRITE, unit="us", memorized=True, hw_memorized=True,
                        doc="The exposure time of the camera", fisallowed='IsCameraOnline')

    Gain = PTS.attribute(
                        label="Gain", dtype=PT.DevDouble, format="%.1f",
                        access=PT.AttrWriteType.READ_WRITE, unit="dB", memorized=True, hw_memorized=True,
                        doc="The gain of the camera", fisallowed='IsCameraOnline')

    EnableTrigger = PTS.attribute(
                        label="Enable trigger", dtype=PT.DevBoolean,
                        access=PT.AttrWriteType.READ_WRITE, memorized=True, hw_memorized=True,
                        doc="Enable external hardware trigger", fisallowed='IsCameraOnline')

    def init_device(self):
        """ Initialize device
        """
        # Set INIT state
        self.set_state(PT.DevState.INIT)

        # Call parent init
        PTS.Device.init_device(self)

        # Enable change events on Image
        self.full_image = None

        # Roi images
        self.rois_img = []

        # Auto exposure monitoring thread
        self.ae_thread = None
        self.ae_cond = threading.Condition()

        # Gain converter
        self.gc = GainConverter()

        # Initialize camera
        self.debug_stream("Looking for camera with serial {0}".format(self.cameraSerial))
        tl_factory = pylon.TlFactory.GetInstance()
        for dev_info in tl_factory.EnumerateDevices():
            self.debug_stream("Found device {0} with serial {1}".format(dev_info.GetModelName(), dev_info.GetSerialNumber()))
            if dev_info.GetSerialNumber() == self.cameraSerial:
                # Found camera
                self.debug_stream("Found camera {0} ({1})".format(dev_info.GetModelName(), dev_info.GetAddress()))
                dev = tl_factory.CreateDevice(dev_info)
                self.camera = pylon.InstantCamera(dev)
                self.camera.Open()

                # NOTE: for testing purpose. Select a moving image when using emulator
                if dev_info.GetDeviceClass() == 'BaslerCamEmu':
                    self.camera.TestImageSelector.SetValue('Testimage2')

                # Register image handling callback
                h = TangoImageHandler(self)
                self.camera.RegisterImageEventHandler(h, pylon.RegistrationMode_Append, pylon.Cleanup_Delete)

                self.set_status("Camera {0} connected ({1})".format(dev_info.GetModelName(), dev_info.GetAddress()))
                self.t = time.time()
                break
        else:
            self.camera = None
            self.error_stream("No camera found")
            self.set_state(PT.DevState.FAULT)
            self.set_status("No camera")

    def delete_device(self):
        """ Delete device. Stop grabbing and close camera if needed
        """
        try:
            if self.camera is not None:
                if self.camera.IsGrabbing():
                    # Stop grabbing
                    self.camera.StopGrabbing()
                if self.camera.IsOpen():
                    # Close camera
                    self.camera.Close()
        except Exception as e:
            print("Exception deleting device:", e)

    def initialize_dynamic_attributes(self):
        """ Initialize dynamic attributes
        """
        if self.IsCameraOnline():

            # Get attributes
            multi_attr = self.get_device_attr()

            # Set minimum and maximum integration time
            attribute = multi_attr.get_attr_by_name("ExposureTime")
            multi_prop = attribute.get_properties()
            multi_prop.unit = self.camera.ExposureTimeAbs.Unit
            multi_prop.min_value = "{0:.1f}".format(self.camera.ExposureTimeAbs.GetMin())
            multi_prop.max_value = "{0:.1f}".format(self.camera.ExposureTimeAbs.GetMax())
            attribute.set_properties(multi_prop)

            # Set minimum and maximum gain
            attribute = multi_attr.get_attr_by_name("Gain")
            multi_prop = attribute.get_properties()
            if "Gain" in dir(self.camera):
                multi_prop.min_value = "{0:.1f}".format(self.camera.Gain.GetMin())
                multi_prop.max_value = "{0:.1f}".format(self.camera.Gain.GetMax())
            else:
                multi_prop.min_value = "{0:.1f}".format(self.gc.raw2db(self.camera.GainRaw.GetMin(), self.camera.DeviceModelName()))
                multi_prop.max_value = "{0:.1f}".format(self.gc.raw2db(self.camera.GainRaw.GetMax(), self.camera.DeviceModelName()))
            attribute.set_properties(multi_prop)

            # Create TriggerSource attribute
            attr = PT.Attr("TriggerSource", PT.DevEnum, PT.AttrWriteType.READ_WRITE)
            attr_prop = PT.UserDefaultAttrProp()
            attr_prop.label = "Trigger Source"
            attr_prop.set_enum_labels(list(self.camera.TriggerSource.GetSymbolics()))
            attr.set_default_properties(attr_prop)
            attr.set_memorized()
            attr.set_memorized_init(True)
            attr.set_change_event(True, False)
            self.add_attribute(attr, self.read_TriggerSource, self.write_TriggerSource, self.IsCameraOnline)

            # Create PixelFormat attribute
            attr = PT.Attr("PixelFormat", PT.DevEnum, PT.AttrWriteType.READ_WRITE)
            attr_prop = PT.UserDefaultAttrProp()
            attr_prop.label = "Pixel Format"
            attr_prop.set_enum_labels(list(self.camera.PixelFormat.GetSymbolics()))
            attr.set_default_properties(attr_prop)
            attr.set_memorized()
            attr.set_memorized_init(True)
            attr.set_change_event(True, False)
            self.add_attribute(attr, self.read_PixelFormat, self.write_PixelFormat, self.IsCameraOnline)

            # Create auto-exposure attribute
            if "ExposureAuto" in dir(self.camera):
                attr = PT.Attr("AutoExoposure", PT.DevEnum, PT.AttrWriteType.READ_WRITE)
                attr_prop = PT.UserDefaultAttrProp()
                attr_prop.label = "Auto exposure"
                attr_prop.set_enum_labels(list(self.camera.ExposureAuto.GetSymbolics()))
                attr.set_default_properties(attr_prop)
                attr.set_change_event(True, False)
                self.add_attribute(attr, self.read_AutoExposure, self.write_AutoExposure, self.IsCameraOnline)

                # Brightness target
                attr = PT.Attr("AutoBrightnessTarget", PT.DevUShort, PT.AttrWriteType.READ_WRITE)
                attr_prop = PT.UserDefaultAttrProp()
                attr_prop.label = "Auto brightness target"
                attr_prop.min_value = "{0:d}".format(self.camera.AutoTargetValue.GetMin())
                attr_prop.max_value = "{0:d}".format(self.camera.AutoTargetValue.GetMax())
                attr.set_default_properties(attr_prop)
                attr.set_memorized()
                attr.set_memorized_init(True)
                attr.set_change_event(True, False)
                self.add_attribute(attr, self.read_AutoBrightnessTarget, self.write_AutoBrightnessTarget, self.IsCameraOnline)

                # Exposure time lower limit
                attr = PT.Attr("AutoExposureLowerLimit", PT.DevDouble, PT.AttrWriteType.READ_WRITE)
                attr_prop = PT.UserDefaultAttrProp()
                attr_prop.label = "Auto exosure time lower limit"
                attr_prop.min_value = "{0:d}".format(self.camera.AutoExposureTimeAbsLowerLimit.GetMin())
                attr_prop.max_value = "{0:d}".format(self.camera.AutoExposureTimeAbsLowerLimit.GetMax())
                attr.set_default_properties(attr_prop)
                attr.set_memorized()
                attr.set_memorized_init(True)
                attr.set_change_event(True, False)
                self.add_attribute(attr, self.read_AutoExposureLowerLimit, self.write_AutoExposureLowerLimit, self.IsCameraOnline)

                # Exposure time upper limit
                attr = PT.Attr("AutoExposureUpperLimit", PT.DevDouble, PT.AttrWriteType.READ_WRITE)
                attr_prop = PT.UserDefaultAttrProp()
                attr_prop.label = "Auto exosure time lower limit"
                attr_prop.min_value = "{0:d}".format(self.camera.AutoExposureTimeAbsLowerLimit.GetMin())
                attr_prop.max_value = "{0:d}".format(self.camera.AutoExposureTimeAbsLowerLimit.GetMax())
                attr.set_default_properties(attr_prop)
                attr.set_memorized()
                attr.set_memorized_init(True)
                attr.set_change_event(True, False)
                self.add_attribute(attr, self.read_AutoExposureUpperLimit, self.write_AutoExposureUpperLimit, self.IsCameraOnline)

            # Create image attributes
            # NOTE: also the full image is created as a dynamic attribute because creating it as a static attribute hang the
            # image callback on push_change_event call
            attr = PT.ImageAttr("Image", PT.DevUShort, PT.AttrWriteType.READ, 2048, 2048)
            attr.set_change_event(True, False)
            self.add_attribute(attr, self.read_image)

            # Create one image for each defined ROIs
            for r in self.rois:
                m = re.match("(\d+):(\d+):(\d+):(\d+)", r)
                if m is not None:
                    try:
                        x = int(m.groups()[0])
                        y = int(m.groups()[1])
                        w = int(m.groups()[2])
                        h = int(m.groups()[3])

                        attr_name = "Image_{0:02d}".format(len(self.rois_img))
                        self.debug_stream("Adding attribute for roi [X: {0:d}, Y: {1:d}, W: {2:d}, H: {3:d}], named '{4}'".format(x, y, w, h, attr_name))

                        attr = PT.ImageAttr(attr_name, PT.DevUShort, PT.AttrWriteType.READ, 2048, 2048)
                        attr.set_change_event(True, False)
                        self.add_attribute(attr, self.read_roi_image)
                        self.rois_img.append({'x': x, 'y': y, 'w': w, 'h': h , 'name': attr_name, 'image': None})
                    except PT.DevFailed as e:
                        self.error_stream("Failed to create attribute. Error: {0!s}".format(e.args[0]))
                    except Exception as e:
                        self.error_stream("Failed to create attribute. Error: {0!s}".format(e))

            # Initialization done, we can start grabbing
            self.start_grabbing()

    def start_grabbing(self):
        """ Start image grab
        """
        try:
            self.camera.StartGrabbing(pylon.GrabStrategy_OneByOne, pylon.GrabLoop_ProvidedByInstantCamera)
            self.set_state(PT.DevState.RUNNING)
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error starting grab", msg, "BaslerGigE::start_grabbing()")

    def stop_grabbing(self):
        """ Stop image grab
        """
        if self.camera.IsGrabbing():
            self.camera.StopGrabbing()
            self.set_state(PT.DevState.STANDBY)
            return True
        else:
            return False

    def read_roi_image(self, attr):
        """ Read method for one ROI image
        """
        m = re.match("Image_(\d+)", attr.get_name())
        if m is not None:
            img_id = int(m.groups()[0])
            self.debug_stream("Reading ROI image {0:d}".format(img_id))
            if self.rois_img[img_id]['image'] is not None:
                attr.set_value(self.rois_img[img_id]['image'])
            else:
                self.info_stream("ROI image is empty")
        else:
            self.error_stream("Failed to parse attribute name {0}".format(attr.get_name()))

    def read_image(self, attr):
        """ Read method for the full image
        """
        if self.full_image is not None:
            attr.set_value(self.full_image)

    def process_image(self, image):
        """ Callback method to process a new image. Push change events.
        """
        if self.get_state() != PT.DevState.INIT:
            # Send full image change event
            self.full_image = image

            # Add event
            self.push_change_event("Image", image, image.shape[1], image.shape[0])

            # Process ROIs
            for i in range(len(self.rois_img)):
                try:
                    r = self.rois_img[i]
                    roi_img = image[r['x']:r['x']+r['w'], r['y']:r['y']+r['h']]

                    self.rois_img[i]['image'] = roi_img
                    self.push_change_event(r['name'], roi_img, roi_img.shape[1], roi_img.shape[0])
                except PT.DevFailed as e:
                    self.error_stream("Tango exception: {0!s}".format(e.args[0].desc))
                except Exception as e:
                    self.error_stream("Generic exception: {0!s}".format(e))

    def IsCameraOnline(self, request=None):
        """ Check if the camera is online and open
        """
        if self.camera is not None and self.camera.IsOpen():
            return True
        else:
            return False

    def read_ExposureTime(self):
        """ Read out camera exposure time
        """
        return self.camera.ExposureTimeAbs.GetValue()

    def write_ExposureTime(self, value):
        """ Set camera exposure time
        """
        if value == self.camera.ExposureTimeAbs.GetValue():
            # Noting to do
            return
        try:
            self.camera.ExposureTimeAbs.SetValue(value)
            self.push_change_event("ExposureTime", value)
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error setting exposure time", msg, "BaslerGigE::write_ExposureTime()")

    def read_Gain(self):
        """ Read camera gain
        """
        if "Gain" in dir(self.camera):
            return self.camera.Gain.GetValue()
        else:
            return self.gc.raw2db(self.camera.GainRaw.GetValue(), self.camera.DeviceModelName())

    def write_Gain(self, gain):
        """ Write camera gain
        """
        if "Gain" in dir(self.camera):
            if gain == self.camera.Gain.GetValue():
                # Noting to do
                return
        else:
            if gain == self.gc.raw2db(self.camera.GainRaw.GetValue(), self.camera.DeviceModelName()):
                # Noting to do
                return

        try:
            if "Gain" in dir(self.camera):
                self.camera.Gain.SetValue(gain)
            else:
                self.camera.GainRaw.SetValue(int(self.gc.db2raw(gain, self.camera.DeviceModelName())))
            self.push_change_event("Gain", gain)
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error setting gain", msg, "BaslerGigE::write_Gain()")

    def read_EnableTrigger(self):
        """ Read trigger status
        """
        if self.camera.TriggerMode.GetValue() == 'On':
            return True
        else:
            return False

    def write_EnableTrigger(self, enable):
        """ Enable/disable trigger
        """
        try:
            if enable:
                if 'FrameStart' in self.camera.TriggerSelector.GetSymbolics():
                    self.camera.TriggerSelector.SetValue('FrameStart')
                else:
                    self.camera.TriggerSelector.SetValue('AcquisitionStart')
                self.camera.TriggerMode.SetValue('On')
            else:
                self.camera.TriggerMode.SetValue('Off')
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error setting exposure time", msg, "BaslerGigE::write_EnableTrigger()")

    def read_TriggerSource(self, attr):
        """ Read trigger source
        """
        values = self.camera.TriggerSource.GetSymbolics()
        attr.set_value(values.index(self.camera.TriggerSource.GetValue()))

    def write_TriggerSource(self, attr):
        """ Set trigger source
        """
        try:
            # TODO
            pass
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error setting exposure time", msg, "BaslerGigE::write_TriggerSource()")

    def read_PixelFormat(self, attr):
        """ Read pixel format
        """
        values = self.camera.PixelFormat.GetSymbolics()
        attr.set_value(values.index(self.camera.PixelFormat.GetValue()))

    def write_PixelFormat(self, attr):
        """ Set pixel format
        """
        w_id = attr.get_write_value()
        w_val = self.camera.PixelFormat.GetSymbolics()[w_id]
        c_val = self.camera.PixelFormat.GetValue()
        if w_val == c_val:
            # No modification. Just return
            return

        try:
            # Need to stop grabbing
            running = self.stop_grabbing()
            self.camera.PixelFormat.SetValue(w_val)
            if running:
                self.start_grabbing()
            self.push_change_event("PixelFormat", w_id)
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error setting exposure time", msg, "BaslerGigE::write_PixelFormat()")

    def read_AutoExposure(self, attr):
        """ Read auto-exposure status
        """
        values = self.camera.ExposureAuto.GetSymbolics()
        attr.set_value(values.index(self.camera.ExposureAuto.GetValue()))

    def write_AutoExposure(self, attr):
        """ Enable auto exposure
        """
        w_id = attr.get_write_value()
        w_val = self.camera.ExposureAuto.GetSymbolics()[w_id]
        c_val = self.camera.ExposureAuto.GetValue()
        if w_val == c_val:
            # No modification. Just return
            return

        if self.ae_thread is not None:
            if self.ae_thread.is_alive():
                with self.ae_cond:
                    self.ae_cond.notify()
            self.ae_thread.join()
            self.ae_thread = None

        try:
            self.camera.ExposureAuto.SetValue(w_val)
            if w_val == 'Once':
                # Start monitoring thread
                self.ae_thread = threading.Thread(target=self.check_AutoExposure)
                self.ae_thread.start()

            self.push_change_event("AutoExposure", w_id)
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error setting auto exposure", msg, "BaslerGigE::write_AutoExposure()")

    def check_AutoExposure(self):
        while True:
            with self.ae_cond:
                timeout = self.ae_cond.wait(timeout=1.0)

            if not timeout:
                break

            if self.camera.ExposureAuto.GetValue() != 'Once':
                # Autoexposure ended. Push change event
                values = self.camera.ExposureAuto.GetSymbolics()
                self.push_change_event("AutoExposure", values.index(self.camera.ExposureAuto.GetValue()))
                break

    def read_AutoBrightnessTarget(self, attr):
        """ Read auto brightness target
        """
        attr.set_value(self.camera.AutoTargetValue.GetValue())

    def write_AutoBrightnessTarget(self, attr):
        """ Write auto brightness value
        """
        w_val = attr.get_write_value()
        c_val = self.camera.AutoTargetValue.GetValue()
        if w_val == c_val:
            # No modification. Just return
            return

        try:
            self.camera.AutoTargetValue.SetValue(w_val)
            self.push_change_event("AutoBrightnessTarget", w_val)
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error setting auto exposure", msg, "BaslerGigE::write_AutoBrightnessTarget()")

    def read_AutoExposureLowerLimit(self, attr):
        """ Read auto brightness target
        """
        attr.set_value(self.camera.AutoExposureTimeAbsLowerLimit.GetValue())

    def write_AutoExposureLowerLimit(self, attr):
        """ Write auto brightness value
        """
        w_val = attr.get_write_value()
        c_val = self.camera.AutoExposureTimeAbsLowerLimit.GetValue()
        if w_val == c_val:
            # No modification. Just return
            return

        try:
            self.camera.AutoExposureTimeAbsLowerLimit.SetValue(w_val)
            self.push_change_event("AutoExposureLowerLimit", w_val)
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error setting auto exposure", msg, "BaslerGigE::write_AutoExposureLowerLimit()")

    def read_AutoExposureUpperLimit(self, attr):
        """ Read auto brightness target
        """
        attr.set_value(self.camera.AutoExposureTimeAbsUpperLimit.GetValue())

    def write_AutoExposureUpperLimit(self, attr):
        """ Write auto brightness value
        """
        w_val = attr.get_write_value()
        c_val = self.camera.AutoExposureTimeAbsUpperLimit.GetValue()
        if w_val == c_val:
            # No modification. Just return
            return

        try:
            self.camera.AutoExposureTimeAbsUpperLimit.SetValue(w_val)
            self.push_change_event("AutoExposureUpperLimit", w_val)
        except pylon.GenericException as e:
            try:
                msg = e.GetDescription()
            except:
                msg = str(e)
            PT.Except.throw_exception("Error setting auto exposure", msg, "BaslerGigE::write_AutoExposureUpperLimit()")

    @PTS.command()
    def Trigger(self):
        """ Software trigger camera
        """
        if self.IsCameraOnline() and self.camera.TriggerSource.GetValue() == 'Software':
            self.camera.TriggerSoftware.Execute();
        else:
            PT.Except.throw_exception("Cannot trigger camera", "Camera is offline or trigger source is not set to software", "BaslerGigE::Trigger()")


if __name__ == "__main__":
    # Start device server
    try:
        PTS.run( (BaslerGigE, ) )
    except PT.DevFailed as e:
        print("Tango exception: {:}".format(e.args[0].desc))
    except Exception as e:
        print("Python exception: {:}".format(repr(e)))
    except str as e:
        print("String exception: {:}".format(e))