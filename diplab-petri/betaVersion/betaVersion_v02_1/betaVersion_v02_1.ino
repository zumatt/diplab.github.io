/* MIT License 2023 Matteo Subet <hi@zumat.ch>
   
   Beta Version 0.2 - DiPLaB
   Spearhead Research Project - https://spearhead-amr.github.io/makeaware/
   
        Coded by Matteo Subet - Last version of 23 August 2023
   
   SUPSI - University of Applied Sciences and Arts of Southern Switzerland
   DACD - Department of Architecture, Construction and Design
   IDe - Design Institute

   Based on Inkplate 6PLUS (ESP32).
   You can find the complete repository here: https://github.com/zumatt/diplab
*/

/*
    -----------------------------------------------------------
                            BOARD CHECK
    -----------------------------------------------------------
*/
#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6PLUS in the boards menu."
#endif

/*
    -----------------------------------------------------------
                            LIBRARIES
    -----------------------------------------------------------
*/
#include "Inkplate-mod.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebSrv.h"
#include "SPIFFS.h"
#include "LIS3DHTR.h"
#include <Wire.h>
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "Fonts/FreeSansBold9pt7b.h"

/*
    -----------------------------------------------------------
                            VARIABLES
    -----------------------------------------------------------
*/

//Buttons setup
#define fwdBtn_PIN 36  // GPIO36(S-VP) or GPIO39(S-VN) pin connected to button
#define bckBtn_PIN 39
int lastState_fwd = HIGH;     // the previous state from the fwd button pin
int currentState_fwd;         // the current reading from the fwd input pin
int lastState_bck = HIGH;     // the previous state from the bck button pin
int currentState_bck;         // the current reading from the bck input pin
int counter = 0;              // the counter indicating the state of the button bck (--) / fwd (++)
String modality;              //ControlCenter modality

//Here you can insert your network credentials
const char* ssid = "DiPLab";
const char* password = "AMRawareness23";

//String where to save the message from the webPage
int    j_state;          //state
String j_bacteria;       //bacteria
String j_ab1;            //ab1
String j_ab2;            //ab2
String j_ab3;            //ab3
String j_name;           //name
int    j_classcode;      //classcode
int    j_readingAB;   //readingAB
int    j_test;           //test (0, 1 or 2)
String j_controlCenter;  //controlCenter

//Save message for the webPage
int historyHours;
String readingAbVal;

//Controller for screen ready to spread bacteria
bool readyToSpread;

//Variables to display stuff on the E-Ink display
int dispW = 1024;                                           //Display Width
int dispH = 758;                                            //Display Height
int petriD = (dispH/2)-10;                                  //Petri dish radius
int xC =  (dispW/2)-(petriD);                               //X position of left side of petri dish
int yC =  (dispH/2)-(petriD);                               //Y position of top side of petri dish
int xC2 = (dispW/2)+(petriD);                               //X position of right side of petri dish
int yC2 = (dispH/2)+(petriD);                               //Y position of bottom side of petri dish
int yPos = dispH/2;                                         //Center y of petri dish
int xPos = dispW/2;                                         //Center x of petri dish
int step = petriD/10;                                       //Step of each line of bacteria
float bactRect = step/2;                                    //General dimension of the rectangle created to spread bacteria
int arrY[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //Array of Y axis for checking if a line was drawn
int lenArrY = *(&arrY + 1) - arrY;                          //Array of Y axis length
bool arrYcompleted;
int arrX[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //Array of X axis for checking if a line was drawn
int lenArrX = *(&arrX + 1) - arrX;                          //Array of X axis length
bool arrXcompleted;
bool arrayCompletedSpreading;
double accX, accY;                                          //Variables to store accellerometer data
int ab1_x, ab1_y;                                           //Position of first antibiotic
int ab2_x, ab2_y;                                           //Position of second antibiotic
int ab3_x, ab3_y;                                           //Position of third antibiotic
int abDiameter = 35;                                        //Dimension of the AB circle
double ab_resistance_multiplier = 0.25;                     //Multiplier from pixels to mm for ab resistance
double ab1_resistance, ab2_resistance, ab3_resistance;      //Store resistance value for each AB (diamenter multiplier!) expressed in mm as reported in the antibiotic sheet
const GFXfont *textFont = &FreeSansBold9pt7b;               //Font setup for FreeSansBold9pt7b
int antibioticCreated = 0;                                  //Numbers of antibiotic placed
int differenceCoordX1 = 0;                                  //Variable to store the difference between the current variable and the old one for X position of AB 1
int differenceCoordY1 = 0;                                  //Variable to store the difference between the current variable and the old one for Y position of AB 1
int differenceCoordX2 = 0;                                  //Variable to store the difference between the current variable and the old one for X position of AB 2
int differenceCoordY2 = 0;                                  //Variable to store the difference between the current variable and the old one for Y position of AB 2
int tresholdAntibiotic = 150;                               //Threshold between two anbiotics pills
float bacteriaDotsX[500];                                   //Array that contains all the X values of the dots created for the bacteria growth
float bacteriaDotsY[500];                                   //Array that contains all the Y values of the dots created for the bacteria growth
int ab1_xWeb, ab1_yWeb;                                     //Position of first antibiotic
int ab2_xWeb, ab2_yWeb;                                     //Position of second antibiotic
int ab3_xWeb, ab3_yWeb;                                     //Position of third antibiotic
double ab1_resistanceWeb, ab2_resistanceWeb, ab3_resistanceWeb;      //Store resistance value for each AB (diamenter multiplier!) expressed in mm as reported in the antibiotic sheet

//Create a string where to save the Access Point IP address
IPAddress serverIP;

int interval = 1000;                                  // send data to the client every 1000ms -> 1s
unsigned long previousMillis = 0;                     // we use the "millis()" command for time reference and this will output an unsigned long

/*
    -----------------------------------------------------------
                    FUNCTIONS TO SETUP THE BOARD
    -----------------------------------------------------------
*/

//Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//Create webSocket on port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Create an object on Inkplate library and also set library into 1 Bit mode (BW)
Inkplate display(INKPLATE_1BIT);

/*
    -----------------------------------------------------------
                      PERSONALISED FUNCTIONS
    -----------------------------------------------------------
*/

//With this function is it possible to check if everything is ok with the SD Card
void initSPIFFS(){
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

//With this function it will start the WiFi
void initWiFi() {
  WiFi.begin();
  WiFi.mode(WIFI_AP);               //WIFI_AP is equal of Access Point mode
  WiFi.softAP(ssid, password);      //AccessPoint ssid and pss
  Serial.print("IP Address: ");
  serverIP = WiFi.softAPIP();    
  Serial.println(serverIP);  //Here we print the IP Address of the AccessPoint
}

/*
    -----------------------------------------------------------
                          SETUP FUNCTION
    -----------------------------------------------------------
*/

void setup() {
  //Serial communication open to test
  Serial.begin(115200);

  //Setup the two buttons' pins
  pinMode(fwdBtn_PIN, INPUT_PULLUP);
  pinMode(bckBtn_PIN, INPUT_PULLUP);
  
  //Begin and clear display
  display.begin();
  display.clearDisplay();
  display.display();

  //Setup accellerometer
  LIS.begin(WIRE,0x19); //IIC init
  delay(100);
  LIS.setFullScaleRange(LIS3DHTR_RANGE_4G);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);

  //Display settings
  display.setFont(textFont);
  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE);
  display.setTextWrap(true);

  Serial.println("WiFi init:");
  initWiFi();
  Serial.println("WiFi init END");
  Serial.println("SPIFFS init START");
  initSPIFFS();
  Serial.println("SPIFFS init END");

  //Here we set what it need to happen after a client open the ESP32 IP address
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  //Load favicon, css and other files on the SD card
  server.serveStatic("/", SPIFFS, "/");

  //Start the server & the webSocket
  server.begin();
  webSocket.begin();

  //Call a function when receiving Event from the webpage
  webSocket.onEvent(webSocketEvent);

  // Init touchscreen and power it on after init (send false as argument to put it in deep sleep right after init)
  if (display.tsInit(true)){
      Serial.println("Touchscreen init ok");}
  else {
      Serial.println("Touchscreen init fail"); while (true);}

  //Antibiotic resistance calculation (for now is just a test)
    //ab1_resistance = 2;
    //ab2_resistance = 3.5;
    //ab3_resistance = 8;

  //Start with the experience
  state0();

  //Create all the dots for the display of bacteria growing
  for(int i=0; i<500; i++){
    bacteriaDotsX[i] = random(xC, xC2);
    bacteriaDotsY[i] = random(yC, yC2);
    //Serial.print("Bacteria dot created. Coordinates -> X: ");
    //Serial.print(bacteriaDotsX[i]);
    //Serial.print("    -    Y: ");
    //Serial.println(bacteriaDotsY[i]);
  }
  Serial.println("Dots created!");
}

/*
    -----------------------------------------------------------
                          LOOP FUNCTION
    -----------------------------------------------------------
*/

void loop() {
  webSocket.loop();

  //Check if we need to activate the digitalRead for buttons
  if(j_state == 11){
  //Serial.println("We are in the loop!");

  /* ERASE COMMENT from here TO ENABLE PHYSICAL BUTTONS
  //Update variables for Buttons
  currentState_fwd = digitalRead(fwdBtn_PIN);
  currentState_bck = digitalRead(bckBtn_PIN);

  //Call function to check buttons condition
  buttonsCondition();
  ERASE COMMENT to here TO ENABLE PHYSICAL BUTTONS */ 

  // Digital buttons using touchscreen (COMMENT THE FOLLOWING LINE IF YOU WANT TO USE THE PHYSICAL ONES)
  buttonTouchScreen();
  } //END IF digitalRead Buttons

  //Check if we need to get data from accellerometer
  if(j_state == 8){
    //Call drawing function for bacteria
    if(j_state == 8 && readyToSpread == 1){
      //Update variables for accellerometer
      accX = LIS.getAccelerationX(); //Get accellerometer X data
      accY = -LIS.getAccelerationY(); //Get accellerometer Y data

      drawingLoop();

      if (arrayCompletedSpreading == true){
        String jsonString = "";
        StaticJsonDocument<200> doc;                      // create a JSON container
        JsonObject object = doc.to<JsonObject>();         // create a JSON Object
        object["spreadingBacteria"] = 1;                  // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
        serializeJson(doc, jsonString);                   // convert JSON object to string
        Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
        webSocket.broadcastTXT(jsonString);               // send JSON string to clients
      } else {
        checkLoopSpreading();
      }
    } //END IF data from accellerometer
  }  
  
  //Call touchscreen detection for placing ABs
  if (j_state == 10){
    abPlacing();
    //Serial.println("Touchscreen activated!");
  }

  //Wait
  unsigned long now = millis();

  //Call control center check for data from smartphone
  if (j_state == 11){
    if ((unsigned long)(now - previousMillis) > interval) {
      Serial.println("Checking for web data...");

      String jsonString = "";
      StaticJsonDocument<200> doc;                      // create a JSON container
      JsonObject object = doc.to<JsonObject>();         // create a JSON Object
      object["historyVal"] = historyHours;              // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
      object["readingVal"] = readingAbVal;
      object["ab1Val"] = j_ab1;
      object["ab2Val"] = j_ab2;
      object["ab3Val"] = j_ab3;
      serializeJson(doc, jsonString);                   // convert JSON object to string
      Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
      webSocket.broadcastTXT(jsonString);               // send JSON string to clients

      previousMillis = now;
    }
  }
  if (j_state == 12){
    if ((unsigned long)(now - previousMillis) > interval) {
      //Create a string containing all the info related to ab position and resistance
      String ab1_values, ab2_values, ab3_values, names;
      String temp_ab1xWeb, temp_ab1yWeb, temp_ab1resistanceWeb, temp_ab2xWeb, temp_ab2yWeb, temp_ab2resistanceWeb, temp_ab3xWeb, temp_ab3yWeb, temp_ab3resistanceWeb, temp_nameBact, temp_nameAb1, temp_nameAb2, temp_nameAb3;

      temp_ab1xWeb = ab1_xWeb;
      temp_ab1yWeb = ab1_yWeb;
      temp_ab1resistanceWeb = ab1_resistanceWeb;
      temp_ab2xWeb = ab2_xWeb;
      temp_ab2yWeb = ab2_yWeb;
      temp_ab2resistanceWeb = ab2_resistanceWeb;
      temp_ab3xWeb = ab3_xWeb;
      temp_ab3yWeb = ab3_yWeb;
      temp_ab3resistanceWeb = ab3_resistanceWeb;
      temp_nameBact = j_bacteria;
      temp_nameAb1 = j_ab1;
      temp_nameAb2 = j_ab2;
      temp_nameAb3 = j_ab3;
      

      ab1_values = temp_ab1xWeb + "," + temp_ab1yWeb + "," + temp_ab1resistanceWeb;
      ab2_values = temp_ab2xWeb + "," + temp_ab2yWeb + "," + temp_ab2resistanceWeb;
      ab3_values = temp_ab3xWeb + "," + temp_ab3yWeb + "," + temp_ab3resistanceWeb;
      names = temp_nameBact + "," + temp_nameAb1 + "," + temp_nameAb2 + "," + temp_nameAb3;
    
      Serial.println("Numeric info continuosly sent to the app:");
      
      String jsonString = "";
      StaticJsonDocument<200> doc;                      // create a JSON container
      JsonObject object = doc.to<JsonObject>();         // create a JSON Object

      object["ab1_values"] = ab1_values;                // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
      object["ab2_values"] = ab2_values;
      object["ab3_values"] = ab3_values;
      object["names"] = names;

      serializeJson(doc, jsonString);                   // convert JSON object to string
      Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
      webSocket.broadcastTXT(jsonString);               // send JSON string to clients

      previousMillis = now;
  }
  }
}