'''

    The Dragonfly is a NASA probe mission to Titan, a moon of Saturn, intended to map the
    geological features of the satellite (to be launched in 2027).

    In due time, when manned missions inevitably land on Titan, a major concern will be the utilization
    of local methane lakes to refuel launch vehicles for the ride back to earth.

    This program seeks to simulate the working of a part of the dragonfly probe meant to detect the
    existence of a methane lake by measuring the electrical conductivity of the probed area
    (liquid methane has a higher conductivity than solid methane or ice), and the atmospheric pressure
    & temperature in the area (for flagging it as a potential landing site) and uploading to thingspeak.

'''

import random
import urllib.request
import requests
import threading


def thingspeak_post_climate() :

    val1 = input('Input conductivity (1/ohm.cm) : ') # conductivity, this input would ideally be provided by probe instrumentation
    val2 = input('Input atmospheric pressure (Pa) : ') # pressure, this input would ideally be provided by probe instrumentation
    val3 = input('Input temperatureq (K) : ') # temperature, this input would ideally be provided by probe instrumentation

    url = 'https://api.thingspeak.com/update?api_key='
    key = 'SMXXYY94W8QJVTP3'
    header = '&field1={}&field2={}&field3={}'.format(val1, val2, val3)
    new_url = url + key + header
    print(new_url)
    print(requests.get(new_url))
    data = urllib.request.urlopen(new_url)
    print(data)

wish = True

while wish :
    thingspeak_post_climate()
    a = input('New entry? (1/0) : ')
    if a == '1' :
        wish = True
    elif a == '0' :
        wish = False
    else :
        print('Invalid entry. Program ended.')
        break