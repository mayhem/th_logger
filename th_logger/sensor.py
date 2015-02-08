#!/usr/bin/env python

import smbus
import config
from models import DataLog

class TempHumSensor(object):
    SENSOR_BUS = 1
    SENSOR_ADDRESS = 0x27

    def read(self):
        sensor = smbus.SMBus(self.SENSOR_BUS)
        data = sensor.read_i2c_block_data(self.SENSOR_ADDRESS, 1, 4)
        h = ((data[0] & 0x3f) << 8) | data[1];
        hum = float(h) / 16382.0
        t = ((data[2] << 8) | data[3]) >> 2;
        temp = (float(t) / 16382) * 165 - 40;

        return { 'humidity' : hum , 'temperature' : temp }

if __name__ == "__main__":
    try:
        print TempHumSensor().read()
    except IOError:
        print "This must be run as root."
