#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 23 18:29:21 2021

@author: wyrdmeister
"""

import time
import h5py as h5
import numpy as np
import PyTango as PT


dev = PT.DeviceProxy("udyni/monitoring/accelerometer1")

period = dev.Period
print("Sample period:", period)
fstep = dev.FrequencyStep
print("Frequency step:", fstep)

out_x = [];
out_y = [];
out_z = [];
raw_x = [];
raw_y = [];
raw_z = [];

def accumulate_callback(ev):
    if not ev.err:
        attr_name = ev.attr_value.name.lower()
        if attr_name == "x_spectrum":
            out_x.append(ev.attr_value.value)
            print("Got X event n.", len(out_x), " Shape:", str(ev.attr_value.value.shape))

        elif attr_name == "y_spectrum":
            out_y.append(ev.attr_value.value)
            print("Got Y event n.", len(out_y), " Shape:", str(ev.attr_value.value.shape))

        elif attr_name == "z_spectrum":
            out_z.append(ev.attr_value.value)
            print("Got Z event n.", len(out_z), " Shape:", str(ev.attr_value.value.shape))

        elif attr_name == "rawx":
            raw_x.append(ev.attr_value.value)
            print("Got X raw event n.", len(raw_x), " Shape:", str(ev.attr_value.value.shape))

        elif attr_name == "rawy":
            raw_y.append(ev.attr_value.value)
            print("Got Y raw event n.", len(raw_y), " Shape:", str(ev.attr_value.value.shape))

        elif attr_name == "rawz":
            raw_z.append(ev.attr_value.value)
            print("Got Z raw event n.", len(raw_z), " Shape:", str(ev.attr_value.value.shape))

        else:
            print("Unforseen event from attribute: ", attr_name)

    else:
        print("Event error: ", ev.errors[0].desc)

id1 = dev.subscribe_event("X_spectrum", PT.EventType.CHANGE_EVENT, accumulate_callback)
id2 = dev.subscribe_event("Y_spectrum", PT.EventType.CHANGE_EVENT, accumulate_callback)
id3 = dev.subscribe_event("Z_spectrum", PT.EventType.CHANGE_EVENT, accumulate_callback)
id4 = dev.subscribe_event("RawX", PT.EventType.CHANGE_EVENT, accumulate_callback)
id5 = dev.subscribe_event("RawY", PT.EventType.CHANGE_EVENT, accumulate_callback)
id6 = dev.subscribe_event("RawZ", PT.EventType.CHANGE_EVENT, accumulate_callback)


while out_x is None or len(out_x) < 100:
    time.sleep(1)

dev.unsubscribe_event(id1)
dev.unsubscribe_event(id2)
dev.unsubscribe_event(id3)
dev.unsubscribe_event(id4)
dev.unsubscribe_event(id5)
dev.unsubscribe_event(id6)

f = h5.File("test.h5", "w")

f.create_dataset("period", data=period)
f.create_dataset("fstep", data=fstep)
f.create_dataset("X", data=np.stack(out_x))
f.create_dataset("Y", data=np.stack(out_y))
f.create_dataset("Z", data=np.stack(out_z))
f.create_dataset("rawX", data=np.stack(raw_x))
f.create_dataset("rawY", data=np.stack(raw_y))
f.create_dataset("rawZ", data=np.stack(raw_z))

f.close()