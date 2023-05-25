# ESP32CAM_RC-CAR

This repository shows how to build a WLAN controlled RC Car with an ESP32CAM.

![RC-Car1](RC-Car1.JPG)

## Wiring
![Wiring](ESP32-CAM-RC-Car.jpg)

## Web Control
![Interface](Web-Interface.png)

## ESP32CAM pinout
![ESP32-CAM-pinout-new.png](ESP32-CAM-pinout-new.png "pinout")

## Programming
You need an external (FTDI) programmer to install the (initial) sketch on the ESP32-CAM module. You have to set the voltage of the adapter to 5 Volt.

![FTDI](https://github.com/AK-Homberger/Alexa-Alarm-System-ESP32CAM/blob/main/Pictures/ESP32-CAM-FTDI-v2.png)

For programming you have to connect IO0 to GND, RX/TX (crossed) and 5V/GND. If you have the power supply already connected, then do not connect the 5V cable from the adapter. Then press the "RST" button on the ESP32-CAM. After that, you can start the upload process within the Arduino IDE (with FTDI USB-Serial port selected). For normal start open IO0 from GND and press "RST" again.
