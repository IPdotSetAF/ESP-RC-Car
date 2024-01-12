#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <LittleFS.h>

#include <uri/UriBraces.h>

const char *ssid = "ESP8266RC";
const char *password = "ESP8266RC";
// const char *ssid = ":|-";
// const char *password = "W0rkH4rdP1ayH4rDer";
const char *hostname = "ESP-8266-RC";
const char *mdns = "esp8266rc";

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

void listFiles()
{
  Serial.println("Listing files:");
  Dir dir = LittleFS.openDir("/");
  while (dir.next())
  {
    Serial.print("  FILE: ");
    Serial.println(dir.fileName());
  }
  Serial.println("End of file list");
}

void handleRoot()
{
  server.sendHeader("Location", "/index.html", true);
  server.send(302, "text/plain", "");
}

void handleStatic()
{
  String path = server.uri();
  Serial.println("requested : " + path);
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
    server.streamFile(file, contentType);
    file.close();
  }
  else
  {
    server.send(404, "text/plain", "File not found");
  }
}

void updateGas()
{
  String gas = server.pathArg(0);
  if (gas == "1" || gas == "true")
    Serial.println("gas on");
  else if (gas == "0" || gas == "false")
    Serial.println("gas off");
  else
    server.send(400, "text/plain", "bad request.");

  server.send(200, "text/plain", "ok");
}

void updateSignal()
{
  String signal = server.pathArg(0);
  if (signal == "left")
    Serial.println("signal left");
  else if (signal == "both")
    Serial.println("flasher");
  else if (signal == "right")
    Serial.println("signal right");
  else if (signal == "off")
    Serial.println("signal off");
  else
    server.send(400, "text/plain", "bad request.");

  server.send(200, "text/plain", "ok");
}

void updateHeadLight()
{
  String light = server.pathArg(0);
  if (light == "1" || light == "true")
    Serial.println("light on");
  else if (light == "0" || light == "false")
    Serial.println("light off");
  else
    server.send(400, "text/plain", "bad request.");

  server.send(200, "text/plain", "ok");
}

void updateHorn()
{
  Serial.println("beeeeep!");
  server.send(200, "text/plain", "ok");
}

void updateSteer()
{
  // try
  // {
  int angle = server.pathArg(0).toInt();
  Serial.println("steering to " + angle);
  server.send(200, "text/plain", "ok");
  // }
  // catch ()
  // {
  //   server.send(400, "text/plain", "bad request.");
  // }
}

void updateGear()
{
  String gear = server.pathArg(0);
  if (gear == "d")
    Serial.println("gear in drive");
  else if (gear == "n")
    Serial.println("gear in neutral");
  else if (gear == "r")
    Serial.println("gear in reverse");
  else
    server.send(400, "text/plain", "bad request.");

  server.send(200, "text/plain", "ok");
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
  Serial.begin(115200);
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
  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  if (MDNS.begin(mdns))
  {
    Serial.println("mDNS responder started");
  }

  LittleFS.begin();

  listFiles();

  configRoutes(&server);
  server.begin();
  httpUpdater.setup(&server);

  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  MDNS.update();
}
