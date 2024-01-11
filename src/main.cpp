#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <LittleFS.h>

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

void handleRoot() {
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

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleStatic);
  server.begin();
  httpUpdater.setup(&server);

  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  MDNS.update();
}
