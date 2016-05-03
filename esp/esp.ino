String const VERZE = "0.1";

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <stdlib.h>
#include <cstdlib>

// Zadejte SSID bezdrátové sítě
const char* ssid = "";
// Zadejte heslo pro připojení k bezdrátové síti
const char* password = "";

ESP8266WebServer server(80);

const int GPIO2 = 2;

// hodnoty měření
int uroven_osvetleni;
int uroven_srazek;
int vlhkost_pudy;
float teplota_vzduchu;
int stariMereni;
int casMereni;

boolean aktualniHodnoty = false;
boolean vypnoutGPIO2 = false;
int casDispleje;
String RXString = "";           // a string to hold incoming data

void serialEvent() {
  while (Serial.available() > 0) {

    RXString = Serial.readStringUntil('\n');

    byte index = RXString.indexOf(',');
    uroven_osvetleni = (RXString.substring(0, index)).toInt();
    RXString = RXString.substring(index + 1, RXString.length());

    index = RXString.indexOf(',');
    uroven_srazek = (RXString.substring(0, index)).toInt();
    RXString = RXString.substring(index + 1, RXString.length());

    index = RXString.indexOf(',');
    vlhkost_pudy = (RXString.substring(0, index)).toInt();
    RXString = RXString.substring(index + 1, RXString.length());

    index = RXString.indexOf(',');
    teplota_vzduchu = std::atof((RXString.substring(0, index)).c_str());

    casMereni = millis();
    aktualniHodnoty = true;
  }
}


void handleRoot() {
  

  String rootPage = "<!DOCTYPE html><html lang='cs'><head><meta charset='utf-8' /><title>Záhon</title>";
  rootPage += "<meta name='viewport' content='width=device-width, initial-scale=1.0 /'>";
  rootPage += "<style>body{background-color: #006600; color: #FFF; margin: 20px; font-family: monospace;}</style></head><body>";
  rootPage += "<header><h1>Záhon</h1><em>Digitální monitorovací stanice</em></header><br />";
  if (aktualniHodnoty) {
    rootPage += "<p style='color: #00FF00; padding: 10px; background-color: #004400; border: 1px solid #FFF;'>DATA AKTUÁLNÍ (" + String(stariMereni) + "&nbsp;s)</p><br />";
  }
  else {
    rootPage += "<p style='color: #FFF; padding: 10px; background-color: #440000; border: 1px solid #FFF;'>DATA neaktuální (" + String(stariMereni) + "&nbsp;s)</p><br />";
  }
  rootPage += "<p>Úroveň osvětlení: <strong>" + String(uroven_osvetleni) + "</strong></p>";
  rootPage += "<p>Množství srážek: <strong>" + String(uroven_srazek) + "</strong></p>";
  rootPage += "<p>Vlhkost půdy: <strong>" + String(vlhkost_pudy) + "</strong></p>";
  rootPage += "<p>Teplota vzduchu: <strong>" + String(teplota_vzduchu) + "&thinsp;&#8451;</strong></p><br />";
  rootPage += "<hr /><p><em>verze: " + VERZE + "<br />&copy; 2016 Martin Tábor</em></p>";
  rootPage += "</body></html>";

  server.send(200, "text/html", rootPage);

  
}

void handleAPI() {
  

  String zprava = "{";
  zprava += "\"cas\":";
  zprava += stariMereni;
  zprava += ",\"osvetleni\":";
  zprava += uroven_osvetleni;
  zprava += ",\"srazky\":";
  zprava += uroven_srazek;
  zprava += ",\"puda\":";
  zprava += vlhkost_pudy;
  zprava += ",\"teplota\":";
  zprava += teplota_vzduchu;
  zprava += "}";

  server.send(200, "application/json", zprava);

  
}

void handleDisplej() {
  digitalWrite(GPIO2, HIGH);
  casDispleje = millis();
  vypnoutGPIO2 = true;

  server.send(200,"text/plain", "OK");
}

void handleNotFound() {
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  
}

void setup(void) {  
  pinMode(GPIO2, OUTPUT);
  
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  if (MDNS.begin("esp8266")) {
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/api", handleAPI);

  server.on("/displej", handleDisplej);

  server.onNotFound(handleNotFound);

  server.begin();
}

void loop(void) {

  stariMereni = (millis() - casMereni) / 1000;
  if (stariMereni > 60) {
    aktualniHodnoty = false;
  }
  if (millis() % 15000 == 0) {
    Serial.println(WiFi.localIP());
    delay(100);
  }
  if (vypnoutGPIO2) {
    if (millis() >= (casDispleje + 15000)) {
      digitalWrite(GPIO2, LOW);
      vypnoutGPIO2 = false;
    }
  }
  
  server.handleClient();
  serialEvent();

}
