#!/usr/bin/env python

import os
import urllib2
import string

def updateHue(hue, value):
	hueTemplate = string.Template('{"hue": $hue}')
        hueString = hueTemplate.substitute(hue=hue)
	opener = urllib2.build_opener(urllib2.HTTPHandler)
	request = urllib2.Request("http://example.com/api/username/lights/3/state", data=hueString)
	request.get_method = lambda: 'PUT'
	print opener.open(request).read()

os.system("stty -F /dev/ttyAMA0 9600")
p = open('/dev/ttyAMA0', "r")
while 1:
	line = p.readline()
	if line == "Switch on\n":
		updateHue(0, "ON")
	if line == "Switch off\n":
		updateHue(20000, "OFF")

