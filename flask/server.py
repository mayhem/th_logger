#!/usr/bin/env python

import subprocess

from flask import Flask
app = Flask(__name__)

@app.route('/')
def index():
    data = subprocess.check_output(["th_logger"])
    temp, hum = data.split(",")
    temp_F = float(temp) * 1.8 + 32
    return '''<html><head><meta http-equiv="refresh" content="60"></head><body>
           <div style="text-align: center; font-family: Arial, Helvetica, sans-serif; font-size: 35pt">
           %s C / %.1f F<br>%s%% humidity</div></body></html>''' % (temp, temp_F, hum)

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=8080, debug=True)
