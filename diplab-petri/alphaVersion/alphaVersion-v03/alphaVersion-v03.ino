/* MIT License 2023 Matteo Subet <hi@zumat.ch>
   
   Alpha Version 0.3 - DiPLaB
   Master Thesis Project from Matteo Subet
   SUPSI MAInD

   Based on Inkplate 6PLUS (ESP32)

   Notes for the beta:
    - Create a block system to block the user in the spreading bacteria if they didn't finished the spreading process.
    - Find another solution for the ab communication is hard to send three different querys from the web
      ^- for that is quite easy: send one single query and then split it...
      ^- or do it in three different steps (3 different webPages first for AB1 etc.)
*/

/*
    -----------------------------------------------------------
                            LIBRARIES
    -----------------------------------------------------------
*/
#include "Inkplate-mod.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "LIS3DHTR.h"
#include <Wire.h>
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

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

//Here you can choose the URI parameters used in the HTML page
const char* PARAM_INPUT_1 = "state";
const char* PARAM_INPUT_2 = "bacteria";
const char* PARAM_INPUT_3 = "ab1";
const char* PARAM_INPUT_4 = "ab2";
const char* PARAM_INPUT_5 = "ab3";
const char* PARAM_INPUT_6 = "name";
const char* PARAM_INPUT_7 = "classcode";
const char* PARAM_INPUT_8 = "microscopeAb";
const char* PARAM_INPUT_9 = "test";
const char* PARAM_INPUT_10 = "controlCenter";

//String where to save the message from the webPage
String inputMessage1;  //state
String inputMessage2;  //bacteria
String inputMessage3;  //ab1
String inputMessage4;  //ab2
String inputMessage5;  //ab3
String inputMessage6;  //name
String inputMessage7;  //classcode
String inputMessage8;  //microscopeAB
String inputMessage9;  //test (0, 1 or 2)
String inputMessage10; //controlCenter

//Controller for screen ready to spread bacteria
bool readyToSpread;

//Variables to display stuff on the E-Ink display
int dispW = 1024;                                           //Display Width
int dispH = 758;                                            //Display Height
int petriD = (dispH/2)-10;                                  //Petri dish diameter
int xC =  (dispW/2)-(petriD);                               //X position of left side of petri dish
int yC =  (dispH/2)-(petriD);                               //Y position of top side of petri dish
int xC2 = (dispW/2)+(petriD);                               //X position of right side of petri dish
int yC2 = (dispH/2)+(petriD);                               //Y position of bottom side of petri dish
int yPos = dispH/2;                                         //Center y of petri dish
int xPos = dispW/2;                                         //Center x of petri dish
int step = petriD/10;                                       //Step of each line of bacteria
int arrY[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //Array of Y axis for checking if a line was drawn
int lenArrY = *(&arrY + 1) - arrY;                          //Array of Y axis length
int arrX[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //Array of X axis for checking if a line was drawn
int lenArrX = *(&arrX + 1) - arrX;                          //Array of X axis length
double accX, accY;                                          //Variables to store accellerometer data
int ab1_x, ab1_y;                                           //Position of first antibiotic
int ab2_x, ab2_y;                                           //Position of second antibiotic
int ab3_x, ab3_y;                                           //Position of third antibiotic
int abDiameter = 25;                                        //Dimension of the AB circle
double ab1_resistance, ab2_resistance, ab3_resistance;      //Store resistance value for each AB (diamenter multiplier!)
double ab1_res_8h  = ab1_resistance / 3;                    //Store resistance value for AB 1 in history mode
double ab1_res_12h = ab1_resistance / 2;
double ab1_res_24h = ab1_resistance;
double ab2_res_8h  = ab2_resistance / 3;                    //Store resistance value for AB 2 in history mode
double ab2_res_12h = ab2_resistance / 2;
double ab2_res_24h = ab2_resistance;
double ab3_res_8h  = ab3_resistance / 3;                    //Store resistance value for AB 3 in history mode
double ab3_res_12h = ab3_resistance / 2;
double ab3_res_24h = ab3_resistance;

//Check how many antibiotics are selected
int arrAb[3] = {0,0,0};

//Create a string where to save the Access Point IP address
IPAddress serverIP;

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
  display.setTextSize(3);
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
    request->send(SPIFFS, "/state0.html", "text/html");
  });
  //Load favicon, css and other files on the SD card
  server.serveStatic("/", SPIFFS, "/");

  //Do something when is received something from the query after "string"
  serverReceive();

  //Start the server & the webSocket
  server.begin();
  webSocket.begin();

  //Call a function when receiving Event from the webpage
  webSocket.onEvent(webSocketEvent);

  //Start with the experience
  state0();
}

/*
    -----------------------------------------------------------
                          LOOP FUNCTION
    -----------------------------------------------------------
*/

void loop() {
  webSocket.loop();

  //Check if we need to activate the digitalRead for buttons
  if(inputMessage1 == "11"){
  //Serial.println("We are in the loop!");

  //Update variables for Buttons
  currentState_fwd = digitalRead(fwdBtn_PIN);
  currentState_bck = digitalRead(bckBtn_PIN);

  //Call function to check buttons condition
  buttonsCondition();
  } //END IF digitalRead Buttons

  //Check if we need to get data from accellerometer
  if(inputMessage1 == "8" || inputMessage1 == "10"){

    //Update variables for accellerometer
    accX = LIS.getAccelerationX(); //Get accellerometer X data
    accY = LIS.getAccelerationY(); //Get accellerometer Y data

    //Call drawing function for bacteria
    if(inputMessage1 == "8" && readyToSpread == 1){
      drawingLoop();
    }
  
    //Call shake detection for placing ABs
    if (inputMessage1 == "10"){
      abShake();
      //Serial.println("Ab shake activated!");
    }
  } //END IF data from accellerometer
}