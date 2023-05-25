/*
  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// ESP32-CAM Remote Controlled Car

// Just change the WLAN credential to use it!!
// No additional libraries have to be installed for this project
// Make sure you have selected ESP32 Wrover Module in Arduino IDE

#include <esp_wifi.h>
#include <esp_camera.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
#include <ArduinoOTA.h>

// Select camera model
#define CAMERA_MODEL_AI_THINKER

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Change WLAN credentials according to your local settings
const char* ssid = "ssid";
const char* password = "password";

// Pin definitions for both motors and servo
const int MotPin0 = 15;  // Left +
const int MotPin1 = 14;  // Left -
const int MotPin2 = 13;  // Right -
const int MotPin3 = 12;  // Right +
const int ServoPin = 2;  // Pin for camera up/down servo

void startCameraServer();// Forward declaration of function


//*****************************************************************************
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // prevent brownouts by silencing them

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);
  s->set_vflip(s, 0);
  s->set_hmirror(s, 0);

  // Init Remote Control Car
  initMotors();
  initServo();

  ledcSetup(7, 5000, 8);
  ledcAttachPin(4, 7);  // pin4 is LED
  
  // Init WiFi
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("RC-Car");
  WiFi.begin(ssid, password);
  delay(500);

  long int StartTime = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    if ((StartTime + 10000) < millis()) break;
  }

  /*
    int8_t power;
    esp_wifi_set_max_tx_power(20);
    esp_wifi_get_max_tx_power(&power);
    Serial.printf("wifi power: %d \n",power);
  */

  startCameraServer();

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");
  } else {
    Serial.println("");
    Serial.println("WiFi disconnected");
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.softAPIP());    // Start Access Point
    Serial.println("' to connect");
    char* apssid = "ESP32-Car";
    char* appassword = "12345678";         // AP password require at least 8 characters.
    WiFi.softAP((WiFi.softAPIP().toString() + "_" + (String)apssid).c_str(), appassword);
  }

  for (int i = 0; i < 5; i++)
  {
    ledcWrite(7, 10); // flash led
    delay(200);
    ledcWrite(7, 0);
    delay(200);
  }

  // Arduino OTA config and start
  ArduinoOTA.setHostname("RC-Car");
  ArduinoOTA.begin();

  // Arduino MDNS config and start
  MDNS.begin("RC-Car");
  MDNS.addService("http", "tcp", 80);
}


//*****************************************************************************
// Initialise the motors
// Define pulse width modulation channels and attach to defined motor pins

void initMotors()
{
  ledcSetup(3, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(4, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(5, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(6, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcAttachPin(MotPin0, 3); // Left +
  ledcAttachPin(MotPin1, 4); // left -
  ledcAttachPin(MotPin2, 5); // Right -
  ledcAttachPin(MotPin3, 6); // Right +
}


//*****************************************************************************
// Initialise the servo
// Define pulse width modulation channel and attach to defined servo pin

void initServo()
{
  ledcSetup(8, 50, 16); // 50 hz PWM, 16-bit resolution, range from 3250 to 6500.
  ledcAttachPin(ServoPin, 8);
  ledcWrite(8, 10 * 520);
}


//*****************************************************************************
void loop() {
  ArduinoOTA.handle();
}
