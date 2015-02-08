#!/usr/bin/env python

import sys
import datetime
import config
from peewee import SqliteDatabase
from models import DataLog
from sensor import TempHumSensor

try:
    data = TempHumSensor().read()
except IOError:
    print "This must be run as root."
    sys.exit(-1)

database = SqliteDatabase(config.DB_FILE, threadlocals=True)

entry = DataLog.create(temp = data['temperature'], hum=data['humidity'], timestamp=datetime.datetime.now())
entry.save()
