#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#include <LittleFS.h>
#include "PCF8574.h"
#include <Servo.h>
#include <ArduinoJson.h>

// #define DEBUG

#define SDA_PIN 0
#define SCL_PIN 2
#define STEER_PIN 3

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

const char *ssid = "ESP8266RC";
const char *password = "ESP8266RC";
// const char *ssid = ":|-";
// const char *password = "W0rkH4rdP1ayH4rDer";
const char *hostname = "ESP-8266-RC";
const char *mdns = "esp8266rc";
const long signalInterval = 1000;


// Definitions
void listFiles();

void configRoutes(ESP8266WebServer *server);
void handleRoot();
void handleStatic();

void getAll();
void updateGas();
void updateSignal();
void updateHeadLight();
void updateHorn();
void updateSteer();
void updateGear();

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

ESP8266WebServer _server(80);
ESP8266HTTPUpdateServer _httpUpdater;

PCF8574 _pcf8574(EXPANDER_I2C_ADDRESS, SDA_PIN, SCL_PIN);

Signal _signal = OFF;
Gear _gear = NEURTAL;
Servo _steer;