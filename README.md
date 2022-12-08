# ESP32-CAM_TANK

Esp32 controlling tracked vehicle while streaming video.
Serial console tells you where to connect. And use your own wifi credentials, not mine.

Wiring:
![esp32cam.jpg](esp32cam.jpg "Wiring")
For the $2 motor dirver search "L298N Motor Driver".

Control:
![DSC02367.jpg](DSC02367.jpg "Control")

Build:
![DSC02365.jpg](DSC02365.jpg "Build")
ESP-Cam, motor driver, 5V switching regulator.

Extern antenna:
![DSC02372.jpg](DSC02372.jpg "extant")

ESP32£CAM pinout:
![ESP32-CAM-pinout-new.png](ESP32-CAM-pinout-new.png "pinout")
If you experience streaming problems try anther xclk frequency like 21mhz or 19mhz. No idea why.

Line: config.xclk_freq_hz = 20000000; // tweak when wifi problems

Video:
https://youtu.be/qUAGnk382mc
