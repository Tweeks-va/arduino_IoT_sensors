# arduino_IoT_sensors
<IMG src=misc/Workshop-graphic-overview.png width=80%>
This is the letscodeblacksburg.org "arduino_IoT_sensors" project for intermediate level arduino hackers.  It leverages the ESP8266 (ESP01) WiFi module to get your IoT (Internet of Things) sensors onto your local WiFi network and serve a very simple web-server (using WiFi ESP by bportaluri https://github.com/bportaluri/WiFiEsp), allowing you monitor things in your home (or datacenter) such as humidity, temperature, water on the floor, motion, etc.Using the WiFiESP plus SMTP support, you can set thresholds and send email/SMS alerts, etc.. or build in automation for things like shutting off water mains, etc.

## Required Parts (most available on Amazon for around $40 US):

<IMG SRC=misc/LCBB_sensor-pinouts.png width=300 align=right>

* 1 – Arduino board (with extra sensorpin headers or sensor shield & USB cable)
* 1 – Dedicated 9v DC wall PSU
* 1 – Passive buzzer
* 1 – Temp+Humidity DHT-22  sensor
* 1 – PIR Motion Sensor HC-SR501 
* 1 – Soil/Floor Water HL-69  sensor
* 1 – WiFi module ESP8266 (ESP01) + 3.3v→5v adapter module
* 11 – F/F Dupont connection wires
* 6 – M/F Dupont connection wires
* (opt) – project case, hot glue, etc

## Required Workshop Documentation:

* The [LCBB IoT Sensors Build Doc](LCBB_IoT_Environment_Monitor_BuildDoc.pdf)
* The [LCBB Arduino Cookbook PDF](https://github.com/LetsCodeBlacksburg/arduino-recipes/blob/master/LCBB_Arduino_Cookbook.pdf)


The build doc is the working document for the class (~6 pages) which relies on the sensor 

All LCBB code and related materials are copy left (open sourced) under the Creative Commons (CC)(BY)(SA)[1] License.

[1] - https://en.wikipedia.org/wiki/Creative_Commons_license#Types_of_licenses
