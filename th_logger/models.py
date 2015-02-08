#!/usr/bin/env python

import config
from peewee import SqliteDatabase, Model, FloatField, DateTimeField

database = SqliteDatabase(config.DB_FILE)

class BaseModel(Model):
    class Meta:
        database = database

class DataLog(BaseModel):
    temp = FloatField()
    hum = FloatField()
    timestamp = DateTimeField(index=True)

    class Meta:
        order_by = ('timestamp',)

if __name__ == "__main__":
     database.connect()
     database.create_tables([DataLog])
