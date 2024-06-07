#include <Arduino.h>

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <ESPmDNS.h>
  #include <ESP32Servo.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESP8266mDNS.h>
  #include <Servo.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ESPAsyncHTTPUpdateServer.h>

#include <LittleFS.h>
#include "PCF8574.h"
#include <ArduinoJson.h>

#define DEBUG

#ifdef ESP32
  #define SDA_PIN 2
  #define SCL_PIN 4
  #define STEER_PIN 5
#elif defined(ESP8266)
  #define SDA_PIN 0
  #define SCL_PIN 2
  #define STEER_PIN 3
#endif

#define EXPANDER_I2C_ADDRESS 0x20
#define HEAD_LIGHT_PIN_E P0
#define BREAK_LIGHT_PIN_E P1
#define REVERSE_LIGHT_PIN_E P2
#define LEFT_SIGNAL_PIN_E P3
#define RIGHT_SIGNAL_PIN_E P4
#define MOTOR_PIN_1_E P5
#define MOTOR_PIN_2_E P6
#define HORN_PIN_E P7

#define IDNAME(name) #name

//#define STATION

#ifdef STATION
  const char *ssid = "wifi-ssid";
  const char *password = "wifi-password";
#else
  const char *ssid = "ESP-RC";
  const char *password = "esprc321";
#endif

const char *otaUsername = "esp-rc";
const char *otaPassword = "esp-rc";
const char *hostname = "ESP-RC";
const char *mdns = "esprc";
const long signalInterval = 500;

// Definitions
void listFiles();

void configRoutes(AsyncWebServer *server);
void notFound(AsyncWebServerRequest *request);

void getAll(AsyncWebServerRequest *request);
void updateGas(AsyncWebServerRequest *request);
void updateSignal(AsyncWebServerRequest *request);
void updateHeadLight(AsyncWebServerRequest *request);
void updateHorn(AsyncWebServerRequest *request);
void updateSteer(AsyncWebServerRequest *request);
void updateGear(AsyncWebServerRequest *request);

enum Signal
{
  OFF,
  LEFT,
  RIGHT,
  BOTH
};

enum Gear
{
  REVERSE,
  NEURTAL,
  DRIVE
};

AsyncWebServer _server(80);
ESPAsyncHTTPUpdateServer _updateServer;

PCF8574 _pcf8574(EXPANDER_I2C_ADDRESS, SDA_PIN, SCL_PIN);

Signal _signal = OFF;
Gear _gear = NEURTAL;
Servo _steer;