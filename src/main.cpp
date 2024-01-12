#include "main.h"

void listFiles()
{
#ifdef DEBUG
  Serial.println("Listing files:");
  Dir dir = LittleFS.openDir("/");
  while (dir.next())
  {
    Serial.print("  FILE: ");
    Serial.println(dir.fileName());
  }
  Serial.println("End of file list");
#endif
}

void handleRoot()
{
  _server.sendHeader("Location", "/index.html", true);
  _server.send(302, "text/plain", "");
}

void handleStatic()
{
  String path = _server.uri();
#ifdef DEBUG
  Serial.println("requested : " + path);
#endif

  if (path != "/" && LittleFS.exists(path))
  {
    String contentType = "text/plain";
    if (path.endsWith(".html"))
      contentType = "text/html";
    else if (path.endsWith(".css"))
      contentType = "text/css";
    else if (path.endsWith(".js"))
      contentType = "application/javascript";
    else if (path.endsWith(".json"))
      contentType = "application/json";
    else if (path.endsWith(".svg"))
      contentType = "image/svg+xml";

    File file = LittleFS.open(path, "r");
    _server.streamFile(file, contentType);
    file.close();
  }
  else
  {
    _server.send(404, "text/plain", "File not found");
  }
}

void updateGas()
{
  String gas = _server.pathArg(0);
  if (gas == "1" || gas == "true")
  {
    switch (_gear)
    {
    case DRIVE:
      _pcf8574.digitalWrite(MOTOR_PIN_1_E, LOW);
      _pcf8574.digitalWrite(MOTOR_PIN_2_E, HIGH);
      _pcf8574.digitalWrite(BREAK_LIGHT_PIN_E, LOW);
      break;
    case REVERSE:
      _pcf8574.digitalWrite(MOTOR_PIN_1_E, HIGH);
      _pcf8574.digitalWrite(MOTOR_PIN_2_E, LOW);
      _pcf8574.digitalWrite(BREAK_LIGHT_PIN_E, LOW);
      break;
    case NEURTAL:
      break;
    }
  }
  else if (gas == "0" || gas == "false")
  {
    _pcf8574.digitalWrite(MOTOR_PIN_1_E, LOW);
    _pcf8574.digitalWrite(MOTOR_PIN_2_E, LOW);
    _pcf8574.digitalWrite(BREAK_LIGHT_PIN_E, HIGH);
  }
  else
    _server.send(400, "text/plain", "bad request.");

  _server.send(200, "text/plain", "ok");
}

void updateSignal()
{
  String signal = _server.pathArg(0);
  if (signal == "left")
    _signal = LEFT;
  else if (signal == "both")
    _signal = BOTH;
  else if (signal == "right")
    _signal = RIGHT;
  else if (signal == "off")
    _signal = OFF;
  else
    _server.send(400, "text/plain", "bad request.");

  _server.send(200, "text/plain", "ok");
}

void updateHeadLight()
{
  String light = _server.pathArg(0);
  if (light == "1" || light == "true")
    _pcf8574.digitalWrite(HEAD_LIGHT_PIN_E, HIGH);
  else if (light == "0" || light == "false")
    _pcf8574.digitalWrite(HEAD_LIGHT_PIN_E, LOW);
  else
    _server.send(400, "text/plain", "bad request.");

  _server.send(200, "text/plain", "ok");
}

void updateHorn()
{
#ifdef DEBUG
  Serial.println("beeeeep!");
#endif
  _server.send(200, "text/plain", "ok");
}

void updateSteer()
{
  int angle = _server.pathArg(0).toInt();
  int value = map(angle, -90, 90, 0, 180);
  _steer.write(value);

#ifdef DEBUG
  Serial.println((String)value + " : steering to : " + (String)angle);
#endif

  _server.send(200, "text/plain", "ok");
  //   server.send(400, "text/plain", "bad request.");
}

void updateGear()
{
  String gear = _server.pathArg(0);
  if (gear == "d" || gear == "D")
  {
    _gear = DRIVE;
    _pcf8574.digitalWrite(REVERSE_LIGHT_PIN_E, LOW);
  }
  else if (gear == "n" || gear == "N")
  {
    _gear = NEURTAL;
    _pcf8574.digitalWrite(REVERSE_LIGHT_PIN_E, LOW);
  }
  else if (gear == "r" || gear == "R")
  {
    _gear = REVERSE;
    _pcf8574.digitalWrite(REVERSE_LIGHT_PIN_E, HIGH);
  }
  else
    _server.send(400, "text/plain", "bad request.");

  _server.send(200, "text/plain", "ok");
}

void configRoutes(ESP8266WebServer *server)
{
  server->on(UriBraces("/api/gas/{}"), HTTP_PUT, updateGas);
  server->on(UriBraces("/api/signal/{}"), HTTP_PUT, updateSignal);
  server->on(UriBraces("/api/headLight/{}"), HTTP_PUT, updateHeadLight);
  server->on("/api/horn", HTTP_PUT, updateHorn);
  server->on(UriBraces("/api/steer/{}"), HTTP_PUT, updateSteer);
  server->on(UriBraces("/api/gear/{}"), HTTP_PUT, updateGear);
  server->on("/", HTTP_GET, handleRoot);
  server->onNotFound(handleStatic);
}

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif
  WiFi.hostname(hostname);

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
#ifdef DEBUG
  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
#endif

#ifdef DEBUG
  if (MDNS.begin(mdns))
    Serial.println("mDNS responder started");
#else
  MDNS.begin(mdns);
#endif

  LittleFS.begin();

  listFiles();

  configRoutes(&_server);
  _server.begin();
  _httpUpdater.setup(&_server);

#ifdef DEBUG
  Serial.println("HTTP server started");
#endif

  _steer.attach(STEER_PIN, 500, 2500);
  _steer.write(0);

  _pcf8574.pinMode(HEAD_LIGHT_PIN_E, OUTPUT, LOW);
  _pcf8574.pinMode(BREAK_LIGHT_PIN_E, OUTPUT, LOW);
  _pcf8574.pinMode(REVERSE_LIGHT_PIN_E, OUTPUT, LOW);
  _pcf8574.pinMode(RIGHT_SIGNAL_PIN_E, OUTPUT, LOW);
  _pcf8574.pinMode(LEFT_SIGNAL_PIN_E, OUTPUT, LOW);
  _pcf8574.pinMode(MOTOR_PIN_1_E, OUTPUT, LOW);
  _pcf8574.pinMode(MOTOR_PIN_2_E, OUTPUT, LOW);

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

void loop()
{
  _server.handleClient();
  MDNS.update();
}
