/* MIT License 2022 Matteo Subet <hi@zumat.ch>
   
   First example of a Web Server uploading files from a SD Card

   Based on a Rui Santos' code
   Reference at https://RandomNerdTutorials.com/esp32-web-server-microsd-card/
*/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Replace with your network credentials
const char* ssid = "DipPLab";
const char* password = "password";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void initSDCard(){
  if(!SD.begin()){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void initWiFi() {
  WiFi.begin();
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, password);
  Serial.print("Connecting to WiFi ..");
  /*while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }*/
  Serial.println(WiFi.softAPIP());
}

void setup() {
  Serial.begin(115200);
  Serial.println("WiFi init:");
  initWiFi();
  Serial.println("WiFi init END");
  Serial.println("SD Card init:");
  initSDCard();
  Serial.println("SD Card init END");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/index.html", "text/html");
  });

  server.serveStatic("/", SD, "/");

  server.begin();
}

void loop() {
  
}