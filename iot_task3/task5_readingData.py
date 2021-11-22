'''

    The Dragonfly is a NASA probe mission to Titan, a moon of Saturn, intended to map the
    geological features of the satellite (to be launched in 2027).

    In due time, when manned missions inevitably land on Titan, a major concern will be the utilization
    of local methane lakes to refuel launch vehicles for the ride back to earth.

    This program seeks to simulate the working of a part of the dragonfly probe control center where
    data is requested from the thingspeak api (uploaded to by the task5_updatingData.py program)
    for processing.

'''

import requests
import json

url = "https://api.thingspeak.com/channels/1560694/feeds.json?results=2"
response = requests.request("GET", url)

line = json.loads(response.text)
#print(line)

data1 = str(line['feeds'][len(line['feeds']) - 1]['field1']) # latest conductivity
data2 = str(line['feeds'][len(line['feeds']) - 1]['field2']) # latest pressure
data3 = str(line['feeds'][len(line['feeds']) - 1]['field3']) # latest temperature

print('Latest conductivity measurement (1/ohm.cm) : ', data1)
print('Latest pressure measurement (Pa) : ', data2)
print('Latest temperature measurement (K) : ', data3)


