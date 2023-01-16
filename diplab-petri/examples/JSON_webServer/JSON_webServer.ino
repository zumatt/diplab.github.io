#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>                              // needed for JSON encapsulation (send multiple variables with one string)
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

const char* ssid = "DiPLab Test";
const char* password = NULL;

String arrText[5] = {"Pippo", "Pluto", "Paperino", "Topolino", "Topolina"};
String character = arrText[0];
int counter;
String JSONtxt;

int interval = 1000;                                  // send data to the client every 1000ms -> 1s
unsigned long previousMillis = 0;                     // we use the "millis()" command for time reference and this will output an unsigned long

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

//With this function is it possible to check if everything is ok with the SD Card
void initSPIFFS(){
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void setup() { 
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin();
  WiFi.mode(WIFI_AP);               //WIFI_AP is equal of Access Point mode
  WiFi.softAP(ssid, password);      //AccessPoint ssid and pss

  // Print ESP32 Local IP Address
  Serial.println(WiFi.softAPIP());

  Serial.println("SPIFFS init START");
  initSPIFFS();
  Serial.println("SPIFFS init END");

  // Initialize webserver URLs
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html","text/html");
  });

  // Start webserver & webSocket
  server.begin();
  webSocket.begin();

  webSocket.onEvent(webSocketEvent);  // when receiving Event from the webpage
}

void loop() {
  webSocket.loop();

  unsigned long now = millis();                           // read out the current "time" ("millis()" gives the time in ms since the Arduino started)
  if ((unsigned long)(now - previousMillis) > interval) { // check if "interval" ms has passed since last time the clients were updated

    String jsonString = "";                           // create a JSON string for sending data to the client
    StaticJsonDocument<200> doc;                      // create a JSON container
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    object["rand1"] = random(100);                    // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
    object["rand2"] = random(100);
    serializeJson(doc, jsonString);                   // convert JSON object to string
    Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
    webSocket.broadcastTXT(jsonString);               // send JSON string to clients
    
    previousMillis = now;                             // reset previousMillis
  }

  //Serial.println("The current character is: " + character);
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) { // event structure

  switch (type) {
    case WStype_DISCONNECTED: // client disconnected
      Serial.println("Client disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("Client connected");
      break;
    case WStype_TEXT:
      // try to decipher the JSON string received
      StaticJsonDocument<200> doc;                    // create a JSON container
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      else {
        // JSON string was received correctly, so information can be retrieved:
        const char* g_brand = doc["brand"];
        const char* g_type = doc["type"];
        const int g_year = doc["year"];
        const char* g_color = doc["color"];
        Serial.println("Received guitar info from user");
        Serial.println("Brand: " + String(g_brand));
        Serial.println("Type: " + String(g_type));
        Serial.println("Year: " + String(g_year));
        Serial.println("Color: " + String(g_color));
      }
      Serial.println("");
      break;
  }
  Serial.println(" ");

}