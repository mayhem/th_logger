#!/usr/bin/env python

import subprocess
import config
import json
from peewee import SqliteDatabase
from models import DataLog
from flask_debugtoolbar import DebugToolbarExtension

from flask import Flask, render_template
app = Flask(__name__,
            static_url_path = "/static",
            static_folder = "../static",
            template_folder = "../template")

app.config['SECRET_KEY'] = config.SECRET_KEY
app.debug = True
toolbar = DebugToolbarExtension(app)

@app.route('/now')
def now():
    entry = DataLog.select().order_by(DataLog.timestamp.desc()).get()

    temp_C = "%.1f" % entry.temp
    temp_F = "%.1f" % (entry.temp * 1.8 + 32)
    hum = "%.1f" % (entry.hum * 100)
    return render_template("now", title = "%sC/%s%%" % (temp_C, hum), temp_C = temp_C, hum = hum, temp_F = temp_F)

@app.route('/')
def index():
    return render_template("index")

@app.route('/api/last-24-hr')
def api_last_24_hours():
    data = []
    entries = DataLog.select().order_by(DataLog.timestamp.desc()).limit(1440)
    for entry in entries:
        data.append({ 't' : "%.1f" % entry.temp, 'h' : "%.1f" % entry.hum, 'ts' : str(entry.timestamp) })
    return json.dumps(data) 

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=8000, debug=True)
