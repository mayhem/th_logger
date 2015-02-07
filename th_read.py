#!/usr/bin/env python

import smbus
import json
from time import sleep

SENSOR_BUS = 1
SENSOR_ADDRESS = 0x27

sensor = smbus.SMBus(SENSOR_BUS)

data = sensor.read_i2c_block_data(SENSOR_ADDRESS, 1, 4)

h = ((data[0] & 0x3f) << 8) | data[1];
hum = float(h) / 16382.0
t = ((data[2] << 8) | data[3]) >> 2;
temp = (float(t) / 16382) * 165 - 40;

print json.dumps({ 'humidity' : "%.3f" % hum , 'temperature' : "%.3f" % temp })
