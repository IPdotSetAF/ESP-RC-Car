#include "main.h"

// void listFiles()
// {
// #ifdef DEBUG
//   Serial.println("Listing files:");
//   Dir dir = LittleFS.openDir("/");
//   while (dir.next())
//   {
//     Serial.print("  FILE: ");
//     Serial.println(dir.fileName());
//   }
//   Serial.println("End of file list");
// #endif
// }

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found.");
}

// MARK:STATUS
void getAll(AsyncWebServerRequest *request)
{
  JsonDocument result;

  result["headLight"] = !_pcf8574.digitalRead(HEAD_LIGHT_PIN_E);
  switch (_gear)
  {
  case DRIVE:
    result["gear"] = "D";
    break;
  case REVERSE:
    result["gear"] = "R";
    break;
  case NEURTAL:
    result["gear"] = "N";
    break;
  }
  switch (_signal)
  {
  case LEFT:
    result["signal"] = "left";
    break;
  case RIGHT:
    result["signal"] = "right";
    break;
  case BOTH:
    result["signal"] = "both";
    break;
  case OFF:
    result["signal"] = "off";
    break;
  }

  String jsonResult;
  serializeJson(result, jsonResult);
  request->send(200, "application/json", jsonResult);
}

// MARK:GAS
void updateGas(AsyncWebServerRequest *request)
{
#ifdef DEBUG
  Serial.println(request->pathArg(0));
#endif
  String gas = request->pathArg(0);
  if (gas == "1" || gas == "true")
  {
    switch (_gear)
    {
    case DRIVE:
      _pcf8574.digitalWrite(MOTOR_PIN_1_E, LOW);
      _pcf8574.digitalWrite(MOTOR_PIN_2_E, HIGH);
      _pcf8574.digitalWrite(BREAK_LIGHT_PIN_E, HIGH);
      break;
    case REVERSE:
      _pcf8574.digitalWrite(MOTOR_PIN_1_E, HIGH);
      _pcf8574.digitalWrite(MOTOR_PIN_2_E, LOW);
      _pcf8574.digitalWrite(BREAK_LIGHT_PIN_E, HIGH);
      break;
    case NEURTAL:
      break;
    }
  }
  else if (gas == "0" || gas == "false")
  {
    _pcf8574.digitalWrite(MOTOR_PIN_1_E, LOW);
    _pcf8574.digitalWrite(MOTOR_PIN_2_E, LOW);
    _pcf8574.digitalWrite(BREAK_LIGHT_PIN_E, LOW);
  }
  else
    request->send(400, "text/plain", "bad request.");

  request->send(200, "text/plain", "ok");
}

// MARK:SIGNAL
void updateSignal(AsyncWebServerRequest *request)
{
#ifdef DEBUG
  Serial.println(request->pathArg(0));
#endif
  String signal = request->pathArg(0);
  if (signal == "left")
    _signal = LEFT;
  else if (signal == "both")
    _signal = BOTH;
  else if (signal == "right")
    _signal = RIGHT;
  else if (signal == "off")
    _signal = OFF;
  else
    request->send(400, "text/plain", "bad request.");

  request->send(200, "text/plain", "ok");
}

// MARK:HEAD LIGHT
void updateHeadLight(AsyncWebServerRequest *request)
{
#ifdef DEBUG
  Serial.println(request->pathArg(0));
#endif
  String light = request->pathArg(0);
  if (light == "1" || light == "true")
    _pcf8574.digitalWrite(HEAD_LIGHT_PIN_E, LOW);
  else if (light == "0" || light == "false")
    _pcf8574.digitalWrite(HEAD_LIGHT_PIN_E, HIGH);
  else
    request->send(400, "text/plain", "bad request.");

  request->send(200, "text/plain", "ok");
}

// MARK:HORN
void updateHorn(AsyncWebServerRequest *request)
{
#ifdef DEBUG
  Serial.println(request->pathArg(0));
#endif
  String horn = request->pathArg(0);
  if (horn == "1" || horn == "true")
    _pcf8574.digitalWrite(HORN_PIN_E, LOW);
  else if (horn == "0" || horn == "false")
    _pcf8574.digitalWrite(HORN_PIN_E, HIGH);
  else
    request->send(400, "text/plain", "bad request.");

  request->send(200, "text/plain", "ok");
}

void updateSteer(AsyncWebServerRequest *request)
{
  int angle = request->pathArg(0).toInt();
  int value = map(angle, -90, 90, 0, 180);
  _steer.write(value);

#ifdef DEBUG
  Serial.println((String)angle + " : steering to : " + (String)value);
#endif

  request->send(200, "text/plain", "ok");
  //   server.send(400, "text/plain", "bad request.");
}

// MARK:GEAR
void updateGear(AsyncWebServerRequest *request)
{
#ifdef DEBUG
  Serial.println(request->pathArg(0));
#endif
  String gear = request->pathArg(0);
  if (gear == "d" || gear == "D")
  {
    _gear = DRIVE;
    _pcf8574.digitalWrite(REVERSE_LIGHT_PIN_E, HIGH);
  }
  else if (gear == "n" || gear == "N")
  {
    _gear = NEURTAL;
    _pcf8574.digitalWrite(REVERSE_LIGHT_PIN_E, HIGH);
  }
  else if (gear == "r" || gear == "R")
  {
    _gear = REVERSE;
    _pcf8574.digitalWrite(REVERSE_LIGHT_PIN_E, LOW);
  }
  else
    request->send(400, "text/plain", "bad request.");

  request->send(200, "text/plain", "ok");
}

// MARK:ROUTES
void configRoutes(AsyncWebServer *server)
{
  server->on("^\\/api\\/all$", HTTP_GET, [](AsyncWebServerRequest *request)
             { getAll(request); });
  server->on("^\\/api\\/gas\\/(true|false|1|0)$", HTTP_PUT, [](AsyncWebServerRequest *request)
             { updateGas(request); });
  server->on("^\\/api\\/signal\\/(off|left|right|both)$", HTTP_PUT, [](AsyncWebServerRequest *request)
             { updateSignal(request); });
  server->on("^\\/api\\/headLight\\/(true|false|1|0)$", HTTP_PUT, [](AsyncWebServerRequest *request)
             { updateHeadLight(request); });
  server->on("^\\/api\\/horn\\/((true|false|1|0))$", HTTP_PUT, [](AsyncWebServerRequest *request)
             { updateHorn(request); });
  server->on("^\\/api\\/steer\\/(-{0,1}[0-9]+)$", HTTP_PUT, [](AsyncWebServerRequest *request)
             { updateSteer(request); });
  server->on("^\\/api\\/gear\\/([drnDRN]{1})$", HTTP_PUT, [](AsyncWebServerRequest *request)
             { updateGear(request); });

  server->serveStatic("/", LittleFS, "/www/").setDefaultFile("index.html");

  server->onNotFound(notFound);
}

// MARK:SETUP
void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif
  WiFi.hostname(hostname);

#ifdef STATION
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
#ifdef DEBUG
    Serial.println("Connecting to WiFi...");
#endif
  }
#ifdef DEBUG
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
#endif
#else
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
#ifdef DEBUG
  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
#endif
#endif

#ifdef DEBUG
  if (MDNS.begin(mdns))
    Serial.println("mDNS responder started");
#else
  MDNS.begin(mdns);
#endif

  LittleFS.begin();

  // listFiles();

  configRoutes(&_server);
  _updateServer.setup(&_server, otaUsername, otaPassword);
  _server.begin();

#ifdef DEBUG
  Serial.println("HTTP server started");
#endif

  _steer.attach(STEER_PIN, 500, 2500);
  _steer.write(90);

  _pcf8574.pinMode(HEAD_LIGHT_PIN_E, OUTPUT, HIGH);
  _pcf8574.pinMode(BREAK_LIGHT_PIN_E, OUTPUT, LOW);
  _pcf8574.pinMode(REVERSE_LIGHT_PIN_E, OUTPUT, HIGH);
  _pcf8574.pinMode(RIGHT_SIGNAL_PIN_E, OUTPUT, HIGH);
  _pcf8574.pinMode(LEFT_SIGNAL_PIN_E, OUTPUT, HIGH);
  _pcf8574.pinMode(MOTOR_PIN_1_E, OUTPUT, LOW);
  _pcf8574.pinMode(MOTOR_PIN_2_E, OUTPUT, LOW);
  _pcf8574.pinMode(HORN_PIN_E, OUTPUT, HIGH);

#ifdef DEBUG
  Serial.print("Init pcf8574...");
  if (_pcf8574.begin())
    Serial.println("OK");
  else
    Serial.println("KO");
#else
  _pcf8574.begin();
#endif
}

unsigned long previousMillis = 0;
PCF8574::DigitalInput prevValues;

// MARK:LOOP
void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= signalInterval)
  {
    previousMillis = currentMillis;
    prevValues = _pcf8574.digitalReadAll();

    switch (_signal)
    {
    case OFF:
      prevValues.p3 = HIGH;
      prevValues.p4 = HIGH;
      break;
    case BOTH:
      if (prevValues.p3)
      {
        prevValues.p3 = LOW;
        prevValues.p4 = LOW;
      }
      else
      {
        prevValues.p3 = HIGH;
        prevValues.p4 = HIGH;
      }
      break;
    case RIGHT:
      if (prevValues.p4)
      {
        prevValues.p3 = HIGH;
        prevValues.p4 = LOW;
      }
      else
      {
        prevValues.p3 = HIGH;
        prevValues.p4 = HIGH;
      }
      break;
    case LEFT:
      if (prevValues.p3)
      {
        prevValues.p3 = LOW;
        prevValues.p4 = HIGH;
      }
      else
      {
        prevValues.p3 = HIGH;
        prevValues.p4 = HIGH;
      }
      break;
    }

    _pcf8574.digitalWriteAll(prevValues);
  }

  yield();
#ifdef ESP8266
  MDNS.update();
#endif
}
