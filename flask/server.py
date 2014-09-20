#!/usr/bin/env python

import subprocess

from flask import Flask, render_template
app = Flask(__name__,
            static_url_path = "/static",
            static_folder = "static",
            template_folder = "template")

@app.route('/')
def index():
    while True:
        try:
            data = subprocess.check_output(["th_logger"])
        except subprocess.CalledProcessError:
            continue
        break
    temp, hum = data.split(",")
    temp_F = float(temp) * 1.8 + 32
    return '''<html><head><title>%sC/%s%%</title><link rel="shortcut icon" href="/static/images/favicon.gif"><meta http-equiv="refresh" content="60"></head><body>
           <div style="text-align: center; font-family: Arial, Helvetica, sans-serif; font-size: 35pt">
           %s C / %.1f F<br>%s%% humidity</div></body></html>''' % (temp, hum, temp, temp_F, hum)

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=80, debug=True)
