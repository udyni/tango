# -*- coding: utf-8 -*-
"""
Created on Thu Oct 29 15:51:46 2015

@author: wyrdmeister
"""

import csv

# Open stage database
f = open('StageDatabase.csv')

# Sniff CSV dialect
dialect = csv.Sniffer().sniff(f.read(1024), delimiters=";,")
f.seek(0)

# Read CSV
reader = csv.reader(f, dialect)

# Get stage names
names = reader.next()

for i in range(3,len(names)):
    print "%d: stage %s" % (i-2, names[i])

selection = int(raw_input("Select the stage for which you want the configuration string: "))

if selection > 0 and selection < len(names)-2:
    # We have a good index
    output = "%s=%s," % (names[1], names[selection+2])
    for par in reader:
        output += "%s=%s," % (par[1], par[selection+2])
    # Remove last comma
    output = output[:-1]

    print "Configuration string:"
    print output
    
else:
    print "Selection out of range"

f.close()
