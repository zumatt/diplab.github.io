/* MIT License 2023 Matteo Subet <hi@zumat.ch>
   
   Alpha Version 0.1 - DiPLaB
   Master Thesis Project from Matteo Subet
   SUPSI MAInD

   Based on Inkplate 6PLUS (ESP32)
*/

/*
    -----------------------------------------------------------
                            LIBRARIES
    -----------------------------------------------------------
*/
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

//The next 5 lines are needed to change the pins for SD Card reader
#define SCK  14
#define MISO  12
#define MOSI  13
#define CS  15
SPIClass spi = SPIClass(VSPI);

/*
    -----------------------------------------------------------
                            VARIABLES
    -----------------------------------------------------------
*/

//Here you can insert your network credentials
const char* ssid = "DiPLab";
const char* password = "AMRawareness23";

//Here you can choose the URI parameters used in the HTML page
const char* PARAM_INPUT_1 = "bacteria";
const char* PARAM_INPUT_2 = "ab";

//Create AsyncWebServer object on port 80
AsyncWebServer server(80);

/*
    -----------------------------------------------------------
                      PERSONALISED FUNCTIONS
    -----------------------------------------------------------
*/

//With this function is it possible to check if everything is ok with the SD Card
void initSDCard(){
  if(!SD.begin(CS,spi,80000000)){
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

//With this function it will start the WiFi
void initWiFi() {
  WiFi.begin();
  WiFi.mode(WIFI_AP);               //WIFI_AP is equal of Access Point mode
  WiFi.softAP(ssid, password);      //AccessPoint ssid and pss
  Serial.print("IP Address: ");     
  Serial.println(WiFi.softAPIP());  //Here we print the IP Address of the AccessPoint
}

/*
    -----------------------------------------------------------
                          SETUP FUNCTION
    -----------------------------------------------------------
*/

void setup() {
  //Serial communication open to test
  Serial.begin(115200);
  //The next line is needed to change the SD Card reader with pins for Inkplate 6PLUS
  spi.begin(SCK, MISO, MOSI, CS);

  Serial.println("WiFi init:");
  initWiFi();
  Serial.println("WiFi init END");
  Serial.println("SD Card init:");
  initSDCard();
  Serial.println("SD Card init END");

  //Here we set what it need to happen after a client open the ESP32 IP address
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/index.html", "text/html");
  });
  //Load favicon, css and other files on the SD card
  server.serveStatic("/", SD, "/");

  //Do something when is received something from the query after "string"
  server.on("/string", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/string?bacteria=<inputMessage1>&antibiotic=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
    }
    else {
      inputMessage1 = "No message sent";
    }
    // GET input2 value on <ESP_IP>/string?bacteria=<inputMessage1>&antibiotic=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
    }
    else {
      inputMessage2 = "No message sent";
    }
    //Print on console the input messages
    Serial.print("Bacteria: ");
    Serial.print(inputMessage1);
    Serial.print(" - Antibiotic: ");
    Serial.println(inputMessage2);
    //Here you send the action after receiving the input
    request->send(SD, "/index.html", "text/html");
  });

  //Start the server
  server.begin();
}

/*
    -----------------------------------------------------------
                          LOOP FUNCTION
    -----------------------------------------------------------
*/

void loop() {
  
}