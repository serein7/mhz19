# openag_mhz19
This repository contains an OpenAg firmware module for reading from the NDIR CO2 sensor based on MH-Z19 chip.
The module defines 1 output, "air_carbon_dioxide" on which reading is sent once per 5 seconds. The module will enter an ERROR state whenever it fails to read from the sensor.

Current implementation is hardcoded to read sensor values from Arduino Mega2560 Serial2 interface.

# Todo
1. Add arguments to configure interface type (Serial or PWM) and pins the sensor is attached to.
