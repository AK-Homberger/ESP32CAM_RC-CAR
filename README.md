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

After initial programming you can du uploads via OTA.

# Parts
- ESP32-CAM [Link](https://www.reichelt.de/index.html?ACTION=446&LA=2&nbc=1&q=esp32cam)
- Car Kit [Link](https://www.reichelt.de/roboter-fahrgestell-kit-fuer-alle-arduino-systeme-robot-car-kit-01-p219024.html?&nbc=1)
- Motor Driver [Link](https://www.reichelt.de/entwicklerboards-motodriver2-l298n-debo-motodriver2-p202829.html?&nbc=1)
- 7805 [Link](https://www.reichelt.de/festspannungsregler-5-v-1-5-a-to-220-3-ua-7805-ckcs-p189073.html?&nbc=1)
- USB to Serial Adapter [Link1](https://www.reichelt.de/entwicklerboards-microusb-buchse-auf-uart-ft232-debo-musb2uart-3-p266053.html?&nbc=1) or [Link2](https://www.amazon.de/dp/B07R17BMTL/ref=sspa_dk_detail_2?psc=1&pd_rd_i=B07R17BMTL&pd_rd_w=ctPSK&pf_rd_p=4060291c-d237-411a-a3fe-4e44df687a4d&pd_rd_wg=enKhZ&pf_rd_r=B02DJZMKW4QRHKBN627Q&pd_rd_r=829304f2-0444-4fec-88dc-e2af9109243b&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUExRVI5V0dQMjFTMjg2JmVuY3J5cHRlZElkPUEwOTIzOTg1MUhUSVNEQ1M2T0lSVSZlbmNyeXB0ZWRBZElkPUEwNTM1MjQ4SldZRlhJQlI0UzNVJndpZGdldE5hbWU9c3BfZGV0YWlsJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==)
